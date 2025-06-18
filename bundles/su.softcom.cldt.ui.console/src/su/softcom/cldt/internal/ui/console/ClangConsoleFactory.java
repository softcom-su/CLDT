package su.softcom.cldt.internal.ui.console;

import java.util.ResourceBundle;

import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.IConsoleFactory;
import org.eclipse.ui.console.IConsoleManager;
import org.eclipse.ui.console.MessageConsole;

/**
 * Factory for Clang consoles.
 * 
 * @author victor.fadeev@softcom.su
 * @author ershov_vladislav@softcom.su
 */
public class ClangConsoleFactory implements IConsoleFactory {

	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	@Override
	public void openConsole() {
		getConsoleManager().showConsoleView(getConsole());
	}

	/**
	 * Get the current console or create a new one.
	 * 
	 * @return console
	 */
	public MessageConsole getConsole() {
		for (IConsole existing : getConsoleManager().getConsoles()) {
			if (existing.getName().equals(getConsoleName())) {
				return (MessageConsole) existing;
			}
		}

		return createConsole();
	}

	/**
	 * Get the current console or create a new one by console name.
	 * 
	 * @param consoleName console name
	 * 
	 * @return console
	 */
	public static MessageConsole getConsole(final String consoleName) {
		for (IConsole existing : getConsoleManager().getConsoles()) {
			if (existing.getName().equals(consoleName)) {
				return (MessageConsole) existing;
			}
		}

		return createConsole(consoleName);
	}

	private String getConsoleName() {
		return RESOURCE_BUNDLE.getString("console.clang"); //$NON-NLS-1$
	}

	private MessageConsole createConsole() {
		MessageConsole console = new MessageConsole(getConsoleName(), null);
		getConsoleManager().addConsoles(new IConsole[] { console });
		return console;
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
