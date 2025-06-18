package su.softcom.cldt.ui.views;

import org.eclipse.core.commands.operations.IUndoContext;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.runtime.Assert;
import org.eclipse.ui.ide.undo.WorkspaceUndoUtil;
import org.eclipse.ui.views.markers.MarkerSupportView;

/**
 * The Build markers view is supplied by the IDE to show build markers.
 * 
 * @author ershov_vladislav@softcom.su
 */
public class BuildMarkersView extends MarkerSupportView {

	/** Create a new instance of the receiver. */
	public BuildMarkersView() {
		super("su.softcom.cldt.ui.buildMarkerGenerator"); //$NON-NLS-1$
	}

	@Override
	protected IUndoContext getUndoContext() {
		return WorkspaceUndoUtil.getProblemsUndoContext();
	}

	@Override
	protected String getDeleteOperationName(IMarker[] markers) {
		Assert.isLegal(markers.length > 0);
		return markers.length == 1 ? Messages.BuildMarkersView_DeleteMarker : Messages.BuildMarkersView_DeleteMarkers;
	}

}
