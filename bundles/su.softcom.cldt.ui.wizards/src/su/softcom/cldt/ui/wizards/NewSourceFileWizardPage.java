package su.softcom.cldt.ui.wizards;

import java.util.Arrays;
import java.util.Optional;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.IPath;

/**
 * New source file wizard page.
 * 
 * @author ershov_vladislav@softcom.su
 */
@Deprecated
public class NewSourceFileWizardPage extends AbstractNewFileWizardPage {

	/**
	 * Constructs new source file wizard page with the given project.
	 * 
	 * @param title      the wizard page title
	 * @param project    the given project
	 * @param folderPath path the given folder path
	 */
	public NewSourceFileWizardPage(String title, Optional<IProject> project, IPath folderPath) {
		super(title, project, folderPath, Arrays.asList(new String[] { "c", "cc", "cpp", "cxx", "c++", "md" }), //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$
				Messages.NewSourceFileWizardPage_PossibleFileExtensions);
	}

}
