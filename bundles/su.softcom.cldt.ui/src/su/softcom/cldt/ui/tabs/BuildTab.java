package su.softcom.cldt.ui.tabs;

import java.io.File;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import su.softcom.cldt.nativebuild.launch.BuildLaunchConfigurationDelegate;

public final class BuildTab extends MainTab {

	public static final String COMPILER_LOCATION = BuildLaunchConfigurationDelegate.COMPILER_LOCATION;

	private Text fTxtCompiler;

	public BuildTab(String[] extensions, String TabName) {
		super(extensions, TabName);
	}

	@Override
	public void initializeFrom(final ILaunchConfiguration configuration) {
		setFDisableUpdate(true);

		getFTxtFile().setText(""); //$NON-NLS-1$
		fTxtCompiler.setText(""); //$NON-NLS-1$

		try {
			getFTxtFile().setText(configuration.getAttribute(FILE_LOCATION, "")); //$NON-NLS-1$
		} catch (CoreException e) {
			e.printStackTrace();
			Platform.getLog(getClass()).error(e.getStackTrace().toString());
		}

		try {
			fTxtCompiler.setText(configuration.getAttribute(COMPILER_LOCATION, "")); //$NON-NLS-1$
		} catch (CoreException e) {
			e.printStackTrace();
			Platform.getLog(getClass()).error(e.getStackTrace().toString());
		}

		setFDisableUpdate(false);
	}

	@Override
	public void performApply(final ILaunchConfigurationWorkingCopy configuration) {

		configuration.setAttribute(FILE_LOCATION, getFTxtFile().getText());

		File Compf = new File(fTxtCompiler.getText());
		if (Compf.exists() && Compf.isFile()) {
			configuration.setAttribute(COMPILER_LOCATION, fTxtCompiler.getText());
		}

	}

	@Override
	public void createControl(final Composite parent) {
		Composite topControl = new Composite(parent, SWT.NONE);
		topControl.setLayout(new GridLayout(1, false));

		ScriptGroup(parent, topControl);

		Group grpCompiler = new Group(topControl, SWT.NONE);
		grpCompiler.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		grpCompiler.setText("Компилятор:");

		grpCompiler.setLayout(new GridLayout(2, false));

		fTxtCompiler = new Text(grpCompiler, SWT.BORDER);
		fTxtCompiler.addModifyListener(new ModifyListener() {

			@Override
			public void modifyText(final ModifyEvent e) {
				if (!getFDisableUpdate()) {
					updateLaunchConfigurationDialog();
				}
			}

		});
		fTxtCompiler.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

		Button btnBrowseCompiler = new Button(grpCompiler, SWT.NONE);
		btnBrowseCompiler.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				FileDialog fileDialog = new FileDialog(parent.getShell());
				fTxtCompiler.setText(fileDialog.open());
			}
		});
		btnBrowseCompiler.setText("Обзор");

		Label label = new Label(topControl, SWT.HORIZONTAL | SWT.VERTICAL | SWT.CENTER);
		label.setText("Предупреждение: указанный скрипт должен принимать в качестве первого аргумента\n"
				+ "путь к папке сборки и опционально вторым аргументом путь к компилятору. ");
		label.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));

		setControl(topControl);
	}

}
