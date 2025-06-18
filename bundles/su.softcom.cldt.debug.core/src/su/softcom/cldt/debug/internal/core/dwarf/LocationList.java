package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;

public class LocationList{
	final long length;
	final short version;
	final byte addressSize;
	final int segmentSelectorSize;
	final long offset;
	public LocationList(ByteBuffer buffer) {
		length = Integer.toUnsignedLong(buffer.getInt());
		version = buffer.getShort();
		addressSize  = buffer.get();
		segmentSelectorSize  = Byte.toUnsignedInt(buffer.get());
		offset = Integer.toUnsignedLong(buffer.getInt());
	}
	public long getLength() {
		return length;
	}
	public short getVersion() {
		return version;
	}
	public byte getAddressSize() {
		return addressSize;
	}
	public int getSegmentSelectorSize() {
		return segmentSelectorSize;
	}
	public long getOffset() {
		return offset;
	}
	
}
