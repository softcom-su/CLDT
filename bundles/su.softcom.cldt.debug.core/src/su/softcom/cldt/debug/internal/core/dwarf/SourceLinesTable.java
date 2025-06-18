package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.Map;

import su.softcom.cldt.debug.core.dwarf.DwarfInfo;

public class SourceLinesTable extends DwarfElement {

	final Map<Long, LineNumberProgram> lnps = new HashMap<>();
	final DwarfInfo info;

	public SourceLinesTable(DwarfInfo info, ByteBuffer section) {
		// , ByteBuffer debugLineStr посмотреть как считывать эту секцию при ее наличии
		this.info = info;
		while (section.hasRemaining()) {
			int offset = 4;
			long length = section.getInt();
			if ((length == 0xffffffffL) || (length == 0)) {
				section.position(section.position() - 4);
				length = section.getLong();
				offset = 8;
				DWARF32 = false;
			}
			int originalLimit = section.limit();
			int position = section.position();
			section.limit((int) (position + length));
			LineNumberProgram lnp = new LineNumberProgram(this, section, offset);
			lnps.put(Long.valueOf(position - offset), lnp);
			section.limit(originalLimit);
			section.position((int) (position + length));
		}

	}

	private static long read4bytes(byte[] data, ByteOrder order) {
		if (data.length != 4) {
			throw new IllegalArgumentException("expected array with length 4");
		}
		return ByteBuffer.wrap(data).order(order).getInt();
	}

	public LineNumberProgram getLNP(long offset) {
		return lnps.get(offset);
	}

}
