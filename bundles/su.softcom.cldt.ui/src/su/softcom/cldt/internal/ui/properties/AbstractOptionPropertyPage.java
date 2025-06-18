package su.softcom.cldt.internal.ui.properties;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.jface.viewers.CellEditor;
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.jface.viewers.TreeViewerColumn;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.internal.ui.AddElementContentProvider;
import su.softcom.cldt.internal.ui.AddElementLabelProvider;

public abstract class AbstractOptionPropertyPage extends AbstractTargetPropertyPage {

	final String optionPropertyName;
	final String defenitionPropertyName;

	final Map<String, List<String>> options;
	final Map<String, List<String>> defines;

	static Image IMAGE_DEF = getResourceManager().createImage(createImageDescriptor("def.png")); //$NON-NLS-1$
	static Image IMAGE_OPT = getResourceManager().createImage(createImageDescriptor("option.png")); //$NON-NLS-1$

	public AbstractOptionPropertyPage(String optionProperty, String defenitionProperty) {
		this.optionPropertyName = optionProperty;
		this.defenitionPropertyName = defenitionProperty;
		options = new HashMap<>();
		defines = new HashMap<>();
	}

	@Override
	public void setElement(IAdaptable element) {
		super.setElement(element);
		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			for (String target : targetsNames) {
				Preferences ts = targetStore.node(target);
				defines.put(target, new ArrayList<>(Arrays.asList(ts.get(defenitionPropertyName, "") //$NON-NLS-1$
						.split(PreferenceConstants.FIELD_SEPARATOR)).stream().filter(Predicate.not(String::isBlank))
						.toList()));
				options.put(target, new ArrayList<>(Arrays.asList(ts.get(optionPropertyName, "") //$NON-NLS-1$
						.split(PreferenceConstants.FIELD_SEPARATOR)).stream().filter(Predicate.not(String::isBlank))
						.toList()));
			}
		}
	}

	@Override
	protected Control createContents(Composite parent) {
		Composite common = createCommon(parent);
		common.setLayout(new GridLayout(1, false));
		common.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
		Combo target = createTargetCombo(common);

		Group groupArguments = new Group(common, SWT.NONE);
		groupArguments.setLayout(new GridLayout(2, false));
		groupArguments.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		groupArguments.setText("Опции"); //$NON-NLS-1$

		Group definesGroup = new Group(common, SWT.NONE);
		definesGroup.setText("Определения"); //$NON-NLS-1$
		definesGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
		definesGroup.setLayout(new GridLayout(1, false));
		TreeViewer argViewer = createDefinesTree(groupArguments, options, IMAGE_OPT);
		TreeViewer defViewer = createDefinesTree(definesGroup, defines, IMAGE_DEF);
		argViewer.setInput(options.get(selectedTargetName));
		defViewer.setInput(defines.get(selectedTargetName));
		return common;
	}

	@Override
	public boolean performOk() {
		for (String target : targetsNames) {
			Preferences targetPrefernces = targetStore.node(target);
			targetPrefernces.put(optionPropertyName,
					options.get(target).stream().collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));
			targetPrefernces.put(defenitionPropertyName,
					defines.get(target).stream().collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));
		}
		return super.performOk();

	}

	TreeViewer createDefinesTree(Group parent, Map<String, List<String>> data, Image icon) {
		TreeViewer treeViewer = new TreeViewer(parent, SWT.BORDER);
		treeViewer.setContentProvider(new AddElementContentProvider());
		Tree tree = treeViewer.getTree();
		tree.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		TreeViewerColumn valueColumnViewer = new TreeViewerColumn(treeViewer, SWT.NONE);
		valueColumnViewer.setLabelProvider(new AddElementLabelProvider(icon));
		TreeColumn valueColumn = valueColumnViewer.getColumn();
		TextCellEditor editor = new TextCellEditor(tree);
		valueColumnViewer.setEditingSupport(new EditingSupport(treeViewer) {

			@Override
			protected CellEditor getCellEditor(Object element) {
				return editor;
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
				List<String> defList = data.get(selectedTargetName);
				if (element instanceof AddElementContentProvider.AddNewElement) {
					if (!element.toString().equals(value)) {
						defList.add(value.toString());
					}
				} else {
					if (((String) value).isBlank()) {
						defList.remove(element);
					} else {
						defList.set(defList.indexOf(element), (String) value);
					}
				}
				treeViewer.refresh();
			}
		});
		valueColumn.setWidth(300);
		return treeViewer;
	}

}
