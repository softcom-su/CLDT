package su.softcom.cldt.debug.internal.core.dwarf.attr;

import java.nio.ByteOrder;

import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.values.Value;

/**
 * Base class for implementation DWARF attributes
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public class Attribute{

	public final int name;
	final int form;
	protected Value value;
	
	Attribute(int name, int form) {
		this.name = name;
		this.form = form;
	}
	
	public Value getValue() {
		return value;
	}
	
	public int getName() {
		return name;
	}

	public int getForm() {
		return form;
	}

	public Attribute setValue(Value value) {
		this.value = value;
		return this;
	}
	
	/** Returns copy of this object with same fields
	 * @return
	 */
	public Attribute copy() {
		Attribute result =  new Attribute(this.name, this.form);
		result.value = this.value;
		return result;
	}
	
	public static Attribute getAttribute(int name, int form, ByteOrder order) {
		switch (name) {
		case DwarfConstants.DW_AT_location:
			return new Location(form);
		case DwarfConstants.DW_AT_encoding:
			return new Encoding(form, order);
		default:
			return new Attribute(name, form);
		}
	}
}
