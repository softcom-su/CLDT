package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IFunction;
import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.GDBClient;
import su.softcom.cldt.debug.internal.core.dwarf.AbstractVariable;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.dwarf.attr.AttributeWIC;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Location;
import su.softcom.cldt.debug.internal.core.dwarf.values.Value;

public class FormalParameter extends AbstractVariable implements IResolvableTag {

	public FormalParameter(Tag prototype) {
		super(prototype);
	}

	public FormalParameter(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_formal_parameter, data);
	}

	@Override
	Tag getCopy() {
		return new FormalParameter(this);
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
				if (a instanceof AttributeWIC attrWIC) {
					attrWIC.setValue(new Value(attrWIC.getForm(), Long.valueOf(attrWIC.getImplicitConst())));
				}
				if (a.name == DwarfConstants.DW_AT_name) {
					name = a.getValue().getValue().toString();
				} else if (a.name == DwarfConstants.DW_AT_type) {
					long offset = a.getValue().getUnsignedLongValue();
					if (cu instanceof CompileUnitImpl cuImpl) {
						type = cuImpl.getTagByOffset((int) offset);
					}
				} else if (a.name == DwarfConstants.DW_AT_location && a instanceof Location l) {
					location = l;
				} else if (a.name == DwarfConstants.DW_AT_decl_line) {
					line = (int) a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_abstract_origin) {
					offsetAbstractOrigin = (int) a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_decl_file) {
					fileIndex = (int) a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_artificial) {
					isArtificial = a.getValue().getUnsignedByteValue() == 1;
				} else {
					unresolvedAttributes.add(a);
				}

			}
		} catch (NullPointerException e) {
			// Some fields not set, skip it
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	@Override
	public IType getType() {
		if (type instanceof IType iType) {
			if (type instanceof ReferenceType t) {
				return t.getType();
			} else if (type instanceof RValueReferenceType t) {
				return t.getType();
			} else {
				return iType;
			}
		} else if (type == null && offsetAbstractOrigin != -1) {
			Tag reference = cu.getTagByOffset(offsetAbstractOrigin);
			if (reference instanceof Variable v) {
				return v.getType();
			}
		}
		return null;
	}

	@Override
	public long getAddress(GDBClient client, IFunction function, ICompileUnit cu) {
		if (location == null) {
			return 0;
		} else if (address == 0) {
			long result = location.getLocation(client, function, cu);
			if (result > 0) {
				address = result;
			}
			return address;
		} else {
			return address;
		}
	}

	@Override
	public long getSize() {
		if (type instanceof IType iType) {
			return iType.getSize();
		}
		return 0;
	}

	@Override
	public String getName() {
		return name;
	}

}
