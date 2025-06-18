package su.softcom.cldt.ui.dialogs;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.ElementTreeSelectionDialog;
import org.eclipse.ui.model.WorkbenchContentProvider;
import org.eclipse.ui.model.WorkbenchLabelProvider;

public class FileSelectionDialog extends ElementTreeSelectionDialog {

	/**
	 * Constructs an instance of <code>BuildFolderSelectionDialog</code>.
	 *
	 * @param parent The parent shell for the dialog
	 */
	public FileSelectionDialog(Shell parent) {
		super(parent, new WorkbenchLabelProvider(), new WorkbenchContentProvider());
	}

	@Override
	protected void updateOKStatus() {
		Status status;
		if (this.getResult().length == 0) {
			status = new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR, "Файл не выбран", null);
			updateStatus(status);
			return;
		}
		for (Object o : this.getResult()) {
			if (!(o instanceof IFile)) {
				status = new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR, "Выбран не файл", null);
				updateStatus(status);
				return;
			}
		}
		status = new Status(IStatus.OK, PlatformUI.PLUGIN_ID, IStatus.OK, "", null); //$NON-NLS-1$
		updateStatus(status);
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		Composite composite = (Composite) super.createDialogArea(parent);
		return composite;
	}

}
