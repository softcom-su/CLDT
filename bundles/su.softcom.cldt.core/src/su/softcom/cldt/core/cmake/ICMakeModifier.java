package su.softcom.cldt.core.cmake;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.CoreException;

public interface ICMakeModifier {

	public static final String I_CMAKE_MODIFIER_ID = "su.softcom.cldt.core.cmakeModifier";

	public static final String CMAKE_LIST_FILE_NAME = "CMakeLists.txt"; //$NON-NLS-1$
	public static final String CMAKE_IMPORT_FILE_NAME = "Import.cmake"; //$NON-NLS-1$

	public String getFileName();

	public CMakeRoot run(IProject project, CMakeRoot tree, String fileName)
			throws IllegalArgumentException, CoreException;

}
