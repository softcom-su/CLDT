package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.reg.RuleRegister;

public class CFARegister implements ICallFrameInstruction {

	int register1;
	int register2;
	
	public CFARegister(long register1, long register2) {
		this.register1 = (int) register1;
		this.register2 = (int) register2;
	}
	
	public void updateRow(TableRow row) {
		row.setRule(register1, new RuleRegister(register2));
	}

}
