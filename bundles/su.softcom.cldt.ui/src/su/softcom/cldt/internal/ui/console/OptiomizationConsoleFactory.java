package su.softcom.cldt.internal.ui.console;

import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.IConsoleFactory;
import org.eclipse.ui.console.IConsoleManager;
import org.eclipse.ui.console.MessageConsole;

public class OptiomizationConsoleFactory implements IConsoleFactory {

	public static final String CONSOLE_NAME = "su.softcom.cldt.ui.console.optimization"; //$NON-NLS-1$

	@Override
	public void openConsole() {
		getConsoleManager().showConsoleView(getConsole());
	}

	/**
	 * Get the current console or create a new one by console name.
	 * 
	 * @param consoleName console name
	 * 
	 * @return console
	 */
	public static MessageConsole getConsole() {
		for (IConsole existing : getConsoleManager().getConsoles()) {
			if (existing.getName().equals(CONSOLE_NAME)) {
				return (MessageConsole) existing;
			}
		}
		return createConsole(CONSOLE_NAME);
	}

	private static MessageConsole createConsole(final String consoleName) {
		MessageConsole console = new MessageConsole(consoleName, null);
		getConsoleManager().addConsoles(new IConsole[] { console });
		return console;
	}

	private static IConsoleManager getConsoleManager() {
		return ConsolePlugin.getDefault().getConsoleManager();
	}

}
