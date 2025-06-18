package su.softcom.cldt.parsers.tests.sln;

import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

/**
 * @author petr.babanov
 *
 */
public class SlnTestCase {
	
	record Project(UUID id, String name, String vcxprojPath, List<UUID> dependencies) {

		public Project(UUID id, String name, String path) {
			this(id, name, path, new ArrayList<UUID>());
		}
		public void addDependency(UUID id) {
			this.dependencies.add(id);
		}
	};
	
	final URI path;
	final List<Project> projects;
	
	/**
	 * @param pathURI sln file 
	 */
	public SlnTestCase(URI pathURI) {
		path = pathURI;
		projects = new ArrayList<Project>();
	}
	
	/**
	 * @param id
	 * @param name
	 * @param path
	 * @return created project
	 */
	public Project addProject(UUID id, String name, String path) {
		Project p = new Project(id, name, path);
		projects.add(p);
		return p;
	}
	
	/**
	 * @return list of project
	 */
	public List<Project> getProjects(){
		return projects;
	}
	
	/**
	 * @return URI to sln file.
	 */
	public URI getPath() {
		return path;
	}
}
