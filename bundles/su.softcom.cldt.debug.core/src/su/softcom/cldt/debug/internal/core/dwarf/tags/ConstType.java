package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.ICompositeType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;

/**
 * Implementation for DWARF type DW_TAG_const_type
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public class ConstType extends Tag implements ICompositeType, IResolvableTag {

	String name;
	Tag type;

	public ConstType(Tag prototype) {
		super(prototype);
	}

	public ConstType(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_array_type, data);
	}

	Tag getCopy() {
		return new ConstType(this);
	}

	@Override
	public long getId() {
		if (type != null) {
			return ((IType) type).getId();
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
					type = cu.getTagByOffset((int) offset);
				} else if (a.name == DwarfConstants.DW_AT_name) {
					name = a.getValue().getObject().toString();
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
	public String toString() {
		return "DW_TAG_const_type"; //$NON-NLS-1$
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
		return type != null ? ((IType) type).getTypeChildren() : new ArrayList<>();
	}

	@Override
	public byte[] getChildData(String name, byte[] data) {
		return ((IType) type).getChildData(name, data);
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

	@Override
	public String getName() {
		return ((IType) type).getName();
	}
	
	@Override
	public IType getType() {
		if (type instanceof IType iType) {
			return iType;
		}
		return null;
	}

}
