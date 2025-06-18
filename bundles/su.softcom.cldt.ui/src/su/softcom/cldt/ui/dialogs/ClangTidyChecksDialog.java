package su.softcom.cldt.ui.dialogs;

import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.dialogs.IMessageProvider;
import org.eclipse.jface.dialogs.TitleAreaDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;

import su.softcom.cldt.internal.ui.ClangTidyCheckboxTreeViewer;

/**
 * A class representing a dialog box for selecting clang-tidy checks.
 */
public class ClangTidyChecksDialog extends TitleAreaDialog {
	private ClangTidyCheckboxTreeViewer checksViewer;
	private static final String CHECKS = "checks"; //$NON-NLS-1$
	private String enabledChecks;

	/**
	 * Creates an instance for ClangTidyChecksDialog.
	 * 
	 * @param parent     parent shell.
	 * @param parameters clang-tidy
	 */
	public ClangTidyChecksDialog(Shell parent, String enabledChecks) {
		super(parent);
		this.enabledChecks = enabledChecks;
	}

	public String getSelectedChecksAsString() {
		return enabledChecks;
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		getShell().setText("Выбор проверок для проекта");
		setTitle("Настройка проверок clang-tidy");
		setMessage("Выберите проверки, которые будут применяться к проекту", IMessageProvider.INFORMATION);

		Composite container = (Composite) super.createDialogArea(parent);
		container.setLayout(new GridLayout(1, false));

		checksViewer = new ClangTidyCheckboxTreeViewer(container, SWT.CHECK | SWT.BORDER | SWT.V_SCROLL | SWT.H_SCROLL);
		GridData treeGridData = new GridData(SWT.FILL, SWT.FILL, true, true);
		checksViewer.getTree().setLayoutData(treeGridData);

		checksViewer.setUp();

		createButtonPanel(container);

		checksViewer.applyChecksFromString(enabledChecks);

		return container;
	}

	@Override
	protected void okPressed() {
		enabledChecks = checksViewer.getSelectedChecksAsString();

		super.okPressed();
	}

	@Override
	protected void createButtonsForButtonBar(Composite parent) {
		createButton(parent, IDialogConstants.OK_ID, IDialogConstants.OK_LABEL, true);
		createButton(parent, IDialogConstants.CANCEL_ID, IDialogConstants.CANCEL_LABEL, false);
	}

	private void createButtonPanel(Composite parent) {
		Composite buttonPanel = new Composite(parent, SWT.NONE);
		buttonPanel.setLayout(new GridLayout(3, true));
		buttonPanel.setLayoutData(new GridData(SWT.FILL, SWT.BOTTOM, true, false));

		Button enableAll = new Button(buttonPanel, SWT.PUSH);
		enableAll.setText("Включить все");
		enableAll.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));
		enableAll.addListener(SWT.Selection, e -> checksViewer.setAllChecks(true));

		Button disableAll = new Button(buttonPanel, SWT.PUSH);
		disableAll.setText("Отключить все");
		disableAll.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));
		disableAll.addListener(SWT.Selection, e -> checksViewer.setAllChecks(false));

		Button defaults = new Button(buttonPanel, SWT.PUSH);
		defaults.setText("По умолчанию");
		defaults.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));
		defaults.addListener(SWT.Selection, e -> checksViewer.restoreDefaultChecks());
	}
}