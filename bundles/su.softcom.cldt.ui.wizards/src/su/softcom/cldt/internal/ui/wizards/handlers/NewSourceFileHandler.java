package su.softcom.cldt.internal.ui.wizards.handlers;

import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.INewWizard;

import su.softcom.cldt.internal.ui.wizards.NewSourceFileWizard;

/**
 * Handler for new source file.
 * 
 * @author ershov_vladislav@softcom.su
 */
public final class NewSourceFileHandler extends AbstractNewFileHandler {

	@Override
	protected INewWizard getNewWizard() {
		return new NewSourceFileWizard();
	}

	@Override
	protected INewWizard getNewWizard(IStructuredSelection selection) {
		return new NewSourceFileWizard(selection);
	}

}
