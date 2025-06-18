package su.softcom.cldt.internal.core.runners;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.ResourceBundle;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.osgi.framework.FrameworkUtil;

import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.core.runners.ProcessRunner;
import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Make runner. */
public class MakeRunner extends ProcessRunner implements IRunner {

	protected static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	// TODO Make normal build path settings
	private static final String DEFAULT_OUT_FOLDER = "release"; //$NON-NLS-1$

	private List<String> args;

	private IFolder buildFolder;

	/**
	 * Create QMake runner.
	 */
	public MakeRunner() {
		args = new ArrayList<String>();
	}

	@Override
	public void addArg(final String arg) {
		args.add(arg);
	}

	@Override
	public void addResource(final IResource resource) {
		if (resource == null || !resource.exists() || !(resource instanceof IFolder)) {
			return;
		}

		buildFolder = (IFolder) resource;
	}

	@Override
	public IResource[] getResults() {
		final IFolder output = buildFolder.getFolder(DEFAULT_OUT_FOLDER);

		try {
			for (final IResource member : output.members()) {
				if (member instanceof IFile && member.getResourceAttributes().isExecutable()
						&& (!CInitializer.IS_WINDOWS || "exe".equals(member.getFileExtension()))) { //$NON-NLS-1$
					System.out.println(member.getLocation().toString());
					return new IResource[] { member };
				}
			}
		} catch (CoreException e) {
			return new IResource[] {};
		}

		return new IResource[] {};
	}

	@Override
	protected Process buildProcess() {
		problems.deleteMarkers();

		final String makePath = getMakePath();
		if (makePath == null) {
			return null;
		}

		args.add(0, makePath);

		ILog log = Platform.getLog(FrameworkUtil.getBundle(getClass()));
		log.info(buildFolder.getLocation().toString() + "> " //$NON-NLS-1$
				+ args.stream().reduce((fst, snd) -> fst + " " + snd).orElse("NON")); //$NON-NLS-1$ //$NON-NLS-2$

		final ProcessBuilder processBuilder = new ProcessBuilder(args);
		processBuilder.directory(new File(buildFolder.getLocation().toString()));

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
				final int firstColon = line.indexOf(":", 2); //$NON-NLS-1$
				final int lastColon = line.lastIndexOf(":"); //$NON-NLS-1$
				if (firstColon == -1 || firstColon == lastColon || lastColon == line.length() - 1) {
					continue;
				}

				String message = line.substring(line.indexOf(" ", lastColon) + 1); //$NON-NLS-1$
				message = message.substring(0, 1).toUpperCase() + message.substring(1);

				if (line.contains("error")) { //$NON-NLS-1$
					problems.emitError(message);
				} else if (line.contains("warning")) { //$NON-NLS-1$
					problems.emitWarning(message);
				} else if (line.contains("note")) { //$NON-NLS-1$
					problems.emitInfo(message);
				}
			}
		}

		reader.close();
	}

	protected String getMakePath() {
		Path path = Paths.get(CInitializer.getPreferenceStore().getString(CInitializer.GCC_PATH), "mingw32-make.exe"); //$NON-NLS-1$

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			path = Paths.get(path.getParent().toString(), CInitializer.IS_WINDOWS ? "make.exe" : "make"); //$NON-NLS-1$ //$NON-NLS-2$
		}

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.makeNotFound")); //$NON-NLS-1$
			return null;
		}

		return path.toAbsolutePath().toString();
	}

}
