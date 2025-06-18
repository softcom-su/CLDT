package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import su.softcom.cldt.debug.internal.core.dwarf.tags.Tag;

public class AbbreviationTable extends DwarfElement {

	public class AbbrCompileUnit {
		final Map<Long, Tag> tags;

		public AbbrCompileUnit(ByteBuffer data) {
			Map<Long, Tag> tmp = new HashMap<>();
			int abbrCode = Short.toUnsignedInt((short) readUleb128(data));
			while (abbrCode != 0) {
				tmp.put(Long.valueOf(abbrCode), Tag.getTag(data));
				abbrCode = Short.toUnsignedInt((short) readUleb128(data));
			}
			tags = Collections.unmodifiableMap(tmp);
		}

		public Tag getEntryByCode(long code) {
			return tags.get(Long.valueOf(code));
		}

		public Tag getEntryByName(long tag) {
			for (Entry<Long, Tag> e : tags.entrySet()) {
				if (e.getValue().getTag() == tag) {
					return e.getValue();
				}
			}
			return null;
		}
	}

	Map<Long, AbbrCompileUnit> units;

	public AbbreviationTable(ByteBuffer section) {
		Map<Long, AbbrCompileUnit> tmp = new HashMap<>();
		while (section.hasRemaining()) {
			tmp.put(Long.valueOf(section.position()), new AbbrCompileUnit(section));
		}
		units = Collections.unmodifiableMap(tmp);
	}

	public AbbrCompileUnit getAbbrCompileUnit(long offset) {
		return units.get(Long.valueOf(offset));
	}
}
