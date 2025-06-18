package su.softcom.cldt.internal.ui.actions;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.StructuredViewer;
import org.eclipse.ui.navigator.CommonActionProvider;
import org.eclipse.ui.navigator.ICommonActionExtensionSite;
import org.eclipse.ui.navigator.ICommonViewerWorkbenchSite;

import su.softcom.cldt.core.CMakeUpdateNature;
import su.softcom.cldt.core.cmake.Target;

public class TargetActionProvider extends CommonActionProvider {

	private TargetNewAction newTargetCreationAction;
	private TargetDeleteAction deleteTargetAction;
	private ICommonViewerWorkbenchSite viewSite;
	private StructuredViewer viewer;

	@Override
	public void init(ICommonActionExtensionSite commonSite) {
		if (commonSite.getViewSite() instanceof ICommonViewerWorkbenchSite site) {
			this.viewSite = site;
			this.viewer = commonSite.getStructuredViewer();
			initActions();
		}
	}

	private void initActions() {
		newTargetCreationAction = new TargetNewAction(() -> viewSite.getShell());
		deleteTargetAction = new TargetDeleteAction(() -> viewSite.getShell(), viewer);
	}

	@Override
	public void fillContextMenu(IMenuManager menu) {
		IStructuredSelection selection = (IStructuredSelection) getContext().getSelection();
		newTargetCreationAction.selectionChanged(selection);
		deleteTargetAction.selectionChanged(selection);
		MenuManager subMenu = new MenuManager("Создать");
		subMenu.add(newTargetCreationAction);
		menu.add(subMenu);
		try {
			if (selection.getFirstElement() instanceof Target tgt
					&& tgt.getProject().getProject().hasNature(CMakeUpdateNature.ID)) {
				menu.add(deleteTargetAction);
			}
		} catch (CoreException e) {
			e.printStackTrace();
		}
	}

}
