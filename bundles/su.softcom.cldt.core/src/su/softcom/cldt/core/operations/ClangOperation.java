package su.softcom.cldt.core.operations;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URI;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.OperationCanceledException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.SubMonitor;

import su.softcom.cldt.core.builders.CMakeBuilderArgs;
import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Clang build operation for C/C++ applications. */
public class ClangOperation implements ICoreRunnable {

	/** Clang additional command line arguments key. */
	public static final String CLANG_ARGS = "CLANG_ARGS"; //$NON-NLS-1$

	/** Clang build folder key. */
	public static final String CLANG_BUILD_FOLDER = "CLANG_BUILD_FOLDER"; //$NON-NLS-1$

	/** Clang target, sets required compile options. */
	public static final String CLANG_TARGET = CMakeBuilderArgs.TARGET_ARCH;

	/** Clang mipsel target key. */
	public static final String CLANG_MIPSEL = CMakeBuilderArgs.ARCH_MIPS;

	/** Key to override Clang build resource by {@link java.net.URI}. */
	public static final String CLANG_RESOURCE_URI = "CLANG_RESOURCE_URI"; //$NON-NLS-1$

	/** Key to include Clang build folder sources, default {@code false} */
	public static final String CLANG_INCLUDE_BUILD_SOURCES = "CLANG_INCLUDE_BUILD_SOURCES"; //$NON-NLS-1$

	/** Key to override default Clang output executable. */
	public static final String CLANG_OUTPUT = "CLANG_OUTPUT"; //$NON-NLS-1$

	public static final String DEFAULT_OUTPUT = CInitializer.IS_WINDOWS ? "out.exe" : "out"; //$NON-NLS-1$ //$NON-NLS-2$
	public static final String DEFAULT_BUILD_FOLDER = "build"; //$NON-NLS-1$

	/** Marker type to generate. */
	public static final String MARKER_TYPE = "marker-type"; //$NON-NLS-1$

	/** Command line argument separation pattern. */
	protected static final Pattern ARGS_PATTERN = Pattern
			.compile("(([^'\"\\s\\|&>]+|'([^']|\\\\')*'|\"([^\"]|\\\")*\")+|[\\|&>]{1,2}+)"); //$NON-NLS-1$

	protected final Map<String, String> args;
	protected final boolean includeBuildSources;
	protected String output;

	/** Marker type to generate ({@link IMarker}. */
	private String markerType;

	protected IResource buildResource;
	protected URI buildFolderURI;
	protected boolean isCpp;

	/**
	 * Create Clang build operation.
	 * 
	 * @param project the project for which this operation is defined
	 * @param args    a table of builder-specific arguments keyed by argument
	 *                name(key type: {@code String} value type: {@code String});
	 *                {@code null} is equivalent to an empty map
	 */
	public ClangOperation(IProject project, Map<String, String> args) {
		this.args = args != null ? args : new HashMap<String, String>();

		try {
			URI resourceURL = new URI(this.args.get(CLANG_RESOURCE_URI));
			IWorkspaceRoot root = ResourcesPlugin.getWorkspace().getRoot();
			this.buildResource = Stream
					.concat(Arrays.stream(root.findContainersForLocationURI(resourceURL)),
							Arrays.stream(root.findFilesForLocationURI(resourceURL)))
					.filter(IResource::exists).findFirst().orElse(project);

		} catch (Exception e) {
			buildResource = project;
		}

		this.output = this.args.getOrDefault(CLANG_OUTPUT, DEFAULT_OUTPUT);

		if (this.args.get(MARKER_TYPE) == null) {
			this.markerType = IMarker.PROBLEM;
		} else {
			this.markerType = this.args.get(MARKER_TYPE);
		}

		this.includeBuildSources = this.args.containsKey(CLANG_INCLUDE_BUILD_SOURCES)
				|| !(this.buildResource instanceof IProject);
		this.buildFolderURI = project.getFolder(this.args.getOrDefault(CLANG_BUILD_FOLDER, DEFAULT_BUILD_FOLDER))
				.getLocationURI();
		this.isCpp = false;
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		final int TOTAL_WORK = 10;
		SubMonitor subMonitor = SubMonitor.convert(monitor,
				MessageFormat.format("Building a {0} project", buildResource.getName()), TOTAL_WORK);

		try {
			List<String> toRun = new ArrayList<String>();

			subMonitor.subTask("Get compiler path");
			toRun.add(getCompilerPath());
			checkCancel(subMonitor);
			subMonitor.worked(1);

			subMonitor.subTask("Collect compile resources");
			toRun.addAll(resourcesToArgs(getCompileResources(buildResource)));
			checkCancel(subMonitor);
			subMonitor.worked(1);

			subMonitor.subTask("Pre build operation");
			toRun.addAll(getMipselArgs());
			Matcher matcher = ARGS_PATTERN.matcher(args.getOrDefault(CLANG_ARGS, "")); //$NON-NLS-1$
			while (matcher.find()) {
				toRun.add(matcher.group(1));
			}
			toRun.add("-o"); //$NON-NLS-1$
			toRun.add(output);
			checkCancel(subMonitor);
			subMonitor.worked(1);

			subMonitor.subTask("Build");
			Platform.getLog(getClass()).info(toRun.stream().collect(Collectors.joining(" "))); //$NON-NLS-1$
			Process process = new ProcessBuilder(toRun).directory(new File(buildFolderURI.getPath()))
					.redirectErrorStream(true).start();
			trackProcess(process, subMonitor.split(7));

		} catch (IOException e) {
			throw new CoreException(new Status(IStatus.ERROR, getClass(), e.getMessage()));
		} finally {
			subMonitor.done();
		}
	}

	/**
	 * Function to check for cancellation as often as possible.
	 * 
	 * @param monitor a progress monitor, or {@code null} if progress reporting and
	 *                cancellation are not desired
	 */
	protected void checkCancel(IProgressMonitor monitor) {
		if (monitor.isCanceled()) {
			throw new OperationCanceledException();
		}
	}

	/**
	 * Track external process execution, throw {@link OperationCanceledException} if
	 * operation canceled.
	 * 
	 * @param process external process
	 * @param monitor a progress monitor, or {@code null} if progress reporting and
	 *                cancellation are not desired
	 * 
	 * @throws CoreException for non-zero exit code
	 * @throws IOException   if an I/O error occurs
	 */
	protected void trackProcess(Process process, final SubMonitor monitor) throws CoreException, IOException {
		final Charset inCs = CInitializer.IS_WINDOWS ? Charset.forName("CP866") : StandardCharsets.UTF_8; //$NON-NLS-1$
		InputStream stream = process.getInputStream();
		BufferedReader reader = new BufferedReader(new InputStreamReader(stream, inCs), 10);

		try {
			CStreamParser parser = new CStreamParser(buildResource, SubMonitor.convert(monitor));

			reader.lines().forEach(parser::parse);

			while (process.isAlive() || stream.available() > 0) {
				checkCancel(monitor);
			}
		} catch (Exception e) {
			process.destroy();
			throw e;
		} finally {
			stream.close();
		}

		if (process.exitValue() != 0) {
			throw new CoreException(
					new Status(IStatus.ERROR, getClass(), "Process finished with exit code " + process.exitValue()));
		}
	}

	/**
	 * Get array of resources to build by DFS of {@link IProject}.
	 * 
	 * @param resource current DFS node
	 * 
	 * @return array of {@link IResource}
	 */
	protected IResource[] getCompileResources(final IResource resource) {
		if (!includeBuildSources && resource.getLocationURI().getPath().contains(buildFolderURI.getPath())) {
			return new IResource[] {};
		}

		if (resource instanceof IContainer) {
			try {
				return Arrays.stream(((IContainer) resource).members())
						.flatMap(member -> Arrays.stream(getCompileResources(member))).distinct()
						.toArray(IResource[]::new);
			} catch (CoreException e) {
				Platform.getLog(getClass()).error(e.toString());
			}
		} else if (isSourceFile((IFile) resource)) {
			return new IResource[] { resource };
		} else if (isHeaderFile((IFile) resource)) {
			return new IResource[] { resource.getParent() };
		}

		return new IResource[] {};
	}

	/**
	 * Convert {@link IResource} array to command line arguments.
	 * 
	 * @param resources include directories or source files
	 * 
	 * @return collection of command line arguments
	 * 
	 * @throws CoreException if resources array is empty
	 */
	protected Collection<String> resourcesToArgs(final IResource[] resources) throws CoreException {
		if (resources.length == 0) {
			emitError("Sources not found");
			throw new CoreException(new Status(IStatus.ERROR, getClass(), "Sources not found"));
		}

		return Arrays.stream(resources)
				.map(resource -> resource instanceof IContainer ? "-I\"" + resource.getLocation().toString() + "\"" //$NON-NLS-1$ //$NON-NLS-2$
						: "\"" + resource.getLocation().toString() + "\"") //$NON-NLS-1$ //$NON-NLS-2$
				.collect(Collectors.toList());
	}

	/**
	 * Get Clang mipsel target compile options, if {@link CLANG_MIPSEL_TARGET} set.
	 *
	 * @return collection of command line arguments
	 *
	 * @throws CoreException if {@link CInitializer#MIPS_TOOLCHAIN_PATH} empty
	 */
	protected Collection<String> getMipselArgs() throws CoreException {
		if (!CLANG_MIPSEL.equals(args.get(CLANG_TARGET))) {
			return Collections.emptyList();
		}

		if (output.endsWith(".exe")) { //$NON-NLS-1$
			output = output.substring(0, output.length() - 4) + ".mipsel"; //$NON-NLS-1$
		}

		try {
			String target = Files
					.list(Paths.get(CInitializer.getPreferenceStore().getString(CInitializer.MIPS_TOOLCHAIN_PATH)))
					.filter(path -> Files.isDirectory(path)).map(path -> path.toFile().getName())
					.filter(path -> path.startsWith("mips")).map(path -> path.replaceFirst("mips", "mipsel")) //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
					.findFirst().get();

			List<String> list = new ArrayList<>();
			list.add("--target=" + target); //$NON-NLS-1$
			list.add("-march=mips32r5"); //$NON-NLS-1$
			list.add(
					"--gcc-toolchain=" + CInitializer.getPreferenceStore().getString(CInitializer.MIPS_TOOLCHAIN_PATH)); //$NON-NLS-1$

			if (CInitializer.getPreferenceStore().getBoolean(CInitializer.IS_SOFT_FLOAT)) {
				list.add("-msoft-float"); //$NON-NLS-1$
			}

			return list;
		} catch (IOException e) {
			emitError("GCC Toolchain for MIPS not found");
			throw new CoreException(new Status(IStatus.ERROR, getClass(), "GCC Toolchain for MIPS not found"));
		} catch (NoSuchElementException e) {
			emitError("MIPS Little Endian target not found");
			throw new CoreException(new Status(IStatus.ERROR, getClass(), "MIPS Little Endian target not found"));
		}
	}

	/**
	 * Get compiler path from preferences.
	 * 
	 * @return absolute compiler path
	 * 
	 * @throws CoreException if compiler not found or not executable
	 */
	protected String getCompilerPath() throws CoreException {
		final String clang = isCpp ? "clang++" : "clang"; //$NON-NLS-1$ //$NON-NLS-2$
		final Path path = Paths.get(CInitializer.getPreferenceStore().getString(CInitializer.CLANG_PATH),
				CInitializer.IS_WINDOWS ? clang + ".exe" : clang); //$NON-NLS-1$

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			emitError("Clang compiler not found");
			throw new CoreException(new Status(IStatus.ERROR, getClass(), "Clang compiler not found"));
		}

		return path.toString();
	}

	/**
	 * Check that current file is source to compile.
	 * 
	 * @param file {@link IFile} to check
	 * 
	 * @return {@code true} if need to compile, {@code false} otherwise
	 */
	protected boolean isSourceFile(final IFile file) {
		if (Stream.of("cpp", "ll").anyMatch(extension -> extension.equals(file.getFileExtension()))) { //$NON-NLS-1$ //$NON-NLS-2$
			isCpp = true;
			return true;
		}

		return Stream.of("c", "s", "S").anyMatch(extension -> extension.equals(file.getFileExtension())); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
	}

	/**
	 * Check that current file is header.
	 * 
	 * @param file {@link IFile} to check
	 * 
	 * @return {@code true} if it's header, {@code false} otherwise
	 */
	protected boolean isHeaderFile(final IFile file) {
		return "h".equals(file.getFileExtension()); //$NON-NLS-1$
	}

	/**
	 * Emit error marker on operation {@link IResource}.
	 * 
	 * @param message error message
	 * 
	 * @throws CoreException if this method fails. Reasons include:
	 *                       <ul>
	 *                       <li>This resource does not exist.</li>
	 *                       <li>This resource is a project that is not open.</li>
	 *                       </ul>
	 */
	protected void emitError(final String message) throws CoreException {
		IMarker marker = buildResource.createMarker(markerType);
		marker.setAttribute(IMarker.MESSAGE, message);
		marker.setAttribute(IMarker.SEVERITY, IMarker.SEVERITY_ERROR);
	}

}
