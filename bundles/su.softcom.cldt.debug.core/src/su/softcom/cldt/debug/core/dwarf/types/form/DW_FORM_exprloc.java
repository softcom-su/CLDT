package su.softcom.cldt.debug.core.dwarf.types.form;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import su.softcom.cldt.debug.core.dwarf.operations.DwarfOperation;

public class DW_FORM_exprloc implements AttributeValueType {
	
	List<Integer> value = new ArrayList<Integer>();
	DwarfOperation operation;
	
	public DwarfOperation getOperation() {
		return operation;
	}
	
	public void setOperation() {
		operation = DwarfOperation.getOperation(value);
	}
	
	public List<Integer> getValue() {
		return value;
	}

	public void addItemToValue(int item) {
		this.value.add(item);
	}
	
	@Override
	public void setValue(String value) {
		value = value.substring(1, value.length() - 1);
		List<String> valueStringList = new ArrayList<String>(Arrays.asList(value.split(", ")));
		for (String item : valueStringList) {
			try{
	            addItemToValue(Integer.parseInt(item));
	        }
	        catch (NumberFormatException e){
	            e.printStackTrace();
	        }
		}
		setOperation();
	}
}
