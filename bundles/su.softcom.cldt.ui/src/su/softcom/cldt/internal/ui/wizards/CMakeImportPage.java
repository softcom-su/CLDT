package su.softcom.cldt.internal.ui.wizards;

import java.io.File;
import java.nio.file.Paths;
import java.util.Objects;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.wizard.IWizardPage;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

public class CMakeImportPage extends WizardPage implements IWizardPage {

	IProject project;

	static String buildFolderName = "build"; //$NON-NLS-1$

	private Text importRootText;
	private Text buildFolderText;
	private Text targetProjectText;
	private Text cmakeListsDirText;
	private Label lblCMakeLists;
	private Button cmakeListsBtn;

	private File buildFolder;
	private File rootFile;
	private File sourceFile;

	public CMakeImportPage() {
		super("Импорт проекта CMake");
		setTitle();
	}

	private void setTitle() {
		setTitle("Импорт проекта CMake");
		setDescription("Параметры импортируемого проекта");
	}

	@Override
	public void createControl(Composite parent) {
		Composite container = new Composite(parent, SWT.NONE);
		container.setLayout(new GridLayout(2, true));
		GridData fdComposite = new GridData();
		container.setLayoutData(fdComposite);

		createRootField(container);
		createBuildFolderFiled(container);
		createExtraSettingsField(container);
		setControl(container);
	}

	private void createExtraSettingsField(Composite container) {
		Group group = new Group(container, SWT.NONE);
		group.setText("Дополнительные настройки");
		group.setToolTipText("Возможно только после указания корневой директории");
		group.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 2, 1));
		group.setLayout(new GridLayout(3, false));
		lblCMakeLists = new Label(group, SWT.NONE);
		lblCMakeLists.setText("Расположение CMakeLists.txt: ");
		lblCMakeLists.setToolTipText("Уточнить расположение файла внутри корневой папки.");
		cmakeListsDirText = new Text(group, SWT.BORDER);
		cmakeListsDirText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true));
		cmakeListsDirText.addModifyListener(event -> {
			if (event.getSource() instanceof Text txt) {
				sourceFile = Paths.get(txt.getText()).toFile();
				validateCMakeLists();
			}
		});
		cmakeListsBtn = new Button(group, SWT.PUSH);
		cmakeListsBtn.setText("Обзор");
		cmakeListsBtn.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				DirectoryDialog d = new DirectoryDialog(getShell());
				if (rootFile != null) {
					d.setFilterPath(rootFile.getAbsolutePath());
				}
				d.setMessage("Укажите расположение файла CMakeLists.txt внутри корневой директории");
				String path = d.open();
				if (path != null && !path.isBlank()) {
					cmakeListsDirText.setText(path);
				}
				validate();
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				widgetSelected(e);
			}
		});
		if (cmakeListsDirText.getText().isBlank()) {
			lblCMakeLists.setEnabled(false);
			cmakeListsDirText.setEnabled(false);
			cmakeListsBtn.setEnabled(false);
		}

	}

	private void createBuildFolderFiled(Composite container) {
		Button defaultBuildNameBtn;
		Group grpBuildFolder = new Group(container, SWT.NONE);
		grpBuildFolder.setText("Директория сборки");
		grpBuildFolder.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 2, 1));
		grpBuildFolder.setLayout(new GridLayout(2, false));
		defaultBuildNameBtn = new Button(grpBuildFolder, SWT.CHECK);
		defaultBuildNameBtn.setSelection(true);
		defaultBuildNameBtn.addListener(SWT.Selection, event -> {
			if (defaultBuildNameBtn.getSelection()) {
				buildFolderText.setEnabled(false);
				buildFolderText.setText(buildFolderName);
			} else {
				buildFolderText.setEnabled(true);
			}
		});

		Label lblBtn = new Label(grpBuildFolder, SWT.NONE);
		lblBtn.setText("Использовать умолчательную директорию сборки");

		buildFolderText = new Text(grpBuildFolder, SWT.BORDER);
		buildFolderText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 2, 1));
		buildFolderText.setText(buildFolderName); // $NON-NLS-1$
		buildFolderText.setEnabled(false);
		buildFolderText.addModifyListener(e -> {
			buildFolder = Paths.get(buildFolderText.getText()).toFile();
			validateBuildFolder();
		});
	}

	private void createRootField(Composite container) {
		Group grpImportRoot = new Group(container, SWT.NONE);
		grpImportRoot.setText("Основные настройки");
		grpImportRoot.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 2, 1));
		grpImportRoot.setLayout(new GridLayout(3, false));

		Label importRootLbl = new Label(grpImportRoot, SWT.NONE);
		importRootLbl.setText("Корневая директория: ");
		importRootText = new Text(grpImportRoot, SWT.BORDER);
		importRootText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		importRootText.addModifyListener(event -> {
			if (event.getSource() instanceof Text txt) {
				rootFile = Paths.get(txt.getText()).toFile();
				validateCMakeLists();
				validateRootDir();
			}
		});

		Button btnImportRoot = new Button(grpImportRoot, SWT.NONE);
		btnImportRoot.setText("Обзор");
		btnImportRoot.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				DirectoryDialog d = new DirectoryDialog(getShell());
				d.setMessage("Выберите корневую папку проекта");
				String pathStr = d.open();
				buildFolder = Paths.get(buildFolderText.getText()).toFile();
				if (pathStr != null && !pathStr.isBlank()) {
					importRootText.setText(pathStr);
					Path path = new Path(pathStr);
					if (path.segmentCount() > 0) {
						String newName = path.segment(path.segmentCount() - 1);
						targetProjectText.setText(createUniqueName(newName));
						cmakeListsDirText.setText(pathStr);
					}
				}
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				widgetSelected(e);
			}
		});
		Label newNameLbl = new Label(grpImportRoot, SWT.NONE);
		newNameLbl.setText("Имя нового проекта: ");
		targetProjectText = new Text(grpImportRoot, SWT.BORDER);
		targetProjectText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 2, 1));
		targetProjectText.addModifyListener(e -> validate());
		if (project != null) {
			targetProjectText.setText(project.getName());
		}
	}

	private String createUniqueName(String oldName) {
		String newName = oldName;
		int newNumber = 1;
		if (!ResourcesPlugin.getWorkspace().getRoot().getProject(newName).exists()) {
			return newName;
		}
		while (ResourcesPlugin.getWorkspace().getRoot().getProject(newName).exists()) {
			newName = oldName + "_" + newNumber; //$NON-NLS-1$
			newNumber += 1;
		}
		return newName;
	}

	public String getSourceFolderName() {
		if (Objects.equals(importRootText.getText(), cmakeListsDirText.getText())) {
			return "";
		}
		return sourceFile.getName();
	}

	public String getBuildFolderName() {
		return buildFolder.getName();
	}

	public File getRoot() {
		return rootFile;
	}

	public File getCMakeListsPlace() {
		return new File(cmakeListsDirText.getText() + IPath.SEPARATOR + "CMakeLists.txt");
	}

	public IProject getProject() {
		return project;
	}

	boolean validateBuildFolder() {
		if ((rootFile != null) && new File(rootFile, buildFolderName).exists()) {
			setMessage("ИмпортируемыЙ проект содержит папку %s, ее содержимое может быть потеряно"
					.formatted(buildFolderName), WARNING);
			return false;
		}
		return true;
	}

	boolean validateRootDir() {
		if (!rootFile.exists()) {
			setErrorMessage("Корневая директория не найдена");
			setPageComplete(false);
			return false;
		} else {
			setErrorMessage(null);
			setPageComplete(true);
			return true;
		}
	}

	boolean validateCMakeLists() {
		if (!(Paths.get(rootFile.getAbsolutePath(), "CMakeLists.txt").toFile().exists() //$NON-NLS-1$
				|| Paths.get(cmakeListsDirText.getText(), "CMakeLists.txt").toFile().exists())) { //$NON-NLS-1$

			setErrorMessage("Файл CMakeLists.txt не найден. Укажите его расположение внутри корневой директории");
			setPageComplete(false);
			cmakeListsDirText.setEnabled(true);
			lblCMakeLists.setEnabled(true);
			cmakeListsBtn.setEnabled(true);
			return false;
		} else {
			setErrorMessage(null);
			setPageComplete(true);
			return true;
		}
	}

	private void validate() {
		setMessage(null);
		if (!validateCMakeLists()) {
			return;
		}
		if (!validateRootDir()) {
			return;
		}
		validateBuildFolder();
		project = ResourcesPlugin.getWorkspace().getRoot().getProject(targetProjectText.getText());
	}

}
