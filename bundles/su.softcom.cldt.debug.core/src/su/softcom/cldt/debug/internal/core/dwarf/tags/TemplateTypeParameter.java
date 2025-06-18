package su.softcom.cldt.debug.internal.core.dwarf.tags;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.dwarf.IResolvableTag;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;

public class TemplateTypeParameter extends Tag implements IResolvableTag, IType {

	Tag type;
	String name;

	public TemplateTypeParameter(Tag prototype) {
		super(prototype);
	}

	public TemplateTypeParameter(ByteBuffer data) {
		super(DwarfConstants.DW_TAG_template_type_parameter, data);
	}

	@Override
	Tag getCopy() {
		return new TemplateTypeParameter(this);
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
					Object val = a.getValue().getValue();
					if (val != null) {
						name = val.toString();
					}
				} else {
					unresolvedAttributes.add(a);
				}

			}
		} catch (NullPointerException e) {
			// Some fields not set, skip it
		}
		return unresolvedAttributes.toArray(new Attribute[0]);
	}

	@Override
	public long getSize() {
		if (type instanceof IType iType) {
			return iType.getSize();
		}
		return 0;
	}

	@Override
	public Object decodeValue(byte[] data) {
		return null;
	}

	@Override
	public String getStringRepresentation(byte[] data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public ModelVariableType getModelVariableType() {
		return ((IType) type).getModelVariableType();
	}

	@Override
	public boolean hasChild() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public List<Tag> getTypeChildren() {
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

}
