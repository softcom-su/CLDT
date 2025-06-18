package su.softcom.cldt.internal.ui.launch.handlers;

import org.eclipse.debug.ui.ILaunchShortcut;

import su.softcom.cldt.internal.ui.launch.ClangLaunchDelegate;
import su.softcom.cldt.ui.launch.handlers.BuildHandler;

/** Clang build command handler. */
public class ClangBuildHandler extends BuildHandler {

	@Override
	protected ILaunchShortcut getLaunchShortcut() {
		return new ClangLaunchDelegate();
	}

}
