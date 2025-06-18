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
import su.softcom.cldt.debug.internal.core.dwarf.attr.Location;

public class Variable extends AbstractVariable implements IResolvableTag {

	/**
	 * 
	 * @param prototype
	 */
	public Variable(Tag prototype) {
		super(prototype);
	}

	/**
	 * 
	 * @param data
	 */
	public Variable(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_variable, data);
	}

	@Override
	public String getName() {
		return name;
	}

	Tag getCopy() {
		return new Variable(this);
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
					name = a.getValue().getValue().toString();
				} else if (a.name == DwarfConstants.DW_AT_type) {
					long offset = a.getValue().getUnsignedLongValue();
					if (cu instanceof CompileUnitImpl cuImpl) {
						type = cuImpl.getTagByOffset((int) offset);
					}
				} else if (a.name == DwarfConstants.DW_AT_location && a instanceof Location l) {
					location = l;
				} else if (a.name == DwarfConstants.DW_AT_decl_line) {
					line = a.getValue().getUnsignedIntValue();
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

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("DW_TAG_variable: ").append(name);
		if (type != null) {
			sb.append(" type: ").append(type.getName());
		}
		return sb.toString();
	}

	// TODO: Refactor
	@Override
	public long getAddress(GDBClient client, IFunction function, ICompileUnit cu) {
		if (location == null) {
			return 0;
		} else if (address == 0) {
			long result = location.getLocation(client, function, cu);
			if (result != 0) {
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

}
