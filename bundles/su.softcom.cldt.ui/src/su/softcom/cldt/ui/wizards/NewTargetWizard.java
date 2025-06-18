package su.softcom.cldt.ui.wizards;

import org.eclipse.core.resources.IProject;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.ui.INewWizard;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.navigator.CommonNavigator;
import org.eclipse.ui.navigator.CommonViewer;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.internal.ui.CMakeTargetsFolder;
import su.softcom.cldt.internal.ui.wizards.NewTargetWizardPage;

public class NewTargetWizard extends Wizard implements INewWizard {

	protected NewTargetWizardPage page1;
	private IProject project;

	/**
	 * The default constructor.
	 */
	public NewTargetWizard() {
		setWindowTitle("Добавление цели сборки");
	}

	@Override
	public void addPages() {
		page1 = new NewTargetWizardPage("New target", project);
		page1.setTitle("Новая цель сборки");
		page1.setDescription("Введите параметры новой цели сборки");
		addPage(page1);
	}

	/**
	 * Initializes the project when trying to create a target using the context menu
	 * of targets or projects (if so, project selection is disabled in the wizard
	 * itself)
	 */
	@Override
	public void init(IWorkbench workbench, IStructuredSelection selection) {
		if (!selection.isEmpty()) {
			Object selectionElement = selection.getFirstElement();
			if (selectionElement instanceof CMakeTargetsFolder) {
				project = ((CMakeTargetsFolder) selection.getFirstElement()).getProject().getProject();
			} else if (selectionElement instanceof IProject) {
				project = (IProject) selection.getFirstElement();
			}
		}
	}

	@Override
	public boolean performFinish() {
		if (project == null) {
			project = page1.getProject();
		}
		Target newTarget = page1.getNewTarget();
		ICMakeProject cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
		cmakeProject.addTarget(newTarget);
		CommonNavigator navigator = (CommonNavigator) PlatformUI.getWorkbench().getActiveWorkbenchWindow()
				.getActivePage().findView("org.eclipse.ui.navigator.ProjectExplorer");
		CommonViewer viewer = navigator.getCommonViewer();
		viewer.getControl().getDisplay().asyncExec(() -> viewer.refresh(project));

		return true;
	}

}
