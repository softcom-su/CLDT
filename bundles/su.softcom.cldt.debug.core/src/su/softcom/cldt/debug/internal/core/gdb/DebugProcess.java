package su.softcom.cldt.debug.internal.core.gdb;


import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.model.IProcess;
import org.eclipse.debug.core.model.IStreamsProxy;

import su.softcom.cldt.debug.internal.core.DebugTarget;


/**
 * Процесс отлаживаемой программы. 
 */
public class DebugProcess implements IProcess {

	private DebugTarget debugTarget;
	private DebugStreamProxy proxy;
	private boolean terminated;

	public DebugProcess(DebugTarget debugTarget) {
		this.debugTarget = debugTarget;
		this.proxy = new DebugStreamProxy();
	}
	
	
	@Override
	public <T> T getAdapter(Class<T> adapter) {
		return debugTarget.getAdapter(adapter);
	}

	@Override
	public boolean canTerminate() {
		return !isTerminated();
	}

	@Override
	public boolean isTerminated() {
		return terminated;
	}

	@Override
	public void terminate() throws DebugException {
		terminated = true;
		DebugPlugin.getDefault().fireDebugEventSet(new DebugEvent[] { new DebugEvent(this, DebugEvent.TERMINATE) });
		debugTarget.terminate();
		
	}

	@Override
	public String getLabel() {
		// TODO set pid here
		return "%s [Debug process]".formatted(debugTarget.getLaunch().getLaunchConfiguration().getName());
	}

	@Override
	public ILaunch getLaunch() {
		return debugTarget.getLaunch();
	}

	@Override
	public IStreamsProxy getStreamsProxy() {
		return proxy;
	}

	@Override
	public void setAttribute(String key, String value) {
		// empty
	}

	@Override
	public String getAttribute(String key) {
		return null;
	}

	@Override
	public int getExitValue() throws DebugException {
		return 0;
	}


}