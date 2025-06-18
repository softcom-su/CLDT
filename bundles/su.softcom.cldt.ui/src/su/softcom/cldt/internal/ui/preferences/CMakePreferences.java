package su.softcom.cldt.internal.ui.preferences;

import org.eclipse.core.runtime.preferences.InstanceScope;
import org.eclipse.jface.preference.BooleanFieldEditor;
import org.eclipse.jface.preference.FieldEditorPreferencePage;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPreferencePage;
import org.eclipse.ui.preferences.ScopedPreferenceStore;

import su.softcom.cldt.core.preferences.PreferenceConstants;

/**
 * Preference page for CMake preferences.
 * 
 * @author ershov_vladislav@softcom.su creator
 */
public class CMakePreferences extends FieldEditorPreferencePage implements IWorkbenchPreferencePage {

	@Override
	public void init(IWorkbench workbench) {
		setPreferenceStore(
				new ScopedPreferenceStore(InstanceScope.INSTANCE, PreferenceConstants.PREFERENCE_SCOPE_NAME));
	}

	@Override
	protected void createFieldEditors() {
		addField(new BooleanFieldEditor(PreferenceConstants.DISABLE_CMAKE_TEST,
				Messages.CMakePreferences_DisableCMakeTest, getFieldEditorParent()));
	}

}
