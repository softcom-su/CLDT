package su.softcom.cldt.internal.ui.wizards.preferences;

import java.util.ResourceBundle;

import org.eclipse.jface.preference.BooleanFieldEditor;
import org.eclipse.jface.preference.DirectoryFieldEditor;
import org.eclipse.jface.preference.FieldEditorPreferencePage;
import org.eclipse.jface.preference.FileFieldEditor;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPreferencePage;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Toolchain Preferences page. */
public class ToolchainPreferences extends FieldEditorPreferencePage implements IWorkbenchPreferencePage {

	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	public ToolchainPreferences() {
		super(GRID);
	}

	@Override
	public void init(IWorkbench workbench) {
		setPreferenceStore(CInitializer.getPreferenceStore());
		setDescription(RESOURCE_BUNDLE.getString("preferences.toolchain.description")); //$NON-NLS-1$
	}

	@Override
	protected void createFieldEditors() {
		addField(new DirectoryFieldEditor(CInitializer.MIPS_TOOLCHAIN_PATH,
				RESOURCE_BUNDLE.getString("preferences.toolchain.mipsToolchainPath"), getFieldEditorParent())); //$NON-NLS-1$
		addField(new BooleanFieldEditor(CInitializer.IS_SOFT_FLOAT,
				RESOURCE_BUNDLE.getString("preferences.toolchain.isSoftFloat"), getFieldEditorParent())); //$NON-NLS-1$

		Label label = new Label(getFieldEditorParent(), SWT.SEPARATOR | SWT.HORIZONTAL);
		label.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 3, 3));

		addField(new DirectoryFieldEditor(CInitializer.ARM_TOOLCHAIN_PATH,
				RESOURCE_BUNDLE.getString("preferences.toolchain.armToolchainPath"), getFieldEditorParent())); //$NON-NLS-1$
		addField(
				new FileFieldEditor(CInitializer.BASH_PATH, RESOURCE_BUNDLE.getString("preferences.toolchain.bashPath"), //$NON-NLS-1$
						getFieldEditorParent()));
	}

}
