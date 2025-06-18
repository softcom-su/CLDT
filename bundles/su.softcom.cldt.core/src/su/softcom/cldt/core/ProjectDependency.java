package su.softcom.cldt.core;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * This class implements the project dependency. All dependencies are the
 * external CMake projects.
 */
public class ProjectDependency {

	/**
	 * Preference constant for storing dependency name
	 */
	public static final String NAME = "name"; //$NON-NLS-1$
	/**
	 * Preference constant for storing dependency path
	 */
	public static final String PATH = "path"; //$NON-NLS-1$
	/**
	 * Preference constant for storing dependency modules list
	 */
	public static final String MODULES_LIST = "modules-list"; //$NON-NLS-1$
	/**
	 * Preference constant for storing dependency "is required" option
	 */
	public static final String REQUIRED = "required"; //$NON-NLS-1$
	/**
	 * Preference constant for storing dependency version
	 */
	public static final String VERSION = "version"; //$NON-NLS-1$

	protected String name;
	protected IPath path;
	protected boolean isRequired;
	protected Optional<String> version;
	protected List<String> modules;

	protected final ICMakeProject project;
	protected final Preferences preferences;

	/**
	 * Constructs a dependency with the provided parameters
	 * 
	 * @param project
	 * @param name
	 * @param path
	 * @param isRequired
	 * @param version
	 * @param modules
	 */
	public ProjectDependency(ICMakeProject project, String name, IPath path, boolean isRequired,
			Optional<String> version, List<String> modules) {
		this.project = project;
		preferences = new ProjectScope(this.project.getProject()).getNode(CMakeCorePlugin.PLUGIN_ID)
				.node(ICMakeProject.DEPENDENCIES_CONTAINER).node(name);
		setName(name);
		setPath(path);
		setRequired(isRequired);
		setVersion(version);
		setModules(modules);
	}

	/**
	 * Constructs new ProjectDependency object using an existing dependency in
	 * preferences.
	 * 
	 * @param name
	 * @param project
	 */
	public ProjectDependency(String name, ICMakeProject project) {
		this.name = name;
		this.project = project;
		preferences = new ProjectScope(project.getProject()).getNode(CMakeCorePlugin.PLUGIN_ID)
				.node(ICMakeProject.DEPENDENCIES_CONTAINER).node(name);
		path = new Path(preferences.get(PATH, "")); //$NON-NLS-1$
		version = Optional.ofNullable(preferences.get(VERSION, null));
		isRequired = preferences.getBoolean(REQUIRED, false);
		modules = Arrays.asList(preferences.get(MODULES_LIST, "").split(PreferenceConstants.FIELD_SEPARATOR)); //$NON-NLS-1$
	}

	/**
	 * Copy constructor
	 * 
	 * @param copy
	 */
	public ProjectDependency(ProjectDependency copy) {
		this(copy.project, copy.name, copy.path, copy.isRequired, copy.version, new ArrayList<>(copy.modules));
	}

	/**
	 * @return dependency name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @return dependency path
	 */
	public IPath getPath() {
		return path;
	}

	/**
	 * @return the isRequired
	 */
	public boolean isRequired() {
		return isRequired;
	}

	/**
	 * @return the version
	 */
	public Optional<String> getVersion() {
		return version;
	}

	/**
	 * @return the components
	 */
	public List<String> getModules() {
		return modules;
	}

	/**
	 * @param isRequired the isRequired to set
	 */
	public void setRequired(boolean isRequired) {
		this.isRequired = isRequired;
	}

	/**
	 * @param version the version to set
	 */
	public void setVersion(Optional<String> version) {
		this.version = version;
	}

	/**
	 * @param modules the modules to set
	 */
	public void setModules(List<String> modules) {
		this.modules = modules;
	}

	/**
	 * Sets new dependency name
	 * 
	 * @param name
	 */
	public void setName(String name) {
		Assert.isNotNull(name);
		this.name = name;
	}

	/**
	 * Sets new dependency path
	 * 
	 * @param path
	 */
	public void setPath(IPath path) {
		Assert.isNotNull(path);
		this.path = path;
	}

	/**
	 * Saves dependency to Eclipse preferences.
	 * 
	 * NB: This method is designed to be called from CMakeProject.save(), so no
	 * flush in needed.
	 */
	public void save() {
		preferences.put(NAME, name);
		preferences.put(PATH, path.toString());
		if (version.isPresent()) {
			preferences.put(VERSION, version.get());
		}
		preferences.putBoolean(REQUIRED, isRequired);
		preferences.put(MODULES_LIST, modules.stream().filter(module -> !module.isBlank())
				.collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));
	}

	@Override
	public String toString() {
		return "Dependency %s of %s".formatted(name, project.getProject().getName()); //$NON-NLS-1$
	}

}
