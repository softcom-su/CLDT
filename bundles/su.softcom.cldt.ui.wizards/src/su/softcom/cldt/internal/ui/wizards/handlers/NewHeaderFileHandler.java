package su.softcom.cldt.internal.ui.wizards.handlers;

import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.INewWizard;

import su.softcom.cldt.internal.ui.wizards.NewHeaderFileWizard;

/**
 * Handler for new header file.
 * 
 * @author ershov_vladislav@softcom.su
 */
public final class NewHeaderFileHandler extends AbstractNewFileHandler {

	@Override
	protected INewWizard getNewWizard() {
		return new NewHeaderFileWizard();
	}

	@Override
	protected INewWizard getNewWizard(IStructuredSelection selection) {
		return new NewHeaderFileWizard(selection);
	}

}
