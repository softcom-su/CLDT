package su.softcom.cldt.ui.launch.tabs;

import java.text.MessageFormat;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.ui.AbstractLaunchConfigurationTab;
import org.eclipse.debug.ui.ILaunchConfigurationTab;
import org.eclipse.debug.ui.ILaunchConfigurationTab2;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.dialogs.ElementListSelectionDialog;
import org.eclipse.ui.model.WorkbenchLabelProvider;

import su.softcom.cldt.ui.launch.AbstractLaunchConfigurationDelegate;

/** The main tab with project selection. */
public class MainTab extends AbstractLaunchConfigurationTab
		implements ILaunchConfigurationTab, ILaunchConfigurationTab2 {

	/**
	 * Project name attribute.
	 */
	public static final String PROJECT = AbstractLaunchConfigurationDelegate.PROJECT;

	/**
	 * Launchable resource attribute.
	 */
	public static final String RESOURCE = "Launchable resource"; //$NON-NLS-1$

	/**
	 * Program arguments attribute.
	 */
	public static final String ARGUMENTS = "Program arguments"; //$NON-NLS-1$

	protected Text text;
	protected Text args;

	@Override
	public void createControl(final Composite parent) {
		Composite control = new Composite(parent, SWT.NONE);
		control.setLayout(new GridLayout(1, false));
		setControl(control);

		Group group = new Group(control, SWT.NONE);
		group.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		group.setText(Messages.MainTab_Project_Group);
		group.setLayout(new GridLayout(2, false));

		text = new Text(group, SWT.BORDER);
		text.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		text.addModifyListener(new ModifyListener() {
			@Override
			public void modifyText(ModifyEvent e) {
				updateLaunchConfigurationDialog();
			}
		});

		Button button = new Button(group, SWT.NONE);
		button.setText(Messages.MainTab_Browse_Button);
		button.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				IProject[] projects = ResourcesPlugin.getWorkspace().getRoot().getProjects();
				ElementListSelectionDialog dialog = new ElementListSelectionDialog(getShell(),
						new WorkbenchLabelProvider());

				dialog.setTitle(Messages.MainTab_Selection_Title);
				dialog.setMessage(Messages.MainTab_Selection_Message);
				dialog.setElements(projects);

				if (dialog.open() == Window.OK) {
					text.setText(((IProject) dialog.getFirstResult()).getName());
					updateLaunchConfigurationDialog();
				}
			}
		});

		createExtendedControl(control);
	}

	@Override
	public void setDefaults(final ILaunchConfigurationWorkingCopy configuration) {
		configuration.setAttribute(PROJECT, ""); //$NON-NLS-1$
		configuration.setAttribute(RESOURCE, ""); //$NON-NLS-1$
		configuration.setAttribute(ARGUMENTS, ""); //$NON-NLS-1$
	}

	@Override
	public void initializeFrom(final ILaunchConfiguration configuration) {
		try {
			text.setText(configuration.getAttribute(PROJECT, "")); //$NON-NLS-1$
		} catch (CoreException e) {
			text.setText(""); //$NON-NLS-1$
		}

		try {
			args.setText(configuration.getAttribute(ARGUMENTS, "")); //$NON-NLS-1$
		} catch (CoreException e) {
			args.setText(""); //$NON-NLS-1$
		}
	}

	@Override
	public void performApply(final ILaunchConfigurationWorkingCopy configuration) {
		configuration.setAttribute(PROJECT, text.getText());
		configuration.setAttribute(ARGUMENTS, args.getText());
	}

	@Override
	public String getName() {
		return Messages.MainTab_Name;
	}

	@Override
	public Image getImage() {
		return ImageDescriptor
				.createFromURL(
						Platform.getBundle("org.eclipse.ui.externaltools").getEntry("icons/full/obj16/main_tab.png")) //$NON-NLS-1$ //$NON-NLS-2$
				.createImage();
	}

	@Override
	public boolean isValid(final ILaunchConfiguration launchConfig) {
		try {
			return checkProject(launchConfig.getAttribute(PROJECT, "")); //$NON-NLS-1$
		} catch (CoreException e) {
			return checkProject(null);
		}
	}

	@Override
	public boolean canSave() {
		return checkProject(text.getText());
	}

	/**
	 * Creates and configures the extension of main control.
	 * 
	 * @implNote Use this as the extension point. Don't forget to call the super
	 *           method, if you need an arguments field.
	 * 
	 * @param parent the composite in which the extension should be created
	 */
	protected void createExtendedControl(Composite parent) {
		Group group = new Group(parent, SWT.NONE);
		group.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		group.setText(Messages.MainTab_Arguments_Group);
		group.setLayout(new GridLayout(2, false));

		args = new Text(group, SWT.MULTI | SWT.BORDER | SWT.WRAP);
		args.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		args.addModifyListener(new ModifyListener() {
			@Override
			public void modifyText(ModifyEvent e) {
				updateLaunchConfigurationDialog();
			}
		});
	}

	private boolean checkProject(final String name) {
		if (name == null || name.equals("")) { //$NON-NLS-1$
			setErrorMessage(Messages.MainTab_Project_Not_Specified);
			return false;
		}

		try {
			if (ResourcesPlugin.getWorkspace().getRoot().getProject(name).exists()) {
				setErrorMessage(null);
				return true;
			}
		} catch (IllegalArgumentException e) {
			Platform.getLog(getClass()).error(e.getMessage());
		}

		setErrorMessage(MessageFormat.format(Messages.MainTab_Project_Not_Exist, name));
		return false;
	}
}
