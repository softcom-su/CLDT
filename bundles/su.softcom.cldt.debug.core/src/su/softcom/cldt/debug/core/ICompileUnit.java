package su.softcom.cldt.debug.core;

import java.nio.file.Path;
import java.util.List;

import org.eclipse.core.resources.IFile;

import su.softcom.cldt.debug.internal.core.dwarf.BreakpointInfo;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Tag;

public interface ICompileUnit {

	public IFunction getFunction(long address);

	/**
	 * Get global variables (static)
	 * 
	 * @return List of global variables
	 */
	public List<IDebugVariable> getVariables();

	public List<IDebugVariable> getVariables(IFunction function, long currentLine);

	/**
	 * Checking contains address in current compile unit
	 * 
	 * @param address
	 * @return {@code true} if contains, {@code false} otherwise
	 */
	public boolean containsAddress(long address);

	/**
	 * Checking is statement address
	 * 
	 * @param address
	 * @return {@code true} if statement == true, {@code false} otherwise
	 */
	public boolean isStatementAddress(long address);

	/**
	 * Check is compile unit contains file in source files
	 * 
	 * @param file
	 * @return {@code true} if contains or {@code false} otherwise
	 */
	public boolean containsFile(IFile file);

	/**
	 * returns path to file from address or null if not contains
	 * 
	 * @param address
	 * @return Path file or null
	 */
	public Path getFilePath(long address);

	/**
	 * @param address - instructure address
	 * @return line number or {@code -1} if not found
	 */
	public int getLine(long address);

	/**
	 * @param address
	 * @return previous address
	 */
	public long getPreviousAddress(long address);

	/**
	 * get preferred breakpoint address for passed file and line
	 * 
	 * @param file
	 * @param line
	 * @return preferred info about breakpoint
	 */
	public BreakpointInfo getBreakpointAddress(IFile file, int line);

	public int getColumn(long address);

	/**
	 * Get address size for this compile unit
	 * 
	 * @return positive {@code long}
	 */
	public int getAddressSize();

	public int getOffset();

	public int getFullOffset();

	public long getAddress(int off);

	public long getStrOffsetBase();

	public Tag getTagByOffset(int offset);
}
