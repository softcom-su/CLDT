package su.softcom.cldt.ui.launch.perspectives;

import org.eclipse.debug.ui.IDebugUIConstants;
import org.eclipse.ui.IFolderLayout;
import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;
import org.eclipse.ui.console.IConsoleConstants;

/** Default perspective factory. */
public class DefaultPerspectiveFactory implements IPerspectiveFactory {

	@Override
	public void createInitialLayout(IPageLayout layout) {
		IFolderLayout explorer = layout.createFolder("su.softcom.cldt.ui.launch.perspectives.default.explorer", //$NON-NLS-1$
				IPageLayout.LEFT, 0.25f, IPageLayout.ID_EDITOR_AREA);
		explorer.addView(IPageLayout.ID_PROJECT_EXPLORER);
		explorer.addPlaceholder(IPageLayout.ID_PROJECT_EXPLORER);
		explorer.addPlaceholder(IDebugUIConstants.ID_DEBUG_VIEW);

		IFolderLayout console = layout.createFolder("su.softcom.cldt.ui.launch.perspectives.default.console", //$NON-NLS-1$
				IPageLayout.BOTTOM, 0.75f, IPageLayout.ID_EDITOR_AREA);
		console.addView(IConsoleConstants.ID_CONSOLE_VIEW);
		console.addView(IPageLayout.ID_PROBLEM_VIEW);
		console.addPlaceholder(IConsoleConstants.ID_CONSOLE_VIEW);
		console.addPlaceholder(IPageLayout.ID_PROBLEM_VIEW);

		layout.addShowViewShortcut(IPageLayout.ID_PROJECT_EXPLORER);
		layout.addShowViewShortcut(IConsoleConstants.ID_CONSOLE_VIEW);
		layout.addShowViewShortcut(IPageLayout.ID_PROBLEM_VIEW);

		layout.addActionSet(IDebugUIConstants.LAUNCH_ACTION_SET);
	}

}
