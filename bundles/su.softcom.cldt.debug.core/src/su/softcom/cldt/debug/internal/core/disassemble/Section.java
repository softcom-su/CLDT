package su.softcom.cldt.debug.internal.core.disassemble;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.NavigableMap;
import java.util.TreeMap;

import su.softcom.cldt.debug.core.IAddressable;

public class Section implements IAddressable {

	private Map<Long, Subsection> subsections;
	private String name;
	private Long startAddress;
	private Long endAddress;
	
	public class Subsection implements IAddressable {
		private NavigableMap<Long, Instruction> instructions;
		private Long startAddress;
		private Long endAddress;
		private String name;
		private Section parent;
		
		public class Instruction {
			int[] opcodes;
			String name;
			String instructionStr;

			public Instruction(int[] opcodes, String name, String instructionStr) {
				this.opcodes = opcodes;
				this.name = name;
				this.instructionStr = instructionStr;
			}

			public int[] getOpcodes() {
				return opcodes;
			}

			public String getName() {
				return name;
			}

			public String getInstruction() {
				return instructionStr;
			}

			@Override
			public String toString() {
				return "Instruction [opcodes=" + Arrays.toString(opcodes) + ", name=" + name + ", instruction="
						+ instructionStr + "]";
			}
		}
		
		public Subsection(Section parent, long startAddress, String name) {
			this.instructions = new TreeMap<>();
			this.startAddress = startAddress;
			this.endAddress = startAddress;
			this.name = name;
			this.parent = parent;
			
		}
		
		public NavigableMap<Long, Instruction> getInstructions() {
			return instructions;
		}
		
		public Entry<Long, Instruction> getNextEntry(Long currentKey) {
			return instructions.higherEntry(currentKey);
		}
		
		public Entry<Long, Instruction> getCurrentEntry(Long currentKey) {
			Entry<Long, Instruction> entry = instructions.floorEntry(currentKey);
			return entry.getKey().equals(currentKey) ? entry : null;
		}
		
		public void addInstruction(long address, Instruction instruction) {
			if(address > endAddress) {
				this.endAddress = address;
				parent.setEndAddress(address);
			}
			instructions.put(address, instruction);
		}

		@Override
		public String toString() {
			return "Subsection [instructions=" + instructions + ", startAddress=" + startAddress + ", endAddress="
					+ endAddress + "]";
		}

		@Override
		public Long getStartAddress() {
			return startAddress;
		}

		@Override
		public Long getEndAddress() {
			return endAddress;
		}

		public String getName() {
			return name;
		}

		public Section getParent() {
			return parent;
		}

		public void setParent(Section parent) {
			this.parent = parent;
		}
		
	}
	
	public Section(String name) {
		this.subsections = new HashMap<>();
		this.name = name;
	}
	
	public Map<Long, Subsection> getSubsections() {
		return this.subsections;
	}
	
	public Subsection getSubsection(long address) {
		return subsections.get(address);
	}
	
	public void addSubsection(long address, Subsection subSection) {
		subsections.put(address, subSection);
	}
	
	public Long getStartAddress() {
		return startAddress;
	}

	public void setStartAddress(Long startAddress) {
		this.startAddress = startAddress;
	}

	public Long getEndAddress() {
		return endAddress;
	}

	public void setEndAddress(Long endAddress) {
		this.endAddress = endAddress;
	}

	@Override
	public String toString() {
		return "Section [name=" + name + " ,subsections=" + subsections  + "]";
	}
	
	
}
