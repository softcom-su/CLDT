package su.softcom.cldt.ui.dialogs;

import java.util.Arrays;
import java.util.List;

import org.eclipse.jface.dialogs.InputDialog;
import org.eclipse.swt.widgets.Shell;

public class ExternalLibrarySelectionDialog extends InputDialog {

	public ExternalLibrarySelectionDialog(Shell parent, String dialogTitle, String dialogMessage) {
		super(parent, dialogTitle, dialogMessage, "", null);
	}

	/**
	 * @return List of external library paths as Strings.
	 */
	public List<String> getResult() {
		String[] libs = this.getValue().split(":");
		List<String> result = Arrays.asList(this.getValue().split(":"));
		for (String lib : result) {
			if (lib.matches(".*[\n\r\t\0\f`?*<>|\";].*")) {
				result.remove(lib);
			}
		}
		return result;
	}

}
