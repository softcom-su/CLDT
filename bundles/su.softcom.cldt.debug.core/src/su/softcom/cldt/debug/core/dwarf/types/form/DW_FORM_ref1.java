package su.softcom.cldt.debug.core.dwarf.types.form;

public class DW_FORM_ref1 implements AttributeValueType {

	private int value;
	public int getValue() {
		return value;
	}

	@Override
	public void setValue(String value) {
		try {
			this.value = Integer.valueOf(value);
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}
}
