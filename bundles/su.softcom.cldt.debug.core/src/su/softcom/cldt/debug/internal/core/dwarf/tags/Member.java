package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;

public class Member extends Tag implements IType, IResolvableTag {

	String name;
	Tag type;
	long memberLocation;
	int bitOffset;
	int bitSize;
	int fileIndex = -1;
	int line = -1;

	public Member(Tag prototype) {
		super(prototype);
	}

	public Member(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_member, data);
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
				} else if (a.name == DwarfConstants.DW_AT_name) {
					name = a.getValue().getObject().toString();
				} else if (a.name == DwarfConstants.DW_AT_data_member_location) {
					memberLocation = a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_bit_offset) {
					bitOffset = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_bit_size) {
					bitSize = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_decl_line) {
					line = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_decl_file) {
					fileIndex = (int) a.getValue().getUnsignedLongValue();
				} else {
					unresolvedAttributes.add(a);
				}
			}
		} catch (NullPointerException e) {
			// Not parsed yet
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	Tag getCopy() {
		return new Member(this);
	}

	@Override
	public long getSize() {
		return ((IType) type).getSize();
	}

	@Override
	public Object decodeValue(byte[] data) {
		return ((IType) type).decodeValue(data);
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		return ((IType) type).getStringRepresentation(data);
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
		return ((IType) type).getTypeChildren();
	}

	@Override
	public String getName() {
		if (name == null) {
			return type.getName();
		}
		return name;
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		if (type instanceof PointerType) {
			return data;
		} else if (type instanceof ArrayType) {
			return data;
		}
		return ((IType) type).getChildData(name, data);
	}

	public IType getType() {
		return (IType) type;
	}

	public long getMemberLocation() {
		return memberLocation;
	}

	public int getBitSize() {
		return bitSize;
	}

	public int getBitOffset() {
		return bitOffset;
	}

	@Override
	public boolean supportNumericalRepresentations() {
		return ((IType) type).supportNumericalRepresentations();
	}

	@Override
	public String getOctRepresentation(byte[] data) {
		return ((IType) type).getOctRepresentation(data);
	}

	@Override
	public String getHexRepresentation(byte[] data) {
		return ((IType) type).getHexRepresentation(data);
	}

	@Override
	public String getBinRepresentation(byte[] data) {
		return ((IType) type).getBinRepresentation(data);
	}
}
