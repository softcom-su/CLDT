package su.softcom.cldt.ui.launch.tabs;

import java.net.URI;
import java.util.Arrays;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerFilter;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.dialogs.ElementTreeSelectionDialog;
import org.eclipse.ui.dialogs.ISelectionStatusValidator;
import org.eclipse.ui.model.WorkbenchContentProvider;
import org.eclipse.ui.model.WorkbenchLabelProvider;

/** The main tab with bash script selection. */
public class BashMainTab extends MainTab {

	/**
	 * Bash script URI attribute.
	 */
	public static final String SCRIPT_URI = "Script URI"; //$NON-NLS-1$

	protected Text scriptText;
	protected Button searchButton;

	@Override
	protected void createExtendedControl(Composite parent) {
		Group group = new Group(parent, SWT.NONE);
		group.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		group.setText(Messages.BashMainTab_Script_Group);
		group.setLayout(new GridLayout(2, false));

		scriptText = new Text(group, SWT.BORDER);
		scriptText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		scriptText.setEditable(false);
		scriptText.addModifyListener(new ModifyListener() {
			@Override
			public void modifyText(ModifyEvent e) {
				updateLaunchConfigurationDialog();
			}
		});

		searchButton = new Button(group, SWT.NONE);
		searchButton.setText(Messages.BashMainTab_Search_Button);
		searchButton.setEnabled(false);
		searchButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				ElementTreeSelectionDialog dialog = new ElementTreeSelectionDialog(getShell(),
						new WorkbenchLabelProvider(), new WorkbenchContentProvider());

				dialog.setTitle(Messages.BashMainTab_Selection_Title);
				dialog.setMessage(Messages.BashMainTab_Selection_Message);
				dialog.setInput(ResourcesPlugin.getWorkspace().getRoot().getProject(text.getText()));
				dialog.setAllowMultiple(false);

				dialog.addFilter(new FileExtensionFilter("sh")); //$NON-NLS-1$
				dialog.setValidator(new ISelectionStatusValidator() {
					@Override
					public IStatus validate(Object[] selection) {
						return Arrays.stream(selection).allMatch(IFile.class::isInstance)
								? new Status(IStatus.OK, getClass(), null)
								: new Status(IStatus.ERROR, getClass(), null);
					}
				});

				if (dialog.open() == Window.OK) {
					scriptText.setText(((IFile) dialog.getFirstResult()).getProjectRelativePath().toString());
					updateLaunchConfigurationDialog();
				}
			}
		});

		super.createExtendedControl(parent);
		text.addModifyListener(new ModifyListener() {
			@Override
			public void modifyText(ModifyEvent e) {
				updateScriptGroup();
				updateLaunchConfigurationDialog();
			}
		});
	}

	@Override
	public void setDefaults(ILaunchConfigurationWorkingCopy configuration) {
		super.setDefaults(configuration);
		configuration.setAttribute(SCRIPT_URI, ""); //$NON-NLS-1$
	}

	@Override
	public void initializeFrom(ILaunchConfiguration configuration) {
		super.initializeFrom(configuration);

		try {
			IFile script = getScript(configuration.getAttribute(SCRIPT_URI, "")); //$NON-NLS-1$
			scriptText.setText(script != null ? script.getProjectRelativePath().toString() : ""); //$NON-NLS-1$
		} catch (CoreException e) {
			args.setText(""); //$NON-NLS-1$
		}
	}

	@Override
	public void performApply(ILaunchConfigurationWorkingCopy configuration) {
		super.performApply(configuration);
		configuration.setAttribute(SCRIPT_URI, getScriptURI(text.getText(), scriptText.getText()));
	}

	@Override
	public boolean isValid(ILaunchConfiguration launchConfig) {
		try {
			return super.isValid(launchConfig) && checkScript(launchConfig.getAttribute(SCRIPT_URI, "")); //$NON-NLS-1$
		} catch (CoreException e) {
			return checkScript(null);
		}
	}

	@Override
	public boolean canSave() {
		return super.canSave() && checkScript(getScriptURI(text.getText(), scriptText.getText()));
	}

	/**
	 * Returns script {@link IFile} by string {@link URI}.
	 * 
	 * @param scriptURI extracted from {@link ILaunchConfiguration}
	 * 
	 * @return script resource, or {@code null}
	 */
	public static IFile getScript(String scriptURI) {
		try {
			return Arrays.stream(ResourcesPlugin.getWorkspace().getRoot().findFilesForLocationURI(new URI(scriptURI)))
					.filter(IResource::exists).findFirst().get();
		} catch (Exception e) {
			return null;
		}
	}

	/**
	 * Updates the enablement of the script group. The widgets should disable when
	 * there are no project specified.
	 */
	protected void updateScriptGroup() {
		boolean isProject = super.canSave();
		scriptText.setEditable(isProject);
		searchButton.setEnabled(isProject);
	}

	private static String getScriptURI(String projectName, String relativePath) {
		if (projectName == null || projectName.isEmpty() || relativePath == null || relativePath.isEmpty()) {
			return null;
		}

		IResource resource = ResourcesPlugin.getWorkspace().getRoot().getProject(projectName);

		for (String path : relativePath.split("/")) { //$NON-NLS-1$
			if (resource instanceof IContainer) {
				resource = ((IContainer) resource).findMember(path);
			}
		}

		return resource != null ? resource.getLocationURI().toString() : null;
	}

	private boolean checkScript(String scriptURI) {
		if (scriptURI != null && !scriptURI.isEmpty()) {
			IFile script = getScript(scriptURI);

			if (script != null && script.exists() && "sh".equals(script.getFileExtension())) { //$NON-NLS-1$
				setErrorMessage(null);
				return true;
			}
		}

		setErrorMessage(Messages.BashMainTab_Wrong_Script);
		return false;
	}

	/** File extension filter. */
	private class FileExtensionFilter extends ViewerFilter {

		private String extension;

		/**
		 * Creates a new file extension filter.
		 * 
		 * @param extension allowed extension
		 */
		public FileExtensionFilter(String extension) {
			this.extension = extension;
		}

		@Override
		public boolean select(Viewer viewer, Object parentElement, Object element) {
			if (element instanceof IFile) {
				return extension.equals(((IFile) element).getFileExtension());
			}

			if (element instanceof IProject && !((IProject) element).isOpen()) {
				return false;
			}

			if (element instanceof IContainer) {
				try {
					return Arrays.stream(((IContainer) element).members())
							.anyMatch(member -> select(viewer, parentElement, member));
				} catch (CoreException e) {
					Platform.getLog(getClass()).error(e.toString());
				}
			}

			return false;
		}
	}

}
