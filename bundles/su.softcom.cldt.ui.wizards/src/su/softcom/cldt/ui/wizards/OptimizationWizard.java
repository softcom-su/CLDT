package su.softcom.cldt.ui.wizards;

import java.util.Optional;
import java.util.Vector;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.Adapters;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.ui.INewWizard;
import org.eclipse.ui.IWorkbench;

/**
 * @author ershov_vladislav@softcom.su
 * 
 *         Optimization wizard for select resources to optimization.
 */
@Deprecated
public class OptimizationWizard extends Wizard implements INewWizard {

	/** This wizard's page. */
	private final OptimizationWizardPage page;

	/** This wizard's imported files. */
	private Vector<IFile> optimizatedFiles;

	/**
	 * Constructs a new optimization wizard from the given selection.
	 * 
	 * @param title     the wizard page title
	 * @param selection the selection wizard was called from
	 */
	public OptimizationWizard(String title, IStructuredSelection selection) {
		Optional<IProject> initialProject;

		Object selected = selection.getFirstElement();
		if (selected instanceof IProject project) {
			initialProject = Optional.of(project);
		} else {
			IProject adaptedProject = Adapters.adapt(selected, IProject.class);
			if (adaptedProject != null && adaptedProject.exists()) {
				initialProject = Optional.of(adaptedProject);
			} else {
				initialProject = Optional.empty();
			}
		}

		page = createPage(initialProject, selection);
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
	public final boolean performFinish() {
		optimizatedFiles = page.getImportedFiles();
		return true;
	}

	/**
	 * Creates this wizard's page with the given program project and group
	 * 
	 * @param program   the initial program project
	 * @param groupPath the initial group path
	 * @return this wizard's page
	 */
	private OptimizationWizardPage createPage(Optional<IProject> program, IStructuredSelection selection) {
		return new OptimizationWizardPage("Выберите файлы для оптимизации", selection);
	}

	/**
	 * Get files selected for optimization.
	 * 
	 * @return optimizatedFiles file for optimization files
	 */
	public Vector<IFile> getOptimizatedFiles() {
		return optimizatedFiles;
	}
}
