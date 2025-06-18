package su.softcom.cldt.debug.core.dwarf.types.form;

public class DW_FORM_flag implements AttributeValueType {
	
	boolean value;
	
	public boolean getValue() {
		return value;
	}

	@Override
	public void setValue(String value) {
		this.value = value.equals("True");
	}
}
