package su.softcom.cldt.internal.ui.properties;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.viewers.CellEditor;
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.Text;

import su.softcom.cldt.core.ProjectDependency;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.internal.ui.AddElementContentProvider;
import su.softcom.cldt.internal.ui.AddElementLabelProvider;

@SuppressWarnings("nls")
public class DependencyDialog extends Dialog {

	public static Image IMAGE_LIB = AbstractPropertyPage.getResourceManager()
			.createImage(AbstractPropertyPage.createImageDescriptor("external_libraries.png"));
	public static String DEFAULT_LABEL_TEXT = "Параметры внешней зависимости:";
	public static String ERROR_SUCH_NAME_ALREADY_EXISTS = "Ошибка: зависимость с данным именем уже существует";
	private ICMakeProject project;
	private ProjectDependency dependency;
	private List<String> existingDependenciesNames;

	private Text dependencyNameText;
	private Text dependencyPathText;
	private Text dependencyVersionText;
	private boolean dependencyIsRequired;
	private List<String> dependencyModules;
	private Button okButton;
	private Label header;

	public DependencyDialog(Shell parentShell, ProjectDependency dependency,
			List<ProjectDependency> existingDependencies, ICMakeProject project) {
		super(parentShell);
		this.dependency = dependency;
		this.existingDependenciesNames = existingDependencies.stream().map(ProjectDependency::getName).toList();
		this.project = project;
		dependencyModules = dependency != null && dependency.getModules() != null
				? new ArrayList<>(dependency.getModules())
				: new ArrayList<>();
	}

	public ProjectDependency getResult() {
		return dependency;
	}

	@Override
	protected void buttonPressed(int buttonId) {
		if (buttonId == IDialogConstants.OK_ID) {
			String name = dependencyNameText.getText();
			IPath path = new Path(dependencyPathText.getText());
			String version = dependencyVersionText.getText();
			dependency = new ProjectDependency(project, name, path, dependencyIsRequired, Optional.ofNullable(version),
					dependencyModules);
		} else {
			dependency = null;
		}
		super.buttonPressed(buttonId);
	}

	@Override
	protected void configureShell(Shell shell) {
		super.configureShell(shell);
		shell.setSize(new Point(500, 500));
		shell.setText(dependency == null ? "Добавление внешней зависимости" : "Редактирование внешней зависимости");
	}

	@Override
	protected void createButtonsForButtonBar(Composite parent) {
		okButton = createButton(parent, IDialogConstants.OK_ID, IDialogConstants.OK_LABEL, true);
		if (dependency == null) {
			okButton.setEnabled(false);
		}
		createButton(parent, IDialogConstants.CANCEL_ID, IDialogConstants.CANCEL_LABEL, false);
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		Composite composite = (Composite) super.createDialogArea(parent);

		header = new Label(composite, SWT.WRAP);
		header.setText(DEFAULT_LABEL_TEXT);
		GridData data = new GridData(SWT.FILL, SWT.FILL, true, false);
		data.widthHint = convertHorizontalDLUsToPixels(IDialogConstants.MINIMUM_MESSAGE_AREA_WIDTH);
		header.setLayoutData(data);

		Composite nameGroup = new Composite(composite, SWT.NONE);
		nameGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
		nameGroup.setLayout(new GridLayout(4, false));

		Label nameLabel = new Label(nameGroup, SWT.NONE);
		nameLabel.setText("Название зависимости:");

		dependencyNameText = new Text(nameGroup, SWT.BORDER);
		dependencyNameText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 3, 1));
		dependencyNameText.setText(dependency != null ? dependency.getName() : "");
		dependencyNameText.addModifyListener(event -> validateName());

		Composite pathGroup = new Composite(composite, SWT.NONE);
		pathGroup.setLayout(new GridLayout(4, false));
		pathGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));

		Label pathLabel = new Label(pathGroup, SWT.None);
		pathLabel.setText("Путь (опционально):");

		dependencyPathText = new Text(pathGroup, SWT.BORDER);
		dependencyPathText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 2, 1));
		dependencyPathText.setText(dependency != null ? dependency.getPath().toString() : "");

		Button overviewButton = new Button(pathGroup, SWT.FLAT);
		overviewButton.setText("Обзор");
		overviewButton.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				DirectoryDialog directoryDialog = new DirectoryDialog(getShell(), SWT.SHEET);
				String filepath = directoryDialog.open();
				if (filepath != null) {
					dependencyPathText.setText(filepath);
				}
			}
		});

		Composite versionGroup = new Composite(composite, SWT.NONE);
		versionGroup.setLayout(new GridLayout(4, false));
		versionGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));

		Label versionLabel = new Label(versionGroup, SWT.None);
		versionLabel.setText("Версия (опционально):");

		dependencyVersionText = new Text(versionGroup, SWT.BORDER);
		dependencyVersionText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, 3, 1));
		dependencyVersionText.setText(
				dependency != null && dependency.getVersion().isPresent() ? dependency.getVersion().get() : "");

		Composite isRequiredGroup = new Composite(composite, SWT.NONE);
		isRequiredGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
		isRequiredGroup.setLayout(new GridLayout(1, false));

		Button isRequiredButton = new Button(isRequiredGroup, SWT.CHECK);
		isRequiredButton.setText("Зависимость обязательна (REQUIRED)");
		isRequiredButton.addListener(SWT.Selection, event -> {
			dependencyIsRequired = isRequiredButton.getSelection();
		});

		Composite tableGroup = new Composite(composite, SWT.NONE);
		tableGroup.setLayout(new GridLayout(1, false));
		tableGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		TableViewer tableViewer = new TableViewer(tableGroup,
				SWT.BORDER | SWT.H_SCROLL | SWT.V_SCROLL | SWT.MULTI | SWT.FULL_SELECTION);
		tableViewer.setContentProvider(new AddElementContentProvider());

		Table table = tableViewer.getTable();
		table.setHeaderVisible(true);
		table.setLinesVisible(true);
		table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		TableViewerColumn moduleColumnViewer = new TableViewerColumn(tableViewer, SWT.FILL);
		moduleColumnViewer.setLabelProvider(new AddElementLabelProvider(IMAGE_LIB));

		TableColumn moduleColumn = moduleColumnViewer.getColumn();
		moduleColumn.setWidth(100);
		moduleColumn.setText("Требуемые модули внешней зависимости");

		moduleColumnViewer.setEditingSupport(new EditingSupport(tableViewer) {

			@Override
			protected CellEditor getCellEditor(Object element) {
				return new TextCellEditor(table);
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
				if (dependencyModules.contains(value.toString()) || value.toString().equals("Добавить")) {
					return;
				}

				if (!value.toString().isBlank()) {
					dependencyModules.add(value.toString());
				}
				dependencyModules.remove(element.toString());

				tableViewer.refresh();
			}

		});

		tableViewer.setInput(dependencyModules);
		tableViewer.refresh();
		return composite;
	}

	protected Button getOkButton() {
		return okButton;
	}

	private void validateName() {
		String initialName = dependency != null ? dependency.getName() : "";
		String newName = dependencyNameText.getText();
		if (newName.isBlank()) {
			okButton.setEnabled(false);
			return;
		}

		for (String dependencyName : existingDependenciesNames) {
			if (dependencyName.equals(newName) && !dependencyName.equals(initialName)) {
				header.setText(ERROR_SUCH_NAME_ALREADY_EXISTS);
				okButton.setEnabled(false);
				return;
			}
		}
		header.setText(DEFAULT_LABEL_TEXT);
		okButton.setEnabled(true);
	}

}
