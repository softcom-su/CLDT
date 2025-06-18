package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;
import java.nio.file.Path;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IDebugVariable;
import su.softcom.cldt.debug.core.IFunction;
import su.softcom.cldt.debug.core.dwarf.DwarfInfo;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.AbbreviationTable.AbbrCompileUnit;
import su.softcom.cldt.debug.internal.core.dwarf.AddressTable.AddressTableItem;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.dwarf.attr.AttributeFactory;
import su.softcom.cldt.debug.internal.core.dwarf.tags.CompileUnitTag;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Subprogram;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Tag;
import su.softcom.cldt.debug.internal.core.dwarf.values.Value;

public class CompileUnitImpl implements ICompileUnit {

	private final DwarfInfo dwarfInfo;

	private int headerSize;
	private int offset;
	private int fullOffset;
	private int addressSize;
	private int position;

	private long length;
	private long abbreviationOffset;
	private long strOffsetBase;
	private long addressStart;
	private long addressEnd;

	private int version;
	private int unitType;

	private int[] functions;

	private List<IDebugVariable> variables;
	private Map<Integer, Tag> tagByOffset = new HashMap<>();
	private List<RangeEntry> rangeEntries = new ArrayList<>();

	private LineNumberProgram lnp;
	private AddressTableItem addressTable;
	private AbbrCompileUnit abbreviations;
	private Tag root;

	/**
	 * 
	 * 
	 * @param dwarfInfo
	 * @param buffer
	 * 
	 * @throws IllegalArgumentException if dwarfInfo is {@code null}
	 */
	public CompileUnitImpl(DwarfInfo dwarfInfo, ByteBuffer buffer) {
		if (dwarfInfo == null) {
			throw new IllegalArgumentException("dwarfInfo required");
		}
		this.dwarfInfo = dwarfInfo;
		setFullOffset(buffer.position());
		length = Integer.toUnsignedLong(buffer.getInt());
		setOffset(4);
		if ((length == 0xffffffffL) || (length == 0)) {
			buffer.position(buffer.position() - 4);
			length = buffer.getLong();
			setOffset(8);
		}
		version = buffer.getShort();
		if (version == 5) {
			readDwarf5Header(buffer);
		} else {
			if (getOffset() == 8) {
				abbreviationOffset = buffer.getLong();
				headerSize = 19;
			} else {
				abbreviationOffset = buffer.getInt();
				headerSize = 11;
			}
			setAddressSize(buffer.get());
			headerSize -= getOffset();
		}
		abbreviations = dwarfInfo.getAbbr(abbreviationOffset);
		position = buffer.position();
	}

	public void initCompileUnit(ByteBuffer buffer) {
		buffer.position(position);
		readDebugInfoEntry(buffer, (int) (buffer.position() + length - headerSize));
		initLNP();
		resolve();
		setParameters();
	}

	private void initLNP() {
		if (root instanceof CompileUnitTag compileUnitTag) {
			Attribute stmtListAttr = compileUnitTag.getArg(DwarfConstants.DW_AT_stmt_list);
			if (stmtListAttr != null) {
				long stmtList = stmtListAttr.getValue().getUnsignedLongValue();
				lnp = this.dwarfInfo.lineInfo.getLNP(stmtList);
			}
		}
	}

	private void resolve() {
		ArrayDeque<Tag> stack = new ArrayDeque<>();
		List<Integer> tmpFunctions = new ArrayList<>();
		stack.push(root);
		while (!stack.isEmpty()) {
			Tag currentTag = stack.pop();

			if (currentTag instanceof IResolvableTag resolvable) {
				resolvable.resolve();
			}

			for (Integer childOffset : currentTag.getChildren()) {
				Tag child = getTagByOffset(childOffset);
				if (child instanceof Subprogram subprogram && subprogram.getStartAddress() != 0) {
					subprogram.setPrologAddress(lnp.getPrologEndAddress(subprogram.getStartAddress()));
					tmpFunctions.add(childOffset);
				}
				stack.push(child);
			}
		}
		functions = tmpFunctions.stream().mapToInt(Integer::intValue).toArray();
	}

	public void readCompileUnitTag(ByteBuffer buffer) {
		buffer.position(position);
		long code = DwarfInfo.readLeb128(buffer, false);
		if (code != 0 && version == 4) {
			Tag tag = abbreviations.getEntryByCode(code);
			if (tag != null) {
				readCUTagInfo(tag, buffer);
			}
		}
		buffer.position((int) (position + length - headerSize));
	}

	void setParameters() {
		if (root instanceof CompileUnitTag compileUnitTag && compileUnitTag.getLowPC() != -1) {
			addressStart = compileUnitTag.getLowPC();

			if (compileUnitTag.getHighPC() == -1 && compileUnitTag.getRanges() != -1) {
				if (version == 5) {
					RangeList rangeList = dwarfInfo.getRangeList(compileUnitTag.getRangeBase());
					rangeEntries = dwarfInfo.getRangeReader().readDebugRanges(rangeList, addressTable,
							compileUnitTag.getRanges());
				} else {
					rangeEntries = dwarfInfo.getRangeReader().readDebugRanges(compileUnitTag.getRanges(), addressStart,
							addressSize);
				}
				Collections.sort(rangeEntries,
						(entry1, entry2) -> (int) (entry1.getStartAddress() - entry2.getStartAddress()));
				if (addressStart == 0) {
					addressStart = rangeEntries.get(0).getStartAddress();
				}
				addressEnd = rangeEntries.get(rangeEntries.size() - 1).getEndAddress();
				Platform.getLog(getClass()).info("CU with " + rangeEntries.size() + " range entries");
			} else {
				addressEnd = compileUnitTag.getHighPC();
			}
		}
	}

	void readDebugInfoEntry(ByteBuffer data, int limit) {
		Tag parent = null;
		List<Integer> childrens = new ArrayList<>();

		while (data.position() < limit) {
			int pos = data.position();
			long code = DwarfInfo.readLeb128(data, false);
			Tag entry = null;
			if (code != 0) {
				Tag proto = dwarfInfo.getAbbr(abbreviationOffset).getEntryByCode(code);
				if (proto != null) {
					entry = createTagFromProto(proto, data);
					entry.setOffset(pos);
					tagByOffset.put(pos, entry);
					if (root == null) {
						root = entry;
						parent = entry;
					}
					if (entry != root) {
						childrens.add(pos);
						entry.setParent(parent);
					}
					if (entry.isHasChild() && (entry != root)) {
						updateParentChildrens(parent, childrens);
						parent = entry;
					}
				}
			} else {
				if (!childrens.isEmpty()) {
					updateParentChildrens(parent, childrens);
				}

				parent = parent.getParent();

			}

			if (dwarfInfo.tracing) {
				System.out.println("Abbreviation: code " + code + " value: " + entry);
			}
		}
	}

	private void updateParentChildrens(Tag parent, List<Integer> childrens) {
		List<Integer> childrensList = new ArrayList<>();

		if (parent.getChildrens() != null) {
			for (int child : parent.getChildrens()) {
				childrensList.add(child);
			}
		}

		childrensList.addAll(childrens);
		parent.setChildrens(childrensList.stream().mapToInt(Integer::intValue).toArray());
		childrens.clear();
	}

	Tag createTagFromProto(Tag proto, ByteBuffer data) {
		List<Attribute> tmp = new ArrayList<>();
		boolean isCuTag = (proto.getTag() == DwarfConstants.DW_TAG_compile_unit);
		for (Attribute a : proto.getArgs()) {
			Attribute attr = AttributeFactory.createAttribute(a, data, dwarfInfo, this);
			if (this.dwarfInfo.tracing) {
				System.out.println(a.toString());
			}
			if (isCuTag && a.name == DwarfConstants.DW_AT_str_offset_base) {
				strOffsetBase = ((Long) attr.getValue().getValue()).longValue();
			} else if (isCuTag && a.name == DwarfConstants.DW_AT_addr_base) {
				addressTable = this.dwarfInfo.addressTable.getTableItem(attr.getValue().getUnsignedLongValue());
			} else {
				tmp.add(attr);
			}
		}

		if (isCuTag) {
			getArgsByForm(tmp, DwarfConstants.DW_FORM_strx1).stream().map(Attribute::getValue)
					.filter(val -> val.getValue() instanceof Long)
					.forEach(val -> val.getString((Long) val.getValue(), this, dwarfInfo));
		}

		return Tag.resolveTag(proto, tmp.toArray(new Attribute[0]), this);
	}

	private void readCUTagInfo(Tag tag, ByteBuffer buffer) {
		for (Attribute a : tag.getArgs()) {
			int attributeName = a.getName();
			int form = a.getForm();
			Value v = Value.getValue(form, buffer, this, dwarfInfo);

			if (attributeName == DwarfConstants.DW_AT_comp_dir || attributeName == DwarfConstants.DW_AT_name) {
				String value = v.getValue() instanceof String str ? str : "";
				switch (attributeName) {
				case DwarfConstants.DW_AT_comp_dir:
					dwarfInfo.setCuCompDir(value);
					break;
				case DwarfConstants.DW_AT_name:
					dwarfInfo.addEntryFile(value);
					break;
				default:
					break;
				}
			}
		}
	}

	public List<Attribute> getArgsByForm(List<Attribute> args, int form) {
		return args.stream().filter(a -> a.getForm() == form).collect(Collectors.toList());
	}

	void readDwarf5Header(ByteBuffer data) {
		unitType = Byte.toUnsignedInt(data.get());
		setAddressSize(Byte.toUnsignedInt(data.get()));
		if (getOffset() == 8) {
			abbreviationOffset = data.getLong();
			headerSize = 16;
		} else {
			abbreviationOffset = Integer.toUnsignedLong(data.getInt());
			headerSize = 12;
		}
		headerSize -= getOffset();
	}

	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("CompileUnit");
		return builder.toString();
	}

	public long getAddress(int index) {
		return addressTable.getAddress(index);
	}

	/**
	 * Get path to source file by address
	 * 
	 * @param address
	 * @return Path source file or {@code null} if file not exists
	 */
	public Path getFilePath(long address) {
		if (lnp.getRow(address) != null) {
			return lnp.getRow(address).getFileDescriptor().getPath();
		}
		return null;
	}

	/**
	 * Get path to source file by index in line number program
	 * 
	 * @param address
	 * @return Path source file or {@code null} if file not exists
	 */
	public Path getFilePath(int index) {
		return lnp.getFiles().get(index).getPath();
	}

	public int getLine(long address) {
		if (lnp.getRow(address) != null) {
			return lnp.getRow(address).getLine();
		}
		return -1;
	}

	public long getPreviousAddress(long currentAddress) {
		Long address = lnp.getPreviousAddress(currentAddress);
		return address != null ? address : 0;
	}

	public int getColumn(long address) {
		return 0;
	}

	public List<RangeEntry> getRangeEntries() {
		return rangeEntries;
	}

	@Override
	public IFunction getFunction(long address) {
		for (int funcOffset : functions) {
			if (getTagByOffset(funcOffset) instanceof IFunction f && f.contents(address)) {
				return f;
			}
		}
		return null;
	}

	@Override
	public List<IDebugVariable> getVariables() {
		return variables;
	}

	@Override
	public boolean containsAddress(long address) {
		return lnp.hasAddress(address);
	}

	public boolean isStatementAddress(long address) {
		return lnp.isStatementAddress(address);
	}

	public long getStrOffsetBase() {
		return strOffsetBase;
	}

	public int getAddressSize() {
		return addressSize;
	}

	public void setAddressSize(int addressSize) {
		this.addressSize = addressSize;
	}

	public int getOffset() {
		return offset;
	}

	public void setOffset(int offset) {
		this.offset = offset;
	}

	public int getFullOffset() {
		return fullOffset;
	}

	public void setFullOffset(int fullOffset) {
		this.fullOffset = fullOffset;
	}

	public long getAddressStart() {
		return addressStart;
	}

	public long getAddressEnd() {
		return addressEnd;
	}

	@Override
	public boolean containsFile(IFile file) {
		Path filePath = file.getLocation().toFile().toPath();
		for (FileDescriptor f : lnp.getFiles()) {
			if (f.getPath().equals(filePath)) {
				return true;
			}
		}
		return false;
	}

	@Override
	public BreakpointInfo getBreakpointAddress(IFile file, int line) {
		return lnp.getBreakpointAddress(file, line);
	}

	public Tag getTagByOffset(int offset) {
		return tagByOffset.get(offset);
	}

	public List<IDebugVariable> getVariables(IFunction function, long currentLine) {
		List<IDebugVariable> listVariables = new ArrayList<>();
		if (function == null) {
			return listVariables;
		}
		ArrayDeque<Tag> stack = new ArrayDeque<>();
		stack.push((Subprogram) function);
		while (!stack.isEmpty()) {
			Tag currentTag = stack.pop();

			if (currentTag.hasChildren()) {
				for (Integer childOffset : currentTag.getChildren()) {
					Tag child = getTagByOffset(childOffset);
					if (child instanceof IDebugVariable debugVariable) {
						if (!debugVariable.isArtificial() && debugVariable.getLine() != -1) {
							listVariables.add(debugVariable);
						}

						if (debugVariable.getLine() >= currentLine) {
							break;
						}
					} else {
						stack.push(child);
					}
				}
			}
		}
		return listVariables;
	}

}
