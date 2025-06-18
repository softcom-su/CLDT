package su.softcom.cldt.internal.ui.launch;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationType;
import org.eclipse.debug.core.model.ILaunchConfigurationDelegate;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.debug.ui.ILaunchShortcut2;

import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.internal.core.runners.IntelRunner;
import su.softcom.cldt.internal.core.runners.WSLRunner;
import su.softcom.cldt.internal.ui.console.ClangConsoleFactory;
import su.softcom.cldt.ui.launch.AbstractLaunchDelegate;
import su.softcom.cldt.ui.launch.handlers.BuildHandler;

/** Windows Subsystem for Linux launch delegate. */
public class WSLLaunchDelegate extends AbstractLaunchDelegate
		implements ILaunchShortcut, ILaunchShortcut2, ILaunchConfigurationDelegate {

	private static final String LAUNCH_CONFIGURATION_TYPE = "su.softcom.cldt.ui.launch.wsl"; //$NON-NLS-1$

	@Override
	protected ILaunchConfigurationType getLaunchConfigurationType() {
		return DebugPlugin.getDefault().getLaunchManager().getLaunchConfigurationType(LAUNCH_CONFIGURATION_TYPE);
	}

	@Override
	protected void launch(final IResource resource, final ILaunchConfiguration configuration, final String mode,
			final ILaunch launch, final IProgressMonitor monitor) {
		final ClangConsoleFactory factory = new ClangConsoleFactory();
		final IRunner runner = BuildHandler.BUILD_MODE.equals(mode) ? new IntelRunner() : new WSLRunner();
		runner.setConsole(factory.getConsole());

		runner.addResource(resource);
		runner.start();
	}

	@Override
	protected IResource getLaunchableResource(final IResource resource) {
		return resource instanceof IFile ? resource : super.getLaunchableResource(resource);
	}

}
