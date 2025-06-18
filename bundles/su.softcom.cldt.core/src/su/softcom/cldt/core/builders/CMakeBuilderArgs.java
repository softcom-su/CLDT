package su.softcom.cldt.core.builders;

public interface CMakeBuilderArgs {

	/** Builder id for the CMake build. */
	public static String BUILDER_ID = "su.softcom.cldt.core.cmakeBuilder"; //$NON-NLS-1$

	public static String TARGET_NAME = "target-name"; //$NON-NLS-1$

	/** Location of the root CMakeLists.txt. */
	public static final String ROOT_DIR = "cmake-root-dir"; //$NON-NLS-1$

	/**
	 * The key for specifying the type of CMake build. Expected values
	 * <ul>
	 * <li>{@link su.softcom.cldt.core.builders.CMakeBuilderArgs#BUILD_TYPE_RELEASE
	 * BUILD_TYPE_RELEASE}</li>
	 * <li>{@link su.softcom.cldt.core.builders.CMakeBuilderArgs#BUILD_TYPE_DEBUG
	 * BUILD_TYPE_DEBUG}</li>
	 * </ul>
	 */
	public static final String BUILD_TYPE = "cmake-build-type"; //$NON-NLS-1$
	/** The value for specifying the build type is Release. */
	public static final String BUILD_TYPE_RELEASE = "release"; //$NON-NLS-1$
	/** The value for specifying the build type is Debug. */
	public static final String BUILD_TYPE_DEBUG = "debug"; //$NON-NLS-1$

	public static final String TARGET_ARCH = "target-arch";

	public static final String ARCH_MIPS = "mips";
	public static final String ARCH_KOMDIV = "komdiv";
	public static final String ARCH_ARM = "arm";

	public static final String TOOLCHAIN_FILE = "cmake-toolchain-file"; //$NON-NLS-1$
	public static final String DYNAMIC_ANALYSIS_TYPE = "dynamic-analysis-type"; //$NON-NLS-1$
	/**
	 * A semicolon separated list of unix style absolute paths for Clang static
	 * analysis.
	 */
	public static final String ANALYSIS_SOURCES = "analysis-sources"; //$NON-NLS-1$
	/** Output console name. */
	public static final String OUTPUT_CONSOLE_NAME = "output-console-name"; //$NON-NLS-1$
	/** Marker type to generate. */
	public static final String MARKER_TYPE = "marker-type"; //$NON-NLS-1$

	public static final String BUILD_FOLDER = "build-folder"; //$NON-NLS-1$

	/**
	 * Additional cmake arguments, passing to cmake call with -D flag. Expected
	 * semicolon separated pairs {@code key=value}
	 */
	public static final String ADDITIONAL_CMAKE_ARGS = "cmake-arguments"; //$NON-NLS-1$

}
