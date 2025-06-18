package su.softcom.cldt.debug.core;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * STOLEN FROM EDT
 * <br>
 * Simple prototype for objects, which generated from binary data source
 * <br>
 * Subclasses must set {@link su.softcom.cldt.debug.core.BinaryReadedObject#endian endian} field 
 * @author petr.babanov@softcom.su
 */
public abstract class BinaryReadedObject {

	/**
	 * Endianness expected while read object
	 */
	private ByteOrder endian;
	
	protected int offset;
	
	public BinaryReadedObject() {
	}
	/**
	 * read short (primitive) (16 bytes) from stream
	 * @param stream
	 * @return read short
	 * @throws IOException
	 */
	protected int readShort(byte[] array) throws IOException {
		offset += 2;
		if (getEndian() == ByteOrder.LITTLE_ENDIAN) {
			return ByteBuffer.wrap(new byte[] {array[offset - 1], array[offset - 2]}).getShort();
		} else {
			return ((int)ByteBuffer.wrap(new byte[] {array[offset - 2], array[offset - 1]}).getShort()) & 0xffff;
		}
	}
	
	protected int readUShort(byte[] array) {
		offset += 2;
		if (getEndian() == ByteOrder.LITTLE_ENDIAN) {
			return Short.toUnsignedInt(ByteBuffer.wrap(new byte[] {array[offset - 1], array[offset - 2]}).getShort());
		} else {
			return Short.toUnsignedInt((ByteBuffer.wrap(new byte[] {array[offset - 2], array[offset - 1]}).getShort()));
		}
	}

	/** read integer (primitive) (32 bytes) from stream
	 * @param stream
	 * @return read integer
	 * @throws IOException
	 */
	protected long readInt(byte[] array) throws IOException {
		if (getEndian() == ByteOrder.LITTLE_ENDIAN) {
			return (readShort(array)& 0xffff | (readShort(array)&0xffff << 16)) & 0xffffffff;
		} else {
			return (((long)readShort(array)) << 16) | (readShort(array));
		}
	}
	
	protected long readUInt(byte[] array) throws IOException {
		if (getEndian() == ByteOrder.LITTLE_ENDIAN) {
			return Integer.toUnsignedLong(readUShort(array) | (readUShort(array) << 16));
		} else {
			return Integer.toUnsignedLong((readUShort(array) << 16) | readUShort(array));
		}
	}

	/** read long (primitive) (64 bytes) from stream
	 * @param stream
	 * @return read long
	 * @throws IOException
	 */
	protected long readLong(byte[] array) throws IOException {
		if (getEndian() == ByteOrder.LITTLE_ENDIAN) {
			return readUInt(array) | (readUInt(array) << 32);
		} else {
			return (readUInt(array) << 32) | readUInt(array);
		}
	}
	
	protected String readString(byte[] array, long l) {
		StringBuilder builder = new StringBuilder();
		while(array[(int) l] != 0) {
			builder.append((char)array[(int) l++]);
		}
		return builder.toString();
	}
	
	/**
	 * check correct state of {@link su.softcom.cldt.debug.core.BinaryReadedObject#endian field} field
	 */
	private void checkEndian() {
		if((getEndian() != ByteOrder.LITTLE_ENDIAN) && (getEndian() != ByteOrder.BIG_ENDIAN)){
			throw new IllegalStateException("field \"endian\" set not correctly");
		}
	}
	
	public static void skip(InputStream stream, long n) {
		if(stream.markSupported()) {
			while(n > 0) {
				try {
					n -= stream.skip(n);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}

	public ByteOrder getEndian() {
		return endian;
	}
	public void setEndian(ByteOrder endian) {
		this.endian = endian;
	}
	
}
