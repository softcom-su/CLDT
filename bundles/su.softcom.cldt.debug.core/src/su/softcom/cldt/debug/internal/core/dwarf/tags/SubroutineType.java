package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.ICompositeType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType.MVType;

public class SubroutineType extends Tag implements ICompositeType, IResolvableTag {

	Tag type;

	public SubroutineType(Tag prototype) {
		super(prototype);
	}

	public SubroutineType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_subroutine_type, data);
	}

	@Override
	Tag getCopy() {
		return new SubroutineType(this);
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
	public long getId() {
		// TODO: исправить на нормальный айди
		return ((IType) type).getId();
	}

	@Override
	public long getSize() {
		return ((IType) type).getSize();
	}

	@Override
	public ModelVariableType getModelVariableType() {
		// TODO: Может вынести в отдельный тип
		return type != null ? ((IType) type).getModelVariableType() : new ModelVariableType(MVType.EMPTY, "", null);
	}

	@Override
	public Object decodeValue(byte[] data) {
		// TODO Auto-generated method stub

		return null;
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean hasChild() {
		return ((IType) type).hasChild();
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
