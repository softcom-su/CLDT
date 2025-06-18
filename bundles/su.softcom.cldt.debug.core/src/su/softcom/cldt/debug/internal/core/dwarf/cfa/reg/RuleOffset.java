package su.softcom.cldt.debug.internal.core.dwarf.cfa.reg;


public class RuleOffset implements IRegRule {
	
	long N;
	
	public RuleOffset(long N) {
		this.N = N;
	}
	
	public long getN() {
		return N;
	}
	
	
	@Override
	public long calculateValue(long cfa) {
		return cfa + N;
	}

}

