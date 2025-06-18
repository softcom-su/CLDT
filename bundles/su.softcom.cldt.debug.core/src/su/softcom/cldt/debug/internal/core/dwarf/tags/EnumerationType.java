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
import su.softcom.cldt.debug.internal.core.model.ModelVariableType.MVType;

public class EnumerationType extends Tag implements ICompositeType, IResolvableTag, IDeclaration {

	private boolean isDeclaration = false;
	Tag type;
	long fileIndex = -1;
	int line = 0;
	long byteSize;
	String name;
	boolean isEnumClass = false;

	public EnumerationType(Tag prototype) {
		super(prototype);
	}

	public EnumerationType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_enumeration_type, data);
	}

	@Override
	Tag getCopy() {
		return new EnumerationType(this);
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
	public void resolve() {
		Attribute[] attrs = resolveArgs();
		super.resolve(attrs);
	}

	private Attribute[] resolveArgs() {
		List<Attribute> unresolvedAttributes = new ArrayList<>();
		try {
			for (Attribute a : args) {
				if (a.name == DwarfConstants.DW_AT_type) {
					long offset = a.getValue().getUnsignedLongValue();
					if (cu instanceof CompileUnitImpl cuImpl) {
						type = cuImpl.getTagByOffset((int) offset);
					}
				} else if (a.name == DwarfConstants.DW_AT_decl_file) {
					fileIndex = (int) a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_decl_line) {
					line = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_byte_size) {
					byteSize = a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_name) {
					name = (String) a.getValue().getObject();
				} else if (a.name == DwarfConstants.DW_AT_enum_class) {
					isEnumClass = (Byte) a.getValue().getObject() == 1;
				} else if (a.name == DwarfConstants.DW_AT_declaration) {
					isDeclaration = a.getValue().getUnsignedByteValue() == 1;
				} else {
					unresolvedAttributes.add(a);
				}
			}
		} catch (NullPointerException e) {
			// Not parsed yet
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	public IType getType() {
		if (type instanceof IType iType) {
			return iType;
		}
		return null;
	}

	@Override
	public long getSize() {
		return byteSize;
	}

	@Override
	public Object decodeValue(byte[] data) {
		return null;
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		return null;
	}

	@Override
	public ModelVariableType getModelVariableType() {
		ModelVariableType child = ((IType) type).getModelVariableType();
		return new ModelVariableType(MVType.ENUMERATION, name, child);
	}

	@Override
	public boolean hasChild() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public List<Tag> getTypeChildren() {
		// Исправить на поддержку Enumerator
		return new ArrayList<Tag>();
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean supportNumericalRepresentations() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public String getOctRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getHexRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getBinRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean isDeclaration() {
		return isDeclaration;
	}

}
