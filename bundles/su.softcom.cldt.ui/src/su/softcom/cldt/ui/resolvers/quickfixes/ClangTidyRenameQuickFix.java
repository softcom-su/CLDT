package su.softcom.cldt.ui.resolvers.quickfixes;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.core.resources.IMarker;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.jface.dialogs.InputDialog;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.PlatformUI;

import su.softcom.cldt.ui.dialogs.VariableRenameDialog;

/**
 * Resolves clang-tidy markers by running clang-refactor
 */
public class ClangTidyRenameQuickFix extends ClangRefactorAbstractQuickFix {
	private String label = "Изменить название переменной";

	@Override
	public String getLabel() {
		return label;
	}

	public ClangTidyRenameQuickFix() {
		super("local-rename", "Изменение названия переменной"); //$NON-NLS-1$ //$NON-NLS-2$
	}

	private String getVariableNameFromMarker(IMarker marker) {
		Pattern variableNamePattern = Pattern.compile("'([a-zA-Z_]\\w*)'"); //$NON-NLS-1$
		Matcher matcher;
		try {
			matcher = variableNamePattern.matcher((String) marker.getAttribute(IMarker.MESSAGE));
			if (matcher.find()) {
				return matcher.group(1);
			}

		} catch (CoreException e) {
			e.printStackTrace();
		}

		return null;
	}

	protected String getNewName(IMarker marker) {
		Shell shell = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getShell();
		String oldName = getVariableNameFromMarker(marker);

		if (oldName == null) {
			MessageDialog.openError(shell, "Ошибка", "Не удалось определить имя переменной");
			return null;
		}

		InputDialog dialog = new VariableRenameDialog(shell, "Изменить название переменной",
				"Введите новое имя для переменной " + oldName + ":", oldName);

		if (dialog.open() == Window.OK) {
			return dialog.getValue();
		}

		return null;
	}

	@Override
	protected List<String> extractParameters(IMarker marker) {
		List<String> args = new ArrayList<>();
		String sourceLocation = ""; //$NON-NLS-1$

		try {
			sourceLocation = (String) marker.getAttribute(IMarker.LOCATION);
		} catch (CoreException e) {
			e.printStackTrace();
		}

		args.add("--selection=" + sourceLocation); //$NON-NLS-1$

		String newName = getNewName(marker);

		if (newName == null || newName.trim().isEmpty()) {
			return Collections.emptyList();
		}

		args.add("--new-name=" + newName); //$NON-NLS-1$

		return args;
	}
}