package su.softcom.cldt.debug.core.info;

import org.eclipse.core.resources.IFile;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IStackInfo;
import su.softcom.cldt.debug.internal.core.dwarf.BreakpointInfo;

public interface IDebugInfo {

	ICompileUnit getCompileUnit(long address);

	/**
	 * Нужен, если в ELF нет секции .eh_frame, то пробуем взять секцию .debug_frame
	 * 
	 * @return
	 */
	IStackInfo getStackInfo();

	/**
	 * return preferred address for breakpoint setting
	 * 
	 * @param file
	 * @param line
	 * @return preffered info about breakpoint
	 */
	BreakpointInfo getBreakpointAddress(IFile file, int line);

}
