package su.softcom.cldt.ui.launch.tabs;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Stream;

import org.eclipse.jface.dialogs.Dialog;
import org.eclipse.jface.widgets.WidgetFactory;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Import environment variables dialog. */
public class ImportDialog extends Dialog {

	private Combo variableSet;

	private Composite scriptContainer;
	private Text scriptPath;

	private Map<Integer, Path> msvcCompilers;
	private Composite msvcContainer;
	private Combo msvcArchitecture;

	private String result;

	/**
	 * Creates a dialog instance.
	 * 
	 * @param parentShell the parent shell, or {@code null} to create a top-level
	 *                    shell
	 */
	public ImportDialog(Shell parentShell) {
		super(parentShell);

		msvcCompilers = new HashMap<Integer, Path>();
		if (CInitializer.IS_WINDOWS) {
			Path programFilesX86 = Paths.get("C:", "Program Files (x86)"); //$NON-NLS-1$ //$NON-NLS-2$

			Path msvc2003 = programFilesX86
					.resolve(Paths.get("Microsoft Visual Studio .NET 2003", "Vc7", "bin", "vcvars32.bat")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
			Path msvc2008 = programFilesX86.resolve(Paths.get("Microsoft Visual Studio 9.0", "VC", "vcvarsall.bat")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
			Path msvc2010 = programFilesX86.resolve(Paths.get("Microsoft Visual Studio 10.0", "VC", "vcvarsall.bat")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
			Path msvc2013 = programFilesX86.resolve(Paths.get("Microsoft Visual Studio 12.0", "VC", "vcvarsall.bat")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$

			if (Files.exists(msvc2003)) {
				msvcCompilers.put(2003, msvc2003);
			}
			if (Files.exists(msvc2008)) {
				msvcCompilers.put(2008, msvc2008);
			}
			if (Files.exists(msvc2010)) {
				msvcCompilers.put(2010, msvc2010);
			}
			if (Files.exists(msvc2013)) {
				msvcCompilers.put(2013, msvc2013);
			}

			Stream.of(Paths.get("C:", "Program Files (x86)", "Microsoft Visual Studio"), //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
					Paths.get("C:", "Program Files", "Microsoft Visual Studio")).flatMap(path -> { //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
						try {
							return Files.list(path);
						} catch (IOException e) {
							return null;
						}
					}).filter(path -> path.getFileName().toString().matches("\\d\\d\\d\\d")) //$NON-NLS-1$
					.map(path -> Map.entry(Integer.parseInt(path.getFileName().toString()), path))
					.flatMap(entry -> Stream
							.of(Paths.get("BuildTools"), Paths.get("Community"), Paths.get("Professional"), //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
									Paths.get("Enterprise")) //$NON-NLS-1$
							.map(path -> Map.entry(entry.getKey(),
									entry.getValue().resolve(path)
											.resolve(Paths.get("VC", "Auxiliary", "Build", "vcvarsall.bat"))))) //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
					.filter(entry -> Files.exists(entry.getValue()))
					.forEach(entry -> msvcCompilers.put(entry.getKey(), entry.getValue()));
		}
	}

	/**
	 * Get environment import script call.
	 * 
	 * @return script path and arguments
	 */
	public String getResult() {
		return result;
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		Composite container = (Composite) super.createDialogArea(parent);
		container.setLayout(new GridLayout());

		Composite set = WidgetFactory.composite(SWT.NONE).layout(new GridLayout())
				.layoutData(new GridData(GridData.FILL_HORIZONTAL)).create(container);
		WidgetFactory.label(SWT.NONE).text(Messages.ImportDialog_Variables_Label).create(set);

		variableSet = new Combo(set, SWT.READ_ONLY);
		variableSet.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		variableSet.addSelectionListener(new SelectionListener() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				updateDialog();
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				updateDialog();
			}
		});

		msvcCompilers.keySet().stream().sorted(Collections.reverseOrder())
				.forEach(version -> variableSet.add(Messages.ImportDialog_MSVC + " " + version)); //$NON-NLS-1$
		variableSet.add(Messages.ImportDialog_User_Script);
		variableSet.select(0);

		createScriptControl(container);
		createMSVCControl(container);
		updateDialog();

		return container;
	}

	@Override
	protected void configureShell(Shell newShell) {
		super.configureShell(newShell);
		newShell.setText(Messages.ImportDialog_Import_Title);
	}

	@Override
	protected Point getInitialSize() {
		Point size = super.getInitialSize();
		size.x *= 2;
		return size;
	}

	@Override
	protected void okPressed() {
		String current = variableSet.getItem(variableSet.getSelectionIndex());
		if (Messages.ImportDialog_User_Script.equals(current)) {
			result = scriptPath.getText();
		} else {
			Integer version = Integer.parseInt(current.substring(current.length() - 4, current.length()));
			String architecture = msvcArchitecture.getItem(msvcArchitecture.getSelectionIndex());

			result = msvcCompilers.get(version).toString();
			if (version != 2003) {
				if (Messages.ImportDialog_x86.equals(architecture)) {
					result = result + " x86"; //$NON-NLS-1$
				} else if (Messages.ImportDialog_x86_amd64.equals(architecture)) {
					result = result + " x86_amd64"; //$NON-NLS-1$
				} else if (Messages.ImportDialog_x86_arm.equals(architecture)) {
					result = result + " x86_arm"; //$NON-NLS-1$
				} else if (Messages.ImportDialog_x86_arm64.equals(architecture)) {
					result = result + " x86_arm64"; //$NON-NLS-1$
				} else if (Messages.ImportDialog_amd64.equals(architecture)) {
					result = result + " amd64"; //$NON-NLS-1$
				} else if (Messages.ImportDialog_amd64_arm.equals(architecture)) {
					result = result + " amd64_arm"; //$NON-NLS-1$
				} else if (Messages.ImportDialog_amd64_arm64.equals(architecture)) {
					result = result + " amd64_arm64"; //$NON-NLS-1$
				} else if (Messages.ImportDialog_amd64_x86.equals(architecture)) {
					result = result + " amd64_x86"; //$NON-NLS-1$
				}
			}
		}

		super.okPressed();
	}

	private void createScriptControl(Composite parent) {
		scriptContainer = WidgetFactory.composite(SWT.NONE).layout(new GridLayout(2, false))
				.layoutData(new GridData(GridData.FILL_HORIZONTAL)).create(parent);

		WidgetFactory.label(SWT.NONE).layoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 2, 1))
				.text(Messages.ImportDialog_Script_Label).create(scriptContainer);

		scriptPath = WidgetFactory.text(SWT.BORDER).layoutData(new GridData(GridData.FILL_HORIZONTAL))
				.create(scriptContainer);

		Button button = WidgetFactory.button(SWT.NONE).text(Messages.ImportDialog_Browse_Button)
				.create(scriptContainer);
		button.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(final SelectionEvent e) {
				FileDialog dialog = new FileDialog(getShell(), SWT.OPEN);
				dialog.setFilterExtensions(new String[] { CInitializer.IS_WINDOWS ? "*.bat" : "*.sh", "*.*" }); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				dialog.setText(Messages.ImportDialog_User_Script);

				String path = dialog.open();
				if (path != null) {
					scriptPath.setText(path);
				}
			}
		});
	}

	private void createMSVCControl(Composite parent) {
		msvcContainer = WidgetFactory.composite(SWT.NONE).layout(new GridLayout())
				.layoutData(new GridData(GridData.FILL_HORIZONTAL)).create(parent);

		WidgetFactory.label(SWT.NONE).text(Messages.ImportDialog_Architecture_Label).create(msvcContainer);

		msvcArchitecture = new Combo(msvcContainer, SWT.READ_ONLY);
		msvcArchitecture.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

		msvcArchitecture.add(Messages.ImportDialog_x86);
		msvcArchitecture.select(0);
	}

	private void updateDialog() {
		boolean isScript = Messages.ImportDialog_User_Script
				.equals(variableSet.getItem(variableSet.getSelectionIndex()));

		((GridData) scriptContainer.getLayoutData()).exclude = !isScript;
		scriptContainer.setVisible(isScript);

		((GridData) msvcContainer.getLayoutData()).exclude = isScript;
		msvcContainer.setVisible(!isScript);
		msvcContainer.getParent().layout(true, true);

		if (!isScript) {
			String msvcCurrent = variableSet.getItem(variableSet.getSelectionIndex());
			Integer msvcVersion = Integer
					.parseInt(msvcCurrent.substring(msvcCurrent.length() - 4, msvcCurrent.length()));

			msvcArchitecture.remove(1, msvcArchitecture.getItemCount() - 1);

			if (msvcVersion > 2003) {
				msvcArchitecture.add(Messages.ImportDialog_x86_amd64);
			}

			if (msvcVersion > 2010) {
				msvcArchitecture.add(Messages.ImportDialog_x86_arm);
			}

			if (msvcVersion > 2013) {
				msvcArchitecture.add(Messages.ImportDialog_x86_arm64);
			}

			if (System.getProperty("os.arch").contains("64") && msvcVersion > 2003) { //$NON-NLS-1$ //$NON-NLS-2$
				msvcArchitecture.add(Messages.ImportDialog_amd64);
				msvcArchitecture.add(Messages.ImportDialog_amd64_x86);

				if (msvcVersion > 2010) {
					msvcArchitecture.add(Messages.ImportDialog_amd64_arm);
				}

				if (msvcVersion > 2013) {
					msvcArchitecture.add(Messages.ImportDialog_amd64_arm64);
				}
			}
		}
	}

}
