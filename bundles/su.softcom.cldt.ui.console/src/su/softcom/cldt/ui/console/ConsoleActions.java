package su.softcom.cldt.ui.console;

import java.util.HashSet;
import java.util.ResourceBundle;
import java.util.Set;
import java.util.concurrent.CompletableFuture;

import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.IToolBarManager;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.IActionBars;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.IConsoleConstants;
import org.eclipse.ui.console.IConsolePageParticipant;
import org.eclipse.ui.console.actions.CloseConsoleAction;
import org.eclipse.ui.part.IPageBookViewPage;

/** Console button settings. */
public class ConsoleActions implements IConsolePageParticipant {

	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	private static final String TERMINATE = "icons/full/elcl16/terminate_co.png"; //$NON-NLS-1$
	private static final String TERMINATE_DISABLED = "icons/full/dlcl16/terminate_co.png"; //$NON-NLS-1$

	private static final String REMOVE = "icons/full/elcl16/rem_co.png"; //$NON-NLS-1$
	private static final String REMOVE_DISABLED = "icons/full/dlcl16/rem_co.png"; //$NON-NLS-1$

	private static boolean isTerminateEnabled = false;
	private static boolean isRemoveEnabled = true;
	private static Set<Action> terminateButtons = new HashSet<Action>();
	private static Set<Action> removeButtons = new HashSet<Action>();
	private static Set<IActionBars> toolbars = new HashSet<IActionBars>();

	private static Process process;
	private static Thread thread;

	private IConsole console;
	private Action terminate, remove;
	private IActionBars toolbar;

	@Override
	@SuppressWarnings("hiding")
	public void init(final IPageBookViewPage page, final IConsole console) {
		this.console = console;
		this.toolbar = page.getSite().getActionBars();

		createTerminateButton();
		createRemoveButton();

		IToolBarManager toolbarManager = toolbar.getToolBarManager();
		toolbarManager.appendToGroup(IConsoleConstants.LAUNCH_GROUP, terminate);
		toolbarManager.appendToGroup(IConsoleConstants.LAUNCH_GROUP, remove);

		toolbar.updateActionBars();
		toolbars.add(toolbar);
	}

	@Override
	public void dispose() {
		console = null;

		terminateButtons.remove(terminate);
		terminate = null;

		removeButtons.remove(remove);
		remove = null;

		toolbars.remove(toolbar);
		toolbar = null;
	}

	@Override
	public void activated() {
	}

	@Override
	public void deactivated() {
	}

	@Override
	public <T> T getAdapter(Class<T> adapter) {
		return null;
	}

	/**
	 * Register execution {@link Process} into console terminate button. If it is
	 * not alive or {@code null}, no action is taken. If existing execution alive,
	 * it would be destroyed.
	 * 
	 * @param exec alive execution
	 */
	public static synchronized void registerExecution(final Process exec) {
		if (exec == null || !exec.isAlive()) {
			return;
		}

		destroyExecution();

		process = exec;
		exec.onExit().thenRunAsync(() -> updateButtons());

		Display.getDefault().asyncExec(() -> {
			updateButtons();
		});
	}

	/**
	 * Register execution {@link Thread} into console terminate button. If it is not
	 * alive or {@code null}, no action is taken. If existing execution alive, it
	 * would be destroyed.
	 * 
	 * @param exec alive execution
	 */
	public static synchronized void registerExecution(final Thread exec) {
		if (exec == null || !exec.isAlive()) {
			return;
		}

		destroyExecution();

		thread = exec;
		CompletableFuture.runAsync(() -> {
			try {
				exec.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}).thenRunAsync(() -> updateButtons());

		updateButtons();
	}

	/**
	 * Check if existing execution alive.
	 * 
	 * @return {@code true} if execution alive
	 */
	public static synchronized boolean isExecution() {
		return process != null && process.isAlive() || thread != null && thread.isAlive();
	}

	/**
	 * Destroy existing execution, if it is alive.
	 */
	public static synchronized void terminateExecution() {
		destroyExecution();
		updateButtons();
	}

	@SuppressWarnings("deprecation")
	private static void destroyExecution() {
		if (process != null && process.isAlive()) {
			process.destroy();
		} else if (thread != null && thread.isAlive()) {
			thread.stop();
		}
	}

	private static void updateButtons() {
		boolean exec = isExecution();
		if (isTerminateEnabled == exec && isRemoveEnabled == !exec) {
			return;
		}

		isTerminateEnabled = exec;
		isRemoveEnabled = !exec;

		terminateButtons.forEach(action -> action.setEnabled(isTerminateEnabled));
		removeButtons.forEach(action -> action.setEnabled(isRemoveEnabled));
		toolbars.forEach(actionBar -> actionBar.updateActionBars());
	}

	private static ImageDescriptor getImageDescriptor(final String path) {
		return ImageDescriptor.createFromURL(Platform.getBundle("org.eclipse.debug.ui").getEntry(path)); //$NON-NLS-1$
	}

	private void createTerminateButton() {
		terminate = new Action(RESOURCE_BUNDLE.getString("console.terminate"), getImageDescriptor(TERMINATE)) { //$NON-NLS-1$
			@Override
			public void run() {
				terminateExecution();
			}
		};

		terminate.setDisabledImageDescriptor(getImageDescriptor(TERMINATE_DISABLED));
		terminate.setEnabled(isTerminateEnabled);

		terminateButtons.add(terminate);
	}

	private void createRemoveButton() {
		remove = new CloseConsoleAction(console);

		remove.setImageDescriptor(getImageDescriptor(REMOVE));
		remove.setDisabledImageDescriptor(getImageDescriptor(REMOVE_DISABLED));
		remove.setEnabled(isRemoveEnabled);

		removeButtons.add(remove);
	}

}
