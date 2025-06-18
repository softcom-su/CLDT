package su.softcom.cldt.core.operations;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.OperationCanceledException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.SubMonitor;
import org.eclipse.ui.console.IOConsole;

/** Base executable file operation. */
public class ExeOperation implements ICoreRunnable {

	/** Executable additional command line arguments key. */
	public static final String EXE_ARGS = "EXE_ARGS"; //$NON-NLS-1$

	/** Executable launch folder {@link java.net.URI} key. */
	public static final String EXE_LAUNCH_FOLDER_URI = "EXE_LAUNCH_FOLDER_URI"; //$NON-NLS-1$

	/** Command line argument separation pattern. */
	protected static final Pattern ARGS_PATTERN = Pattern
			.compile("(([^'\"\\s\\|&>]+|'([^']|\\\\')*'|\"([^\"]|\\\")*\")+|[\\|&>]{1,2}+)"); //$NON-NLS-1$

	protected final Map<String, String> args;
	protected final IFile executable;
	protected File folder;

	private final IOConsole console;

	/**
	 * Create executable file operation.
	 * 
	 * @param executable the file for which this operation is defined
	 * @param args       a table of builder-specific arguments keyed by argument
	 *                   name(key type: {@code String} value type: {@code String});
	 *                   {@code null} is equivalent to an empty map
	 */
	public ExeOperation(IFile executable, Map<String, String> args) {
		this(executable, args, null);
	}

	/**
	 * Create executable file operation.
	 * 
	 * @param executable the file for which this operation is defined
	 * @param args       a table of builder-specific arguments keyed by argument
	 *                   name(key type: {@code String} value type: {@code String});
	 *                   {@code null} is equivalent to an empty map
	 * @param console    I/O console
	 */
	public ExeOperation(IFile executable, Map<String, String> args, IOConsole console) {
		this.executable = executable;
		this.args = args != null ? args : new HashMap<String, String>();
		this.folder = executable.getLocation().toFile().getParentFile();
		this.console = console;

		try {
			URI folderURL = new URI(this.args.get(EXE_LAUNCH_FOLDER_URI));
			IContainer[] containers = ResourcesPlugin.getWorkspace().getRoot().findContainersForLocationURI(folderURL);

			if (containers.length > 0 && containers[0].exists() && containers[0] instanceof IFolder) {
				folder = containers[0].getLocation().toFile();
			}
		} catch (Exception e) {
			// could not found EXE_LAUNCH_FOLDER_URI attribute, ignore
		}
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		SubMonitor subMonitor = SubMonitor.convert(monitor, "Run " + executable.getName(), 1);

		try {
			List<String> toRun = new ArrayList<String>();
			toRun.add(executable.getLocation().toString());

			Matcher matcher = ARGS_PATTERN.matcher(args.getOrDefault(EXE_ARGS, "")); //$NON-NLS-1$
			while (matcher.find()) {
				toRun.add(matcher.group(1));
			}

			Platform.getLog(getClass()).info(toRun.stream().collect(Collectors.joining(" "))); //$NON-NLS-1$
			Process process = new ProcessBuilder(toRun).directory(folder).redirectErrorStream(true).start();

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
		if (ioConsole == null) {
			trackProcess(process, monitor);
			return;
		}

		OutputStream out = ioConsole.newOutputStream();
		Charset outCs = ioConsole.getCharset();
		InputStream in = process.getInputStream();
		Charset inCs = StandardCharsets.UTF_8;

		try {
			while (process.isAlive() || in.available() > 0) {
				checkCancel(monitor);

				byte[] bytes = in.readNBytes(in.available());
				out.write(outCs.encode(inCs.decode(ByteBuffer.wrap(bytes))).array());
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
	protected void trackProcess(Process process, IProgressMonitor monitor) throws CoreException, IOException {
		try {
			while (process.isAlive()) {
				checkCancel(monitor);
			}
		} catch (Exception e) {
			process.destroy();
			throw e;
		}
	}

}
