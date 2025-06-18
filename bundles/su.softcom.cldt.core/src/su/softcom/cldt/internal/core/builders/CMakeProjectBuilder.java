package su.softcom.cldt.internal.core.builders;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;

/**
 * Запускается build с уже сгенерированными файлами с первого этапа в build
 * директории.
 */
public class CMakeProjectBuilder extends CMakeBuilder {

	public static final String ID = "su.softcom.cldt.core.builder.build"; //$NON-NLS-1$

	private static final String MESSAGE_STRING = "Сборка";

	@Override
	protected IProject[] build(int kind, Map<String, String> args, IProgressMonitor monitor) throws CoreException {
		List<String> commands = new ArrayList<>();
		commands.add("--build"); //$NON-NLS-1$
		commands.add(getBuildFolder().getLocation().toOSString());
		if (getConfigurationStatus()) {
			startCMakeProcess(commands, monitor, MESSAGE_STRING);
		} else {
			builderLog.info("Сборка не начата, т.к. конфигурация завершилась ошибкой");
		}
		getCMakeProject().setBuildStatus(!monitor.isCanceled());
		return new IProject[] { getProject() };
	}

}
