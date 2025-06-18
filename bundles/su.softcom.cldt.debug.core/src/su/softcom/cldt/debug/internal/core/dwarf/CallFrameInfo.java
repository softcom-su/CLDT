package su.softcom.cldt.debug.internal.core.dwarf;

import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Deque;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import javax.swing.plaf.basic.BasicInternalFrameTitlePane.RestoreAction;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IFunction;
import su.softcom.cldt.debug.core.IStackInfo;
import su.softcom.cldt.debug.core.info.IDebugInfo;
import su.softcom.cldt.debug.internal.core.GDBClient;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.ICallFrameInstruction;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.ACreationRowCFA;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.CFAFactory;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.RememberStateCFA;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.RestoreStateCFA;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.reg.IRegRule;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.reg.RuleRegister;
import su.softcom.cldt.debug.internal.core.dwarf.op.Operation;

public class CallFrameInfo extends DwarfElement implements IStackInfo {

	public class CFARule {

		long reg;
		long offset;
		byte[] expression;

		public void setReg(long reg) {
			this.reg = reg;
			if (expression != null) {
				this.expression = null;
			}
		}

		public void setOffset(long offset) {
			this.offset = offset;
			if (expression != null) {
				this.expression = null;
			}
		}

		public long getReg() {
			return reg;
		}

		public long getOffset() {
			return offset;
		}

		public byte[] getExpression() {
			return expression;
		}

		public void setExpression(byte[] expression) {
			this.expression = expression;
			if (expression != null) {
				this.reg = -1;
				this.offset = -1;
			}
		}

	}

	public class TableRow {
		
		Map<Integer, IRegRule> regRules = new HashMap<>();
		CFARule cfaRule = new CFARule();
		long address;

		public TableRow() {
			
		}

		public TableRow(TableRow prevRow, long address) {
			this.regRules = new HashMap<>(prevRow.getRegRules());
			this.address = address;
			cfaRule.offset = prevRow.getCFARule().offset;
			cfaRule.reg = prevRow.getCFARule().reg;
			cfaRule.expression = prevRow.getCFARule().expression;
		}

		public TableRow(TableRow prevRow, ACreationRowCFA operation) {
			this(prevRow, operation.getAddress(prevRow));
		}

		public void setAddress(long address) {
			this.address = address;
		}
		
		public void setRegister(long address) {
			this.address = address;
		}

		public long getAddress() {
			return address;
		}
		
		public void setRule(int regiter, IRegRule rule) {
			this.regRules.put(regiter, rule);
		}
 
		public IRegRule getRegRule(int register) {
			return regRules.get(register);
		}

		public CFARule getCFARule() {
			return cfaRule;
		}
		
		public Map<Integer, IRegRule> getRegRules() {
			return regRules;
		}

		public Map<Integer, IRegRule> getChangedRegs(Map<Integer, BigInteger> regsValues) {
			Map<Integer, IRegRule> result = new HashMap<>();
			for (int i = 0; i < regsValues.size(); i++) {
				IRegRule rule = getRegRule(i);
				if (rule != null) {
					result.put(i, rule);
				}
			}
			return Collections.unmodifiableMap(result);
		}

		// FIXME refactor!!!
		public long getCFAAddress(GDBClient client, Map<Integer, BigInteger> oldRegs, IFunction function,
				ICompileUnit cu) {
			if (cfaRule.getExpression() != null) {
				return Operation.evaluateExpression(client, oldRegs, function, cu, cfaRule.getExpression());
			}
			long registerValue = oldRegs.get((int) cfaRule.getReg()).longValue();
			return registerValue + cfaRule.getOffset();
		}

		public void setRegRules(Map<Integer, IRegRule> restored) {
			this.regRules = restored;
		}

	}

	public class CommonInfoEntry {

		private int version;
		private int addressSize;
		private int segmentSelectorSize;

		private long ehData;
		private long codeAlignmentFactor;
		private long dataAlignmentFactor;
		private long retAddressReg;

		private long augmentationLength;
		private long personalityOffsetInCIE;
		private long personality;
		private boolean hasAugmentationData = false;
		private long augmentationData;
		private byte lsdaEncoding;

		private TableRow startRow;
		private String augmentation;

		CommonInfoEntry(long length, ByteBuffer data) {
			int start = data.position() - (DWARF32 ? 4 : 8);
			version = Byte.toUnsignedInt(data.get());
			StringBuilder sb = new StringBuilder();
			byte c = data.get();
			while (c != 0) {
				sb.append((char) c);
				c = data.get();
			}
			augmentation = sb.toString();
			if ("eh".equals(augmentation)) {
				ehData = DWARF32 ? data.getInt() : data.getLong();
			}
			codeAlignmentFactor = readUleb128(data);
			setDataAlignmentFactor(readLeb128(data));
			retAddressReg = readUleb128(data);

			if (augmentation.startsWith("z")) {
				augmentationLength = readUleb128(data);
				for (char symbol : augmentation.toCharArray()) {
					if (symbol == 'z') {
						hasAugmentationData = true;
						continue;
					}
					if (symbol == 'R') {
						augmentationData = data.get();
					} else if (symbol == 'P') {
						long code = data.get() & 0xFF;
						personalityOffsetInCIE = data.position();
						personality = readEncodedValue(data, (int) code);
					} else if (symbol == 'L') {
						lsdaEncoding = data.get();

					}
				}
			}
			startRow = new TableRow();
			while (data.position() < start + length) {
				ICallFrameInstruction instr = CFAFactory.getCFA(data, this);
				if (instr != null) {
					instr.updateRow(startRow);
				}
			}
		}

		public String getAugmentation() {
			return augmentation;
		}

		public long getDataAlignmentFactor() {
			return dataAlignmentFactor;
		}

		public void setDataAlignmentFactor(long dataAlignmentFactor) {
			this.dataAlignmentFactor = dataAlignmentFactor;
		}

		public boolean isHasAugmentationData() {
			return hasAugmentationData;
		}

		public byte getLsdaEncoding() {
			return lsdaEncoding;
		}

		public long getRetAddressReg() {
			return retAddressReg;
		}
	}

	class FrameDescriptionEntry {

		CommonInfoEntry root;
		Map<Long, TableRow> rows = new TreeMap<>();
		long startAddress;
		long endAddress;
		long length;
		long augmentationLength;
		long augmentationData;
		long lsda;

		FrameDescriptionEntry(long length, ByteBuffer data, CommonInfoEntry cie) {
			int start = data.position() - (DWARF32 ? 4 : 8);
			root = cie;
			if (cie == null) {
				System.out.println();
			}
			long pcBegin = readEncodedValue(data, (int) cie.augmentationData);
			long pcRange = readEncodedValue(data, (int) cie.augmentationData & 0x0F);

			startAddress = pcBegin;
			endAddress = pcBegin + pcRange;
			TableRow row = new TableRow(cie.startRow, startAddress);
			Deque<Map<Integer, IRegRule>> stack = new ArrayDeque<>();
			if (cie.isHasAugmentationData()) {
				augmentationLength = readUleb128(data);
				if (cie.getLsdaEncoding() != 0x80) {
					int lsdaStart = data.position();
					if (readEncodedValue(data, cie.getLsdaEncoding() & 0x0F) != 0) {
						data.position(lsdaStart);
						lsda = readEncodedValue(data, cie.getLsdaEncoding());
					}
				}
			}
			while (data.position() < start + length) {
				ICallFrameInstruction instr = CFAFactory.getCFA(data, cie);
				if (instr instanceof ACreationRowCFA cfaRow) {
					rows.put(Long.valueOf(row.getAddress()), row);
					row = new TableRow(row, cfaRow);
				} else if (instr instanceof RememberStateCFA) {
					stack.push(row.getRegRules());
				} else if (instr instanceof RestoreStateCFA) {
					row.setRegRules(stack.pop());
				} else if (instr != null) {
					instr.updateRow(row);
				}
			}
			rows.put(Long.valueOf(row.getAddress()), row);
		}

		public TableRow getRow(long address) {
			TableRow res = rows.get(Long.valueOf(startAddress));
			if (rows.containsKey(Long.valueOf(address))) {
				res = rows.get(Long.valueOf(address));
			} else {
				for (Entry<Long, TableRow> e : rows.entrySet()) {
					if (e.getKey() > Long.valueOf(address)) {
						break;
					}
					res = e.getValue();
				}
			}
			return res;
		}

		public boolean contains(long address) {
			return (startAddress <= address) && (endAddress > address);
		}

		public CommonInfoEntry getRoot() {
			return root;
		}
	}

	long posCurrentSection = 0;
	Map<Long, CommonInfoEntry> cies = new HashMap<>();
	List<FrameDescriptionEntry> fdes = new ArrayList<>();
	IDebugInfo info;

	public CallFrameInfo(ByteBuffer data, long posCurrentSection) {
		this.posCurrentSection = posCurrentSection;
		while (data.hasRemaining()) {
			data.order(ByteOrder.LITTLE_ENDIAN);
			long position = data.position();
			long length = getLength(data);
			if (length == 0) {
				break;
			}
			long isCie = Integer.toUnsignedLong(data.getInt());
			if (isCie == 0) {
				cies.put(Long.valueOf(position), new CommonInfoEntry(length, data));
			} else {
				CommonInfoEntry currentCie = cies.get(Long.valueOf(data.position() - 4 - isCie));
				fdes.add(new FrameDescriptionEntry(length, data, currentCie));
			}
		}
	}

	public long readEncodedValue(ByteBuffer buffer, int encoding) {
		if (encoding == 0) {
			return 0;
		}
		long base = buffer.position();

		long value;
		switch (encoding & 0x0F) {
		case 0x01:
			value = readUleb128(buffer);
			break;
		case 0x02:
			value = Short.toUnsignedInt(buffer.getShort());
			break;
		case 0x03:
			value = Integer.toUnsignedLong(buffer.getInt());
			break;
		case 0x04:
			value = buffer.getLong();
			break;
		case 0x09:
			value = readLeb128(buffer);
			break;
		case 0x0A:
			value = buffer.getShort();
			break;
		case 0x0B:
			value = buffer.getInt();
			break;
		case 0x0C:
			value = buffer.getLong();
			break;
		case 0xFF:
			return 0;
		default:
			throw new IllegalArgumentException("Unsupported base encoding: 0x" + Integer.toHexString(encoding & 0x0F));
		}

		switch (encoding & 0xF0) {
		case 0x10:
			value += posCurrentSection + base;
			break;
		case 0x20:
			throw new UnsupportedOperationException("DW_EH_PE_textrel not implemented");
		case 0x30:
			throw new UnsupportedOperationException("DW_EH_PE_datarel not implemented");
		case 0x40:
			throw new UnsupportedOperationException("DW_EH_PE_funcrel not implemented");
		case 0x50:
			int alignment = (int) (Math.pow(2, value) - 1);
			value = (value + alignment) & ~alignment;
			break;
		default:
			break;
		}

		return value;
	}

	@Override
	protected long getLength(ByteBuffer buffer) {
		long result = Integer.toUnsignedLong(buffer.getInt());
		if (result == 0xffffffff) {
			result = buffer.getLong();
			DWARF32 = false;
		}
		return result;
	}

	public TableRow getRow(long address) {
		for (FrameDescriptionEntry fde : fdes) {
			if (fde.contains(address)) {
				return fde.getRow(address);
			}
		}
		return null;
	}

	public int getReturnAddressRegister(long address) {
		for (FrameDescriptionEntry fde : fdes) {
			if (fde.contains(address)) {
				return (int) fde.getRoot().getRetAddressReg();
			}
		}
		return -1;
	}

	@Override
	public long getCFAoffset(long address) {
		return 0;
	}
}
