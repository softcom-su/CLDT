package su.softcom.cldt.debug.core.debugger.events;

import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IRegister;
import org.eclipse.debug.core.model.IRegisterGroup;
import org.eclipse.debug.core.model.IValue;

import su.softcom.cldt.debug.core.IDebuggerEvent;
import su.softcom.cldt.debug.internal.core.DebugThread;
import su.softcom.cldt.debug.internal.core.dwarf.RegisterDTO;
import su.softcom.cldt.debug.internal.core.gdb.commands.FeaturesReadCommand.Target;
import su.softcom.cldt.debug.internal.core.gdb.commands.FeaturesReadCommand.Target.Reg;


public class LaunchedProcessEvent implements IDebuggerEvent {

	//TODO: add more info, if needed
	private List<Long> debugThreadIds;
	private Map<String, List<RegisterDTO>> registers = null;
	
	public LaunchedProcessEvent(List<Long> debugThreadIds) {
		this.debugThreadIds = debugThreadIds;
	}
	
	public LaunchedProcessEvent(Target target) {
		this.registers = groupRegistersByGroup(target);
	}
	
    private static Map<String, List<RegisterDTO>> groupRegistersByGroup(Target target) {
        if (target.getFeature() == null) {
            return Collections.emptyMap();
        }

        return target.getFeature().stream()
                .collect(Collectors.groupingBy(Reg::getGroup,
                        Collectors.mapping(
                                reg -> new RegisterDTO(reg.getName(), reg.getBitSize(), reg.getRegnum()),
                                Collectors.toList())));
    }
	
	public List<Long> getDebugThreadIds() {
		return debugThreadIds;
	}

	public Map<String, List<RegisterDTO>> getRegisters() {
		return registers;
	}
	
}
