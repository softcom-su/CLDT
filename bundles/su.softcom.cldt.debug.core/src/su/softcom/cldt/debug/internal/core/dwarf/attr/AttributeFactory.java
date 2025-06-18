package su.softcom.cldt.debug.internal.core.dwarf.attr;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

import su.softcom.cldt.debug.core.dwarf.DwarfInfo;
import su.softcom.cldt.debug.internal.core.dwarf.CompileUnitImpl;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.values.Value;

public class AttributeFactory {

	private static Map<Value, Attribute> cache = new HashMap<>();

    public static Attribute createAttribute(Attribute a, ByteBuffer data, DwarfInfo dwarfInfo, CompileUnitImpl cu) {

        int form = a.getForm();
		Value v = null;
		if(form == DwarfConstants.DW_FORM_indirect) {
			int f = (int)DwarfInfo.readLeb128(data, false);
			v = new Value(dwarfInfo, cu, f, data);
		} else if (form == DwarfConstants.DW_FORM_implicit_const && a instanceof AttributeWIC attrWIC) {
			v = new Value(form, Long.valueOf(attrWIC.getImplicitConst()));
		} else {
			v = Value.getValue(form, data, cu, dwarfInfo);
		}
		
		if(a.getName() == DwarfConstants.DW_AT_decl_line && form == DwarfConstants.DW_FORM_data1) {
			v.setObject(Byte.toUnsignedInt((Byte) v.getValue()));
		}
        
		if (cache.containsKey(v)) {
			Attribute cachedAttr = cache.get(v);
			if (cachedAttr.getForm() == form && cachedAttr.getName() == a.getName()) {
				return cachedAttr;
			}
		}
		Attribute newAttr = a.copy().setValue(v);
		cache.put(v, newAttr);
		return newAttr;
    }
    
    public static void clear() {
    	cache = new HashMap<>();
    }
}
