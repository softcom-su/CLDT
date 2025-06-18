package su.softcom.cldt.internal.ui.launch.handlers;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Microsoft Visual C++ 2013 compiling command handler. */
public class MSVC2013Handler extends MSVCHandler {

	@Override
	protected String getCompilerPath() {
		return CInitializer.getPreferenceStore().getString(CInitializer.MSVC_2013);
	}

	@Override
	protected String getToolsetPath() {
		return CInitializer.getPreferenceStore().getString(CInitializer.TOOLSET_2013);
	}

}
