package su.softcom.cldt.debug.internal.core.model;

import java.util.List;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IRegister;
import org.eclipse.debug.core.model.IRegisterGroup;

import su.softcom.cldt.debug.core.AbstractRegisterGroup;

public class RegisterGroup extends AbstractRegisterGroup implements IRegisterGroup {
		
	public RegisterGroup(IDebugTarget target, String name, List<IRegister> regs) {
		super(target, name);
		registers = regs;
	}
	
	@Override
	public String getName() throws DebugException {
		return name;
	}

}
