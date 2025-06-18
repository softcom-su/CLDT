package su.softcom.cldt.debug.internal.core.dwarf.attr;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IFunction;
import su.softcom.cldt.debug.internal.core.GDBClient;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.op.Operation;

public class Location extends Attribute {

	public Location(int form) {
		super(DwarfConstants.DW_AT_location, form);
	}
	
	@Override
	public Attribute copy() {
		Attribute result =  new Location(this.form);
		result.value = value;
		return result;
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder("DW_AT_location: form:").append(form);
		if(value != null) {
			sb.append(" value: ").append(value.toString());
		}
		return sb.toString();
	}
	
	/**
	 * Метод возвращает адрес, по которому находится значение тега
	 * @param client
	 * @param function
	 * @param cu
	 * @return адрес
	 */
	public long getLocation(GDBClient client, IFunction function, ICompileUnit cu) {
		if (value.getValue() instanceof byte[] array) {
			return Operation.evaluateExpression(client, client.getRegisters(), function, cu, array);
		}
		
		return 0;
	}

}