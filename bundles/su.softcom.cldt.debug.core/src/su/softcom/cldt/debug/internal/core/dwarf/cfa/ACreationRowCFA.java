package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo;



public abstract class ACreationRowCFA implements ICallFrameInstruction {
	
	public abstract long getAddress(CallFrameInfo.TableRow row);
}

