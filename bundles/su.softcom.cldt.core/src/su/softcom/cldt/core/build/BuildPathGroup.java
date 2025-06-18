package su.softcom.cldt.core.build;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.runtime.IPath;

/**
 * group for build path
 */
public abstract class BuildPathGroup implements IBuildPathGroup {

	final String name;
	final List<IPath> resources = new ArrayList<>();

	/**
	 * @param name name of group
	 */
	public BuildPathGroup(String name) {
		this.name = name;
	}

	@Override
	final public List<IPath> getResources() {
		return resources;
	}

	@Override
	final public String getName() {
		return name;
	}

	@Override
	public void deleteResource(IPath resource) {
		if (resources.contains(resource)) {
			resources.remove(resource);
			return;
		}
		throw new IllegalStateException(
				"resource %s not contains at build path group %s".formatted(resource.toString(), this.toString()));
	}

	@Override
	final public void addResource(IPath resource) {
		if (resource != null) {
			resources.add(resource);
			return;
		}
		throw new IllegalArgumentException("Expected not null argument");
	}

	@Override
	final public void deleteAllResources() {
		resources.clear();
	}

}
