package su.softcom.cldt.internal.ui.preferences;

import java.io.File;
import java.nio.file.Paths;
import java.util.regex.Pattern;

import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.jface.dialogs.MessageLine;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SegmentEvent;
import org.eclipse.swt.events.SegmentListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.SelectionStatusDialog;

import su.softcom.cldt.core.preferences.PreferenceConstants;

/**
 * Dialog for create folder.
 * 
 * @author ershov_vladislav@softcom.su
 */
public class CreateFolderDialog extends SelectionStatusDialog {

	/** Name folder. */
	private String nameFolder;

	/** Status line. */
	private MessageLine statusLine;

	/**
	 * Constructor for CreateFolderDialog.
	 * 
	 * @param parentShell parent shell
	 */
	public CreateFolderDialog(Shell parentShell) {
		super(parentShell);

		setStatusLineAboveButtons(true);
		setShellStyle(getShellStyle());
		setTitle(Messages.CreateFolderDialog_CreationFolder);
		setMessage(Messages.CreateFolderDialog_EnterFolderName);
	}

	/**
	 * Validate the receiver and update the ok status.
	 */
	protected void updateOKStatus() {
		if (nameFolder == "" || !checkInvalidNameFolder()) { //$NON-NLS-1$
			updateStatus(new Status(IStatus.ERROR, PlatformUI.PLUGIN_ID, IStatus.ERROR,
					Messages.CreateFolder_InvalidFolderName, null));
		} else {
			updateStatus(new Status(IStatus.OK, PlatformUI.PLUGIN_ID, IStatus.OK, "", //$NON-NLS-1$
					null));
		}
	}

	/**
	 * Check invalid name folder.
	 */
	private boolean checkInvalidNameFolder() {
		if (Pattern.matches("\\.+", nameFolder) || Pattern.matches(".*\\.", nameFolder)) { //$NON-NLS-1$ //$NON-NLS-2$
			return false;
		}

		File file = new File(nameFolder);

		try {
			Paths.get(file.toURI());
			return true;
		} catch (IllegalArgumentException e) {
			return false;
		}
	}

	@Override
	protected void updateStatus(IStatus status) {
		if (statusLine != null && !statusLine.isDisposed()) {
			updateButtonsEnableState(status);
			statusLine.setErrorStatus(status);
		}
	}

	@Override
	protected Control createDialogArea(Composite parent) {
		Composite composite = (Composite) super.createDialogArea(parent);

		createMessageArea(composite);

		statusLine = new MessageLine(composite);
		statusLine.setAlignment(SWT.LEFT);
		GridData statusData = new GridData(GridData.FILL_HORIZONTAL);
		statusLine.setErrorStatus(null);
		statusLine.setFont(parent.getFont());
		statusLine.setLayoutData(statusData);

		Text name = new Text(composite, SWT.BORDER);
		name.setText(PreferenceConstants.CLDT_PROJECT_BUILD_FOLDER_DEFAULT);
		name.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

		name.addSegmentListener(new SegmentListener() {
			@Override
			public void getSegments(SegmentEvent event) {
				nameFolder = ((Text) event.getSource()).getText();
				updateOKStatus();
			}
		});

		return composite;
	}

	/**
	 * Get name folder.
	 * 
	 * @return name folder
	 */
	public String getNameFolder() {
		return nameFolder;
	}

	@Override
	protected void computeResult() {
		// Not need yet
	}

}
