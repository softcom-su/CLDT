package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;

public abstract class AStateCFA implements ICallFrameInstruction {

	@Override
	public void updateRow(TableRow row) {
		// empty
	}
	
}
