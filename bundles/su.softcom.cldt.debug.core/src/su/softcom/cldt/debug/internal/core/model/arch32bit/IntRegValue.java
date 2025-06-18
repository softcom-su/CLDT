package su.softcom.cldt.debug.internal.core.model.arch32bit;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IValue;
import org.eclipse.debug.core.model.IVariable;

public final class IntRegValue extends DebugElement implements IValue {

	private final int fValue;

	public IntRegValue(IDebugTarget target, int value) {
		super(target);
		fValue = value;
	}

	@Override
	public String getReferenceTypeName() throws DebugException {
		return "integer type";
	}

	@Override
	public String getValueString() throws DebugException {
		return "0x" + Integer.toHexString(fValue);
	}

	@Override
	public boolean isAllocated() throws DebugException {
		return true;
	}

	@Override
	public IVariable[] getVariables() throws DebugException {
		return new IVariable[0];
	}

	@Override
	public boolean hasVariables() throws DebugException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean equals(Object o) {
		try {
			if ((o instanceof IntRegValue) && (((IntRegValue) o).getValueString().equals(getValueString()))) {
				return true;
			}
		} catch (DebugException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}

	@Override
	public String getModelIdentifier() {
		return getDebugTarget().getModelIdentifier();
	}

}
