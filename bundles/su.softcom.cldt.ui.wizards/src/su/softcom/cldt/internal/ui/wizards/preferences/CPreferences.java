package su.softcom.cldt.internal.ui.wizards.preferences;

import java.util.ResourceBundle;

import org.eclipse.jface.preference.DirectoryFieldEditor;
import org.eclipse.jface.preference.FieldEditorPreferencePage;
import org.eclipse.jface.preference.FileFieldEditor;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPreferencePage;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/**
 * C/C++ Preferences page.
 * 
 * @author victor.fadeev@softcom.su creator
 */
public class CPreferences extends FieldEditorPreferencePage implements IWorkbenchPreferencePage {

	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	public CPreferences() {
		super(GRID);
	}

	@Override
	public void init(IWorkbench workbench) {
		setPreferenceStore(CInitializer.getPreferenceStore());
		setDescription(RESOURCE_BUNDLE.getString("preferences.cpp.description")); //$NON-NLS-1$
	}

	@Override
	protected void createFieldEditors() {
		addField(new DirectoryFieldEditor(CInitializer.CLANG_PATH,
				RESOURCE_BUNDLE.getString("preferences.cpp.clangPath"), getFieldEditorParent())); //$NON-NLS-1$
		addField(new FileFieldEditor(CInitializer.CMAKE_PATH, RESOURCE_BUNDLE.getString("preferences.cpp.cmakePath"), //$NON-NLS-1$
				getFieldEditorParent()));
		addField(new DirectoryFieldEditor(CInitializer.INTEL_PATH,
				RESOURCE_BUNDLE.getString("preferences.cpp.intelPath"), getFieldEditorParent())); //$NON-NLS-1$

		Label label = new Label(getFieldEditorParent(), SWT.SEPARATOR | SWT.HORIZONTAL);
		label.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 3, 3));

		addField(new FileFieldEditor(CInitializer.NINJA_PATH, RESOURCE_BUNDLE.getString("preferences.cpp.ninjaPath"), //$NON-NLS-1$
				getFieldEditorParent()));

		label = new Label(getFieldEditorParent(), SWT.SEPARATOR | SWT.HORIZONTAL);
		label.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 3, 3));

		addField(new DirectoryFieldEditor(CInitializer.GCC_PATH, RESOURCE_BUNDLE.getString("preferences.cpp.gccPath"), //$NON-NLS-1$
				getFieldEditorParent()));
		addField(new FileFieldEditor(CInitializer.QMAKE_PATH, RESOURCE_BUNDLE.getString("preferences.cpp.qmakePath"), //$NON-NLS-1$
				getFieldEditorParent()));

		label = new Label(getFieldEditorParent(), SWT.SEPARATOR | SWT.HORIZONTAL);
		label.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 3, 3));

		addField(new FileFieldEditor(CInitializer.MSVC_2013, RESOURCE_BUNDLE.getString("preferences.cpp.msvc2013"), //$NON-NLS-1$
				getFieldEditorParent()));
		addField(
				new FileFieldEditor(CInitializer.TOOLSET_2013, RESOURCE_BUNDLE.getString("preferences.cpp.toolset2013"), //$NON-NLS-1$
						getFieldEditorParent()));

		addField(new FileFieldEditor(CInitializer.MSVC_2010, RESOURCE_BUNDLE.getString("preferences.cpp.msvc2010"), //$NON-NLS-1$
				getFieldEditorParent()));
		addField(
				new FileFieldEditor(CInitializer.TOOLSET_2010, RESOURCE_BUNDLE.getString("preferences.cpp.toolset2010"), //$NON-NLS-1$
						getFieldEditorParent()));

		addField(new FileFieldEditor(CInitializer.MSVC_2008, RESOURCE_BUNDLE.getString("preferences.cpp.msvc2008"), //$NON-NLS-1$
				getFieldEditorParent()));
		addField(
				new FileFieldEditor(CInitializer.TOOLSET_2008, RESOURCE_BUNDLE.getString("preferences.cpp.toolset2008"), //$NON-NLS-1$
						getFieldEditorParent()));

		addField(new FileFieldEditor(CInitializer.MSVC_2003, RESOURCE_BUNDLE.getString("preferences.cpp.msvc2003"), //$NON-NLS-1$
				getFieldEditorParent()));
		addField(
				new FileFieldEditor(CInitializer.TOOLSET_2003, RESOURCE_BUNDLE.getString("preferences.cpp.toolset2003"), //$NON-NLS-1$
						getFieldEditorParent()));
	}

}
