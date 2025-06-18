package su.softcom.cldt.debug.internal.core.dwarf.values;

import java.nio.ByteBuffer;
import java.util.Objects;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.dwarf.DwarfInfo;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfElement;

/**
 * Common implementation object for dwarf attribute values
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public class Value extends DwarfElement {

	protected Object o;
	int form;
	private boolean tmp;

	public Value(DwarfInfo info, int form, ByteBuffer data) {
		this(info, null, form, data);
	}

	public Value(int form, Object value) {
		this.form = form;
		setObject(value);
	}

	public Value(DwarfInfo info, ICompileUnit compileUnit, int form, ByteBuffer buffer) {
		this.form = form;
		readValue(buffer, compileUnit, info);
	}

	/**
	 * @param info
	 * @param compileUnit
	 * @param form
	 * @param buffer
	 */
	protected void readValue(ByteBuffer buffer, ICompileUnit compileUnit, DwarfInfo info) {
		byte[] data;
		long size;
		long off;
		long off_off;
		switch (form) {
		case DwarfConstants.DW_FORM_addr:
			switch (compileUnit.getAddressSize()) {
			case 2:
				setObject(Short.valueOf(buffer.getShort()));
				break;
			case 4:
				setObject(Integer.valueOf(buffer.getInt()));
				break;
			case 8:
				setObject(Long.valueOf(buffer.getLong()));
				break;
			default:
			}
			break;
		case DwarfConstants.DW_FORM_data2:
			setObject(Short.valueOf(buffer.getShort()));
			break;
		case DwarfConstants.DW_FORM_data4:
			setObject(Integer.valueOf(buffer.getInt()));
			break;
		case DwarfConstants.DW_FORM_data8:
			setObject(Long.valueOf(buffer.getLong()));
			break;
		case DwarfConstants.DW_FORM_string:
			int c;
			StringBuilder b = new StringBuilder();
			while ((c = buffer.get()) != -1) {
				if (c == 0) {
					break;
				}
				b.append((char) c);
			}
			setObject(b.toString());
			break;
		case DwarfConstants.DW_FORM_block1:
			size = (int) (buffer.get() & 0xffL);
			setObject(new byte[(int) (size & 0xff)]);
			buffer.get((byte[]) getObject());
			break;
		case DwarfConstants.DW_FORM_data1:
			setObject(Byte.valueOf(buffer.get()));
			break;
		case DwarfConstants.DW_FORM_flag:
			setObject(Byte.valueOf(buffer.get()));
			break;
		case DwarfConstants.DW_FORM_sdata:
			setObject(Long.valueOf(readLeb128(buffer)));
			break;
		case DwarfConstants.DW_FORM_strp:
			off = compileUnit.getOffset() == 4 ? off = buffer.getInt() : buffer.getLong();
			if (off < 0 || off > buffer.capacity()) {
				setObject("");
			} else {
				setObject(info.getString((int) off));
			}

			break;
		case DwarfConstants.DW_FORM_udata:
			setObject(Long.valueOf(readUleb128(buffer)));
			break;
		case DwarfConstants.DW_FORM_ref4:
			setObject(Long.valueOf(Integer.toUnsignedLong((int) compileUnit.getFullOffset() + buffer.getInt())));
			break;
		case DwarfConstants.DW_FORM_indirect:
			System.out.println("unexpected form DW_FORM_indirect");
			break; // Cannot correctly this case.
		case DwarfConstants.DW_FORM_sec_offset:
			setObject(Long.valueOf(DWARF32 ? buffer.getInt() : buffer.getLong()));
			break;
		case DwarfConstants.DW_FORM_flag_present:
			setObject(Byte.valueOf((byte) 1));
			break;
		case DwarfConstants.DW_FORM_addrx:
			off = readUleb128(buffer);
			try {
				setObject(Long.valueOf(compileUnit.getAddress((int) off)));
			} catch (NullPointerException e) {
				tmp = true;
				setObject(Integer.valueOf((int) off)); // addressTable not setted in compileUnit
			}
			break;
		case DwarfConstants.DW_FORM_data16:
			data = new byte[16];
			buffer.get(data);
			setObject(data);
			break;
		case DwarfConstants.DW_FORM_line_strp:
			int offset = buffer.getInt();
			ByteBuffer strs = info.getLineString();
			strs.position(offset);
			StringBuilder sb = new StringBuilder();
			byte s = strs.get();
			while (s != 0) {
				sb.append((char) s);
				s = strs.get();
			}
			setObject(sb.toString());
			break;
		case DwarfConstants.DW_FORM_loclistx:
			off = readUleb128(buffer);
			setObject(info.getLocationList(off));
			break;
		case DwarfConstants.DW_FORM_rnglistx:
			off = readUleb128(buffer);
			setObject(off);
			break;
		case DwarfConstants.DW_FORM_strx1:
			off_off = Byte.toUnsignedInt(buffer.get());
			if (compileUnit.getStrOffsetBase() == 0) {
				setObject(Long.valueOf(off_off));
				break;
			}
			getString(off_off, compileUnit, info);
			break;
		case DwarfConstants.DW_FORM_strx2:
			off_off = Short.toUnsignedInt(buffer.getShort());
			if (compileUnit.getStrOffsetBase() == 0) {
				setObject(Long.valueOf(off_off));
				break;
			}
			off = info.getStringOffset(compileUnit.getStrOffsetBase(), (int) off_off, DWARF32);
//			if (off < 0 || off > buffer.capacity()) {
//				setObject("");
//			} else {
			setObject(info.getString((int) off));
//			}
			break;
		case DwarfConstants.DW_FORM_implicit_const:
			System.out.println("unexpected form DW_FORM_implicit_const");
			break;
		default:
			System.out.println("Unhandled form: " + Long.toHexString(form)); //$NON-NLS-1$
			break;
		}
	}

	@Override
	public String toString() {
		if (getObject() != null) {
			return getObject().toString();
		}
		return "null";
	}

	public Object getValue() {
		return getObject();
	}

	public void getString(long offset, ICompileUnit compileUnit, DwarfInfo info) {
		long off = info.getStringOffset(compileUnit.getStrOffsetBase(), (int) offset, DWARF32);
		setObject(info.getString((int) off));
	}

	public Object getObject() {
		return o;
	}

	public void setObject(Object o) {
		this.o = o;
	}

	public static Value getValue(int form, ByteBuffer data, CompileUnitImpl cu, DwarfInfo info) {
		switch (form) {
		case DwarfConstants.DW_FORM_exprloc:
			return new Exprloc(info, data, cu);
		default:
			return new Value(info, cu, form, data);
		}
	}

	@Override
	public int hashCode() {
		return Objects.hash(o, tmp, form);
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		Value other = (Value) obj;
		return Objects.equals(o, other.o) && form == other.form;
	}

	public boolean isTmp() {
		return tmp;
	}

	public void setTmp(boolean tmp) {
		this.tmp = tmp;
	}

	public long getUnsignedLongValue() {
		return doGetUnsignedLongValue();
	}

	public int getUnsignedIntValue() {
		if ((o instanceof Integer i) && (i.intValue() < 0)) {
			throw new IllegalStateException(
					"Cannot convert value %n as unsigned int, try getUnsignedLongValue()".formatted(i));
		}
		return doGetUnsignedIntValue();
	}

	public short getUnsignedShortValue() {
		if ((o instanceof Short s) && (s.shortValue() < 0)) {
			throw new IllegalStateException(
					"Cannot convert value %n as unsigned short, try getUnsignedIntValue()".formatted(s));
		}
		return doGetUnsignedShortValue();
	}

	public byte getUnsignedByteValue() {
		if ((o instanceof Byte b) && (b.byteValue() < 0)) {
			throw new IllegalStateException(
					"Cannot convert value %n as unsigned byte, try getUnsignedIntValue()".formatted(b));
		}
		return doGetUnsignedByteValue();
	}

	long doGetUnsignedLongValue() {
		if (o instanceof Long l) {
			return l.longValue();
		} else {
			return ((long) doGetUnsignedIntValue()) & 0xffffffffL;
		}
	}

	int doGetUnsignedIntValue() {
		if (o instanceof Integer i) {
			return i.intValue();
		} else {
			return ((int) doGetUnsignedShortValue()) & 0xffff;
		}
	}

	short doGetUnsignedShortValue() {
		if (o instanceof Short s) {
			return s.shortValue();
		} else {
			return (short) (((short) doGetUnsignedByteValue()) & 0xff);
		}
	}

	byte doGetUnsignedByteValue() {
		if (o instanceof Byte b) {
			return b.byteValue();
		} else {
			throw new IllegalStateException(
					"Cannot decode instance of class %s as numerical data".formatted(o.getClass().getName()));
		}
	}
}
