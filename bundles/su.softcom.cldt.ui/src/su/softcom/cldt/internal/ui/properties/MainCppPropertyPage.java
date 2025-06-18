package su.softcom.cldt.internal.ui.properties;

import java.util.List;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.IWorkbenchPropertyPage;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.builders.LanguageStandartManager;
import su.softcom.cldt.core.builders.LanguageStandartManager.ILanguageStandart;
import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * Property page for setup build parameters <br>
 * TODO:
 * <li>in plugin.xml change visibility from IProject to SoftwareCompoent
 * <li>may be migrate to eclipse properties from jface properties
 * 
 * 
 * @author petr.babanov@softcom.su
 *
 */
@SuppressWarnings("nls")
public class MainCppPropertyPage extends AbstractTargetPropertyPage implements IWorkbenchPropertyPage {

	Combo languageStandart;

	public static final String DEFAULT_CXX_STANARD = "cxx_std_11";
	public static final String DEFAULT_C_STANARD = "c_std_99";
	/**
	 * 
	 * CXX language: index = 1 <br>
	 * C language: index = 0
	 */
	Combo projectLanguage;

	/**
	 * default constructor
	 */
	public MainCppPropertyPage() {
		super();
	}

	@Override
	public void setElement(IAdaptable element) {
		super.setElement(element);
		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
			store = new ProjectScope(project).getNode(CMakeCorePlugin.PLUGIN_ID);
		} else {
			throw new IllegalArgumentException("Element must be adapt to IProject"); //$NON-NLS-1$
		}
	}

	@Override
	protected Control createContents(Composite parent) {
		Composite common = createCommon(parent);
		createLanguageSelectionGroup(common);

		return common;
	}

	void createLanguageSelectionGroup(Composite parent) {
		Group group = new Group(parent, SWT.NONE);
		group.setLayout(new GridLayout(3, true));
		group.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		group.setText("Общие настройки проекта");
		Label tLabel = new Label(group, SWT.NONE);
		tLabel.setText("Язык проекта");
		projectLanguage = new Combo(group, SWT.DROP_DOWN | SWT.BORDER);
		projectLanguage.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, true, 2, 1));
		projectLanguage.add("C"); //$NON-NLS-1$
		projectLanguage.add("C++"); //$NON-NLS-1$
		// CXX language = 1, C = 0
		projectLanguage.select(store.get(ICMakeProject.PROJECT_SELECTED_LANGUAGE, ICMakeProject.LANGUAGE_CXX)
				.equals(ICMakeProject.LANGUAGE_CXX) ? 1 : 0);
		projectLanguage.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				updateLanguageStandard();
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// empty
			}

		});
		Label lblNewLabel = new Label(group, SWT.NONE);
		lblNewLabel.setText("Стандарт языка");
		languageStandart = new Combo(group, SWT.DROP_DOWN | SWT.BORDER);
		languageStandart.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, true, 2, 1));
		updateLanguageStandard();
	}

	@Override
	public boolean performOk() {
		store.put(ICMakeProject.PROJECT_SELECTED_LANGUAGE,
				projectLanguage.getSelectionIndex() == 0 ? ICMakeProject.LANGUAGE_C : ICMakeProject.LANGUAGE_CXX);
		store.put(ICMakeProject.DEFAULT_TARGET_LANGUAGE_STANDARD, LanguageStandartManager
				.getLanguageStandartFromUIName(languageStandart.getText()).getPreferenceString());
		return super.performOk();
	}

	private void updateLanguageStandard() {
		languageStandart.removeAll();
		List<ILanguageStandart> languageStandarts = projectLanguage.getSelectionIndex() == 1
				? LanguageStandartManager.getAvailableLanguageStandarts(ICMakeProject.LANGUAGE_CXX)
				: LanguageStandartManager.getAvailableLanguageStandarts(ICMakeProject.LANGUAGE_C);
		languageStandarts.forEach(standart -> languageStandart.add(standart.getUIName()));

		if (LanguageStandartManager
				.standardIsC(store.get(ICMakeProject.DEFAULT_TARGET_LANGUAGE_STANDARD, DEFAULT_CXX_STANARD))
				&& projectLanguage.getSelectionIndex() == 1) {
			languageStandart.setText(
					LanguageStandartManager.getLanguageStandartFromPreferenceString(DEFAULT_CXX_STANARD).getUIName());
		} else if (!LanguageStandartManager
				.standardIsC(store.get(ICMakeProject.DEFAULT_TARGET_LANGUAGE_STANDARD, DEFAULT_CXX_STANARD))
				&& projectLanguage.getSelectionIndex() == 0) {
			languageStandart.setText(
					LanguageStandartManager.getLanguageStandartFromPreferenceString(DEFAULT_C_STANARD).getUIName());
		} else {
			languageStandart.setText(LanguageStandartManager
					.getLanguageStandartFromPreferenceString(
							store.get(ICMakeProject.DEFAULT_TARGET_LANGUAGE_STANDARD, DEFAULT_CXX_STANARD))
					.getUIName());
		}
	}

	@Override
	protected void performDefaults() {
		languageStandart.removeAll();
		List<ILanguageStandart> languageStandarts = projectLanguage.getSelectionIndex() == 1
				? LanguageStandartManager.getAvailableLanguageStandarts(ICMakeProject.LANGUAGE_CXX)
				: LanguageStandartManager.getAvailableLanguageStandarts(ICMakeProject.LANGUAGE_C);
		languageStandarts.forEach(standart -> languageStandart.add(standart.getUIName()));
		languageStandart.setText(
				LanguageStandartManager.getLanguageStandartFromPreferenceString(DEFAULT_CXX_STANARD).getUIName());
		projectLanguage.select(1);
		updateApplyButton();
	}

	@Override
	public boolean isValid() {
		return true;
	}

}
