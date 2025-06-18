package su.softcom.cldt.internal.ui;

import org.eclipse.jface.viewers.ViewerComparator;

public class AddElementComparator extends ViewerComparator {

	@Override
	public int category(Object element) {
		if (element instanceof AddElementContentProvider.AddNewElement) {
			return 1;
		}
		return 0;
	}

}
