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
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusAdapter;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.FontData;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.eclipse.ui.dialogs.PreferencesUtil;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.internal.ui.AddElementContentProvider;
import su.softcom.cldt.internal.ui.AddElementLabelProvider;
import su.softcom.cldt.ui.dialogs.ClangTidyChecksDialog;

/**
 * The page for clang-tidy settings.
 */
public final class ClangTidyPropertyPage extends AbstractPropertyPage implements IWorkbenchPropertyPage {

	protected static final Image IMAGE_OPT = getResourceManager().createImage(createImageDescriptor("option.png")); //$NON-NLS-1$
	protected static final Image IMAGE_ADD = getResourceManager().createImage(createImageDescriptor("add.png")); //$NON-NLS-1$
	protected static final String USE_GLOBAL_CHECKS = "Глобальные для всех проектов";
	protected static final String USE_LOCAL_CHECKS = "Установлены для этого проекта";
	protected static final String CHECKS = "checks"; //$NON-NLS-1$
	protected static final String PREFERENCES_PAGE = "su.softcom.cldt.internal.ui.preferences.ClangTidyChecksPreferences"; //$NON-NLS-1$

	private Map<String, String> parameters;
	private List<String> keys = new ArrayList<>();

	private Button resetButton;
	private Button globalChecksButton;
	private Button projectChecksButton;

	/**
	 * Creates a new instance of ClangTidyPropertyPage.
	 */
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

	@Override
	protected Control createContents(Composite parent) {
		noDefaultButton();

		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(1, false));
		common.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		createTopPanel(common);

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

	private void createTopPanel(Composite parent) {
		Composite topPanel = new Composite(parent, SWT.NONE);
		GridLayout topPanelLayout = new GridLayout(2, false);
		topPanelLayout.horizontalSpacing = 10;
		topPanel.setLayout(topPanelLayout);

		topPanel.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));

		Composite checksInformation = new Composite(topPanel, SWT.NONE);
		checksInformation.setLayout(new GridLayout(2, false));
		checksInformation.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		Label checksLabel = new Label(checksInformation, SWT.NONE);
		checksLabel.setText("Проверки:");
		FontData[] fontData = checksLabel.getFont().getFontData();
		fontData[0].setStyle(SWT.BOLD);
		checksLabel.setFont(new Font(checksLabel.getDisplay(), fontData));

		Label statusLabel = new Label(checksInformation, SWT.NONE);
		statusLabel.setText(isUsingProjectSettings() ? USE_LOCAL_CHECKS : USE_GLOBAL_CHECKS);
		statusLabel.setLayoutData(new GridData(SWT.LEFT, SWT.BOTTOM, true, false));
		GridData statusGridData = new GridData(SWT.FILL, SWT.CENTER, true, false);
		statusGridData.widthHint = 250;
		statusLabel.setLayoutData(statusGridData);

		Composite buttonPanel = new Composite(topPanel, SWT.NONE);
		buttonPanel.setLayout(new GridLayout(1, false));
		buttonPanel.setLayoutData(new GridData(SWT.RIGHT, SWT.TOP, false, false));

		resetButton = new Button(buttonPanel, SWT.PUSH);
		resetButton.setText("Сброс");
		resetButton.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
		resetButton.addListener(SWT.Selection, e -> {
			resetToGlobalSettings();
			statusLabel.setText(USE_GLOBAL_CHECKS);
			updateButtonsState();
		});

		globalChecksButton = new Button(buttonPanel, SWT.PUSH);
		globalChecksButton.setText("Редактировать глобальные проверки");
		globalChecksButton.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
		globalChecksButton.addListener(SWT.Selection,
				e -> PreferencesUtil.createPreferenceDialogOn(getShell(), PREFERENCES_PAGE, null, null) // $NON-NLS-1$
						.open());

		projectChecksButton = new Button(buttonPanel, SWT.PUSH);
		projectChecksButton.setText("Выбрать для этого проекта");
		projectChecksButton.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false));
		projectChecksButton.addListener(SWT.Selection, e -> {
			ClangTidyChecksDialog dialog = new ClangTidyChecksDialog(getShell(), parameters.getOrDefault(CHECKS, ""));

			if (dialog.open() == Window.OK) {
				parameters.remove(CHECKS);
				parameters.put(CHECKS, dialog.getSelectedChecksAsString());
				statusLabel.setText(USE_LOCAL_CHECKS);
				updateButtonsState();
			}
		});

		updateButtonsState();
	}

	private TableViewer createTable(Composite parent) {
		Composite tableComposite = new Composite(parent, parent.getStyle());
		tableComposite.setLayout(new GridLayout(1, false));
		tableComposite.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		Label label = new Label(tableComposite, SWT.NONE);
		label.setText("Дополнительные параметры");

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

						createValueText(table, keys.size() - 1, newName, tableViewer);
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

		valueText.addTraverseListener(e -> {
			if (e.detail == SWT.TRAVERSE_RETURN) {
				String value = valueText.getText();
				parameters.put(name, value);
				valueText.dispose();
				tableViewer.refresh();
				setValid(validatePage());
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
		System.out.println("SETTING PARAMS");
		Map<String, String> clangTidyParams = new HashMap<>();

		try {
			String[] storeKeys = store.keys();
			for (String key : storeKeys) {
				String value = store.get(key, ""); //$NON-NLS-1$
				clangTidyParams.put(key, value);
				if (!key.equals(CHECKS)) {
					keys.add(key);
				}
			}
		} catch (BackingStoreException e) {
			System.out.println("SETTING ERROR");
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
		if (keys.contains(CHECKS)) {
			setErrorMessage("Не указывайте проверки вручную. Используйте \"Выбрать для этого проекта\"");
			return false;
		}

		for (var entry : parameters.entrySet()) {
			String parameterName = entry.getKey();
			String parameterValue = entry.getValue();

			boolean isBlankValue = parameterValue.isBlank();

			if (isBlankValue) {
				setErrorMessage("Укажите параметр %s.".formatted(parameterName));
				return false;
			}
		}

		setMessage(null);
		setErrorMessage(null);
		return true;
	}

	private boolean isUsingProjectSettings() {
		return parameters.getOrDefault(CHECKS, null) != null;
	}

	private void resetToGlobalSettings() {
		parameters.remove(CHECKS);
	}

	private void updateButtonsState() {
		boolean usingProjectSettings = isUsingProjectSettings();
		globalChecksButton.setEnabled(!usingProjectSettings);
		projectChecksButton.setEnabled(true);
		resetButton.setEnabled(usingProjectSettings);
	}
}
