package su.softcom.cldt.ui.resolver.quickfix;

import java.io.File;

import org.eclipse.core.filesystem.EFS;
import org.eclipse.core.filesystem.IFileStore;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.text.IDocument;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.IMarkerResolution;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PartInitException;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.ide.IDE;
import org.eclipse.ui.part.FileEditorInput;
import org.eclipse.ui.texteditor.ITextEditor;

public abstract class AbstractQuickFix implements IMarkerResolution {

	private static final String C_EDITOR_ID = "su.softcom.cldt.editors.ceditor"; //$NON-NLS-1$
	protected String fileName = new String();
	private String label;

	protected ILog log = Platform.getLog(getClass());

	public AbstractQuickFix(String label) {
		setLabel(label);
	}

	@Override
	public String getLabel() {
		return label;
	}

	private void setLabel(String label) {
		this.label = label;
	}

	@Override
	public void run(IMarker marker) {
		int lineNum = marker.getAttribute(IMarker.LINE_NUMBER, -1);
		IDocument document = openDocument(marker);
		try {
			if (lineNum >= 0) {
				fixProblem(document, lineNum);
			}
			marker.delete();
		} catch (CoreException e) {
			log.error("Проблема при удалении маркера после разрешения ошибки");
		}
	}

	protected IDocument openDocument(IMarker marker) {
		IEditorPart editorPart;
		try {
			editorPart = openInEditor(marker);
		} catch (PartInitException e) {
			return null;
		}
		if (editorPart instanceof ITextEditor) {
			ITextEditor editor = (ITextEditor) editorPart;
			IDocument doc = editor.getDocumentProvider().getDocument(editor.getEditorInput());
			return doc;
		}
		return null;
	}

	private IEditorPart openInEditor(IMarker marker) throws PartInitException {
		String href = getLocationHRef(marker);
		String file = getFileFromURL(href);
		IResource markerResource = marker.getResource();
		fileName = markerResource.getName();

		IFile efile = null;
		if (markerResource instanceof IFile) {
			efile = (IFile) markerResource;
		}
		if (efile != null) {
			IWorkbenchPage page = getActivePage();
			IEditorPart part = page.openEditor(new FileEditorInput(efile), C_EDITOR_ID);
			return part;
		}
		File fileToOpen = new File(file);
		if (fileToOpen.exists() && fileToOpen.isFile()) {
			IFileStore fileStore = EFS.getLocalFileSystem().getStore(fileToOpen.toURI());
			IWorkbenchPage page = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage();
			try {
				return IDE.openEditorOnFileStore(page, fileStore);
			} catch (PartInitException e) {
				Platform.getLog(getClass()).error(e.getMessage());
			}
		}
		return null;
	}

	private String getLocationHRef(IMarker marker) {
		String loc = marker.getResource().getLocationURI().toString();
		String loc2 = marker.getAttribute(IMarker.LOCATION, ""); //$NON-NLS-1$
		int line = marker.getAttribute(IMarker.LINE_NUMBER, 0);
		if (loc2.length() > 0) {
			loc = "file:" + loc2.replaceAll("[^:]*: ", ""); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
		}
		String href = loc + "#" + line; //$NON-NLS-1$
		return href;
	}

	private static String getFileFromURL(String link) {
		String file = link.replaceFirst("^file:", ""); //$NON-NLS-1$ //$NON-NLS-2$
		file = file.replaceAll("#\\d+$", ""); //$NON-NLS-1$//$NON-NLS-2$
		return file;
	}

	private IWorkbenchPage getActivePage() {
		IWorkbenchWindow activeWorkbenchWindow = PlatformUI.getWorkbench().getActiveWorkbenchWindow();
		if (activeWorkbenchWindow == null) {
			return null;
		}
		IWorkbenchPage activePage = activeWorkbenchWindow.getActivePage();
		return activePage;
	}

	protected abstract void fixProblem(IDocument document, int lineNum);

}
