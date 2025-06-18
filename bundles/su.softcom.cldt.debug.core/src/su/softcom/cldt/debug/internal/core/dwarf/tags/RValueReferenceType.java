package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.ICompositeType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType.MVType;

public class RValueReferenceType extends Tag implements ICompositeType, IResolvableTag {

	Tag type;

	public RValueReferenceType(Tag prototype) {
		super(prototype);
	}

	public RValueReferenceType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_rvalue_reference_type, data);
	}

	@Override
	Tag getCopy() {
		return new RValueReferenceType(this);
	}

	@Override
	public long getId() {
		if (type != null) {
			return ("rvalref:" + ((IType) type).getId()).hashCode();
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

	public IType getType() {
		if (type instanceof IType itype) {
			return itype;
		}
		return null;
	}

	@Override
	public long getSize() {
		return type instanceof IType t ? t.getSize() : 0;
	}

	@Override
	public Object decodeValue(byte[] data) {
		// TODO: Посмотреть как представляются ссылки, относительно тэгов
		return ((IType) type).decodeValue(data);
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		return null;
	}

	@Override
	public ModelVariableType getModelVariableType() {
		ModelVariableType child = ((IType) type).getModelVariableType();
		return new ModelVariableType(MVType.REFERENCE, "", child);
	}

	@Override
	public boolean hasChild() {
		return false;
	}

	@Override
	public List<Tag> getTypeChildren() {
		return type != null ? Collections.nCopies(1, type) : new ArrayList<>();
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
