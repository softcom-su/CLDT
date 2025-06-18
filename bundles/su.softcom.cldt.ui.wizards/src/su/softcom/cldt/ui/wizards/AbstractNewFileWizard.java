package su.softcom.cldt.ui.wizards;

import java.io.IOException;
import java.io.InputStream;
import java.util.Optional;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.Adapters;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.ui.INewWizard;
import org.eclipse.ui.IWorkbench;

/**
 * Abstract wizard for creating a new file.
 * 
 * @author ershov_vladislav@softcom.su
 */
@Deprecated
public abstract class AbstractNewFileWizard extends Wizard implements INewWizard {

	/** Target project. */
	private IProject project = null;
	/** Wizard page. */
	protected AbstractNewFileWizardPage page;

	/**
	 * Constructor <code>AbstractNewFileWizard</code> with empty input fields.
	 * 
	 * @param title the wizard page title
	 */
	protected AbstractNewFileWizard(String title) {
		page = createPage(Optional.empty(), Path.EMPTY);
		setWindowTitle(title);
	}

	/**
	 * Constructor <code>AbstractNewFileWizard</code> from the given selection.
	 * 
	 * @param title     the wizard page title
	 * @param selection the selection wizard was called from
	 */
	protected AbstractNewFileWizard(String title, IStructuredSelection selection) {
		Optional<IProject> initialProject;
		IPath initialFolderPath;

		Object selected = selection.getFirstElement();
		if (selected instanceof IProject selectedProject) {
			initialProject = Optional.of(selectedProject);
			initialFolderPath = selectedProject.getFullPath();
		} else if (selected instanceof IFolder folder) {
			initialProject = Optional.of(folder.getProject());
			initialFolderPath = folder.getFullPath();
		} else if (selected instanceof IFile file) {
			initialProject = Optional.of(file.getProject());
			initialFolderPath = file.getParent().getFullPath();
		} else {
			IProject adaptedProject = Adapters.adapt(selected, IProject.class);
			if (adaptedProject != null && adaptedProject.exists()) {
				initialProject = Optional.of(adaptedProject);
				initialFolderPath = adaptedProject.getFullPath();
			} else {
				initialProject = Optional.empty();
				initialFolderPath = Path.EMPTY;
			}
		}

		page = createPage(initialProject, initialFolderPath);
		setWindowTitle(title);
	}

	@Override
	public final void init(IWorkbench workbench, IStructuredSelection selection) {
		// No need to do anything
	}

	@Override
	public final void addPages() {
		addPage(page);
	}

	@Override
	public boolean performFinish() {
		IWorkspaceRoot root = ResourcesPlugin.getWorkspace().getRoot();
		IPath folderPath = page.getFolderPath();
		project = page.getProject();

		IFile sourceFile;
		if (page.getProject().getFullPath().equals(folderPath)) {
			sourceFile = root.getProject(page.getProject().getName()).getFile(page.getFileName());
		} else {
			sourceFile = root.getFolder(page.getFolderPath()).getFile(page.getFileName());
		}

		InputStream stream = InputStream.nullInputStream();
		Job creation = Job.create(Messages.AbstractNewFileWizard_FileCreation, (IProgressMonitor monitor) -> {
			sourceFile.create(stream, IResource.FORCE, monitor);
			project.refreshLocal(IResource.DEPTH_INFINITE, null);

		});

		try {
			creation.schedule();
			creation.join();
		} catch (InterruptedException e) {
			Platform.getLog(AbstractNewFileWizard.class).error(e.getMessage());
			return false;
		} finally {
			try {
				stream.close();
			} catch (IOException e) {
				Platform.getLog(AbstractNewFileWizard.class).error(e.getMessage());
			}
		}

		return true;
	}

	/**
	 * Creates this wizard's page with the given project and folder.
	 * 
	 * @param initialProject    the initial project
	 * @param initialfolderPath the initial folder path
	 * 
	 * @return this wizard's page
	 */
	protected abstract AbstractNewFileWizardPage createPage(Optional<IProject> initialProject, IPath initialfolderPath);

}
