package su.softcom.cldt.debug.internal.core;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IStorage;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.IPath;
import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IRegisterGroup;
import org.eclipse.debug.core.model.IStackFrame;
import org.eclipse.debug.core.model.IThread;
import org.eclipse.debug.core.model.IVariable;

import su.softcom.cldt.debug.core.IRegisterModel;
import su.softcom.cldt.debug.core.IStackFrameDescription;
import su.softcom.cldt.debug.core.info.IModelVariable;
import su.softcom.cldt.debug.core.model.events.FetchVariablesEvent;
import su.softcom.cldt.debug.internal.core.dwarf.VariableDTO;
import su.softcom.cldt.debug.internal.core.model.ModelVariable;

public class StackFrame extends AbstractStackFrame implements IStackFrame {

	private int fLineNumber = 1;
	private long id = 0;
	private long currentAddress = 0;
	private Map<Integer, BigInteger> registers;
	private boolean fDirtyVariables = true;
	private boolean fDirtyRegisters = true;

	/**
	 * @param target
	 * @param thread
	 * @param lineNumber
	 * @param file
	 */
	public StackFrame(IDebugTarget target, IThread thread, int lineNumber, IStorage file) {
		super(target, thread);
		this.fLineNumber = lineNumber;
		this.file = file;
	}

	/**
	 * @param target
	 * @param thread
	 */
	public StackFrame(IDebugTarget target, IThread thread) {
		super(target, thread);
	}

	/**
	 * @param description
	 * @param target
	 * @param thread
	 * @return StackFrame по описанию
	 */
	public static StackFrame fromDescription(IStackFrameDescription description, IDebugTarget target, IThread thread) {
		long address = description.getCFA();
		IStorage sourceFile = findFile(description.getPath());
		StackFrame stackFrame = new StackFrame(target, thread, description.getLineNumber(), sourceFile);
		stackFrame.setId(address);
		stackFrame.setCurrentAddress(description.getIP());
		stackFrame.setRegisters(description.getRegisters());
		stackFrame.fireCreationEvent();
		return stackFrame;
	}

	/**
	 * @param lineNumber
	 */
	public void setLineNumber(int lineNumber) {
		fLineNumber = lineNumber;
	}

	@Override
	public int getLineNumber() {
		return fLineNumber;
	}

	@Override
	public boolean isStepping() {
		return thread.isStepping();
	}

	@Override
	public void stepInto() throws DebugException {
		if (thread.getTopStackFrame() == this) {
			thread.stepInto();
		} else {
			thread.getTopStackFrame().stepInto();
		}

	}

	@Override
	public void stepOver() throws DebugException {
		if (thread.getTopStackFrame() == this) {
			thread.stepOver();
		} else {
			thread.getTopStackFrame().stepOver();
		}

	}

	@Override
	public void stepReturn() throws DebugException {
		if (thread.getTopStackFrame() == this) {
			thread.stepReturn();
		} else {
			thread.getTopStackFrame().stepReturn();
		}
	}

	@Override
	public boolean isSuspended() {
		return thread.isSuspended();
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
		return thread.isTerminated();
	}

	@Override
	public void terminate() throws DebugException {
		getDebugTarget().terminate();
	}

	@Override
	public IThread getThread() {
		return thread;
	}

	@Override
	public synchronized IVariable[] getVariables() throws DebugException {
		if (fDirtyVariables) {
			fDirtyVariables = false;
			if (getDebugTarget() instanceof DebugTarget dTarget) {
				dTarget.addEvent(new FetchVariablesEvent(id, null, currentAddress));
			}
		}

		return variables.toArray(new IVariable[variables.size()]);
	}

	private void variablesResetState() {
		Deque<IModelVariable> stack = new ArrayDeque<>();

		for (IModelVariable variable : variables) {
			stack.addAll(variable.getChildrens());
		}
		while (!stack.isEmpty()) {
			IModelVariable currentVariable = stack.pop();
			currentVariable.setDirty(true);
			for (IModelVariable variable : currentVariable.getChildrens()) {
				stack.addAll(variable.getChildrens());
			}
		}

	}

	@Override
	public boolean hasVariables() throws DebugException {
		return !variables.isEmpty();
	}

	/**
	 * Добавить переменные
	 * 
	 * @param pathId
	 * @param variables
	 * @throws DebugException
	 */
	public synchronized void setVariables(List<Long> pathId, List<VariableDTO> variables) throws DebugException {
		if (pathId != null) {
			int i = 0;
			IModelVariable currentVariable = null;
			List<IModelVariable> currentVariables = this.variables;
			while (pathId.size() != i) {
				for (IModelVariable v : currentVariables) {
					if (v.getId() == pathId.get(i)) {
						currentVariable = v;
						currentVariables = v.getChildrens();
						i++;
						break;
					}
				}
			}
			if (currentVariable instanceof ModelVariable mVariable) {
				mVariable.updateVariables(variables);
			}
		} else {
			List<IModelVariable> resolvedVariables = new ArrayList<>();
			for (VariableDTO variableDTO : variables) {
				String name = variableDTO.getName();
				boolean processed = false;

				// try to find existing variable
				for (IVariable variable : this.variables) {
					if (variable instanceof ModelVariable v && v.getName() != null && v.getName().equals(name)
							&& v.getAddress() == variableDTO.getAddress().longValue()) {
						v.setValue(variableDTO.getValue());
						v.fireChangeEvent(DebugEvent.CONTENT);
						resolvedVariables.add(v);
						processed = true;
						break;
					}
				}

				if (!processed) {
					// not found, create new variable
					ModelVariable v = new ModelVariable(getDebugTarget(), variableDTO, id, null);
					this.variables.add(v);
					v.fireCreationEvent();
					resolvedVariables.add(v);
				}
			}
			this.variables.clear();
			this.variables.addAll(resolvedVariables);
		}
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
		// TODO: Добавить потом нормальное отображение имени фрейма.
		// Например, добавить имя функции.
		return getFile() != null ? getFile().getName() + ", line " + getLineNumber() : "Frame";
	}

	protected void addVariable(AbstractVariable variable) {
		// TODO Auto-generated method stub

	}

	public long getCurrentInstruction() {
		return address;
	}

//	public void addVariable(IVariable variable) {
//		variables.add(variable);
//	}

	/**
	 * @return IStorage associated with StackFrame or {@code null} if not presented
	 */
	@Override
	public IStorage getFile() {
		return this.file;
	}

	public void setFile(Path sourceFile) {
		if (sourceFile != null) {			
			this.file = findFile(sourceFile);
		}
	}

	@Override
	public IRegisterModel getRegisterModel() {
		return null;
	}

	@Override
	public boolean canStepInto() {
		return thread.isSuspended();
	}

	@Override
	public boolean canStepOver() {
		return thread.isSuspended();
	}

	@Override
	public boolean canStepReturn() {
		return thread.isSuspended();
	}

	@Override
	public boolean canResume() {
		return thread.canResume();
	}

	@Override
	public boolean canSuspend() {
		return false;
	}

	@Override
	public boolean canTerminate() {
		return true;
	}

	@Override
	public synchronized IRegisterGroup[] getRegisterGroups() throws DebugException {
		if (getDebugTarget() instanceof DebugTarget target) {
			return target.getfRegisterGroups().toArray(new IRegisterGroup[0]);
		}
		return new IRegisterGroup[0];
	}

	@Override
	public boolean hasRegisterGroups() throws DebugException {
		return getRegisterGroups().length > 0;
	}

	public Map<Integer, BigInteger> getRegisters() {
		return registers;
	}

	public void setRegisters(Map<Integer, BigInteger> registers) {
		this.registers = registers;
	}

	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public long getCurrentAddress() {
		return currentAddress;
	}

	public void setCurrentAddress(long currentAddress) {
		this.currentAddress = currentAddress;
	}

	@Override
	public synchronized void fireChangeEvent(int detail) {
		fDirtyVariables = true;
		fDirtyRegisters = true;
		variablesResetState();
		super.fireChangeEvent(detail);
	}

	private static IStorage findFile(Path path) {
		if (path != null) {
			IFile[] files = ResourcesPlugin.getWorkspace().getRoot().findFilesForLocationURI(path.toUri());
			if (files != null && files.length > 0) {
				return files[0];
			}
			if (Files.exists(path)) {
				File file = path.toFile();
				IPath p = new org.eclipse.core.runtime.Path(path.toString());
				try {
					return new FileStorage(new FileInputStream(file), p);
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				}
			}
		}
		return null;
	}
}
