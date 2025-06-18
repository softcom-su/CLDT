package su.softcom.cldt.internal.ui.launch.handlers;

import org.eclipse.debug.ui.ILaunchShortcut;

import su.softcom.cldt.internal.ui.launch.WSLLaunchDelegate;
import su.softcom.cldt.ui.launch.handlers.BuildHandler;

/** Intel optimize command handler. */
public class IntelOptHandler extends BuildHandler {

	@Override
	protected ILaunchShortcut getLaunchShortcut() {
		return new WSLLaunchDelegate();
	}

}
