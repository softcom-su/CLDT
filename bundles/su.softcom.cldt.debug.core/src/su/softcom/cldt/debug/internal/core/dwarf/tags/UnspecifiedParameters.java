package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;

public class UnspecifiedParameters extends Tag implements IResolvableTag {

	int fileIndex = -1;
	int line = -1;

	public UnspecifiedParameters(Tag prototype) {
		super(prototype);
	}

	public UnspecifiedParameters(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_unspecified_parameters, data);
	}

	@Override
	Tag getCopy() {
		return new UnspecifiedParameters(this);
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
				if (a.name == DwarfConstants.DW_AT_decl_file) {
					fileIndex = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_decl_line) {
					line = a.getValue().getUnsignedIntValue();
				} else {
					unresolvedAttributes.add(a);
				}
			}
		} catch (NullPointerException e) {
			// Not parsed yet
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

}
