package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IDebugVariable;
import su.softcom.cldt.debug.core.IFunction;
import su.softcom.cldt.debug.internal.core.GDBClient;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.dwarf.op.Operation;
import su.softcom.cldt.debug.internal.core.dwarf.values.Exprloc;

/**
 * {@link su.softcom.cldt.debug.internal.core.dwarf.tags.Tag Tag} implementation
 * for {@code DwarfConstants.DW_TAG_subprogram}
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 * @author Drumov Maxim (drumov.maxim@softcom.su)
 *
 */
public class Subprogram extends Tag implements IFunction {

	int[] variables = new int[0];
	int fileIndex = -1;
	String name;
	long sibling;
	boolean external = false;
	long startAddress = 0;
	long endAddress = 0;
	long prologAddress = 0;
	Exprloc frameBase;
	Subprogram specification;

	Subprogram(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_subprogram, data);
	}

	public Subprogram(Tag prototype) {
		super(prototype);
	}

	@Override
	public Tag getCopy() {
		return new Subprogram(this);
	}

	public void setVariables(int[] variables) {
		this.variables = variables;
	}

	@Override
	public boolean contents(long address) {
//		if (specification != null) {
//			return specification.contents(address);
//		}
		return ((address <= endAddress) && (address >= startAddress));
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("DW_TAG_subprogram: ").append(name);
		return sb.toString();
	}

	@Override
	public String getName() {
		if (specification != null && specification.getName() != null) {
			return specification.getName();
		}
		return name;
	}

	public int[] getVariables() {
		return variables;
	}

	@Override
	public void resolve(Attribute[] attrs) {
		List<Attribute> unresolvedAttributes = new ArrayList<>();
		for (Attribute a : attrs) {
			if (a.getValue() == null) {
				unresolvedAttributes.add(a);
				continue;
			}
			switch (a.name) {
			case DwarfConstants.DW_AT_name:
				name = (String) a.getValue().getObject();
				break;
			case DwarfConstants.DW_AT_low_pc:
				startAddress = a.getValue().getUnsignedLongValue();
				break;
			case DwarfConstants.DW_AT_high_pc:
				if (a.getForm() == DwarfConstants.DW_FORM_addrx) {
					endAddress = a.getValue().getUnsignedLongValue();
				} else {
					endAddress = startAddress + a.getValue().getUnsignedLongValue();
				}
				break;
			case DwarfConstants.DW_AT_sibling:
				sibling = (long) a.getValue().getObject();
				break;
			case DwarfConstants.DW_AT_external:
				external = (byte) a.getValue().getObject() != 0;
				break;
			case DwarfConstants.DW_AT_frame_base:
				if (a.getValue() instanceof Exprloc loc) {
					frameBase = loc;
				}
				break;
			case DwarfConstants.DW_AT_decl_file:
				fileIndex = (int) a.getValue().getUnsignedLongValue();
				break;
			case DwarfConstants.DW_AT_specification:
				specification = this.getClass().cast(cu.getTagByOffset((int) a.getValue().getUnsignedLongValue()));
				break;
			default:
				unresolvedAttributes.add(a);
				break;
			}
		}
		super.resolve(unresolvedAttributes.toArray(new Attribute[0]));

	}

	// TODO: Refactor!!!
	@Override
	public long resolveFrameBase(GDBClient client, Map<Integer, BigInteger> regs) {
		Object value = frameBase.getValue();
		if (value instanceof byte[] operations) {
			return Operation.evaluateExpression(client, regs, (IFunction) this, getCompileUnit(), operations);
		}
		return -1;
	}

	public long getSibling() {
		return sibling;
	}

	@Override
	public List<IDebugVariable> getArguments() {
		// TODO Auto-generated method stub
		return null;
	}

	public long getStartAddress() {
		if (specification != null && specification.getStartAddress() > 0) {
			return specification.getStartAddress();
		}
		return startAddress;
	}

	public long getEndAddress() {
		if (specification != null && specification.getEndAddress() > 0) {
			return specification.getEndAddress();
		}
		return endAddress;
	}

	public boolean isExternal() {
		return external;
	}

	public int getFileIndex() {
		if (fileIndex == -1 && specification != null && specification.getFileIndex() > 0) {
			return specification.getFileIndex();
		}
		return fileIndex;
	}

	/**
	 * Назначить адрес окончания пролога
	 * 
	 * @param prologAddress
	 */
	public void setPrologAddress(long prologAddress) {
		this.prologAddress = prologAddress;
	}

	@Override
	public ICompileUnit getCompileUnit() {
		return cu;
	}

	@Override
	public boolean isProlog(long address) {
		return prologAddress != 0 && prologAddress > address;
	}

}