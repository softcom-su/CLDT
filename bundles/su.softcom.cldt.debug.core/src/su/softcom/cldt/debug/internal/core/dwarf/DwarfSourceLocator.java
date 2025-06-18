package su.softcom.cldt.debug.internal.core.dwarf;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.model.IPersistableSourceLocator;
import org.eclipse.debug.core.model.IStackFrame;
import su.softcom.cldt.debug.internal.core.AbstractStackFrame;

/**
 * Returns file from {@link su.softcom.cldt.debug.internal.core.AbstractStackFrame AbstractStackFrame} and {@code null} otherwise 
 * 
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public class DwarfSourceLocator implements IPersistableSourceLocator {

	@Override
	public Object getSourceElement(IStackFrame stackFrame) {
		if(stackFrame instanceof AbstractStackFrame asf) {
			return asf.getFile();
		}
		return null;
	}

	@Override
	public String getMemento() throws CoreException {
		return null;
	}

	@Override
	public void initializeFromMemento(String memento) throws CoreException {

	}

	@Override
	public void initializeDefaults(ILaunchConfiguration configuration) throws CoreException {

	}

}

