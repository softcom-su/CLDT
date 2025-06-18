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

public class ClassType extends Tag implements IType, IResolvableTag, IDeclaration {

	private boolean isDeclaration = false;
	private long byteSize = -1;
	private int bitSize;
	private String name;
	private int fileIndex = -1;
	private int line = -1;
	private ClassType specification;

	public ClassType(Tag prototype) {
		super(prototype);
	}

	public ClassType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_class_type, data);
	}

	@Override
	Tag getCopy() {
		return new ClassType(this);
	}

	@Override
	public long getId() {
		if (name != null && fileIndex > -1 && line > -1 && cu instanceof CompileUnitImpl compileUnit) {
			StringBuilder sb = new StringBuilder();
			sb.append(compileUnit.getFilePath(fileIndex));
			sb.append(line);
			return sb.toString().hashCode();
		} else if (name != null) {
			// Рекурсивно по родителям воостановить namespace: std::smth::Init
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
				if (a.name == DwarfConstants.DW_AT_byte_size) {
					byteSize = a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_bit_size) {
					bitSize = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_name) {
					name = a.getValue().getObject().toString();
				} else if (a.name == DwarfConstants.DW_AT_decl_line) {
					line = a.getValue().getUnsignedIntValue();
				} else if (a.name == DwarfConstants.DW_AT_decl_file) {
					fileIndex = (int) a.getValue().getUnsignedLongValue();
				} else if (a.name == DwarfConstants.DW_AT_specification) {
					specification = this.getClass().cast(cu.getTagByOffset((int) a.getValue().getUnsignedLongValue()));
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
		// TODO: Может и не null
		return new ModelVariableType(MVType.CLASS, getName(), null);
	}

	@Override
	public boolean hasChild() {
		int i;
		return hasMember(this, childrenOffsets);
	}

	private boolean hasMember(Tag tag, int[] childrenOffsets) {
		if (tag instanceof ClassType || tag instanceof StructureType) {
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

	public long getByteSize() {
		if (specification != null && specification.getByteSize() != -1) {
			return specification.getByteSize();
		}
		return byteSize;
	}

	public int getBitSize() {
		return bitSize;
	}

	@Override
	public String getName() {
		StringBuilder sb = new StringBuilder();
		Tag parent = this.getParent();
		while (parent != null) {
			if (parent instanceof Namespace) {
				sb.append("%s::".formatted(parent.getName()));
			}
			parent = parent.getParent();
		}

		if (specification != null && specification.getName() != null) {
			return sb.append(specification.getName()).toString();
		}
		return sb.append(name).toString();
	}

	public int getFileIndex() {
		if (specification != null && specification.getFileIndex() != -1) {
			return specification.getFileIndex();
		}
		return fileIndex;
	}

	public int getLine() {
		if (specification != null && specification.getLine() != -1) {
			return specification.getLine();
		}
		return line;
	}

	@Override
	public boolean isDeclaration() {
		return isDeclaration;
	}

}
