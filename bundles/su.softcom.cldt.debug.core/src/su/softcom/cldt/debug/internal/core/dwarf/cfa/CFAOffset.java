package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.CommonInfoEntry;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.reg.RuleOffset;

/**
 * The DW_CFA_offset instruction takes two operands: a register number (encoded
 * with the opcode) and an unsigned LEB128 constant representing a factored
 * offset. The required action is to change the rule for the register indicated
 * by the register number to be an offset(N) rule where the value of N is
 * factored offset * data_alignment_factor
 */
public class CFAOffset implements ICallFrameInstruction {

	byte register;
	long N;

	public CFAOffset(byte register, long offset, CommonInfoEntry cie) {
		this.register = register;
		this.N = offset * cie.getDataAlignmentFactor();
	}

	@Override
	public void updateRow(TableRow row) {
		row.setRule(register, new RuleOffset(N));
	}

}