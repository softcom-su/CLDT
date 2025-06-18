package su.softcom.cldt.debug.internal.core.types;

import java.util.HashMap;
import java.util.Map;

import su.softcom.cldt.debug.core.info.IType;

public class TypeFactory {
	
	private static Map<Long, IType> cache = new HashMap<>();

    public static IType createType() {
    	return null;
    	
    }
    
    public static void clear() {
    	cache = new HashMap<>();
    }
}
