package su.softcom.cldt.core.cmake;

import java.util.List;

/**
 * Interface for interacting with command in CMake files
 * 
 * @author Petr Babanov (petr.babanov@softcom.su) Initial implementation
 *
 */
public interface ICMakeCommand {

	/**
	 * get command identifier
	 * 
	 * @return name
	 */
	public String getName();

	/**
	 * get arguments
	 * 
	 * @return immutable copy arguments list
	 */
	public List<ArgumentNode> getArgs();

	/**
	 * add single argument to command
	 * 
	 * @param argument
	 */
	public void addArg(ArgumentNode argument);

	/**
	 * add all arguments from list to exists arguments
	 * 
	 * @param arguments - arguments to add
	 */
	public void addAllArg(List<ArgumentNode> arguments);

	/**
	 * replace all arguments by values from list
	 * 
	 * @param arguments
	 */
	public void setArgs(List<ArgumentNode> arguments);

}
