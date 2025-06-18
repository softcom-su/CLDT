package su.softcom.cldt.internal.ui;

import java.util.List;

import org.eclipse.jface.viewers.CellEditor;
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.jface.viewers.TreeViewer;

public final class TextEditingSupport extends EditingSupport {

	private final TreeViewer treeViewer;
	private final TextCellEditor editor;

	public TextEditingSupport(TreeViewer treeViewer) {
		super(treeViewer);
		this.treeViewer = treeViewer;
		this.editor = new TextCellEditor(treeViewer.getTree());
	}

	@Override
	protected CellEditor getCellEditor(Object element) {
		return editor;
	}

	@Override
	protected boolean canEdit(Object element) {
		return true;
	}

	@Override
	protected Object getValue(Object element) {
		return element.toString();
	}

	@Override
	protected void setValue(Object element, Object value) {
		Object dataObj = treeViewer.getInput();
		if (dataObj instanceof List data) {
			if (element instanceof AddElementContentProvider.AddNewElement) {
				if (!element.toString().equals(value)) {
					data.add(value.toString());
				}
			} else {
				if (((String) value).isBlank()) {
					data.remove(element);
				} else {
					data.set(data.indexOf(element), value);
				}
			}
			treeViewer.refresh();
		}

	}
}