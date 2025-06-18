package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.dwarf.values.Value;

public class Enumerator extends Tag implements IResolvableTag {

	long constValue = 0;
	String name;

	public Enumerator(Tag prototype) {
		super(prototype);
	}

	public Enumerator(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_enumerator, data);
	}

	@Override
	Tag getCopy() {
		return new Enumerator(this);
	}

	@Override
	public void resolve() {
		Attribute[] attrs = resolveArgs();
		super.resolve(attrs);
	}

	private Attribute[] resolveArgs() {
		List<Attribute> unresolvedAttributes = new ArrayList<>();
		try {
			for (Attribute a : args) {
				if (a.name == DwarfConstants.DW_AT_name) {
					Value val = a.getValue();
					if (val.getValue() != null) {
						name = a.getValue().getValue().toString();
					}
				} else if (a.name == DwarfConstants.DW_AT_const_value) {
					constValue = a.getValue().getUnsignedLongValue();
				} else {
					unresolvedAttributes.add(a);
				}

			}
		} catch (NullPointerException e) {
			// Some fields not set, skip it
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

}
