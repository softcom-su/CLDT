package su.softcom.cldt.internal.core.cmake.fileapi;

import java.util.List;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.Platform;

import com.fasterxml.jackson.annotation.JsonSetter;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;

public class ConfigurationObject {

	public static class ProjectObject {
		public int[] directoryIndexes;
		public String name;
		public int[] targetIndexes;
		public int[] childIndexes;
		public int parentIndex = -1;
	}

	public static class TargetObject {
		public int directoryIndex;
		public String id;
		public String jsonFile;
		public String name;
		public int projectIndex;
	}

	public static class DirectoryObject {
		public String build;
		public String jsonFile;
		public Version minimumCMakeVersion;
		public int projectIndex;
		public String source;
		public int[] targetIndexes;
		public int[] childIndexes;
		public int parentIndex;
		public boolean hasInstallRule;

	}

	public ConfigurationObject(String name, List<TargetObject> targets, List<DirectoryObject> directories,
			List<ProjectObject> projects) {
		this.name = name;
		this.targets = targets;
		this.directories = directories;
		this.projects = projects;
	}

	final String name;
	public final List<TargetObject> targets;
	final List<DirectoryObject> directories;
	final List<ProjectObject> projects;

	@JsonSetter("projects")
	public void setProjects(List<ProjectObject> projects) {
		if (projects.size() != 1) {
			Platform.getLog(getClass()).warn("Unexpected size of projects list");
		}
		for (ProjectObject prj : projects) {
			IProject project = ResourcesPlugin.getWorkspace().getRoot().getProject(prj.name);
			if (!project.exists()) {
				throw new IllegalArgumentException("Project with name %s doesn't exists".formatted(prj.name));
			}
			ICMakeProject cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
		}
	}
}
