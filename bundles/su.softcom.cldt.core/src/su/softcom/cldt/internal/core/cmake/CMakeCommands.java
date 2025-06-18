package su.softcom.cldt.internal.core.cmake;

/**
 * Storage for CMake command strings
 * 
 * @author petrb
 *
 */
public interface CMakeCommands {

	public static final String SOURCES_COMMAND = "target_sources"; //$NON-NLS-1$

	public static final String EXEC_TARGET_COMMAND = "add_executable"; //$NON-NLS-1$

	public static final String LIB_TARGET_COMMAND = "add_library"; //$NON-NLS-1$

	public static final String HEADERS_FOLDERS_COMMAND = "target_include_directories"; //$NON-NLS-1$

	public static final String COMPILE_OPTIONS = "target_compile_options"; //$NON-NLS-1$

	public static final String INCLUDE_EXT_LIBS = "target_link_libraries"; //$NON-NLS-1$

	public static final String IMPORT_COMMAND = "include"; //$NON-NLS-1$

	public static final String CUSTOM_COMMAND = "add_custom_command";

}
