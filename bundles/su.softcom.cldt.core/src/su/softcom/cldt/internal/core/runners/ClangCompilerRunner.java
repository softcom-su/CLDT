package su.softcom.cldt.internal.core.runners;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.ResourceBundle;
import java.util.Set;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.eclipse.ui.console.IOConsole;
import org.osgi.framework.FrameworkUtil;

import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.core.runners.ProcessRunner;
import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Clang compiler runner. */
public class ClangCompilerRunner extends ProcessRunner implements IRunner {

	protected static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	// TODO Make normal build path settings
	private static final String DEFAULT_BUILD_FOLDER = "build"; //$NON-NLS-1$
	private static final String DEFAULT_OUTPUT = "out.exe"; //$NON-NLS-1$

	private Set<IFolder> folders;
	private Set<IFile> files;
	private List<String> args;

	private String outputName;
	private IFolder outputFolder;

	private String userBuildFolder;

	protected boolean isRemovingMarkers;

	protected boolean isCpp;

	/**
	 * Create Clang compiler runner.
	 */
	public ClangCompilerRunner() {
		this(null);
	}

	/**
	 * Create Clang compiler runner with specified I/O console.
	 * 
	 * @param console I/O console
	 */
	public ClangCompilerRunner(final IOConsole console) {
		super(console);

		folders = new HashSet<IFolder>();
		files = new HashSet<IFile>();
		args = new ArrayList<String>();

		outputName = DEFAULT_OUTPUT;
		isCpp = false;
		isRemovingMarkers = true;
		userBuildFolder = null;
	}

	@Override
	public void addArg(final String arg) {
		if (arg.startsWith("-o")) { //$NON-NLS-1$
			outputName = arg.substring(2);
			return;
		}

		args.add(arg);
	}

	@Override
	public void addResource(final IResource resource) {
		if (resource == null || !resource.exists()) {
			return;
		}

		if (resource instanceof IFile) {
			addFile((IFile) resource);
		} else if (resource.getLocation().toString().contains(DEFAULT_BUILD_FOLDER)) {
			return;
		} else if (resource instanceof IContainer) {
			try {
				if (resource instanceof IFolder) {
					addFolder((IFolder) resource);
				} else if (resource instanceof IProject && !(problems.getDefault() instanceof IProject)) {
					problems.setDefault(resource);
				}

				for (final IResource member : ((IContainer) resource).members()) {
					addResource(member);
				}
			} catch (CoreException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public IResource[] getResults() {
		return new IResource[] { outputFolder.getFile(outputName) };
	}

	@Override
	protected Process buildProcess() {
		if (isRemovingMarkers) {
			problems.deleteMarkers();
		}

		final String compilerPath = getCompilerPath();
		if (compilerPath == null) {
			return null;
		}

		final String buildFolder = setBuildFolder();
		if (buildFolder == null) {
			return null;
		}

		final List<String> toRun = new ArrayList<String>();
		toRun.add(compilerPath);

		toRun.add("-o" + outputName); //$NON-NLS-1$

		toRun.addAll(args);
		toRun.addAll(files.stream().map(file -> file.getLocation().toString()).collect(Collectors.toList()));
		toRun.addAll(
				folders.stream().map(folder -> "-I" + folder.getLocation().toString()).collect(Collectors.toList())); //$NON-NLS-1$

		ILog log = Platform.getLog(FrameworkUtil.getBundle(getClass()));
		log.info(toRun.stream().reduce((fst, snd) -> fst + " " + snd).orElse("NON")); //$NON-NLS-1$//$NON-NLS-2$

		final ProcessBuilder processBuilder = new ProcessBuilder(toRun);
		processBuilder.directory(new File(buildFolder));

		try {
			return processBuilder.start();
		} catch (IOException e) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.cannotStartCompiling")); //$NON-NLS-1$
			return null;
		}
	}

	@Override
	protected void streamsRedirector(final Process process) throws IOException {
		final BufferedReader reader = new BufferedReader(new InputStreamReader(process.getErrorStream()));

		while (process.isAlive() || reader.ready()) {
			if (reader.ready()) {
				final String line = reader.readLine();

				// Just for Windows paths, like "C:\..."
				final int firstColon = line.indexOf(':', 2); // $NON-NLS-1$
				final int lastColon = line.lastIndexOf(':'); // $NON-NLS-1$
				if (firstColon == -1 || firstColon == lastColon || lastColon == line.length() - 1) {
					continue;
				}

				String message = line.substring(line.indexOf(' ', lastColon) + 1); // $NON-NLS-1$
				message = message.substring(0, 1).toUpperCase() + message.substring(1);

				final String path = line.substring(0, firstColon);
				final int secondColon = line.indexOf(':', firstColon + 1); // $NON-NLS-1$
				final int lineNumber = secondColon != lastColon && Character.isDigit(line.charAt(firstColon + 1))
						? Integer.parseInt(line.substring(firstColon + 1, secondColon))
						: -1;

				if (line.contains("error")) { //$NON-NLS-1$
					problems.emitError(path, lineNumber, message);
				} else if (line.contains("warning")) { //$NON-NLS-1$
					problems.emitWarning(path, lineNumber, message);
				} else if (line.contains("note")) { //$NON-NLS-1$
					problems.emitInfo(path, lineNumber, message);
				}
			}
		}

		reader.close();
	}

	protected String getCompilerPath() {
		final Path path = Paths.get(CInitializer.getPreferenceStore().getString(CInitializer.CLANG_PATH),
				CInitializer.IS_WINDOWS ? isCpp ? "clang++.exe" : "clang.exe" : isCpp ? "clang++" : "clang"); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			problems.emitError(MessageFormat.format(RESOURCE_BUNDLE.getString("errors.compilerNotFound"), "Clang")); //$NON-NLS-1$ //$NON-NLS-2$
			return null;
		}

		return path.toAbsolutePath().toString();
	}

	private void addFile(final IFile file) {
		if ("c".equals(file.getFileExtension()) || "s".equals(file.getFileExtension()) //$NON-NLS-1$//$NON-NLS-2$
				|| "S".equals(file.getFileExtension())) { //$NON-NLS-1$
			files.add(file);
		} else if ("cpp".equals(file.getFileExtension()) || "ll".equals(file.getFileExtension())) { //$NON-NLS-1$ //$NON-NLS-2$
			files.add(file);
			isCpp = true;
		}
	}

	private void addFolder(final IFolder folder) throws CoreException {
		for (final IResource member : folder.members()) {
			if (member instanceof IFile && "h".equals(member.getFileExtension())) { //$NON-NLS-1$
				folders.add(folder);
				return;
			}
		}
	}

	protected String setBuildFolder() {
		if (files.isEmpty()) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.sourcesNotFound")); //$NON-NLS-1$
			return null;
		}

		final IProject project = problems.getDefault() instanceof IProject ? problems.getDefault().getProject()
				: files.stream().findFirst().get().getProject();

		final IFolder buildFolder = userBuildFolder == null ? project.getFolder(DEFAULT_BUILD_FOLDER)
				: project.getFolder(userBuildFolder);
		if (!buildFolder.exists()) {
			try {
				buildFolder.create(IResource.DERIVED, true, null);
			} catch (CoreException e) {
				problems.emitError(RESOURCE_BUNDLE.getString("errors.cannotCreateBuildFolder")); //$NON-NLS-1$
				return null;
			}
		}

		outputFolder = buildFolder;
		return buildFolder.getLocation().toString();
	}

	/**
	 * @param flag set if currently removing markers
	 */
	public void setRemovingMarkers(boolean flag) {
		isRemovingMarkers = flag;
	}

	/**
	 * @param folder folder
	 */
	public void setUserBuildFolder(String folder) {
		userBuildFolder = folder;
	}

}
