package su.softcom.cldt.debug.internal.core;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IValue;
import org.eclipse.debug.core.model.IVariable;

public abstract class AbstractVariable extends CLDTDebugElement implements IVariable {
	
	protected IValue value;
	protected String name;

	public AbstractVariable(IDebugTarget target, String name, IValue value) {
		super(target);
		this.name = name;
		this.value = value;
	}


	@Override
	public void setValue(String expression) throws DebugException {
		// TODO Auto-generated method stub

	}

	@Override
	public void setValue(IValue value) throws DebugException {
		this.value = value;
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
		return value.getReferenceTypeName();
	}

	@Override
	public boolean hasValueChanged() throws DebugException {
		// TODO Auto-generated method stub
		return false;
	}

}

