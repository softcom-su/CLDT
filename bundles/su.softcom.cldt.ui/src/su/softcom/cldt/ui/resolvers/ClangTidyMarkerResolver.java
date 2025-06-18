package su.softcom.cldt.ui.resolvers;

import org.eclipse.core.resources.IMarker;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.ui.IMarkerResolution;
import org.eclipse.ui.IMarkerResolutionGenerator;

import su.softcom.cldt.ui.resolvers.quickfixes.ClangTidyAutoQuickFix;
import su.softcom.cldt.ui.resolvers.quickfixes.ClangTidyChecks;
import su.softcom.cldt.ui.resolvers.quickfixes.ClangTidyRenameQuickFix;

/**
 * Creates resolutions for a clang-tidy generated marker.
 */
public class ClangTidyMarkerResolver implements IMarkerResolutionGenerator {
	@Override
	public IMarkerResolution[] getResolutions(IMarker marker) {
		if (!isClangTidyMarker(marker)) {
			return new IMarkerResolution[0];
		}

		String checkId = marker.getAttribute("checkId", ""); //$NON-NLS-1$//$NON-NLS-2$
		if (checkId == null || !ClangTidyChecks.supportsFix(checkId)) {
			return new IMarkerResolution[0];
		}

		if (ClangTidyChecks.supportsRenameFix(checkId)) {
			return new IMarkerResolution[] { new ClangTidyRenameQuickFix() };
		} else if (ClangTidyChecks.supportsAutoFix(checkId)) {
			return new IMarkerResolution[] { new ClangTidyAutoQuickFix(marker) };
		}

		return new IMarkerResolution[0];

	}

	private boolean isClangTidyMarker(IMarker marker) {
		try {
			return "su.softcom.cldt.core.marker.clangTidyMarker".equals(marker.getType()); //$NON-NLS-1$
		} catch (CoreException e) {
			return false;
		}
	}
}