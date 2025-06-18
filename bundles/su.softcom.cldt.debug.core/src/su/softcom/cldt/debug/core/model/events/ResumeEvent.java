package su.softcom.cldt.debug.core.model.events;

import su.softcom.cldt.debug.core.IModelEvent;

public class ResumeEvent implements IModelEvent {
	
	private long threadId = -1;
	private ResumeState state;
	
	public enum ResumeState {
		STEP_OVER,
		STEP_INTO,
		STEP_RETURN,
		CONTINUE,
		RUN
	}
	
	// FOR all threads resume
	public ResumeEvent(ResumeState state) {
		this.state = state;
	}
	
	// FOR exact thread resume
	public ResumeEvent(ResumeState state, long threadId) {
		this.state = state;
		this.threadId = threadId;
	}
	
	public long getThreadId() {
		return threadId;
	}

	public ResumeState getState () {
		return this.state;
	}
}
