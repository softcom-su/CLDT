package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;

public class DefCFARegister extends ADefCFA {
	
	long register;

	public DefCFARegister(long register) {
		this.register = register;
	}

	@Override
	public void updateRow(TableRow row) {
		row.getCFARule().setReg(register);
	}

}
