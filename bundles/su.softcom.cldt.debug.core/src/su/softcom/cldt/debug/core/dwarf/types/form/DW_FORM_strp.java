package su.softcom.cldt.debug.core.dwarf.types.form;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class DW_FORM_strp implements AttributeValueType {

	private List<String> value = new ArrayList<String>();

	public List<String> getValue() {
		return value;
	}

	public void addItemToValue(String item) {
		this.value.add(item);
	}
	
	@Override	
	public void setValue(String value) {
		value = value.substring(2, value.length() - 1);
		this.value = new ArrayList<String>(Arrays.asList(value.split(" ")));
	}
}
