
package su.softcom.cldt.core;

import java.nio.file.Path;

/**
 * Toolchain information.
 */
public interface IToolchain {

	/**
	 * Set toolchain name.
	 * 
	 * @param name
	 */
	public void setName(String name);

	/**
	 * Set toolchain path.
	 * 
	 * @param path
	 */
	public void setPath(Path path);

	/**
	 * Get toolchain name.
	 * 
	 * @return The name of toolchain.
	 */
	public String getName();

	/**
	 * Get toolchain path.
	 * 
	 * @return The path of toolchain executable file.
	 */
	public Path getPath();

}