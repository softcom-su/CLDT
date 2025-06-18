package su.softcom.cldt.internal.ui.preferences;

import java.nio.file.Paths;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import su.softcom.cldt.core.IToolchain;
import su.softcom.cldt.internal.core.Toolchain;

public class AddToolchainDialog extends Dialog {

	class ShowExplorerHandler implements Listener {

		Text textToFill;
		String stringToSet;

		ShowExplorerHandler(Text textToFill, String stringToSet) {
			this.textToFill = textToFill;
			this.stringToSet = stringToSet;
		}

		@Override
		public void handleEvent(Event event) {
			if (event.type == SWT.Selection) {
				FileDialog dialog = new FileDialog(getShell(), SWT.SHEET);
				dialog.setText(""); //$NON-NLS-1$
				String filename = dialog.open();
				if (filename != null) {
					textToFill.setText(filename);
					stringToSet = filename;
				}
			}
		}
	}

	private Button okButton;

	private Text toolchainNameText;
	private Text toolchainPathText;

	private IToolchain toolchain;

	protected AddToolchainDialog(Shell parentShell) {
		super(parentShell);
	}

	@Override
	protected void buttonPressed(int buttonId) {
		if (buttonId == IDialogConstants.OK_ID) {
			String name = toolchainNameText.getText();
			String path = toolchainPathText.getText();
			if (name.length() > 0 && path.length() > 0) {
				this.toolchain = new Toolchain(toolchainNameText.getText(), Paths.get(toolchainPathText.getText()));
			} else {
				this.toolchain = null;
			}
		} else {
			this.toolchain = null;
		}
		super.buttonPressed(buttonId);
	}

	@Override
	protected void configureShell(Shell shell) {
		super.configureShell(shell);
		shell.setText("Добавление тулчейна");
	}

	@Override
	protected void createButtonsForButtonBar(Composite parent) {
		okButton = createButton(parent, IDialogConstants.OK_ID, IDialogConstants.OK_LABEL, true);
		createButton(parent, IDialogConstants.CANCEL_ID, IDialogConstants.CANCEL_LABEL, false);
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		Composite composite = (Composite) super.createDialogArea(parent);

		Label label = new Label(composite, SWT.WRAP);
		label.setText("Укажите название тулчейна и путь к исполняемому файлу");
		GridData data = new GridData(GridData.GRAB_HORIZONTAL | GridData.GRAB_VERTICAL | GridData.HORIZONTAL_ALIGN_FILL
				| GridData.VERTICAL_ALIGN_CENTER);
		data.widthHint = convertHorizontalDLUsToPixels(IDialogConstants.MINIMUM_MESSAGE_AREA_WIDTH);
		label.setLayoutData(data);
		label.setFont(parent.getFont());

		Composite toolchainNameGroup = new Composite(composite, SWT.NONE);
		toolchainNameGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, false, false, 1, 1));
		toolchainNameGroup.setLayout(new GridLayout(4, true));

		Label toolchainNameLabel = new Label(toolchainNameGroup, SWT.NONE);
		toolchainNameLabel.setText("Название:");

		this.toolchainNameText = new Text(toolchainNameGroup, SWT.BORDER);
		this.toolchainNameText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 2, 1));
		this.toolchainNameText.setText(toolchain != null ? toolchain.getName() : ""); //$NON-NLS-1$

		Composite toolchainPathGroup = new Composite(composite, SWT.NONE);
		toolchainPathGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, false, false, 1, 1));
		toolchainPathGroup.setLayout(new GridLayout(4, true));

		Label toolchainPathLabel = new Label(toolchainPathGroup, SWT.NONE);
		toolchainPathLabel.setText("Расположение:");

		String toolchainPath = toolchain != null ? toolchain.getPath().toString() : ""; //$NON-NLS-1$
		this.toolchainPathText = new Text(toolchainPathGroup, SWT.BORDER);
		this.toolchainPathText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 2, 1));
		this.toolchainPathText.setText(toolchainPath);

		Button toolchainPathBrowseButton = new Button(toolchainPathGroup, SWT.NONE);
		toolchainPathBrowseButton.setText("Открыть...");
		toolchainPathBrowseButton.addListener(SWT.Selection, new ShowExplorerHandler(this.toolchainPathText, null));
		toolchainPathBrowseButton.setLayoutData(new GridData(SWT.RIGHT, SWT.FILL, true, false, 1, 1));

		applyDialogFont(composite);
		return composite;
	}

	protected Button getOkButton() {
		return okButton;
	}

	public IToolchain getResult() {
		return this.toolchain;
	}
}
