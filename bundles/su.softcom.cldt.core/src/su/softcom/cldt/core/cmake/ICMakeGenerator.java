package su.softcom.cldt.core.cmake;

import java.util.List;

public interface ICMakeGenerator {

	public String getName();

	public boolean isPlatformSupport();

	public List<ICMakePlatform> getPlatforms();

}
