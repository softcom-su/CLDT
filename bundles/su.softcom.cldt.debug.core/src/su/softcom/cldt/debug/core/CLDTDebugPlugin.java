package su.softcom.cldt.debug.core;

import org.eclipse.core.runtime.Plugin;

public class CLDTDebugPlugin extends Plugin {
	
	static CLDTDebugPlugin instance;

	public static CLDTDebugPlugin getDefault() {
		if(instance == null) {
			instance = new CLDTDebugPlugin();
		}
		return instance;
	}

}
