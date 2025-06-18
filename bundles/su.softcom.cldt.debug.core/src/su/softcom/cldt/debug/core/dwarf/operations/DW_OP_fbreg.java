package su.softcom.cldt.debug.core.dwarf.operations;

public class DW_OP_fbreg extends DwarfOperation {
	public static final int NUMBER = 0x91;
	
	public long getOffset() {
		return operands.get(0);
	}
	
}