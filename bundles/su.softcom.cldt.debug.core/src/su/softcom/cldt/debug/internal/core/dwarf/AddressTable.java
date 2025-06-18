package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.eclipse.core.runtime.Platform;

/**
 * Presentation for content of {@code .debug_addr} section
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public class AddressTable extends DwarfElement{
	
	/**
	 * Item of {@code .debug_addr} content, contents table with addresses for {@code CompileUnut}
	 * 
	 * @author Petr Babanov (petr.babanov@softcom.su)
	 *
	 *
	 */
	public class AddressTableItem{
		long size;
		final int version;
		final int addressSize;
	    final long[] addresses; 
		final short addressSelectorSize;
		final long offset;
		public AddressTableItem(ByteBuffer buffer) {
			size = getLength(buffer);
			version = buffer.getShort();
			addressSize = buffer.get();
			addressSelectorSize = buffer.get();
	        
			int count = (int)((size - 4) / addressSize);
	        addresses = new long[count];
	        
	        offset = buffer.position();
	        for (int i = 0; i < count; i++) {
	            if (addressSize == 4) {
	                addresses[i] = buffer.getInt();
	            } else {
	                addresses[i] = buffer.getLong();
	            }
	        }
		}
		
		public long getOffset() {
			return offset;
		}
		
		public long getAddress(int index) {
	        if (index >= addresses.length) {
	            throw new IllegalArgumentException("Index " + index + " not present in table");
	        }
	        return addresses[index];
		}
	}
	
	final Map<Long, AddressTableItem> table;
	
	public AddressTable(ByteBuffer section) {
		Map<Long, AddressTableItem> tmp = new HashMap<>();
		while(section.hasRemaining()) {
			AddressTableItem t = new AddressTableItem(section);
			tmp.put(Long.valueOf(t.getOffset()), t);
		}
		table = Collections.unmodifiableMap(tmp);
	}
	
	public AddressTableItem getTableItem(long offset) {
		AddressTableItem result = table.get(Long.valueOf(offset));
		if(result != null) {
			return result;
		}
		throw new IllegalArgumentException("offset " + offset + " not found in table");
	}

}

