package su.softcom.cldt.internal.ui.preferences;

import java.util.function.BiConsumer;
import java.util.function.Function;

import org.eclipse.jface.viewers.CellEditor;
import org.eclipse.jface.viewers.ColumnViewer;
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.swt.widgets.Composite;

public class TextGetSetEditingSupport<T> extends EditingSupport {

	private Function<T, String> getter;
	private BiConsumer<T, String> setter;
	private ColumnViewer viewer;

	/**
	 * @param viewer viewer
	 * @param getter getter
	 * @param setter setter
	 */
	public TextGetSetEditingSupport(ColumnViewer viewer, Function<T, String> getter, BiConsumer<T, String> setter) {
		super(viewer);
		this.viewer = viewer;
		this.getter = getter;
		this.setter = setter;
	}

	@Override
	protected CellEditor getCellEditor(Object element) {
		return new TextCellEditor((Composite) getViewer().getControl());

	}

	@Override
	protected boolean canEdit(Object element) {
		return true;
	}

	@SuppressWarnings("unchecked")
	@Override
	protected Object getValue(Object element) {
		return getter.apply((T) element);
	}

	@SuppressWarnings("unchecked")
	@Override
	protected void setValue(Object element, Object value) {
		setter.accept((T) element, (String) value);
		viewer.update(element, null);

	}

}
