package su.softcom.cldt.internal.ui.properties;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.dialogs.PropertyPage;

import su.softcom.cldt.common.preferences.PreferenceConstants;

public class EmulatorPropertyPage extends PropertyPage {

	Button addBtn;
	Button remBtn;
	Table table;

	IProject project;
	List<String> functionNames;
	IEclipsePreferences store;
	TableViewer tableViewer;

	private class EmulPage extends Dialog {

		private Button okButton;

		private Text targetNameText;

		private String ret;

		public EmulPage(Shell parentShell, String str) {
			super(parentShell);
			this.ret = str;
		}

		@Override
		protected void buttonPressed(int buttonId) {
			if (buttonId == IDialogConstants.OK_ID) {
				this.ret = targetNameText.getText();
			} else {
				this.ret = null;
			}
			super.buttonPressed(buttonId);
		}

		@Override
		protected void configureShell(Shell shell) {
			super.configureShell(shell);
			shell.setText(this.ret == null ? "Добавление функции для эмуляции" : "Редактирование функции для эмуляции");
		}

		@Override
		protected void createButtonsForButtonBar(Composite parent) {
			okButton = createButton(parent, IDialogConstants.OK_ID, IDialogConstants.OK_LABEL, true);
			createButton(parent, IDialogConstants.CANCEL_ID, IDialogConstants.CANCEL_LABEL, false);
		}

		@Override
		protected Control createDialogArea(Composite parent) {
			Composite composite = (Composite) super.createDialogArea(parent);

			Label label = new Label(composite, SWT.WRAP);
			label.setText("Заполните поля");
			GridData data = new GridData(GridData.GRAB_HORIZONTAL | GridData.GRAB_VERTICAL
					| GridData.HORIZONTAL_ALIGN_FILL | GridData.VERTICAL_ALIGN_CENTER);
			data.widthHint = convertHorizontalDLUsToPixels(IDialogConstants.MINIMUM_MESSAGE_AREA_WIDTH);
			label.setLayoutData(data);
			label.setFont(parent.getFont());

			Composite nameGroup = new Composite(composite, SWT.NONE);
			nameGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, false, false, 1, 1));
			nameGroup.setLayout(new GridLayout(4, true));

			Label nameLabel = new Label(nameGroup, SWT.NONE);
			nameLabel.setText("Функция:");

			this.targetNameText = new Text(nameGroup, SWT.BORDER);
			this.targetNameText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 3, 1));
			this.targetNameText.setText(ret != null ? ret : ""); //$NON-NLS-1$

			return composite;
		}

		protected Button getOkButton() {
			return okButton;
		}

		public String getResult() {
			return this.ret;
		}
	}

	public EmulatorPropertyPage() {
		functionNames = new ArrayList<String>();
	}

	void initFunctionNames() {
		functionNames.addAll(Stream.of(
				store.get(PreferenceConstants.EMULATOR_FUNCTION_TARGETS, "").split(PreferenceConstants.FIELD_SEPARATOR))
				.filter(name -> {
					return !name.isBlank();
				}).toList());
	}

	private void initTable() {
		tableViewer.setInput(functionNames);

		tableViewer.refresh();
	}

	@Override
	protected Control createContents(Composite parent) {
		parent.setLayout(new GridLayout(1, false));

		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(2, false));
		common.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));

		GridLayout layout = new GridLayout(1, false);
		layout.marginWidth = 0;
		layout.marginHeight = 0;
		GridData data = new GridData(GridData.FILL_BOTH);
		data.horizontalSpan = 1;

		Composite tableComposite = new Composite(common, parent.getStyle());
		tableComposite.setLayout(layout);
		tableComposite.setLayoutData(data);
		tableComposite.setFont(common.getFont());

		tableViewer = new TableViewer(tableComposite,
				SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL | SWT.READ_ONLY | SWT.MULTI | SWT.FULL_SELECTION);
		table = tableViewer.getTable();
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setLayoutData(new GridData(GridData.FILL_BOTH));

		tableViewer.addSelectionChangedListener(new ISelectionChangedListener() {

			@Override
			public void selectionChanged(SelectionChangedEvent event) {
				if (!event.getSelection().isEmpty()) {
					remBtn.setEnabled(true);
				} else {
					remBtn.setEnabled(false);
				}
			}
		});

		TableViewerColumn viewerColumn1 = new TableViewerColumn(tableViewer, SWT.NONE);
		viewerColumn1.setLabelProvider(ColumnLabelProvider.createTextProvider(element -> {
			return (String) element;
		}));

		TableColumn column1 = viewerColumn1.getColumn();
		column1.setText("Имя функции");
		column1.pack();

		Composite composite = new Composite(common, SWT.NONE);
		composite.setLayout(new GridLayout(1, false));

		addBtn = new Button(composite, SWT.NONE);
		addBtn.setText("Добавить функцию");
		addBtn.pack();
		addBtn.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				String emulFunc = null;

				EmulPage dialog = new EmulPage(getShell(), null);
				if (dialog.open() == IDialogConstants.OK_ID) {
					emulFunc = dialog.getResult();
					if (emulFunc != null && !emulFunc.isBlank()) {
						if (!functionNames.contains(emulFunc)) {
							functionNames.add(emulFunc);
							initTable();
						}
					}
				}

			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// TODO Auto-generated method stub

			}
		});

		remBtn = new Button(composite, SWT.NONE);
		remBtn.setText("Удалить функцию");
		remBtn.pack();
		remBtn.setEnabled(false);
		remBtn.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				TableItem[] ti = table.getSelection();
				if (ti.length > 0) {
					functionNames.remove(ti[0].getText());
					initTable();
				} else {
					Platform.getLog(this.getClass())
							.warn("(unexpectadly)Clicking remove function name while none are selected.");
				}
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				// TODO Auto-generated method stub

			}
		});

		new Label(common, SWT.NONE);
		new Label(common, SWT.NONE);
		tableViewer.setContentProvider(ArrayContentProvider.getInstance());

//		new ArrayList<String>();
		tableViewer.setInput(functionNames);

		return null;
	}

	@Override
	public void setElement(IAdaptable element) {
		super.setElement(element);
		project = element.getAdapter(IProject.class);
		store = new ProjectScope(project).getNode(PreferenceConstants.NODE);
		initFunctionNames();
	}

	@Override
	public boolean performOk() {

		store.put(PreferenceConstants.EMULATOR_FUNCTION_TARGETS,
				functionNames.stream().collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));
		return true;
	}
}
