package su.softcom.cldt.common.ui;

import org.eclipse.ui.IFolderLayout;
import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;

/**
 * Class for optimization perspective factory
 *
 */
public class OptimizationPerspective implements IPerspectiveFactory {

	/** Perspective ID. */
	public static final String ID = "su.softcom.cldt.ui.perspective.optimization"; //$NON-NLS-1$
	/** Left placeholder for navigating through solutions. */
	public static final String ID_FOLDER_NAVIGATION = ID + "folders.navigation"; //$NON-NLS-1$
	/** Bottom placeholder for navigating through problems. */
	public static final String ID_FOLDER_PROBLEMS = ID + "folders.problems"; //$NON-NLS-1$

	@Override
	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();

		IFolderLayout navigation = layout.createFolder(ID_FOLDER_NAVIGATION, IPageLayout.LEFT, 0.2f, editorArea);
		if (isViewExist("su.softcom.cldt.meta.ui", "su.softcom.cldt.meta.ui.views.OptimizationExplorer")) {
			navigation.addView("su.softcom.cldt.ui.views.optimization");
		} else {
			navigation.addView(IPageLayout.ID_PROJECT_EXPLORER);
		}

		IFolderLayout problems = layout.createFolder(ID_FOLDER_PROBLEMS, IPageLayout.BOTTOM, 0.7f, editorArea);
		problems.addView(IPageLayout.ID_PROBLEM_VIEW);
		problems.addView("org.eclipse.ui.console.ConsoleView"); //$NON-NLS-1$
	}

	private boolean isViewExist(String packageName, String className) {
		try {
			Bundle[] bs = FrameworkUtil.getBundle(BuildPerspectiveFactory.class).getBundleContext().getBundles();
			for (Bundle b : bs) {
				if (b.getSymbolicName().equals(packageName)) {
					b.loadClass(className);
					return true;
				}
			}
		} catch (ClassNotFoundException | SecurityException e) {
			e.printStackTrace();
		}
		return false;
	}
}
