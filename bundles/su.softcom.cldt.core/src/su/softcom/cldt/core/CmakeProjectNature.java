package su.softcom.cldt.core;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.eclipse.core.resources.ICommand;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.IProjectNature;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.NullProgressMonitor;

import su.softcom.cldt.internal.core.builders.CMakeConfigureBuilder;
import su.softcom.cldt.internal.core.builders.CMakeProjectBuilder;

/**
 * General nature for any CMake projects. Adds Configure and Build builders for
 * project to the end of builders list. Any modifiers for CMakeLists must be
 * inserted before this builders.
 * 
 * @author Petr Babanov
 */
public final class CmakeProjectNature implements IProjectNature {

	public static final String ID = "su.softcom.cldt.core.cnature"; //$NON-NLS-1$

	public static final String RELEASE_BUILD_TYPE = "Release"; //$NON-NLS-1$

	public static final String DEBUG_BUILD_TYPE = "Debug"; //$NON-NLS-1$

	public static final String MIN_SIZE_REL_BUILD_TYPE = "MinSizeRel"; //$NON-NLS-1$

	public static final String REL_WITH_DEBUG_INFO_BUILD_TYPE = "RelWithDebInfo"; //$NON-NLS-1$

	protected IProject project;

	@Override
	public void configure() throws CoreException {
		IProjectDescription description = project.getDescription();
		List<ICommand> commands = new ArrayList<>(Arrays.asList(description.getBuildSpec()));
		List<String> newCommandsIDs = new ArrayList<>(List.of(CMakeConfigureBuilder.ID, CMakeProjectBuilder.ID));
		for (ICommand command : commands) {
			if (newCommandsIDs.contains(command.getBuilderName())) {
				newCommandsIDs.remove(command.getBuilderName());
			}
		}
		for (String builderID : newCommandsIDs) {
			commands.add(getBuildCommand(builderID));
		}
		ResourcesPlugin.getWorkspace().newBuildConfig(project.getName(), RELEASE_BUILD_TYPE);
		ResourcesPlugin.getWorkspace().newBuildConfig(project.getName(), DEBUG_BUILD_TYPE);
		ResourcesPlugin.getWorkspace().newBuildConfig(project.getName(), MIN_SIZE_REL_BUILD_TYPE);
		ResourcesPlugin.getWorkspace().newBuildConfig(project.getName(), REL_WITH_DEBUG_INFO_BUILD_TYPE);
		description.setBuildConfigs(new String[] { RELEASE_BUILD_TYPE, DEBUG_BUILD_TYPE, MIN_SIZE_REL_BUILD_TYPE,
				REL_WITH_DEBUG_INFO_BUILD_TYPE });
		description.setBuildSpec(commands.toArray(new ICommand[commands.size()]));
		project.setDescription(description, new NullProgressMonitor());
	}

	private ICommand getBuildCommand(String builderID) throws CoreException {
		ICommand buildCommand = project.getDescription().newCommand();
		buildCommand.setBuilderName(builderID);
		return buildCommand;
	}

	@Override
	public final void deconfigure() throws CoreException {
		IProjectDescription projectDescription = project.getDescription();
		List<ICommand> commands = new ArrayList<ICommand>(Arrays.asList(projectDescription.getBuildSpec()));
		commands = commands.stream().filter(command -> {
			return !(command.getBuilderName().equals(CMakeConfigureBuilder.ID)
					|| (command.getBuilderName().equals(CMakeProjectBuilder.ID))); // $NON-NLS-1$
		}).toList();
		projectDescription.setBuildSpec(commands.toArray(new ICommand[commands.size()]));
		project.setDescription(projectDescription, new NullProgressMonitor());
	}

	@Override
	public IProject getProject() {
		return project;
	}

	@Override
	public void setProject(IProject project) {
		this.project = project;
	}

}
