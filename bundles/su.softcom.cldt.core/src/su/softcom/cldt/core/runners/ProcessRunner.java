package su.softcom.cldt.core.runners;

import java.io.IOException;

import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.ui.IWindowListener;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.console.IOConsole;

import su.softcom.cldt.ui.console.ConsoleActions;

/** Abstract external processes runner. */
public abstract class ProcessRunner implements IRunner {

	protected ProblemsEmitter problems;

	protected IOConsole console;
	protected IRunner next;

	/**
	 * Initialize internal fields.
	 */
	public ProcessRunner() {
		this(null);
	}

	/**
	 * Initialize internal fields and set I/O console.
	 * 
	 * @param console I/O console
	 */
	public ProcessRunner(final IOConsole console) {
		this.console = console;
		this.problems = new ProblemsEmitter();
	}

	@Override
	public final void setConsole(final IOConsole console) {
		this.console = console;
	}

	@Override
	public final IOConsole getConsole() {
		return console;
	}

	@Override
	public void start() {
		final Process process = buildProcess();
		if (process == null) {
			return;
		}

		process.onExit().thenRunAsync(() -> {
			try {
				problems.getDefault().refreshLocal(IResource.DEPTH_INFINITE, null);
			} catch (CoreException e) {
				e.printStackTrace();
			}

			if (process.exitValue() == 0 && next != null) {
				next.addResources(getResults());
				next.start();
			}
		});

		parseAndRedirectStreams(process);
		ConsoleActions.registerExecution(process);
		destroyOnWindowClose(process);
	}

	@Override
	public final IRunner next(final IRunner runner) {
		if (runner != null && console != null) {
			runner.setConsole(console);
		}

		next = runner;
		return next;
	}

	/**
	 * Build execution process from resources.
	 * 
	 * @return running external process
	 */
	protected abstract Process buildProcess();

	/**
	 * Logic of execution streams processing and redirection. Will be run into
	 * support thread of execution process.
	 * 
	 * @param process execution process
	 * 
	 * @throws IOException if an I/O error occurs
	 */
	protected abstract void streamsRedirector(final Process process) throws IOException;

	/**
	 * Launch streams processing into support thread of execution process.
	 * 
	 * @param process execution process
	 */
	protected final void parseAndRedirectStreams(final Process process) {
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					streamsRedirector(process);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}).start();
	}

	/**
	 * Make external process destructible on the main window close.
	 * 
	 * @param process running external process
	 */
	protected static void destroyOnWindowClose(final Process process) {
		if (process == null || !process.isAlive()) {
			return;
		}

		final IWindowListener listener = new IWindowListener() {
			@Override
			public void windowActivated(final IWorkbenchWindow window) {
			}

			@Override
			public void windowClosed(final IWorkbenchWindow window) {
				process.destroy();
			}

			@Override
			public void windowDeactivated(final IWorkbenchWindow window) {
			}

			@Override
			public void windowOpened(final IWorkbenchWindow window) {
			}
		};

		PlatformUI.getWorkbench().addWindowListener(listener);
		process.onExit().thenRunAsync(() -> PlatformUI.getWorkbench().removeWindowListener(listener));
	}
}
