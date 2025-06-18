package su.softcom.cldt.core.operations;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.SubMonitor;
import org.eclipse.jface.operation.IRunnableWithProgress;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Environment import operation. */
public class EnvOperation implements IRunnableWithProgress {

	private static final Pattern ARGS_PATTERN = Pattern
			.compile("((([^'\"\\s]+|'([^']|\\\\')*'|\"([^\"]|\\\\\")*\")+)+)"); //$NON-NLS-1$

	private Map<String, String> table;
	private String command;

	/**
	 * Creates a operation instance.
	 * 
	 * @param table   environment output table
	 * @param command a string containing the script and its arguments
	 */
	public EnvOperation(Map<String, String> table, String command) {
		this.table = table;
		this.command = command;
	}

	@Override
	public void run(IProgressMonitor monitor) throws InvocationTargetException, InterruptedException {
		if (table == null) {
			throw new InvocationTargetException(new Throwable(), "Output table isn't set");
		}

		if (command == null || command.isEmpty()) {
			throw new InvocationTargetException(new Throwable(), "Script isn't set");
		}

		final int TOTAL_WORK = 3;
		SubMonitor subMonitor = SubMonitor.convert(monitor, "Import environment variables", TOTAL_WORK);

		try {
			subMonitor.subTask("Split command to script and its arguments");
			List<String> toRun = splitCommand();
			checkCancel(subMonitor);
			subMonitor.worked(1);

			subMonitor.subTask("Execute script");
			trackProcess(new ProcessBuilder(toRun).start(), subMonitor);
			checkCancel(subMonitor);
			subMonitor.worked(1);

			subMonitor.subTask("Intersect environment tables");
			intersectTable();
			subMonitor.worked(1);

		} catch (IOException e) {
			throw new InvocationTargetException(new Throwable(), e.getMessage());
		} finally {
			subMonitor.done();
		}
	}

	/**
	 * Function to check for cancellation as often as possible.
	 * 
	 * @param monitor a progress monitor, or {@code null} if progress reporting and
	 *                cancellation are not desired
	 * 
	 * @throws InterruptedException acknowledged exception
	 */
	protected void checkCancel(IProgressMonitor monitor) throws InterruptedException {
		if (monitor.isCanceled()) {
			throw new InterruptedException();
		}
	}

	/**
	 * Track external process execution, throw {@link InterruptedException} if
	 * operation canceled.
	 * 
	 * @param process external process
	 * @param monitor a progress monitor, or {@code null} if progress reporting and
	 *                cancellation are not desired
	 * 
	 * @throws IOException          if an I/O error occurs
	 * @throws InterruptedException acknowledged exception for request to cancel
	 */
	protected void trackProcess(Process process, IProgressMonitor monitor) throws IOException, InterruptedException {
		BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));

		try {
			while (process.isAlive() || reader.ready()) {
				checkCancel(monitor);

				if (reader.ready()) {
					String line = reader.readLine();
					int index = line.indexOf('=');
					if (index != -1) {
						table.put(line.substring(0, index), line.substring(index + 1, line.length()));
					}
				}
			}
		} catch (InterruptedException e) {
			process.destroy();
			throw e;
		} finally {
			reader.close();
		}
	}

	/**
	 * Split command to script and its arguments.
	 * 
	 * @return command list for {@link ProcessBuilder}
	 */
	private List<String> splitCommand() {
		List<String> toRun = new LinkedList<String>();

		if (CInitializer.IS_WINDOWS) {
			int extensionIndex = command.indexOf(" ", command.lastIndexOf(".bat")); //$NON-NLS-1$ //$NON-NLS-2$

			if (extensionIndex == -1) {
				toRun.add(command);
			} else {
				toRun.add(command.substring(0, extensionIndex)); // $NON-NLS-1$

				Matcher matcher = ARGS_PATTERN.matcher(command.substring(extensionIndex, command.length()));
				while (matcher.find()) {
					toRun.add(matcher.group(1));
				}
			}

			toRun.add(">"); //$NON-NLS-1$
			toRun.add("nul"); //$NON-NLS-1$
			toRun.add("&&"); //$NON-NLS-1$
			toRun.add("set"); //$NON-NLS-1$
		} else {
			toRun.add("/bin/bash"); //$NON-NLS-1$
			toRun.add("-c"); //$NON-NLS-1$

			toRun.add("source " + command + " &> /dev/null && printenv"); //$NON-NLS-1$ //$NON-NLS-2$
		}

		return toRun;
	}

	/**
	 * Remove already exists environment variables.
	 */
	private void intersectTable() {
		System.getenv().entrySet().stream().filter(entry -> table.containsKey(entry.getKey())).forEach(entry -> {
			String value = table.get(entry.getKey());
			List<String> list = new ArrayList<String>(Arrays.asList(value.split(";"))); //$NON-NLS-1$
			list.removeAll(Arrays.asList(entry.getValue().split(";"))); //$NON-NLS-1$
			list.removeIf(String::isEmpty);

			value = list.stream().reduce((fst, snd) -> fst + ";" + snd).orElse(null); //$NON-NLS-1$
			if (value == null) {
				table.remove(entry.getKey());
			} else {
				table.put(entry.getKey(), value);
			}
		});

		table.remove("CI_COMMIT_MESSAGE"); //$NON-NLS-1$
		table.remove("CI_COMMIT_AUTHOR"); //$NON-NLS-1$
		table.remove("CI_COMMIT_TITLE"); //$NON-NLS-1$
		table.remove("CI_COMMIT_DESCRIPTION"); //$NON-NLS-1$
		table.remove("GITLAB_USER_NAME"); //$NON-NLS-1$

		table.remove("PWD"); //$NON-NLS-1$
		table.remove("_"); //$NON-NLS-1$

		table.remove("PROMPT"); //$NON-NLS-1$
		table.remove("__VSCMD_PREINIT_PATH"); //$NON-NLS-1$
	}

}
