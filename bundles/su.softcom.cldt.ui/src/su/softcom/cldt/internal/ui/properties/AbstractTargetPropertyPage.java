package su.softcom.cldt.internal.ui.properties;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.preferences.DefaultScope;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * Property page for setup build parameters
 */
public abstract class AbstractTargetPropertyPage extends AbstractPropertyPage {

	protected static String selectedTargetName;

	protected Preferences targetStore;
	protected List<String> targetsNames;
	protected ICMakeProject cmakeProject;

	protected AbstractTargetPropertyPage() {
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
			this.defaults = DefaultScope.INSTANCE.getNode(PreferenceConstants.NODE);
			this.targetStore = this.store.node(PreferenceConstants.NODE_OF_TARGETS);

			targetsNames = new ArrayList<String>(Stream.of(this.store.get(PreferenceConstants.TARGETS_LIST, "") //$NON-NLS-1$
					.split(PreferenceConstants.FIELD_SEPARATOR)).filter(str -> !str.isEmpty()).toList());
			if (targetsNames.size() == 0) {
				targetsNames.add(project.getName());
			}
			selectedTargetName = targetsNames.get(0);
		} else {
			throw new IllegalArgumentException("Element must be adapt to IProject"); //$NON-NLS-1$
		}
	}

	protected Combo createTargetCombo(Composite parent) {
		Group group = new Group(parent, SWT.NONE);
		group.setText("Цель сборки");
		group.setLayout(new GridLayout(4, false));
		group.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		Combo target = new Combo(group, SWT.DROP_DOWN | SWT.BORDER);
		target.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		target.setText(selectedTargetName);
		for (String targetName : targetsNames) {
			target.add(targetName);
		}

		return target;
	}

}
