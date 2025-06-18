package su.softcom.cldt.internal.core.runners;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.LinkedList;
import java.util.List;
import java.util.ResourceBundle;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.eclipse.ui.console.IOConsole;
import org.osgi.framework.FrameworkUtil;

import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.core.runners.ProcessRunner;
import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Windows Subsystem for Linux runner. */
public class WSLRunner extends ProcessRunner implements IRunner {

	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	private static final String WSL_PATH = CInitializer.getPreferenceStore().getString(CInitializer.WSL_PATH);
	private static final String SUFFIX = "/mnt/"; //$NON-NLS-1$

	private List<String> args;
	private IFile executable;

	/**
	 * Create WSL runner.
	 */
	public WSLRunner() {
		this(null);
	}

	/**
	 * Create WSL runner with specified I/O console.
	 * 
	 * @param console I/O console
	 */
	public WSLRunner(final IOConsole console) {
		super(console);
		args = new LinkedList<String>();
	}

	@Override
	public void addArg(final String arg) {
		args.add(arg);
	}

	@Override
	public void addResource(final IResource resource) {
		if (resource instanceof IFile) {
			args.clear();
			executable = (IFile) resource;
			problems.setDefault(resource.getProject());
		}
	}

	@Override
	protected Process buildProcess() {
		problems.deleteMarkers();

		List<String> toRun = new LinkedList<String>();
		toRun.add(WSL_PATH);

		if (CInitializer.IS_WINDOWS) {
			toRun.add("run"); //$NON-NLS-1$
		}

		toRun.add("stdbuf"); //$NON-NLS-1$
		toRun.add("-o0"); //$NON-NLS-1$

		final String script = getExecutable();
		if (script == null) {
			return null;
		}

		toRun.add(script);
		toRun.addAll(args);

		final ILog log = Platform.getLog(FrameworkUtil.getBundle(getClass()));
		log.info(toRun.stream().reduce((fst, snd) -> fst + " " + snd).orElse("")); //$NON-NLS-1$//$NON-NLS-2$

		final ProcessBuilder processBuilder = new ProcessBuilder(toRun);
		processBuilder.redirectErrorStream(true);

		try {
			return processBuilder.start();
		} catch (IOException e) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.ubuntuProblem")); //$NON-NLS-1$
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

	/**
	 * Get executable file path.
	 * 
	 * @return executable path
	 */
	protected String getExecutable() {
		if (executable == null) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.executableNotFound")); //$NON-NLS-1$
			return null;
		}

		return toUnixPath(executable.getLocation().toString());
	}

	/**
	 * Convert Windows path to UNIX format
	 * 
	 * @param path Windows path
	 * 
	 * @return UNIX path
	 */
	protected static String toUnixPath(final String path) {
		if (!CInitializer.IS_WINDOWS || path == null || path.equals("")) { //$NON-NLS-1$
			return path;
		}

		String unix = path.replaceAll("\\\\", "/"); //$NON-NLS-1$ //$NON-NLS-2$
		return unix.charAt(1) == ':' ? unix = SUFFIX + Character.toLowerCase(unix.charAt(0)) + unix.substring(2) // $NON-NLS-1$
				: unix;
	}

}
