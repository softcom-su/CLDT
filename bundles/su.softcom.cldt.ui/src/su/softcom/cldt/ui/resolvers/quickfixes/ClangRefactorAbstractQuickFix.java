package su.softcom.cldt.ui.resolvers.quickfixes;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.ui.IMarkerResolution;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.operations.ClangRefactorOperation;

/**
 * Resolves markers by running clang-refactor and rewriting source files
 */
public abstract class ClangRefactorAbstractQuickFix implements IMarkerResolution {

	private String label;
	private List<String> args;
	private String clangRefactorAction;

	public ClangRefactorAbstractQuickFix(String action, String label) {
		setClangRefactorAction(action);
		setLabel(label);
	}

	@Override
	public String getLabel() {
		return label;
	}

	private void setLabel(String label) {
		this.label = label;
	}

	public String getClangRefactorAction() {
		return clangRefactorAction;
	}

	private void setClangRefactorAction(String action) {
		this.clangRefactorAction = action;
	}

	@Override
	public void run(IMarker marker) {
		ClangRefactorOperation сLangRefactorOperation;
		IProject Project = marker.getResource().getProject();
		Set<IFile> files = new HashSet<IFile>();

		if (marker.getResource() instanceof IFile file) {
			files.add(file);
		}

		String buildFolder = new ProjectScope(Project).getNode(PreferenceConstants.NODE)
				.get(PreferenceConstants.BUILD_FOLDER, PreferenceConstants.DEFAULT_BUILD_FOLDER);

		args = new ArrayList<>();
		args.add(clangRefactorAction);
		args.add("-p"); //$NON-NLS-1$
		args.add(Project.getFolder(buildFolder).getLocation().toOSString());
		args.addAll(extractParameters(marker));

		сLangRefactorOperation = new ClangRefactorOperation(Project, files, args, marker);
		Job job = Job.create("Выполнение clang-refactor", сLangRefactorOperation); //$NON-NLS-1$
		job.setRule(marker.getResource());
		job.schedule();
	}

	/**
	 * Extracts all needed parameters from given marker
	 */
	protected abstract List<String> extractParameters(IMarker marker);

}
