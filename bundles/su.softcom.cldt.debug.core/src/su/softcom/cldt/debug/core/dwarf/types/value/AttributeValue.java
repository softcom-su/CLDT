package su.softcom.cldt.debug.core.dwarf.types.value;

import su.softcom.cldt.debug.core.dwarf.types.form.AttributeValueType;

public class AttributeValue {
	
	private AttributeValueType value;
	private String raw_value;
	private int offset;
	
	public AttributeValueType getValue() {
		return value;
	}
	public void setValue(AttributeValueType value) {
		this.value = value;
	}
	public String getRawValue() {
		return raw_value;
	}
	public void setRawValue(String raw_value) {
		this.raw_value = raw_value;
	}
	public int getOffset() {
		return offset;
	}
	public void setOffset(int offset) {
		this.offset = offset;
	}
}
