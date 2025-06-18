package su.softcom.cldt.debug.core.debugger.events;

import java.math.BigInteger;
import java.util.List;
import java.util.Map;

import su.softcom.cldt.debug.core.IDebuggerEvent;
import su.softcom.cldt.debug.core.IStackFrameDescription;

public class SuspendedEvent implements IDebuggerEvent {

	private final long address;
	private final long threadID;
	private final List<IStackFrameDescription> stackFrames;
	private final Map<Integer, BigInteger> registers;
	private final StopReason stopReason;

	public enum StopReason {
		UNKNOWN, BREAKPOINT, STEP, EXCEPTION
	}

	public SuspendedEvent(long address, List<IStackFrameDescription> stackFrames, Map<Integer, BigInteger> regs,
			StopReason stopReason, long threadID) {
		this.threadID = threadID;
		this.address = address;
		this.stackFrames = stackFrames;
		this.registers = regs;
		this.stopReason = stopReason;
	}

	public long getAddress() {
		return address;
	}

	public long getThreadID() {
		return threadID;
	}

	public Map<Integer, BigInteger> getRegs() {
		return this.registers;
	}

	public StopReason getStopReason() {
		return stopReason;
	}

	public List<IStackFrameDescription> getStackFrames() {
		return stackFrames;
	}

}
