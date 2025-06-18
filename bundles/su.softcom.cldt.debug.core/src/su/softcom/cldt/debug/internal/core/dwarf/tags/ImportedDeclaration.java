package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;

public class ImportedDeclaration extends Tag implements IResolvableTag {

	int fileIndex = -1;
	int line = -1;
	long offsetImport = 0;

	public ImportedDeclaration(Tag prototype) {
		super(prototype);
	}

	public ImportedDeclaration(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_imported_declaration, data);
	}

	@Override
	Tag getCopy() {
		return new ImportedDeclaration(this);
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
				if (a.name == DwarfConstants.DW_AT_decl_file) {
					fileIndex = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_decl_line) {
					line = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_import) {
					offsetImport = a.getValue().getUnsignedLongValue();
				} else {
					unresolvedAttributes.add(a);
				}

			}
		} catch (NullPointerException e) {
			// Some fields not set, skip it
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

}
