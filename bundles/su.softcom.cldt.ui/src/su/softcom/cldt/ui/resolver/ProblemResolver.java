package su.softcom.cldt.ui.resolver;

import java.util.List;
import java.util.Map;

import org.eclipse.core.resources.IMarker;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.ui.IMarkerResolution;
import org.eclipse.ui.IMarkerResolutionGenerator2;

import su.softcom.cldt.core.operations.IProblemType;
import su.softcom.cldt.ui.resolver.quickfix.DefaultQuickFix;
import su.softcom.cldt.ui.resolver.quickfix.ExpectedSemicolonQuickFix;
import su.softcom.cldt.ui.resolver.quickfix.MainReturnValueQuickFix;

public class ProblemResolver implements IMarkerResolutionGenerator2 {

	Map<IMarker, List<String>> keyWords;

	@Override
	public IMarkerResolution[] getResolutions(IMarker marker) {
		if (marker == null) {
			return new IMarkerResolution[0];
		}

		int markerErrorType = -1;
		try {
			markerErrorType = (int) marker.getAttribute("error_type");
		} catch (CoreException e) {
			e.printStackTrace();
		}

		switch (markerErrorType) {
		/*
		 * case IProblemType.ILLEGAL_ARGUMENTS: { return new IMarkerResolution[] { new
		 * IllegalArgumentsQuickFix("Illegal function arguments problem quick fix") }; }
		 */
		case IProblemType.MAIN_RETURN_VALUE:
			return new IMarkerResolution[] { new MainReturnValueQuickFix() };
		case IProblemType.EXPECTED_SEMICOLON:
			return new IMarkerResolution[] { new ExpectedSemicolonQuickFix() };
		default:
			return new IMarkerResolution[] { new DefaultQuickFix() };
		}
	}

	@Override
	public boolean hasResolutions(IMarker marker) {
		return marker != null;
	}
}
