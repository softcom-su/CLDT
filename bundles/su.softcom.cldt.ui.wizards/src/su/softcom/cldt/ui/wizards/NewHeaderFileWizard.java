package su.softcom.cldt.ui.wizards;

import java.util.Optional;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.IPath;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.INewWizard;

/**
 * Implementation of a wizard for creating a new header file.
 * 
 * @author ershov_vladislav@softcom.su
 */
@Deprecated
public class NewHeaderFileWizard extends AbstractNewFileWizard implements INewWizard {

	/** Constructor <code>NewHeaderFileWizard</code> with empty input fields. */
	public NewHeaderFileWizard() {
		super(Messages.NewHeaderFileWizard_NewHeaderFile);
	}

	/**
	 * Constructor <code>NewHeaderFileWizard</code> from the given selection.
	 * 
	 * @param selection the selection wizard was called from
	 */
	public NewHeaderFileWizard(IStructuredSelection selection) {
		super(Messages.NewHeaderFileWizard_NewHeaderFile, selection);
	}

	@Override
	protected NewHeaderFileWizardPage createPage(Optional<IProject> initialProject, IPath initialFolderPath) {
		return new NewHeaderFileWizardPage(Messages.NewHeaderFileWizard_NewHeaderFile, initialProject,
				initialFolderPath);
	}

}
