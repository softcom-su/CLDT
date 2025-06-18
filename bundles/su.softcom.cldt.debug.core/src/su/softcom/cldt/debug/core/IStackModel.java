package su.softcom.cldt.debug.core;

import java.util.List;

import org.eclipse.debug.core.DebugException;

import su.softcom.cldt.debug.core.info.IDebugInfo;

/**Describes stack model for emulator architecture
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public interface IStackModel {

	
	public IRegisterModel getRegisterModel();
	
	public long getReturnAddress(List<String> regs, byte[] stack);
	
	/** Find address mapped to calling current function. 
	 * Not required, must throws {@link org.eclipse.debug.core.DebugException DebugException} if not supported
	 * @param info - required not null, throws {@code IllegalArgumentException}
	 * @param returnAddress - start address for searching calling function
	 * @return address, mapped for function call or {@code -1 } if not found
	 * @throws DebugException if this functionality not supported
	 */
	public long getCallPlace(IDebugInfo info, long returnAddress) throws DebugException;

}
