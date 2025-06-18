package su.softcom.cldt.debug.internal.core.model;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IValue;
import org.eclipse.debug.core.model.IVariable;

import su.softcom.cldt.debug.core.info.IHasVariables;
import su.softcom.cldt.debug.core.info.IModelVariable;
import su.softcom.cldt.debug.internal.core.dwarf.VariableDTO;

public class ModelVariable extends DebugElement implements IModelVariable, IHasVariables {

	private long id;
	private long address;
	private boolean hasChild;
	private String name;
	private ModelVariableType type;
	private ModelValue value;
	private ModelVariable parent;
	private boolean isDirty = true;
	private boolean isPhantom = false;
	private long stackFrameId;

	/**
	 * Конструктор для модели переменной
	 * 
	 * @param target
	 * @param desc
	 * @param stackFrameId
	 * @param parent
	 */
	public ModelVariable(IDebugTarget target, VariableDTO desc, long stackFrameId, ModelVariable parent) {
		super(target);
		this.id = desc.getId();
		this.name = desc.getName();
		this.address = desc.getAddress().longValue();
		this.type = desc.getType();
		this.hasChild = desc.isHasChild();
		this.stackFrameId = stackFrameId;
		this.parent = parent;
		value = new ModelValue(getDebugTarget(), this, desc.getValue());
	}

	public ModelVariable(IDebugTarget target) {
		super(target);
		this.hasChild = false;
		this.isPhantom = true;
		value = new ModelValue(getDebugTarget(), this, null);
	}

	@Override
	public String getModelIdentifier() {
		return getDebugTarget().getModelIdentifier();
	}

	@Override
	public void setValue(String expression) throws DebugException {
		// TODO Auto-generated method stub

	}

	@Override
	public void setValue(IValue value) throws DebugException {
		// TODO Auto-generated method stub

	}

	@Override
	public boolean supportsValueModification() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean verifyValue(String expression) throws DebugException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean verifyValue(IValue value) throws DebugException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public IValue getValue() throws DebugException {
		return value;
	}

	@Override
	public String getName() throws DebugException {
		return name;
	}

	@Override
	public String getReferenceTypeName() throws DebugException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean hasValueChanged() throws DebugException {
		// TODO Auto-generated method stub
		return false;
	}

	public long getAddress() {
		return address;
	}

	@Override
	public IHasVariables getParent() {
		return parent;
	}

	@Override
	public boolean hasVariables() throws DebugException {
		return value.hasVariables();
	}

	@Override
	public boolean hasParent() {
		return parent != null;
	}

	@Override
	public List<IVariable> getVariables() {
		return value.getChildrens().stream().filter(IVariable.class::isInstance).map(IVariable.class::cast)
				.collect(Collectors.toList());
	}

	@Override
	public void setValue(Object newValue) {
		value.setData(newValue);
		isDirty = false;
	}

	@Override
	public List<IModelVariable> getChildrens() {
		List<ModelVariable> vars = value.getChildrens();
		List<IModelVariable> iModelVariables = new ArrayList<>(vars.size());
		for (ModelVariable v : vars) {
			if (v instanceof IModelVariable) {
				iModelVariables.add(v);
			}
		}
		return iModelVariables;
	}

	/**
	 * @return если есть потомки true, иначе false
	 */
	public boolean isHasChild() {
		return hasChild;
	}

	/**
	 * @return тип переменной
	 */
	public ModelVariableType getType() {
		return type;
	}

	/**
	 * @return идентификатор переменной
	 */
	public long getId() {
		return id;
	}

	/**
	 * @return идентификатор стекового кадра, в котором находится переменная (Пока
	 *         планирую добавить эту информация только у рут переменной. Может и для
	 *         одинаковых переменных завести какую-то мапу и переиспользовать
	 *         обьекты переменных модели)
	 * 
	 */
	public long getStackFrameId() {
		return stackFrameId;
	}

	public void updateVariables(List<VariableDTO> variables) throws DebugException {
		for (VariableDTO variableDTO : variables) {
			String varName = variableDTO.getName();
			boolean processed = false;

			// try to find existing variable
			for (IVariable variable : getChildrens()) {
				if (variable instanceof ModelVariable v && v.isPhantom()) {
					continue;
				}

				if (variable instanceof ModelVariable v && v.getName().equals(varName)
						&& v.getAddress() == variableDTO.getAddress().longValue()) {
					v.setValue(variableDTO.getValue());
					v.fireChangeEvent(DebugEvent.CONTENT);

					processed = true;
					break;
				}
			}

			if (!processed) {
				// not found, create new variable
				ModelVariable textVariable = new ModelVariable(getDebugTarget(), variableDTO, 0, this);
				value.addChildren(textVariable);
				textVariable.fireCreationEvent();
			}
		}
	}

	public boolean isDirty() {
		return isDirty;
	}

	public void setDirty(boolean isDirty) {
		this.isDirty = isDirty;
	}

	public boolean isPhantom() {
		return isPhantom;
	}

}
