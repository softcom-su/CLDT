package su.softcom.cldt.debug.internal.core.types;

import java.nio.file.Path;
import java.util.List;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.IVariable;

import su.softcom.cldt.debug.core.info.IHasVariables;
import su.softcom.cldt.debug.core.info.IType;

public class ArrayType implements IType {

	@Override
	public Path getFilePath() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int getLineNumber() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public List<IVariable> getVariables() throws DebugException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean hasVariables() throws DebugException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public IHasVariables getParent() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean hasParent() {
		// TODO Auto-generated method stub
		return false;
	}

	
}
