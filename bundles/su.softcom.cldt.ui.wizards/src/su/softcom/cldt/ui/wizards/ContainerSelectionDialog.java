package su.softcom.cldt.ui.wizards;

import java.io.File;

import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.jface.viewers.ILabelProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.ElementTreeSelectionDialog;

/**
 * A class to select elements out of a tree structure for select container.
 *
 * @author ershov_vladislav@softcom.su
 */
@Deprecated
public class ContainerSelectionDialog extends ElementTreeSelectionDialog {

	/** Error message. */
	private String errorMessage;

	/**
	 * Constructs an instance of <code>ContainerSelectionDialog</code>.
	 *
	 * @param parent          the parent shell for the dialog
	 * @param labelProvider   the label provider to render the entries. It must be
	 *                        compatible with the Viewerreturned from
	 *                        {@link #doCreateTreeViewer(Composite, int)}
	 * @param contentProvider the content provider to evaluate the tree structure
	 * @param errorMessage    error message
	 */
	public ContainerSelectionDialog(Shell parent, ILabelProvider labelProvider, ITreeContentProvider contentProvider,
			String errorMessage) {
		super(parent, labelProvider, contentProvider);
		this.errorMessage = errorMessage;
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		return super.createDialogArea(parent);
	}

	@Override
	protected void updateOKStatus() {
		Status status;
		if (getFirstResult() == null) {
			status = new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR, errorMessage, null);
		} else {
			File buildFolderPath = new File(((IResource) getFirstResult()).getLocation().toOSString());
			if (!buildFolderPath.isDirectory()) {
				status = new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR, errorMessage, null);
			} else {
				status = new Status(IStatus.OK, PlatformUI.PLUGIN_ID, IStatus.OK, "", //$NON-NLS-1$
						null);
			}
		}

		updateStatus(status);
	}

}
