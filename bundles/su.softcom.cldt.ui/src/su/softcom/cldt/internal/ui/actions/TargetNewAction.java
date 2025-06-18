package su.softcom.cldt.internal.ui.actions;

import org.eclipse.core.runtime.Assert;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.window.IShellProvider;
import org.eclipse.jface.wizard.WizardDialog;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.actions.SelectionListenerAction;

import su.softcom.cldt.ui.wizards.NewTargetWizard;

public class TargetNewAction extends SelectionListenerAction {

	private static final String TARGET_NEW_ACTION_NAME = "Цель сборки";
	private IShellProvider shellProvider;

	/**
	 * Creates a new NewTarget action.
	 *
	 * @param provider the shell provider to use. Must not be <code>null</code>.
	 * @since 3.4
	 */
	public TargetNewAction(IShellProvider provider) {
		super(TARGET_NEW_ACTION_NAME);
		Assert.isNotNull(provider);
		this.shellProvider = provider;
	}

	@Override
	public void run() {
		IWorkbench workbench = PlatformUI.getWorkbench();
		IWorkbenchWindow window = workbench.getActiveWorkbenchWindow();
		NewTargetWizard wizard = new NewTargetWizard();
		wizard.init(workbench, (IStructuredSelection) window.getSelectionService().getSelection());
		WizardDialog dialog = new WizardDialog(window.getShell(), wizard);
		dialog.open();
	}

}
