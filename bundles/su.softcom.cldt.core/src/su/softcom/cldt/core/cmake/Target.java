package su.softcom.cldt.core.cmake;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.Path;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.source.ISourceFile;
import su.softcom.cldt.core.source.SourceFile;

/**
 * Описание цели сборки CMake
 */
public class Target {

	static final String PROPERTY_TYPE = "build-type"; //$NON-NLS-1$

	static final String ARTIFACTS_CONTAINER = "artifacts"; //$NON-NLS-1$

	static final String COMPILE_OPTIONS = "compile_options"; //$NON-NLS-1$

	static final String LINK_DIRS = "link_dirs"; //$NON-NLS-1$

	static final String LINK_OPTIONS = "link_options"; //$NON-NLS-1$

	static final String TARGET_LANGUAGE_STANDARD = "language_standard"; //$NON-NLS-1$

	static final String DEPENDENCIES = "dependencies"; //$NON-NLS-1$

	static final String SOURCE_LIST = "source_list"; //$NON-NLS-1$

	/**
	 * Типы целей сборки CMake.
	 */
	public enum Type {
		/**
		 * Исполняемый файл (EXECUTABLE).
		 * <p>
		 * Соответствует команде CMake: {@code add_executable(<name> [sources...])}
		 * </p>
		 */
		EXECUTABLE,

		/**
		 * Статическая библиотека (STATIC_LIBRARY).
		 * <p>
		 * Соответствует команде CMake: {@code add_library(<name> STATIC [sources...])}
		 * </p>
		 */
		STATIC_LIBRARY,

		/**
		 * Объектная библиотека (OBJECT_LIBRARY).
		 * <p>
		 * Соответствует команде CMake: {@code add_library(<name> OBJECT [sources...])}
		 * </p>
		 */
		OBJECT_LIBRARY,

		/**
		 * Интерфейсная библиотека (INTERFACE_LIBRARY).
		 * <p>
		 * Не компилируется, содержит только интерфейсные зависимости. Соответствует
		 * команде CMake: {@code add_library(<name> INTERFACE)}
		 * </p>
		 */
		INTERFACE_LIBRARY,

		/**
		 * Модульная библиотека (MODULE_LIBRARY).
		 * <p>
		 * Используется для динамически загружаемых модулей. Соответствует команде
		 * CMake: {@code add_library(<name> MODULE [sources...])}
		 * </p>
		 */
		MODULE_LIBRARY,

		/**
		 * Динамическая библиотека (SHARED_LIBRARY).
		 * <p>
		 * Соответствует команде CMake: {@code add_library(<name> SHARED [sources...])}
		 * </p>
		 */
		SHARED_LIBRARY,

		/**
		 * Кастомная цель (UTILITY).
		 * <p>
		 * Используется для нестандартных задач сборки (например, пользовательские
		 * команды). Соответствует команде CMake: {@code add_custom_target()}
		 * </p>
		 */
		UTILITY;

		/**
		 * @param name имя типа
		 * @return Тип по имени
		 */
		public static Type getType(String name) {
			switch (name) {
			case "EXECUTABLE": { //$NON-NLS-1$
				return Type.EXECUTABLE;
			}
			case "INTERFACE_LIBRARY": { //$NON-NLS-1$
				return Type.INTERFACE_LIBRARY;
			}
			case "MODULE_LIBRARY": { //$NON-NLS-1$
				return Type.MODULE_LIBRARY;
			}
			case "OBJECT_LIBRARY": { //$NON-NLS-1$
				return Type.OBJECT_LIBRARY;
			}
			case "SHARED_LIBRARY": { //$NON-NLS-1$
				return Type.SHARED_LIBRARY;
			}
			case "STATIC_LIBRARY": { //$NON-NLS-1$
				return Type.STATIC_LIBRARY;
			}
			default:
				return Type.UTILITY;
			}
		}

	}

	private String name = null;
	private final ICMakeProject project;
	private Preferences preferences;

	private Type type;
	private Map<String, String> attributes;
	private List<ISourceFile> sources = new ArrayList<>();
	private List<IFile> artifacts;
	private List<String> compileOptions;
	private List<String> linkOptions;
	private List<String> deps;
	private List<IFolder> includes;

	/**
	 * Constructs new Target instance using an existing target from preferences.
	 * 
	 * @param name
	 * @param project
	 * @param preferences
	 */
	public Target(String name, ICMakeProject project, Preferences preferences) {
		this.name = name;
		this.project = project;
		this.preferences = preferences;
		type = Type.getType(preferences.get(PROPERTY_TYPE, "")); //$NON-NLS-1$
		compileOptions = new ArrayList<>(Arrays.asList(preferences.get(COMPILE_OPTIONS, "").split(";"))); //$NON-NLS-1$ //$NON-NLS-2$
		linkOptions = new ArrayList<>(Arrays.asList(preferences.get(LINK_OPTIONS, "").split(";"))); //$NON-NLS-1$ //$NON-NLS-2$
		artifacts = new ArrayList<>(Arrays.stream(preferences.get(ARTIFACTS_CONTAINER, "").split(";")) //$NON-NLS-1$ //$NON-NLS-2$
				.filter(e -> !e.isBlank()).map(e -> project.getProject().getFile(Path.fromPortableString(e))).toList());
		attributes = new HashMap<>();
		attributes.put(TARGET_LANGUAGE_STANDARD, preferences.get(TARGET_LANGUAGE_STANDARD, "")); //$NON-NLS-1$
		includes = new ArrayList<>(Arrays.stream(preferences.get(LINK_DIRS, "").split(";")).filter(e -> !e.isEmpty()) //$NON-NLS-1$ //$NON-NLS-2$
				.map(e -> ResourcesPlugin.getWorkspace().getRoot().getFolder(new Path(e))).toList());
		deps = new ArrayList<>(Arrays.asList(preferences.get(DEPENDENCIES, "").split(";"))); //$NON-NLS-1$ //$NON-NLS-2$
		Stream.of(preferences.get(SOURCE_LIST, "").split(";")) //$NON-NLS-1$ //$NON-NLS-2$
				.filter(str -> !str.isEmpty()).forEach(srcName -> addSource(new SourceFile(srcName, this)));
	}

	/**
	 * Constructor for creating an empty new target from Deserializer
	 * 
	 * @param name
	 * 
	 * @param project
	 */
	public Target(String name, ICMakeProject project) {
		this.name = name;
		this.project = project;
		preferences = new ProjectScope(project.getProject()).getNode(CMakeCorePlugin.PLUGIN_ID)
				.node(ICMakeProject.TARGET_CONTAINER).node(name);
		compileOptions = new ArrayList<>();
		linkOptions = new ArrayList<>();
		artifacts = new ArrayList<>();
		sources = new ArrayList<>();
		attributes = new HashMap<>();
		attributes.put(TARGET_LANGUAGE_STANDARD, "C"); // FIXME
	}

	/**
	 * Constructor for creating a pure new target from wizard or property page.
	 * 
	 * @param name
	 * @param project
	 * @param type
	 * @param languageStandard
	 */
	public Target(String name, ICMakeProject project, Type type, String languageStandard) {
		this.name = name;
		this.project = project;
		this.type = type;
		preferences = new ProjectScope(project.getProject()).getNode(CMakeCorePlugin.PLUGIN_ID)
				.node(ICMakeProject.TARGET_CONTAINER).node(name);
		compileOptions = new ArrayList<>();
		linkOptions = new ArrayList<>();
		artifacts = new ArrayList<>();
		sources = new ArrayList<>();
		attributes = new HashMap<>();
		attributes.put(TARGET_LANGUAGE_STANDARD, languageStandard);// FIXME
	}

	/**
	 * @return string name of this Target
	 */
	public String getName() {
		return name;
	}

	/**
	 * @param files
	 */
	public void setArtifacts(List<IFile> files) {
		Assert.isNotNull(files);
		artifacts = files;
	}

	/**
	 * Get artifacts for target.
	 * 
	 * @return unmodifiable list of targets. Never {@code null}
	 */
	public List<IFile> getArtifacts() {
		return Collections.unmodifiableList(artifacts);
	}

	/**
	 * @return CMakeProject of this Target
	 */
	public ICMakeProject getProject() {
		return project;
	}

	/**
	 * @param type {@link Type} of this Target
	 */
	public void setType(Type type) {
		Assert.isNotNull(type);
		this.type = type;
	}

	/**
	 * @return {@link Type} of this Target
	 */
	public Type getType() {
		return type;
	}

	/**
	 * @param sources
	 */
	public void setSources(List<ISourceFile> sources) {
		Assert.isNotNull(sources);
		this.sources = sources;
	}

	/**
	 * @return sources
	 */
	public List<ISourceFile> getSources() {
		return Collections.unmodifiableList(sources);
	}

	/**
	 * @return string of language standard attribute
	 */
	public String getLanguageStandard() {
		return attributes.get(TARGET_LANGUAGE_STANDARD);
	}

	/**
	 * This method is called from CMakeProject.save(), so no flush in needed.
	 * 
	 * @throws BackingStoreException
	 * 
	 */
	public void save() throws BackingStoreException {
		preferences.put(PROPERTY_TYPE, type.name());
		preferences.put(COMPILE_OPTIONS, compileOptions.stream().collect(Collectors.joining(";"))); //$NON-NLS-1$
		preferences.put(LINK_OPTIONS, linkOptions.stream().collect(Collectors.joining(";"))); //$NON-NLS-1$
		preferences.put(ARTIFACTS_CONTAINER,
				artifacts.stream()
						.map(e -> e.getFullPath().makeRelativeTo(project.getProject().getFullPath()).toPortableString())
						.collect(Collectors.joining(";"))); //$NON-NLS-1$
		preferences.put(TARGET_LANGUAGE_STANDARD, attributes.get(TARGET_LANGUAGE_STANDARD));
		preferences.put(SOURCE_LIST, sources.stream().map(ISourceFile::getName).collect(Collectors.joining(";"))); //$NON-NLS-1$
		for (ISourceFile isrc : sources) {
			SourceFile src = (SourceFile) isrc;
			src.save();
		}
		preferences.flush();
	}

	/**
	 * Установить имя цели сборки
	 * 
	 * @param name
	 */
	public void setName(String name) {
		Assert.isNotNull(name);
		this.name = name;
	}

	/**
	 * @return compileOptions
	 */
	public List<String> getCompileOptions() {
		return Collections.unmodifiableList(compileOptions);
	}

	/**
	 * @return linkOptions
	 */
	public List<String> linkOptions() {
		return Collections.unmodifiableList(linkOptions);
	}

	/**
	 * @return dependencies
	 */
	public List<String> dependencies() {
		return Collections.unmodifiableList(deps);
	}

	/**
	 * @return includes
	 */
	public List<IFolder> includes() {
		return Collections.unmodifiableList(includes);
	}

	/**
	 * 
	 * @return статус Target
	 */
	public boolean isDirty() {
		return false;
	}

	private void addSource(SourceFile src) {
		Assert.isNotNull(src, "CppSource file must not be null");
		sources.add(src);
	}

	@Override
	public String toString() {
		return "Target: %s".formatted(this.getName()); //$NON-NLS-1$
	}

	/**
	 * @param source
	 * @return true if any elements were removed
	 */
	public boolean removeSourceFile(ISourceFile source) {
		return sources.remove(source);
	}

}
