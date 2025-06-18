package su.softcom.cldt.ui.resolvers;

import org.eclipse.core.resources.IMarker;
import org.eclipse.ui.IMarkerResolution;
import org.eclipse.ui.IMarkerResolutionGenerator;

import su.softcom.cldt.ui.resolvers.quickfixes.UninitializedPrimitiveVariableQuickFix;

public class UninitializedMarkerResolver implements IMarkerResolutionGenerator {

	@Override
	public IMarkerResolution[] getResolutions(IMarker marker) {

		if (marker == null) {
			return new IMarkerResolution[0];
		}

		return new IMarkerResolution[] { new UninitializedPrimitiveVariableQuickFix() };
	}

}
