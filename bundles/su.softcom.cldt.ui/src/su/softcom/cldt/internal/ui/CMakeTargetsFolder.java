package su.softcom.cldt.internal.ui;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.resources.IProject;

import su.softcom.cldt.core.cmake.Target;

public class CMakeTargetsFolder {

	private List<Target> targets = new ArrayList<>();
	private final IProject project;

	public CMakeTargetsFolder(List<Target> targets, IProject project) {
		if (targets != null) {
			this.targets.addAll(targets);
		}
		if (project == null) {
			throw new IllegalArgumentException("project must be not null");
		}
		this.project = project;
	}

	public List<Target> getTargets() {
		return targets;
	}

	@Override
	public String toString() {
		return "Цели сборки";
	}

	public IProject getProject() {
		return project;
	}

	public void setTargets(List<Target> targets) {
		this.targets = targets;
	}
}
