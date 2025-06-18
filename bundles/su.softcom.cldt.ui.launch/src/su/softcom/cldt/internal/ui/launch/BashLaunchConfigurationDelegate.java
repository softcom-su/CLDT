package su.softcom.cldt.internal.ui.launch;

import java.util.Map;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.ui.console.IOConsole;

import su.softcom.cldt.core.operations.BashOperation;
import su.softcom.cldt.internal.ui.console.ClangConsoleFactory;
import su.softcom.cldt.ui.launch.AbstractLaunchConfigurationDelegate;
import su.softcom.cldt.ui.launch.tabs.BashMainTab;
import su.softcom.cldt.ui.launch.tabs.MainTab;

/** Bash script launch configuration delegate. */
public class BashLaunchConfigurationDelegate extends AbstractLaunchConfigurationDelegate {

	public static final String LAUNCH_CONFIGURATION_TYPE = "su.softcom.cldt.ui.launch.bash"; //$NON-NLS-1$

	@Override
	public void launch(ILaunchConfiguration configuration, String mode, ILaunch launch, IProgressMonitor monitor)
			throws CoreException {
		IResource resource = getLaunchableResource(configuration);
		if (!(resource instanceof IFile)) {
			return;
		}

		IOConsole console = new ClangConsoleFactory().getConsole();
		console.clearConsole();

		Job script = Job.create("Run " + resource.getName(), new ICoreRunnable() {
			@Override
			public void run(IProgressMonitor subMonitor) throws CoreException {
				IFolder folder = resource.getProject().getFolder(BashOperation.DEFAULT_LAUNCH_FOLDER);
				if (!folder.exists()) {
					folder.create(false, true, subMonitor);
				}

				new BashOperation((IFile) resource,
						Map.ofEntries(Map.entry(BashOperation.BASH_LAUNCH_FOLDER, BashOperation.DEFAULT_LAUNCH_FOLDER),
								Map.entry(BashOperation.BASH_ARGS, configuration.getAttribute(MainTab.ARGUMENTS, ""))), //$NON-NLS-1$
						console).run(subMonitor);

				folder.refreshLocal(IResource.DEPTH_INFINITE, subMonitor);
			}
		});

		script.setProgressGroup(monitor, IProgressMonitor.UNKNOWN);
		script.schedule();
	}

	@Override
	protected IResource setLaunchableResource(ILaunchConfiguration configuration, String mode) {
		try {
			IResource resource = getBuildableResource(configuration);
			if (resource instanceof IProject) {
				return BashMainTab.getScript(configuration.getAttribute(BashMainTab.SCRIPT_URI, "")); //$NON-NLS-1$

			}

			ILaunchConfigurationWorkingCopy copy = configuration.getWorkingCopy();
			copy.setAttribute(BashMainTab.SCRIPT_URI, resource.getLocationURI().toString());
			copy.doSave();

			return resource;
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.toString());
		}

		return null;
	}

}
