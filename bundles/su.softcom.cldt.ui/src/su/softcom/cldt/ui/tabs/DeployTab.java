package su.softcom.cldt.ui.tabs;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;

public final class DeployTab extends MainTab {

	public DeployTab(String[] extensions, String TabName) {
		super(extensions, TabName);
		// TODO Auto-generated constructor stub
	}

	@Override
	public void initializeFrom(final ILaunchConfiguration configuration) {
		setFDisableUpdate(true);

		getFTxtFile().setText("");
		try {
			getFTxtFile().setText(configuration.getAttribute(FILE_LOCATION, ""));
		} catch (CoreException e) {
			e.printStackTrace();
			Platform.getLog(getClass()).error(e.getStackTrace().toString());
		}

		setFDisableUpdate(false);
	}

	@Override
	public void performApply(final ILaunchConfigurationWorkingCopy configuration) {
		configuration.setAttribute(FILE_LOCATION, getFTxtFile().getText());
	}

	@Override
	public void createControl(final Composite parent) {
		Composite topControl = new Composite(parent, SWT.NONE);
		topControl.setLayout(new GridLayout(1, false));

		ScriptGroup(parent, topControl);

		Label label = new Label(topControl, SWT.HORIZONTAL | SWT.VERTICAL | SWT.CENTER);
		label.setText("Предупреждение: Указанный скрипт должен принимать в качестве первого аргумента"
				+ " путь к папке выгрузки.");
		label.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		setControl(topControl);

	}
}
