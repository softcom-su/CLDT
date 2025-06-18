package su.softcom.cldt.debug.core.debugger.events;

import su.softcom.cldt.debug.core.IDebuggerEvent;

public class TerminatedEvent implements IDebuggerEvent {

	private int exitCode = -1;
	
	public TerminatedEvent(int exitCode) {
		this.exitCode = exitCode;
	}
	
	public int getExitCode() {
		return exitCode;
	}

}
