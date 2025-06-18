package su.softcom.cldt.ui;

import org.eclipse.jface.viewers.IDecoration;
import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.jface.viewers.ILightweightLabelDecorator;

import su.softcom.cldt.core.source.ISourceElement;

public class SourceElementDecorator implements ILightweightLabelDecorator {

	@Override
	public void addListener(ILabelProviderListener listener) {
		// empty
	}

	@Override
	public void dispose() {
		// empty
	}

	@Override
	public boolean isLabelProperty(Object element, String property) {
		return true;
	}

	@Override
	public void removeListener(ILabelProviderListener listener) {
		// empty
	}

	@Override
	public void decorate(Object element, IDecoration decoration) {
		if (element instanceof ISourceElement srcElem && srcElem.getDetail() != null) {
			decoration.addSuffix(" " + srcElem.getDetail()); //$NON-NLS-1$
		}
	}

}
