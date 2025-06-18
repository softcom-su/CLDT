package su.softcom.cldt.debug.internal.core;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.Job;

import su.softcom.cldt.debug.core.IDebugEventBus;
import su.softcom.cldt.debug.core.IDebuggerEvent;
import su.softcom.cldt.debug.core.IEvent;
import su.softcom.cldt.debug.core.IEventListener;
import su.softcom.cldt.debug.core.IModelEvent;

public class DebugEventBus extends Job implements IDebugEventBus {

	final Queue<IEvent> eventQueue = new ConcurrentLinkedQueue<>();
	
	/**
	 * model = GDBDebugTarget
	 */
	IEventListener model;
	/**
	 * debugger = GDBClient
	 */
	IEventListener debugger;
	
	boolean terminate = false;
	
	public DebugEventBus(String name) {
		super("Debug event bus for process: %s".formatted(name));
	}

	@Override
	protected IStatus run(IProgressMonitor monitor) {
		Assert.isNotNull(model, "The 'model' must be set before EventBus");
		Assert.isNotNull(debugger, "The 'debugger' must be set before EventBus");
		while(!monitor.isCanceled() && !terminate) {
			if(!eventQueue.isEmpty()) {
				IEvent event =  eventQueue.poll();
				if (event instanceof IModelEvent) {
					model.handleEvent(event);
				} else if (event instanceof IDebuggerEvent) {
					debugger.handleEvent(event);
				}
			}
		}
		return Status.OK_STATUS;
	}

	public void addEvent(IEvent event) {
		eventQueue.add(event);
	}
	
	public void setDebugger(IEventListener debugger) {
		this.debugger = debugger;
	}
	
	public void setModel(IEventListener model) {
		this.model = model;
	}

	@Override
	public void start() {
		schedule();
	}

	@Override
	public void stop() {
		terminate = true;
	}
	
	
}
