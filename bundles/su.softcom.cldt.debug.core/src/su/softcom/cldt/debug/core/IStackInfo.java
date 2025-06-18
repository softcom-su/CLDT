package su.softcom.cldt.debug.core;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;

public interface IStackInfo {
	
	public long getCFAoffset(long address);
		
	public TableRow getRow(long address);

	public int getReturnAddressRegister(long address);
}
