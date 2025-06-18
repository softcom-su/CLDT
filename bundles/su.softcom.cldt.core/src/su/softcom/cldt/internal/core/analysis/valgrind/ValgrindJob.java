package su.softcom.cldt.internal.core.analysis.valgrind;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.Job;

/**
 * Запуск valgrind и вывод его результатов работы
 * 
 * Status.OK_STATUS - valgrind не обнаружил ошибок Status(IStatus.ERROR, ... -
 * valgrind обнаружил ошибки Status.CANCEL_STATUS - работа принудительно
 * остановлена
 */

public class ValgrindJob extends Job {

	private final IProject project;
	private final List<String> valgrindArgs;
	private final String filesName;

	/**
	 * @param project
	 * @param filesName
	 * @param valgrindArgs
	 */
	public ValgrindJob(IProject project, String filesName, String... valgrindArgs) {
		super("Valgrind Analysis"); //$NON-NLS-1$
		this.project = project;
		this.valgrindArgs = new ArrayList<>(Arrays.asList(valgrindArgs));
		this.filesName = filesName;
	}

	@Override
	public IStatus run(IProgressMonitor monitor) {
		try {
			String executablePath = filesName;

			List<String> command = new ArrayList<>();
			command.add("valgrind"); //$NON-NLS-1$
			command.add("--error-exitcode=1"); //$NON-NLS-1$
			command.add("--xml=yes"); //$NON-NLS-1$
			command.addAll(valgrindArgs);
			command.add(executablePath);

			ProcessBuilder pb = new ProcessBuilder(command);
			pb.redirectErrorStream(false); // Don't merge stdout and stderr

			if (monitor.isCanceled()) {
				return Status.CANCEL_STATUS;
			}

			Process process = pb.start();

			StringBuilder valgrindOutput = new StringBuilder();
			StringBuilder valgrindErrorOutput = new StringBuilder();

			// Поток для чтения стандартного вывода Valgrind
			Thread outputReaderThread = new Thread(() -> {
				try (BufferedReader outputReader = new BufferedReader(
						new InputStreamReader(process.getInputStream()))) {
					String line;
					while ((line = outputReader.readLine()) != null) {
						valgrindOutput.append(line).append("\n"); //$NON-NLS-1$
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			});

			// Поток для чтения ошибок Valgrind
			Thread errorReaderThread = new Thread(() -> {
				try (BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()))) {
					String line;
					while ((line = errorReader.readLine()) != null) {
						valgrindErrorOutput.append(line).append("\n"); //$NON-NLS-1$
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			});

			outputReaderThread.start();
			errorReaderThread.start();

			int exitCode = process.waitFor();
			outputReaderThread.join();
			errorReaderThread.join();

			if (monitor.isCanceled()) {
				return Status.CANCEL_STATUS;
			}

			if (exitCode == 0) {
				return Status.OK_STATUS;
			} else {
				return new Status(IStatus.ERROR, "valgrind.plugin", //$NON-NLS-1$
						"Valgrind process exited with code " + exitCode + ":\n" + valgrindOutput.toString() + "\n" //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
								+ valgrindErrorOutput.toString());
			}

		} catch (IOException | InterruptedException e) {
			return new Status(IStatus.ERROR, "valgrind.plugin", "Error while running Valgrind: " + e.getMessage(), e); //$NON-NLS-1$ //$NON-NLS-2$
		}
	}

	/**
	 * @param project11
	 * @param fileName
	 * @return String
	 * @throws CoreException
	 */
	public String getExecutablePath(IProject project11, String fileName) throws CoreException {
		return findFileRecursively(project11, fileName);
	}

	private String findFileRecursively(IResource resource, String fileName) {
		try {
			if (resource instanceof IFile && resource.getName().equals(fileName)) {
				return resource.getLocation().toOSString();
			} else if (resource instanceof IProject || resource instanceof org.eclipse.core.resources.IFolder) {
				IProject project = (IProject) resource;

				// Проверка, что проект открыт
				if (!project.isOpen()) {
					project.open(null); // Открываем проект, если он закрыт
				}

				IResource[] members = project.members();

				// Проверка на null, чтобы избежать NullPointerException
				if (members != null) {
					for (IResource member : members) {
						String result = findFileRecursively(member, fileName);
						if (result != null) {
							return result;
						}
					}
				}
			}
		} catch (CoreException e) {
			e.printStackTrace(); // Логируем исключение для диагностики
			return null;
		}
		return null;
	}
}
