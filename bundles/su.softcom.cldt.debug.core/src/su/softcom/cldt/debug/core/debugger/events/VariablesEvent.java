package su.softcom.cldt.debug.core.debugger.events;

import java.util.List;

import su.softcom.cldt.debug.core.IDebuggerEvent;
import su.softcom.cldt.debug.internal.core.dwarf.VariableDTO;

public class VariablesEvent implements IDebuggerEvent {

	private final List<VariableDTO> variables;
	private final long stackFrameId;
	private final List<Long> pathId;

	/**
	 * @param variables
	 * @param pathId
	 * @param stackFrameId
	 */
	public VariablesEvent(List<VariableDTO> variables, List<Long> pathId, long stackFrameId) {
		this.variables = variables;
		this.stackFrameId = stackFrameId;
		this.pathId = pathId;
	}

	/**
	 * @return variables
	 */
	public List<VariableDTO> getVariables() {
		return variables;
	}

	/**
	 * @return threadId
	 */
	public long getStackFrameId() {
		return stackFrameId;
	}

	/**
	 * Путь из идентификаторов переменных, чтобы восстановить местонахождение
	 * переменных, которые сейчас прочитали
	 * 
	 * @return pathId
	 */
	public List<Long> getPathId() {
		return pathId;
	}

}
