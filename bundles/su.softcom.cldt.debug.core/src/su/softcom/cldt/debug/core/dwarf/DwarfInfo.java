package su.softcom.cldt.debug.core.dwarf;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.function.Function;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.ELF;
import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IStackInfo;
import su.softcom.cldt.debug.core.info.IDebugInfo;
import su.softcom.cldt.debug.internal.core.dwarf.AbbreviationTable;
import su.softcom.cldt.debug.internal.core.dwarf.AbbreviationTable.AbbrCompileUnit;
import su.softcom.cldt.debug.internal.core.dwarf.AddressTable;
import su.softcom.cldt.debug.internal.core.dwarf.BreakpointInfo;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfElement;
import su.softcom.cldt.debug.internal.core.dwarf.LocationList;
import su.softcom.cldt.debug.internal.core.dwarf.RangeEntry;
import su.softcom.cldt.debug.internal.core.dwarf.RangeList;
import su.softcom.cldt.debug.internal.core.dwarf.RangeSectionReader;
import su.softcom.cldt.debug.internal.core.dwarf.SourceLinesTable;
import su.softcom.cldt.debug.internal.core.dwarf.attr.AttributeFactory;

/**
 * Moved from EDT <br>
 * Representation for DWARF info
 * 
 * @author Petr Babanov (petr.babanov@softcom.su) <br>
 * @author Edited by: maxim.drumov@softcom.su
 */
public class DwarfInfo extends DwarfElement implements IDebugInfo {

	private final ELF elfFile;
	
	final ILog dwarfLOG = Platform.getLog(this.getClass());
	final ByteOrder order;
	List<CompileUnitImpl> compileUnits;

	static final String DEBUG_TRACE = "su.softcom.cldt.debug.core/debug/tracing"; //$NON-NLS-1$
	public boolean tracing = "true".equalsIgnoreCase(Platform.getDebugOption(DEBUG_TRACE)); //$NON-NLS-1$

	private byte[] debugInfo;
	ByteBuffer stringOffsets;
	byte[] locations;
	RangeSectionReader rangeReader;
	ByteBuffer str;
	ByteBuffer lineStr;
	public SourceLinesTable lineInfo;
	public AddressTable addressTable;
	AbbreviationTable abbrTable;
	CallFrameInfo callFrameInfo;
	Map<Long, LocationList> locationLists = new HashMap<>();
	Map<Long, RangeList> rangeLists = new HashMap<>();
	String cuCompDir;
	LinkedList<String> entryFile = new LinkedList<>();

	public DwarfInfo(ELF elfFile) {
		this.compileUnits = new ArrayList<>();
		this.order = elfFile.getEndian();
		this.elfFile = elfFile;
		debugInfo = getDebugSection(DwarfConstants.DEBUG_INFO);
		if (debugInfo == null) {
			// stop reading, no debugInfo
		} else {
			locations = elfFile.getDebugSection(DwarfConstants.DEBUG_LOCATIONS);
			str = getDebugSectionAsBuffer(DwarfConstants.DEBUG_STR);
			readAbbreviationTable();
			readOptionalSections();
			compileUnits = readDebugInfo();
		}
		Platform.getLog(getClass()).info("Отладочная информация прочитана");
	}

	private ByteBuffer getDebugSectionAsBuffer(String sectionName) {
		return wrapByteArray(getDebugSection(sectionName));
	}

	private ByteBuffer wrapByteArray(byte[] section) {
		return ByteBuffer.wrap(section).order(order);
	}

	private void readAbbreviationTable() {
		ByteBuffer abbrTableBuffer = getDebugSectionAsBuffer(DwarfConstants.DEBUG_ABBREV);
		abbrTable = new AbbreviationTable(abbrTableBuffer);
	}

	private void readAllEntryFiles() {
		ByteBuffer buffer = ByteBuffer.wrap(debugInfo);
		buffer.rewind();
		buffer.order(order);
		while (buffer.hasRemaining()) {
			CompileUnitImpl unit = new CompileUnitImpl(this, buffer);
			unit.readCompileUnitTag(buffer);
			compileUnits.add(unit);
		}
	}

	private void readOptionalSections() {
		lineStr = wrapOptionalSection(DwarfConstants.DEBUG_LINES_STR);
		stringOffsets = wrapOptionalSection(DwarfConstants.DEBUG_STR_OFFSET);
		if (locations != null)
			readLocationLists();
		initDebugRangeReader();
		if (rangeReader != null)
			rangeLists = rangeReader.readRangesLists();

		addressTable = wrapOptionalSection(DwarfConstants.DEBUG_ADDRESSES, AddressTable::new);
		readAllEntryFiles();
		lineInfo = wrapOptionalSection(DwarfConstants.DEBUG_LINES, buf -> new SourceLinesTable(this, buf));
	}

	private <T> T wrapOptionalSection(String sectionName, Function<ByteBuffer, T> constructor) {
		byte[] section = elfFile.getDebugSection(sectionName);
		if (section != null) {
			ByteBuffer buffer = wrapByteArray(section);
			return constructor != null ? constructor.apply(buffer) : null;
		}
		return null;
	}

	private ByteBuffer wrapOptionalSection(String sectionName) {
		byte[] section = elfFile.getDebugSection(sectionName);
		return (section != null) ? wrapByteArray(section) : null;
	}

	private byte[] getDebugSection(String name) {
		byte[] result = elfFile.getDebugSection(name);
		if (result == null) {
			dwarfLOG.error("Section " + name + " not found in file");
		}
		return result;
	}

	private List<CompileUnitImpl> readDebugInfo() {
		if (tracing) {
			dwarfLOG.info("reading .debug_info from elf file");
		}
		ByteBuffer buffer = ByteBuffer.wrap(debugInfo);
		buffer.rewind();
		buffer.order(order);
		if (!compileUnits.isEmpty()) {
			for (CompileUnitImpl cu : compileUnits) {
				cu.initCompileUnit(buffer);
				if (tracing) {
					dwarfLOG.info(cu.toString());
				}
			}
		}
		if (debugInfo != null) {
			dispose();
			return compileUnits;
		}
		Platform.getLog(getClass()).error("There is no .debug_info section in elf file");
		throw new IllegalArgumentException("There is no .debug_info section in elf file");
	}

	private void dispose() {
		AttributeFactory.clear();
		rangeReader = null;
		rangeLists = null;
		locationLists = null;
		locations = null;
		str = null;
		stringOffsets = null;
		debugInfo = null;
		entryFile = null;
		abbrTable = null;
	}

	/**
	 * @param data
	 * @param signed
	 * @return
	 */
	public static long readLeb128(ByteBuffer data, boolean signed) {
		long result = 0;
		int shift = 0;
		byte b;
		while (true) {
			b = data.get();
			result |= (b & 0x7f) << shift;
			if ((b & 0x80) == 0) {
				break;
			}
			shift += 7;
		}
		if (signed && (shift < 64) && ((b & 0x40) != 0)) {
			result |= (~0 << shift);
		}

		return result;
	}

	static long read2bytes(byte[] data, ByteOrder order) {
		if (data.length != 2) {
			throw new IllegalArgumentException("expected array with length 2");
		}
		return ByteBuffer.wrap(data).order(order).getShort();
	}

	static long read4bytes(byte[] data, ByteOrder order) {
		if (data.length != 4) {
			throw new IllegalArgumentException("expected array with length 4");
		}
		return ByteBuffer.wrap(data).order(order).getInt();
	}

	static long read8bytes(byte[] data, ByteOrder order) {
		if (data.length != 8) {
			throw new IllegalArgumentException("expected array with length 8");
		}
		return ByteBuffer.wrap(data).order(order).getLong();
	}

	public String getString(int offset) {
		str.position(offset);
		StringBuilder sb = new StringBuilder();
		while (str.hasRemaining()) {
			byte sc = str.get();
			if (sc == 0) {
				break;
			}
			sb.append((char) sc);
		}
		return sb.toString();
	}

	public ELF getFile() {
		return this.elfFile;
	}

	void readLocationLists() {
		ByteBuffer section = ByteBuffer.wrap(locations);
		section.order(order);
		while (section.hasRemaining()) {
			long offset = section.position();
			LocationList list = new LocationList(section);
			section.position((int) (section.position() + list.getLength() - 8));
			locationLists.put(Long.valueOf(offset), list);
		}
	}

	private void initDebugRangeReader() {
		ByteBuffer buffer = null;
		byte[] rnglists = elfFile.getDebugSection(DwarfConstants.DEBUG_RNGLISTS);
		if (rnglists != null) {
			buffer = wrapByteArray(rnglists);
		} else {
			byte[] ranges = elfFile.getDebugSection(DwarfConstants.DEBUG_RANGES);
			if (ranges != null) {
				buffer = wrapByteArray(ranges);
			}
		}
		if (buffer != null) {
			rangeReader = new RangeSectionReader(buffer, this);
		}
	}

	public RangeSectionReader getRangeReader() {
		return rangeReader;
	}

	public long getStringOffset(long baseOffset, int offset, boolean dwarf32) {
		int size = dwarf32 ? 4 : 8;
		stringOffsets.position((int) (baseOffset + offset * size));
		if (size == 4) {
			return Integer.toUnsignedLong(stringOffsets.getInt());
		}
		return stringOffsets.getLong();
	}

	public LocationList getLocationList(long key) {
		return locationLists.get(Long.valueOf(key));
	}

	public RangeList getRangeList(long key) {
		return rangeLists.get(Long.valueOf(key));
	}

	public ByteBuffer getLineString() {
		return lineStr;
	}

	public AbbrCompileUnit getAbbr(long offset) {
		return abbrTable.getAbbrCompileUnit(offset);
	}

	@Override
	public ICompileUnit getCompileUnit(long address) {
		for (CompileUnitImpl unit : compileUnits) {
			if ((unit.getAddressStart() <= address) && (unit.getAddressEnd() > address)) {
				List<RangeEntry> rangeEntries = unit.getRangeEntries();
				if (!rangeEntries.isEmpty()) {
					for (RangeEntry range : rangeEntries) {
						if ((range.getStartAddress() <= address) && (range.getEndAddress() > address)) {
							return unit;
						}
					}
				} else {
					return unit;
				}
			}
		}
		return null;
	}

	@Override
	public BreakpointInfo getBreakpointAddress(IFile file, int line) {
		for (ICompileUnit cu : compileUnits) {
			if (cu.containsFile(file)) {
				return cu.getBreakpointAddress(file, line);
			}
		}
		return new BreakpointInfo(-1, -1L);
	}

	public String getCuCompDir() {
		return cuCompDir;
	}

	public void setCuCompDir(String cuCompDir) {
		this.cuCompDir = cuCompDir;
	}

	public void addEntryFile(String entryFile) {
		this.entryFile.add(entryFile);
	}

	public String getEntryFile() {
		return entryFile.isEmpty() ? "" : entryFile.removeFirst();
	}

	@Override
	public IStackInfo getStackInfo() {
		return callFrameInfo;
	}

	public List<CompileUnitImpl> getAllCu() {
		return compileUnits;
	}
}