package su.softcom.cldt.internal.ui.launch;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IResource;

import su.softcom.cldt.ui.launch.AbstractLaunchShortcut;

/** Bash script launch shortcut. */
public class BashLaunchShortcut extends AbstractLaunchShortcut {

	@Override
	protected String getLaunchConfigurationType() {
		return BashLaunchConfigurationDelegate.LAUNCH_CONFIGURATION_TYPE;
	}

	@Override
	protected IResource getLaunchableResource(IResource resource) {
		return resource instanceof IFile && "sh".equals(resource.getFileExtension()) ? resource : resource.getProject(); //$NON-NLS-1$
	}

}
