package su.softcom.cldt.internal.ui.preferences;

import java.util.List;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import su.softcom.cldt.core.builders.LanguageStandartManager;
import su.softcom.cldt.core.builders.LanguageStandartManager.ILanguageStandart;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.ui.TargetTypeDisplay;

public class TargetDialog extends Dialog {

	public static final String USE_DEFAULT_LANGUAGE_STANDARD = "Умолчательный стандарт";

	private Button okButton;
	private Text targetNameText;
	private Combo targetTypeCombo;
	private Target target;

	private ICMakeProject cmakeProject;
	private Combo languageStandardCombo;
	private List<ILanguageStandart> languageStandardsList;

	public TargetDialog(Shell parentShell, Target target, List<ILanguageStandart> languageStandardsList,
			ICMakeProject cmakeProject) {
		super(parentShell);
		this.target = target;
		this.languageStandardsList = languageStandardsList;
		this.cmakeProject = cmakeProject;
	}

	@Override
	protected void buttonPressed(int buttonId) {
		if (buttonId == IDialogConstants.OK_ID) {
			String name = targetNameText.getText();
			Target.Type type = TargetTypeDisplay.getType(targetTypeCombo.getText());
			String languageStandard = LanguageStandartManager
					.getLanguageStandartFromUIName(languageStandardCombo.getText()).getPreferenceString();
			if (!name.isEmpty() && type != null) {
				target = new Target(name, cmakeProject, type, languageStandard);
			} else {
				return;
			}
		} else {
			target = null;
		}
		super.buttonPressed(buttonId);
	}

	@Override
	protected void configureShell(Shell shell) {
		super.configureShell(shell);
		shell.setText(target == null ? "Добавление цели сборки" : "Редактирование цели сборки");
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
		label.setText("Заполните поля");
		GridData data = new GridData(GridData.GRAB_HORIZONTAL | GridData.GRAB_VERTICAL | GridData.HORIZONTAL_ALIGN_FILL
				| GridData.VERTICAL_ALIGN_CENTER);
		data.widthHint = convertHorizontalDLUsToPixels(IDialogConstants.MINIMUM_MESSAGE_AREA_WIDTH);
		label.setLayoutData(data);
		label.setFont(parent.getFont());

		Composite nameGroup = new Composite(composite, SWT.NONE);
		nameGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, false, false, 1, 1));
		nameGroup.setLayout(new GridLayout(4, true));

		Label nameLabel = new Label(nameGroup, SWT.NONE);
		nameLabel.setText("Имя цели:");

		targetNameText = new Text(nameGroup, SWT.BORDER);
		targetNameText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 3, 1));
		targetNameText.setText(this.target != null ? this.target.getName() : ""); //$NON-NLS-1$

		Composite typeGroup = new Composite(composite, SWT.NONE);
		typeGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, false, false, 1, 1));
		typeGroup.setLayout(new GridLayout(4, true));

		Label typeLabel = new Label(typeGroup, SWT.NONE);
		typeLabel.setText("Тип цели:");

		targetTypeCombo = new Combo(typeGroup, SWT.NONE);
		targetTypeCombo.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 3, 1));
		targetTypeCombo.setItems(TargetTypeDisplay.getSupportedTypes().toArray(new String[0]));
		targetTypeCombo.setText(target != null ? TargetTypeDisplay.getDisplayName(target.getType()) : ""); //$NON-NLS-1$

		Composite additionGroup1 = new Composite(composite, SWT.NONE);
		additionGroup1.setLayoutData(new GridData(SWT.FILL, SWT.FILL, false, false, 1, 1));
		additionGroup1.setLayout(new GridLayout(4, true));

		Label languageStandardLabel = new Label(additionGroup1, SWT.NONE);
		languageStandardLabel.setText("Стандарт языка:");

		languageStandardCombo = new Combo(additionGroup1, SWT.NONE);
		languageStandardCombo.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 3, 1));
		languageStandardCombo.add(USE_DEFAULT_LANGUAGE_STANDARD);
		languageStandardsList.forEach(standartd -> languageStandardCombo.add(standartd.getUIName()));
		languageStandardCombo.setText(target == null ? USE_DEFAULT_LANGUAGE_STANDARD
				: LanguageStandartManager.getLanguageStandartUIName(target.getLanguageStandard()));

		applyDialogFont(composite);
		return composite;
	}

	protected Button getOkButton() {
		return okButton;
	}

	public Target getResult() {
		return target;
	}

}
