package su.softcom.cldt.core.cmake;

/**
 * CMake semantic tree. Common interface for root tree and any subtrees.
 */
public interface ICMakeTree {

	/**
	 * Add node into tree.
	 * 
	 * @param command non-null {@link CMakeNode}
	 */
	void addChild(CMakeNode command);

}
