package su.softcom.cldt.debug.internal.core.dwarf.attr;

/**
 * Helper class for Attribute with DW_FORM_implicit_const
 */
public class AttributeWIC extends Attribute {

	long implicitConst;
	
	public AttributeWIC(int name, int form, long value) {
		super(name, form);
		this.implicitConst = value;
	}
	
	public long getImplicitConst () {
		return implicitConst;
	}
	
	@Override
	public Attribute copy() {
		AttributeWIC result =  new AttributeWIC(this.name, this.form, this.implicitConst);
		result.value = this.value;
		return result;
	}
}
