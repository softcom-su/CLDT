package su.softcom.cldt.debug.core;

import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspaceRunnable;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.model.IBreakpoint;
import org.eclipse.debug.core.model.LineBreakpoint;

import su.softcom.cldt.debug.internal.core.model.Module;

/**
 * Command Line breakpoint 
 * 
 * @author maxim.drumov@softcom.su
 * <br> Edited: rinat.nizamov@softcom.su
 */
public class CommandLineBreakpoint extends LineBreakpoint {
	
	private Module module = null;
	/**
	  * Default constructor needed by the Eclipse debug framework.
	  */
	public CommandLineBreakpoint() {
	}

	public CommandLineBreakpoint(final IResource resource, final int lineNumber) throws CoreException {
		this(resource, lineNumber, true);
	}

	protected CommandLineBreakpoint(final IResource resource, final int lineNumber, final boolean persistent) throws CoreException {
		IWorkspaceRunnable runnable = new IWorkspaceRunnable() {
			@Override
			public void run(IProgressMonitor monitor) throws CoreException {
				IMarker marker = resource.createMarker(DebugConstants.LINE_BREAKPOINT_MARKER);
				marker.setAttribute(IBreakpoint.ENABLED, true);
				marker.setAttribute(IBreakpoint.PERSISTED, persistent);
				marker.setAttribute(IMarker.LINE_NUMBER, lineNumber);
				marker.setAttribute(IBreakpoint.ID, getModelIdentifier()); 
				marker.setAttribute(IMarker.MESSAGE, "Line Breakpoint: " + resource.getName() + " [line: " + lineNumber + "]");
				setMarker(marker);
			}
		};
		run(getMarkerRule(resource), runnable);
	}
	


	@Override
	public void setEnabled(boolean enabled) throws CoreException {
		if (enabled != isEnabled()) {
			setAttribute(ENABLED, enabled);
			if (isTriggerPoint()) {
				DebugPlugin.getDefault().getBreakpointManager().refreshTriggerpointDisplay();
			}
		}
	}
	
	/**
	 * @return associated resource
	 */
	public IResource getResource() {
		return getMarker().getResource();
	}

	@Override
	public String getModelIdentifier() {
		return DebugConstants.MODEL_IDENTIFIER;
	}

	public Module getModule() {
		return module;
	}

	public void setModule(Module linkedModule) {
		this.module = linkedModule;
	}


}