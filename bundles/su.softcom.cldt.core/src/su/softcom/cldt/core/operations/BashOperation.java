package su.softcom.cldt.core.operations;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.OperationCanceledException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.SubMonitor;
import org.eclipse.ui.console.IOConsole;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Bash script executable operation. */
public class BashOperation implements ICoreRunnable {

	/** Script additional command line arguments key. */
	public static final String BASH_ARGS = "BASH_ARGS"; //$NON-NLS-1$

	/** Script launch folder name key. */
	public static final String BASH_LAUNCH_FOLDER = "BASH_LAUNCH_FOLDER"; //$NON-NLS-1$

	public static final String DEFAULT_LAUNCH_FOLDER = "build"; //$NON-NLS-1$

	private final String BASH_PATH = CInitializer.getPreferenceStore().getString(CInitializer.BASH_PATH);
	private final String SUFFIX = BASH_PATH.contains("cygwin") ? "/cygdrive/" //$NON-NLS-1$ //$NON-NLS-2$
			: BASH_PATH.contains("msys") || BASH_PATH.contains("Git") ? "/" : "/mnt/"; //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$

	protected final Map<String, String> args;
	protected final IFile script;
	protected final URI folderURI;

	private final IOConsole console;

	/**
	 * Create bash script executable operation.
	 * 
	 * @param script  the script for which this operation is defined
	 * @param args    a table of builder-specific arguments keyed by argument
	 *                name(key type: {@code String} value type: {@code String});
	 *                {@code null} is equivalent to an empty map
	 * @param console I/O console
	 */
	public BashOperation(IFile script, Map<String, String> args, IOConsole console) {
		this.script = script;
		this.args = args != null ? args : new HashMap<String, String>();
		this.console = console;

		IFolder folder = this.script.getProject()
				.getFolder(this.args.getOrDefault(BASH_LAUNCH_FOLDER, DEFAULT_LAUNCH_FOLDER));
		this.folderURI = folder.exists() ? folder.getLocationURI() : this.script.getParent().getLocationURI();
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		SubMonitor subMonitor = SubMonitor.convert(monitor, "Run " + script.getName(), 1);

		try {
			String command = toUnixPath(script.getLocation().toString()) + " " //$NON-NLS-1$
					+ Arrays.stream(args.getOrDefault(BASH_ARGS, "").replace("\r", "").split("\\n")) //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
							.map(arg -> toUnixPath(arg)).reduce((fst, snd) -> fst + " " + snd).orElse(""); //$NON-NLS-1$//$NON-NLS-2$

			Platform.getLog(getClass()).info(getShellPath() + " -c \"" + command + "\""); //$NON-NLS-1$ //$NON-NLS-2$

			Process process = new ProcessBuilder(getShellPath(), "-c", command).directory(new File(folderURI.getPath())) //$NON-NLS-1$
					.redirectErrorStream(true).start();
			trackProcess(process, subMonitor, console);
			subMonitor.worked(1);

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
	 * @param process   external process
	 * @param monitor   a progress monitor, or {@code null} if progress reporting
	 *                  and cancellation are not desired
	 * @param ioConsole I/O console
	 * 
	 * @throws CoreException for non-zero exit code
	 * @throws IOException   if an I/O error occurs
	 */
	protected void trackProcess(Process process, IProgressMonitor monitor, IOConsole ioConsole)
			throws CoreException, IOException {
		OutputStream out = ioConsole.newOutputStream();
		Charset outCs = ioConsole.getCharset();
		InputStream in = process.getInputStream();
		Charset inCs = StandardCharsets.UTF_8;

		try {
			while (process.isAlive() || in.available() > 0) {
				checkCancel(monitor);

				byte[] bytes = in.readNBytes(in.available());
				out.write(outCs.encode(inCs.decode(ByteBuffer.wrap(bytes))).array());
				System.out.write(outCs.encode(inCs.decode(ByteBuffer.wrap(bytes))).array());
				out.flush();
			}
		} catch (Exception e) {
			process.destroy();
			throw e;
		} finally {
			in.close();
			out.close();
		}
	}

	/**
	 * Get bash shell path from preferences.
	 * 
	 * @return absolute bash shell path
	 * 
	 * @throws CoreException if shell not found or not executable
	 */
	protected String getShellPath() throws CoreException {
		Path path = Paths.get(BASH_PATH);

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			emitError("Bash shell not found");
			throw new CoreException(new Status(IStatus.ERROR, getClass(), "Bash shell not found"));
		}

		return path.toString();
	}

	/**
	 * Convert Windows path to UNIX format
	 * 
	 * @param path Windows path
	 * 
	 * @return UNIX path
	 */
	protected String toUnixPath(String path) {
		if (!CInitializer.IS_WINDOWS || path == null || path.equals("")) { //$NON-NLS-1$
			return path;
		}

		String unix = path.replaceAll("\\\\", "/"); //$NON-NLS-1$ //$NON-NLS-2$
		return unix.charAt(1) == ':' ? unix = SUFFIX + Character.toLowerCase(unix.charAt(0)) + unix.substring(2) // $NON-NLS-1$
				: unix;
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
	protected void emitError(String message) throws CoreException {
		IMarker marker = script.getProject().createMarker(IMarker.PROBLEM);
		marker.setAttribute(IMarker.MESSAGE, message);
		marker.setAttribute(IMarker.SEVERITY, IMarker.SEVERITY_ERROR);
	}

}
