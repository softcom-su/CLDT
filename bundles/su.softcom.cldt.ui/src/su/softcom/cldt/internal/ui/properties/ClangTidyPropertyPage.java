package su.softcom.cldt.internal.ui.properties;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.jface.viewers.CellEditor;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusAdapter;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.TraverseEvent;
import org.eclipse.swt.events.TraverseListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.internal.ui.AddElementContentProvider;
import su.softcom.cldt.internal.ui.AddElementLabelProvider;

public final class ClangTidyPropertyPage extends AbstractPropertyPage implements IWorkbenchPropertyPage {

	static Image IMAGE_OPT = getResourceManager().createImage(createImageDescriptor("option.png")); //$NON-NLS-1$
	static final Image IMAGE_ADD = getResourceManager().createImage(createImageDescriptor("add.png")); //$NON-NLS-1$
	private Map<String, String> parameters;
	private List<String> keys = new ArrayList<String>();

	public ClangTidyPropertyPage() {
		super();
	}

	@Override
	public void setElement(IAdaptable element) {
		super.setElement(element);

		IProject proj = element.getAdapter(IProject.class);
		if (proj != null) {
			project = proj;
			store = (IEclipsePreferences) new ProjectScope(project).getNode(PreferenceConstants.NODE)
					.node("clang-tidy"); //$NON-NLS-1$
			parameters = getClangTidyParameters();
		}
	}

	@Override
	protected Control createContents(Composite parent) {
		parent.setLayout(new GridLayout(1, false));
		noDefaultButton();
		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(1, false));
		common.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		TableViewer tableViewer = createTable(common);
		initTable(tableViewer);
		Table table = tableViewer.getTable();
		packColumns(table);
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		setValid(validatePage());

		return common;
	}

	private TableViewer createTable(Composite parent) {
		Composite tableComposite = new Composite(parent, parent.getStyle());
		tableComposite.setLayout(new GridLayout(1, false));
		tableComposite.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		TableViewer tableViewer = new TableViewer(tableComposite,
				SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL | SWT.MULTI | SWT.FULL_SELECTION);
		tableViewer.setContentProvider(new AddElementContentProvider());
		Table table = tableViewer.getTable();

		TableViewerColumn nameColumnViewer = new TableViewerColumn(tableViewer, SWT.NONE);
		TableViewerColumn valueColumnViewer = new TableViewerColumn(tableViewer, SWT.NONE);

		nameColumnViewer.setLabelProvider(new AddElementLabelProvider(IMAGE_OPT));
		valueColumnViewer.setLabelProvider(ColumnLabelProvider.createTextProvider(element -> parameters.get(element)));

		TableColumn nameColumn = nameColumnViewer.getColumn();
		TableColumn valueColumn = valueColumnViewer.getColumn();

		nameColumn.setText("Название параметра"); //$NON-NLS-1$
		valueColumn.setText("Значение параметра"); //$NON-NLS-1$

		nameColumn.addListener(SWT.Selection, event -> {
		});
		valueColumn.addListener(SWT.Selection, event -> {
		});

		nameColumnViewer.setEditingSupport(new EditingSupport(tableViewer) {
			@Override
			protected CellEditor getCellEditor(Object element) {
				return new TextCellEditor(tableViewer.getTable());
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
				String oldName = element.toString();
				String newName = (String) value;

				if (!(element instanceof AddElementContentProvider.AddNewElement)) {
					String parameterValue = parameters.get(oldName);

					if (!newName.equals(oldName) && !newName.isBlank() && !newName.equals("Добавить")) { //$NON-NLS-1$
						parameters.remove(oldName);
						parameters.put(newName, parameterValue);
						if (!keys.contains(newName)) {
							keys.add(keys.indexOf(oldName), newName);
						}

						keys.remove(oldName);
					} else if (newName.isBlank()) {
						parameters.remove(oldName);
						keys.remove(oldName);
					}
				} else {
					if (!newName.equals(oldName) && !newName.isBlank()) {
						parameters.put(newName, ""); //$NON-NLS-1$
						if (!keys.contains(newName)) {
							keys.add(newName);
						}

						createValueText(table, parameters.size() - 1, newName, tableViewer);
					}
				}

				tableViewer.refresh();
				setValid(validatePage());
			}
		});

		valueColumnViewer.setEditingSupport(new EditingSupport(tableViewer) {

			@Override
			protected CellEditor getCellEditor(Object element) {
				return new TextCellEditor(tableViewer.getTable());
			}

			@Override
			protected boolean canEdit(Object element) {
				return true;
			}

			@Override
			protected Object getValue(Object element) {
				return parameters.get(element);
			}

			@Override
			protected void setValue(Object element, Object value) {
				String parameterName = element.toString();
				String newValue = (String) value;
				parameters.replace(parameterName, !newValue.isBlank() ? newValue : ""); //$NON-NLS-1$
				tableViewer.refresh();
				setValid(validatePage());
			}
		});

		return tableViewer;
	}

	@Override
	public boolean performOk() {
		try {
			store.clear();
			for (Map.Entry<String, String> entry : parameters.entrySet()) {
				store.put(entry.getKey(), entry.getValue());
			}

			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		return true;
	}

	private void createValueText(Composite parent, int index, String name, TableViewer tableViewer) {
		Text valueText = new Text(parent, SWT.BORDER);

		Table table = tableViewer.getTable();
		int headerHeight = table.getHeaderHeight();
		int rowHeight = table.getItemHeight();
		int nameColumnWidth = tableViewer.getTable().getColumns()[0].getWidth();
		int tableWidth = tableViewer.getTable().getClientArea().width;

		valueText.setBounds(parent.getClientArea().x + nameColumnWidth,
				parent.getClientArea().y + headerHeight + 1 + (index * rowHeight), tableWidth - nameColumnWidth,
				rowHeight);

		valueText.moveAbove(null);
		valueText.setFocus();

		valueText.addFocusListener(new FocusAdapter() {
			@Override
			public void focusLost(FocusEvent e) {
				String value = valueText.getText();
				parameters.put(name, value);
				valueText.dispose();
				tableViewer.refresh();
				setValid(validatePage());
			}
		});

		valueText.addTraverseListener(new TraverseListener() {
			@Override
			public void keyTraversed(TraverseEvent e) {
				if (e.detail == SWT.TRAVERSE_RETURN) {
					String value = valueText.getText();
					parameters.put(name, value);
					valueText.dispose();
					tableViewer.refresh();
					setValid(validatePage());
				}
			}
		});

		parent.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseDown(MouseEvent e) {
				valueText.setFocus();
			}
		});

		tableViewer.refresh();
		setValid(validatePage());
	}

	private Map<String, String> getClangTidyParameters() {
		Map<String, String> clangTidyParams = new HashMap<>();

		try {
			String[] storeKeys = store.keys();
			for (String key : storeKeys) {
				String value = store.get(key, ""); //$NON-NLS-1$
				clangTidyParams.put(key, value);
				keys.add(key);
			}
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		return clangTidyParams;
	}

	private void initTable(TableViewer tableViewer) {
		tableViewer.setInput(keys);
		tableViewer.refresh();
	}

	private void packColumns(Table table) {
		for (TableColumn column : table.getColumns()) {
			column.pack();
		}
	}

	private boolean validatePage() {
		for (var entry : parameters.entrySet()) {
			String parameterName = entry.getKey();
			String parameterValue = entry.getValue();

			boolean isBlankValue = parameterValue.isBlank();

			if (isBlankValue) {
				setErrorMessage("Укажите параметр %s.".formatted(parameterName)); //$NON-NLS-1$
				return false;
			}
		}

		setMessage(null);
		setErrorMessage(null);
		return true;
	}
}
