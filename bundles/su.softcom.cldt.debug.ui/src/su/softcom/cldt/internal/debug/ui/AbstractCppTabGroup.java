package su.softcom.cldt.internal.debug.ui;

import org.eclipse.debug.ui.AbstractLaunchConfigurationTabGroup;
import org.eclipse.debug.ui.CommonTab;
import org.eclipse.debug.ui.EnvironmentTab;
import org.eclipse.debug.ui.ILaunchConfigurationDialog;

public class AbstractCppTabGroup extends AbstractLaunchConfigurationTabGroup {

	public AbstractCppTabGroup() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public void createTabs(ILaunchConfigurationDialog dialog, String mode) {
		setTabs(new DebugLaunchMainTab(mode), new EnvironmentTab(), new CommonTab());
	}

}
