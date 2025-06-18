package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;

public class RangeList extends DwarfElement {
	private final long length;
	private final short version;
	private final byte addressSize;
	private final int segmentSelectorSize;
	private final long offsetEntryCount;
	private final int[] offsets;
	private long baseOffset;
	
	public RangeList(ByteBuffer buffer) {
		length = getLength(buffer);
		version = buffer.getShort();
		addressSize  = buffer.get();
		segmentSelectorSize  = Byte.toUnsignedInt(buffer.get());
		offsetEntryCount = Integer.toUnsignedLong(buffer.getInt());
		if (offsetEntryCount > 0) {
			offsets = new int[(int) offsetEntryCount];
			baseOffset = buffer.position();
		    for (int i = 0; i < offsetEntryCount; i++) {
		        offsets[i] = (int) (baseOffset + (isDWARF32() ? buffer.getInt() & 0xFFFFFFFFL : buffer.getLong()));
		    }
		} else {
			offsets = null;
		} 
		
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
		return offsetEntryCount;
	}

	public int[] getOffsets() {
		return offsets;
	}

	public long getBaseOffset() {
		return baseOffset;
	}

	@Override
	public String toString() {
		return "RangeList [length=" + length + ", version=" + version + ", addressSize=" + addressSize
				+ ", segmentSelectorSize=" + segmentSelectorSize + ", offset=" + offsetEntryCount + "]";
	}

}
