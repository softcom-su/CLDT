package su.softcom.cldt.ui;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerComparator;

import su.softcom.cldt.core.source.ISourceElement;
import su.softcom.cldt.core.source.ISourceFile;
import su.softcom.cldt.internal.ui.CMakeTargetsFolder;

public class CMakeViewerComparator extends ViewerComparator {

	// Categories
	private static final int TARGETS_FOLDER = 1;
	private static final int FOLDER = 2;
	private static final int CPP_SOURCE_FILE = 3;
	private static final int FILE = 4;
	private static final int SOURCE_ELEMENT = 5;

	private static final int OTHERS = 42;

	@Override
	public int category(Object element) {
		if (element instanceof CMakeTargetsFolder) {
			return TARGETS_FOLDER;
		}
		if (element instanceof IFolder) {
			return FOLDER;
		}
		if (element instanceof IFile) {
			return FILE;
		}
		if (element instanceof ISourceFile) {
			return CPP_SOURCE_FILE;
		}
		if (element instanceof ISourceElement) {
			return SOURCE_ELEMENT;
		}
		return OTHERS;
	}

	@Override
	public int compare(Viewer viewer, Object e1, Object e2) {
		int cat1 = category(e1);
		int cat2 = category(e2);
		int diff = cat1 - cat2;
		if (cat1 == SOURCE_ELEMENT && cat2 == SOURCE_ELEMENT) {
			diff = compareSourceElements((ISourceElement) e1, (ISourceElement) e2);
		}
		return diff;
	}

	private int compareSourceElements(ISourceElement srcElem1, ISourceElement srcElem2) {
		return srcElem1.getStartPosition().getLine() - srcElem2.getStartPosition().getLine();
	}

}
