package su.softcom.cldt.ui.resolvers.quickfixes;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.ui.IMarkerResolution;

import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.operations.ClangTidyOperation;
import su.softcom.cldt.internal.core.CMakeProject;
import su.softcom.cldt.internal.core.builders.CMakeConfigureBuilder;

/**
 * Resolves markers by running clang-tidy with --fix argument.
 */
public class ClangTidyAutoQuickFix implements IMarkerResolution {

	private String label;

	protected static final String COMPILE_COMMANDS = "compile_commands.json"; //$NON-NLS-1$

	/**
	 * Creates a new instance for ClangTidyAutoQuickFix.
	 * 
	 * @param marker for making a fix.
	 */
	public ClangTidyAutoQuickFix(IMarker marker) {
		String checkId = ""; //$NON-NLS-1$
		try {
			checkId = (String) marker.getAttribute("checkId"); //$NON-NLS-1$
		} catch (CoreException e) {
			e.printStackTrace();
		}

		this.label = String.format("Clang-tidy auto fix: %s", checkId); //$NON-NLS-1$
	}

	@Override
	public String getLabel() {
		return label;
	}

	@Override
	public void run(IMarker marker) {
		ICMakeProject project = new CMakeProject(marker.getResource().getProject());

		if (!(marker.getResource() instanceof IFile file)) {
			return;
		}

		final List<String> args = prepareClangTidyArgs(marker, file);

		String buildFolderPath = project.getBuildFolder().getLocation().toOSString();

		prepareCompileCommands(project.getProject(), buildFolderPath);

		Job job = Job.create("Быстрое исправление clang-tidy", monitor -> {
			try {
				ClangTidyOperation operation = new ClangTidyOperation(Set.of(file), args, buildFolderPath);
				operation.run(monitor);

				marker.delete();

				file.touch(monitor);
				return Status.OK_STATUS;

			} catch (CoreException e) {
				return new Status(IStatus.ERROR, "Clang-tidy", "Ошибка при исправлении", e); //$NON-NLS-1$
			}
		});

		job.setRule(marker.getResource());
		job.schedule();
	}

	private List<String> prepareClangTidyArgs(IMarker marker, IFile file) {
		List<String> args = new ArrayList<>();

		String checkId;
		int lineNumber;
		try {
			checkId = (String) marker.getAttribute("checkId"); //$NON-NLS-1$
			lineNumber = (int) marker.getAttribute(IMarker.LINE_NUMBER);
		} catch (CoreException e) {
			e.printStackTrace();
			return Collections.emptyList();
		}

		args.add("--checks=-*," + checkId); //$NON-NLS-1$

		String filePath = file.getLocation().toOSString();

		String lineFilter = "--line-filter=[{\"name\":\"%s\",\"lines\":[[%d,%d]]}]".formatted(filePath, lineNumber, //$NON-NLS-1$
				lineNumber);
		args.add(lineFilter);

		args.add("--fix"); //$NON-NLS-1$

		return args;
	}

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