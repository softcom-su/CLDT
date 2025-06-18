package su.softcom.cldt.debug.internal.core.model.arch32bit;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IRegister;
import org.eclipse.debug.core.model.IRegisterGroup;
import org.eclipse.debug.core.model.IValue;

public final class IntRegister extends DebugElement implements IRegister {

	private final String fName;
	private IntRegValue fValue;
	private boolean fDirty = true;

	public IntRegister(IDebugTarget target, String regName, int regValue) {
		super(target);
		fName = regName;
		fValue = new IntRegValue(target, regValue);
	}

	@Override
	public IValue getValue() throws DebugException {
		return fValue;
	}

	@Override
	public String getName() throws DebugException {
		return fName;
	}

	@Override
	public String getReferenceTypeName() throws DebugException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean hasValueChanged() throws DebugException {
		// TODO Auto-generated method stub
		return fDirty;
	}

	@Override
	public void setValue(String expression) throws DebugException {
//		try {
//			getDebugTarget().fireModelEvent(new ChangeRegisterRequest(fName, Integer.parseInt(expression, 16)));
//		} catch (Exception e) {
//			throw new DebugException(new Status(IStatus.ERROR, Activator.getDefault().getBundle().getSymbolicName(),
//					"Error in parsing register value"));
//		}

	}

	@Override
	public void setValue(IValue value) throws DebugException {
		if (value instanceof IntRegValue) {
			if (fValue.equals(value)) {
				fDirty = false;
			} else {
				fDirty = true;
			}
			fValue = (IntRegValue) value;
		}
	}

	@Override
	public boolean supportsValueModification() {
		return true;
	}

	@Override
	public boolean verifyValue(String expression) throws DebugException {
		try {
			Integer.parseInt(expression, 16);
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	@Override
	public boolean verifyValue(IValue value) throws DebugException {
		return (value instanceof IntRegValue);
	}

	@Override
	public IRegisterGroup getRegisterGroup() throws DebugException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getModelIdentifier() {
		return getDebugTarget().getModelIdentifier();
	}

}

