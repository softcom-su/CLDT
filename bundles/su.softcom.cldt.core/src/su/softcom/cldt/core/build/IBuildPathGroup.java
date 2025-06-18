package su.softcom.cldt.core.build;

import java.util.List;

import org.eclipse.core.runtime.IPath;

/**
 * Interface for build path group
 */
public interface IBuildPathGroup {

	/**
	 * @return list of added resources
	 */
	public List<IPath> getResources();

	/**
	 * @return name
	 */
	public String getName();

	/**
	 * Deletes resource if such was added
	 * 
	 * @param resource resource to delete
	 */
	public void deleteResource(IPath resource);

	public void deleteAllResources();

	/**
	 * @param resource resource to add
	 */
	public void addResource(IPath resource);

}
