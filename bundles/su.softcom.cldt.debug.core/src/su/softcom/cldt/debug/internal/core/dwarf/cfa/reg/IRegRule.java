package su.softcom.cldt.debug.internal.core.dwarf.cfa.reg;

import java.math.BigInteger;

public interface IRegRule {

	/**
	 * The register columns contain rules that describe whether a given register has
	 * been saved and the rule to find the value for the register in the previous
	 * frame.
	 * 
	 * @param cfa значение CFA, нужен для некоторых rules. Может не использоваться.
	 * 
	 * @return value for the register in the previous frame.
	 */
	public long calculateValue(long cfa);

}
