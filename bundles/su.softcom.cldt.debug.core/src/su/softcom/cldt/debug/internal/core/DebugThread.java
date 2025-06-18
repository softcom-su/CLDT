package su.softcom.cldt.debug.internal.core;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.model.IBreakpoint;
import org.eclipse.debug.core.model.IStackFrame;
import org.eclipse.debug.core.model.IThread;

import su.softcom.cldt.debug.core.model.events.ResumeEvent;
import su.softcom.cldt.debug.core.model.events.ResumeEvent.ResumeState;

public class DebugThread extends CLDTDebugElement implements IThread {

	final long id;
	private State state;
	private final Map<Long, IStackFrame> stackFrames = new LinkedHashMap<>();

	public DebugThread(DebugTarget target, long threadID) {
		super(target);
		this.id = threadID;
	}

	@Override
	public boolean canResume() {
		return isSuspended();
	}

	@Override
	public boolean canSuspend() {
		return false;
	}

	@Override
	public boolean isSuspended() {
		return state == State.SUSPENDED;
	}

	@Override
	public void resume() throws DebugException {
		DebugTarget target = (DebugTarget) getDebugTarget();
		target.addEvent(new ResumeEvent(ResumeState.RUN, this.getId()));
		fireResumeEvent(DebugEvent.RESUME);
	}

	@Override
	public void suspend() throws DebugException {
		// TODO Auto-generated method stub

	}

	@Override
	public boolean canStepInto() {
		return isSuspended();
	}

	@Override
	public boolean canStepOver() {
		return isSuspended();
	}

	@Override
	public boolean canStepReturn() {
		return isSuspended();
	}

	@Override
	public boolean isStepping() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void stepInto() throws DebugException {
		if (getDebugTarget() instanceof DebugTarget debugTarget) {
			debugTarget.addEvent(new ResumeEvent(ResumeState.STEP_INTO, id));
			fireResumeEvent(DebugEvent.RESUME);
		}
		state = State.RESUMED;
	}

	@Override
	public void stepOver() throws DebugException {
		if (getDebugTarget() instanceof DebugTarget debugTarget) {
			debugTarget.addEvent(new ResumeEvent(ResumeState.STEP_OVER, id));
			fireResumeEvent(DebugEvent.RESUME);
		}
		state = State.RESUMED;
	}

	@Override
	public void stepReturn() throws DebugException {
		if (getDebugTarget() instanceof DebugTarget debugTarget) {
			debugTarget.addEvent(new ResumeEvent(ResumeState.STEP_RETURN, id));
			fireResumeEvent(DebugEvent.RESUME);
		}
		state = State.RESUMED;
	}

	@Override
	public boolean canTerminate() {
		return false;
		// return !isTerminated(); HOW?
		// TODO: terminate exact thread?
	}

	@Override
	public boolean isTerminated() {
		return state == State.TERMINATED;
	}

	@Override
	public void terminate() throws DebugException {
		stackFrames.clear();
		state = State.TERMINATED;
		// TODO: terminate exact thread?
	}

	@Override
	public IStackFrame[] getStackFrames() throws DebugException {
		return stackFrames.values().toArray(new IStackFrame[stackFrames.size()]);
	}

	/**
	 * Возвращает стекфрейм по его id
	 * 
	 * @param stackFrameId
	 * @return stackFrame
	 */
	public IStackFrame getStackFrame(long stackFrameId) {
		return stackFrames.get(stackFrameId);
	}

	/**
	 * Temporary decision
	 */
	public void clearStackFrames() {
		stackFrames.clear();
	}

	@Override
	public boolean hasStackFrames() throws DebugException {
		return getStackFrames().length > 0;
	}

	@Override
	public int getPriority() throws DebugException {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * @param stackFrames
	 */
	public void setStackFrames(List<StackFrame> stackFrames) {
		clearStackFrames();
		for (StackFrame stackFrame : stackFrames) {
			this.stackFrames.put(stackFrame.getId(), stackFrame);
		}
	}

	@Override
	public IStackFrame getTopStackFrame() throws DebugException {
		return !stackFrames.isEmpty() ? stackFrames.values().iterator().next() : null;
	}

	@Override
	public String getName() throws DebugException {
		return "Thread ID:%s".formatted(Long.toHexString(id));
	}

	@Override
	public IBreakpoint[] getBreakpoints() {
		return DebugPlugin.getDefault().getBreakpointManager().getBreakpoints(getModelIdentifier());
	}

	public long getId() {
		return id;
	}

	public State getState() {
		return state;
	}

	public void setState(State threadState) {
		this.state = threadState;
		fireChangeEvent(DebugEvent.CHANGE);
	}

}
