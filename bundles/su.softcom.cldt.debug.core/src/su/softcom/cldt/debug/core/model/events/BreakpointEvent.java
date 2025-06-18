package su.softcom.cldt.debug.core.model.events;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.resources.IResource;
import org.eclipse.debug.core.model.IBreakpoint;

import su.softcom.cldt.debug.core.IModelEvent;
import su.softcom.cldt.debug.internal.core.gdb.commands.BreakpointCommand.BreakpointAction;

public class BreakpointEvent implements IModelEvent {

	private List<Long> bpAddresses = new ArrayList<>();
	private IBreakpoint bp;
	private int lineNumber;
	private BreakpointAction action;
	
	public BreakpointEvent(BreakpointAction action, IBreakpoint bp, int lineNumber) {
		this.bp = bp;
		this.lineNumber = lineNumber;
		this.action = action;
	}
	
	public BreakpointAction getAction() {
		return action;
	}
	
	public List<Long> getAddresses() {
		return bpAddresses;	
	}
	
	public IBreakpoint getBreakpoint() {
		return this.bp;
	}
	
	public int getLineNumber() {
		return this.lineNumber;
	}
}
