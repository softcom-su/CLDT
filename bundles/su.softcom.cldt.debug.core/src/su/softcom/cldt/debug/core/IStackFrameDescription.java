package su.softcom.cldt.debug.core;

import java.math.BigInteger;
import java.nio.file.Path;
import java.util.Map;

public interface IStackFrameDescription {
	
	/**
	 * @return instruction pointer
	 */
	public long getIP();
	
	/**
	 * @return cfa
	 */
	public long getCFA();
	
	/**
	 * @return registers
	 */
	public Map<Integer, BigInteger> getRegisters();
	
	/**
	 * @return path to file
	 */
	public Path getPath();
	
	/**
	 * @return line number in file
	 */
	public int getLineNumber();
	
}

