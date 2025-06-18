package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Encoding;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType.MVType;

public class BaseType extends Tag implements IType, IResolvableTag {

	String name;
	long byteSize;
	int bitSize;
	Encoding encoding;

	public BaseType(Tag prototype) {
		super(prototype);
	}

	public BaseType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_base_type, data);
	}

	@Override
	Tag getCopy() {
		return new BaseType(this);
	}

	@Override
	public long getId() {
		return name != null ? name.hashCode() : 0;
	}

	@Override
	public void resolve() {
		Attribute[] attrs = resolveArgs();
		super.resolve(attrs);
	}

	private Attribute[] resolveArgs() {
		List<Attribute> unresolvedAttributes = new ArrayList<>();
		for (Attribute a : args) {
			if (a.name == DwarfConstants.DW_AT_byte_size) {
				byteSize = a.getValue().getUnsignedLongValue();
			} else if (a.name == DwarfConstants.DW_AT_bit_size) {
				bitSize = a.getValue().getUnsignedIntValue();
			} else if (a.name == DwarfConstants.DW_AT_encoding) {
				encoding = (Encoding) a;
			} else if (a.name == DwarfConstants.DW_AT_name) {
				name = a.getValue().getObject().toString();
			} else {
				unresolvedAttributes.add(a);
			}
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	@Override
	public String getName() {
		return name;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("DW_TAG_base_type: ").append(name).append(" size: ").append(byteSize);
		return sb.toString();
	}

	@Override
	public long getSize() {
		return byteSize;
	}

	@Override
	public Object decodeValue(byte[] data) {
		return encoding.decode(data);
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		try {
			return String.valueOf(encoding.decode(data));
		} catch (NullPointerException e) {
			return ""; // FIXME
		}
	}

	@Override
	public ModelVariableType getModelVariableType() {
		MVType mvType;
		switch (encoding.getEncoding()) {
		case DwarfConstants.DW_ATE_signed, DwarfConstants.DW_ATE_unsigned:
			mvType = MVType.INTEGER;
			break;
		case DwarfConstants.DW_ATE_signed_char, DwarfConstants.DW_ATE_unsigned_char:
			mvType = MVType.CHARACTER;
			break;
		case DwarfConstants.DW_ATE_float:
			mvType = MVType.FLOAT;
			break;
		case DwarfConstants.DW_ATE_boolean:
			mvType = MVType.BOOLEAN;
			break;
		default:
			throw new IllegalArgumentException("Не поддержан базовый тип: " + encoding.getEncoding());
		}

		return new ModelVariableType(mvType, name, null);
	}

	@Override
	public boolean hasChild() {
		return false;
	}

	@Override
	public List<Tag> getTypeChildren() {
		return null;
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		return null;
	}

	@Override
	public boolean supportNumericalRepresentations() {
		return true;
	}

	@Override
	public String getOctRepresentation(byte[] data) {
		return Long.toOctalString(new BigInteger((String) encoding.decode(data)).longValue());
	}

	@Override
	public String getHexRepresentation(byte[] data) {
		return Long.toHexString(new BigInteger((String) encoding.decode(data)).longValue());
	}

	@Override
	public String getBinRepresentation(byte[] data) {
		return Long.toBinaryString(new BigInteger((String) encoding.decode(data)).longValue());
	}

}
