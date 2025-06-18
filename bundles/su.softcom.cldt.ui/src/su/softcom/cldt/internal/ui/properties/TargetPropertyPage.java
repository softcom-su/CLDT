package su.softcom.cldt.internal.ui.properties;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.preferences.DefaultScope;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.layout.PixelConverter;
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
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.PropertyPage;
import org.eclipse.ui.navigator.CommonNavigator;
import org.eclipse.ui.navigator.CommonViewer;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.builders.LanguageStandartManager;
import su.softcom.cldt.core.builders.LanguageStandartManager.ILanguageStandart;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.internal.ui.preferences.TargetDialog;

public class TargetPropertyPage extends PropertyPage implements IWorkbenchPropertyPage {

	protected ICMakeProject cmakeProject;
	protected IEclipsePreferences store;
	protected IEclipsePreferences defaults;

	private Map<String, Target> targets;

	public TargetPropertyPage() {
		super();
	}

	@Override
	public IAdaptable getElement() {
		return cmakeProject.getProject();
	}

	@Override
	public void setElement(IAdaptable element) {
		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(prj);
			store = new ProjectScope(cmakeProject.getProject()).getNode(PreferenceConstants.NODE);
			defaults = DefaultScope.INSTANCE.getNode(PreferenceConstants.NODE);
			targets = new HashMap<>();
			Stream.of(this.store.get(PreferenceConstants.TARGETS_LIST, "") //$NON-NLS-1$
					.split(PreferenceConstants.FIELD_SEPARATOR)).filter(str -> !str.isEmpty()).forEach(targetName -> {
						targets.put(targetName, new Target(targetName, cmakeProject));
					});
		} else {
			throw new IllegalArgumentException("Element must be adapt to IProject"); //$NON-NLS-1$
		}
	}

	private List<ILanguageStandart> getStandards() {
		return LanguageStandartManager.getAvailableLanguageStandarts(
				store.get(PreferenceConstants.PROJECT_SELECTED_LANGUAGE, PreferenceConstants.LANGUAGE_CXX));
	}

	@Override
	protected Control createContents(Composite parent) {
		parent.setLayout(new GridLayout(1, false));
		noDefaultButton();

		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(2, false));
		common.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));

		TableViewer tableViewer = createTable(common);
		Table table = tableViewer.getTable();
		initTable(tableViewer);
		packColumns(table);

		Composite buttonsComposite = createButtonsComposite(common);
		Button addButton = createButton(buttonsComposite, "Добавить");
		Button removeButton = createButton(buttonsComposite, "Удалить");
		removeButton.setEnabled(false);
		Button copyButton = createButton(buttonsComposite, "Копировать");
		copyButton.setEnabled(false);
		Button editButton = createButton(buttonsComposite, "Изменить");
		editButton.setEnabled(false);

		table.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				TableItem tableItem = (TableItem) event.item;
				if (tableItem != null) {
					table.setSelection(tableItem);

					removeButton.setEnabled(true);
					copyButton.setEnabled(true);
					editButton.setEnabled(true);
				} else {
					removeButton.setEnabled(false);
					copyButton.setEnabled(false);
					editButton.setEnabled(false);
				}
			}

		});

		addButton.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				Target target = null;

				TargetDialog dialog = new TargetDialog(getShell(), null, getStandards(), cmakeProject);
				if (dialog.open() == IDialogConstants.OK_ID) {
					target = dialog.getResult();
				}

				if (target != null && !targets.keySet().contains(target.getName())) {
					targets.put(target.getName(), target);

					initTable(tableViewer);
					setSelection(table, target.getName());
				}
			}

		});

		removeButton.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				targets.remove(((Target) tableViewer.getTable().getSelection()[0].getData()).getName());

				removeButton.setEnabled(false);
				copyButton.setEnabled(false);
				editButton.setEnabled(false);

				initTable(tableViewer);
			}

		});

		copyButton.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				Target target = ((Target) tableViewer.getTable().getSelection()[0].getData());
				String copyName = createCopyName(target.getName());
				Target newTarget = new Target(copyName, cmakeProject, target.getType(), target.getLanguageStandard());
				targets.put(copyName, newTarget);

				initTable(tableViewer);
				setSelection(table, copyName);
			}

		});

		editButton.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				Target target = ((Target) tableViewer.getTable().getSelection()[0].getData());

				Target newTarget = null;

				TargetDialog dialog = new TargetDialog(getShell(), target, getStandards(), cmakeProject);
				if (dialog.open() == IDialogConstants.OK_ID) {
					newTarget = dialog.getResult();
				}

				if (newTarget != null) {
					if (targets.containsKey(newTarget.getName()) && !newTarget.getName().equals(target.getName())) {
						newTarget.setName(createCopyName(newTarget.getName()));
					}
					targets.remove(target.getName());
					targets.put(newTarget.getName(), newTarget);

					initTable(tableViewer);
					setSelection(table, newTarget.getName());
				}
			}

		});

		return common;
	}

	private TableViewer createTable(Composite parent) {
		GridLayout layout = new GridLayout(1, false);
		layout.marginWidth = 0;
		layout.marginHeight = 0;
		GridData data = new GridData(GridData.FILL_BOTH);
		data.horizontalSpan = 1;

		Composite tableComposite = new Composite(parent, parent.getStyle());
		tableComposite.setLayout(layout);
		tableComposite.setLayoutData(data);
		tableComposite.setFont(parent.getFont());

		TableViewer tableViewer = new TableViewer(tableComposite,
				SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL | SWT.READ_ONLY | SWT.MULTI | SWT.FULL_SELECTION);
		tableViewer.setContentProvider(ArrayContentProvider.getInstance());

		Table table = tableViewer.getTable();
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setLayoutData(new GridData(GridData.FILL_BOTH));

		TableViewerColumn viewerColumn1 = new TableViewerColumn(tableViewer, SWT.NONE);
		viewerColumn1.setLabelProvider(ColumnLabelProvider.createTextProvider(element -> ((Target) element).getName()));

		TableColumn column1 = viewerColumn1.getColumn();
		column1.setText("Имя цели");

		TableViewerColumn viewerColumn2 = new TableViewerColumn(tableViewer, SWT.NONE);
		viewerColumn2.setLabelProvider(
				ColumnLabelProvider.createTextProvider(element -> ((Target) element).getType().name()));

		TableColumn column2 = viewerColumn2.getColumn();
		column2.setText("Тип цели");

		TableViewerColumn viewerColumn4 = new TableViewerColumn(tableViewer, SWT.NONE);
		viewerColumn4.setLabelProvider(ColumnLabelProvider.createTextProvider(element -> LanguageStandartManager
				.getLanguageStandartFromPreferenceString(((Target) element).getLanguageStandard()).getUIName()));

		TableColumn column4 = viewerColumn4.getColumn();
		column4.setText("Стандарт языка:");

		// TODO Table fixes
		// Create third column with width weight = 0
		// Add multi selection
		// Fix table columns width

		return tableViewer;
	}

	private Composite createButtonsComposite(Composite parent) {
		GridLayout layout = new GridLayout(1, false);
		layout.marginWidth = 0;
		layout.marginHeight = 0;
		GridData data = new GridData(GridData.VERTICAL_ALIGN_BEGINNING | GridData.HORIZONTAL_ALIGN_END);
		data.horizontalSpan = 1;

		Composite buttonsComposite = new Composite(parent, SWT.NONE);
		buttonsComposite.setLayout(layout);
		buttonsComposite.setLayoutData(data);
		buttonsComposite.setFont(parent.getFont());

		return buttonsComposite;
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

	private void initTable(TableViewer tableViewer) {
		tableViewer.setInput(targets.values());

		tableViewer.refresh();
	}

	private void packColumns(Table table) {
		for (TableColumn column : table.getColumns()) {
			column.pack();
		}
	}

	private String createCopyName(String name) {
		String copyName;
		int i = 1;
		do {
			copyName = name + "(" + i++ + ")"; //$NON-NLS-1$//$NON-NLS-2$
		} while (targets.keySet().contains(copyName));

		return copyName;
	}

	private void setSelection(Table table, String name) {
		for (TableItem tableItem : table.getItems()) {

			if (((Target) tableItem.getData()).getName().equals(name)) {
				table.setSelection(tableItem);
				break;
			}
		}
	}

	@Override
	public boolean performOk() {
		store.put(PreferenceConstants.TARGETS_LIST, String.join(PreferenceConstants.FIELD_SEPARATOR, targets.keySet()));

		for (Target target : targets.values()) {
			Preferences targetPreferences = store.node(PreferenceConstants.NODE_OF_TARGETS).node(target.getName());

			targetPreferences.put(PreferenceConstants.BUILD_TYPE, target.getType().name());
			targetPreferences.put(PreferenceConstants.TARGET_LANGUAGE_STANDARD,
					target.getLanguageStandard().equals(TargetDialog.USE_DEFAULT_LANGUAGE_STANDARD)
							? PreferenceConstants.TARGET_USES_DEFAULT_LANGUAGE_STANDARD
							: LanguageStandartManager.getLanguageStandartFromUIName(target.getLanguageStandard())
									.getPreferenceString());
		}

		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
			return false;
		}

		// Set new targets list to cmake projectS
		cmakeProject.setTargets(targets.keySet().stream().map(targetName -> new Target(targetName, cmakeProject))
				.collect(Collectors.toList()));

		// Refresh project explorer to get the new target shown there
		CommonNavigator navigator = (CommonNavigator) PlatformUI.getWorkbench().getActiveWorkbenchWindow()
				.getActivePage().findView("org.eclipse.ui.navigator.ProjectExplorer");
		CommonViewer viewer = navigator.getCommonViewer();
		viewer.getControl().getDisplay().asyncExec(() -> {
			viewer.refresh(cmakeProject.getProject());
		});

		return true;
	}

}
