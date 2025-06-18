package su.softcom.cldt.internal.ui.preferences;

import org.eclipse.core.runtime.preferences.DefaultScope;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPreferencePage;
import org.eclipse.ui.dialogs.PropertyPage;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.common.preferences.PreferenceConstants;

/**
 * Preference page for C/C++ preferences.
 * 
 * @author ershov_vladislav@softcom.su creator
 */
public class CPreferences extends PropertyPage implements IWorkbenchPreferencePage {
	public CPreferences() {
	}

	class ShowExplorerHandler implements Listener {

		Text text;

		ShowExplorerHandler(Text text) {
			this.text = text;
		}

		@Override
		public void handleEvent(Event event) {
			if (event.type == SWT.Selection) {
				DirectoryDialog dd = new DirectoryDialog(getShell(), SWT.SHEET);
				dd.setText("");
				String filename = dd.open();
				if (filename != null) {
					text.setText(filename);
				}
			}
		}
	}

	IEclipsePreferences defaults;
	IEclipsePreferences store;
	Composite common;

	Text clangPath;
	Text ninjaPath;
	Text cmakePath;

	@Override
	public void init(IWorkbench workbench) {
		defaults = DefaultScope.INSTANCE.getNode(PreferenceConstants.NODE);
		store = InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE);
	}

	@Override
	protected Control createContents(Composite parent) {
		parent.setLayout(new GridLayout(1, false));
		common = new Composite(parent, SWT.None);
		GridData commonData = new GridData();
		commonData.grabExcessVerticalSpace = true;
		commonData.verticalAlignment = SWT.FILL;
		commonData.grabExcessHorizontalSpace = true;
		commonData.horizontalAlignment = SWT.FILL;
		commonData.horizontalSpan = 0;
		commonData.verticalSpan = 0;
		common.setLayoutData(commonData);
		common.setLayout(new GridLayout(1, false));
		Group group = new Group(common, SWT.NONE);
		group.setText("Инструменты сборки");
		group.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		group.setLayout(new GridLayout(3, false));
		createCMakeEditor(group);
		createNinjaEditor(group);
		createClangEditor(group);
		return common;
	}

	private void createClangEditor(Composite parent) {
		Label lblNewLabel = new Label(parent, SWT.NONE);
		lblNewLabel.setText("Путь установки clang");
		clangPath = new Text(parent, SWT.BORDER);
		clangPath.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		clangPath.setText(store.get(PreferenceConstants.CLANG_PATH, defaults.get(PreferenceConstants.CLANG_PATH, "")));
		Button btnNewButton = new Button(parent, SWT.NONE);
		btnNewButton.setText("Обзор");
		btnNewButton.addListener(SWT.Selection, new ShowExplorerHandler(clangPath));
	}

	private void createNinjaEditor(Composite parent) {
		Label lblNewLabel = new Label(parent, SWT.NONE);
		lblNewLabel.setText("Путь установки Ninja");
		ninjaPath = new Text(parent, SWT.BORDER);
		ninjaPath.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		ninjaPath.setText(store.get(PreferenceConstants.NINJA_PATH, defaults.get(PreferenceConstants.NINJA_PATH, "")));
		Button btnNewButton = new Button(parent, SWT.NONE);
		btnNewButton.setText("Обзор");
		btnNewButton.addListener(SWT.Selection, new ShowExplorerHandler(ninjaPath));
	}

	private void createCMakeEditor(Composite parent) {
		Label lblNewLabel = new Label(parent, SWT.NONE);
		lblNewLabel.setText("Путь установки cmake");
		cmakePath = new Text(parent, SWT.BORDER);
		cmakePath.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		cmakePath.setText(store.get(PreferenceConstants.CMAKE_PATH, defaults.get(PreferenceConstants.CMAKE_PATH, "")));
		Button btnNewButton = new Button(parent, SWT.NONE);
		btnNewButton.setText("Обзор");
		btnNewButton.addListener(SWT.Selection, new ShowExplorerHandler(cmakePath));
	}

	@Override
	public boolean performOk() {
		try {
			store.put(PreferenceConstants.CMAKE_PATH, cmakePath.getText());
			store.put(PreferenceConstants.NINJA_PATH, ninjaPath.getText());
			store.put(PreferenceConstants.CLANG_PATH, clangPath.getText());
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	@Override
	public void performDefaults() {
		clangPath.setText(defaults.get(PreferenceConstants.CLANG_PATH, ""));
		ninjaPath.setText(defaults.get(PreferenceConstants.NINJA_PATH, ""));
		cmakePath.setText(defaults.get(PreferenceConstants.CMAKE_PATH, ""));
		return;
	}

}
