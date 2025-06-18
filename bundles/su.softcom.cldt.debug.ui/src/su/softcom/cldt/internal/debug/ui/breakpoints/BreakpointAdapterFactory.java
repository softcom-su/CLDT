package su.softcom.cldt.internal.debug.ui.breakpoints;

import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.IAdapterFactory;
import org.eclipse.debug.ui.actions.IToggleBreakpointsTarget;
import org.eclipse.ui.texteditor.ITextEditor;

public class BreakpointAdapterFactory implements IAdapterFactory {


	@Override
	public <T> T getAdapter(Object adaptableObject, Class<T> adapterType) {
		if (adaptableObject instanceof ITextEditor) {
			if (IToggleBreakpointsTarget.class.equals(adapterType)) {
				ITextEditor editorPart = (ITextEditor) adaptableObject;
				IResource resource = (IResource) editorPart.getEditorInput().getAdapter(IResource.class);
				if (resource != null) {
					String extension = resource.getFileExtension();
					if (extension != null) { // TODO: Work on the extensions later
						return (T) new BreakpointTarget();
					}
				}
			}
		}
		return null;
	}

	@Override
	public Class<?>[] getAdapterList() {
		return new Class[] { IToggleBreakpointsTarget.class };
	}

}
