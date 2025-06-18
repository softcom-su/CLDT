package su.softcom.cldt.ui.launch;

import java.net.URI;
import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationType;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.core.model.ILaunchConfigurationDelegate;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.debug.ui.ILaunchShortcut2;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.part.FileEditorInput;

import su.softcom.cldt.ui.launch.tabs.MainTab;

/** The common part of launch delegates. */
public abstract class AbstractLaunchDelegate
		implements ILaunchShortcut, ILaunchShortcut2, ILaunchConfigurationDelegate {

	/**
	 * @see ILaunchShortcut
	 */
	@Override
	public final void launch(final IEditorPart editor, final String mode) {
		launch(getLaunchableResource(editor), mode);
	}

	/**
	 * @see ILaunchShortcut
	 */
	@Override
	public final void launch(final ISelection selection, final String mode) {
		launch(getLaunchableResource(selection), mode);
	}

	/**
	 * @see ILaunchShortcut2
	 */
	@Override
	public final IResource getLaunchableResource(final IEditorPart editor) {
		final IEditorInput input = editor.getEditorInput();
		if (input instanceof FileEditorInput) {
			return ((FileEditorInput) input).getFile();
		}

		return null;
	}

	/**
	 * @see ILaunchShortcut2
	 */
	@Override
	public final IResource getLaunchableResource(final ISelection selection) {
		if (selection instanceof IStructuredSelection) {
			for (final Object element : ((IStructuredSelection) selection).toArray()) {
				if (element instanceof IResource) {
					return (IResource) element;
				}
			}
		}

		return null;
	}

	/**
	 * @see ILaunchShortcut2
	 */
	@Override
	public final ILaunchConfiguration[] getLaunchConfigurations(final IEditorPart editor) {
		ILaunchConfiguration[] configurations = getLaunchConfigurations(getLaunchableResource(editor));
		return configurations == null ? new ILaunchConfiguration[0] : configurations;
	}

	/**
	 * @see ILaunchShortcut2
	 */
	@Override
	public final ILaunchConfiguration[] getLaunchConfigurations(final ISelection selection) {
		ILaunchConfiguration[] configurations = getLaunchConfigurations(getLaunchableResource(selection));
		return configurations == null ? new ILaunchConfiguration[0] : configurations;
	}

	/**
	 * @see ILaunchConfigurationDelegate
	 */
	@Override
	public void launch(final ILaunchConfiguration configuration, final String mode, final ILaunch launch,
			final IProgressMonitor monitor) throws CoreException {
		Display.getDefault().asyncExec(new Runnable() {
			@Override
			public void run() {
				PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage().saveAllEditors(true);
				IResource resource = getLaunchableResource(configuration);

				if (resource != null) {
					// we have a valid script, let's feed it to the script engine
					launch(resource, configuration, mode, launch, monitor);
				}
			}
		});
	}

	/**
	 * Returns the launch configuration type extension associated with current
	 * {@code LaunchDelegate}.
	 * 
	 * @return the launch configuration type extension
	 */
	protected abstract ILaunchConfigurationType getLaunchConfigurationType();

	/**
	 * Execute script code from an {@link IResource}.
	 * 
	 * @param resource      the resource to launch
	 * @param configuration the configuration to launch
	 * @param mode          the mode in which to launch
	 * @param launch        the launch object to contribute processes and debug
	 *                      targets to
	 * @param monitor       progress monitor
	 */
	protected abstract void launch(final IResource resource, final ILaunchConfiguration configuration,
			final String mode, final ILaunch launch, final IProgressMonitor monitor);

	/**
	 * Get launchable resource by selected.
	 * 
	 * @param resource selected {@code IResource}
	 * 
	 * @return the resource to launch
	 */
	protected IResource getLaunchableResource(final IResource resource) {
		return resource.getProject();
	}

	/**
	 * Retrieve launchable resource from an {@link ILaunchConfiguration}.
	 * 
	 * @param configuration the configuration to use
	 * 
	 * @return an {@code IResource}, or {@code null} if none
	 */
	private static IResource getLaunchableResource(final ILaunchConfiguration configuration) {
		IProject project;

		try {
			String projectName = configuration.getAttribute(MainTab.PROJECT, ""); //$NON-NLS-1$
			project = ResourcesPlugin.getWorkspace().getRoot().getProject(projectName);

			if (!project.exists()) {
				return null;
			}
		} catch (CoreException e) {
			return null;
		}

		try {
			URI launchableResource = new URI(configuration.getAttribute(MainTab.RESOURCE, "")); //$NON-NLS-1$
			IContainer[] containers = ResourcesPlugin.getWorkspace().getRoot()
					.findContainersForLocationURI(launchableResource);

			if (containers.length > 0) {
				return containers[0].exists() ? containers[0]
						: ResourcesPlugin.getWorkspace().getRoot().findFilesForLocationURI(launchableResource)[0];
			}
		} catch (Exception e) {
			// could not found RESOURCE attribute, continue
		}

		try {
			ILaunchConfigurationWorkingCopy copy = configuration.getWorkingCopy();
			copy.setAttribute(MainTab.RESOURCE, project.getLocationURI().toString());
			copy.doSave();
		} catch (CoreException e) {
			e.printStackTrace();
		}

		return project;
	}

	/**
	 * Get all launch configurations that target a dedicated resource.
	 * 
	 * @param resource the resource to launch
	 * 
	 * @return {@link ILaunchConfiguration}'s using resource
	 */
	private ILaunchConfiguration[] getLaunchConfigurations(final IResource resource) {
		if (resource == null) {
			return new ILaunchConfiguration[0];
		}
		List<ILaunchConfiguration> configurations = new ArrayList<ILaunchConfiguration>();
		ILaunchManager manager = DebugPlugin.getDefault().getLaunchManager();
		ILaunchConfigurationType type = getLaunchConfigurationType();

		// try to find existing configurations using the same resource
		try {
			String projectName = resource.getProject().getName();

			for (ILaunchConfiguration configuration : manager.getLaunchConfigurations(type)) {
				try {
					if (configuration.getAttribute(MainTab.PROJECT, "").equals(projectName)) { //$NON-NLS-1$
						configurations.add(configuration);
					}
				} catch (CoreException e) {
					// could not found PROJECT attribute, ignore
				}
			}
		} catch (CoreException e) {
			// could not load configurations, ignore
		}

		configurations.forEach(configuration -> {
			String launchableResource = getLaunchableResource(resource).getLocationURI().toString();

			try {
				ILaunchConfigurationWorkingCopy copy = configuration.getWorkingCopy();
				copy.setAttribute(MainTab.RESOURCE, launchableResource);
				copy.doSave();
			} catch (CoreException e) {
				// could not found RESOURCE attribute, ignore
			}
		});

		return configurations.toArray(new ILaunchConfiguration[configurations.size()]);
	}

	/**
	 * Launch a resource. Try to launch using a launch configuration. Used for
	 * contextual launches.
	 * 
	 * @param resource the resource to launch
	 * @param mode     the mode in which to launch
	 */
	private void launch(final IResource resource, final String mode) {
		if (resource != null) {
			try {
				ILaunchConfiguration[] configurations = getLaunchConfigurations(resource);
				if (configurations.length == 0) {
					// no configuration found, create new one
					ILaunchConfigurationType type = getLaunchConfigurationType();
					ILaunchConfigurationWorkingCopy configuration = type.newInstance(null, resource.getName());
					configuration.setAttribute(MainTab.PROJECT, resource.getProject().getName());
					configuration.setAttribute(MainTab.RESOURCE,
							getLaunchableResource(resource).getLocationURI().toString());

					// save and return new configuration
					configuration.doSave();
					configurations = new ILaunchConfiguration[] { configuration };
				}

				// launch
				configurations[0].launch(mode, new NullProgressMonitor());

			} catch (CoreException e) {
				PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage().saveAllEditors(true);

				// could not create launch configuration, run file directly
				launch(getLaunchableResource(resource), null, mode, null, new NullProgressMonitor());
			}
		}
	}

}
