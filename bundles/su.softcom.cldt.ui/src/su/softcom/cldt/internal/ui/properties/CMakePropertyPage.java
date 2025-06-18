package su.softcom.cldt.internal.ui.properties;

import java.io.File;
import java.text.Collator;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IBuildConfiguration;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.DefaultScope;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.layout.PixelConverter;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.CellEditor;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerComparator;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.IToolchain;
import su.softcom.cldt.core.ToolchainManager;
import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * Property page for build arguments.
 * 
 */
public final class CMakePropertyPage extends AbstractPropertyPage implements IWorkbenchPropertyPage {

	public static final String NO_CACHE_AVAILABLE_MESSAGE = "Файл CMakeCache не обнаружен. Необходимо сконфигурировать проект.";

	/**
	 * Build arguments which are specified by the user.
	 */
	private Map<String, String> arguments;
	protected ICMakeProject cmakeProject;
	private IToolchain selectedToolchain;
	private Map<String, IToolchain> toolchains;
	private String buildFolderName;
	private Map<String, String> cachedArguments;
	// UI
	private Combo buildConfigCombo;
	private Text buildFolderText;
	private Button showCachedVariablesButton;
	private Button removeButton;
	private TableViewer tableViewer;

	public CMakePropertyPage() {
		super();
	}

	@Override
	public void setElement(IAdaptable element) {
		super.setElement(element);
		ToolchainManager toolchainManager;

		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			project = prj;
			cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
			store = new ProjectScope(project).getNode(CMakeCorePlugin.PLUGIN_ID);
			defaults = DefaultScope.INSTANCE.getNode(CMakeCorePlugin.PLUGIN_ID);
			arguments = cmakeProject.getBuildArguments();
			cachedArguments = cmakeProject.getCache().exists() ? ICMakeProject.parseCMakeCache(cmakeProject.getCache())
					: new HashMap<>();
			if (cachedArguments.isEmpty()) {
				Platform.getLog(getClass()).warn(NO_CACHE_AVAILABLE_MESSAGE);
			}
			buildFolderName = cmakeProject.getBuildFolder().getProjectRelativePath().segment(0);

			toolchainManager = ToolchainManager.getInstance();
			selectedToolchain = toolchainManager.getToolchain(this.project);
			try {
				toolchains = toolchainManager.getToolchainsList();
			} catch (BackingStoreException e) {
				toolchains = new HashMap<>();
				Platform.getLog(getClass()).error("Cannot read toolchains list"); //$NON-NLS-1$
			}
		}
	}

	@Override
	protected Control createContents(Composite parent) {
		parent.setLayout(new GridLayout(1, false));
		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(1, false));
		// COMMON UI
		Group buildOutputGroup = createBuildOutputGroup(common);
		createOutputFolderText(buildOutputGroup);
		createToolchainCombo(buildOutputGroup);
		createBuildConfigCombo(buildOutputGroup);
		// ARGS UI
		Group argsGroup = createArgsGroup(common);
		createArgumentsTable(argsGroup);
		createShowCacheButton(argsGroup);

		setValid(validatePage());
		return common;
	}

	private void createShowCacheButton(Group argsGroup) {
		showCachedVariablesButton = new Button(argsGroup, SWT.CHECK);
		showCachedVariablesButton.setText("Показывать переменные CMakeCache");
		showCachedVariablesButton.setSelection(false);
		showCachedVariablesButton.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 2, 1));
		showCachedVariablesButton.addListener(SWT.Selection, event -> {
			if (showCachedVariablesButton.getSelection()) {
				cachedArguments.forEach(arguments::putIfAbsent);
			} else {
				Set<String> keysToRemove = arguments.keySet().stream()
						.filter(argumentName -> cachedArguments.get(argumentName) != null
								&& cachedArguments.get(argumentName).equals(arguments.get(argumentName)))
						.collect(Collectors.toSet());
				keysToRemove.forEach(arguments::remove);
			}
			tableViewer.refresh();
			setValid(validatePage());
		});
	}

	private Group createArgsGroup(Composite parent) {
		Group group = new Group(parent, SWT.NONE);
		group.setLayout(new GridLayout(2, false));
		group.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		group.setText("Аргументы CMake");
		return group;
	}

	private Group createBuildOutputGroup(Composite parent) {
		Group group = new Group(parent, SWT.NONE);
		group.setLayout(new GridLayout(3, true));
		group.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		group.setText("Основные настройки");
		return group;
	}

	private void createOutputFolderText(Composite parent) {
		Label outputFolderLabel = new Label(parent, SWT.NONE);
		outputFolderLabel.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		outputFolderLabel.setText("Директория сборки");

		buildFolderText = new Text(parent, SWT.BORDER);
		buildFolderText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 2, 1));
		buildFolderText.setText(this.buildFolderName);
		buildFolderText.addModifyListener(event -> buildFolderName = buildFolderText.getText());
	}

	private void createToolchainCombo(Composite parent) {
		Label toolchainLabel = new Label(parent, SWT.NONE);
		toolchainLabel.setText("Кросс-компиляция");

		Combo toolchainCombo = new Combo(parent, SWT.DROP_DOWN | SWT.BORDER);
		toolchainCombo.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, true, 2, 1));
		for (String name : this.toolchains.keySet()) {
			toolchainCombo.add(name);
		}
		toolchainCombo.setText(selectedToolchain != null ? selectedToolchain.getName() : "Не подключена"); //$NON-NLS-1$
		if (toolchains.isEmpty()) {
			toolchainCombo.setEnabled(false);
		}
		toolchainCombo.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				selectedToolchain = toolchains.get(toolchainCombo.getText());
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				widgetSelected(e);
			}

		});
	}

	private void createBuildConfigCombo(Composite parent) {
		try {
			Label buildConfigLabel = new Label(parent, SWT.NONE);
			buildConfigLabel.setText("Тип сборки");

			IBuildConfiguration[] buildConfigArray;

			buildConfigArray = project.getBuildConfigs();

			String buildConfig = project.getActiveBuildConfig().getName();
			buildConfigCombo = new Combo(parent, SWT.DROP_DOWN | SWT.BORDER);
			buildConfigCombo.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, true, 2, 1));
			for (IBuildConfiguration conf : buildConfigArray) {
				buildConfigCombo.add(conf.getName());
			}
			buildConfigCombo.setText(buildConfig);
		} catch (CoreException e) {
			e.printStackTrace();
		}
	}

	private void createArgumentsTable(Composite common) {
		Button addButton;
		createTable(common);
		Table table = tableViewer.getTable();
		packColumns(table);
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		Composite buttonsComposite = createButtonsComposite(common);
		addButton = createButton(buttonsComposite, "Добавить");
		addButton.addListener(SWT.Selection, event -> {
			String newArgumentName = arguments.containsKey("New") ? createCopyName("New") : "New";
			arguments.put(newArgumentName, "");
			tableViewer.refresh();
			setSelection(table, newArgumentName);
			setValid(validatePage());
		});

		removeButton = createButton(buttonsComposite, "Удалить");
		removeButton.addListener(SWT.Selection, event -> {
			for (var argumentTableItem : tableViewer.getTable().getSelection()) {
				String argumentName = argumentTableItem.getData().toString();

				// If cached arguments are shown, replace argument with its cached version
				if (cachedArguments.containsKey(argumentName) && showCachedVariablesButton.getSelection()) {
					arguments.replace(argumentName, cachedArguments.get(argumentName));
				} else {
					arguments.remove(argumentName);
				}
			}
			removeButton.setEnabled(false);
			tableViewer.refresh();
			setValid(validatePage());
		});
		removeButton.setEnabled(false);

		table.addListener(SWT.Selection, event -> removeButton.setEnabled(((TableItem) event.item) != null));
	}

	/**
	 * Creates a table to fill for this page.
	 * 
	 * @param parent
	 */
	private void createTable(Composite parent) {
		Composite tableComposite = new Composite(parent, parent.getStyle());
		tableComposite.setLayout(new GridLayout(1, false));
		tableComposite.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));

		tableViewer = new TableViewer(tableComposite,
				SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL | SWT.MULTI | SWT.FULL_SELECTION);
		tableViewer.setContentProvider(ArrayContentProvider.getInstance());

		TableViewerColumn nameColumnViewer = new TableViewerColumn(tableViewer, SWT.NONE);
		nameColumnViewer.setLabelProvider(ColumnLabelProvider.createTextProvider(element -> element.toString()));
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
				String argumentValue = arguments.get(oldName);
				boolean oldNameIsCached = cachedArguments.containsKey(oldName);
				boolean showCacheIsOn = showCachedVariablesButton.getSelection();

				if (!newName.equals(oldName) && !newName.isBlank()) {
					// If cached arguments are shown, replace argument with its cached version
					if (showCacheIsOn && oldNameIsCached) {
						arguments.replace(oldName, cachedArguments.get(oldName));
					} else {
						arguments.remove(oldName);
					}
					arguments.put(newName, argumentValue);
				} else if (newName.isBlank() && argumentValue.isBlank()) {
					if (showCacheIsOn && oldNameIsCached) {
						arguments.replace(oldName, cachedArguments.get(oldName));
					} else {
						arguments.remove(oldName);
					}
					removeButton.setEnabled(false);
				}

				tableViewer.refresh();
				setValid(validatePage());
			}
		});
		TableColumn nameColumn = nameColumnViewer.getColumn();
		nameColumn.setText("Имя аргумента");

		TableViewerColumn valueColumnViewer = new TableViewerColumn(tableViewer, SWT.NONE);
		valueColumnViewer.setLabelProvider(ColumnLabelProvider.createTextProvider(element -> arguments.get(element)));
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
				return arguments.get(element);
			}

			@Override
			protected void setValue(Object element, Object value) {
				String argumentName = element.toString();
				String newValue = (String) value;
				arguments.replace(argumentName, !newValue.isBlank() ? newValue : "");
				tableViewer.refresh();
				setValid(validatePage());
			}
		});
		TableColumn valueColumn = valueColumnViewer.getColumn();
		valueColumn.setText("Значение аргумента");
		sortTable(nameColumn);
		tableViewer.setInput(arguments.keySet());
		tableViewer.refresh();
	}

	@Override
	public boolean performOk() {
		if (showCachedVariablesButton.getSelection()) {
			arguments.keySet().removeIf(argumentName -> {
				String cachedArgValue = cachedArguments.get(argumentName);

				// No cached value, do not remove it
				if (cachedArgValue == null) {
					return false;
				}

				String tableArgValue = arguments.get(argumentName);

				// Check if the values are equal or if the last segment of the cached value
				// equals the table value
				return cachedArgValue.equals(tableArgValue) || cachedArgValue
						.substring(cachedArgValue.lastIndexOf(File.separator) + 1).equals(tableArgValue);
			});
		}
		storeBuildConfiguration();
		cmakeProject.setBuildArguments(arguments);
		CMakeCorePlugin.getDefault().getProject(project).setBuildFolder(buildFolderName);

		if (selectedToolchain != null) {
			store.put(ICMakeProject.TOOLCHAIN, this.selectedToolchain.getName());
		} else {
			store.remove(ICMakeProject.TOOLCHAIN);
		}

		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	private void storeBuildConfiguration() {
		try {
			String buildConfig = buildConfigCombo.getText();
			IProjectDescription desc = project.getDescription();
			desc.setActiveBuildConfig(buildConfig);
			project.setDescription(desc, new NullProgressMonitor());
		} catch (CoreException | IllegalArgumentException e) {
			Platform.getLog(getClass()).error(e.getMessage());
		}
	}

	/**
	 * Strings comparator used by sorting method.
	 */
	private class ArgumentComparator extends ViewerComparator {
		private final Collator collator = Collator.getInstance(Locale.getDefault());

		@Override
		public int compare(Viewer viewer, Object e1, Object e2) {
			String value1 = e1.toString();
			String value2 = e2.toString();
			return collator.compare(value1, value2);
		}
	}

	/**
	 * Sorts the table with by the arguments's names.
	 * 
	 * @param column
	 */
	private void sortTable(TableColumn column) {
		tableViewer.getTable().setSortColumn(column);
		tableViewer.getTable().setSortDirection(SWT.UP);
		tableViewer.setComparator(new ArgumentComparator());
		tableViewer.refresh();
	}

	/**
	 * Sets the normal size for specified table's each column.
	 * 
	 * @param table
	 */
	private void packColumns(Table table) {
		for (TableColumn column : table.getColumns()) {
			column.pack();
		}
	}

	/**
	 * Creates a new composite for buttons on the specified parent composite.
	 * 
	 * @param parent
	 * @return buttonsComposite
	 */
	private Composite createButtonsComposite(Composite parent) {
		GridLayout layout = new GridLayout(1, false);
		layout.marginWidth = 0;
		layout.marginHeight = 0;
		GridData data = new GridData(SWT.END, SWT.BEGINNING, false, true);
		data.horizontalSpan = 1;

		Composite buttonsComposite = new Composite(parent, SWT.NONE);
		buttonsComposite.setLayout(layout);
		buttonsComposite.setLayoutData(data);
		buttonsComposite.setFont(parent.getFont());

		return buttonsComposite;
	}

	/**
	 * Creates a new button with the specified label for the specified composite.
	 * 
	 * @param parent
	 * @param label
	 * @return button
	 */
	private Button createButton(Composite parent, String label) {
		Button button = new Button(parent, SWT.PUSH);
		button.setFont(parent.getFont());
		button.setText(label);

		GridData gd = new GridData();
		button.setLayoutData(gd);

		PixelConverter converter = new PixelConverter(button);
		int widthHint = converter.convertHorizontalDLUsToPixels(IDialogConstants.BUTTON_WIDTH);
		gd.widthHint = Math.max(widthHint, button.computeSize(SWT.DEFAULT, SWT.DEFAULT, true).x);
		gd.horizontalAlignment = GridData.FILL;

		return button;
	}

	/**
	 * Selects an argument with the specified name the specified table.
	 * 
	 * @param table
	 * @param newArgumentName
	 */
	private void setSelection(Table table, String newArgumentName) {
		for (TableItem tableItem : table.getItems()) {
			if (tableItem.getData().equals(newArgumentName)) {
				table.setSelection(tableItem);
				break;
			}
		}

	}

	/**
	 * Creates a unique copy name.
	 * 
	 * @param name
	 * @return copyName
	 */
	private String createCopyName(String name) {
		String copyName;
		int i = 1;
		do {
			copyName = name + "(" + i++ + ")"; //$NON-NLS-1$//$NON-NLS-2$
		} while (arguments.keySet().contains(copyName));

		return copyName;
	}

	private boolean validatePage() {
		for (var entry : arguments.entrySet()) {
			String argumentName = entry.getKey();
			String argumentValue = entry.getValue();

			boolean isBlankValue = argumentValue.isBlank();
			boolean isCachedArgumentEmpty = cachedArguments.get(argumentName) != null
					&& cachedArguments.get(argumentName).isEmpty();

			if (isBlankValue && !(showCachedVariablesButton.getSelection() && isCachedArgumentEmpty)) {
				setErrorMessage("Укажите значение аргумента %s.".formatted(argumentName));
				return false;
			}
		}

		setMessage(showCachedVariablesButton.getSelection() && cachedArguments.isEmpty() ? NO_CACHE_AVAILABLE_MESSAGE
				: null);
		setErrorMessage(null);
		return true;
	}

	@Override
	protected void performDefaults() {
		buildConfigCombo.setText("Release");
		buildFolderText.setText("build");
		showCachedVariablesButton.setSelection(false);
		updateApplyButton();
	}

}
