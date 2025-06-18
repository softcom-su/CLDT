package su.softcom.cldt.internal.ui.wizards;

import java.util.Optional;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.IPath;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.INewWizard;

/**
 * Implementation of a wizard for creating a new source file.
 * 
 * @author Vladislav Ershov (ershov_vladislav@softcom.su) - creator
 */
public class NewSourceFileWizard extends AbstractNewFileWizard implements INewWizard {

	/** Constructor <code>NewSourceFileWizard</code> with empty input fields. */
	public NewSourceFileWizard() {
		super(Messages.NewSourceFileWizard_NewSourceFile);
	}

	/**
	 * Constructor <code>NewSourceFileWizard</code> from the given selection.
	 * 
	 * @param selection the selection wizard was called from
	 */
	public NewSourceFileWizard(IStructuredSelection selection) {
		super(Messages.NewSourceFileWizard_NewSourceFile, selection);
	}

	@Override
	protected NewSourceFileWizardPage createPage(Optional<IProject> initialProject, IPath initialFolderPath) {
		return new NewSourceFileWizardPage(Messages.NewSourceFileWizard_NewSourceFile, initialProject,
				initialFolderPath);
	}

}
