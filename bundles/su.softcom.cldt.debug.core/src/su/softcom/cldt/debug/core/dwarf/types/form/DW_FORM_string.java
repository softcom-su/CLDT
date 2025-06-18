package su.softcom.cldt.debug.core.dwarf.types.form;

public class DW_FORM_string implements AttributeValueType {

	private String value;

	public String getValue() {
		return value;
	}

	@Override
	public void setValue(String value) {
		value = value.substring(2, value.length() - 1);
		this.value = value;
	}
}
