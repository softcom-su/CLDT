package su.softcom.cldt.core.runners;

import org.eclipse.core.resources.IResource;
import org.eclipse.ui.console.IOConsole;

/** Execution runner interface. */
public interface IRunner {

	/**
	 * Set console for execution I/O streams redirection.
	 * 
	 * @param console I/O console
	 */
	public void setConsole(final IOConsole console);

	/**
	 * Get runner I/O console.
	 * 
	 * @return I/O console
	 */
	public IOConsole getConsole();

	/**
	 * Add new parameter to executor.
	 * 
	 * @param arg executor parameter
	 */
	public default void addArg(final String arg) {
	}

	/**
	 * Add array of executor parameters.
	 * 
	 * @param args executor parameters
	 */
	public default void addArgs(final String[] args) {
		if (args != null) {
			for (final String arg : args) {
				addArg(arg);
			}
		}
	}

	/**
	 * Add collection of executor parameters.
	 * 
	 * @param args executor parameters
	 */
	public default void addArgs(final Iterable<String> args) {
		if (args != null) {
			args.forEach(arg -> addArg(arg));
		}
	}

	/**
	 * Add new resource to execute.
	 * 
	 * @param resource resource to execute
	 */
	public default void addResource(final IResource resource) {
	}

	/**
	 * Add array of resources to execute.
	 * 
	 * @param resources resources to execute
	 */
	public default void addResources(final IResource[] resources) {
		if (resources != null) {
			for (final IResource resource : resources) {
				addResource(resource);
			}
		}
	}

	/**
	 * Add collection of resources to execute.
	 * 
	 * @param resources resources to execute
	 */
	public default void addResources(final Iterable<IResource> resources) {
		if (resources != null) {
			resources.forEach(resource -> addResource(resource));
		}
	}

	/**
	 * Get result of execution.
	 * 
	 * @return output resources, or {@code null} if none
	 */
	public default IResource[] getResults() {
		return null;
	}

	/**
	 * Start execution.
	 */
	public void start();

	/**
	 * Set next {@link IRunner} of pipeline, which will be run, if current
	 * successfully complete. It's get the same console and result resources of
	 * current runner.
	 * 
	 * @param runner next runner of pipeline
	 * 
	 * @return next runner for sequential calls
	 */
	public IRunner next(final IRunner runner);

}
