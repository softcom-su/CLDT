package su.softcom.cldt.ui.launch;

import java.net.URI;
import java.util.Arrays;
import java.util.Map;
import java.util.stream.Stream;

import org.eclipse.core.resources.ICommand;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.OperationCanceledException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.core.model.ILaunchConfigurationDelegate;
import org.eclipse.debug.core.model.ILaunchConfigurationDelegate2;
import org.eclipse.debug.core.model.LaunchConfigurationDelegate;

/** The common part of launch configuration delegates. */
public abstract class AbstractLaunchConfigurationDelegate extends LaunchConfigurationDelegate
		implements ILaunchConfigurationDelegate, ILaunchConfigurationDelegate2 {

	/**
	 * Project name attribute.
	 */
	public static final String PROJECT = "Project name"; //$NON-NLS-1$

	/**
	 * Building resource attribute.
	 */
	public static final String TO_BUILD = "Resource to build"; //$NON-NLS-1$

	/**
	 * Launchable resource attribute.
	 */
	public static final String TO_LAUNCH = "Launchable resource"; //$NON-NLS-1$

	@Override
	protected final IProject[] getBuildOrder(ILaunchConfiguration configuration, String mode) throws CoreException {
		return getBuildableResource(configuration).equals(setLaunchableResource(configuration, mode)) ? null
				: new IProject[] { modifyProject(configuration, mode) };
	}

	@Override
	protected final IProject[] getProjectsForProblemSearch(ILaunchConfiguration configuration, String mode)
			throws CoreException {
		IResource launchableResource = setLaunchableResource(configuration, mode);
		if (launchableResource != null) {
			ILaunchConfigurationWorkingCopy copy = configuration.getWorkingCopy();
			copy.setAttribute(TO_LAUNCH, launchableResource.getLocationURI().toString());
			copy.doSave();
		}

		return getBuildableResource(configuration).equals(launchableResource) ? null
				: new IProject[] { getProject(configuration) };
	}

	/**
	 * Returns a table of the arguments that maps to given launch configuration, or
	 * {@code null} if there are no arguments. The argument names and values are
	 * both strings.
	 * 
	 * @param configuration the current launch configuration
	 * 
	 * @return a table of command arguments (key type : {@code String} value type :
	 *         {@code String}), or {@code null}
	 */
	public final Map<String, String> getArguments(ILaunchConfiguration configuration) {
		try {
			IProjectDescription description = getProject(configuration).getDescription();
			return Arrays.stream(description.getBuildSpec())
					.filter(buildCommand -> buildCommand.getBuilderName().equals(getBuilderName()))
					.map(ICommand::getArguments).findFirst().orElse(null);
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.toString());
			return null;
		}
	}

	/**
	 * Returns an {@code IProject} that maps to given launch configuration for
	 * launch purposes.
	 * 
	 * @param configuration the current launch configuration
	 * 
	 * @return an {@code IProject} that maps to the given launch configuration for
	 *         launch purposes
	 */
	public static IProject getProject(ILaunchConfiguration configuration) {
		try {
			IProject project = ResourcesPlugin.getWorkspace().getRoot()
					.getProject(configuration.getAttribute(PROJECT, "")); //$NON-NLS-1$

			if (project.exists()) {
				return project;
			}
		} catch (CoreException e) {
			// could not found PROJECT attribute, cancel
		}

		throw new OperationCanceledException();
	}

	/**
	 * Returns an {@code IResource} that maps to given launch configuration for
	 * build purposes.
	 * 
	 * @param configuration the current launch configuration
	 * 
	 * @return an {@code IResource} that maps to the given launch configuration for
	 *         build purposes
	 */
	public static IResource getBuildableResource(ILaunchConfiguration configuration) {
		try {
			URI resourceURL = new URI(configuration.getAttribute(TO_BUILD, "")); //$NON-NLS-1$
			IWorkspaceRoot root = ResourcesPlugin.getWorkspace().getRoot();
			IResource resource = Stream
					.concat(Arrays.stream(root.findContainersForLocationURI(resourceURL)),
							Arrays.stream(root.findFilesForLocationURI(resourceURL)))
					.filter(IResource::exists).findFirst().orElse(null);

			if (resource != null) {
				return resource;
			}
		} catch (Exception e) {
			// could not found TO_BUILD attribute, cancel
		}

		return getProject(configuration);
	}

	/**
	 * Returns an {@code IResource} that maps to given launch configuration for
	 * launch purposes.
	 * 
	 * @param configuration the current launch configuration
	 * 
	 * @return an {@code IResource} that maps to the given launch configuration for
	 *         launch purposes
	 */
	public static IResource getLaunchableResource(ILaunchConfiguration configuration) {
		try {
			URI resourceURL = new URI(configuration.getAttribute(TO_LAUNCH, "")); //$NON-NLS-1$
			IWorkspaceRoot root = ResourcesPlugin.getWorkspace().getRoot();
			IResource resource = Stream
					.concat(Arrays.stream(root.findContainersForLocationURI(resourceURL)),
							Arrays.stream(root.findFilesForLocationURI(resourceURL)))
					.filter(IResource::exists).findFirst().orElse(null);

			if (resource != null) {
				return resource;
			}
		} catch (Exception e) {
			Platform.getLog(AbstractLaunchConfigurationDelegate.class).error(e.getMessage());
		}

		throw new OperationCanceledException();
	}

	/**
	 * Sets this launch purpose's {@code IResource} to be that maps to the given
	 * launch configuration.
	 * 
	 * @param configuration the configuration being launched
	 * @param mode          launch mode
	 * 
	 * @return an {@code IResource} that maps to the given launch configuration for
	 *         launch purposes
	 */
	protected abstract IResource setLaunchableResource(ILaunchConfiguration configuration, String mode);

	/**
	 * Returns the name of the builder to run for this launch configuration, or
	 * {@code null} if the name has not been set.
	 * 
	 * @return Returns the name of the builder to run for this command, or
	 *         {@code null} if the name has not been set.
	 */
	protected String getBuilderName() {
		return null;
	}

	/**
	 * Sets arguments to be the given table of name-values pairs, or to {@code null}
	 * if there are no arguments. The argument names and values are both strings. *
	 * 
	 * @param configuration the configuration being launched
	 * @param mode          launch mode
	 * 
	 * @return a table of command arguments (key type : {@code String} value type :
	 *         {@code String}), or {@code null}
	 */
	protected Map<String, String> setArguments(ILaunchConfiguration configuration, String mode) {
		return getArguments(configuration);
	}

	/**
	 * Sets build command to run when building the described project.
	 * 
	 * @param configuration the configuration being launched
	 * @param mode          launch mode
	 * 
	 * @return modified project
	 * 
	 * @throws CoreException if an exception occurs
	 */
	private IProject modifyProject(ILaunchConfiguration configuration, String mode) throws CoreException {
		IProject project = getProject(configuration);
		String builderName = getBuilderName();
		if (builderName == null) {
			return project;
		}

		IProjectDescription description = project.getDescription();
		ICommand[] buildSpec = description.getBuildSpec();

		ICommand command = Arrays.stream(buildSpec)
				.filter(buildCommand -> buildCommand.getBuilderName().equals(builderName)).findFirst().orElse(null);
		if (command == null) {
			command = description.newCommand();
			command.setBuilderName(builderName);
			buildSpec = Arrays.copyOf(buildSpec, buildSpec.length + 1);
			buildSpec[buildSpec.length - 1] = command;
		}

		command.setArguments(setArguments(configuration, mode));

		description.setBuildSpec(buildSpec);
		project.setDescription(description, new NullProgressMonitor());
		return project;
	}

}
