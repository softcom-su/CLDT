package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;

public class CompileUnitTag extends Tag implements IResolvableTag {

	long lowPC = -1;
	long highPC = -1;
	long stmtList = -1;
	long ranges = -1;
	long rangeBase = -1;

	public CompileUnitTag(Tag prototype) {
		super(prototype);
	}

	public CompileUnitTag(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_compile_unit, data);
	}

	@Override
	Tag getCopy() {
		return new CompileUnitTag(this);
	}

	@Override
	public void resolve() {
		Attribute[] attrs = resolveArgs();
		super.resolve(attrs);
	}

	private Attribute[] resolveArgs() {
		List<Attribute> unresolvedAttributes = new ArrayList<>();
		try {
			for (Attribute a : args) {
				if (a.name == DwarfConstants.DW_AT_low_pc) {
					Long tmp = null;
					Object o = a.getValue().getValue();
					if (o instanceof Long l) {
						tmp = l;
					} else if (o instanceof Integer i && !a.getValue().isTmp()) {
						tmp = Long.valueOf(i.longValue());
					}

					if (tmp == null) {
						long address = cu.getAddress((int) a.getValue().getValue());
						a.getValue().setObject(Long.valueOf(address));
					}
					lowPC = a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_high_pc) {
					if (a.getForm() == DwarfConstants.DW_FORM_addrx) {
						highPC = a.getValue().getUnsignedLongValue();
					} else {
						highPC = lowPC + a.getValue().getUnsignedLongValue();
					}
				} else if (a.name == DwarfConstants.DW_AT_ranges) {
					ranges = a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_stmt_list) {
					stmtList = a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_rnglists_base) {
					rangeBase = a.getValue().getUnsignedLongValue();
				} else {
					unresolvedAttributes.add(a);
				}
			}
		} catch (NullPointerException e) {
			// Not parsed yet
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	public long getLowPC() {
		return lowPC;
	}

	public long getHighPC() {
		return highPC;
	}

	public long getStmtList() {
		return stmtList;
	}

	public long getRanges() {
		return ranges;
	}

	public long getRangeBase() {
		return rangeBase;
	}

}
