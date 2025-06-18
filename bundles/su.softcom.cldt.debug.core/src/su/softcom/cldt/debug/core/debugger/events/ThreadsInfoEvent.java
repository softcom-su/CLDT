package su.softcom.cldt.debug.core.debugger.events;


import java.util.List;
import su.softcom.cldt.debug.core.IDebuggerEvent;

public class ThreadsInfoEvent implements IDebuggerEvent {

	private List<Long> threadIds;
	private List<Long> stoppedThreads;

	
	public ThreadsInfoEvent(List<Long> threadIds, List<Long> stoppedThreads) {
		this.threadIds = threadIds;
		this.stoppedThreads = stoppedThreads;
	}
	
	public List<Long> getThreadIds() {
		return threadIds;
	}


	public List<Long> getStoppedThreads() {
		return stoppedThreads;
	}
	
}
