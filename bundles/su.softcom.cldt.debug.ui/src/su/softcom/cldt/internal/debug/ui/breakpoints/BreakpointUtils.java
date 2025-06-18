package su.softcom.cldt.internal.debug.ui.breakpoints;

import org.eclipse.core.resources.IResource;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.ui.IWorkbenchPart;
import org.eclipse.ui.texteditor.ITextEditor;

import su.softcom.cldt.debug.core.DebugConstants;
import su.softcom.cldt.debug.internal.core.DebugTarget;

public class BreakpointUtils {
	
	private BreakpointUtils() {

	}

	public static DebugTarget getDebugTarget() throws DebugException {
		// TODO: It's not quite clear how to differentiate between GDBClient, because the modelIdentifier will all be the same.
		IDebugTarget[] targets = DebugPlugin.getDefault().getLaunchManager().getDebugTargets();
			for (IDebugTarget target : targets) {
				if (target.getModelIdentifier().equals(DebugConstants.MODEL_IDENTIFIER) && target instanceof DebugTarget debugTarget) {
					return debugTarget;
				}
			}
			
		return null;
	}
	
	public static ITextEditor getEditor(IWorkbenchPart part) {
		if (part instanceof ITextEditor editorPart) {
			IResource resource = editorPart.getEditorInput().getAdapter(IResource.class);
			if (resource != null) {
				String extension = resource.getFileExtension();
				if (extension != null) //  && extension.equals("c") TODO: Work on the extensions later
					return editorPart;
			}
		}

		return null;
	}
	
}
