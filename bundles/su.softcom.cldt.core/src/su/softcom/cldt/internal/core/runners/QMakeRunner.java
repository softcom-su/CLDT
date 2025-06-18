package su.softcom.cldt.internal.core.runners;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.ResourceBundle;

import org.eclipse.core.resources.IContainer;
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

/** QMake runner. */
public class QMakeRunner extends ProcessRunner implements IRunner {

	protected static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	// TODO Make normal build path settings
	private static final String DEFAULT_BUILD_FOLDER = "build"; //$NON-NLS-1$

	private List<String> args;

	private IFolder outputFolder;
	private IFile projectFile;

	/**
	 * Create QMake runner.
	 */
	public QMakeRunner() {
		args = new ArrayList<String>();
	}

	@Override
	public void addArg(final String arg) {
		args.add(arg);
	}

	@Override
	public void addResource(final IResource resource) {
		if (projectFile != null || resource == null || !resource.exists()) {
			return;
		}

		if (resource instanceof IFile && "pro".equals(resource.getFileExtension())) { //$NON-NLS-1$
			projectFile = (IFile) resource;
		} else if (DEFAULT_BUILD_FOLDER.equals(resource.getName())) {
			return;
		} else if (resource instanceof IContainer) {
			try {
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
		return new IResource[] { outputFolder };
	}

	@Override
	protected Process buildProcess() {
		problems.deleteMarkers();

		final String qmakePath = getQMakePath();
		if (qmakePath == null) {
			return null;
		}

		final String buildFolder = setBuildFolder();
		if (buildFolder == null) {
			return null;
		}

		final List<String> toRun = new ArrayList<String>();
		toRun.add(qmakePath);

		toRun.add(projectFile.getLocation().toString());
		toRun.addAll(args);

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
		if (console == null) {
			return;
		}

		console.clearConsole();
		final Charset cs = console.getCharset();
		final OutputStream out = console.newOutputStream();

		final BufferedReader in = new BufferedReader(
				new InputStreamReader(process.getInputStream(), StandardCharsets.UTF_8));
		final BufferedReader err = new BufferedReader(
				new InputStreamReader(process.getErrorStream(), StandardCharsets.UTF_8));

		while (process.isAlive() || in.ready() || err.ready()) {
			BufferedReader reader = in.ready() ? in : err;

			int ch = reader.read();
			while (ch != -1) {
				out.write(cs.encode(Character.toString(ch)).array());
				ch = reader.read();
			}

			out.flush();
		}

		in.close();
		err.close();
		out.close();
	}

	protected String getQMakePath() {
		final Path path = Paths.get(CInitializer.getPreferenceStore().getString(CInitializer.QMAKE_PATH));

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.qmakeNotFound")); //$NON-NLS-1$
			return null;
		}

		return path.toAbsolutePath().toString();
	}

	private String setBuildFolder() {
		if (projectFile == null) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.proNotFound")); //$NON-NLS-1$
			return null;
		}

		final IFolder buildFolder = projectFile.getProject().getFolder(DEFAULT_BUILD_FOLDER);
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

}
