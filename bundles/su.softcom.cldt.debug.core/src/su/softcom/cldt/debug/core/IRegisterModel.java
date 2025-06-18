package su.softcom.cldt.debug.core;

import java.util.List;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.IRegister;
import org.eclipse.debug.core.model.IRegisterGroup;

/**
 * Register model for emulator architectre. Describes set of registers for
 * emulator, its numbers and functions
 * 
 * 
 * @author petr.babanov@softcom.su
 *
 */
public interface IRegisterModel {

	public IRegisterGroup[] getRegisterGroups();

	/**
	 * Get new register model representation instance with set values
	 * 
	 * @param values - list with length same as number of registers
	 * @return List of {@link org.eclipse.debug.core.model.IRegisterGroup
	 *         IRegisterGroup} contains registers with set values
	 * @throws IllegalArgumentException
	 *                                  <ul>
	 *                                  <li>size of {@code values} not same as
	 *                                  number of registers
	 *                                  <li>same value cannot be parsed as register
	 *                                  value
	 *                                  </ul>
	 * @throws DebugException
	 */
	public List<IRegisterGroup> getRegisters(List<String> values) throws IllegalArgumentException, DebugException;

	public void updateRegisters(List<String> values);

	public boolean isEmpty();

	public IRegister getRegister(String name);

	public IRegister getRegister(int number);

	/**
	 * get byte array representation for register value
	 * 
	 * @param name of register, case sensitive
	 * @return byte array or {@code null} if number not presented in register model
	 */
	public byte[] getValue(String name);

	/**
	 * get byte array representation for register value
	 * 
	 * @param number of array. May be got from
	 *               {@link IRegisterModel#getRegisterNumber(String)
	 *               getRegisterNumber} method
	 * @return byte array or {@code null} if number not presented in register model
	 */
	public byte[] getValue(int number);

	/**
	 * get exit code after terminate
	 * 
	 * @return exit code after program terminated
	 */
	public long getExitCode();

	/**
	 * Returns index of registers value in value array
	 * 
	 * @param name of register
	 * @return index of register or {@code -1} if not found
	 */
	public int getRegisterNumber(String name);

	/**
	 * Get value for register by name.
	 * 
	 * @param values
	 * @param name
	 * @return String value representation for passed register value
	 */
	public String getRegisterValue(List<String> values, String name);

	public int getRegisterSize(int number);

	public String getInstructurePointer(List<String> argValues);

	public String getStackPointer(List<String> regs);
}
