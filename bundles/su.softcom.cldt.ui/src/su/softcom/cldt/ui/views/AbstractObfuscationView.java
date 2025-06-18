package su.softcom.cldt.ui.views;

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.ui.part.ViewPart;

/**
 * Abstract view for obfuscation information.
 */
public abstract class AbstractObfuscationView extends ViewPart {

	/** Folder with file with statistics. */
	protected static IFolder folderForInfo;

	/** Viewer for table. */
	protected TableViewer viewer;

	@Override
	public void createPartControl(Composite parent) {
		viewer = new TableViewer(parent, SWT.MULTI | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

		createColumns();

		Table table = viewer.getTable();
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		viewer.setContentProvider(new ArrayContentProvider());

		refresh();

		for (TableColumn column : table.getColumns()) {
			column.pack();
		}
		table.pack();
	}

	@Override
	public void setFocus() {
		// No need to implement
	}

	/**
	 * Gets input from file, generated in obfuscation.
	 * 
	 * @param fileName name of file with input
	 * 
	 * @return array of rows in file
	 */
	protected String[] getInput(String fileName) {
		IFile file = folderForInfo.getFile(fileName);
		try {
			file.setCharset(StandardCharsets.UTF_8.toString(), new NullProgressMonitor());
		} catch (CoreException e1) {
			Platform.getLog(getClass()).error(e1.getLocalizedMessage());
		}

		try (InputStream contents = file.getContents()) {
			String stat_string = new String(contents.readAllBytes(), StandardCharsets.UTF_8);
			String[] rows = stat_string.split("\n"); //$NON-NLS-1$

			return rows;
		} catch (IOException | CoreException e) {
			Platform.getLog(getClass()).error(e.getLocalizedMessage());
		}

		return null;
	}

	/**
	 * Creates column with given name and index.
	 * 
	 * @param name  name of column
	 * @param index index of column
	 */
	protected void createColumn(String name, int index) {
		TableViewerColumn columnStatic = new TableViewerColumn(viewer, SWT.NONE);
		columnStatic.getColumn().setText(name);
		columnStatic.getColumn().setResizable(true);
		columnStatic.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				if (element instanceof String) {
					String[] row = ((String) element).split("\t"); //$NON-NLS-1$
					return row[index];
				}
				return element.toString();
			}
		});
	}

	/**
	 * Refreshes viewer.
	 */
	public abstract void refresh();

	/**
	 * Creates columns of table viewer.
	 */
	protected abstract void createColumns();

}
