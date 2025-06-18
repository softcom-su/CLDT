package su.softcom.cldt.common.preferences;

/**
 * String constants for preference access. Use only for E4 API.
 * 
 * @author Petr Babanov - Initial implementation.
 * 
 * @see {@link org.eclipse.core.runtime.preferences.IEclipsePreferences}
 *
 */
public interface PreferenceConstants {

	/**
	 * Main node for CLDT core preferences. Used by
	 * {@link org.eclipse.core.runtime.preferences.InstanceScope InstanceScope},
	 * {@link org.eclipse.core.runtime.preferences.ConfigurationScope
	 * ConfigurationScope}, {@link org.eclipse.core.runtime.preferences.DefaultScope
	 * DefaultScope}, {@link org.eclipse.core.resources.ProjectScope ProjectScope}
	 */
	public static final String NODE = "su.softcom.cldt.core"; //$NON-NLS-1$

	public static final String BUILD_CONFIG_NODE = "build_configurations"; //$NON-NLS-1$

	/**
	 * @deprecated TEMPORARY API, DO NOT USE
	 */
	@Deprecated
	public static final String CIINITIALIZER_NODE = "su.softcom.cldt.core"; //$NON-NLS-1$

	/**
	 * @deprecated TEMPORARY API, DO NOT USE
	 */
	@Deprecated
	public static final String CIINITIALIZER_CLANG_PATH = "clangPath"; //$NON-NLS-1$

	/**
	 * @deprecated TEMPORARY API, DO NOT USE
	 */
	@Deprecated
	public static final String CORE_BUILD_FOLDER = "su.softcom.cldt.ui.project.preference.buildFolder"; //$NON-NLS-1$

	/**
	 * List of ':' separated source files.
	 */
	public static final String SRC_FILES = "source_files"; //$NON-NLS-1$

	/**
	 * List of ':' separated resource files.
	 */
	public static final String RES_FILES = "resource_files"; //$NON-NLS-1$

	/**
	 * List of ':' separated folders with headers files.
	 */
	public static final String LOCAL_INC_FILES = "local_includes_dirs"; //$NON-NLS-1$

	/**
	 * List of ':' separated folders with headers files outside of project.
	 */
	public static final String GLOBAL_INC_FILES = "global_includes_dirs"; //$NON-NLS-1$

	/**
	 * List of ':' separated external libraries.
	 */
	public static final String EXTERNAL_LIBRARIES = "link_libraries"; //$NON-NLS-1$

	/**
	 * List of ':' separated directories for searching external libraries.
	 */
	public static final String LIB_FOLDERS = "link_directories"; //$NON-NLS-1$

	/**
	 * List of ':' separated internal libraries import.cmake paths. FIXME: rename
	 */
	public static final String LIBRARIES = "libraries"; //$NON-NLS-1$

	/**
	 * List of ':' separated drivers import.cmake paths. FIXME: rename
	 */
	public static final String DRIVERS = "drivers"; //$NON-NLS-1$

	/**
	 * List of '\n' separated compiler arguments.
	 */
	public static final String COMPILER_ARGUMENTS = "compiler_arguments"; //$NON-NLS-1$

	/**
	 * List of '\n' separated linker arguments.
	 */
	public static final String LINKER_ARGUMENTS = "linker_arguments"; //$NON-NLS-1$

	/**
	 * Map of project's dependencies.
	 */
	public static final String DEPENDENCIES_ARGUMENTS = "depend_arguments"; //$NON-NLS-1$

	/**
	 * Name of generated executable file.
	 */
	public static final String EXECUTABLE_NAME = "executable_name"; //$NON-NLS-1$

	public static final String NATIVEBUILD_BUILD_SCRIPT = "nativebuild.build.script"; //$NON-NLS-1$
	public static final String NATIVEBUILD_BUILD_COMPILER = "nativebuild.build.compiler"; //$NON-NLS-1$
	public static final String NATIVEBUILD_DEPLOY_SCRIPT = "nativebuild.deploy.script"; //$NON-NLS-1$

	/**
	 * BuildConfihurations node
	 */
	public static final String BUILD_CONFIGURATIONS = "build.configurations"; //$NON-NLS-1$

	/**
	 * Represents the separator used with BuildConfiguration parameters
	 */
	public static final String FIELD_SEPARATOR = ";"; //$NON-NLS-1$
	/**
	 * Needed for target property -> OUTPUT_DIRECTORY_.* variants
	 */
	public static final String BUILD_FOLDER = "build-folder"; //$NON-NLS-1$

	/**
	 * Default value for {link {@link #BUILD_FOLDER} value
	 */
	public static final String DEFAULT_BUILD_FOLDER = "build"; //$NON-NLS-1$

	/**
	 * Deafult falue for cxx standart
	 */
	public static final String DEFAULT_TARGET_LANGUAGE_STANDARD = "default.cxx.standard"; //$NON-NLS-1$

	/**
	 * Needed for add_executable or add_library
	 */
	public static final String BUILD_TYPE = "build-type"; //$NON-NLS-1$

	/**
	 * Needed to define extension of resulting files
	 */
	public static final String EXTENSION = "build.configurations.extension"; //$NON-NLS-1$

	/**
	 * Needed to set property COMMON_LANGUAGE_RUNTIME
	 */
	public static final String CLR_SUPPORT = "build.configurations.clr.support"; //$NON-NLS-1$

	/**
	 * Unclear.
	 */
	public static final String OPTIMIZATION = "build.configurations.optimization"; //$NON-NLS-1$

	/**
	 * Needed to set preprocessor definitions.
	 */
	public static final String COMPILER_DEFINITIOINS = "build.configurations.preprocessor.definitions"; //$NON-NLS-1$

	/**
	 * for CONCAT and $<$<CONFIG:Debug>: parameters
	 */
	public static final String RUNTIME_LIBRARY = "build.configurations.runtime.library"; //$NON-NLS-1$

	/**
	 * Needed to set precompiled headers
	 */
	public static final String PRECOMPILED_HEADER = "build.configurations.precompiled.header"; //$NON-NLS-1$

	/**
	 * Unclear. Seems necessary though.
	 */
	public static final String ADDITIONAL_DEPENDENCIES = "build.configurations.additional.dependencies"; //$NON-NLS-1$

	/**
	 * To set $<$<CONFIG:Debug>: /SUBSYSTEM
	 */
	public static final String SUB_SYSTEM = "build.configurations.sub.system"; //$NON-NLS-1$

	/**
	 * To set $<$<CONFIG:Debug>: /MACHINE
	 */
	public static final String TARGET_MACHINE = "build.configurations.target.machine"; //$NON-NLS-1$

	/**
	 * Unclear
	 */
	public static final String ADDITIONAL_DIRECTORIES = "build.configurations.additional.include.directories"; //$NON-NLS-1$

	public static final String BUILD_TYPE_EXECUTABLE = "EXECUTABLE"; //$NON-NLS-1$

	public static final String BUILD_TYPE_LIBRARY = "build-type-library"; //$NON-NLS-1$

	public static final String BUILD_LIB_TYPE = "build-lib-type"; //$NON-NLS-1$

	public static final int BUILD_TYPE_LIB_STATIC = 1;

	public static final int BUILD_TYPE_LIB_DYNAMIC = 2;

	public static final int BUILD_TYPE_LIB_OBJECT = 3;

	public static final int BUILD_TYPE_LIB_INTERFACE = 4;

	public static final int BUILD_TYPE_LIB_MODULE = 5;

	public static final String BUILD_TYPE_GENERIC = "build.type.generic"; //$NON-NLS-1$

	public static final String FILES_TO_OPTIMIZE = "files.to.optimize"; //$NON-NLS-1$

	/**
	 * Property for {@link org.eclipse.core.resources.ProjectScope ProjectScope}.
	 * Must be a {@code boolean}
	 */
	public static final String ENABLE_OPT = "optimization.enable"; //$NON-NLS-1$

	public static final String CMAKE_PATH = "cmake-path"; //$NON-NLS-1$
	public static final String MSVC_PREFERNECES = "msvc.preferences"; //$NON-NLS-1$
	public static final String MSVC_PATH = "msvc.path"; // public static final String CMAKE_PATH = //$NON-NLS-1$
														// "cmake_path";

	public static final String NINJA_PATH = "ninja-path"; //$NON-NLS-1$

	public static final String CLANG_PATH = "clang-path"; //$NON-NLS-1$

	public static final String BUILD_SYSTEM_NINJA = "ninja"; //$NON-NLS-1$

	/**
	 * 
	 */
	public static final String TOOLCHAIN_LIST = "toolchain-list"; //$NON-NLS-1$

	public static final String TOOLCHAIN = "toolchain"; //$NON-NLS-1$
	public static final String TOOLCHAIN_NAME = "toolchain-name"; //$NON-NLS-1$
	public static final String TOOLCHAIN_PATH = "toolchain-path"; //$NON-NLS-1$

	/**
	 * Property for {@link org.eclipse.core.resources.ProjectScope ProjectScope}.
	 * Must be a {@link java.lang.String String}
	 */
	public static final String PREFERENCE_BUILD_SYSTEM = "project_build_system"; //$NON-NLS-1$

	/**
	 * Property for {@link org.eclipse.core.resources.ProjectScope ProjectScope}.
	 * Must be a {@link java.lang.String String}
	 */
	public static final String TARGET_ARCH = "project-target-arch"; //$NON-NLS-1$

	public static final String TARGET_ARCH_MIPS = "mips"; //$NON-NLS-1$
	public static final String TARGET_ARCH_KOMDIV = "komdiv"; //$NON-NLS-1$
	public static final String TARGET_ARCH_ARM = "arm"; //$NON-NLS-1$
	public static final String TARGET_ARCH_X64 = "x64"; //$NON-NLS-1$
	public static final String TARGET_ARCH_HOST = "host"; //$NON-NLS-1$
	public static final String TARGET_ARCH_HOST_CROSSCOMPILE = "host-crosscompile"; //$NON-NLS-1$

	public static final String OPT_FOLDER = "optimization-folder";

	public static final String BUILD_OUTPUT_NAME = "build-name";

	public static final String BUILD_ENV_VARS = "build-env-vars";

	public static final String SELECTED_TARGET_NAME = "selected-target-name";
	public static final String TARGETS_LIST = "targets-list";
	public static final String NODE_OF_TARGETS = "targets-container";

	public static final String TARGET_TYPE = "target-type";
	public static final String TARGET_ADDINION = "targets-addition";
	public static final String EMULATOR_FUNCTION_TARGETS = "emulator.function.targets";
	public static final String TARGET_LANGUAGE_STANDARD = "target.language.standard";
	public static final String TARGET_USES_DEFAULT_LANGUAGE_STANDARD = "target.uses.default.language.standard";
	public static final String PROJECT_SELECTED_LANGUAGE = "project.selected.language";
	public static final String LANGUAGE_C = "language.c";
	public static final String LANGUAGE_CXX = "language.cxx";

	public static final String LINKER_DEFENITIONS = "linker_defenitions"; //$NON-NLS-1$
}
