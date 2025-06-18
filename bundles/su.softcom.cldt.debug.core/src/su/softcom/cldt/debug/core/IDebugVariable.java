package su.softcom.cldt.debug.core;

import su.softcom.cldt.debug.internal.core.GDBClient;

/**
 * Public API for variable descriptions
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public interface IDebugVariable {

	// TODO: Refactor
	/**
	 * 
	 * Get full memory address (for global variables)
	 * 
	 * @param client
	 * @param function
	 * @param cu
	 * @return
	 */
	public long getAddress(GDBClient client, IFunction function, ICompileUnit cu);

	public long getAddress();

	/**
	 * Взять размер переменной
	 * 
	 * @return размер переменной
	 */
	public long getSize();

	/**
	 * Взять имя переменной
	 * 
	 * @return имя переменной
	 */
	public String getName();

	public IType getType();

	public int getLine();

	public int getFileIndex();

	public boolean isArtificial();
}