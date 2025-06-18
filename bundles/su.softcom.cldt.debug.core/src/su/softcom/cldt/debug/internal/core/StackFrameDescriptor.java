package su.softcom.cldt.debug.internal.core;

import java.math.BigInteger;
import java.nio.file.Path;
import java.util.Map;

import su.softcom.cldt.debug.core.IStackFrameDescription;

/**
 * Internal implementation for
 * {@link su.softcom.cldt.debug.core.IStackFrameDescription
 * IStackFrameDescription}
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 * @author maxim.drumov
 */
public class StackFrameDescriptor implements IStackFrameDescription {

	private final Map<Integer, BigInteger> registers;
	private final long ip;
	private final long cfa;
	private final Path path;
	private final int lineNumber;

	/**
	 * @param registers
	 * @param cfa
	 * @param ip
	 * @param path
	 * @param lineNumber
	 */
	public StackFrameDescriptor(Map<Integer, BigInteger> registers, long cfa, long ip, Path path, int lineNumber) {
		this.registers = registers;
		this.ip = ip;
		this.cfa = cfa;
		this.path = path;
		this.lineNumber = lineNumber;
	}

	@Override
	public long getIP() {
		return ip;
	}

	@Override
	public long getCFA() {
		return cfa;
	}

	@Override
	public Map<Integer, BigInteger> getRegisters() {
		return registers;
	}

	@Override
	public Path getPath() {
		return path;
	}

	@Override
	public int getLineNumber() {
		return lineNumber;
	}

}
