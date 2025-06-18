package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfElement;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.dwarf.attr.AttributeWIC;

/**
 * Base class for all DW_TAG units. Subclasses describes one and only one DW_TAG
 * type.
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public class Tag extends DwarfElement {

	protected int[] childrenOffsets = new int[0];
	protected ICompileUnit cu;
	protected Attribute[] args;
	private int tag;
	private long offset;
	private boolean hasChild;
	private Tag parent;

	protected Tag() {
	}

	protected Tag(Tag prototype) {
		tag = prototype.getTag();
		setHasChild(prototype.isHasChild());
		args = null;
	}

	protected Tag(int code, ByteBuffer data) {
		tag = code;
		setHasChild(data.get() == 1);
		List<Attribute> tmp = new ArrayList<>();
		while (data.hasRemaining()) {
			int name = Short.toUnsignedInt((short) readUleb128(data));
			int form = Short.toUnsignedInt((short) readUleb128(data));
			Long value = null;
			if (form == DwarfConstants.DW_FORM_implicit_const) {
				value = readLeb128(data);
			}
			if ((name != 0) && (form != 0) && (value == null)) {
				tmp.add(Attribute.getAttribute(name, form, data.order()));
			} else if ((name != 0) && (form != 0)) {
				tmp.add(new AttributeWIC(name, form, value));
			} else {
				break;
			}
		}
		args = tmp.toArray(new Attribute[0]);
	}

	Tag getCopy() {
		return new Tag(this);
	}

	public void setOffset(int offset) {
		this.offset = offset;
	}

	protected void resolve(Attribute[] v) {
		args = v;
	}

	public String getName() {
		return "";
	}

	public Attribute[] getArgs() {
		return this.args;
	}

	public Attribute getArg(int name) {
		for (Attribute a : args) {
			if (a.name == name) {
				return a;
			}
		}
		return null;
	}

	public boolean hasChildren() {
		return childrenOffsets.length != 0;
	}

	public String toString() {
		String name;
		switch (getTag()) {
		case 0x05:
			return "DW_TAG_formal_parameter(0x05)";
		case 0x11:
			return "DW_TAG_compile_unit(0x11)";
		case 0x21:
			return "DW_TAG_subrange_type(0x21)";
		}
		return "";
	}

	public int getTag() {
		return tag;
	}

	public int[] getChildren() {
		return childrenOffsets;
	}

	static Set<Integer> set = new HashSet<>();

	public static final Tag getTag(ByteBuffer data) {
		int code = Short.toUnsignedInt((short) readUleb128(data));
		switch (code) {
		case DwarfConstants.DW_TAG_array_type:
			return new ArrayType(data);
		case DwarfConstants.DW_TAG_member:
			return new Member(data);
		case DwarfConstants.DW_TAG_structure_type:
			return new StructureType(data);
		case DwarfConstants.DW_TAG_typedef:
			return new Typedef(data);
		case DwarfConstants.DW_TAG_union_type:
			return new UnionType(data);
		case DwarfConstants.DW_TAG_pointer_type:
			return new PointerType(data);
		case DwarfConstants.DW_TAG_base_type:
			return new BaseType(data);
		case DwarfConstants.DW_TAG_const_type:
			return new ConstType(data);
		case DwarfConstants.DW_TAG_subprogram:
			return new Subprogram(data);
		case DwarfConstants.DW_TAG_variable:
			return new Variable(data);
		case DwarfConstants.DW_TAG_volatile_type:
			return new VolatileType(data);
		case DwarfConstants.DW_TAG_lexical_block:
			return new LexicalBlock(data);
		case DwarfConstants.DW_TAG_reference_type:
			return new ReferenceType(data);
		case DwarfConstants.DW_TAG_rvalue_reference_type:
			return new RValueReferenceType(data);
		case DwarfConstants.DW_TAG_class_type:
			return new ClassType(data);
		case DwarfConstants.DW_TAG_subrange_type:
			return new SubrangeType(data);
		case DwarfConstants.DW_TAG_enumeration_type:
			return new EnumerationType(data);
		case DwarfConstants.DW_TAG_formal_parameter:
			return new FormalParameter(data);
		case DwarfConstants.DW_TAG_enumerator:
			return new Enumerator(data);
		case DwarfConstants.DW_TAG_imported_declaration:
			return new ImportedDeclaration(data);
		case DwarfConstants.DW_TAG_template_type_parameter:
			return new TemplateTypeParameter(data);
		case DwarfConstants.DW_TAG_template_value_parameter:
			return new TemplateValueParameter(data);
		case DwarfConstants.DW_TAG_label:
			return new Label(data);
		case DwarfConstants.DW_TAG_compile_unit:
			return new CompileUnitTag(data);
		case DwarfConstants.DW_TAG_subroutine_type:
			return new SubroutineType(data);
		case DwarfConstants.DW_TAG_unspecified_parameters:
			return new UnspecifiedParameters(data);
		case DwarfConstants.DW_TAG_inheritance:
			return new Inheritance(data);
		case DwarfConstants.DW_TAG_ptr_to_member_type:
			return new PtrToMemberType(data);
		case DwarfConstants.DW_TAG_restrict_type:
			return new RestrictType(data);
		case DwarfConstants.DW_TAG_namespace:
			return new Namespace(data);
		case DwarfConstants.DW_TAG_imported_module:
			return new ImportedModule(data);
		case DwarfConstants.DW_TAG_unspecified_type:
			return new UnspecifiedType(data);
		case DwarfConstants.DW_TAG_inlined_subroutine:
			return new InlinedSubroutine(data);
		default:
			Platform.getLog(Tag.class).info("Tag with code " + code);
			set.add(code);
			return new Tag(code, data);
		}
	}

	public static final Tag resolveTag(Tag t, Attribute[] v, CompileUnitImpl compileUnit) {
		Tag result = t.getCopy();
		result.setCompileUnit(compileUnit);
		result.resolve(v);
		return result;
	}

//	public void addChild(Integer child) {
//		childrenOffsets.add(child);
//	}
	public void setChildrens(int[] childrenOffsets) {
		this.childrenOffsets = childrenOffsets;
	}

	public int[] getChildrens() {
		return childrenOffsets;
	}

	public Tag getParent() {
		return parent;
	}

	public void setParent(Tag parent) {
		this.parent = parent;
	}

	public boolean isHasChild() {
		return hasChild;
	}

	public void setHasChild(boolean hasChild) {
		this.hasChild = hasChild;
	}

	public void setCompileUnit(ICompileUnit cu) {
		this.cu = cu;
	}

	public ICompileUnit getCompileUnit() {
		return cu;
	}

}
