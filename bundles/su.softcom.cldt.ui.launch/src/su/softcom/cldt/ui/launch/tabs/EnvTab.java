package su.softcom.cldt.ui.launch.tabs;

import java.lang.reflect.InvocationTargetException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.stream.Stream;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.ui.EnvironmentTab;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.operation.IRunnableWithProgress;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;

import su.softcom.cldt.core.operations.EnvOperation;

/** The environment table tab. */
public class EnvTab extends EnvironmentTab {

	/**
	 * Environment table attribute.
	 */
	public static final String ENVIRONMENT = ILaunchManager.ATTR_ENVIRONMENT_VARIABLES;

	protected Button envImportButton;
	private ILaunchConfigurationWorkingCopy configurationCopy;

	@Override
	public void initializeFrom(ILaunchConfiguration configuration) {
		super.initializeFrom(configuration);
		try {
			this.configurationCopy = configuration.getWorkingCopy();
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.toString());
		}
	}

	@Override
	protected void createTableButtons(Composite parent) {
		super.createTableButtons(parent);
		envImportButton = super.envSelectButton;
		envImportButton.setText(Messages.EnvTab_Import_Button);

		Arrays.stream(envImportButton.getListeners(SWT.Selection))
				.forEach(listener -> envImportButton.removeListener(SWT.Selection, listener));

		envImportButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent event) {
				handleEnvImportButtonSelected();
				updateLaunchConfigurationDialog();
			}
		});
	}

	@Override
	protected void createAppendReplace(Composite parent) {
		super.createAppendReplace(parent);
		((GridData) super.appendEnvironment.getLayoutData()).exclude = true;
		((GridData) super.replaceEnvironment.getLayoutData()).exclude = true;
	}

	/**
	 * Import environment variables to the table.
	 */
	private void handleEnvImportButtonSelected() {
		ImportDialog dialog = new ImportDialog(getShell());
		if (dialog.open() == Window.OK) {
			try {
				getLaunchConfigurationDialog().run(true, true, new IRunnableWithProgress() {
					@Override
					public void run(IProgressMonitor monitor) throws InvocationTargetException, InterruptedException {
						Map<String, String> table = new HashMap<String, String>();
						new EnvOperation(table, dialog.getResult()).run(monitor);
						getControl().getDisplay().asyncExec(new ImportTable(table));
					}
				});
			} catch (InvocationTargetException | InterruptedException e) {
				Platform.getLog(getClass()).error(e.toString());
			}
		}
	}

	/**
	 * Import environment table into UI.
	 */
	private class ImportTable implements Runnable {
		private Map<String, String> table;

		/**
		 * Creates a runnable instance.
		 * 
		 * @param table table to import
		 */
		public ImportTable(Map<String, String> table) {
			this.table = table;
		}

		@Override
		public void run() {
			try {
				performApply(configurationCopy);
				Map<String, String> current = configurationCopy.getAttribute(ENVIRONMENT,
						new HashMap<String, String>());

				current.entrySet().stream().forEach(entry -> table.putIfAbsent(entry.getKey(), entry.getValue()));
				Set<String> intersection = current.keySet();
				intersection.retainAll(table.keySet());

				if (!intersection.isEmpty()) {
					MessageDialog dialog = new MessageDialog(getShell(), Messages.EnvTab_Intersection_Title, null,
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
			} catch (CoreException e) {
				// could not found ENVIRONMENT attribute, ignore
			}

			configurationCopy.setAttribute(ENVIRONMENT, table);
			updateEnvironment(configurationCopy);
		}
	}
}
