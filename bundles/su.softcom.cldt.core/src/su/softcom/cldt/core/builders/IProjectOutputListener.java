package su.softcom.cldt.core.builders;

import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * 
 */
public interface IProjectOutputListener {
	/**
	 * Called when a command is invoked.
	 * 
	 * @param line the command invocation string
	 */
	public void outLineReceived(String line);

	/**
	 * Called when a line of message text has been received.
	 * 
	 * @param line the line of text
	 */
	public void systemLineReceived(String line);

	/**
	 * Called when a line of error text has been received.
	 * 
	 * @param line the line of text
	 */
	public void errorLineReceived(String line);

	/**
	 * Get project
	 * 
	 * @return ICMakeProject
	 * 
	 */
	public ICMakeProject getListenersProject();

}