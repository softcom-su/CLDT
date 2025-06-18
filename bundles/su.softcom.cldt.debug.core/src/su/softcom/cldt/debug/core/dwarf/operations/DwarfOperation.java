package su.softcom.cldt.debug.core.dwarf.operations;

import java.util.ArrayList;
import java.util.List;

public class DwarfOperation {

	public static DwarfOperation getOperation(List<Integer> rawValue) {
		DwarfOperation operation = null;
		switch (rawValue.get(0)) {
		case DW_OP_addr.NUMBER:
			operation = new DW_OP_addr();
			break;
		case DW_OP_fbreg.NUMBER:
			operation = new DW_OP_fbreg();
			break;
		case DW_OP_call_frame_cfa.NUMBER:
			operation = new DW_OP_call_frame_cfa();
			break;
		default:
			return null;
		}
		if (rawValue.size() != 1) {
			operation.setOperands(rawValue.subList(1, rawValue.size()));
		}
		return operation;
	}

	protected List<Integer> operands = new ArrayList<Integer>();

	public List<Integer> getOperands() {
		return operands;
	}

	public void addOperand(Integer operand) {
		this.operands.add(operand);
	}

	public void setOperands(List<Integer> operands) {
		this.operands = operands;
	}
}
