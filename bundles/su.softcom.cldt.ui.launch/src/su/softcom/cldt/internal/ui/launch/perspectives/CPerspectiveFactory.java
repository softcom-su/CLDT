package su.softcom.cldt.internal.ui.launch.perspectives;

import org.eclipse.debug.ui.IDebugUIConstants;
import org.eclipse.ui.IFolderLayout;
import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;
import org.eclipse.ui.console.IConsoleConstants;

/** C/C++ perspective factory. */
public class CPerspectiveFactory implements IPerspectiveFactory {

	public static final String ID = "su.softcom.cldt.ui.launch.perspectives.cperspective"; //$NON-NLS-1$
	public static final String ID_FOLDER_NAVIGATION = ID + ".folders.navigation"; //$NON-NLS-1$
	public static final String ID_FOLDER_OUTPUT = ID + ".folders.output"; //$NON-NLS-1$
	public static final String ID_FOLDER_PROBLEMS = ID + ".folders.problems"; //$NON-NLS-1$

	@Override
	public void createInitialLayout(IPageLayout layout) {
		IFolderLayout navigation = layout.createFolder(ID_FOLDER_NAVIGATION, IPageLayout.LEFT, 0.25f,
				IPageLayout.ID_EDITOR_AREA);
		navigation.addView(IPageLayout.ID_PROJECT_EXPLORER); // FIXME Replace to custom explorer
		navigation.addPlaceholder(IPageLayout.ID_PROJECT_EXPLORER);
		navigation.addPlaceholder(IDebugUIConstants.ID_DEBUG_VIEW);

		IFolderLayout problems = layout.createFolder(ID_FOLDER_PROBLEMS, IPageLayout.BOTTOM, 0.75f,
				IPageLayout.ID_EDITOR_AREA);
		problems.addView(IPageLayout.ID_PROBLEM_VIEW);
		problems.addView(IConsoleConstants.ID_CONSOLE_VIEW);
		problems.addPlaceholder(IPageLayout.ID_PROBLEM_VIEW);
		problems.addPlaceholder(IConsoleConstants.ID_CONSOLE_VIEW);

		IFolderLayout output = layout.createFolder(ID_FOLDER_OUTPUT, IPageLayout.BOTTOM, 0.5f, ID_FOLDER_NAVIGATION);
		output.addView(IPageLayout.ID_OUTLINE);
		output.addPlaceholder(IPageLayout.ID_OUTLINE);

		layout.addShowViewShortcut(IPageLayout.ID_PROJECT_EXPLORER);
		layout.addShowViewShortcut(IPageLayout.ID_PROBLEM_VIEW);
		layout.addShowViewShortcut(IConsoleConstants.ID_CONSOLE_VIEW);
		layout.addShowViewShortcut(IPageLayout.ID_OUTLINE);

		layout.addActionSet(IDebugUIConstants.LAUNCH_ACTION_SET);
	}

}
