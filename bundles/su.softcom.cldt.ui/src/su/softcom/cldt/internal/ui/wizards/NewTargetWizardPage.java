package su.softcom.cldt.internal.ui.wizards;

import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Stream;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.wizard.IWizardPage;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.builders.LanguageStandartManager;
import su.softcom.cldt.core.builders.LanguageStandartManager.ILanguageStandart;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.ui.TargetTypeDisplay;

public class NewTargetWizardPage extends WizardPage implements IWizardPage {

	private IProject[] availableProjects;
	private IProject project;
	private Set<String> targetNames;

	private Text targetNameText;
	private Combo projectCombo;
	private Combo targetTypeCombo;
	private Combo languageStandardCombo;

	public static final String USE_DEFAULT_LANGUAGE_STANDARD = "Умолчательный стандарт";
	public static final String FILL_TARGET_FIELDS = "Заполните поля для создания цели сборки";
	public static final String SELECT_PROJECT = "Выберите проект для создания цели";
	public static final String ERROR_FILL_TARGET_FIELD = "Ошибка: заполните поле имени цели";
	public static final String ERROR_TARGET_EXISTS = "Ошибка: цель с таким именем уже существует";
	public static final String ERROR_FILL_TYPE_FIELD = "Ошибка: укажите тип цели";
	public static final String ERROR_FILL_LANGUAGE_STANDARD_FIELD = "Ошибка: укажите стандарт языка";

	/**
	 * Creates a new wizard page and sets project if the provided one in not
	 * <code>null</code> (if it is <code>null</code>, page project selection is on,
	 * and off otherwise)
	 * 
	 * @param pageName
	 * @param project
	 */
	public NewTargetWizardPage(String pageName, IProject project) {
		super(pageName);
		availableProjects = ResourcesPlugin.getWorkspace().getRoot().getProjects();
		this.project = project;

		if (project != null) {
			setMessage(null);
		} else {
			setMessage(SELECT_PROJECT);
		}

		setPageComplete(false);
	}

	/**
	 * Manages a project selection if initial one was <code>null</code>
	 */
	public Listener projectSelectionHandler = event -> {
		for (IProject availableProject : availableProjects) {
			if (availableProject.getName().equals(projectCombo.getText())) {
				project = availableProject;
				break;
			}
		}
		setAvailableLanguageStandards();
	};

	/**
	 * Is designed to be triggered when any page field (text or combos) is changed.
	 * Sets page as complete if all fields are filled with correct data and as
	 * incomplete otherwise
	 */
	public Listener pageValidator = event -> setPageComplete(validatePage());

	@Override
	public void createControl(Composite parent) {
		Composite composite = new Composite(parent, SWT.NONE);
		composite.setLayout(new GridLayout(1, false));

		Composite projectGroup = new Composite(composite, SWT.NONE);
		projectGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
		projectGroup.setLayout(new GridLayout(2, true));

		Label projectLabel = new Label(projectGroup, SWT.NONE);
		projectLabel.setText("Проект для создания цели сборки:");

		projectCombo = new Combo(projectGroup, SWT.READ_ONLY | SWT.V_SCROLL | SWT.BORDER);
		projectCombo.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
		projectCombo.setItems(Stream.of(availableProjects).map(IProject::getName).toArray(String[]::new));
		projectCombo.addListener(SWT.Selection, projectSelectionHandler);

		Composite nameGroup = new Composite(composite, SWT.NONE);
		nameGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
		nameGroup.setLayout(new GridLayout(2, true));

		Label nameLabel = new Label(nameGroup, SWT.NONE);
		nameLabel.setText("Имя цели:"); //$NON-NLS-1$

		targetNameText = new Text(nameGroup, SWT.BORDER);
		targetNameText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
		targetNameText.addListener(SWT.Modify, pageValidator);

		Composite typeGroup = new Composite(composite, SWT.NONE);
		typeGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
		typeGroup.setLayout(new GridLayout(2, true));

		Label typeLabel = new Label(typeGroup, SWT.NONE);
		typeLabel.setText("Тип цели:");

		targetTypeCombo = new Combo(typeGroup, SWT.READ_ONLY | SWT.V_SCROLL | SWT.BORDER);
		targetTypeCombo.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
		targetTypeCombo.setItems(TargetTypeDisplay.getSupportedTypes().toArray(new String[0]));
		targetTypeCombo.addListener(SWT.Selection, pageValidator);

		Composite languageStandardGroup = new Composite(composite, SWT.NONE);
		languageStandardGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
		languageStandardGroup.setLayout(new GridLayout(2, true));

		Label languageStandardLabel = new Label(languageStandardGroup, SWT.NONE);
		languageStandardLabel.setText("Стандарт языка:");

		languageStandardCombo = new Combo(languageStandardGroup, SWT.READ_ONLY | SWT.V_SCROLL | SWT.BORDER);
		languageStandardCombo.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		// If the project is specified at this point, it means that it was initially
		// provided and the project selection should be disabled (available language
		// standards should be set as well)
		if (project != null) {
			projectCombo.setText(project.getName());
			projectCombo.setEnabled(false);
			setAvailableLanguageStandards();
		}
		setControl(composite);
		Dialog.applyDialogFont(composite);
	}

	private boolean validatePage() {
		if (targetNameText.getText().isEmpty()) {
			setErrorMessage(ERROR_FILL_TARGET_FIELD);
			return false;
		}

		if (targetNames.contains(targetNameText.getText())) {
			setErrorMessage(ERROR_TARGET_EXISTS);
			return false;
		}

		if (targetTypeCombo.getText().isEmpty()) {
			setErrorMessage(ERROR_FILL_TYPE_FIELD);
			return false;
		}

		setErrorMessage(null);
		setMessage(null);
		return true;
	}

	/**
	 * Returns a selected project for this page (may return <code>null</code> if
	 * called not from NewTargetWizard's overridden performFinish())
	 * 
	 * @return selected project
	 */
	public IProject getProject() {
		return project;
	}

	/**
	 * It is designed to be used only in NewTargetWizard's overridden
	 * performFinish(). If used in any other places, it may return <code>null</code>
	 * 
	 * @return new target
	 */
	public Target getNewTarget() {
		if (validatePage()) {
			String name = targetNameText.getText();
			ICMakeProject cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
			Target.Type type = TargetTypeDisplay.getType(targetTypeCombo.getText());
			String languageStandard = LanguageStandartManager
					.getLanguageStandartFromUIName(languageStandardCombo.getText()).getPreferenceString();
			return new Target(name, cmakeProject, type, languageStandard);// FIXME
		}
		return null;
	}

	/**
	 * Sets available language standards for the selected project
	 */
	private void setAvailableLanguageStandards() {
		List<ILanguageStandart> languageStandardsList;
		IEclipsePreferences store = new ProjectScope(project).getNode(PreferenceConstants.NODE);
		targetNames = new HashSet<>(Arrays
				.asList(store.get(PreferenceConstants.TARGETS_LIST, "").split(PreferenceConstants.FIELD_SEPARATOR)));

		languageStandardsList = LanguageStandartManager.getAvailableLanguageStandarts(
				store.get(PreferenceConstants.PROJECT_SELECTED_LANGUAGE, PreferenceConstants.LANGUAGE_CXX));
		languageStandardCombo
				.setItems(languageStandardsList.stream().map(ILanguageStandart::getUIName).toArray(String[]::new));
		languageStandardCombo.add(USE_DEFAULT_LANGUAGE_STANDARD);
		languageStandardCombo.setText(USE_DEFAULT_LANGUAGE_STANDARD);
		setMessage(FILL_TARGET_FIELDS);
	}

}
