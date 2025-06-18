package su.softcom.cldt.internal.core.runners;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.ResourceBundle;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.osgi.framework.FrameworkUtil;

import su.softcom.cldt.core.operations.CStreamParser;
import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.core.runners.ProcessRunner;
import su.softcom.cldt.internal.core.preferences.CInitializer;

/** CMake runner. */
public class CMakeRunner extends ProcessRunner implements IRunner {

	protected static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	private static final String CMAKE_LISTS = "CMakeLists.txt"; //$NON-NLS-1$
	private static final String CMAKE_CACHE = "CMakeCache.txt"; //$NON-NLS-1$

	// TODO Make normal build path settings
	private static final String DEFAULT_BUILD_FOLDER = "build"; //$NON-NLS-1$

	private String config;
	private List<String> args;

	private IFolder outputFolder;
	private IContainer target;

	private String userBuildFolder;
	private boolean isBuild = false;

	/**
	 * Create СMake runner.
	 */
	public CMakeRunner() {
		args = new ArrayList<String>();
		userBuildFolder = null;
	}

	@Override
	@SuppressWarnings("nls")
	public void addArg(final String arg) {
		if (arg.contains("Debug")) {
			config = "Debug";
		} else if (arg.contains("Release")) {
			config = "Release";
		}

		args.add(arg);
	}

	@Override
	public void addResource(final IResource resource) {
		if (resource != null && resource instanceof IContainer) {
			problems.setDefault(resource.getProject());
			target = (IContainer) resource;
		}
	}

	@Override
	public IResource[] getResults() {
		if (target == null) {
			return null;
		}

		if (target.findMember(CMAKE_LISTS) != null) {
			return new IResource[] { outputFolder };
		}

		IResource exec = searchExecutable(target);
		if (exec == null) {
			final IResource release = searchExecutable(target.findMember("Release")); //$NON-NLS-1$
			final IResource debug = searchExecutable(target.findMember("Debug")); //$NON-NLS-1$

			exec = debug == null ? release
					: release == null ? debug
							: debug.getLocalTimeStamp() < release.getLocalTimeStamp() ? release : debug;
		}

		return new IResource[] { exec };
	}

	@Override
	protected Process buildProcess() {
		problems.deleteMarkers();

		final String cmakePath = getСMakePath();
		if (cmakePath == null) {
			return null;
		}

		if (target == null) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.sourcesNotFound")); //$NON-NLS-1$
			return null;
		}

		ProcessBuilder processBuilder;
		final List<String> toRun = new ArrayList<String>();
		toRun.add(cmakePath);

		if (target.findMember(CMAKE_LISTS) != null) {
			final String buildFolder = setBuildFolder();
			if (buildFolder == null) {
				return null;
			}

			toRun.add(target.getLocation().toString()); // $NON-NLS-1$
			toRun.addAll(args);

			processBuilder = new ProcessBuilder(toRun);
			processBuilder.directory(new File(buildFolder));

		} else if (target.findMember(CMAKE_CACHE) != null) {
			isBuild = true;

			toRun.add("--build"); //$NON-NLS-1$
			toRun.add(target.getLocation().toString());

			if (config != null) {
				toRun.add("--config"); //$NON-NLS-1$
				toRun.add(config);
			}

			toRun.addAll(args);

			processBuilder = new ProcessBuilder(toRun);
		} else {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.cmakeListsNotFound")); //$NON-NLS-1$
			return null;
		}

		processBuilder.redirectErrorStream(true);

		ILog log = Platform.getLog(FrameworkUtil.getBundle(getClass()));
		log.info(toRun.stream().reduce((fst, snd) -> fst + " " + snd).orElse("NON")); //$NON-NLS-1$//$NON-NLS-2$

		try {
			return processBuilder.start();
		} catch (IOException e) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.cannotStartCompiling")); //$NON-NLS-1$
			return null;
		}
	}

	@Override
	protected void streamsRedirector(final Process process) throws IOException {
		final Charset inCs = CInitializer.IS_WINDOWS ? Charset.forName("CP866") : StandardCharsets.UTF_8; //$NON-NLS-1$
		InputStream stream = process.getInputStream();
		BufferedReader reader = new BufferedReader(new InputStreamReader(stream, inCs), 10);
		CStreamParser parser = new CStreamParser(problems.getDefault(), null);

		reader.lines().forEach(parser::parse);

		stream.close();
	}

	protected String getСMakePath() {
		final Path path = Paths.get(CInitializer.getPreferenceStore().getString(CInitializer.CMAKE_PATH));

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.cmakeNotFound")); //$NON-NLS-1$
			return null;
		}

		return path.toAbsolutePath().toString();
	}

	private String setBuildFolder() {
		final IFolder buildFolder = userBuildFolder == null ? target.getProject().getFolder(DEFAULT_BUILD_FOLDER)
				: target.getProject().getFolder(userBuildFolder);
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

	private IResource searchExecutable(final IResource resource) {
		if (resource == null || !(resource instanceof IContainer)) {
			return null;
		}

		try {
			for (final IResource member : ((IContainer) resource).members()) {
				if ("exe".equals(member.getFileExtension()) || member.getResourceAttributes().isExecutable()) { //$NON-NLS-1$
					return member;
				}
			}
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.getMessage());
		}

		return null;
	}

	/**
	 * @param folder folder
	 */
	public void setUserBuildFolder(String folder) {
		userBuildFolder = folder;
	}

}
