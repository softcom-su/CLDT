package su.softcom.cldt.internal.debug.ui;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationType;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.ui.DebugUITools;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.part.FileEditorInput;

import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.debug.core.DebugConstants;

public class HostLaunchShortcut extends AbstractLaunchShortcut implements ILaunchShortcut {
	
	ILog LOG = Platform.getLog(getClass());

	@Override
	public void launch(ISelection selection, String mode) {
		if(selection instanceof IStructuredSelection strSelection) {
			Object element = strSelection.getFirstElement();
			LOG.info("Selected element %s".formatted(element.toString()));
			if(element instanceof IAdaptable adaptable) {
				IProject project = adaptable.getAdapter(IProject.class);
				if(project != null) {
					LOG.info("Computed project: %s".formatted(project.getName()));
					try {
						launchProject(project, mode);
					} catch (CoreException e) {
						e.printStackTrace();
					}
				} else if(element instanceof Target target) {
					try {
						launchTarget(target, mode);
					} catch (CoreException e) {
						e.printStackTrace();
					}
					
				}
			}
		}
		
	}

	private void launchTarget(Target target, String mode) throws CoreException {
		LOG.info("Selected target: %s".formatted(target.getName()));
		ILaunchManager launchManager = getLaunchManager();
		ILaunchConfigurationType launchConfType = launchManager.getLaunchConfigurationType(DebugConstants.HOST_LAUNCH_ID);
		ILaunchConfiguration[] launchConfigurations = launchManager.getLaunchConfigurations(launchConfType);
		ICMakeProject project = target.getProject();
		for(ILaunchConfiguration configuration : launchConfigurations) {
			String projectName = configuration.getAttribute(DebugConstants.PROJECT_NAME, "");
			String targetName = configuration.getAttribute(DebugConstants.TARGET_NAME, "");
			if(target.getName().equals(targetName) && project.getProject().equals(projectName)) {
				DebugUITools.launch(configuration, mode);
				return;
			}
		}
		ILaunchConfigurationWorkingCopy workingCopy = launchConfType.newInstance(null, launchManager.generateLaunchConfigurationName(project.getProject().getName() + "_%s".formatted(target.getName())));
		workingCopy.setAttribute(DebugConstants.PROJECT_NAME, project.getProject().getName());
		workingCopy.setAttribute(DebugConstants.TARGET_NAME, target.getName());
		DebugUITools.launch(workingCopy, mode);
	}

	@Override
	public void launch(IEditorPart editor, String mode) {
		IEditorInput editorInput = editor.getEditorInput();
		if(editorInput instanceof FileEditorInput fileInput) {
			try {
				launchProject(fileInput.getFile().getProject(), mode);
			} catch (CoreException e) {
				e.printStackTrace();
			}
		}
		
	}
	
	private void launchProject(IProject project, String mode) throws CoreException {
		
	}
}
