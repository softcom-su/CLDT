package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.ICompositeType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;

public class VolatileType extends Tag implements ICompositeType, IResolvableTag {

	Tag type;

	public VolatileType(Tag prototype) {
		super(prototype);
	}

	public VolatileType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_volatile_type, data);
	}

	@Override
	public String getName() {
		return ((IType) type).getName();
	}

	@Override
	public long getId() {
		if (type != null) {
			return ((IType) type).getId();
		}
		return 0L;
	}

	@Override
	public IType getType() {
		if (type instanceof IType iType) {
			return iType;
		}
		return null;
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
					if (cu instanceof CompileUnitImpl cuImpl) {
						type = cuImpl.getTagByOffset((int) offset);
					}
				} else {
					unresolvedAttributes.add(a);
				}
			}
		} catch (NullPointerException e) {
			// Not parsed yet
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	@Override
	Tag getCopy() {
		return new VolatileType(this);
	}

	@Override
	public long getSize() {
		if (type instanceof IType) {
			return ((IType) type).getSize();
		}
		return 0;
	}

	@Override
	public Object decodeValue(byte[] data) {
		return ((IType) type).decodeValue(data);
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		if (type instanceof IType) {
			return ((IType) type).getStringRepresentation(data);
		}
		return "";
	}

	@Override
	public ModelVariableType getModelVariableType() {
		return ((IType) type).getModelVariableType();
	}

	@Override
	public boolean hasChild() {
		if (type instanceof IType iType) {
			return iType.hasChild();
		}
		return false;
	}

	@Override
	public List<Tag> getTypeChildren() {
		return ((IType) type).getTypeChildren();
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		return ((IType) type).getChildData(name, data);
	}

	@Override
	public boolean supportNumericalRepresentations() {
		return ((IType) type).supportNumericalRepresentations();
	}

	@Override
	public String getOctRepresentation(byte[] data) {
		return ((IType) type).getOctRepresentation(data);
	}

	@Override
	public String getHexRepresentation(byte[] data) {
		return ((IType) type).getHexRepresentation(data);
	}

	@Override
	public String getBinRepresentation(byte[] data) {
		return ((IType) type).getBinRepresentation(data);
	}

}
