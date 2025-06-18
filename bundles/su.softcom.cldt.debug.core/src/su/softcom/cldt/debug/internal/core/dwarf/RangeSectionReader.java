package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import su.softcom.cldt.debug.core.dwarf.DwarfInfo;
import su.softcom.cldt.debug.internal.core.dwarf.AddressTable.AddressTableItem;

public class RangeSectionReader extends DwarfElement {

    private static final long MAX_OFFSET_32 = 0xFFFFFFFFL; 
    private static final long MAX_OFFSET_64 = 0xFFFFFFFFFFFFFFFFL; 
	
	DwarfInfo info;
	ByteBuffer buffer;
	
	public RangeSectionReader(ByteBuffer buffer, DwarfInfo info) {
		this.info = info;
		this.buffer = buffer;
	}

	public Map<Long, RangeList> readRangesLists() {
		Map<Long, RangeList> rangeLists = new HashMap<>();
		while(buffer.hasRemaining()) {
			long offset = buffer.position();
			RangeList list = new RangeList(buffer);
			buffer.position((int) (offset + list.getLength() + 4));
		    rangeLists.put(list.getBaseOffset(), list);
		}
		return rangeLists;
	}
	
	// read .debug_rnglists
	public List<RangeEntry> readDebugRanges(RangeList rnglist, AddressTableItem tableItem, long index) {
        List<RangeEntry> rangeEntries = new ArrayList<>();
        
		int[] array = rnglist.getOffsets();
		int offset = array[(int) index];
		buffer.position(offset);
		long startAddress = 0;
		long endAddress = 0;
		long baseAddress = 0;
		byte code = buffer.get();
		while(code != 0) {
			switch (code) {
			case DwarfConstants.DW_RLE_end_of_list:
				continue;
			case DwarfConstants.DW_RLE_base_addressx:
				int baseAddressIndex = (int) readUleb128(buffer);
				baseAddress = tableItem.getAddress(baseAddressIndex);
				break;
			case DwarfConstants.DW_RLE_startx_endx:
				int fstIndex = (int) readUleb128(buffer);
				int scdIndex = (int) readUleb128(buffer);
				if (tableItem != null) {					
					startAddress = tableItem.getAddress(fstIndex);
					endAddress = tableItem.getAddress(scdIndex);
					rangeEntries.add(new RangeEntry(startAddress, endAddress));
				}
				break;
			case DwarfConstants.DW_RLE_startx_length:
				// The first value is an address index (into the .debug_addr section) that indicates the beginning of the address range
				int i = (int) readUleb128(buffer); 
				// The second value is the length of the range.
				long length = readUleb128(buffer);
				if (tableItem != null) {
					startAddress = tableItem.getAddress(i);
					endAddress = startAddress + length;
					rangeEntries.add(new RangeEntry(startAddress, endAddress));
				}
				break;
			case DwarfConstants.DW_RLE_offset_pair:
				long startOffset = readUleb128(buffer);
				long endOffset = readUleb128(buffer);
				startAddress = baseAddress + startOffset;
				endAddress = baseAddress + endOffset;
				rangeEntries.add(new RangeEntry(startAddress, endAddress));
				break;
			case DwarfConstants.DW_RLE_base_address:
				break;
			case DwarfConstants.DW_RLE_start_end:
				break;
			case DwarfConstants.DW_RLE_start_length:
				break;
			}
			code = buffer.get();
		}
		
		return rangeEntries;
	}
	
	// read .debug_ranges
	public List<RangeEntry> readDebugRanges(long rangesOffset, long compileUnitBaseAddress, int addressSize) {
        List<RangeEntry> rangeEntries = new ArrayList<>();

        long baseAddress = compileUnitBaseAddress;
        long maxOffset = addressSize == 8 ? MAX_OFFSET_64 : MAX_OFFSET_32;

		while (buffer.remaining() >= 2 * addressSize) {
            long startOffset = readAddress(buffer, addressSize);
            long endOffset = readAddress(buffer, addressSize);

            if (startOffset == 0 && endOffset == 0) {
                break;
            } else if (startOffset == maxOffset) {
                baseAddress = endOffset;
            } else {
                long startAddress = baseAddress + startOffset;
                long endAddress = baseAddress + endOffset;
                rangeEntries.add(new RangeEntry(startAddress, endAddress));
            }
        }
		
		return rangeEntries;
	}
	
	private long readAddress(ByteBuffer buffer, int addressSize) {
        return addressSize == 4 ? buffer.getInt() & 0xFFFFFFFFL :  buffer.getLong();
    }
	
}
