package su.softcom.cldt.debug.internal.core.dwarf.cfa.reg;


public class RuleRegister implements IRegRule {
	
	private int register;
	
	public RuleRegister(int register) {
		this.register = register;
	}

	@Override
	public long calculateValue(long cfa) {
		return -1;
	}

}
