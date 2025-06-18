package su.softcom.cldt.core.build;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.runtime.IPath;

/**
 * Group for standard library build path
 */
public final class StandartLibBuildPathGroup extends BuildPathGroup {

	final List<String> resources = new ArrayList<String>();

	/**
	 * Default constructor
	 */
	public StandartLibBuildPathGroup() {
		super("Стандартные библиотеки");
	}

	final public List<String> getStringResources() {
		return resources;
	}

	/**
	 * @param resource resource resource name. Can't be "" or null
	 */
	@Override
	final public void deleteResource(IPath resource) {
		if (resources.contains(resource)) {
			resources.remove(resource);
			return;
		}
		throw new IllegalStateException(
				"resource %s not contains at build path group %s".formatted(resource, this.toString()));
	}

	/**
	 * @param resource resource name. Can't be "" or null
	 */
	final public void addResource(String resource) {
		if (resource != null && resource != "") {
			if (!resources.contains(resource)) {
				resources.add(resource);
			}
			return;
		}
		throw new IllegalArgumentException("Expected not null argument");
	}
}
