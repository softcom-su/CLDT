package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import su.softcom.cldt.debug.core.IDeclaration;
import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType.MVType;

public class ArrayType extends Tag implements IType, IResolvableTag, IDeclaration {

	private boolean isDeclaration = false;
	Tag type;
	int typeOffset;

	public ArrayType(Tag prototype) {
		super(prototype);
	}

	public ArrayType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_array_type, data);
	}

	Tag getCopy() {
		return new ArrayType(this);
	}

	@Override
	public long getId() {
		if (type != null) {
			// Перепроверить
			return ("arr:" + type.getName()).hashCode();
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
					typeOffset = (int) offset;
				} else if (a.name == DwarfConstants.DW_AT_declaration) {
					isDeclaration = a.getValue().getUnsignedByteValue() == 1;
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
	public String toString() {
		return "DW_TAG_array_type"; //$NON-NLS-1$
	}

	@Override
	public String getName() {
		return "Массив: " + type.getName();
	}

	@Override
	public long getSize() {
		if (type instanceof IType) {
			return ((IType) type).getSize() * getLength();
		}
		return 0;
	}

	/**
	 * @return длину массива
	 */
	public int getLength() {
		for (Integer childOffset : childrenOffsets) {
			Tag t = cu.getTagByOffset(childOffset);
			if (t != null && t instanceof SubrangeType subrangeType) {
				return (int) subrangeType.getSize();
			}
		}
		// точно ли нужно возвращать единицу?
		return 0;
	}

	@Override
	public Object decodeValue(byte[] data) {
		return null;
	}

	@Override
	public String getStringRepresentation(byte[] data) {
//		return "Массив %s[%d]".formatted(((IType) type).getName(), Integer.valueOf(getLength()));
		return null;
	}

	@Override
	public ModelVariableType getModelVariableType() {
		ModelVariableType child = null;
		for (Integer childOffset : childrenOffsets) {
			Tag t = cu.getTagByOffset(childOffset);
			if (t != null && t instanceof SubrangeType subrangeType) {
				child = subrangeType.getModelVariableType();
			}
		}
		return new ModelVariableType(MVType.ARRAY, "", child);
	}

	@Override
	public boolean hasChild() {
		return getLength() > 0;
	}

	public List<Tag> getTypeChildren() {
//		Collections.nCopies(getLength(), typeOffset).toArray();
		return Collections.nCopies(getLength(), type);
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		int index = 0;
		try {
			index = Integer.decode(name.substring(1, name.length() - 1));
		} catch (NumberFormatException e) {
			return new byte[0];
		}

		return Arrays.copyOfRange(data, (int) ((IType) type).getSize() * index,
				(int) ((IType) type).getSize() * (index + 1));
	}

	@Override
	public boolean supportNumericalRepresentations() {
		// TODO Auto-generated method stub
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

	@Override
	public boolean isDeclaration() {
		return isDeclaration;
	}

}
