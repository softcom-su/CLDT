package su.softcom.cldt.ui.dialogs;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.ElementTreeSelectionDialog;
import org.eclipse.ui.model.WorkbenchLabelProvider;

public class FolderSelectionDialog extends ElementTreeSelectionDialog {

	public record WorkspaceProjectRecord(IWorkspace workspace, IProject project) {
	};

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
	static class FolderContentProvider implements ITreeContentProvider {

		@Override
		public Object[] getElements(Object element) {
			if (element instanceof WorkspaceProjectRecord pair) {
				return new Object[] { pair.project };
			}
			if (element instanceof IProject project) {
				try {
					List<Object> tList = new ArrayList<Object>();
					tList.addAll(Arrays.asList(project.members()).stream().filter(e -> {
						return e instanceof IFolder;
					}).toList());
					return tList.toArray();
				} catch (CoreException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			return new Object[0];
		}

		@Override
		public Object[] getChildren(Object element) {
			if (element instanceof IContainer folder) {
				if (folder.exists()) {
					try {
						return Arrays.asList(folder.members()).stream().filter(e -> {
							return e instanceof IFolder;
						}).toList().toArray();
					} catch (CoreException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
			return new Object[0];
		}

		@Override
		public Object getParent(Object element) {
			if (element instanceof IResource res) {
				res.getParent();
			}
			return null;
		}

		@Override
		public boolean hasChildren(Object element) {
			return element instanceof IProject || getChildren(element).length > 0;
		}

	}

	public FolderSelectionDialog(Shell parent) {
		super(parent, new WorkbenchLabelProvider(), new FolderContentProvider());
	}

	@Override
	protected void updateOKStatus() {
		Status status;
		if (this.getResult().length == 0) {
			status = new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR, "Папка не выбрана", null);
			updateStatus(status);
			return;
		}
		for (Object o : this.getResult()) {
			if (!(o instanceof IFolder || o instanceof IProject)) {
				status = new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR, "Выбрана не папка", null);
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
