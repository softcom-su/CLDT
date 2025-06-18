package su.softcom.cldt.core.cmake;

import org.eclipse.core.runtime.Path;

public interface ICMakeInstance {

	Path getPath();

	String getName();

	CMakeInfo getInfo();

	/**
	 * Returns {@code true} if CMake instance supports {@code --toolchain} argument.
	 * 
	 * @return
	 */
	boolean supportsToolchainArgument();

}
