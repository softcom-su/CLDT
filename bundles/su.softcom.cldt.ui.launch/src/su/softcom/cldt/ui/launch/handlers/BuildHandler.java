package su.softcom.cldt.ui.launch.handlers;

import org.eclipse.core.commands.AbstractHandler;
import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PlatformUI;

/** Abstract handler for build mode commands. */
public abstract class BuildHandler extends AbstractHandler {

	public static final String BUILD_MODE = "build"; //$NON-NLS-1$

	@Override
	public Object execute(final ExecutionEvent event) throws ExecutionException {
		IWorkbenchWindow window = PlatformUI.getWorkbench().getActiveWorkbenchWindow();
		ISelection selection = window.getSelectionService().getSelection();

		getLaunchShortcut().launch(selection, BUILD_MODE);
		return null;
	}

	/**
	 * Get {@link ILaunchShortcut} to build selected resource.
	 */
	protected abstract ILaunchShortcut getLaunchShortcut();

}
