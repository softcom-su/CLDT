package su.softcom.cldt.analysis.core;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.handlers.HandlerUtil;

import su.softcom.cldt.core.operations.ClangTidyAnalysisTool;

/**
 * Launch clang-tidy analysis handler. Found {@link IResource} from current
 * selection and try to launch {@link ClangTidyAnalysisTool} for it.
 */
public class ClangTidyAnalysisHandler extends AnalysisHandler {

	@Override
	public Object execute(ExecutionEvent event) throws ExecutionException {
		ISelection selection = HandlerUtil.getCurrentSelection(event);
		if (selection instanceof IStructuredSelection sSelection) {
			List<IResource> resources = new ArrayList<>();

			for (Object element : sSelection.toArray()) {
				if (element instanceof IResource resource) {
					resources.add(resource);
				} else if (element instanceof IAdaptable adaptable) {
					IResource resource = adaptable.getAdapter(IResource.class);
					if (resource != null) {
						resources.add(resource);
					}
				}
			}

			if (!resources.isEmpty()) {
				ClangTidyAnalysisTool cta = new ClangTidyAnalysisTool();
				cta.execute(resources, new NullProgressMonitor());
			}
		}
		return new Object();
	}
}
