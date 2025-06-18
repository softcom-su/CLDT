package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;

import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;

public class InlinedSubroutine extends Tag {

	public InlinedSubroutine(Tag prototype) {
		super(prototype);
	}

	public InlinedSubroutine(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_inlined_subroutine, data);
	}

}
