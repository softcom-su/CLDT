package su.softcom.cldt.internal.ui.launch.handlers;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Microsoft Visual C++ 2010 compiling command handler. */
public class MSVC2010Handler extends MSVCHandler {

	@Override
	protected String getCompilerPath() {
		return CInitializer.getPreferenceStore().getString(CInitializer.MSVC_2010);
	}

	@Override
	protected String getToolsetPath() {
		return CInitializer.getPreferenceStore().getString(CInitializer.TOOLSET_2010);
	}

}
