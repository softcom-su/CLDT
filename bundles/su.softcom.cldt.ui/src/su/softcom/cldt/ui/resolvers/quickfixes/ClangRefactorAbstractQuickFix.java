package su.softcom.cldt.ui.resolvers.quickfixes;

import java.io.File;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.ui.IMarkerResolution;

import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.operations.ClangRefactorOperation;
import su.softcom.cldt.internal.core.CMakeProject;
import su.softcom.cldt.internal.core.builders.CMakeConfigureBuilder;

/**
 * Resolves markers by running clang-refactor and rewriting source files
 */
public abstract class ClangRefactorAbstractQuickFix implements IMarkerResolution {

	protected static final String COMPILE_COMMANDS = "compile_commands.json"; //$NON-NLS-1$

	private String label;
	private String clangRefactorAction;

	protected ClangRefactorAbstractQuickFix(String action, String label) {
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
		ClangRefactorOperation clangRefactorOperation;
		ICMakeProject project = new CMakeProject(marker.getResource().getProject());
		Set<IFile> files = new HashSet<>();

		if (marker.getResource() instanceof IFile file) {
			files.add(file);
		}

		String buildFolderPath = project.getBuildFolder().getLocation().toOSString();

		prepareCompileCommands(project.getProject(), buildFolderPath);

		List<String> args = new ArrayList<>();
		args.add(clangRefactorAction);
		args.add("-p"); //$NON-NLS-1$
		args.add(buildFolderPath);
		args.add("-i"); //$NON-NLS-1$

		List<String> parameters = extractParameters(marker);
		if (parameters == null) {
			return;
		}

		args.addAll(parameters);

		clangRefactorOperation = new ClangRefactorOperation(project.getProject(), files, args, marker);
		Job job = Job.create("Выполнение clang-refactor", clangRefactorOperation); //$NON-NLS-1$
		job.setRule(marker.getResource());
		job.schedule();
	}

	/**
	 * Extracts all needed parameters from given marker
	 */
	protected abstract List<String> extractParameters(IMarker marker);

	@SuppressWarnings("restriction")
	private void prepareCompileCommands(IProject project, String buildFolderPath) {
		File compileCommandsFile = new File(buildFolderPath, COMPILE_COMMANDS);

		if (!compileCommandsFile.exists()) {
			try {
				project.build(IncrementalProjectBuilder.INCREMENTAL_BUILD, CMakeConfigureBuilder.ID, null,
						new NullProgressMonitor());
			} catch (CoreException e) {
				e.printStackTrace();
			}
		}
	}

}
