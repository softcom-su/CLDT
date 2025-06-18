package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.CFARule;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;

public class DefCFA extends ADefCFA {

	private long register;
	private long offset;
	
	public DefCFA(long register, long offset) {
		this.register = register;
		this.offset = offset;
	}

	public void updateRow(TableRow row) {
		CFARule rule = row.getCFARule();
		rule.setReg(register);
		rule.setOffset(offset);
	}

}
