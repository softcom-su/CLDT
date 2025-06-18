package su.softcom.cldt.internal.ui.preferences;

import java.io.File;
import java.nio.file.Paths;
import java.util.regex.Pattern;

import org.eclipse.jface.preference.StringFieldEditor;
import org.eclipse.swt.widgets.Composite;

/**
 * A field editor for a string type preference for select execution file.
 * 
 * @author ershov_vladislav@softcom.su
 */
public class ExecutionFileFieldEditor extends StringFieldEditor {

	/**
	 * Creates a execution file field editor with name, labelText, parent.
	 *
	 * @param name      the name of the preference this field editor works on
	 * @param labelText the label text of the field editor
	 * @param parent    the parent of the field editor's control
	 */
	protected ExecutionFileFieldEditor(String name, String labelText, Composite parent) {
		super(name, labelText, parent);
	}

	@Override
	protected boolean doCheckState() {
		if (getStringValue().equals("")) { //$NON-NLS-1$
			setErrorMessage(Messages.ExecutionFileFieldEditor_NotEnteredFileName);
			return false;
		} else if (Pattern.matches("\\.+", getStringValue()) || Pattern.matches(".*\\.", getStringValue())) { //$NON-NLS-1$ //$NON-NLS-2$
			setErrorMessage(Messages.ExecutionFileFieldEditor_UncorrectFileName);
			return false;
		}

		File executionFile = new File(getStringValue());
		try {
			Paths.get(executionFile.toURI());
			return true;
		} catch (IllegalArgumentException e) {
			setErrorMessage(Messages.ExecutionFileFieldEditor_UncorrectFileName);
			return false;
		}
	}

}
