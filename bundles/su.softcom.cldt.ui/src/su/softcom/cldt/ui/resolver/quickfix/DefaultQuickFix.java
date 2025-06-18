package su.softcom.cldt.ui.resolver.quickfix;

import org.eclipse.core.resources.IMarker;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.IMarkerResolution;

public class DefaultQuickFix implements IMarkerResolution {

	@Override
	public String getLabel() {
		return "Стандартное быстрое исправление";
	}

	@Override
	public void run(IMarker marker) {
		MessageDialog.openInformation(new Shell(), getLabel(), "Not implemented yet");
	}

}
