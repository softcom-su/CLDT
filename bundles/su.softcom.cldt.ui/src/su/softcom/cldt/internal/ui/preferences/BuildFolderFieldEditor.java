package su.softcom.cldt.internal.ui.preferences;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.preference.StringButtonFieldEditor;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.model.WorkbenchContentProvider;
import org.eclipse.ui.model.WorkbenchLabelProvider;
import org.eclipse.ui.views.navigator.ResourceComparator;

/**
 * A field editor for a string type preference for select build folder.
 * 
 * @author ershov_vladislav@softcom.su
 */
public class BuildFolderFieldEditor extends StringButtonFieldEditor {

	/** Target project. */
	private IProject project = null;

	/**
	 * Creates a build folder field editor with name, labelText, parent and target
	 * project.
	 *
	 * @param name      the name of the preference this field editor works on
	 * @param labelText the label text of the field editor
	 * @param parent    the parent of the field editor's control
	 * @param project   target project
	 */
	public BuildFolderFieldEditor(String name, String labelText, Composite parent, IProject project) {
		super(name, labelText, parent);
		this.project = project;
	}

	@Override
	protected String changePressed() {
		BuildFolderSelectionDialog dialog = new BuildFolderSelectionDialog(getShell(), new WorkbenchLabelProvider(),
				new WorkbenchContentProvider(), project);

		dialog.setTitle(Messages.BuildFolderFieldEditor_ChooseFolderForBuild);
		dialog.setMessage(Messages.BuildFolderFieldEditor_ChooseFolder);
		dialog.setInput(project);
		dialog.setComparator(new ResourceComparator(ResourceComparator.NAME));

		if (dialog.open() == IDialogConstants.OK_ID) {
			IResource resource = (IResource) dialog.getFirstResult();
			if (resource != null) {
				return resource.getProjectRelativePath().toOSString();
			}
		}

		return null;
	}

	@Override
	protected boolean doCheckState() {
		if (Path.EMPTY.isValidPath(getStringValue())) {
			return true;
		} else {
			setErrorMessage(Messages.CreateFolder_InvalidFolderName);
			return false;
		}
	}

}
