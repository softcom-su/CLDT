package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;

public class DefCFAOffset extends ADefCFA {
	
	long offset;

	public DefCFAOffset(long offset) {
		this.offset = offset;
	}

	@Override
	public void updateRow(TableRow row) {
		row.getCFARule().setOffset(offset);
	}
}
