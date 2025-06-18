package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;

public class Inheritance extends Tag implements IResolvableTag {

	Tag type;
	int fileIndex = -1;
	long memberLocation;
	int line = -1;

	public Inheritance(Tag prototype) {
		super(prototype);
	}

	public Inheritance(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_inheritance, data);
	}

	@Override
	Tag getCopy() {
		return new Inheritance(this);
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
				if (a.name == DwarfConstants.DW_AT_type) {
					long offset = a.getValue().getUnsignedLongValue();
					type = cu.getTagByOffset((int) offset);
				} else if (a.name == DwarfConstants.DW_AT_decl_file) {
					fileIndex = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_decl_line) {
					line = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_data_member_location) {
					memberLocation = a.getValue().getUnsignedLongValue();
				} else {
					unresolvedAttributes.add(a);
				}
			}
		} catch (NullPointerException e) {
			// Not parsed yet
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	public IType getType() {
		if (type instanceof IType iType) {
			return iType;
		}
		return null;
	}

	public long getMemberLocation() {
		return memberLocation;
	}

}
