package su.softcom.cldt.internal.debug.ui.breakpoints;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.ui.actions.IToggleBreakpointsTarget;
import org.eclipse.debug.ui.actions.RulerToggleBreakpointActionDelegate;
import org.eclipse.jface.action.Action;
import org.eclipse.jface.action.IAction;
import org.eclipse.jface.text.BadLocationException;
import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.IRegion;
import org.eclipse.jface.text.ITextSelection;
import org.eclipse.jface.text.TextSelection;
import org.eclipse.jface.text.source.IVerticalRulerInfo;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.ui.IEditorActionDelegate;
import org.eclipse.ui.texteditor.AbstractRulerActionDelegate;
import org.eclipse.ui.texteditor.IDocumentProvider;
import org.eclipse.ui.texteditor.ITextEditor;

public class RulerToggleAction extends AbstractRulerActionDelegate implements IEditorActionDelegate {
//	
//	public RulerToggleAction() {
//		// TODO Auto-generated constructor stub
//	}

	private class ToggleBreakpointAction extends Action {

		private final IVerticalRulerInfo fRulerInfo;
		private final ITextEditor fEditor;

		public ToggleBreakpointAction(ITextEditor editor, IVerticalRulerInfo rulerInfo) {
			super("Toggle line breakpoint");
			fEditor = editor;
			fRulerInfo = rulerInfo;
		}

		@Override
		public void run() {
			IDocumentProvider provider = fEditor.getDocumentProvider();
			ITextSelection selection = null;
			try {
				int line = fRulerInfo.getLineOfLastMouseButtonActivity();
				provider.connect(this);
				IDocument document = provider.getDocument(fEditor.getEditorInput());
				IRegion region = document.getLineInformation(line);
				selection = new TextSelection(document, region.getOffset(), region.getLength());			
			} catch (CoreException e1) {
			} catch (BadLocationException e) {
			} finally {
				provider.disconnect(this);
			}

			if (selection != null) {
				IToggleBreakpointsTarget target = (IToggleBreakpointsTarget) fEditor.getAdapter(IToggleBreakpointsTarget.class);
				try {
					if ((target != null) && (target.canToggleLineBreakpoints(fEditor, selection)))
						target.toggleLineBreakpoints(fEditor, selection);
				} catch (CoreException e) {
				}
			}

		}
	}

//	@Override
//	public void mouseDoubleClick(MouseEvent e) {
//		Platform.getLog(getClass()).info("Double clicked " + e.count);
//		
//	}
	
	@Override
	protected IAction createAction(ITextEditor editor, IVerticalRulerInfo rulerInfo) {
		return new ToggleBreakpointAction(editor, rulerInfo);	
	}
	
}
