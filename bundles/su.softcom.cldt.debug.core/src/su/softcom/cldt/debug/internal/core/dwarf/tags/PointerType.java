package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.ICompositeType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType.MVType;

public class PointerType extends Tag implements ICompositeType, IResolvableTag {

	Tag type;
	int typeOffset;
	long address = -1;

	public PointerType(Tag prototype) {
		super(prototype);
	}

	public PointerType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_pointer_type, data);
	}

	@Override
	public String getName() {
		if (type != null) {
			return type.getName().concat(" *");
		} else {
			return "void *";
		}
	}

	@Override
	public long getId() {
		if (type != null) {
			return ("ptr:" + type.getName()).hashCode();
		}
		// TODO: Доделать для случаев когда у нас нет атрибутов у указателя
		// в этом случае может быть ссылка на класс (посмотреть другие атрбиуты
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
					typeOffset = (int) offset;
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
		return new PointerType(this);
	}

	@Override
	public IType getType() {
		if (type instanceof IType iType) {
			return iType;
		}
		return null;
	}

	@Override
	public long getSize() {
		return cu.getAddressSize();
	}

	@Override
	public Object decodeValue(byte[] data) {
		// TODO: Посмотреть как представляются указатели, относительно тэгов
		if (type == null) {
			return null;
		}
		return ((IType) type).decodeValue(data);
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		if (data == null) {
			return "NULL"; //$NON-NLS-1$
		}
		int addressSize = cu.getAddressSize();
		String strValue = " * [0x%0".concat(Integer.toString(addressSize * 2)).concat("d]") //$NON-NLS-1$ //$NON-NLS-2$
				.formatted(Long.valueOf(new BigInteger(data).longValue()));
		if (type instanceof IType) {
			return type.getName().concat(strValue);
		}
		return "void".concat(strValue); //$NON-NLS-1$
	}

	@Override
	public ModelVariableType getModelVariableType() {
		if (type == null) {
			return new ModelVariableType(MVType.POINTER, "", null);
		}
		ModelVariableType child = ((IType) type).getModelVariableType();
		return new ModelVariableType(MVType.POINTER, "", child);
	}

	@Override
	public boolean hasChild() {
		return type != null;
	}

	@Override
	public List<Tag> getTypeChildren() {
		return type != null ? Collections.nCopies(1, type) : new ArrayList<>();
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		return data;
	}

	@Override
	public boolean supportNumericalRepresentations() {
		return false;
	}

	@Override
	public String getOctRepresentation(byte[] data) {
		throw new UnsupportedOperationException();
	}

	@Override
	public String getHexRepresentation(byte[] data) {
		throw new UnsupportedOperationException();
	}

	@Override
	public String getBinRepresentation(byte[] data) {
		throw new UnsupportedOperationException();
	}

	public long getAddress() {
		return address;
	}

	public void setAddress(long address) {
		this.address = address;
	}

}
