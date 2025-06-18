package su.softcom.cldt.core;

import java.nio.file.Paths;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.internal.core.Toolchain;

/**
 * Object representing the toolchains manager in the Eclipse preferences
 * hierarchy. Can be used as a context for searching for toolchains.
 */
public class ToolchainManager {

	private static ToolchainManager instance = new ToolchainManager();
	private static IEclipsePreferences instanceScope = InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE);

	/**
	 * Singleton instance of an Toolchain Manager object.
	 * 
	 * @return Instance of Toolchain Manager.
	 */
	public static ToolchainManager getInstance() {
		return instance;
	}

	/**
	 * Get selected Toolchain for project.
	 * 
	 * @param project for which the toolchain is being searched.
	 * 
	 * @return the toolchain associated with the project, or null if no toolchains
	 *         associated with the project.
	 */
	public IToolchain getToolchain(IProject project) {
		IEclipsePreferences store = new ProjectScope(project).getNode(PreferenceConstants.NODE);
		String name = store.get(PreferenceConstants.TOOLCHAIN, ""); //$NON-NLS-1$

		Map<String, IToolchain> toolchains;
		try {
			toolchains = this.getToolchainsList();
		} catch (BackingStoreException e) {
			return null;
		}

		return toolchains.get(name);
	}

	/**
	 * Get list of existing Toolchains.
	 * 
	 * @return list of toolchains.
	 * 
	 * @throws BackingStoreException
	 */
	public Map<String, IToolchain> getToolchainsList() throws BackingStoreException {
		Preferences toolchainListNode = instanceScope.node(PreferenceConstants.TOOLCHAIN_LIST);

		Map<String, IToolchain> list = new HashMap<String, IToolchain>();
		for (String name : toolchainListNode.childrenNames()) {
			Preferences toolchainNode = toolchainListNode.node(name);
			String toolchainName = toolchainNode.get(PreferenceConstants.TOOLCHAIN_NAME, ""); //$NON-NLS-1$
			if (toolchainName.length() > 0) {
				list.put(toolchainName, new Toolchain(toolchainName,
						Paths.get(toolchainNode.get(PreferenceConstants.TOOLCHAIN_PATH, "")))); //$NON-NLS-1$
			}
		}

		return list;
	}

	/**
	 * Add Toolchains list to Instance Scope.
	 * 
	 * @param toolchains list of toolchains.
	 * 
	 * @return true, if has no errors during the operation.
	 */
	public boolean setToolchainsList(Collection<IToolchain> toolchains) {
		Preferences toolchainListNode = instanceScope.node(PreferenceConstants.TOOLCHAIN_LIST);

		try {
			for (String name : toolchainListNode.childrenNames()) {
				toolchainListNode.node(name).removeNode();
			}
		} catch (BackingStoreException e) {
			return false;
		}

		for (IToolchain toolchain : toolchains) {
			Preferences toolchainNode = toolchainListNode.node(toolchain.getName());
			toolchainNode.put(PreferenceConstants.TOOLCHAIN_NAME, toolchain.getName());
			toolchainNode.put(PreferenceConstants.TOOLCHAIN_PATH, toolchain.getPath().toString());
		}

		try {
			instanceScope.flush();
		} catch (BackingStoreException e) {
			return false;
		}

		return true;
	}
}
