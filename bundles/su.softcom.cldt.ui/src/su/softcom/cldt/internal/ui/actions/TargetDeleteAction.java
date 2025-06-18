package su.softcom.cldt.internal.ui.actions;

import java.util.List;

import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.jface.viewers.StructuredViewer;
import org.eclipse.jface.window.IShellProvider;
import org.eclipse.swt.widgets.Control;
import org.eclipse.ui.actions.SelectionListenerAction;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;

public class TargetDeleteAction extends SelectionListenerAction {

	private static final String TARGET_DELETE_ACTION_NAME = "Удалить";
	private IShellProvider shellProvider;
	private StructuredViewer viewer;

	/**
	 * Creates a new delete Target action.
	 *
	 * @param provider the shell provider to use. Must not be <code>null</code>.
	 * @since 3.4
	 */
	public TargetDeleteAction(IShellProvider provider, StructuredViewer viewer) {
		super(TARGET_DELETE_ACTION_NAME);
		Assert.isNotNull(provider);
		this.shellProvider = provider;
		this.viewer = viewer;
	}

	@Override
	public void run() {
		List<?> list = getSelectedNonResources();
		if (list.isEmpty()) {
			return;
		}
		
		Job deletionJob = new Job(TARGET_DELETE_ACTION_NAME) {

			@Override
			protected IStatus run(IProgressMonitor monitor) {
				boolean wasDeleted = false;
				for (Object obj : list) {
					if (obj instanceof Target tgt) {
						ICMakeProject cmakeProject = CMakeCorePlugin.getDefault()
								.getProject(tgt.getProject().getProject());
						wasDeleted = cmakeProject.removeTarget(tgt);
					}
				}
				if (wasDeleted) {
					Control ctrl = shellProvider.getShell();
					ctrl.getDisplay().asyncExec(() -> {
						// Abort if this happens after disposes
						Control ctrl1 = viewer.getControl();
						if (ctrl1 == null || ctrl1.isDisposed()) {
							return;
						}
						viewer.refresh();
					});
				}
				return Status.OK_STATUS;
			}
		};

		deletionJob.schedule();

	}

}
