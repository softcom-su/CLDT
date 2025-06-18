package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;

public class SubrangeType extends Tag implements IType, IResolvableTag {

	Tag type;
	int count = 0;

	public SubrangeType(Tag prototype) {
		super(prototype);
	}

	public SubrangeType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_subrange_type, data);
	}

	@Override
	Tag getCopy() {
		return new SubrangeType(this);
	}

	@Override
	public long getId() {
		if (type != null) {
			// Может нужно представять по-другому так как это потомок массива
			return ((IType) type).getId();
		}
		return 0L;
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
				} else if (a.name == DwarfConstants.DW_AT_count) {
					count = a.getValue().getUnsignedIntValue();
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

	@Override
	public long getSize() {
		return count;
	}

	@Override
	public Object decodeValue(byte[] data) {
		return null;
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public ModelVariableType getModelVariableType() {
		return ((IType) type).getModelVariableType();
	}

	@Override
	public boolean hasChild() {
		return childrenOffsets.length > 0;
	}

	@Override
	public List<Tag> getTypeChildren() {
		return type != null ? ((IType) type).getTypeChildren() : new ArrayList<>();
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean supportNumericalRepresentations() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public String getOctRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getHexRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getBinRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

}
