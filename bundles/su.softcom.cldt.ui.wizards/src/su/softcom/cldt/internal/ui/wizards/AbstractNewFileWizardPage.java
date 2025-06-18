package su.softcom.cldt.internal.ui.wizards;

import java.io.File;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.regex.Pattern;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.window.Window;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SegmentEvent;
import org.eclipse.swt.events.SegmentListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.model.BaseWorkbenchContentProvider;
import org.eclipse.ui.model.WorkbenchContentProvider;
import org.eclipse.ui.model.WorkbenchLabelProvider;

/**
 * Abstract new file wizard page.
 * 
 * @author Vladislav Ershov (ershov_vladislav@softcom.su) - creator
 */
public abstract class AbstractNewFileWizardPage extends WizardPage {

	/** The project in which the file is created. */
	private IProject project = null;
	/** List of allowed file extensions. */
	private List<String> possibleFileExtensions;
	/** Possible extension message. */
	private String possibleFileExtensionMessage;

	/** The name of project in which the file is created. */
	private String projectNameString;
	/** Relative path to the folder in which the file is created. */
	private String folderPathString;
	/** Text field for relative path to the folder in which the file is created. */
	private Text folderPathText;
	/** Button for browse folder path. */
	private Button buttonFolderPath;
	/** File name. */
	private String fileName = ""; //$NON-NLS-1$
	/** Is source file. */
	private boolean addToBuild = true;

	/**
	 * Constructs abstract new file wizard page with the given title, project,
	 * possibleFileExtensions and possibleFileExtensionMessage.
	 * 
	 * @param title                        the wizard page title
	 * @param project                      the given project
	 * @param folderPath                   the given folder path
	 * @param possibleFileExtensions       possible file extensions
	 * @param possibleFileExtensionMessage possible file extensions message
	 */
	public AbstractNewFileWizardPage(String title, Optional<IProject> project, IPath folderPath,
			List<String> possibleFileExtensions, String possibleFileExtensionMessage) {
		super(AbstractNewFileWizardPage.class.getName());
		setTitle(title);
		if (project.isPresent()) {
			this.project = project.get();
			this.projectNameString = this.project.getName();
		}
		if (folderPath != Path.EMPTY) {
			this.folderPathString = folderPath.toOSString();
		}
		this.possibleFileExtensions = possibleFileExtensions;
		this.possibleFileExtensionMessage = possibleFileExtensionMessage;
		setPageComplete(false);
	}

	/**
	 * Check invalid file name.
	 */
	protected void checkInvalidFileName() {
		if (projectNameString.equals("")) { //$NON-NLS-1$
			setPageComplete(false);
			setErrorMessage(Messages.AbstractNewFileWizardPage_SelectProject);
			return;
		} else if (folderPathString.equals("")) { //$NON-NLS-1$
			setPageComplete(false);
			setErrorMessage(Messages.AbstractNewFileWizardPage_SelectFolder);
			return;
		} else if (fileName.equals("")) { //$NON-NLS-1$
			setPageComplete(false);
			setErrorMessage(Messages.AbstractNewFileWizardPage_EnterFileName);
			return;
		} else if (!fileName.contains(".") || Pattern.matches(".*\\.", fileName)) { //$NON-NLS-1$ //$NON-NLS-2$
			setPageComplete(false);
			setErrorMessage(Messages.AbstractNewFileWizardPage_EnterFileExtension);
			return;
		} else if (!possibleFileExtensions.contains(fileName.split("\\.")[fileName.split("\\.").length - 1])) { //$NON-NLS-1$//$NON-NLS-2$
			setPageComplete(false);
			setErrorMessage(possibleFileExtensionMessage);
			return;
		}

		File file = new File(fileName);
		try {
			Paths.get(file.toURI());
			setPageComplete(true);
			setErrorMessage(null);
		} catch (IllegalArgumentException e) {
			setPageComplete(false);
			setErrorMessage(Messages.AbstractNewFileWizardPage_IncorrectFileName);
		}
	}

	@Override
	public void createControl(Composite parent) {
		initializeDialogUnits(parent);

		Composite composite = new Composite(parent, SWT.NONE);
		composite.setLayout(new GridLayout());
		composite.setLayoutData(new GridData(GridData.FILL_BOTH));
		composite.setFont(parent.getFont());

		createCompositeProjectName(composite);
		createCompositeFolderPath(composite);
		createCompositeFileName(composite);
		createCompositeAddToBuild(composite);

		composite.layout();
		setControl(composite);
	}

	protected void createCompositeFileName(Composite composite) {
		Composite compositeFileName = new Composite(composite, SWT.FILL);
		compositeFileName.setLayout(new GridLayout(7, true));
		compositeFileName.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

		Label fileNameLabel = new Label(compositeFileName, SWT.FILL);
		fileNameLabel.setText(Messages.AbstractNewFileWizardPage_FileName);
		fileNameLabel.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 2, 1));

		Text fileNameText = new Text(compositeFileName, SWT.BORDER | SWT.FILL);
		fileNameText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 5, 1));
		fileNameText.addSegmentListener(new SegmentListener() {
			@Override
			public void getSegments(SegmentEvent event) {
				fileName = ((Text) event.getSource()).getText();
				checkInvalidFileName();
			}
		});
	}

	protected void createCompositeFolderPath(Composite composite) {
		Composite compositeFolderPath = new Composite(composite, SWT.FILL);
		compositeFolderPath.setLayout(new GridLayout(7, true));
		compositeFolderPath.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

		Label folderPathLabel = new Label(compositeFolderPath, SWT.FILL);
		folderPathLabel.setText(Messages.AbstractNewFileWizardPage_FileCreationFolder);
		folderPathLabel.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 2, 1));

		folderPathText = new Text(compositeFolderPath, SWT.BORDER | SWT.FILL | SWT.READ_ONLY);

		if (project == null) {
			folderPathText.setText(""); //$NON-NLS-1$
		} else {
			folderPathText.setText(folderPathString);
		}

		folderPathText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 4, 1));
		folderPathText.addSegmentListener(new SegmentListener() {
			@Override
			public void getSegments(SegmentEvent event) {
				folderPathString = ((Text) event.getSource()).getText();
			}
		});

		buttonFolderPath = new Button(compositeFolderPath, SWT.NONE);
		buttonFolderPath.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 1, 1));
		buttonFolderPath.setText(Messages.AbstractNewFileWizardPage_Browse);
		buttonFolderPath.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent event) {
				ContainerSelectionDialog dialog = new ContainerSelectionDialog(composite.getShell(),
						new WorkbenchLabelProvider(), new WorkbenchContentProvider(),
						Messages.AbstractNewFileWizardPage_NoFolderSelected);

				dialog.setTitle(Messages.AbstractNewFileWizardPage_FolderSelection);
				dialog.setMessage(Messages.AbstractNewFileWizardPage_ChooseFolder);
				dialog.setInput(project);

				if (dialog.open() == Window.OK) {
					IResource resource = (IResource) dialog.getFirstResult();
					if (resource != null) {
						folderPathString = Paths.get(resource.getFullPath().toOSString()).toString();
						folderPathText.setText(folderPathString);
					}
					checkInvalidFileName();
				}
			}
		});

		if (project == null) {
			buttonFolderPath.setEnabled(false);
		}
	}

	protected void createCompositeAddToBuild(Composite composite) {
		Composite compositeFileName = new Composite(composite, SWT.FILL);
		compositeFileName.setLayout(new GridLayout(7, true));
		compositeFileName.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

		Label addToBuildLabel = new Label(compositeFileName, SWT.FILL);
		addToBuildLabel.setText(Messages.AbstractNewFileWizardPage_AddToSourceFile);
		addToBuildLabel.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 2, 1));

		Button addToBuildButton = new Button(compositeFileName, SWT.CHECK);
		addToBuildButton.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 5, 1));
		addToBuildButton.setSelection(true);
		addToBuildButton.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				addToBuild = addToBuildButton.getSelection();
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// Not need implement
			}

		});
	}

	protected void createCompositeProjectName(Composite composite) {
		Composite compositeProjectName = new Composite(composite, SWT.FILL);
		compositeProjectName.setLayout(new GridLayout(7, true));
		compositeProjectName.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

		Label projectNameLabel = new Label(compositeProjectName, SWT.FILL);
		projectNameLabel.setText(Messages.AbstractNewFileWizardPage_Project);
		projectNameLabel.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 2, 1));

		Text projectNameText = new Text(compositeProjectName, SWT.BORDER | SWT.FILL | SWT.READ_ONLY);

		if (project == null) {
			projectNameText.setText(""); //$NON-NLS-1$
		} else {
			projectNameText.setText(project.getName());
		}

		projectNameText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 4, 1));
		projectNameText.addSegmentListener(new SegmentListener() {
			@Override
			public void getSegments(SegmentEvent event) {
				projectNameString = ((Text) event.getSource()).getText();
			}
		});

		Button buttonProjectName = new Button(compositeProjectName, SWT.NONE);
		buttonProjectName.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, true, 1, 1));
		buttonProjectName.setText(Messages.AbstractNewFileWizardPage_Browse);
		buttonProjectName.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent event) {
				ContainerSelectionDialog dialog = new ContainerSelectionDialog(composite.getShell(),
						new WorkbenchLabelProvider(), getProjectTreeContentProvider(),
						Messages.AbstractNewFileWizardPage_NoProjectSelected);

				dialog.setTitle(Messages.AbstractNewFileWizardPage_ProjectSelection);
				dialog.setMessage(Messages.AbstractNewFileWizardPage_ChooseProject);
				dialog.setInput(ResourcesPlugin.getWorkspace().getRoot());

				if (dialog.open() == Window.OK) {
					IResource resource = (IResource) dialog.getFirstResult();
					if (resource != null) {
						project = resource.getProject();
						projectNameString = resource.getProject().getName();
						folderPathString = resource.getProject().getFullPath().toOSString();
						folderPathText.setText(resource.getProject().getFullPath().toOSString());
						projectNameText.setText(projectNameString);
						buttonFolderPath.setEnabled(true);
					}
					checkInvalidFileName();
				}
			}
		});

	}

	/**
	 * Get file name.
	 * 
	 * @return file name
	 */
	public String getFileName() {
		return fileName;
	}

	/**
	 * Get relative path to the folder in which the file is created.
	 * 
	 * @return folder path
	 */
	public IPath getFolderPath() {
		return new Path(folderPathString);
	}

	/**
	 * Get project in which the file is created.
	 * 
	 * @return project in which the file is created
	 */
	public IProject getProject() {
		return project;
	}

	/**
	 * Add file to build.
	 * 
	 * @return add to build
	 */
	public boolean addToBuild() {
		return addToBuild;
	}

	private ITreeContentProvider getProjectTreeContentProvider() {
		return new BaseWorkbenchContentProvider() {
			@Override
			public Object[] getElements(final Object inputObject) {
				if (inputObject instanceof IWorkspaceRoot) {
					IWorkspaceRoot root = (IWorkspaceRoot) inputObject;
					List<IProject> projects = new ArrayList<IProject>(Arrays.asList(root.getProjects()));
					projects.remove(root.getProject(".su.softcom.hsa.solution")); //$NON-NLS-1$
					return projects.toArray();
				}
				return super.getElements(inputObject);
			}

			@Override
			public Object[] getChildren(final Object element) {
				if (element instanceof IProject) {
					return new Object[0];
				}
				return super.getChildren(element);
			}
		};
	}

}
