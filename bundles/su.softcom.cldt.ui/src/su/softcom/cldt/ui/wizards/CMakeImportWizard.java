package su.softcom.cldt.ui.wizards;

import java.lang.reflect.InvocationTargetException;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.ui.IImportWizard;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.dialogs.IOverwriteQuery;
import org.eclipse.ui.wizards.datatransfer.FileSystemStructureProvider;
import org.eclipse.ui.wizards.datatransfer.ImportOperation;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.CmakeProjectNature;
import su.softcom.cldt.internal.core.CMakeProject;
import su.softcom.cldt.internal.ui.wizards.CMakeImportPage;

/**
 * Визард импорта сторонних CMake проектов
 */
public class CMakeImportWizard extends Wizard implements IImportWizard {

	final CMakeImportPage page;

	public static final String ID = "su.softcom.cldt.ui.wizard.cmakeImportWizard"; //$NON-NLS-1$

	public CMakeImportWizard() {
		page = new CMakeImportPage();
		addPage(page);
	}

	@Override
	public void init(IWorkbench workbench, IStructuredSelection selection) {
		setWindowTitle("Импорт прокета CMake");
	}

	@Override
	public boolean performFinish() {
		IProject targetProject = page.getProject();
		if (!targetProject.exists()) {
			try {
				getContainer().run(false, true, monitor -> {
					try {
						targetProject.create(monitor);
						targetProject.open(monitor);
					} catch (CoreException exc) {
						exc.printStackTrace();
					}
				});
			} catch (InvocationTargetException | InterruptedException e) {
				e.printStackTrace();
			}
		}
		ImportOperation operation = new ImportOperation(page.getProject().getFullPath().addTrailingSeparator(),
				page.getRoot(), FileSystemStructureProvider.INSTANCE, path -> IOverwriteQuery.NO);
		operation.setCreateContainerStructure(false);
		try {
			IProjectDescription desc = targetProject.getDescription();
			desc.setNatureIds(new String[] { CmakeProjectNature.ID });
			targetProject.setDescription(desc, new NullProgressMonitor());
			CMakeProject cmakeProject = (CMakeProject) CMakeCorePlugin.eINSTANCE.getProject(targetProject);
			cmakeProject.setBuildFolder(page.getBuildFolderName());
		} catch (CoreException e) {
			e.printStackTrace();
		}

		try {
			getContainer().run(false, true, operation);
		} catch (InvocationTargetException | InterruptedException e) {
			e.printStackTrace();
		}
		return true;
	}

}
