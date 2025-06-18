package su.softcom.cldt.internal.ui.preferences;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.eclipse.jface.preference.FieldEditorPreferencePage;
import org.eclipse.jface.preference.FileFieldEditor;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.eclipse.ui.preferences.ScopedPreferenceStore;

import su.softcom.cldt.core.preferences.PreferenceConstants;

public class DeployOptionsPage extends FieldEditorPreferencePage implements IWorkbenchPropertyPage {

	IProject project;

	/**
	 * default constructor
	 */
	public DeployOptionsPage() {
		super();
	}

	@Override
	public IAdaptable getElement() {
		return project;
	}

	@Override
	public void setElement(IAdaptable element) {
		if (element instanceof IProject) {
			project = (IProject) element;
		}

		setPreferenceStore(new ScopedPreferenceStore(InstanceScope.INSTANCE, "su.softcom.cldt.nativebuild"));
	}

	@Override
	protected void createFieldEditors() {
		FileFieldEditor scriptBrowser = new FileFieldEditor(PreferenceConstants.NATIVEBUILD_DEPLOY_SCRIPT,
				"&Выберите скрипт:", true, getFieldEditorParent());
		addField(scriptBrowser);
	}

}
