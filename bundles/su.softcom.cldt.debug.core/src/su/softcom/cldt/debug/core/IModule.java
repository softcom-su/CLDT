package su.softcom.cldt.debug.core;

import su.softcom.cldt.debug.core.info.IDebugInfo;

public interface IModule {

	public IDebugInfo getDebugInfo();
	
	public long getBaseAddress();
	
}
