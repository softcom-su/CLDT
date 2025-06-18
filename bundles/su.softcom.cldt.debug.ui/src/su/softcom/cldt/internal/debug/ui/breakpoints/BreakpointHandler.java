package su.softcom.cldt.internal.debug.ui.breakpoints;

import org.eclipse.core.commands.AbstractHandler;
import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.ui.actions.IToggleBreakpointsTarget;
import org.eclipse.jface.text.BadLocationException;
import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.IRegion;
import org.eclipse.jface.text.ITextSelection;
import org.eclipse.jface.text.TextSelection;
import org.eclipse.jface.text.source.IVerticalRulerInfo;
import org.eclipse.ui.IWorkbenchPart;
import org.eclipse.ui.handlers.HandlerUtil;
import org.eclipse.ui.texteditor.IDocumentProvider;
import org.eclipse.ui.texteditor.ITextEditor;

public class BreakpointHandler extends AbstractHandler {
	
	public BreakpointHandler() {		
		// TODO Auto-generated constructor stub
	}
	
	@Override
	public Object execute(ExecutionEvent event) throws ExecutionException {
		ITextEditor fEditor = BreakpointUtils.getEditor(HandlerUtil.getActiveEditor(event));
		IVerticalRulerInfo fRulerInfo = null;
		
		Object ruler = fEditor.getAdapter(IVerticalRulerInfo.class); 
        if (ruler instanceof IVerticalRulerInfo rulerInfo) {
        	fRulerInfo = rulerInfo;
        }
        
		if (fRulerInfo != null) {
			IDocumentProvider provider = fEditor.getDocumentProvider();
			ITextSelection selection = null;

			try {
				int line = fRulerInfo.getLineOfLastMouseButtonActivity();
				provider.connect(this);
				IDocument document = provider.getDocument(fEditor.getEditorInput());
				IRegion region = document.getLineInformation(line);
				selection = new TextSelection(document, region.getOffset(), region.getLength());			
			} catch (CoreException | BadLocationException e1) {
			}  finally {
				provider.disconnect(this);
			}
			
			if (selection != null) {
				IToggleBreakpointsTarget target = fEditor.getAdapter(IToggleBreakpointsTarget.class);
				try {
					if ((target != null) && (target.canToggleLineBreakpoints(fEditor, selection))) {
						target.toggleLineBreakpoints(fEditor, selection);
					}
				} catch (CoreException e) {
				}
			}
		}
		
		return null;
	}

}
