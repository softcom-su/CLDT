package su.softcom.cldt.debug.core.info;

import java.util.List;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.IVariable;

/**
 * IType, IFunction
 * 
 */
public interface IHasVariables {

	public List<IVariable> getVariables() throws DebugException;
	
	public boolean hasVariables() throws DebugException;
	
}
