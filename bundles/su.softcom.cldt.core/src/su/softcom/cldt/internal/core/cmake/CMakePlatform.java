package su.softcom.cldt.internal.core.cmake;

import su.softcom.cldt.core.cmake.ICMakePlatform;

public class CMakePlatform implements ICMakePlatform {

	final String name;

	public CMakePlatform(String name) {
		this.name = name;
	}

	@Override
	public String getName() {
		return name;
	}

}
