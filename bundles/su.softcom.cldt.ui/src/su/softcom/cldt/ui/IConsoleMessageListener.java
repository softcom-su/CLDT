package su.softcom.cldt.ui;

import org.eclipse.core.runtime.IStatus;

public interface IConsoleMessageListener {
	/**
	 * Called when a command is invoked.
	 * 
	 * @param line the command invocation string
	 */
	public void commandInvoked(String line);

	/**
	 * Called when a line of message text has been received.
	 * 
	 * @param line   the line of text
	 * @param status the status returned from the command message parser
	 */
	public void messageLineReceived(String line, IStatus status);

	/**
	 * Called when a line of error text has been received.
	 * 
	 * @param line   the line of text
	 * @param status the status returned from the command message parser
	 */
	public void errorLineReceived(String line, IStatus status);

	/**
	 * Called when a command has been completed.
	 * 
	 * @param status    the status code, or null if not applicable
	 * @param exception an exception, or null if not applicable
	 */
	public void commandCompleted(IStatus status, Exception exception);
}