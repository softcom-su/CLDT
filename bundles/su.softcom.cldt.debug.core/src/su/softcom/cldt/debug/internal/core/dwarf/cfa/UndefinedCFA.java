package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.reg.IRegRule;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.reg.RuleUndefined;

public class UndefinedCFA implements ICallFrameInstruction {

	private int register;

	public UndefinedCFA(long register) {
		this.register = (int) register;	
	}

	@Override
	public void updateRow(TableRow row) {
		IRegRule undeffRule = new RuleUndefined();
		row.setRule(register, undeffRule);
	}

}
