package su.softcom.cldt.internal.debug.ui;

import static org.eclipse.swt.events.SelectionListener.widgetSelectedAdapter;

import java.io.File;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.ui.AbstractLaunchConfigurationTab;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;
import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.CmakeProjectNature;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.debug.core.DebugConstants;
import su.softcom.cldt.debug.ui.filters.ExecutableFilter;
import su.softcom.cldt.ui.dialogs.ProjectSelectionDialog;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.FocusListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.fieldassist.AutoCompleteField;
import org.eclipse.jface.fieldassist.ComboContentAdapter;
import org.eclipse.jface.viewers.ComboViewer;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.LabelProvider;

public class DebugLaunchMainTab extends AbstractLaunchConfigurationTab {
	
	// Project UI widgets
	private Text fProjectText;
	private Text fPrgrmArgText;
	private Combo fComboTarget;
	private Button fBreakOnMain;
	private ComboViewer fComboViewer;
	private String launchMode;
	private List<Target> targets;
	private String currentBuildType = "";
	private String selectedTarget = "";
	

	private class WrongBuildTypeDialog extends MessageDialog {

		public WrongBuildTypeDialog(Shell parentShell, String currentBuildType) {
			super(parentShell, "Неправильный тип сборки", null,
					"Выбранный проект собран в режиме %s".formatted(currentBuildType),
					MessageDialog.WARNING, 0, "Продолжить");
		}

		@Override
		protected Control createCustomArea(Composite parent) {
			Label msgLbl = new Label(parent, SWT.SINGLE);
			msgLbl.setText("Многие функции отладки будут недостпуны. Чтобы включить их, соберите проект в режиме Debug");
			return parent;
		}

	}

	private class TargetLabelProvider extends LabelProvider {

		@Override
		public String getText(Object element) {
			if (element instanceof Target tgt) {
				return tgt.getName();
			}
			return super.getText(element);
		}
	}

	public DebugLaunchMainTab(String mode) {
		this.launchMode = mode;
	}

	/**
	 * @wbp.parser.entryPoint
	 */
	@Override
	public void createControl(Composite parent) {
		Composite composite = new Composite(parent, SWT.NONE);
		setControl(composite);
		composite.setLayout(new GridLayout(1, false));
		createProjectField(composite);
		createTargetField(composite);
		createArgsField(composite);
		createBreakOnMainCheckbox(composite);
		if (launchMode.equals(ILaunchManager.DEBUG_MODE)) {
			setWarningMessage("Для корректной работы отладки проект должен быть собран в режиме Debug");
		}

	}

	private void createArgsField(Composite parent) {
		Group argsGroup = new Group(parent, SWT.NONE);
		argsGroup.setLayout(new GridLayout(1, false));
		GridData gridData = new GridData(SWT.FILL, SWT.TOP, true, false);
		gridData.heightHint = 200;
		argsGroup.setLayoutData(gridData);
		argsGroup.setText("Аргументы программы");
		fPrgrmArgText = new Text(argsGroup, SWT.MULTI | SWT.WRAP | SWT.BORDER | SWT.V_SCROLL);
		fPrgrmArgText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		fPrgrmArgText.addModifyListener(evt -> scheduleUpdateJob());
		Label argLbl = new Label(argsGroup, SWT.NONE);
		argLbl.setText("*аргументы разделены переносом строки");
		argLbl.setEnabled(false);
	}

	private void createTargetField(Composite parent) {

		Group targetGroup = new Group(parent, SWT.NONE);
		targetGroup.setLayout(new GridLayout(1, false));
		targetGroup.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
		targetGroup.setText("Цель сборки");
		fComboViewer = new ComboViewer(targetGroup, SWT.NONE);
		fComboTarget = fComboViewer.getCombo();
		fComboTarget.addModifyListener(evt -> scheduleUpdateJob());
		AutoCompleteField autoField = new AutoCompleteField(fComboTarget, new ComboContentAdapter(), new String[] {});

		fComboViewer.setContentProvider((IStructuredContentProvider) inputElement -> {
		    if (inputElement instanceof Collection<?> c) {
		        return c.toArray();
		    }
		    return new Object[0];
		});
		fComboViewer.setLabelProvider(new TargetLabelProvider());
		fComboViewer.setFilters(new ExecutableFilter());
		fComboTarget.addSelectionListener(new SelectionListener() {
			
			@Override
			public void widgetSelected(SelectionEvent e) {
				selectedTarget = fComboTarget.getText();
				scheduleUpdateJob();
			}
			
			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// empty
				
			}
		});
		fComboTarget.addFocusListener(new FocusListener() {

			@Override
			public void focusLost(FocusEvent e) {
				// empty
			}

			@Override
			public void focusGained(FocusEvent e) {
				autoField.setProposals(getTargetStrs());
			}
		});
		fComboTarget.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
	}

	private void createProjectField(Composite parent) {

		Button fProjectButton;
		SelectionListener projectListener = new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				ProjectSelectionDialog d = new ProjectSelectionDialog(getShell());
				d.setInput(ResourcesPlugin.getWorkspace().getRoot());
				d.setTitle("Выбор проекта");
				if (d.open() == Window.OK) {
					IProject prj = (IProject) d.getFirstResult();
					fProjectText.setText(prj.getName());
					if (ILaunchManager.DEBUG_MODE.equals(launchMode)) {
						try {
							currentBuildType = prj.getActiveBuildConfig().getName();
							if (checkProjectBuildStatus(currentBuildType)) {
								WrongBuildTypeDialog dialog = new WrongBuildTypeDialog(getShell(), currentBuildType);
								dialog.open();
							}
						} catch (CoreException e1) {
							e1.printStackTrace();
						}
					}
					updateLaunchConfigurationDialog();
				}
			}


			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// empty
			}
		};

		Group projectGroup = new Group(parent, SWT.NONE);
		projectGroup.setLayout(new GridLayout(2, false));
		projectGroup.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
		projectGroup.setText("Проект");
		fProjectText = new Text(projectGroup, SWT.BORDER);
		fProjectText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));
		fProjectButton = new Button(projectGroup, SWT.PUSH);
		fProjectButton.setText("Обзор");

		fProjectButton.addSelectionListener(projectListener);
		fProjectText.addModifyListener(event -> {
			scheduleUpdateJob();
			if (!fProjectText.getText().isBlank()) {
				ICMakeProject prj = CMakeCorePlugin.eINSTANCE.getProject(fProjectText.getText());
				if (prj != null) {
					targets = prj.getTargets();
				}
				fComboViewer.setInput(targets);
			}
		});
	}

	private void createBreakOnMainCheckbox(Composite parent) {
		fBreakOnMain = createCheckButton(parent, "Остановить на точке входа");
		if (ILaunchManager.RUN_MODE.equals(launchMode)) {
			fBreakOnMain.setSelection(false);
			fBreakOnMain.setEnabled(false);
		}
		GridData data = new GridData(GridData.HORIZONTAL_ALIGN_FILL);
		data.horizontalSpan = 2;
		fBreakOnMain.setLayoutData(data);
		fBreakOnMain.setFont(parent.getFont());
		fBreakOnMain.addSelectionListener(widgetSelectedAdapter(e -> updateLaunchConfigurationDialog()));
	}

	@Override
	public void setDefaults(ILaunchConfigurationWorkingCopy config) {
		// empty
	}

	@Override
	public void initializeFrom(ILaunchConfiguration configuration) {
		try {
			fProjectText.setText(configuration.getAttribute(DebugConstants.PROJECT_NAME, ""));
			fComboTarget.setText(configuration.getAttribute(DebugConstants.TARGET_NAME, ""));

			fPrgrmArgText.setText(configuration.getAttribute(DebugConstants.LAUNCH_ARGUMENTS, ""));
			if (ILaunchManager.DEBUG_MODE.equals(launchMode)) {
				fBreakOnMain.setSelection(configuration.getAttribute(DebugConstants.MAIN_BREAK, true));
			}
		} catch (CoreException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void performApply(ILaunchConfigurationWorkingCopy configuration) {

		if (ILaunchManager.DEBUG_MODE.equals(launchMode)) {
			configuration.setAttribute(DebugConstants.MAIN_BREAK, fBreakOnMain.getSelection());
		} 
		configuration.setAttribute(DebugConstants.LAUNCH_ARGUMENTS, getAttributeValueFrom(fPrgrmArgText));
		configuration.setAttribute(DebugConstants.TARGET_NAME, fComboTarget.getText());
		configuration.setAttribute(DebugConstants.PROJECT_NAME, getAttributeValueFrom(fProjectText));
		configuration.setAttribute(DebugConstants.WRONG_BUILD_TYPE_FOR_DEBUG, checkProjectBuildStatus(currentBuildType));
	}

	protected String getAttributeValueFrom(Text text) {
		String content = text.getText().trim();
		if (!content.isEmpty()) {
			return content;
		}
		return null;
	}

	@Override
	public String getName() {
		return "Главная";
	}

	private String[] getTargetStrs() {
		List<String> list = new ArrayList<>();
		for (Target tgt : targets) {
			for (IFile ifile : tgt.getArtifacts()) {
				File file = ifile.getRawLocation().makeAbsolute().toFile();
				if (file.canExecute()) {
					list.add(tgt.getName());
				}
			}
		}
		return list.toArray(new String[0]);
	}
	
	/**
	 * Возвращает TRUE, если проект собран в неподоходящем для отладки режиме
	 * @param currentBuildType
	 * @return
	 */
	private boolean checkProjectBuildStatus(String currentBuildType) {
		return !(currentBuildType.equals(CmakeProjectNature.DEBUG_BUILD_TYPE) || currentBuildType.equals(CmakeProjectNature.REL_WITH_DEBUG_INFO_BUILD_TYPE) );
	}
}
