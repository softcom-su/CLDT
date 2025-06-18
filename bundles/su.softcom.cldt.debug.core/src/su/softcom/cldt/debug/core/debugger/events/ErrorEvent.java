package su.softcom.cldt.debug.core.debugger.events;

import su.softcom.cldt.debug.core.IDebuggerEvent;

//TODO сообщать в консоль код ошибки
public class ErrorEvent implements IDebuggerEvent {

	private String errorMessage ;
	
	public ErrorEvent(String errorMessage) {
		this.errorMessage = errorMessage;
	}
	
	public String getExitCode() {
		return errorMessage;
	}

	
}
