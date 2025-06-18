package su.softcom.cldt.internal.ui.actions;

import org.eclipse.jface.action.IMenuManager;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.IActionBars;
import org.eclipse.ui.actions.ActionContext;
import org.eclipse.ui.navigator.CommonActionProvider;
import org.eclipse.ui.navigator.ICommonActionExtensionSite;
import org.eclipse.ui.navigator.ICommonViewerWorkbenchSite;

public class CppSourceActionProvider extends CommonActionProvider {

	private Shell shell;
	private CppSourceActionGroup cppGroup;

	private ICommonViewerWorkbenchSite viewSite;

	@Override
	public void init(ICommonActionExtensionSite aConfig) {
		if (aConfig.getViewSite() instanceof ICommonViewerWorkbenchSite site) {
			viewSite = site;
			shell = viewSite.getShell();
			cppGroup = new CppSourceActionGroup(shell, viewSite);
		}
	}

	@Override
	public void dispose() {
		cppGroup.dispose();
	}

	@Override
	public void fillActionBars(IActionBars actionBars) {
		cppGroup.fillActionBars(actionBars);
	}

	@Override
	public void fillContextMenu(IMenuManager menu) {
		cppGroup.fillContextMenu(menu);
	}

	@Override
	public void setContext(ActionContext context) {
		cppGroup.setContext(context);
	}

	@Override
	public void updateActionBars() {
		cppGroup.updateActionBars();
	}

}
