package su.softcom.cldt.internal.ui.handlers;

import java.util.Collections;

import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.jobs.IJobChangeEvent;
import org.eclipse.core.runtime.jobs.IJobChangeListener;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.jobs.JobChangeAdapter;
import org.eclipse.core.runtime.jobs.JobGroup;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;

import su.softcom.cldt.core.builders.CMakeBuilderArgs;
import su.softcom.cldt.ui.launch.handlers.CompileHandler;

/**
 * Technological build command handler.
 * 
 * @author Vladislav Ershov (ershov_vladislav@softcom.su) - add monitor and
 *         jobGroup; add getIJobChangeListener; externalized
 */
public class TechnologicalHandler extends CompileHandler {

	/** Progress monitor for technological build. */
	private IProgressMonitor monitor;
	/** Job group for technological build. */
	private JobGroup jobGroup;
	/** Is successful build. */
	private boolean isSuccessfulBuild;

	@Override
	protected void compile(IResource resource) throws ExecutionException {
		IProject project = resource.getProject();

		Job job = Job.create(Messages.TechnologicalHandler_TechnologicalBuild, (mon) -> {
			try {
				project.build(IncrementalProjectBuilder.FULL_BUILD, CMakeBuilderArgs.BUILDER_ID, Collections.emptyMap(),
						mon);
				isSuccessfulBuild = true;
			} catch (CoreException e) {
				isSuccessfulBuild = false;
				Platform.getLog(getClass()).error(e.getLocalizedMessage());
			}
		});

		monitor = Job.getJobManager().createProgressGroup();
		jobGroup = new JobGroup(Messages.TechnologicalHandler_TechnologicalBuild, 0, 0);

		job.addJobChangeListener(getIJobChangeListener());
		job.setProgressGroup(monitor, 100);
		job.setJobGroup(jobGroup);
		job.schedule();

		monitor.beginTask(Messages.TechnologicalHandler_TechnologicalBuild, 100);
	}

	/**
	 * Get change listener for technological build.
	 * 
	 * @return change listener for technological build
	 */
	private IJobChangeListener getIJobChangeListener() {
		return new JobChangeAdapter() {
			@Override
			public void done(IJobChangeEvent event) {
				if (isSuccessfulBuild) {
					Display.getDefault()
							.syncExec(() -> MessageDialog.openInformation(new Shell(),
									Messages.TechnologicalHandler_TechnologicalTranslation,
									Messages.TechnologicalHandler_TranslationCompletedSuccessfully));
				} else {
					Display.getDefault()
							.syncExec(() -> MessageDialog.openError(new Shell(),
									Messages.TechnologicalHandler_TechnologicalTranslation,
									Messages.TechnologicalHandler_ErrorsDuringTranslation));
				}
			}
		};
	}

}
