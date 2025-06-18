package su.softcom.cldt.internal.ui;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.jface.viewers.ITreeContentProvider;

public class AddElementContentProvider implements ITreeContentProvider {

	public static class AddNewElement {

		@Override
		public String toString() {
			return "Добавить";
		}

	}

	private static AddNewElement addElement = new AddNewElement();

	@Override
	public Object[] getChildren(Object parentElement) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object getParent(Object element) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean hasChildren(Object element) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public Object[] getElements(Object inputElement) {
		if (inputElement instanceof List l) {
			ArrayList<Object> list = new ArrayList<>(l);
			list.add(addElement);
			return list.toArray();
		} else if (inputElement instanceof Set l) {
			Set<Object> list = new HashSet<>(l);
			list.add(addElement);
			return list.toArray();
		}
		return new Object[0];
	}
}
