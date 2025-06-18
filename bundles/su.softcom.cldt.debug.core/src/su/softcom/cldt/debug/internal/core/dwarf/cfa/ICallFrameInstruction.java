package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;

public interface ICallFrameInstruction {

	void updateRow(TableRow row);

}
