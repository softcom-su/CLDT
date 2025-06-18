package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;

public class LexicalBlock extends Tag implements IResolvableTag {

	long startAddress = 0;
	long endAddress = 0;

	LexicalBlock(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_lexical_block, data);
	}

	public LexicalBlock(Tag prototype) {
		super(prototype);
	}

	@Override
	public Tag getCopy() {
		return new LexicalBlock(this);
	}

	public long getStartAddress() {
		return startAddress;
	}

	public long getEndAddress() {
		return endAddress;
	}

	@Override
	public void resolve() {
		Attribute[] attrs = resolveArgs();
		super.resolve(attrs);
	}

	private Attribute[] resolveArgs() {
		List<Attribute> unresolvedAttributes = new ArrayList<>();
		for (Attribute a : args) {
			if (a.getValue() == null) {
				unresolvedAttributes.add(a);
				continue;
			}
			switch (a.name) {
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
			default:
				unresolvedAttributes.add(a);
				break;
			}
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

}
