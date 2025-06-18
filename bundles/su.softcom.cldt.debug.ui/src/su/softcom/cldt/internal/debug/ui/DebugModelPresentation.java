package su.softcom.cldt.internal.debug.ui;

import java.util.HashMap;
import java.util.Map;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IStorage;
import org.eclipse.debug.core.model.IValue;
import org.eclipse.debug.ui.IDebugModelPresentation;
import org.eclipse.debug.ui.IValueDetailListener;
import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.ui.IEditorDescriptor;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.part.FileEditorInput;

import su.softcom.cldt.debug.core.CommandLineBreakpoint;
import su.softcom.cldt.debug.internal.core.FileStorage;

public class DebugModelPresentation implements IDebugModelPresentation {
	
	private final Map<String, Object> attributes = new HashMap<>();

	@Override
	public void addListener(ILabelProviderListener listener) {
		// TODO Auto-generated method stub

	}

	@Override
	public void dispose() {
		// TODO Auto-generated method stub

	}

	@Override
	public boolean isLabelProperty(Object element, String property) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void removeListener(ILabelProviderListener listener) {
		// TODO Auto-generated method stub

	}

	@Override
	public IEditorInput getEditorInput(Object element) {
		if (element instanceof IFile file) {
			return new FileEditorInput(file);
		} else if(element instanceof CommandLineBreakpoint bp && bp.getResource() instanceof IFile file) {
			return new FileEditorInput(file);
		} else if (element instanceof FileStorage file) {
			return new ExternalEditorInput(file, file.getFullPath());
		}
		
		return null;
	}
	
	private IEditorDescriptor getIdFromFile (IStorage file) {
		return PlatformUI.getWorkbench().getEditorRegistry().getDefaultEditor(file.getName());
	}
	
	@Override
	public String getEditorId(IEditorInput input, Object element) {
		IStorage storage = null;
		IEditorDescriptor editor;
		if (element instanceof IFile file) {
			storage = file;
		} else if(element instanceof CommandLineBreakpoint bp && bp.getResource() instanceof IFile file) {
			storage = file;
		} else if (element instanceof FileStorage file) {
			storage = file;
		}

		if (storage != null && (editor = getIdFromFile(storage)) != null ) {
			return editor.getId();
		}
		
		return "org.eclipse.ui.genericeditor.GenericEditor";
	}

	@Override
	public void setAttribute(String attribute, Object value) {
		if (value == null) {
			return;
		}
		synchronized (attributes) {
			attributes.put(attribute, value);
		}
	}

	@Override
	public Image getImage(Object element) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getText(Object element) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void computeDetail(IValue value, IValueDetailListener listener) {
		// TODO Auto-generated method stub

	}

}
