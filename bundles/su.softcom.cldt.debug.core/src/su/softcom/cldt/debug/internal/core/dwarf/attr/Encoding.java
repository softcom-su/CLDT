package su.softcom.cldt.debug.internal.core.dwarf.attr;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.values.Value;

public class Encoding extends Attribute {

	short encoding;
	ByteOrder order;

	/**
	 * Конструктор для декодирования значения по типу
	 * 
	 * @param form
	 * @param order
	 */
	public Encoding(int form, ByteOrder order) {
		super(DwarfConstants.DW_AT_encoding, form);
		this.order = order;
	}

	@Override
	public Attribute copy() {
		Attribute result = new Encoding(this.form, this.order);
		result.value = value;
		return result;
	}

	@Override
	public Attribute setValue(Value v) {
		super.setValue(v);
		encoding = Long.valueOf(v.getValue().toString()).shortValue();
		return this;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder("DW_AT_location: form:").append(form);
		if (value != null) {
			sb.append(" value: ").append(value.toString());
		}
		return sb.toString();
	}

	/**
	 * @return код базового типа
	 */
	public int getEncoding() {
		return encoding;
	}

	/**
	 * @param data
	 * @return строковое предстfвление
	 */
	public Object decode(byte[] data) {
		String result = "";
		switch (encoding) {
		case DwarfConstants.DW_ATE_boolean:
			return Byte.valueOf(data[0]);
		case DwarfConstants.DW_ATE_signed:
			return readSigned(data);
		case DwarfConstants.DW_ATE_unsigned:
			return readUnsigned(data);
		case DwarfConstants.DW_ATE_signed_char:
			return readSigned(data);
		case DwarfConstants.DW_ATE_unsigned_char:
			return readUnsigned(data);
		case DwarfConstants.DW_ATE_float:
			if (data.length == 4) {
				return readFloat(data);
			} else {
				return readDouble(data);
			}
		default:
			break;
		}
		return result;
	}

	private Object readSigned(byte[] data) {
		if (data == null) {
			return "";
		}
		ByteBuffer buffer = ByteBuffer.wrap(data).order(order);
		switch (data.length) {
		case 1:
			return Byte.valueOf(data[0]);
		case 2:
			return buffer.getShort();
		case 4:
			return buffer.getInt();
		case 8:
			return buffer.getLong();
		default: 			
			return Byte.valueOf((byte) 0);
		}
	}

	private Object readUnsigned(byte[] data) {
		if (data == null) {
			return "";
		}
		ByteBuffer buffer = ByteBuffer.wrap(data).order(order);
		switch (data.length) {
		case 1:
			return Byte.toUnsignedLong(data[0]);
		case 2:
			return Short.toUnsignedLong(buffer.getShort());
		case 4:
			return Integer.toUnsignedLong(buffer.getInt());
		case 8:
			return buffer.getLong();
		default: 			
			return Byte.valueOf((byte) 0);
		}
	}

	private Double readDouble(byte[] data) {
		return ByteBuffer.wrap(data).order(order).getDouble();
	}

	private Float readFloat(byte[] data) {
		return ByteBuffer.wrap(data).order(order).getFloat();
	}
}
