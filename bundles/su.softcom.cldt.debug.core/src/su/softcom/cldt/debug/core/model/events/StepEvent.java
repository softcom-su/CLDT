package su.softcom.cldt.debug.core.model.events;

import org.eclipse.core.runtime.Assert;

import su.softcom.cldt.debug.core.IModelEvent;
import su.softcom.cldt.debug.internal.core.DebugThread;

public class StepEvent implements IModelEvent {

	private long threadId = -1;
	
	// FOR all threads resume
	public StepEvent() {
	}
	
	// FOR exact thread resume
	public StepEvent(long threadId) {
		this.threadId = threadId;
	}
	
	public long getThreadId() {
		return threadId;
	}
	
}
