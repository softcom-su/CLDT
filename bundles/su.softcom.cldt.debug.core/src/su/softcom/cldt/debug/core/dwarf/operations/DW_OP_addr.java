package su.softcom.cldt.debug.core.dwarf.operations;

public class DW_OP_addr extends DwarfOperation {
	public static final int NUMBER = 0x03;
	
	public long getAddress() {
		long result = 0;
		Integer[] arr = operands.toArray(new Integer[operands.size()]);
		for(int i = 3; i >-1; i--) {
			result = (result << 8) + (arr[i] & 0xFF);
		}
		return result;
	}
	
}
