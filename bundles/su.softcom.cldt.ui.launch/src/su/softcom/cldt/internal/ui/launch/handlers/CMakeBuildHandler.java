package su.softcom.cldt.internal.ui.launch.handlers;

import org.eclipse.debug.ui.ILaunchShortcut;

import su.softcom.cldt.internal.ui.launch.CMakeLaunchDelegate;
import su.softcom.cldt.ui.launch.handlers.BuildHandler;

/** CMake build command handler. */
public class CMakeBuildHandler extends BuildHandler {

	@Override
	protected ILaunchShortcut getLaunchShortcut() {
		return new CMakeLaunchDelegate();
	}

}
