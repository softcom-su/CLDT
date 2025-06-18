package su.softcom.cldt.debug.core;

import java.math.BigInteger;
import java.util.List;
import java.util.Map;

import su.softcom.cldt.debug.internal.core.GDBClient;

/**
 * Public API for function descriptions
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 * @author Drumov Maxim (drumov.maxim@softcom.su)
 */
public interface IFunction {

	/**
	 * Указывается содержится ли адрес в этой функции
	 * 
	 * @param address
	 * @return {@code true} если содержится, {@code false} иначе
	 */
	public boolean contents(long address);

	/**
	 * Возвращает массив смещений, для нахождения локальных переменных
	 * 
	 * @return int[]
	 */
	public int[] getVariables();

	/**
	 * Возвращает имя функции
	 * 
	 * @return сткровое представление имени
	 */
	public String getName();

	/**
	 * Возвращает аргументы функции или пустой список, если функция (void)
	 * 
	 * @return new unmodifiable {@link java.util.List List}
	 */
	public List<IDebugVariable> getArguments();

	// TODO: Refactor
	/**
	 * Расшифровывает frame base для расшифровки операции
	 * {@link su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants
	 * DwarfConstants.DW_OP_fbreg}
	 * 
	 * @param client
	 * @param regs
	 * @return frame base
	 */
	public long resolveFrameBase(GDBClient client, Map<Integer, BigInteger> regs);

	/**
	 * Возвращает текущий блок компиляции для этой функции
	 * 
	 * @return текущий {@link su.coftcom.cldt.debug.core.ICompileUnit ICompileUnit}
	 */
	public ICompileUnit getCompileUnit();

	/**
	 * Находится ли адрес в прологе функции
	 * 
	 * @param address адрес, который необходимо проверить на принадлежность прологу
	 * 
	 * @return {@code true} если адрес в прологе, иначе {@code false}
	 */
	public boolean isProlog(long address);

}
