package su.softcom.cldt.ui.resolvers.quickfixes;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.resources.IMarker;
import org.eclipse.core.runtime.CoreException;

public class UninitializedPrimitiveVariableQuickFix extends ClangRefactorAbstractQuickFix {

	public UninitializedPrimitiveVariableQuickFix() {
		super("init-primitives", "Инициализация переменных"); //$NON-NLS-1$ //$NON-NLS-2$
	}

	@Override
	protected List<String> extractParameters(IMarker marker) {
		List<String> args = new ArrayList<>();
		String SourceLocation = ""; //$NON-NLS-1$

		try {
			SourceLocation = (String) marker.getAttribute(IMarker.LOCATION);
		} catch (CoreException e) {
			e.printStackTrace();
		}

		args.add("--location=" + SourceLocation); //$NON-NLS-1$
		return args;
	}

}
