package su.softcom.cldt.internal.core.builders;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.DefaultScope;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.builders.CMakeProcessMonitor;
import su.softcom.cldt.core.builders.CMakeProcessMonitor.OUTPUT_TYPE;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.internal.core.CMakeProject;

abstract class CMakeBuilder extends IncrementalProjectBuilder {

	protected ICMakeProject project;
	private static final String SUCCESSFULL_OPERATION_MESSAGE = "%s проекта %s успешно завершена";
	private static final String ERROR_OPERATION_MESSAGE = "%s проекта %s завершилась ошибкой";
	private static final String DELIMITER_STRING = "\n-------------------------------------------------------------------------------\n"; //$NON-NLS-1$
	DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"); //$NON-NLS-1$

	protected ILog builderLog = Platform.getLog(getClass());
	IEclipsePreferences instanceNode = InstanceScope.INSTANCE.getNode(CMakeCorePlugin.PLUGIN_ID);
	IEclipsePreferences defaultNode = DefaultScope.INSTANCE.getNode(CMakeCorePlugin.PLUGIN_ID);
	CMakeProcessMonitor proxy = CMakeCorePlugin.getCMakeBuildMonitor();

	protected String getCMakeInstance() {
		return CMakeCorePlugin.eINSTANCE.getProject(getProject()).getCmakeInstance().getPath().toOSString();
	}

	protected CMakeProject getCMakeProject() {
		if (project == null) {
			project = CMakeCorePlugin.eINSTANCE.getProject(getProject());
		}
		return (CMakeProject) project;
	}

	protected IFolder getBuildFolder() {
		return getCMakeProject().getBuildFolder();
	}

	/**
	 * Возвращает статус процесса конфигурации. Если true, то конфигурация прошла
	 * успешно и проект можно собрать.
	 * 
	 * @return статуc конфигурации проекта
	 *         {@link su.softcom.cldt.internal.core.CMakeProject#getConfigurationStatus()
	 *         getConfigurationStatus()}
	 */
	protected boolean getConfigurationStatus() {
		return getCMakeProject().getConfigurationStatus();
	}

	private void readFromStream(BufferedReader reader, Consumer<String> appendFunction) throws IOException {
		StringBuilder tmp = new StringBuilder();
		while (reader.ready()) {
			tmp.append((char) reader.read());
		}
		if (tmp.length() > 0) {
			appendFunction.accept(tmp.toString());
		}
	}

	private void readOutput(BufferedReader outReader, BufferedReader errorReader) throws IOException {
		readFromStream(outReader, str -> proxy.addLine(project, str, OUTPUT_TYPE.OUT));
		readFromStream(errorReader, str -> proxy.addLine(project, str, OUTPUT_TYPE.ERROR));
	}

	protected void startCMakeProcess(List<String> args, IProgressMonitor monitor, String message) {
		List<String> commands = new ArrayList<>();
		commands.add(getCMakeInstance());
		commands.addAll(args);
		builderLog.info(commands.stream().collect(Collectors.joining(" "))); //$NON-NLS-1$
		ProcessBuilder builder = new ProcessBuilder(commands);
		try {
			proxy.addLine(project, "%s:%n".formatted(message), OUTPUT_TYPE.SYSTEM); //$NON-NLS-1$
			proxy.addLine(project, builder.command().stream().collect(Collectors.joining(" ")), OUTPUT_TYPE.SYSTEM); //$NON-NLS-1$
			proxy.addLine(project, DELIMITER_STRING, OUTPUT_TYPE.SYSTEM);

			Process process = builder.start();
			try (InputStream stderr = process.getErrorStream(); InputStream stdout = process.getInputStream()) {
				captureProcessOutput(monitor, message, process, stderr, stdout);
			}
		} catch (IOException e1) {
			builderLog.error("CMake process encountered an error");
		} catch (CoreException e2) {
			builderLog.error("Error while refreshing project");
		}
	}

	private void captureProcessOutput(IProgressMonitor monitor, String message, Process process, InputStream stderr,
			InputStream stdout) throws IOException, CoreException {

		try (BufferedReader stdOutReader = new BufferedReader(new InputStreamReader(stdout));
				BufferedReader stdErrReader = new BufferedReader(new InputStreamReader(stderr))) {
			long startTime = System.currentTimeMillis();
			while (process.isAlive()) {
				readOutput(stdOutReader, stdErrReader);
			}
			double finalTimeSeconds = (System.currentTimeMillis() - startTime) / 1000.0;
			readOutput(stdOutReader, stdErrReader);
			if (process.exitValue() != 0) {
				String errorLine = ERROR_OPERATION_MESSAGE.formatted(message, project.getProject().getName()) + "\n"; //$NON-NLS-1$
				builderLog.error(errorLine);
				sendFinalString(finalTimeSeconds, errorLine, true);
				monitor.setCanceled(true);
			} else {
				String successLine = SUCCESSFULL_OPERATION_MESSAGE.formatted(message, project.getProject().getName())
						+ "\n"; //$NON-NLS-1$
				builderLog.info(successLine);
				sendFinalString(finalTimeSeconds, successLine, false);
			}
			project.getProject().refreshLocal(IResource.DEPTH_INFINITE, monitor);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void sendFinalString(double finalTimeSeconds, String line, boolean isError) {
		proxy.addLine(project, DELIMITER_STRING, OUTPUT_TYPE.SYSTEM);
		if (isError) {
			proxy.addLine(project, line, OUTPUT_TYPE.ERROR);
		} else {
			proxy.addLine(project, line, OUTPUT_TYPE.SYSTEM);
		}
		proxy.addLine(project, "Заняло времени: %s с%n".formatted(finalTimeSeconds), OUTPUT_TYPE.SYSTEM);
		proxy.addLine(project, "Закончилось в: %s".formatted(LocalDateTime.now().format(formatter)),
				OUTPUT_TYPE.SYSTEM);
		proxy.addLine(project, DELIMITER_STRING, OUTPUT_TYPE.SYSTEM);
	}
}
