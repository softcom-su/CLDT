package su.softcom.cldt.internal.ui.wizards.handlers;

import org.eclipse.core.commands.AbstractHandler;
import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.WizardDialog;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.INewWizard;
import org.eclipse.ui.handlers.HandlerUtil;

/**
 * Abstract implementation of a handler for new file.
 * 
 * @author ershov_vladislav@softcom.su
 */
public abstract class AbstractNewFileHandler extends AbstractHandler {

	@Override
	public final Object execute(ExecutionEvent event) throws ExecutionException {
		ISelection selection = HandlerUtil.getCurrentSelection(event);
		Shell parentShell = HandlerUtil.getActiveShell(event);

		INewWizard testCreationWizard;
		if (selection instanceof IStructuredSelection) {
			testCreationWizard = getNewWizard((IStructuredSelection) selection);
		} else {
			return null;
		}

		WizardDialog dialog = new WizardDialog(parentShell, testCreationWizard);
		dialog.create();
		dialog.open();

		return null;
	}

	/**
	 * Returns a new wizard with empty input fields.
	 * 
	 * @return a new wizard
	 */
	protected abstract INewWizard getNewWizard();

	/**
	 * Returns a new wizard from the given selection.
	 * 
	 * @param selection the selection wizard was called from
	 * 
	 * @return a new wizard
	 */
	protected abstract INewWizard getNewWizard(IStructuredSelection selection);

}
