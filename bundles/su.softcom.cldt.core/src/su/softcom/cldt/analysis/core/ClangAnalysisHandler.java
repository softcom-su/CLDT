package su.softcom.cldt.analysis.core;

import java.util.List;

import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.handlers.HandlerUtil;

import su.softcom.cldt.core.operations.CLangStaticAnalysisTool;

/**
 * Launch clang-check static analysis handler. Found {@link IResource} from
 * current selection and try to launch {@link CLangStaticAnalysisTool} for it.
 */
public class ClangAnalysisHandler extends AnalysisHandler {

	@Override
	public Object execute(ExecutionEvent event) throws ExecutionException {

		ISelection selection = HandlerUtil.getCurrentSelection(event);
		if (selection instanceof IStructuredSelection sSelection) {
			Object selectionObject = sSelection.getFirstElement();
			if (selectionObject instanceof IResource selectionResource) {
				CLangStaticAnalysisTool cco = new CLangStaticAnalysisTool();
				// FIXME needs correct progress monitor
				cco.execute(List.of(selectionResource), new NullProgressMonitor());
			} else if (selectionObject instanceof IAdaptable adaptable) {
				IResource selectionResource = adaptable.getAdapter(IResource.class);
				if (selectionResource != null) {
					CLangStaticAnalysisTool cco = new CLangStaticAnalysisTool();
					cco.execute(List.of(selectionResource), new NullProgressMonitor());
				}
			}
		}
		return new Object();
	}

}
