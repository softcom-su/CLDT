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

public class UnionType extends Tag implements IType, IResolvableTag, IDeclaration {

	private boolean isDeclaration = false;
	long byteSize;
	List<IType> members = new ArrayList<>();
	String name;
	int fileIndex = -1;
	int line = -1;

	public UnionType(Tag prototype) {
		super(prototype);
	}

	public UnionType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_structure_type, data);
	}

	Tag getCopy() {
		return new UnionType(this);
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
		for (Attribute a : args) {
			if (a.name == DwarfConstants.DW_AT_byte_size) {
				byteSize = a.getValue().getUnsignedLongValue();
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

//	@Override
//	public void addChild(Integer child) {
//		// TODO: Fix
////		if(child instanceof Member) {
////			members.add((IType)child);
////		}
//		super.addChild(child);
//	}

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
		StringBuilder sb = new StringBuilder("Union {");
		for (IType t : members) {
			sb.append(t.getName().concat(", "));
		}
		return sb.substring(0, sb.length() - 2).concat("}");
	}

	@Override
	public ModelVariableType getModelVariableType() {
		return new ModelVariableType(MVType.UNION, name, null);
	}

	@Override
	public boolean hasChild() {
		return hasMember(this, childrenOffsets);
	}

	private boolean hasMember(Tag tag, int[] childrenOffsets) {
		if (tag.getClass().isInstance(this)) {
			for (Integer offset : childrenOffsets) {
				Tag child = cu.getTagByOffset(offset);
				if (child instanceof Member || (child instanceof Inheritance inheritance
						&& hasMember((Tag) inheritance.getType(), ((Tag) inheritance.getType()).childrenOffsets))) {
					return true;
				}
			}
		} else if (tag instanceof ICompositeType type) {
			Tag typeTag = (Tag) type.getType();
			return hasMember(typeTag, typeTag.childrenOffsets);
		}
		return false;
	}

	@Override
	public List<Tag> getTypeChildren() {
		List<Tag> children = new ArrayList<>();
		for (Integer offset : childrenOffsets) {
			Tag tag = cu.getTagByOffset(offset);
			if (tag instanceof Member || tag instanceof Inheritance) {
				children.add(tag);
			}
		}
		return children;
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		return data;
	}

	@Override
	public String getName() {
		return name != null ? name : "Union";
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
