package su.softcom.cldt.debug.internal.core;

import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IDebugTarget;

import su.softcom.cldt.debug.core.DebugConstants;

public abstract class CLDTDebugElement extends DebugElement {

	public CLDTDebugElement(IDebugTarget target) {
		super(target);
	}

	@Override
	public String getModelIdentifier() {
		return DebugConstants.MODEL_IDENTIFIER;
	}
	
}
