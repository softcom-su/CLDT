package su.softcom.cldt.internal.debug.ui.breakpoints;

import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.model.IBreakpoint;
import org.eclipse.debug.core.model.ILineBreakpoint;
import org.eclipse.debug.ui.actions.IToggleBreakpointsTarget;
import org.eclipse.jface.text.ITextSelection;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.ui.IWorkbenchPart;
import org.eclipse.ui.texteditor.ITextEditor;

import su.softcom.cldt.debug.core.DebugConstants;
import su.softcom.cldt.debug.core.CommandLineBreakpoint;
import su.softcom.cldt.debug.internal.core.DebugTarget;
import su.softcom.cldt.debug.internal.core.gdb.commands.BreakpointCommand.BreakpointAction;
import su.softcom.cldt.debug.core.model.events.BreakpointEvent;

public class BreakpointTarget implements IToggleBreakpointsTarget {
	
	public BreakpointTarget() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public void toggleLineBreakpoints(IWorkbenchPart part, ISelection selection) throws CoreException, DebugException {
		ITextEditor textEditor = BreakpointUtils.getEditor(part);
		if (textEditor != null) {
			IResource resource = textEditor.getEditorInput().getAdapter(IResource.class);
			ITextSelection textSelection = (ITextSelection) selection;
			int lineNumber = textSelection.getStartLine();
			
			DebugTarget client = BreakpointUtils.getDebugTarget();
			IBreakpoint[] breakpoints = DebugPlugin.getDefault().getBreakpointManager().getBreakpoints(DebugConstants.MODEL_IDENTIFIER);
			for (int i = 0; i < breakpoints.length; i++) {
				IBreakpoint breakpoint = breakpoints[i];
				if (resource.equals(breakpoint.getMarker().getResource()) && ((ILineBreakpoint) breakpoint).getLineNumber() == (lineNumber + 1)) {
						if (client != null) {
							BreakpointEvent event = new BreakpointEvent(BreakpointAction.REMOVE, breakpoint, ((ILineBreakpoint) breakpoint).getLineNumber());
							client.addEvent(event);
						}
						
						breakpoint.delete();
						return;
					}
				
			}
			
			CommandLineBreakpoint lineBreakpoint = new CommandLineBreakpoint(resource, lineNumber + 1);
			if (client != null) {
				BreakpointEvent event = new BreakpointEvent(BreakpointAction.ADD, lineBreakpoint, lineBreakpoint.getLineNumber());
				client.addEvent(event);
			}
			
			DebugPlugin.getDefault().getBreakpointManager().addBreakpoint(lineBreakpoint);
		}
	}
	
	@Override
	public boolean canToggleLineBreakpoints(IWorkbenchPart part, ISelection selection) {
		return true;
	}

	@Override
	public void toggleMethodBreakpoints(IWorkbenchPart part, ISelection selection) throws CoreException {
		throw new RuntimeException("toggleMethodBreakpoints() not supported");
	}

	@Override
	public boolean canToggleMethodBreakpoints(IWorkbenchPart part, ISelection selection) {
		return false;
	}

	@Override
	public void toggleWatchpoints(IWorkbenchPart part, ISelection selection) throws CoreException {
		throw new RuntimeException("toggleWatchpoints() not supported");
	}

	@Override
	public boolean canToggleWatchpoints(IWorkbenchPart part, ISelection selection) {
		return false;
	}

}
