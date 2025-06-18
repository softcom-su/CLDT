package su.softcom.cldt.ui.launch.handlers;

import org.eclipse.core.commands.AbstractHandler;
import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.Adapters;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PlatformUI;

/** Abstract handler for compiling commands. */
public abstract class CompileHandler extends AbstractHandler {

	@Override
	public Object execute(final ExecutionEvent event) throws ExecutionException {
		IWorkbenchWindow window = PlatformUI.getWorkbench().getActiveWorkbenchWindow();
		ISelection selection = window.getSelectionService().getSelection();

		if (selection instanceof IStructuredSelection) {
			for (final Object element : ((IStructuredSelection) selection).toArray()) {
				if (element instanceof IResource) {
					compile((IResource) element);
				} else {
					IProject project = Adapters.adapt(element, IProject.class);
					if (project != null && project.exists()) {
						compile(project);
					}
				}
			}
		}

		return null;
	}

	/**
	 * Compile selected resource.
	 * 
	 * @param resource the selected resource
	 * @throws ExecutionException if resource's project nature is unsupported
	 */
	protected abstract void compile(final IResource resource) throws ExecutionException;

}
