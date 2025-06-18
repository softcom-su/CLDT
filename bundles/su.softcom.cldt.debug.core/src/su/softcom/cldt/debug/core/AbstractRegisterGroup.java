package su.softcom.cldt.debug.core;

import java.util.List;

import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IRegister;
import org.eclipse.debug.core.model.IRegisterGroup;

public abstract class AbstractRegisterGroup extends DebugElement implements IRegisterGroup {

	protected final String name;
	protected List<IRegister> registers;

	public AbstractRegisterGroup(IDebugTarget target, String name) {
		super(target);
		this.name = name;
	}

	@Override
	public <T> T getAdapter(Class<T> adapter) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public IRegister[] getRegisters() throws DebugException {
		return registers.toArray(new IRegister[0]);	
	}

	public boolean haveRegister(String name) {
		return getRegister(name) != null;
	}

	@Override
	public boolean hasRegisters() throws DebugException {
		return !registers.isEmpty();
	}

	@Override
	public String getModelIdentifier() {
		return getDebugTarget().getModelIdentifier();
	}
	
	public IRegister getRegister(String name) {
		for(IRegister register: registers) {
			try {
				if (register.getName().equals(name)) {
					return register;
				}
			} catch (DebugException e) {
				Platform.getLog(getClass()).error(e.getLocalizedMessage());
			}
		}
		return null;
	}
}
