package su.softcom.cldt.debug.core.debugger.events;

import su.softcom.cldt.debug.core.IDebuggerEvent;

/**
 * Эвент, сообщающий таргету информацию, которую нужно вывести в консоль.
 * Флаг isErrorStream управляет потоком.
 */
public class ConsoleOutputEvent  implements IDebuggerEvent {

	private String content = "";
	private boolean isErrorStream = false;
	
	public ConsoleOutputEvent(String str, boolean isErrorStream) {
		this.content = str;
		this.isErrorStream = isErrorStream;
	}

	public String getContent() {
		return content;
	}
	
	public boolean isErrorStream() {
		return isErrorStream;
	}
	
}
