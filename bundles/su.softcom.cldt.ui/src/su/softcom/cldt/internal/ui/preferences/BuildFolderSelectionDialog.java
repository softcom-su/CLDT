package su.softcom.cldt.internal.ui.preferences;

import java.io.File;

import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.viewers.ILabelProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.ElementTreeSelectionDialog;

/**
 * A class to select elements out of a tree structure for select build folder.
 *
 * @author ershov_vladislav@softcom.su
 */
public class BuildFolderSelectionDialog extends ElementTreeSelectionDialog {

	/** Button for create folder. */
	private Button createButton;
	/** Target project. */
	private IProject project = null;

	/**
	 * Constructs an instance of <code>BuildFolderSelectionDialog</code>.
	 *
	 * @param parent          The parent shell for the dialog
	 * @param labelProvider   the label provider to render the entries. It must be
	 *                        compatible with the Viewerreturned from
	 *                        {@link #doCreateTreeViewer(Composite, int)}
	 * @param contentProvider the content provider to evaluate the tree structure
	 * @param project         target project
	 */
	public BuildFolderSelectionDialog(Shell parent, ILabelProvider labelProvider, ITreeContentProvider contentProvider,
			IProject project) {
		super(parent, labelProvider, contentProvider);

		this.project = project;
	}

	@Override
	protected void updateOKStatus() {
		Status status;
		if (getFirstResult() == null) {
			status = new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR,
					Messages.BuildFolderFieldEditor_ChooseNotFolder, null);
		} else {
			File buildFolderPath = new File(((IResource) getFirstResult()).getLocation().toOSString());
			if (!buildFolderPath.isDirectory()) {
				status = new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR,
						Messages.BuildFolderFieldEditor_ChooseNotFolder, null);
			} else {
				status = new Status(IStatus.OK, PlatformUI.PLUGIN_ID, IStatus.OK, "", //$NON-NLS-1$
						null);
			}
		}

		updateStatus(status);
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		Composite composite = (Composite) super.createDialogArea(parent);

		createButton = new Button(composite, SWT.FLAT);
		createButton.setText(Messages.BuildFolderSelectionDialog_CreateFolder);
		createButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				CreateFolderDialog dialog = new CreateFolderDialog(parent.getShell());

				if (dialog.open() == IDialogConstants.OK_ID) {
					String nameFolder = dialog.getNameFolder();
					if (nameFolder != "") { //$NON-NLS-1$
						IFolder folder;
						if (getFirstResult() != null && getFirstResult() instanceof IFolder) {
							IFolder parentFolder = (IFolder) ((IResource) getFirstResult());
							folder = parentFolder.getFolder(nameFolder);
						} else {
							folder = project.getFolder(nameFolder);
						}

						try {
							createFolder(folder);
						} catch (CoreException e1) {
							Platform.getLog(BuildFolderSelectionDialog.class).error(e1.getMessage());
						}
					}
				}
			}
		});

		return composite;
	}

	/**
	 * Creating a project folder.
	 * 
	 * @param folder project folder
	 * 
	 * @throws CoreException if an error occurs when creating a folder
	 */
	private void createFolder(IFolder folder) throws CoreException {
		if (!folder.exists()) {
			folder.create(IResource.FORCE | IResource.DERIVED, true, new NullProgressMonitor());
		}
	}

}
