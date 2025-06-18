package su.softcom.cldt.internal.ui.wizards;

import java.util.Arrays;
import java.util.Optional;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.IPath;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;

/**
 * New header file wizard page.
 * 
 * @author Vladislav Ershov (ershov_vladislav@softcom.su) - creator
 */
public class NewHeaderFileWizardPage extends AbstractNewFileWizardPage {

	/**
	 * Constructs new header file wizard page with the given project.
	 * 
	 * @param title      the wizard page title
	 * @param project    the given project
	 * @param folderPath path the given folder path
	 */
	public NewHeaderFileWizardPage(String title, Optional<IProject> project, IPath folderPath) {
		super(title, project, folderPath, Arrays.asList(new String[] { "h", "hh", "hpp", "hxx", "h++" }), //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$
				Messages.NewHeaderFileWizardPage_PossibleFileExtensions);
	}

	@Override
	public void createControl(Composite parent) {
		initializeDialogUnits(parent);

		Composite composite = new Composite(parent, SWT.NONE);
		composite.setLayout(new GridLayout());
		composite.setLayoutData(new GridData(GridData.FILL_BOTH));
		composite.setFont(parent.getFont());

		createCompositeProjectName(composite);
		createCompositeFolderPath(composite);
		createCompositeFileName(composite);

		composite.layout();
		setControl(composite);
	}

}
