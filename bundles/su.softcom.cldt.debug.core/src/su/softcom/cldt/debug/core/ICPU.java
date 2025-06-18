package su.softcom.cldt.debug.core;

import java.util.List;

import org.eclipse.debug.core.DebugException;

public interface ICPU extends IModelProvider{
	
	public byte[] getMemory(long startAddress, long length) throws DebugException;

	public List<String> getRegisters() throws DebugException;
	
	public void changeRegister(String regName, byte[] data)throws DebugException;
	
	public void changeMemory(long startAddress, byte[] data) throws DebugException;

	/** returns memory from start stack pointer to set stack pointer
	 * @param address
	 * @return
	 */
	public byte[] getStack(long address);
}

