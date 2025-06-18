package su.softcom.cldt.internal.ui.actions;

import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.IActionBars;
import org.eclipse.ui.ISharedImages;
import org.eclipse.ui.IWorkbenchCommandConstants;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.actions.ActionGroup;
import org.eclipse.ui.actions.DeleteResourceAction;
import org.eclipse.ui.actions.MoveResourceAction;
import org.eclipse.ui.actions.OpenFileAction;
import org.eclipse.ui.actions.RenameResourceAction;
import org.eclipse.ui.navigator.ICommonActionConstants;
import org.eclipse.ui.navigator.ICommonMenuConstants;
import org.eclipse.ui.navigator.ICommonViewerWorkbenchSite;

public class CppSourceActionGroup extends ActionGroup {

	private OpenFileAction openAction;
	private DeleteResourceAction deleteAction;
	// TODO later
	private MoveResourceAction moveAction;
	private RenameResourceAction renameAction;

	private Shell shell;

	public CppSourceActionGroup(Shell shell, ICommonViewerWorkbenchSite viewSite) {
		this.shell = shell;
		openAction = new OpenFileAction(viewSite.getPage());
		makeActions();
	}

	@Override
	public void fillContextMenu(IMenuManager menu) {
		IStructuredSelection selection = (IStructuredSelection) getContext().getSelection();
		deleteAction.selectionChanged(selection);
//		moveAction.selectionChanged(selection);
//		renameAction.selectionChanged(selection);
		menu.appendToGroup(ICommonMenuConstants.GROUP_EDIT, deleteAction);
//		menu.appendToGroup(ICommonMenuConstants.GROUP_EDIT, moveAction);
//		menu.appendToGroup(ICommonMenuConstants.GROUP_EDIT, renameAction);
	}

	@Override
	public void fillActionBars(IActionBars actionBars) {
		if (getContext() == null) {
			return;
		}
		IStructuredSelection selection = (IStructuredSelection) getContext().getSelection();
		openAction.selectionChanged(selection);
		if (openAction.isEnabled()) {
			actionBars.setGlobalActionHandler(ICommonActionConstants.OPEN, openAction);
		}
	}

	protected void makeActions() {

		ISharedImages images = PlatformUI.getWorkbench().getSharedImages();
		deleteAction = new DeleteResourceAction(() -> shell);
		deleteAction.setDisabledImageDescriptor(images.getImageDescriptor(ISharedImages.IMG_TOOL_DELETE_DISABLED));
		deleteAction.setImageDescriptor(images.getImageDescriptor(ISharedImages.IMG_TOOL_DELETE));
		deleteAction.setActionDefinitionId(IWorkbenchCommandConstants.EDIT_DELETE);
//
//		moveAction = new MoveResourceAction(() -> shell);
//		moveAction.setDisabledImageDescriptor(images.getImageDescriptor(ISharedImages.IMG_TOOL_DELETE_DISABLED));
//		moveAction.setImageDescriptor(images.getImageDescriptor(ISharedImages.IMG_TOOL_DELETE));
//		moveAction.setActionDefinitionId(IWorkbenchCommandConstants.FILE_MOVE);
//
//		renameAction = new RenameResourceAction(() -> shell);
//		renameAction.setDisabledImageDescriptor(images.getImageDescriptor(ISharedImages.IMG_TOOL_DELETE_DISABLED));
//		renameAction.setImageDescriptor(images.getImageDescriptor(ISharedImages.IMG_TOOL_DELETE));
//		renameAction.setActionDefinitionId(IWorkbenchCommandConstants.FILE_RENAME);
	}

}
