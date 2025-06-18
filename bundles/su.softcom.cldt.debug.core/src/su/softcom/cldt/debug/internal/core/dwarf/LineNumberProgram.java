package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.internal.core.dwarf.values.Value;

public class LineNumberProgram {

	private final SourceLinesTable sourceLinesTable;

	public long length;
	private final int version;
	private short addressSize;
	private short segmentSelectorSize;
	private final long headerLength;
	private final short minimumInstructionLength;
	private final short maximumOperationPerInstruction;
	private final short defaultStmt;
	private final byte lineBase;
	private final short lineRange;
	private final short opcodeBase;
	private final List<Short> standartOpcodeLength;
	private short directoryEntryFormatCount;
	private long directoriesCount;
	private short filesEntryFormatCount;
	private long filesCount;
	private List<FolderDescriptor> directories;
	private List<FileDescriptor> files;
	private final TreeMap<Long, MatrixRow> matrix = new TreeMap<>();

	class MatrixRow {
		final boolean prologue;
		final boolean epilogue;
		final long file;
		final long line;
		final boolean isStmt;

		public MatrixRow(boolean prologue, boolean epilogue, long file, long line, boolean isStmt) {
			this.prologue = prologue;
			this.epilogue = epilogue;
			this.file = file;
			this.line = line;
			this.isStmt = isStmt;
		}

		public FileDescriptor getFileDescriptor() {
			return files.get((int) file);
		}

		public int getLine() {
			return (int) line;
		}

		@Override
		public String toString() {
			return "file: " + file + "line: " + line + "prologue: " + prologue + "epilogue" + epilogue;
		}

		public boolean statement() {
			return isStmt;
		}
	}

	public LineNumberProgram(SourceLinesTable sourceLinesTable, ByteBuffer debugLine, int offset) {
		this.sourceLinesTable = sourceLinesTable;
		version = debugLine.getShort();
		if (version == 5) {
			addressSize = debugLine.get();
			segmentSelectorSize = debugLine.get();
		}
		headerLength = offset == 4 ? debugLine.getInt() : debugLine.getLong();
		minimumInstructionLength = debugLine.get();
		maximumOperationPerInstruction = debugLine.get();
		defaultStmt = debugLine.get();
		lineBase = debugLine.get();
		lineRange = debugLine.get();
		opcodeBase = debugLine.get();
		standartOpcodeLength = new ArrayList<>();
		for (int i = 0; i < opcodeBase - 1; i++) {
			standartOpcodeLength.add((short) debugLine.get());
		}

		if (version == 5) {
			readSourceFilesDwarf5(debugLine);
		} else {
			readSourceFiles(debugLine);
		}

		getMatrix(debugLine);
	}

	private void readSourceFiles(ByteBuffer debugLine) {
		directories = new ArrayList<>();
		String cuCompDir = sourceLinesTable.info.getCuCompDir();
		directories.add(new FolderDescriptor(cuCompDir));

		while (true) {
			String str = readString(debugLine);
			if (str.isEmpty()) {
				break;
			}

			directories.add(new FolderDescriptor(isAbsolutePath(cuCompDir, str)));
		}

		files = new ArrayList<>();
		String entryFile = sourceLinesTable.info.getEntryFile();
		files.add(new FileDescriptor(isAbsolutePath(cuCompDir, entryFile)));
		while (true) {
			String fileName = readString(debugLine);
			if (fileName.isEmpty()) { // null bytes
				break;
			}

			// read dirIndex
			long dirIndex = sourceLinesTable.readUleb128(debugLine);

			files.add(new FileDescriptor(directories.get((int) dirIndex), fileName));

			// skip modification time and file size in bytes
			sourceLinesTable.readUleb128(debugLine);
			sourceLinesTable.readUleb128(debugLine);
		}
	}

	private String isAbsolutePath(String directory, String str) {
		Path path = Paths.get(str);
		if (!path.isAbsolute()) {
			path = Paths.get(directory + "/" + str);
		}
		return path.toString();
	}

	private void readSourceFilesDwarf5(ByteBuffer debugLine) {
		directoryEntryFormatCount = debugLine.get();
		Map<Long, Long> directoryEntryFormat = new HashMap<>();
		for (int i = 0; i < directoryEntryFormatCount; i++) {
			directoryEntryFormat.put(SourceLinesTable.readUleb128(debugLine), SourceLinesTable.readUleb128(debugLine));
		}
		directoriesCount = SourceLinesTable.readUleb128(debugLine);
		directories = new ArrayList<>();
		readFolders(debugLine, directoryEntryFormat);
		filesEntryFormatCount = debugLine.get();
		Map<Long, Long> filesEntryFormat = new HashMap<>();
		for (int i = 0; i < filesEntryFormatCount; i++) {
			filesEntryFormat.put(SourceLinesTable.readUleb128(debugLine), SourceLinesTable.readUleb128(debugLine));
		}
		filesCount = SourceLinesTable.readUleb128(debugLine);
		files = new ArrayList<>();
		readFiles(debugLine, filesEntryFormat);
	}

	String readString(ByteBuffer data) {
		String str;

		StringBuilder sb = new StringBuilder();
		while (data.hasRemaining()) {
			byte c = data.get();
			if (c == 0) {
				break;
			}
			sb.append((char) c);
		}

		str = sb.toString();
		return str;
	}

	void readFolders(ByteBuffer buffer, Map<Long, Long> format) {
		for (int i = 0; i < directoriesCount; i++) {
			String name = "";
			for (Entry<Long, Long> e : format.entrySet()) {
				switch (e.getKey().intValue()) {
				case DwarfConstants.DW_LNCT_path:
					name = (String) new Value(this.sourceLinesTable.info, e.getValue().intValue(), buffer).getValue();
					break;
				default:
					Platform.getLog(getClass()).info("Skipped: " + e.getKey().intValue() + " DW_LNCT in folders");
					;
				}
			}
			if (name.startsWith("/")) {
				directories.add(new FolderDescriptor(name));
			} else {
				directories.add(new FolderDescriptor(directories.get(0), name));
			}
		}
	}

	void readFiles(ByteBuffer buffer, Map<Long, Long> format) {
		for (int i = 0; i < filesCount; i++) {
			String name = "";
			long folder = -1;
			for (Entry<Long, Long> e : format.entrySet()) {
				Value v = new Value(this.sourceLinesTable.info, e.getValue().intValue(), buffer);
				switch (e.getKey().intValue()) {
				case DwarfConstants.DW_LNCT_path:
					name = (String) v.getValue();
					break;
				case DwarfConstants.DW_LNCT_directory_index:
					folder = ((Long) v.getValue()).longValue();
					break;
				default:
					Platform.getLog(getClass()).info("Skipped: " + e.getKey().intValue() + " DW_LNCT in files");
					break;
				}
			}

			if (folder == -1 || name.startsWith("/")) {
				files.add(new FileDescriptor(name));
			} else {
				files.add(new FileDescriptor(directories.get((int) folder), name));
			}
		}

	}

	public MatrixRow getRow(long address) {
		return matrix.get(Long.valueOf(address));
	}

	/**
	 * Функция находит адрес конца пролога для адреса, являющегося началом функции
	 * 
	 * @param startAddress
	 * @return адрес конца пролога
	 */
	public long getPrologEndAddress(long startAddress) {
		Entry<Long, MatrixRow> currentEntry = matrix.floorEntry(Long.valueOf(startAddress));

		while (currentEntry != null && !currentEntry.getValue().prologue) {
			currentEntry = matrix.higherEntry(currentEntry.getKey());
		}

		return currentEntry != null ? currentEntry.getKey() : 0L;
	}

	void getMatrix(ByteBuffer data) {
		long address = 0;
		long opIndex = 0;
		long file = 1;
		long line = 1;
		long column = 0;
		boolean isStmt = defaultStmt == 1;
		boolean basicBlock = false;
		boolean endSequence = false;
		boolean prologueEnd = false;
		boolean epilogueBegin = false;
		long isa = 0;
		long desc = 0;
		while (data.hasRemaining()) {
			int opcode = Byte.toUnsignedInt(data.get());
			if (opcode == 0) {
				long length = SourceLinesTable.readLeb128(data);
				byte instruction = data.get();
				switch (instruction) {
				case DwarfConstants.DW_LNE_end_sequence:
					endSequence = true;
					matrix.put(Long.valueOf(address), new MatrixRow(prologueEnd, epilogueBegin, file, line, isStmt));
					address = 0;
					opIndex = 0;
					file = 1;
					line = 1;
					column = 0;
					isStmt = defaultStmt == 1;
					basicBlock = false;
					endSequence = false;
					prologueEnd = false;
					epilogueBegin = false;
					isa = 0;
					desc = 0;
					break;
				case DwarfConstants.DW_LNE_set_address:
					if (length == 5) {
						address = data.getInt();
					} else {
						address = data.getLong();
					}
					opIndex = 0;
					break;
				case DwarfConstants.DW_LNE_set_descriminator:
					desc = SourceLinesTable.readUleb128(data);
					break;
				default:
					Platform.getLog(getClass()).info("Skipped: " + instruction + "extended opcode");
					break;
				}
				// TODO: extended opcode
			} else {
				int adjustedOpcode = 0;
				int operationAdvance = 0;
				switch (opcode) {
				case DwarfConstants.DW_LNS_copy:
					matrix.put(Long.valueOf(address), new MatrixRow(prologueEnd, epilogueBegin, file, line, isStmt));
					desc = 0;
					prologueEnd = false;
					epilogueBegin = false;
					break;
				case DwarfConstants.DW_LNS_advance_pc:
					long arg = SourceLinesTable.readUleb128(data);
					address += minimumInstructionLength * arg;
					break;
				case DwarfConstants.DW_LNS_advance_line:
					line += SourceLinesTable.readLeb128(data);
					break;
				case DwarfConstants.DW_LNS_set_file:
					file = SourceLinesTable.readUleb128(data);
					break;
				case DwarfConstants.DW_LNS_set_column:
					column = SourceLinesTable.readUleb128(data);
					break;
				case DwarfConstants.DW_LNS_negate_stmt:
					isStmt = !isStmt;
					break;
				case DwarfConstants.DW_LNS_set_basic_block:
					basicBlock = true;
					break;
				case DwarfConstants.DW_LNS_const_add_pc:
					operationAdvance = (255 - opcodeBase) / lineRange;
					address += minimumInstructionLength
							* ((opIndex + operationAdvance) / maximumOperationPerInstruction);
					opIndex = (opIndex + operationAdvance) % maximumOperationPerInstruction;
					break;
				case DwarfConstants.DW_LNS_fixed_advance_pc:
					address += Short.toUnsignedInt(data.getShort());
					opIndex = 0;
					break;
				case DwarfConstants.DW_LNS_set_prologue_end:
					prologueEnd = true;
					break;
				case DwarfConstants.DW_LNS_set_epilogue_begin:
					epilogueBegin = true;
					break;
				case DwarfConstants.DW_LNS_set_isa:
					isa = SourceLinesTable.readUleb128(data);
					break;
				default:
					adjustedOpcode = opcode - opcodeBase;
					operationAdvance = adjustedOpcode / lineRange;
					address += minimumInstructionLength
							* ((opIndex + operationAdvance) / maximumOperationPerInstruction);
					opIndex = (opIndex + operationAdvance) % maximumOperationPerInstruction;
					line += lineBase + (adjustedOpcode % lineRange);
					matrix.put(Long.valueOf(address), new MatrixRow(prologueEnd, epilogueBegin, file, line, isStmt));
					basicBlock = false;
					prologueEnd = false;
					epilogueBegin = false;
					desc = 0;
					break;
				}
			}
		}
	}

	public boolean hasAddress(long address) {
		return matrix.get(Long.valueOf(address)) != null;
	}

	public boolean isStatementAddress(long address) {
		MatrixRow row = matrix.get(Long.valueOf(address));
		return row != null ? row.isStmt : false;
	}

	public List<FileDescriptor> getFiles() {
		return files;
	}

	public Long getPreviousAddress(long address) {
		Entry<Long, MatrixRow> currentRow = matrix.lowerEntry(address);
		return currentRow != null ? currentRow.getKey() : null;
	}

	/**
	 * A method that should return the address in memory related to the line number.
	 * If there is no matching row in MatrixRow, then we check if there are matching
	 * rows after the required one. If there are, then select it and take this
	 * address and put a breakpoint on it
	 *
	 * @param file The file for which the breakpoint address is to be retrieved.
	 * @param line The line number to check for a corresponding memory address.
	 * @return A {@link BreakpointInfo} object containing the line number and memory
	 *         address. If no matching row is found, returns a
	 *         {@link BreakpointInfo} with values (-1, -1L).
	 */
	public BreakpointInfo getBreakpointAddress(IFile file, int line) {
		int currentLine = line;
		Path currentPath = file.getLocation().toFile().toPath();
		while (true) {
			Entry<Long, MatrixRow> foundEntry = null;

			for (Entry<Long, MatrixRow> entry : matrix.entrySet()) {
				MatrixRow value = entry.getValue();
				if (value.getFileDescriptor().getPath().equals(currentPath) && value.getLine() == currentLine
						&& value.statement()) {
					foundEntry = entry;
					break;
				}

			}

			if (foundEntry != null) {
				return new BreakpointInfo(currentLine, foundEntry.getKey().longValue());
			}

			int lineCheck = currentLine;
			MatrixRow suitableRow = matrix.entrySet().stream().filter(row -> row.getValue().getLine() > lineCheck)
					.map(Entry::getValue).findFirst().orElse(null);

			if (suitableRow != null) {
				currentLine = suitableRow.getLine();
			} else {
				break;
			}

		}
		return new BreakpointInfo(-1, -1L);
	}
}
