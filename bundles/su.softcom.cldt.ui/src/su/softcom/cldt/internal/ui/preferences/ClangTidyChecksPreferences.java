package su.softcom.cldt.internal.ui.preferences;

import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.eclipse.jface.preference.FieldEditorPreferencePage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPreferencePage;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.internal.ui.ClangTidyCheckboxTreeViewer;

/**
 * A class representing the clang-tidy checks preference page.
 */
public class ClangTidyChecksPreferences extends FieldEditorPreferencePage implements IWorkbenchPreferencePage {

	private static final String CLANG_TIDY = "clang-tidy"; //$NON-NLS-1$
	private ClangTidyCheckboxTreeViewer checksViewer;
	protected IEclipsePreferences prefs = (IEclipsePreferences) InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE)
			.node(CLANG_TIDY);

	@Override
	public void init(IWorkbench workbench) {
		setDescription("Выберите глобальные проверки clang-tidy");
	}

	@Override
	protected void createFieldEditors() {
		// TODO Auto-generated method stub
	}

	@Override
	protected Control createContents(Composite parent) {
		noDefaultAndApplyButton();

		Composite container = new Composite(parent, SWT.NONE);
		container.setLayout(new GridLayout(1, false));

		checksViewer = new ClangTidyCheckboxTreeViewer(container, SWT.CHECK | SWT.BORDER | SWT.V_SCROLL | SWT.H_SCROLL);
		GridData treeGridData = new GridData(SWT.FILL, SWT.FILL, true, true);
		checksViewer.getTree().setLayoutData(treeGridData);

		checksViewer.setUp();

		Composite buttonComposite = new Composite(container, SWT.NONE);
		buttonComposite.setLayout(new GridLayout(3, false));
		buttonComposite.setLayoutData(new GridData(SWT.FILL, SWT.BOTTOM, true, false));

		Button enableAllButton = new Button(buttonComposite, SWT.PUSH);
		enableAllButton.setText("Включить все");
		enableAllButton.addListener(SWT.Selection, e -> checksViewer.setAllChecks(true));

		Button disableAllButton = new Button(buttonComposite, SWT.PUSH);
		disableAllButton.setText("Отключить все");
		disableAllButton.addListener(SWT.Selection, e -> checksViewer.setAllChecks(false));

		Button restoreDefaultsButton = new Button(buttonComposite, SWT.PUSH);
		restoreDefaultsButton.setText("Восстановить значения по умолчанию");
		restoreDefaultsButton.addListener(SWT.Selection, e -> checksViewer.restoreDefaultChecks());

		Button applyButton = new Button(container, SWT.PUSH);
		applyButton.setText("Применить");
		applyButton.setLayoutData(new GridData(SWT.RIGHT, SWT.BOTTOM, false, false));
		applyButton.addListener(SWT.Selection, e -> performApply());

		String savedChecks = prefs.get("checks", ""); //$NON-NLS-1$ //$NON-NLS-2$
		checksViewer.applyChecksFromString(savedChecks);

		return container;
	}

	@Override
	public boolean performOk() {
		try {
			prefs.clear();
			prefs.put("checks", checksViewer.getSelectedChecksAsString()); //$NON-NLS-1$
			prefs.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
			return false;
		}

		return true;
	}
}
