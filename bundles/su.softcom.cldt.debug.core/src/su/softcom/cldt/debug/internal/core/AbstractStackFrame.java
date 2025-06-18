package su.softcom.cldt.debug.internal.core;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.resources.IStorage;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IStackFrame;
import org.eclipse.debug.core.model.IThread;
import org.eclipse.debug.core.model.IVariable;

import su.softcom.cldt.debug.core.IRegisterModel;
import su.softcom.cldt.debug.core.info.IModelVariable;

public abstract class AbstractStackFrame extends CLDTDebugElement implements IStackFrame{
	
	protected long address;
	protected int lineNumber;
	protected final IThread thread;
	protected final List<IModelVariable> variables = new ArrayList<>();
	protected IStorage file;

	public AbstractStackFrame(IDebugTarget target, IThread thread) {
		super(target);
		this.thread = thread;
	}
	
	@Override
	public boolean isStepping() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void stepInto() throws DebugException {
		if(thread.getTopStackFrame() == this) {
			thread.stepInto();
		} else {
			thread.getTopStackFrame().stepInto();
		}
		

	}

	@Override
	public void stepOver() throws DebugException {
		if(thread.getTopStackFrame() == this) {
			thread.stepOver();
		} else {
			thread.getTopStackFrame().stepOver();
		}

	}

	@Override
	public void stepReturn() throws DebugException {
		if(thread.getTopStackFrame() == this) {
			thread.stepReturn();
		} else {
			thread.getTopStackFrame().stepReturn();
		}
	}

	@Override
	public boolean isSuspended() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void resume() throws DebugException {
		thread.resume();
	}

	@Override
	public void suspend() throws DebugException {
		thread.suspend();
	}

	@Override
	public boolean isTerminated() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void terminate() throws DebugException {
		thread.terminate();
	}

	@Override
	public IThread getThread() {
		// TODO Auto-generated method stub
		return thread;
	}

	@Override
	public IVariable[] getVariables() throws DebugException {
		return variables.toArray(new IVariable[variables.size()]);
	}

	@Override
	public boolean hasVariables() throws DebugException {
		return !variables.isEmpty(); 
	}

	@Override
	public int getCharStart() throws DebugException {
		// TODO Auto-generated method stub
		return -1;
	}

	@Override
	public int getCharEnd() throws DebugException {
		// TODO Auto-generated method stub
		return -1;
	}

	@Override
	public String getName() throws DebugException {
		return getFile().getName() + ", line " + getLineNumber();
	}

	protected void addVariable(AbstractVariable variable) {
		// TODO Auto-generated method stub
		
	}
	
	public long getCurrentInstruction() {
		return address;
	}
	
	public void addVariable(IModelVariable variable) {
		variables.add(variable);
	}
	
	/**
	 * @return IStorage associated with StackFrame or {@code null} if not presented
	 */
	public IStorage getFile() {
		return file;
	}
	
	public abstract IRegisterModel getRegisterModel();
}

