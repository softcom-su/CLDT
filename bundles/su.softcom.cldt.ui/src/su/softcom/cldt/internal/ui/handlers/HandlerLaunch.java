package su.softcom.cldt.internal.ui.handlers;

import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfigurationType;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.core.ILaunchManager;

import su.softcom.cldt.nativebuild.launch.BuildLaunchConfigurationDelegate;

public class HandlerLaunch {

	public static final String LAUNCH_CONFIGURATION_TYPE_ID = "su.softcom.cldt.nativebuild.launchConfigurationType";
	public static final String SCRIPT_NOT_FOUND = "Cкрипт не найден или не указан";
	public static final String SCRIPT_NOT_LAUNCH = "Не удалость запустить скрипт";
	public static final String SCRIPT_NOT_IN_PLUGIN = "Скрипт должен находиться в плагине: ";

	/**
	 * @param script   script
	 * @param compiler compiler name
	 * @throws CoreException when cn't create core exception
	 */
	public void scriptLaunch(String script, String compiler) throws CoreException {

		if (script == null || script.equals("")) {
			Platform.getLog(getClass()).error(SCRIPT_NOT_FOUND);
			BuildLaunchConfigurationDelegate.newMarker(script, SCRIPT_NOT_FOUND);
			IStatus status = new Status(IStatus.ERROR, getClass(), SCRIPT_NOT_FOUND);
			throw new CoreException(status);
		}

		script = script.replaceAll("\\\\", "/");
		String project = ResourcesPlugin.getWorkspace().getRoot().getLocation().toPortableString();

		if (!script.startsWith(project)) {
			Platform.getLog(getClass()).error(SCRIPT_NOT_IN_PLUGIN + project);
			BuildLaunchConfigurationDelegate.newMarker(script, SCRIPT_NOT_IN_PLUGIN + project);
			IStatus status = new Status(IStatus.ERROR, getClass(), SCRIPT_NOT_FOUND);
			throw new CoreException(status);

		}

		script = script.replaceFirst(project, "");

		ILaunchConfigurationType configurationType = getLaunchManager()
				.getLaunchConfigurationType(LAUNCH_CONFIGURATION_TYPE_ID);
		String configurationName = getLaunchManager().generateLaunchConfigurationName(script);
		ILaunchConfigurationWorkingCopy copy;
		try {
			copy = configurationType.newInstance(null, configurationName);
			copy.setAttribute(BuildLaunchConfigurationDelegate.FILE_LOCATION, script);
			if (compiler != null && !compiler.equals("")) {
				copy.setAttribute(BuildLaunchConfigurationDelegate.COMPILER_LOCATION, compiler);
			}

			BuildLaunchConfigurationDelegate d = new BuildLaunchConfigurationDelegate();
			d.launch(copy, (String) null, (ILaunch) null, (IProgressMonitor) null);

		} catch (CoreException e) {
			throw e;
		}
	}

	private static ILaunchManager getLaunchManager() {
		return DebugPlugin.getDefault().getLaunchManager();
	}

}
