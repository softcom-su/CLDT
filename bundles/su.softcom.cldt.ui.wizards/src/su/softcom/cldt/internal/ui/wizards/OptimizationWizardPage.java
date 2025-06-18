package su.softcom.cldt.internal.ui.wizards;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Vector;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.Adapters;
import org.eclipse.jface.layout.GridLayoutFactory;
import org.eclipse.jface.viewers.ICheckStateListener;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.dialogs.FileSystemElement;
import org.eclipse.ui.ide.dialogs.ResourceTreeAndListGroup;
import org.eclipse.ui.model.IWorkbenchAdapter;
import org.eclipse.ui.model.WorkbenchContentProvider;
import org.eclipse.ui.model.WorkbenchLabelProvider;
import org.eclipse.ui.model.WorkbenchViewerComparator;

/**
 * Implementation of a optimization wizard page.
 * 
 * @author ershov_vladislav@softcom.su
 */
public class OptimizationWizardPage extends WizardPage {

	/** Selected project for obfuscation. */
	private IProject selectedProject;

	/** The current file selection. */
	private Vector<IFile> fileSelection;

	/**
	 * The <code>selectionGroup</code> field should have been created with a private
	 * modifier. Subclasses should not access this field directly.
	 *
	 * @noreference This field is not intended to be referenced by clients.
	 */
	private ResourceTreeAndListGroup selectionGroup;

	/**
	 * Creates an instance of this class.
	 * 
	 * @param title     for page
	 * @param selection which call obfuscation
	 */
	protected OptimizationWizardPage(String title, IStructuredSelection selection) {
		super(title);
		setTitle(title);
		setMessage(Messages.OptimizationWizardPage_SelectFiles);
		setPageComplete(false);

		if (selection != null) {
			Object selected = selection.getFirstElement();

			if (selected instanceof IProject project) {
				this.selectedProject = project;
			} else {
				IProject adaptedProject = Adapters.adapt(selected, IProject.class);
				if (adaptedProject != null && adaptedProject.exists()) {
					this.selectedProject = adaptedProject;
				}
			}
		}

		this.fileSelection = new Vector<IFile>();
	}

	@Override
	public void createControl(Composite parent) {
		Composite composite = new Composite(parent, SWT.NONE);
		composite.setLayout(GridLayoutFactory.fillDefaults().create());

		createFileSelectionGroup(composite);

		setControl(composite);
	}

	/**
	 * Create the import source selection widget.
	 * 
	 * @param parent composite parent
	 */
	private void createFileSelectionGroup(Composite parent) {
		// Just create with a dummy root.
		this.selectionGroup = new ResourceTreeAndListGroup(parent, new FileSystemElement("Dummy", null, true), //$NON-NLS-1$
				getFolderProvider(), new WorkbenchLabelProvider(), getFileProvider(), new WorkbenchLabelProvider(),
				SWT.NONE, true);

		ICheckStateListener listener = event -> updateWidgetEnablements();

		WorkbenchViewerComparator comparator = new WorkbenchViewerComparator();
		this.selectionGroup.setTreeComparator(comparator);
		this.selectionGroup.setListComparator(comparator);
		this.selectionGroup.setRoot(selectedProject);
		this.selectionGroup.addCheckStateListener(listener);

	}

	/** Check if widgets are enabled or disabled by a change in the dialog. */
	private void updateWidgetEnablements() {
		fileSelection.clear();
		for (Object object : selectionGroup.getAllCheckedListItems()) {
			if (object instanceof IFile) {
				fileSelection.add((IFile) object);
			}
		}

		setPageComplete(selectionGroup.getCheckedElementCount() != 0);
		if (selectionGroup.getCheckedElementCount() != 0) {
			setErrorMessage(null);
		} else {
			setErrorMessage(Messages.OptimizationWizardPage_NotSelectedFiles);
		}
	}

	/**
	 * Returns a content provider that returns only files as children.
	 * 
	 * @return ITreeContentProvider content provider
	 */
	private ITreeContentProvider getFileProvider() {
		String[] allowExtensions = { "c" }; //$NON-NLS-1$
		return new WorkbenchContentProvider() {
			@Override
			public Object[] getChildren(Object element) {
				IWorkbenchAdapter adapter = getAdapter(element);

				ArrayList<Object> children = new ArrayList<Object>();
				if (adapter != null) {
					for (Object object : adapter.getChildren(element)) {
						if (object != null && object instanceof IFile
								&& Arrays.asList(allowExtensions).contains(((IFile) object).getFileExtension())) {
							children.add(object);
						}
					}
				}

				return children.toArray();
			}
		};
	}

	/**
	 * Returns a content provider that returns only folders as children.
	 * 
	 * @return ITreeContentProvider content provider
	 */
	private ITreeContentProvider getFolderProvider() {
		return new WorkbenchContentProvider() {
			@Override
			public Object[] getChildren(Object element) {
				IWorkbenchAdapter adapter = getAdapter(element);

				ArrayList<Object> children = new ArrayList<Object>();
				if (adapter != null) {
					for (Object object : adapter.getChildren(element)) {
						if (object instanceof IFolder || object instanceof IProject) {
							children.add(object);
						}
					}
				}

				return children.toArray();
			}
		};
	}

	/**
	 * Get files selected for obfuscation.
	 * 
	 * @return selected files
	 */
	public Vector<IFile> getImportedFiles() {
		return fileSelection;
	}

}
