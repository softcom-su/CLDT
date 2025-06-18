package su.softcom.cldt.ui.launch;

import java.util.Arrays;

import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationType;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.ui.DebugUITools;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.debug.ui.ILaunchShortcut2;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.part.FileEditorInput;

/** The common part of launch shortcuts. */
public abstract class AbstractLaunchShortcut implements ILaunchShortcut, ILaunchShortcut2 {

	/**
	 * Project name attribute.
	 */
	public static final String PROJECT = AbstractLaunchConfigurationDelegate.PROJECT;

	/**
	 * Building resource attribute.
	 */
	public static final String TO_BUILD = AbstractLaunchConfigurationDelegate.TO_BUILD;

	@Override
	public ILaunchConfiguration[] getLaunchConfigurations(ISelection selection) {
		return getLaunchConfigurations(getLaunchableResource(selection));
	}

	@Override
	public ILaunchConfiguration[] getLaunchConfigurations(IEditorPart editorpart) {
		return getLaunchConfigurations(getLaunchableResource(editorpart));
	}

	@Override
	public IResource getLaunchableResource(ISelection selection) {
		if (selection instanceof IStructuredSelection) {
			for (Object element : (IStructuredSelection) selection) {
				if (element instanceof IResource) {
					return (IResource) element;
				}
			}
		}

		return null;
	}

	@Override
	public IResource getLaunchableResource(IEditorPart editorpart) {
		IEditorInput input = editorpart.getEditorInput();
		if (input instanceof FileEditorInput) {
			return ((FileEditorInput) input).getFile();
		}

		return null;
	}

	@Override
	public void launch(ISelection selection, String mode) {
		launch(getLaunchableResource(selection), mode);
	}

	@Override
	public void launch(IEditorPart editor, String mode) {
		launch(getLaunchableResource(editor), mode);
	}

	/**
	 * Returns the launch configuration type id associated with current
	 * {@link ILaunchShortcut}.
	 * 
	 * @return the launch configuration type id
	 */
	protected abstract String getLaunchConfigurationType();

	/**
	 * Returns an {@code IResource} that maps to given resource for build purposes.
	 * 
	 * @param resource the current resource
	 * 
	 * @return an {@code IResource} that maps to the given resource for build
	 *         purposes
	 */
	protected IResource getLaunchableResource(IResource resource) {
		return resource.getProject();
	}

	/**
	 * Returns an array of {@code ILaunchConfigurations} that apply to the specified
	 * resource, an empty collection if one could be created but does not exist, or
	 * {@code null} if default resource mappings should be used to derive associated
	 * configurations.
	 * 
	 * @param resource the current resource
	 * 
	 * @return an array of existing {@code ILaunchConfigurations} that could be used
	 *         to launch the given resource, an empty array if one could be created
	 *         but does not exist, or {@code null} if default resource mappings
	 *         should be used to derive associated configurations
	 */
	private ILaunchConfiguration[] getLaunchConfigurations(IResource resource) {
		if (resource == null) {
			return new ILaunchConfiguration[0];
		}
		try {
			return Arrays
					.stream(DebugPlugin.getDefault().getLaunchManager()
							.getLaunchConfigurations(DebugPlugin.getDefault().getLaunchManager()
									.getLaunchConfigurationType(getLaunchConfigurationType())))
					.filter(configuration -> {
						try {
							if (resource.getProject() == null) {
								return false;
							}
							return configuration.getAttribute(PROJECT, "") //$NON-NLS-1$
									.equals(resource.getProject().getName());
						} catch (CoreException e) {
							// could not found PROJECT attribute, ignore
							return false;
						}
					}).peek(configuration -> {
						try {
							ILaunchConfigurationWorkingCopy copy = configuration.getWorkingCopy();
							copy.setAttribute(TO_BUILD, getLaunchableResource(resource).getLocationURI().toString());
							copy.doSave();
						} catch (Exception e) {
							// could not found TO_BUILD attribute, ignore
						}
					}).toArray(ILaunchConfiguration[]::new);
		} catch (CoreException e) {
			// could not load configurations, ignore
			return new ILaunchConfiguration[0];
		}
	}

	/**
	 * Launches the given resource in the specified mode.
	 * 
	 * @param resource the resource to launch
	 * @param mode     the mode in which to launch
	 */
	private void launch(IResource resource, String mode) {
		if (resource == null) {
			return;
		}
		try {
			IResource launchableResource = getLaunchableResource(resource);
			ILaunchConfiguration[] configurations = getLaunchConfigurations(resource);
			if (configurations.length == 0) {
				// no configuration found, create new one
				ILaunchConfigurationType type = DebugPlugin.getDefault().getLaunchManager()
						.getLaunchConfigurationType(getLaunchConfigurationType());
				ILaunchConfigurationWorkingCopy configuration = type.newInstance(null, resource.getName());
				configuration.setAttribute(PROJECT, resource.getProject().getName());
				configuration.setAttribute(TO_BUILD, launchableResource.getLocationURI().toString());

				// save and return new configuration
				configuration.doSave();
				configurations = new ILaunchConfiguration[] { configuration };
			}

			// launch
			DebugUITools.launch(configurations[0], mode);
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.toString());
		}
	}

}
