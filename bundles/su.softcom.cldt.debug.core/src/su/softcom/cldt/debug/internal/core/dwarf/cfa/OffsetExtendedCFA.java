package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.CommonInfoEntry;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.reg.RuleOffset;

/**
 * The DW_CFA_offset_extended_sf instruction takes two operands: an unsigned
 * LEB128 value representing a register number and a signed LEB128 factored
 * offset. This instruction is identical to DW_CFA_offset_extended except that
 * the second operand is signed and factored. The resulting offset is
 * factored_offset * data_alignment_factor.
 */
public class OffsetExtendedCFA implements ICallFrameInstruction {

	long register;
	long N;

	public OffsetExtendedCFA(long register, long offset, CommonInfoEntry cie) {
		this.register = register;
		this.N = offset * cie.getDataAlignmentFactor();
	}

	@Override
	public void updateRow(TableRow row) {
		//TODO casting, мб сломается
		row.setRule((int) register, new RuleOffset(N));
	}

}