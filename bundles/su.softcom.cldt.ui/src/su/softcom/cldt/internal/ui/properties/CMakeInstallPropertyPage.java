package su.softcom.cldt.internal.ui.properties;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.eclipse.core.resources.ICommand;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.preferences.DefaultScope;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.ui.IWorkbenchPropertyPage;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.internal.core.builders.CMakeInstallBuilder;
import su.softcom.cldt.internal.core.builders.CMakeProjectBuilder;

@SuppressWarnings("restriction")
public class CMakeInstallPropertyPage extends AbstractPropertyPage implements IWorkbenchPropertyPage {

	private static final String CMAKE_INSTALL_PREFIX = "CMAKE_INSTALL_PREFIX"; //$NON-NLS-1$
	private static final String CMAKE_INSTALL_LIBDIR = "CMAKE_INSTALL_LIBDIR"; //$NON-NLS-1$

	private ICMakeProject cmakeProject;
	private boolean enableInstall;
	private Map<String, String> currentArgs;

	@Override
	public void setElement(IAdaptable element) {
		super.setElement(element);
		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			project = prj;
			cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
			store = new ProjectScope(project).getNode(CMakeCorePlugin.PLUGIN_ID);
			defaults = DefaultScope.INSTANCE.getNode(CMakeCorePlugin.PLUGIN_ID);
			currentArgs = cmakeProject.getBuildArguments();

			try {
				enableInstall = List.of(prj.getDescription().getBuildSpec()).stream()
						.anyMatch(e -> e.getBuilderName().equals(CMakeInstallBuilder.ID));
			} catch (CoreException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	protected Control createContents(Composite parent) {
		parent.setLayout(new GridLayout(1, false));
		GridData commonData = new GridData(SWT.FILL, SWT.FILL, true, true);
		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(1, false));
		common.setLayoutData(commonData);
		Button btnCheckButton = new Button(common, SWT.CHECK);
		btnCheckButton.setText("Выполнять установку после сборки");
		btnCheckButton.setLayoutData(commonData);
		btnCheckButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				enableInstall = btnCheckButton.getSelection();
			}
		});
		btnCheckButton.setSelection(enableInstall);
		return common;
	}

	@Override
	public boolean performOk() {
		try {
			IProjectDescription desc = project.getDescription();
			List<ICommand> buildCommands = new ArrayList<>(List.of(desc.getBuildSpec()));
			if (enableInstall) {
				buildCommands.stream().filter(t -> t.getBuilderName().equals(CMakeInstallBuilder.ID)).findFirst()
						.ifPresentOrElse(e -> {
						}, () -> {
							ICommand cmd = buildCommands.stream()
									.filter(t -> t.getBuilderName().equals(CMakeProjectBuilder.ID)).findFirst().get();
							ICommand installCommand = desc.newCommand();
							installCommand.setBuilderName(CMakeInstallBuilder.ID);
							buildCommands.add(buildCommands.indexOf(cmd) + 1, installCommand);
						});
				currentArgs.put(CMAKE_INSTALL_PREFIX, cmakeProject.getBuildFolder()
						.getFolder(ICMakeProject.DEFAULT_INSTALL_PATH_POSTFIX).getLocation().toOSString());
				currentArgs.put(CMAKE_INSTALL_LIBDIR, "lib");
			} else {
				buildCommands.stream().filter(t -> t.getBuilderName().equals(CMakeInstallBuilder.ID)).findFirst()
						.ifPresent(buildCommands::remove);
				currentArgs.remove(CMAKE_INSTALL_PREFIX);
				currentArgs.remove(CMAKE_INSTALL_LIBDIR);
			}
			desc.setBuildSpec(buildCommands.toArray(new ICommand[0]));
			project.setDescription(desc, null);
			cmakeProject.setBuildArguments(currentArgs);
			return true;
		} catch (CoreException e) {
			e.printStackTrace();
			return false;
		}

	}
}
