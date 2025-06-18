package su.softcom.cldt.debug.core.dwarf.types.value;

public class DW_AT_byte_size extends AttributeValue {
	
	public int getSize() {
		return Integer.parseInt(getRawValue());
	}

}
