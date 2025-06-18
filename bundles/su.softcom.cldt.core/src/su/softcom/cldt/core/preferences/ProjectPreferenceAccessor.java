package su.softcom.cldt.core.preferences;

import java.util.HashMap;
import java.util.Map;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.ui.preferences.ScopedPreferenceStore;

/**
 * Class for access to build property for selected project
 * 
 * 
 * @author petr.babanov@softcom.su
 *
 */
public class ProjectPreferenceAccessor {

	/**
	 * Default compiler is clang <br>
	 * Will be replaced to clang after generating preferences in import process
	 */
	public static final String DEFAULT_COMPILER = PreferenceConstants.CLDT_COMPILERS_CLANG;
	/**
	 * Default build tool
	 */
	public static final String DEFAULT_BUILD_TOOL = PreferenceConstants.CLDT_BUILD_SYSTEM_NINJA;
	/**
	 * default empty environment
	 */
	public static final String DEFAULT_BUILD_ENV = "{}"; //$NON-NLS-1$

	static Map<String, String> defaultValues = new HashMap<String, String>();

	static {
		defaultValues.put(PreferenceConstants.CLDT_PROJECT_PREFERENCE_BUILD_SYSTEM, DEFAULT_BUILD_TOOL);
		defaultValues.put(PreferenceConstants.CLDT_PROJECT_BUILD_FOLDER,
				PreferenceConstants.CLDT_PROJECT_BUILD_FOLDER_DEFAULT);
		defaultValues.put(PreferenceConstants.CLDT_EXECUTION_FILE, PreferenceConstants.CLDT_EXECUTION_FILE_DEFAULT);
		defaultValues.put(PreferenceConstants.CLDT_PROJECT_PREFERENCE_TARGET_ARCHITECTURE, "x32"); //$NON-NLS-1$
		defaultValues.put(PreferenceConstants.CLDT_PROJECT_PREFERENCE_COMPILER, DEFAULT_COMPILER);
		defaultValues.put(PreferenceConstants.CLDT_PROJECT_PREFERENCE_PLATFORM_NAME,
				PreferenceConstants.CLDT_PLATFORM_INTEL);
		defaultValues.put(PreferenceConstants.CLDT_PROJECT_PREFERENCE_BUILD_ENV_VAR, DEFAULT_BUILD_ENV);
	}

	/**
	 * returns preference for project by name or default value if preference not
	 * selected
	 * 
	 * @param project <code>IProject</code>
	 * @param name    -- name of preference
	 * @return value of selected preference
	 */
	public static String getPreference(IProject project, String name) {
		ScopedPreferenceStore store = new ScopedPreferenceStore(new ProjectScope(project),
				PreferenceConstants.CLDT_PROJECT_PREFERENCE_STORE_NAME);
		if (store.getString(name).equals("")) { //$NON-NLS-1$
			return defaultValues.get(name);
		} else {
			return store.getString(name);
		}
	}

}
