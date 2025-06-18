package su.softcom.cldt.internal.ui.preferences;

import java.util.HashMap;
import java.util.Map;

import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.layout.PixelConverter;
import org.eclipse.jface.preference.PreferencePage;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPreferencePage;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.core.IToolchain;
import su.softcom.cldt.core.ToolchainManager;

public class ToolchainsPreferences extends PreferencePage implements IWorkbenchPreferencePage {

	private class TableSelectionListener implements Listener {
		@Override
		public void handleEvent(Event event) {
			Table table = tableViewer.getTable();

			TableItem tableItem = (TableItem) event.item;
			table.setSelection(tableItem);

			removeButton.setEnabled(true);
		}
	}

	private class AddButtonListener implements Listener {

		@Override
		public void handleEvent(Event event) {
			if (event.type == SWT.Selection) {
				IToolchain toolchain = null;

				AddToolchainDialog dialog = new AddToolchainDialog(getShell());
				if (dialog.open() == IDialogConstants.OK_ID) {
					toolchain = dialog.getResult();
				}

				if (toolchain != null) {
					toolchains.put(toolchain.getName(), toolchain);
					initTable();
				}
			}
		}
	}

	private class RemoveButtonListener implements Listener {

		@Override
		public void handleEvent(Event event) {
			if (event.type == SWT.Selection) {
				toolchains.remove(((IToolchain) tableViewer.getTable().getSelection()[0].getData()).getName());
				removeButton.setEnabled(false);
				initTable();
			}
		}
	}

	private ToolchainManager toolchainManager;

	private Map<String, IToolchain> toolchains;

	private TableViewer tableViewer;

	private Button addButton;
	private Button removeButton;

	public ToolchainsPreferences() {
	}

	@Override
	protected Control createContents(Composite parent) {
		parent.setLayout(new GridLayout(1, false));
		noDefaultButton();

		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(2, false));
		common.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		GridLayout layout = new GridLayout(1, false);
		layout.marginWidth = 0;
		layout.marginHeight = 0;
		Composite tableComposite = new Composite(common, SWT.TRAIL);
		tableComposite.setLayout(layout);
		tableComposite.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		createTable(tableComposite);
		initTable();
		Composite buttonsComposite = createButtonsComposite(common);
		createButtons(buttonsComposite);

		return tableComposite;
	}

	@Override
	public void init(IWorkbench workbench) {
		toolchainManager = ToolchainManager.getInstance();
		try {
			toolchains = toolchainManager.getToolchainsList();
		} catch (BackingStoreException e) {
			toolchains = new HashMap<String, IToolchain>();
		}
	}

	@Override
	public boolean performOk() {
		return toolchainManager.setToolchainsList(toolchains.values());
	}

	private void createTable(Composite parent) {
		tableViewer = new TableViewer(parent,
				SWT.BORDER | SWT.READ_ONLY | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION);
		tableViewer.setContentProvider(ArrayContentProvider.getInstance());

		TableViewerColumn viewerNameColumn = new TableViewerColumn(tableViewer, SWT.NONE);
		viewerNameColumn.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				if (element instanceof IToolchain) {
					return ((IToolchain) element).getName();
				}
				return ""; //$NON-NLS-1$
			}
		});

		TableViewerColumn viewerPathColumn = new TableViewerColumn(tableViewer, SWT.NONE | SWT.H_SCROLL);
		viewerPathColumn.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				if (element instanceof IToolchain) {
					return ((IToolchain) element).getPath().toString();
				}
				return ""; //$NON-NLS-1$
			}
		});

		Table table = tableViewer.getTable();
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		TableColumn nameColumn = viewerNameColumn.getColumn();
		nameColumn.setText("Название тулчейна"); //$NON-NLS-1$
		nameColumn.setWidth(150);

		TableColumn pathColumn = viewerPathColumn.getColumn();
		pathColumn.setText("Расположение"); //$NON-NLS-1$
		pathColumn.setWidth(300);

		table.addListener(SWT.Selection, new TableSelectionListener());
		table.addListener(SWT.DefaultSelection, new TableSelectionListener());
	}

	private Composite createButtonsComposite(Composite parent) {
		GridLayout layout = new GridLayout(1, false);
		layout.marginWidth = 0;
		layout.marginHeight = 0;

		Composite buttonsComposite = new Composite(parent, SWT.NONE);
		buttonsComposite.setLayout(layout);
		buttonsComposite.setLayoutData(new GridData(SWT.END, SWT.BEGINNING, true, true));
		buttonsComposite.setFont(parent.getFont());

		return buttonsComposite;
	}

	void createButtons(Composite parent) {
		addButton = createButton(parent, "Добавить"); //$NON-NLS-1$
		addButton.addListener(SWT.Selection, new AddButtonListener());
		removeButton = createButton(parent, "Удалить"); //$NON-NLS-1$
		removeButton.addListener(SWT.Selection, new RemoveButtonListener());
		removeButton.setEnabled(false);
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

	private void initTable() {
		tableViewer.setInput(toolchains.values());
		tableViewer.refresh();
	}
}
