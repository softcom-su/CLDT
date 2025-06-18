package su.softcom.cldt.ui.tabs;

import java.io.File;

import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.ui.AbstractLaunchConfigurationTab;
import org.eclipse.debug.ui.ILaunchConfigurationTab;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.dialogs.ElementTreeSelectionDialog;
import org.eclipse.ui.model.WorkbenchContentProvider;
import org.eclipse.ui.model.WorkbenchLabelProvider;
import org.eclipse.ui.views.navigator.ResourceComparator;

import su.softcom.cldt.nativebuild.launch.BuildLaunchConfigurationDelegate;

public abstract class MainTab extends AbstractLaunchConfigurationTab implements ILaunchConfigurationTab {

	private Text fTxtFile;

	public Text getFTxtFile() {
		return fTxtFile;
	}

	public void setFTxtFile(Text fTxtFile) {
		this.fTxtFile = fTxtFile;
	}

	private String tabName;

	private boolean fDisableUpdate = false;

	public boolean getFDisableUpdate() {
		return fDisableUpdate;
	}

	public void setFDisableUpdate(boolean fDisableUpdate) {
		this.fDisableUpdate = fDisableUpdate;
	}

	public static final String FILE_LOCATION = BuildLaunchConfigurationDelegate.FILE_LOCATION;

	public MainTab(final String[] extensions, String tabName) {
		this.tabName = tabName;
	}

	@Override
	public void setDefaults(final ILaunchConfigurationWorkingCopy configuration) {
		configuration.setAttribute(FILE_LOCATION, "");
	}

	@Override
	public boolean isValid(final ILaunchConfiguration launchConfig) {
		// allow launch when a file is selected and file exists
		try {
			String fileName = ResourcesPlugin.getWorkspace().getRoot().getLocation().toPortableString()
					+ launchConfig.getAttribute(FILE_LOCATION, "");
			File file = new File(fileName);
			String fileTipe = getFileExtension(file);
			return file.exists() && (fileTipe.equals("bat") || fileTipe.equals("sh"));
		} catch (CoreException e) {
			// on any configuration error
			setErrorMessage("Некорректный атрибут" + FILE_LOCATION + ".");
		} catch (NullPointerException e1) {
			// on any configuration error
			setErrorMessage("Файл не существует");
		}

		return false;
	}

	private static String getFileExtension(File file) {
		String fileName = file.getName();
		if (fileName.lastIndexOf(".") != -1 && fileName.lastIndexOf(".") != 0) { //$NON-NLS-1$ //$NON-NLS-2$
			return fileName.substring(fileName.lastIndexOf(".") + 1); //$NON-NLS-1$
		} else {
			return ""; //$NON-NLS-1$
		}
	}

	@Override
	public boolean canSave() {
		return !fTxtFile.getText().isEmpty();
	}

	@Override
	public String getName() {
		return tabName;
	}

	protected void ScriptGroup(final Composite parent, Composite topControl) {
		Group grpLaunch = new Group(topControl, SWT.NONE);
		grpLaunch.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		grpLaunch.setText("Скрипт");
		grpLaunch.setLayout(new GridLayout(2, false));

		setFTxtFile(new Text(grpLaunch, SWT.BORDER));
		getFTxtFile().addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				if (!fDisableUpdate) {
					updateLaunchConfigurationDialog();
				}
			}

		});
		getFTxtFile().setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		Button btnBrowseScript = new Button(grpLaunch, SWT.NONE);
		btnBrowseScript.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				ElementTreeSelectionDialog dialog = new ElementTreeSelectionDialog(parent.getShell(),
						new WorkbenchLabelProvider(), new WorkbenchContentProvider());

				dialog.setTitle("Проект");
				dialog.setMessage("Выберете скрипт");
				dialog.setInput(ResourcesPlugin.getWorkspace().getRoot());
				dialog.setComparator(new ResourceComparator(ResourceComparator.NAME));
				if (dialog.open() == IDialogConstants.OK_ID) {
					IResource resource = (IResource) dialog.getFirstResult();
					fTxtFile.setText(resource.getFullPath().toPortableString());
				}
			}
		});
		btnBrowseScript.setText("Обзор");
	}

}
