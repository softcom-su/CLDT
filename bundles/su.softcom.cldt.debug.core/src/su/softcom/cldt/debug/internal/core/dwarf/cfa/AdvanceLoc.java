package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;

public class AdvanceLoc extends ACreationRowCFA {

	int delta;
	int size;
	
	public AdvanceLoc(int lowBits, int size) {
		this.delta = lowBits;
		this.size = size;
	}

	@Override
	public long getAddress(TableRow row) {
		return row.getAddress() + delta;
	}

	@Override
	public void updateRow(TableRow row) {
		// empty
	}

}
