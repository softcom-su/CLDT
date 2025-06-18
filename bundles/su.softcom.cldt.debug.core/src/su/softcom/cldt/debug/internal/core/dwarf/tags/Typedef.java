package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IDeclaration;
import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.ICompositeType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;

public class Typedef extends Tag implements ICompositeType, IResolvableTag, IDeclaration {

	private boolean isDeclaration = false;
	Tag type;
	String name;
	int fileIndex = -1;
	int line = -1;

	public Typedef(Tag prototype) {
		super(prototype);
	}

	public Typedef(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_typedef, data);
	}

	Tag getCopy() {
		return new Typedef(this);
	}

	@Override
	public long getId() {
		if (name != null && fileIndex > -1 && line > -1 && cu instanceof CompileUnitImpl compileUnit) {
			StringBuilder sb = new StringBuilder();
			sb.append(compileUnit.getFilePath(fileIndex));
			sb.append(line);
			return sb.toString().hashCode();
		} else if (name != null) {
			return name.hashCode();
		}
		return 0L;
	}

	@Override
	public IType getType() {
		if (type instanceof IType iType) {
			return iType;
		}
		return null;
	}

	@Override
	public String getName() {
		return name;
	}

	@Override
	public void resolve() {
		Attribute[] attrs = resolveArgs();
		super.resolve(attrs);
	}

	private Attribute[] resolveArgs() {
		List<Attribute> unresolvedAttributes = new ArrayList<>();
		for (Attribute a : args) {
			if (a.name == DwarfConstants.DW_AT_type) {
				try {
					long offset = a.getValue().getUnsignedLongValue();
					if (cu instanceof CompileUnitImpl cuImpl) {
						type = cuImpl.getTagByOffset((int) offset);
					}
				} catch (NullPointerException e) {
					// Not parsed yet
				}
			} else if (a.name == DwarfConstants.DW_AT_name) {
				name = a.getValue().getObject().toString();
			} else if (a.name == DwarfConstants.DW_AT_decl_line) {
				line = a.getValue().getUnsignedIntValue();
			} else if (a.name == DwarfConstants.DW_AT_decl_file) {
				fileIndex = (int) a.getValue().getUnsignedLongValue();
			} else if (a.name == DwarfConstants.DW_AT_declaration) {
				isDeclaration = a.getValue().getUnsignedByteValue() == 1;
			} else {
				unresolvedAttributes.add(a);
			}
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	@Override
	public long getSize() {
		return ((IType) type).getSize();
	}

	@Override
	public Object decodeValue(byte[] data) {
		return null;
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		return name;
	}

	@Override
	public ModelVariableType getModelVariableType() {
		return ((IType) type).getModelVariableType();
	}

	@Override
	public boolean hasChild() {
		return ((IType) type).hasChild();
	}

	@Override
	public List<Tag> getTypeChildren() {
		return type != null ? ((IType) type).getTypeChildren() : new ArrayList<>();
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		return ((IType) type).getChildData(name, data);
	}

	@Override
	public boolean supportNumericalRepresentations() {
		return false;
	}

	@Override
	public String getOctRepresentation(byte[] data) {
		throw new UnsupportedOperationException();
	}

	@Override
	public String getHexRepresentation(byte[] data) {
		throw new UnsupportedOperationException();
	}

	@Override
	public String getBinRepresentation(byte[] data) {
		throw new UnsupportedOperationException();
	}

	@Override
	public boolean isDeclaration() {
		return isDeclaration;
	}
}
