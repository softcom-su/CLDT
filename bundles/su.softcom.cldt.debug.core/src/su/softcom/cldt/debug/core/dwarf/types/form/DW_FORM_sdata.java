package su.softcom.cldt.debug.core.dwarf.types.form;

public class DW_FORM_sdata implements AttributeValueType {
	
	private long value;

	public long getValue() {
		return value;
	}

	@Override
	public void setValue(String value) {
		try {
			this.value = Long.valueOf(value);
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}
}
