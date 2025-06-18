package su.softcom.cldt.core.preferences;

/**
 * Preference constants for CLDT.
 * 
 * @author Petr Babanov (petr.babanov@softcom.su) - creator
 * @author Vladislav Ershov (ershov_vladislav@softcom.su) - add
 *         PREFERENCE_SCOPE_NAME, DISABLE_CMAKE_TEST,
 *         DISABLE_CMAKE_TEST_DEFAULT_VALUE, ADD_CLANG_MATH_LIB,
 *         INTERNAL_LIBRARIES, EXTERNAL_LIBRARIES
 */
public interface PreferenceConstants {

	/**
	 * Project preference for set CMake flag.
	 * 
	 */
	static final String CLDT_PROJECT_PREFERENCE_SAFETY_LINK = "su.softcom.cldt.ui.project.preference.safetyLink"; //$NON-NLS-1$
	/**
	 * Target architecture for build to host executable.
	 */
	static final String CLDT_PROJECT_PREFERENCE_TARGET_ARCHITECTURE = "su.softcom.cldt.ui.project.preference.targetArch"; //$NON-NLS-1$
	/**
	 * Generator for CMake build.
	 */
	static final String CLDT_PROJECT_PREFERENCE_BUILD_SYSTEM = "su.softcom.cldt.ui.project.preference.buildSystem"; //$NON-NLS-1$
	/**
	 * Compiler using for build process.
	 */
	static final String CLDT_PROJECT_PREFERENCE_COMPILER = "su.softcom.cldt.ui.project.preference.compiler"; //$NON-NLS-1$
	/**
	 * JSON notated environment variables for build.
	 */
	static final String CLDT_PROJECT_PREFERENCE_BUILD_ENV_VAR = "su.softcom.cldt.ui.project.preference.build.enviroment.var"; //$NON-NLS-1$
	/**
	 * name for get and set scoped preference store.
	 */
	static final String CLDT_PROJECT_PREFERENCE_STORE_NAME = "compiler.preferences"; //$NON-NLS-1$
	/**
	 * target build directory.
	 */
	static final String CLDT_PROJECT_BUILD_FOLDER = "su.softcom.cldt.ui.project.preference.buildFolder"; //$NON-NLS-1$
	/**
	 * target build directory default value.
	 */
	static final String CLDT_PROJECT_BUILD_FOLDER_DEFAULT = "build"; //$NON-NLS-1$
	/**
	 * Environment for build.
	 */
	static final String CLDT_COMPILERS_CLANG = "clang"; //$NON-NLS-1$
	/**
	 * clang compiler with MSVC-styled options.
	 */
	static final String CLDT_COMPILERS_CLANG_CL = "clang-cl"; //$NON-NLS-1$
	/**
	 * ninja build tool.
	 */
	static final String CLDT_BUILD_SYSTEM_NINJA = "ninja"; //$NON-NLS-1$
	/**
	 * MSVC build tool
	 */
	static final String CLDT_BUILD_SYSTEM_MSVC = "Visual Studio 16 2019"; //$NON-NLS-1$
	/**
	 * <code>x64</code> target architecture.
	 */
	static final String CLDT_TARGET_PLATFORM_X64 = "x64"; //$NON-NLS-1$
	/**
	 * <code>x32</code> target architecture.
	 */
	static final String CLDT_TARGET_PLATFORM_X32 = "x32"; //$NON-NLS-1$

	static final String CLDT_PROJECT_PREFERENCE_PLATFORM_NAME = "su.softcom.cldt.ui.project.preference.platform"; //$NON-NLS-1$
	/**
	 * platform name Intel.
	 */
	static final String CLDT_PLATFORM_INTEL = "intel"; //$NON-NLS-1$
	/**
	 * platform name ARM.
	 */
	static final String CLDT_PLATFORM_ARM = "arm"; //$NON-NLS-1$
	/**
	 * platform name MIPS.
	 */
	static final String CLDT_PLATFORM_MIPS = "mips"; //$NON-NLS-1$
	/**
	 * platform name komdiv.
	 */
	static final String CLDT_PLATFORM_KOMDIV = "komdiv"; //$NON-NLS-1$
	/**
	 * Build mode.
	 */
	static final String CLDT_PROJECT_PREFERENCE_BUILD_MODE = "build.mode"; //$NON-NLS-1$
	/**
	 * Build debug.
	 */
	static final String CLDT_BUILD_DEBUG = "debug"; //$NON-NLS-1$
	/**
	 * Build RelWithDebInfo: optimized, *with* debug info, but no debug (output)
	 * code or asserts.
	 */
	static final String CLDT_BUILD_REL_WITH_DEB_INFO = "relWithDebInfo"; //$NON-NLS-1$
	/**
	 * Build MinSizeRel: same as Release but optimizing for size rather than speed.
	 */
	static final String CLDT_BUILD_MIN_SIZE_REL = "minSizeRel"; //$NON-NLS-1$
	/**
	 * Build release.
	 */
	static final String CLDT_BUILD_RELEASE = "release"; //$NON-NLS-1$
	/**
	 * Execution file.
	 */
	static final String CLDT_EXECUTION_FILE = "su.softcom.cldt.ui.project.preference.executionFile"; //$NON-NLS-1$
	/**
	 * Execution file default value.
	 */
	static final String CLDT_EXECUTION_FILE_DEFAULT = "execution_file"; //$NON-NLS-1$
	/**
	 * Build arguments.
	 */
	static final String CLDT_BUILD_ARGUMENTS = "build.arguments"; //$NON-NLS-1$

	/** Preferences constnts for e4 preference. */
	/** Preferences scope name for CLDT. */
	public static final String PREFERENCE_SCOPE_NAME = "su.softcom.cldt.preferences"; //$NON-NLS-1$
	/** Disable CMake test. */
	public static final String DISABLE_CMAKE_TEST = "disableCMakeTest"; //$NON-NLS-1$
	/** Disable CMake test default value. */
	public static final boolean DISABLE_CMAKE_TEST_DEFAULT_VALUE = false;
	/** Add clang math library. */
	@Deprecated
	public static final String ADD_CLANG_MATH_LIB = "addClangMathLib"; //$NON-NLS-1$
	/** Key for saving external libraries. */
	public static final String EXTERNAL_LIBRARIES = "externalLibraries"; //$NON-NLS-1$
	/** Key for saving internal libraries. */
	public static final String INTERNAL_LIBRARIES = "internalLibraries"; //$NON-NLS-1$

	public static final String NATIVEBUILD_BUILD_SCRIPT = "nativebuild.build.script"; //$NON-NLS-1$
	public static final String NATIVEBUILD_BUILD_COMPILER = "nativebuild.build.compiler"; //$NON-NLS-1$
	public static final String NATIVEBUILD_DEPLOY_SCRIPT = "nativebuild.deploy.script"; //$NON-NLS-1$
	public static final String SRC_FILES = "source_files"; //$NON-NLS-1$
	public static final String INC_FOLDERS = "includes_dirs"; //$NON-NLS-1$
	public static final String NODE = "su.softcom.cldt.preferences"; //$NON-NLS-1$
	public static final String EXECUTABLE_NAME = "executable_name"; //$NON-NLS-1$

}
