package su.softcom.cldt.internal.ui.properties;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import java.util.stream.StreamSupport;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.internal.ui.DebugPluginImages;
import org.eclipse.debug.internal.ui.MultipleInputDialog;
import org.eclipse.debug.internal.ui.launchConfigurations.LaunchConfigurationsMessages;
import org.eclipse.debug.ui.IDebugUIConstants;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.dialogs.ProgressMonitorDialog;
import org.eclipse.jface.layout.PixelConverter;
import org.eclipse.jface.layout.TableColumnLayout;
import org.eclipse.jface.operation.IRunnableWithProgress;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.ColumnWeightData;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerComparator;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.dnd.Clipboard;
import org.eclipse.swt.dnd.TextTransfer;
import org.eclipse.swt.dnd.Transfer;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.eclipse.ui.dialogs.PropertyPage;
import org.osgi.service.prefs.BackingStoreException;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.operations.EnvOperation;
import su.softcom.cldt.internal.ui.preferences.EnvironmentVariable;
import su.softcom.cldt.ui.launch.tabs.ImportDialog;
import su.softcom.cldt.ui.launch.tabs.Messages;

/**
 * Property page for environment variables.
 * 
 * @author Dmitrii Davladov - creator
 */
public class EnvironmentPropertyPage extends PropertyPage implements IWorkbenchPropertyPage {

	private class EnvironmentVariableLabelProvider extends LabelProvider implements ITableLabelProvider {

		@Override
		public String getColumnText(Object element, int columnIndex) {
			String result = null;
			if (element != null) {
				EnvironmentVariable var = (EnvironmentVariable) element;
				switch (columnIndex) {
				case 0: // variable
					result = var.getName();
					break;
				case 1: // value
					result = var.getValue();
					break;
				default:
					break;
				}
			}
			return result;
		}

		@Override
		public Image getColumnImage(Object element, int columnIndex) {
			if (columnIndex == 0) {
				return DebugPluginImages.getImage(IDebugUIConstants.IMG_OBJS_ENV_VAR);
			}
			return null;
		}

	}

	private class EnvironmentVariableContentProvider implements IStructuredContentProvider {

		@Override
		public Object[] getElements(Object inputElement) {
			Map<String, String> m = null;
			if (inputElement instanceof Map<?, ?>) {
				m = (Map<String, String>) inputElement;
			}
			EnvironmentVariable[] elements = new EnvironmentVariable[0];
			if (m != null && !m.isEmpty()) {
				elements = new EnvironmentVariable[m.size()];
				String[] varNames = new String[m.size()];
				m.keySet().toArray(varNames);
				for (int i = 0; i < m.size(); i++) {
					elements[i] = new EnvironmentVariable(varNames[i], m.get(varNames[i]));
				}
			}
			return elements;
		}

		@Override
		public void dispose() {
		}

		@Override
		public void inputChanged(Viewer viewer, Object oldInput, Object newInput) {
			if (newInput == null) {
				return;
			}
			if (viewer instanceof TableViewer) {
				TableViewer tableViewer = (TableViewer) viewer;
				if (tableViewer.getTable().isDisposed()) {
					return;
				}
				tableViewer.setComparator(new ViewerComparator() {
					@Override
					public int compare(Viewer iviewer, Object e1, Object e2) {
						if (e1 == null) {
							return -1;
						} else if (e2 == null) {
							return 1;
						} else {
							return ((EnvironmentVariable) e1).getName()
									.compareToIgnoreCase(((EnvironmentVariable) e2).getName());
						}
					}
				});
			}
		}

	}

	private class ImportTable implements Runnable {

		private TableViewer environmentTable;
		private Map<String, String> table;

		/**
		 * Creates a runnable instance.
		 * 
		 * @param table table to import
		 */
		public ImportTable(Map<String, String> table, TableViewer environmentTable) {
			this.table = table;
			this.environmentTable = environmentTable;
		}

		@Override
		public void run() {
			current.entrySet().stream().forEach(entry -> table.putIfAbsent(entry.getKey(), entry.getValue()));
			Set<String> intersection = current.keySet();
			intersection.retainAll(table.keySet());

			if (!intersection.isEmpty()) {
				MessageDialog dialog = new MessageDialog(shell, Messages.EnvTab_Intersection_Title, null,
						Messages.EnvTab_Intersection_Message, MessageDialog.QUESTION,
						new String[] { Messages.EnvTab_Replace_Button, Messages.EnvTab_Merge_Button }, 0);

				if (dialog.open() != 0) {
					intersection.stream()
							.map(key -> Map.entry(key, Stream.of(table.get(key), current.get(key))
									.flatMap(value -> Arrays.stream(value.split(";"))).distinct() //$NON-NLS-1$
									.filter(value -> !value.isEmpty()).reduce((fst, snd) -> fst + ";" + snd).get())) //$NON-NLS-1$
							.forEach(entry -> table.put(entry.getKey(), entry.getValue()));
				}
			}

//				ScopedPreferenceStore store = new ScopedPreferenceStore(new ProjectScope(project),
//						PreferenceConstants.CLDT_PROJECT_PREFERENCE_STORE_NAME);
//				store.setValue(PreferenceConstants.CLDT_PROJECT_PREFERENCE_BUILD_ENV_VAR,
//						mapper.writerWithDefaultPrettyPrinter().writeValueAsString(table));

			updateEnvironmentTable(table, this.environmentTable);
		}

	}

	// FIXME remove this from field
	private TableViewer environmentTable;
	private Shell shell;

	private IProject project;
	private IEclipsePreferences store;

	private Map<String, String> current;
	private String environment;

	private String[] envTableColumnHeaders = { "Название", "Значение" };
	private static final String P_VARIABLE = "variable";
	private static final String P_VALUE = "value";
	private static final String NAME_LABEL = "Имя";
	private static final String VALUE_LABEL = "Значение";

	public EnvironmentPropertyPage() {
		super();
	}

	@Override
	public IAdaptable getElement() {
		return project;
	}

	@Override
	public void setElement(IAdaptable element) {
		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			this.project = prj;
			this.store = new ProjectScope(project).getNode(PreferenceConstants.NODE);

			this.environment = this.store.get(PreferenceConstants.BUILD_ENV_VARS, ""); //$NON-NLS-1$

			try {
				ObjectMapper mapper = new ObjectMapper();
				TypeReference<HashMap<String, String>> typeRef = new TypeReference<HashMap<String, String>>() {
				};
				this.current = mapper.readValue(new ProjectScope(project).getNode(PreferenceConstants.NODE)
						.get(PreferenceConstants.BUILD_ENV_VARS, ""), typeRef); //$NON-NLS-1$

			} catch (JsonProcessingException e) {
				this.current = new HashMap<String, String>();
			}
		}
	}

	@Override
	protected Control createContents(Composite parent) {
		Composite common = createCommon(parent);

		Group group = new Group(common, SWT.NONE);
		group.setLayout(new GridLayout(2, false));
		group.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));

		this.environmentTable = createEnvironmentTable(group);
		this.shell = getShell();

		Composite buttonsComposite = createButtonsComposite(group);

		Button envAddButton = createButton(buttonsComposite, LaunchConfigurationsMessages.EnvironmentTab_Add_4);
		Button envImportButton = createButton(buttonsComposite, Messages.EnvTab_Import_Button);
//		Button envSelectButton = createButton(buttonsComposite, LaunchConfigurationsMessages.EnvironmentTab_18);
		Button envEditButton = createButton(buttonsComposite, LaunchConfigurationsMessages.EnvironmentTab_Edit_5);
		Button envRemoveButton = createButton(buttonsComposite, LaunchConfigurationsMessages.EnvironmentTab_Remove_6);
		Button envCopyButton = createButton(buttonsComposite, LaunchConfigurationsMessages.EnvironmentTab_Copy);
		Button envPasteButton = createButton(buttonsComposite, LaunchConfigurationsMessages.EnvironmentTab_Paste);

		environmentTable.addSelectionChangedListener(event -> {
			int size = event.getStructuredSelection().size();
			envEditButton.setEnabled(size == 1);
			envRemoveButton.setEnabled(size > 0);
			envCopyButton.setEnabled(size > 0);
		});

		envEditButton.setEnabled(false);
		envRemoveButton.setEnabled(false);
		envCopyButton.setEnabled(false);

		envAddButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent event) {
				handleEnvAddButtonSelected(environmentTable);
			}
		});

//		envSelectButton.addSelectionListener(new SelectionAdapter() {
//			@Override
//			public void widgetSelected(SelectionEvent event) {
//				handleEnvSelectButtonSelected(environmentTable);
//			}
//		});

		envEditButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent event) {
				handleEnvEditButtonSelected(environmentTable);
			}
		});

		envRemoveButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent event) {
				handleEnvRemoveButtonSelected(environmentTable);
			}
		});

		envCopyButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent event) {
				handleEnvCopyButtonSelected(environmentTable);
			}
		});

		envPasteButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent event) {
				handleEnvPasteButtonSelected(environmentTable);
			}
		});

		envImportButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent event) {
				handleEnvImportButtonSelected(environmentTable);
				// updateLaunchConfigurationDialog();
			}
		});

		if (this.environment.length() > 0) {
			setValueToTable(environment, environmentTable);
		}

		return common;
	}

	protected Composite createCommon(Composite parent) {
		parent.setLayout(new GridLayout(1, false));

		GridData commonData = new GridData();
		commonData.grabExcessVerticalSpace = true;
		commonData.verticalAlignment = SWT.FILL;
		commonData.grabExcessHorizontalSpace = true;
		commonData.horizontalAlignment = SWT.FILL;

		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(1, false));
		common.setLayoutData(commonData);

		return common;
	}

	private TableViewer createEnvironmentTable(Composite parent) {
		Font font = parent.getFont();
		Label l = new Label(parent, SWT.NONE);
		l.setFont(parent.getFont());
		l.setText(LaunchConfigurationsMessages.EnvironmentTab_Environment_variables_to_set__3);
		GridData gd = new GridData(GridData.FILL_HORIZONTAL);
		gd.horizontalSpan = 2;
		gd.grabExcessHorizontalSpace = false;
		l.setLayoutData(gd);

		// Create table composite
		Composite tableComposite = new Composite(parent, parent.getStyle());// SWTFactory.createComposite(parent, font,
																			// 1, 1, GridData.FILL_BOTH, 0, 0);
		GridLayout layout = new GridLayout(1, false);
		layout.marginWidth = 0;
		layout.marginHeight = 0;
		tableComposite.setLayout(layout);
		tableComposite.setFont(font);
		gd = new GridData(GridData.FILL_BOTH);
		gd.horizontalSpan = 1;
		tableComposite.setLayoutData(gd);

		// Create table
		TableViewer environmentTable = new TableViewer(tableComposite,
				SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL | SWT.MULTI | SWT.FULL_SELECTION);
		Table table = environmentTable.getTable();
		table.setLayoutData(new GridData(GridData.FILL_BOTH));
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setFont(font);

		environmentTable.setContentProvider(new EnvironmentVariableContentProvider());
		environmentTable.setLabelProvider(new EnvironmentVariableLabelProvider());
		environmentTable.setColumnProperties(new String[] { P_VARIABLE, P_VALUE });

		// Setup environment variable name column
		final TableViewerColumn tcv1 = new TableViewerColumn(environmentTable, SWT.NONE, 0);
		tcv1.setLabelProvider(
				ColumnLabelProvider.createTextProvider(element -> ((EnvironmentVariable) element).getName()));

		TableColumn tc1 = tcv1.getColumn();
		tc1.setText(envTableColumnHeaders[0]);

		// Setup environment variable value column
		final TableViewerColumn tcv2 = new TableViewerColumn(environmentTable, SWT.NONE, 1);
		tcv2.setLabelProvider(
				ColumnLabelProvider.createTextProvider(element -> ((EnvironmentVariable) element).getValue()));

		TableColumn tc2 = tcv2.getColumn();
		tc2.setText(envTableColumnHeaders[1]);

		// Create table column layout
		TableColumnLayout tableColumnLayout = new TableColumnLayout(true);
		PixelConverter pixelConverter = new PixelConverter(font);
		tableColumnLayout.setColumnData(tc1, new ColumnWeightData(1, pixelConverter.convertWidthInCharsToPixels(20)));
		tableColumnLayout.setColumnData(tc2, new ColumnWeightData(2, pixelConverter.convertWidthInCharsToPixels(20)));
		tableComposite.setLayout(tableColumnLayout);

		return environmentTable;
	}

	private Composite createButtonsComposite(Composite parent) {
		// Create button composite
		Composite buttonComposite = new Composite(parent, SWT.NONE); // SWTFactory.createComposite(parent,
																		// parent.getFont(), 1, 1,
		// GridData.VERTICAL_ALIGN_BEGINNING | GridData.HORIZONTAL_ALIGN_END, 0, 0);
		GridLayout layout = new GridLayout(1, false);
		layout.marginWidth = 0;
		layout.marginHeight = 0;
		buttonComposite.setLayout(layout);

		buttonComposite.setFont(parent.getFont());

		GridData gd = new GridData(GridData.VERTICAL_ALIGN_BEGINNING | GridData.HORIZONTAL_ALIGN_END);
		gd.horizontalSpan = 1;
		buttonComposite.setLayoutData(gd);

		return buttonComposite;
	}

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

	private void handleEnvAddButtonSelected(TableViewer environmentTable) {
		MultipleInputDialog dialog = new MultipleInputDialog(getShell(),
				LaunchConfigurationsMessages.EnvironmentTab_22);
		dialog.addTextField(NAME_LABEL, null, false);
		dialog.addVariablesField(VALUE_LABEL, null, true);

		if (dialog.open() != Window.OK) {
			return;
		}

		String name = dialog.getStringValue(NAME_LABEL);
		String value = dialog.getStringValue(VALUE_LABEL);

		if (name != null && value != null && name.length() > 0 && value.length() > 0) {
			// Trim the environment variable name but *NOT* the value
			addVariable(new EnvironmentVariable(name.trim(), value), environmentTable);
		}
	}

	private void handleEnvSelectButtonSelected(TableViewer environmentTable) {
		// get Environment Variables from the OS
		Map<String, EnvironmentVariable> envVariables = getNativeEnvironment();

		// get Environment Variables from the table
		TableItem[] items = environmentTable.getTable().getItems();
		for (TableItem item : items) {
			EnvironmentVariable var = (EnvironmentVariable) item.getData();
			envVariables.remove(var.getName());
		}
	}

	private void handleEnvEditButtonSelected(TableViewer environmentTable) {
		IStructuredSelection sel = environmentTable.getStructuredSelection();
		EnvironmentVariable var = (EnvironmentVariable) sel.getFirstElement();
		if (var == null) {
			return;
		}
		String originalName = var.getName();
		String value = var.getValue();
		MultipleInputDialog dialog = new MultipleInputDialog(getShell(),
				LaunchConfigurationsMessages.EnvironmentTab_11);
		dialog.addTextField(NAME_LABEL, originalName, false);
		if (value != null && value.contains(System.lineSeparator())) {
			dialog.addMultilinedVariablesField(VALUE_LABEL, value, true);
		} else {
			dialog.addVariablesField(VALUE_LABEL, value, true);
		}

		if (dialog.open() != Window.OK) {
			return;
		}

		String name = dialog.getStringValue(NAME_LABEL);
		value = dialog.getStringValue(VALUE_LABEL);
		if (!originalName.equals(name)) {
			// Trim the environment variable name but *NOT* the value
			if (addVariable(new EnvironmentVariable(name.trim(), value), environmentTable)) {
				environmentTable.remove(var);
			}
		} else {
			var.setValue(value);
			environmentTable.update(var, null);
		}
	}

	private void handleEnvRemoveButtonSelected(TableViewer environmentTable) {
		IStructuredSelection sel = environmentTable.getStructuredSelection();
		try {
			environmentTable.getControl().setRedraw(false);
			for (Iterator<?> i = sel.iterator(); i.hasNext();) {
				EnvironmentVariable var = (EnvironmentVariable) i.next();
				environmentTable.remove(var);
			}
		} finally {
			environmentTable.getControl().setRedraw(true);
		}
	}

	private void handleEnvCopyButtonSelected(TableViewer environmentTable) {
		Iterable<?> iterable = () -> environmentTable.getStructuredSelection().iterator();
		String data = StreamSupport.stream(iterable.spliterator(), false).filter(o -> o instanceof EnvironmentVariable)
				.map(EnvironmentVariable.class::cast).map(var -> String.format("%s=%s", var.getName(), var.getValue())) //$NON-NLS-1$
				.collect(Collectors.joining(System.lineSeparator()));

		Clipboard clipboard = new Clipboard(getShell().getDisplay());
		try {
			clipboard.setContents(new Object[] { data }, new Transfer[] { TextTransfer.getInstance() });
		} finally {
			clipboard.dispose();
		}
	}

	private void handleEnvPasteButtonSelected(TableViewer environmentTable) {
		Clipboard clipboard = new Clipboard(getShell().getDisplay());
		try {
			List<EnvironmentVariable> variables = convertEnvironmentVariablesFromData(
					clipboard.getContents(TextTransfer.getInstance()));
			addVariables(variables, environmentTable);
		} finally {
			clipboard.dispose();
		}
	}

	private void handleEnvImportButtonSelected(TableViewer environmentTable) {
		ImportDialog dialog = new ImportDialog(getShell());
		if (dialog.open() == Window.OK) {
			try {
				new ProgressMonitorDialog(new Shell()).run(true, false, new IRunnableWithProgress() {
					@Override
					public void run(IProgressMonitor monitor) throws InvocationTargetException, InterruptedException {
						Map<String, String> table = new HashMap<String, String>();
						new EnvOperation(table, dialog.getResult()).run(null);
						getControl().getDisplay().asyncExec(new ImportTable(table, environmentTable));

//						table.forEach((name, value) -> {
//							getControl().getDisplay().asyncExec((new Runnable() {
//								@Override
//								public void run() {
//									addVariable(new EnvironmentVariable(name.trim(), value));
//									updateAppendReplace();
//								}
//							}));
//
//						});

					}
				});
			} catch (InvocationTargetException | InterruptedException e) {
				Platform.getLog(getClass()).error(e.toString());
			}
		}
	}

	private boolean addVariable(EnvironmentVariable variable, TableViewer environmentTable) {
		String name = variable.getName();
		TableItem[] items = environmentTable.getTable().getItems();
		for (TableItem item : items) {
			EnvironmentVariable existingVariable = (EnvironmentVariable) item.getData();
			if (existingVariable.getName().equals(name)) {

				boolean overWrite = MessageDialog.openQuestion(getShell(),
						LaunchConfigurationsMessages.EnvironmentTab_12,
						MessageFormat.format(LaunchConfigurationsMessages.EnvironmentTab_13, new Object[] { name })); //
				if (!overWrite) {
					return false;
				}
				environmentTable.remove(existingVariable);
				break;
			}
		}
		environmentTable.add(variable);
		return true;
	}

	private int addVariables(List<EnvironmentVariable> variables, TableViewer environmentTable) {
		if (variables.isEmpty()) {
			return 0;
		}

		List<EnvironmentVariable> remove = new LinkedList<>();
		List<EnvironmentVariable> conflicting = new LinkedList<>();
		Map<String, String> requested = variables.stream()
				.collect(Collectors.toMap(EnvironmentVariable::getName, EnvironmentVariable::getValue));

		for (TableItem item : environmentTable.getTable().getItems()) {
			EnvironmentVariable existingVariable = (EnvironmentVariable) item.getData();
			String name = existingVariable.getName();
			String currentValue = requested.get(name);
			if (currentValue != null) {
				remove.add(existingVariable);
				if (!currentValue.equals(existingVariable.getValue())) {
					conflicting.add(existingVariable);
				}
			}
		}

		if (!conflicting.isEmpty()) {
			String names = conflicting.stream().map(EnvironmentVariable::getName).collect(Collectors.joining(", ")); //$NON-NLS-1$
			boolean overWrite = MessageDialog.openQuestion(getShell(),
					LaunchConfigurationsMessages.EnvironmentTab_Paste_Overwrite_Title,
					MessageFormat.format(LaunchConfigurationsMessages.EnvironmentTab_Paste_Overwrite_Message,
							new Object[] { names })); //
			if (!overWrite) {
				return 0;
			}
		}

		remove.forEach(environmentTable::remove);
		variables.forEach(environmentTable::add);

		return variables.size();
	}

	private static List<EnvironmentVariable> convertEnvironmentVariablesFromData(Object data) {
		if (!(data instanceof String)) {
			return Collections.emptyList();
		}

		String entries[] = ((String) data).split("\\R"); //$NON-NLS-1$
		List<EnvironmentVariable> result = new ArrayList<>(entries.length);
		for (String entry : entries) {
			int idx = entry.indexOf('=');
			if (idx < 1) {
				continue;
			}
			// the name is trimmed ...
			String name = entry.substring(0, idx).trim();
			// .. but the value is *not* trimmed
			String value = entry.substring(idx + 1);
			result.add(new EnvironmentVariable(name, value));
		}
		return Collections.unmodifiableList(result);
	}

	private Map<String, EnvironmentVariable> getNativeEnvironment() {
		Map<String, String> stringVars = DebugPlugin.getDefault().getLaunchManager()
				.getNativeEnvironmentCasePreserved();
		HashMap<String, EnvironmentVariable> vars = new HashMap<>();
		for (Entry<String, String> entry : stringVars.entrySet()) {
			vars.put(entry.getKey(), new EnvironmentVariable(entry.getKey(), entry.getValue()));
		}
		return vars;
	}

	private void updateEnvironmentTable(Map<String, String> configuration, TableViewer environmentTable) {
		environmentTable.setInput(configuration);
	}

	private String getStringValueFromTable(TableViewer environmentTable) {
		try {
			Map<String, String> table = new HashMap<String, String>();

			EnvironmentVariable t = (EnvironmentVariable) environmentTable.getElementAt(0);

			for (int i = 1; t != null; i++) {
				table.put(t.getName(), t.getValue());
				t = (EnvironmentVariable) environmentTable.getElementAt(i);
			}
			getControl().getDisplay().asyncExec(new ImportTable(table, environmentTable));

			return (new ObjectMapper()).writerWithDefaultPrettyPrinter().writeValueAsString(table);
		} catch (IOException e) {
			return ""; //$NON-NLS-1$
		}
	}

	private void setValueToTable(String value, TableViewer environmentTable) {
		ObjectMapper mapper = new ObjectMapper();
		TypeReference<HashMap<String, String>> typeRef = new TypeReference<HashMap<String, String>>() {
		};
		try {
			Map<String, String> env = mapper.readValue(value, typeRef);
			updateEnvironmentTable(env, environmentTable);
		} catch (JsonProcessingException e) {
			updateEnvironmentTable(new HashMap<String, String>(), environmentTable);
		}
	}

	@Override
	public boolean performOk() {
		Map<String, String> table = new HashMap<String, String>();

		EnvironmentVariable t = (EnvironmentVariable) environmentTable.getElementAt(0);

		for (int i = 1; t != null; i++) {
			table.put(t.getName(), t.getValue());
			t = (EnvironmentVariable) environmentTable.getElementAt(i);
		}
		getControl().getDisplay().asyncExec(new ImportTable(table, environmentTable));

		this.store.put(PreferenceConstants.BUILD_ENV_VARS, getStringValueFromTable(environmentTable));

		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
			return false;
		}

		return true;
	}

}
