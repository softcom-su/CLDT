package su.softcom.cldt.debug.core;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.dwarf.DwarfInfo;
import su.softcom.cldt.debug.core.info.IDebugInfo;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo;

/**
 * Moved from EDT <br>
 * Representation for ELF file
 * 
 * @author petr.babanov@softcom.su <br>
 * @author Edited by: rinat.nizamov@softcom.su
 */
/**
 * 
 */
@SuppressWarnings("unused")
public class ELF extends BinaryReadedObject {

	public static final String STRTAB = ".strtab";
	public static final String DYNSTRTAB = ".dynstr";

	class TableProgramHeaderItem {

		final long p_type;
		final long p_flags;
		final long p_offset;
		final long p_vaddr;
		final long p_paddr;
		final long p_filesz;
		final long p_memsz;
		final long p_align;

		public TableProgramHeaderItem(byte[] array) throws IOException {
			p_type = readUInt(array);
			if (EI_CLASS == ELF32) {
				p_offset = readUInt(array);
				p_vaddr = readUInt(array);
				p_paddr = readUInt(array);
				p_filesz = readUInt(array);
				p_memsz = readUInt(array);
				p_flags = readUInt(array);
				p_align = readUInt(array);
			} else {
				p_flags = readUInt(array);
				p_offset = readLong(array);
				p_vaddr = readLong(array);
				p_paddr = readLong(array);
				p_filesz = readLong(array);
				p_memsz = readLong(array);
				p_align = readLong(array);
			}
		}
	}

	/**
	 * 
	 */
	public class TableSectionHeaderItem {

		static final int SH_TYPE_SYMTAB = 2;
		static final int SH_TYPE_STRTAB = 3;
		static final int SH_TYPE_DYNSYM = 11;

		String name;

		final int index;
		final long sh_name;
		final long sh_type;
		final long sh_flags;
		final long sh_address;
		final long sh_offset;
		final long sh_size;
		final long sh_link;
		final long sh_info;
		final long sh_addralign;
		final long sh_entsize;

		/**
		 * 
		 * @param array raw байты секции
		 * @param index индекс секции
		 * @throws IOException
		 */
		public TableSectionHeaderItem(byte[] array, int index) throws IOException {
			this.index = index;
			sh_name = readUInt(array);
			sh_type = readUInt(array);
			if (EI_CLASS == ELF32) {
				sh_flags = readUInt(array);
				sh_address = readUInt(array);
				sh_offset = readUInt(array);
				sh_size = readUInt(array);
				sh_link = readUInt(array);
				sh_info = readUInt(array);
				sh_addralign = readUInt(array);
				sh_entsize = readUInt(array);
			} else {
				sh_flags = readLong(array);
				sh_address = readLong(array);
				sh_offset = readLong(array);
				sh_size = readLong(array);
				sh_link = readUInt(array);
				sh_info = readUInt(array);
				sh_addralign = readLong(array);
				sh_entsize = readLong(array);
			}
		}

		/**
		 * 
		 * @return name
		 */
		public String getName() {
			return name;
		}

		/**
		 * 
		 * @param name to set
		 */
		public void setName(String name) {
			this.name = name;
		}

		/**
		 * @return sh_offset value
		 */
		public long getOffset() {
			return sh_offset;
		}

		/**
		 * @return sh_size value
		 */
		public long getSize() {
			return sh_size;
		}

		/**
		 * @return sh_address value
		 */
		public long getAddress() {
			return sh_address;
		}

		/**
		 * @return sh_link
		 */
		public long getLink() {
			return sh_link;
		}

		/**
		 * @return index
		 */
		public long getIndex() {
			return index;
		}

		@Override
		public String toString() {
			return "name: %s;%n index: %d".formatted(name, index);
		}
	}

	private class SymbolTableItem implements ISymbol {

		final long st_value;
		final long st_size;
		final byte st_info;
		final byte st_other;
		final short st_shndx;
		final String name;

		// ELF64
		private SymbolTableItem(String name, byte st_info, byte st_other, short st_shndx, long st_value, long st_size) {
			this.st_value = st_value;
			this.st_size = st_size;
			this.st_info = st_info;
			this.st_other = st_other;
			this.st_shndx = st_shndx;
			this.name = name;
		}

		// ELF32
		private SymbolTableItem(String name, long st_value, long st_size, byte st_info, byte st_other, short st_shndx) {
			this(name, st_info, st_other, st_shndx, st_value, st_size);
		}

		@Override
		public String getName() {
			return name;
		}

		@Override
		public long getValue() {
			return st_value;
		}

		@Override
		public String toString() {
			return "name: %s; %n value: %d".formatted(name, st_value);
		}
	}

	/**
	 * value for {@link su.softcom.cldt.debug.core.ELF#EI_CLASS} indicates Elf32
	 * structure
	 */
	private static final int ELF32 = 0x1;
	/**
	 * value for {@link su.softcom.cldt.debug.core.ELF#EI_CLASS} indicates Elf64
	 * structure
	 */
	private static final int ELF64 = 0x2;
	private static final int ELF32_PROGRAM_HEADER_SIZE = 32;
	private static final int ELF64_PROGRAM_HEADER_SIZE = 56;

	private static final int ELF32_SECTION_HEADER_SIZE = 40;
	private static final int ELF64_SECTION_HEADER_SIZE = 66;

	private static final int ELF32_HEADER_SIZE = 52;
	private static final int ELF64_HEADER_SIZE = 66;

	/**
	 * Indicates elf bitness {@link su.softcom.cldt.debug.core.ELF#ELF32 elf32} or
	 * {@link su.softcom.cldt.debug.core.ELF#ELF64 elf64}
	 */
	private int EI_CLASS;
	private int EI_VERSION;
	private int EI_OSABI;
	private int EI_ABIVERSION;

	private int E_TYPE;
	private int E_MACHINE;
	private long E_VERSION;

	private long E_ENTRY;
	private long E_PHOFF;
	private long E_SHOFF;
	private long E_FLAGS;
	private int E_EHSIZE;
	private int E_PHENTSIZE;
	private int E_PHNUM;
	private int E_SHNUM;
	private int E_SHENTSIZE;
	private int E_SHSTRNDX;

	private byte[] frameSection = new byte[0];
	private List<TableProgramHeaderItem> tableProgramHeader = new ArrayList<>();
	private List<TableSectionHeaderItem> tableSectionHeader = new ArrayList<>();
	private Map<String, byte[]> debugSections = new HashMap<>();

	/**
	 * <b>KEY</b> is sh_link to str tab of this symTabSection<br>
	 * <br>
	 * <b>VALUE</b> is this symTabSection
	 */
	private Map<Long, byte[]> symTabSections = new HashMap<>();
	/**
	 * <b>KEY</b> is sh_link of this strTabSection <br>
	 * <br>
	 * <b>VALUE</b> is this strTabSection
	 */
	private Map<Long, byte[]> strTabSections = new HashMap<>();
	private Map<String, ISymbol> symbolMap = new HashMap<>();
	/**
	 * Символы, которые ищем "быстро".
	 */
	private List<String> symbolsToFind = new ArrayList<>();
	private IDebugInfo debugInfo = null;
	private IPath path;
	private IStackInfo callFrameInfo = null;

	/**
	 * @param result raw bytes of ELF file
	 */
	public ELF(byte[] result) {
		if (readMagic(result)) {
			readHeader(result);
		}
	}

	/**
	 * get path to file in EFS
	 * 
	 * @return IPath or null if file read from byteArrayInputStream
	 */
	public IPath getPath() {
		return path;
	}

	private boolean readMagic(byte[] array) throws IllegalArgumentException {
		return (array[0] == 0x7f) && (array[1] == 0x45) && (array[2] == 0x4c) && (array[3] == 0x46);
	}

	private void readHeader(byte[] header) {
		try {
			offset = 4;
			EI_CLASS = header[offset++];
			int intEndian = header[offset++];
			if (intEndian == 1) {
				setEndian(ByteOrder.LITTLE_ENDIAN);
			} else if (intEndian == 2) {
				setEndian(ByteOrder.BIG_ENDIAN);
			} else {
				throw new IllegalArgumentException("unexpected \"EI_CLASS\" filed in file header");
			}
			EI_VERSION = header[offset++];
			EI_OSABI = header[offset++];
			EI_ABIVERSION = header[offset++];
			offset += 7;
			E_TYPE = readShort(header);
			E_MACHINE = readShort(header);
			E_VERSION = readUInt(header);
			if (EI_CLASS == ELF32) {
				E_ENTRY = readUInt(header);
				E_PHOFF = readUInt(header);
				E_SHOFF = readUInt(header);
			} else {
				E_ENTRY = readLong(header);
				E_PHOFF = readLong(header);
				E_SHOFF = readLong(header);
			}
			E_FLAGS = readUInt(header);
			E_EHSIZE = readShort(header);
			E_PHENTSIZE = readShort(header);
			E_PHNUM = readShort(header);
			E_SHENTSIZE = readShort(header);
			E_SHNUM = readShort(header);
			E_SHSTRNDX = readShort(header);
		} catch (IOException e) {
			Platform.getLog(getClass()).error(e.getLocalizedMessage());
			e.printStackTrace();
		}
	}

	/**
	 * @return true, if E_TYPE == 3
	 */
	public boolean isElfTypeDYN() {
		return E_TYPE == 3;
	}

	/**
	 * @return E_SHENTSIZE * E_SHNUM value
	 */
	public int calculateTableSectionSize() {
		return E_SHENTSIZE * E_SHNUM;
	}

	/**
	 * @return E_SHOFF value
	 */
	public long getSectionHeaderOffset() {
		return E_SHOFF;
	}

	/**
	 * @return .shstrtab header
	 */
	public TableSectionHeaderItem getSectionHeaderStrTab() {
		return E_SHSTRNDX != 0 ? tableSectionHeader.get(E_SHSTRNDX) : null;
	}

	/**
	 * @param name
	 * @return TableSectionHeaderItem with this name
	 */
	public TableSectionHeaderItem getSectionHeader(String name) {
		return tableSectionHeader.stream().filter(item -> item.name.equals(name)).findFirst().orElse(null);
	}

	/**
	 * @param name
	 * @return sectionHeader address
	 */
	public long getAddressSectionHeader(String name) {
		TableSectionHeaderItem sectionHeader = tableSectionHeader.stream().filter(item -> item.name.equals(name))
				.findFirst().orElse(null);
		if (sectionHeader != null) {
			return sectionHeader.getAddress();
		}
		return -1;
	}

	/**
	 * @param array
	 */
	public void readSectionHeaderNames(byte[] array) {
		for (TableSectionHeaderItem item : tableSectionHeader) {
			item.setName(readString(array, item.sh_name));
		}
	}

	/**
	 * 
	 * @return debug sections: .debug*
	 */
	public List<TableSectionHeaderItem> readDebugSections() {
		List<TableSectionHeaderItem> dSections = new ArrayList<>();
		for (TableSectionHeaderItem item : tableSectionHeader) {
			if (item.name.startsWith(".debug")) {
				dSections.add(item);
			}
		}
		return dSections;
	}
	

	/**
	 * 
	 * @return stack frame sections: .eh_frame
	 */
	public List<TableSectionHeaderItem> readFrameSections() {
		List<TableSectionHeaderItem> dSections = new ArrayList<>();
		for (TableSectionHeaderItem item : tableSectionHeader) {
			//TODO: read eh_frame_hdr too -> change to .contains(".eh_frame")
			if (item.name.equals(".eh_frame")) {
				dSections.add(item);
			}
		}
		return dSections;
	}

	/**
	 * @return dynsym and symtab sections
	 */
	public List<TableSectionHeaderItem> getSymTabSections() {
		List<TableSectionHeaderItem> stSections = new ArrayList<>();
		for (TableSectionHeaderItem item : tableSectionHeader) {
			if (item.sh_type == TableSectionHeaderItem.SH_TYPE_SYMTAB) {
				stSections.add(item);
			}
			if (item.sh_type == TableSectionHeaderItem.SH_TYPE_DYNSYM) {
				stSections.add(item);
			}
		}
		return stSections;
	}

	/**
	 * @return all str tab section. sh_type == 3
	 */
	public List<TableSectionHeaderItem> getStrTabSections() {
		List<TableSectionHeaderItem> stSections = new ArrayList<>();
		for (TableSectionHeaderItem item : tableSectionHeader) {
			if (item.sh_type == TableSectionHeaderItem.SH_TYPE_STRTAB) {
				stSections.add(item);
			}
		}
		return stSections;
	}

	/**
	 * @param index
	 * @return section header by it's index
	 */
	public TableSectionHeaderItem getSectionHeader(long index) {
		return tableSectionHeader.stream().filter(item -> item.getIndex() == index).findFirst().orElse(null);
	}

	/**
	 * @param dSections
	 */
	public void setDebugSections(Map<String, byte[]> dSections) {
		this.debugSections = dSections;
	}
	
	/**
	 * @param fSections
	 */
	public void setFrameSection(byte[] fSection) {
		this.frameSection = fSection;
	}

	/**
	 * @param array
	 * @throws IOException
	 */
	public void readSectionHeaders(byte[] array) throws IOException {
		offset = 0;
		for (int i = 0; i < E_SHNUM; i++) {
			tableSectionHeader.add(new TableSectionHeaderItem(array, i));
		}
	}

	/**
	 * 
	 * @param name
	 * @return debugSection
	 */
	public byte[] getDebugSection(String name) {
		byte[] debugSection = debugSections.get(name);
		debugSections.remove(name);
		return debugSection;
	}

	
	/**
	 * @return frameSections
	 */
	public byte[] getFrameSections() {
		return frameSection;
	}

	private boolean littleEndian() {
		return getEndian() == ByteOrder.LITTLE_ENDIAN;
	}

	private int bitness() {
		if (EI_CLASS == ELF32) {
			return 32;
		}
		return 64;
	}

	private String getSymString(long localOffset, long index) {
		byte[] strtab = getStrTab(index);
		if (localOffset != 0 && strtab != null) {
			TableSectionHeaderItem strtabSection = getSectionHeader(index);
			if (strtabSection.sh_size > localOffset) {
				int i = (int) localOffset;
				byte b = strtab[i];
				StringBuilder result = new StringBuilder();
				while (i < strtabSection.sh_size && b != 0) {
					result.append((char) b);
					i++;
					b = strtab[i];
				}
				return result.toString();
			}
		}
		return ""; //$NON-NLS-1$
	}

	/**
	 * Updates symbolMap, if finds symbols from symbolsToFind field
	 * 
	 * @param link
	 * @param table
	 * @return
	 */
	private void readSymTable(long link, byte[] table) {
		List<SymbolTableItem> symbols = EI_CLASS == ELF32 ? read32elfSymtab(link, table) : read64elfSymtab(link, table);
		for (SymbolTableItem symbol : symbols) {
			symbolMap.put(symbol.getName(), symbol);
		}
	}

	// FIXME
	private List<SymbolTableItem> read32elfSymtab(long link, byte[] section) {
		List<SymbolTableItem> result = new ArrayList<>();
		ByteBuffer buffer = ByteBuffer.allocate(section.length);
		buffer.order(getEndian());
		byte[] strtab = getStrTab(link);
		buffer.put(section);
		for (int i = 0; i < section.length / 16; i++) {
			int localoffset = i * 16;
			long nameOffset = buffer.getInt(localoffset);
			if (strtab[(int) nameOffset] == 'm') {
				String name = getSymString(nameOffset, link);
				if (name.equals("main")) {
					result.add(new SymbolTableItem(name, buffer.getInt(localoffset + 4), buffer.getInt(localoffset + 8),
							buffer.get(localoffset + 12), buffer.get(localoffset + 13),
							buffer.getShort(localoffset + 14)));
					break;
				}
			}
		}
		return result;
	}

	/**
	 * 
	 * FIXME сделать второй метод. плохо что этот метод читает не все символы, а
	 * лишь symbolsToFind
	 * 
	 * @param index   секции
	 * @param section raw byte секции
	 * @return List of SymbolTableItem
	 */
	private List<SymbolTableItem> read64elfSymtab(long index, byte[] section) {
		List<SymbolTableItem> result = new ArrayList<>();
		ByteBuffer buffer = ByteBuffer.wrap(section);
		buffer.order(getEndian());
		for (int i = 0; i < section.length / 24; i++) {
			int localoffset = i * 24;
			long nameOffset = buffer.getInt(localoffset);
			String name = getSymString(nameOffset, index);
			if (symbolsToFind.contains(name)) {
				symbolsToFind.remove(name);
				result.add(new SymbolTableItem(name, buffer.get(localoffset + 4), buffer.get(localoffset + 5),
						buffer.getShort(localoffset + 6), buffer.getLong(localoffset + 8),
						buffer.getLong(localoffset + 16)));
				if (symbolsToFind.isEmpty()) {
					break;
				}
			}
		}
		return result;
	}

	/**
	 * 
	 * @param name
	 * @return symbol or null, if symbol not found
	 */
	public ISymbol getSymbol(String name) {
		if (!symbolsToFind.contains(name)) {
			addSymbolToFind(name);
		}
		ISymbol symbol = null;
		if (!symbolMap.containsKey(name)) {
			readSymbols();
			symbol = symbolMap.get(name);
			if (symbol == null) {
				Platform.getLog(getClass()).info("Symbol %s not found in ELF file.".formatted(name));
				return null;
			}
		}
		return symbol;
	}

	/**
	 * TODO: убрать из public
	 */
	public void readSymbols() {
		for (Map.Entry<Long, byte[]> entry : symTabSections.entrySet()) {
			readSymTable(entry.getKey(), entry.getValue());
		}
	}

	/**
	 * 
	 * @param index
	 * @param section
	 */
	public void addStrTabSection(long index, byte[] section) {
		strTabSections.put(index, section);
	}

	/**
	 * 
	 * @param link
	 * @param section
	 */
	public void addSymTabSection(long link, byte[] section) {
		symTabSections.put(link, section);
	}

	/**
	 * 
	 * @param name
	 */
	public void addSymbolToFind(String name) {
		if (!symbolsToFind.contains(name)) {
			symbolsToFind.add(name);
		} else {
			Platform.getLog(getClass()).info("Symbol %s already added to quick search".formatted(name));
		}
	}

	private byte[] getStrTab(long index) {
		return strTabSections.get(index);
	}

	/**
	 * 
	 * @return debugInfo
	 */
	public IDebugInfo getDebugInfo() {
		if (debugInfo == null) {
			readDebugInfo();
		}
		return debugInfo;
	}

	/**
	 * TODO: refactor it!!!!
	 */
	public void readDebugInfo() {
		this.debugInfo = new DwarfInfo(this);
		if (debugInfo == null) {
			Platform.getLog(getClass()).info("No .debug info found in this ELF file");
		}
	}

	/**
	 * @return
	 */
	public IStackInfo getStackInfo() {
		if (debugInfo != null && callFrameInfo == null) {
			return debugInfo.getStackInfo();
		}
		return callFrameInfo;
	}

	public void setStackInfo(IStackInfo stackInfo) {
		this.callFrameInfo = stackInfo;
	}
	
}
