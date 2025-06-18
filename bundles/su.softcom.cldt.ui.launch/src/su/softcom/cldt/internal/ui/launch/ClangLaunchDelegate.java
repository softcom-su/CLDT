package su.softcom.cldt.internal.ui.launch;

import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationType;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.core.model.ILaunchConfigurationDelegate;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.debug.ui.ILaunchShortcut2;

import su.softcom.cldt.core.runners.ExeRunner;
import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.internal.core.runners.ClangCompilerRunner;
import su.softcom.cldt.internal.ui.console.ClangConsoleFactory;
import su.softcom.cldt.ui.launch.AbstractLaunchDelegate;
import su.softcom.cldt.ui.launch.handlers.BuildHandler;

/** Clang launch delegate. */
public class ClangLaunchDelegate extends AbstractLaunchDelegate
		implements ILaunchShortcut, ILaunchShortcut2, ILaunchConfigurationDelegate {

	private static final String LAUNCH_CONFIGURATION_TYPE = "su.softcom.cldt.ui.launch.clang"; //$NON-NLS-1$

	@Override
	protected ILaunchConfigurationType getLaunchConfigurationType() {
		return DebugPlugin.getDefault().getLaunchManager().getLaunchConfigurationType(LAUNCH_CONFIGURATION_TYPE);
	}

	@Override
	protected void launch(final IResource resource, final ILaunchConfiguration configuration, final String mode,
			final ILaunch launch, final IProgressMonitor monitor) {
		final ClangConsoleFactory factory = new ClangConsoleFactory();
		final IRunner compiler = new ClangCompilerRunner(factory.getConsole());
		compiler.addResource(resource);

		if (ILaunchManager.DEBUG_MODE.equals(mode)) {
			compiler.addArg("-g"); //$NON-NLS-1$
			compiler.addArg("-O0"); //$NON-NLS-1$
		}

		if (!BuildHandler.BUILD_MODE.equals(mode)) {
			final IRunner executor = new ExeRunner();
			compiler.next(executor);
		}

		compiler.start();
	}

}
