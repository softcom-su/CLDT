package su.softcom.cldt.internal.ui.properties;

import java.util.List;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.preferences.DefaultScope;
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

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.ProjectDependency;
import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * External dependencies property page
 * 
 * @author pavel.desyatnikov@softcom.su
 */
@SuppressWarnings("nls")
public class DependenciesPropertyPage extends AbstractPropertyPage implements IWorkbenchPropertyPage {

	private List<ProjectDependency> dependencies;
	private ICMakeProject selectedProject;

	public DependenciesPropertyPage() {
		super();
	}

	@Override
	protected Control createContents(Composite parent) {
		Composite common = createCommon(parent);
		noDefaultButton();

		Composite tableComposite = new Composite(common, common.getStyle());
		tableComposite.setLayout(new GridLayout(1, false));
		tableComposite.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		TableViewer tableViewer = createTable(tableComposite);
		initTable(tableViewer);
		Table table = tableViewer.getTable();

		Composite buttonComposite = createButtonsComposite(common);

		Button addButton = createButton(buttonComposite, "Добавить");
		Button editButton = createButton(buttonComposite, "Редактировать");
		editButton.setEnabled(false);
		Button removeButton = createButton(buttonComposite, "Удалить");
		removeButton.setEnabled(false);

		table.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				TableItem tableItem = (TableItem) event.item;
				if (tableItem != null) {
					removeButton.setEnabled(true);
					editButton.setEnabled(table.getSelection().length == 1 ? true : false);
				} else {
					removeButton.setEnabled(false);
					editButton.setEnabled(false);
				}
			}

		});

		addButton.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				ProjectDependency dependency = null;

				DependencyDialog dialog = new DependencyDialog(getShell(), null, dependencies, selectedProject);
				if (dialog.open() == IDialogConstants.OK_ID) {
					dependency = dialog.getResult();
				}

				if (dependency != null) {
					dependencies.add(dependency);

					initTable(tableViewer);
					setSelection(table, dependency.getName());
				}
			}

		});

		editButton.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				ProjectDependency dependency = ((ProjectDependency) tableViewer.getTable().getSelection()[0].getData());
				ProjectDependency newDependency = null;

				DependencyDialog dialog = new DependencyDialog(getShell(), dependency, dependencies, selectedProject);
				if (dialog.open() == IDialogConstants.OK_ID) {
					newDependency = dialog.getResult();
				}

				if (newDependency != null) {
					dependencies.remove(dependency);
					dependencies.add(newDependency);

					initTable(tableViewer);
					setSelection(table, newDependency.getName());
				}
			}

		});

		removeButton.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				for (TableItem item : table.getSelection()) {
					dependencies.remove(item.getData());
				}

				removeButton.setEnabled(false);
				editButton.setEnabled(false);

				initTable(tableViewer);
			}

		});

		return common;
	}

	private void setSelection(Table table, String dependencyName) {
		for (TableItem item : table.getItems()) {
			if (((ProjectDependency) item.getData()).getName().equals(dependencyName)) {
				table.setSelection(item);
				break;
			}
		}
	}

	/**
	 * Creates table used in this page and returns its viewer
	 * 
	 * @param parent
	 */
	private TableViewer createTable(Composite parent) {
		TableViewer tableViewer = new TableViewer(parent,
				SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL | SWT.MULTI | SWT.FULL_SELECTION);
		tableViewer.setContentProvider(ArrayContentProvider.getInstance());
		Table table = tableViewer.getTable();

		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		TableViewerColumn nameColumnViewer = new TableViewerColumn(tableViewer, SWT.LEFT);
		nameColumnViewer.setLabelProvider(
				ColumnLabelProvider.createTextProvider(element -> ((ProjectDependency) element).getName()));

		TableViewerColumn pathColumnViewer = new TableViewerColumn(tableViewer, SWT.LEFT);
		pathColumnViewer.setLabelProvider(
				ColumnLabelProvider.createTextProvider(element -> ((ProjectDependency) element).getPath().toString()));

		TableColumn nameColumn = nameColumnViewer.getColumn();
		nameColumn.setWidth(110);
		nameColumn.setText("Имя");

		TableColumn pathColumn = pathColumnViewer.getColumn();
		pathColumn.setWidth(150);
		pathColumn.setText("Путь (опционально)");

		return tableViewer;
	}

	private void initTable(TableViewer tableViewer) {
		tableViewer.setInput(dependencies);
		tableViewer.refresh();
	}

	@Override
	public void setElement(IAdaptable element) {
		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			project = prj;
			selectedProject = CMakeCorePlugin.eINSTANCE.getProject(project);
			dependencies = selectedProject.getDependencies().stream().map(ProjectDependency::new)
					.collect(Collectors.toList());
			store = new ProjectScope(project).getNode(PreferenceConstants.NODE);
			defaults = DefaultScope.INSTANCE.getNode(PreferenceConstants.NODE);
		} else {
			throw new IllegalArgumentException("Element must be adapt to IProject"); //$NON-NLS-1$
		}

	}

	@Override
	public boolean performOk() {
		selectedProject.setDependencies(dependencies);
		return true;
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

	@Override
	protected Composite createCommon(Composite parent) {
		Composite common = super.createCommon(parent);
		common.setLayout(new GridLayout(2, false));
		return common;
	}

}
