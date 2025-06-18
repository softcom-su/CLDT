package su.softcom.cldt.internal.ui.properties;

import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.JFaceResources;
import org.eclipse.jface.resource.LocalResourceManager;
import org.eclipse.jface.resource.ResourceManager;
import org.eclipse.jface.viewers.CheckStateChangedEvent;
import org.eclipse.jface.viewers.CheckboxTreeViewer;
import org.eclipse.jface.viewers.DelegatingStyledCellLabelProvider;
import org.eclipse.jface.viewers.DelegatingStyledCellLabelProvider.IStyledLabelProvider;
import org.eclipse.jface.viewers.ICheckStateListener;
import org.eclipse.jface.viewers.ICheckStateProvider;
import org.eclipse.jface.viewers.IToolTipProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.StyledString;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.common.preferences.PreferenceConstants;

public class OptimizationPropertyPage extends AbstractTargetPropertyPage implements IWorkbenchPropertyPage {

	static ImageDescriptor createImageDescriptor(String fileName) {
		Bundle bundle = FrameworkUtil.getBundle(ViewLabelProvider.class);
		URL url = FileLocator.find(bundle, new Path("icons" + IPath.SEPARATOR + "view16" + IPath.SEPARATOR + fileName), //$NON-NLS-1$//$NON-NLS-2$
				null);
		return ImageDescriptor.createFromURL(url);
	}

	/**
	 * Provides labels (as RichText) & images from tree elements
	 */
	class ViewLabelProvider extends LabelProvider implements IStyledLabelProvider, IToolTipProvider {

		private static ResourceManager resourceManager;

		/**
		 * Source file image
		 */
		static Image srcIcon = getResourceManager().createImage(SOURCE_IMAGE_DESCRIPTOR);

		/**
		 * Folder image
		 */
		static Image folderIcon = getResourceManager().createImage(FODLER_IMAGE_DESCRIPTOR);

		@Override
		public StyledString getStyledText(Object element) {

			if (element instanceof IProject tProject) {
				return new StyledString(tProject.getName());
			} else if (element instanceof IFolder tFolder) {
				return new StyledString(tFolder.getName());
			} else if (element instanceof IFile tFile) {
				return new StyledString(tFile.getName());
			}
			return new StyledString(element.toString());
		}

		@Override
		public Image getImage(Object element) {
			if (element instanceof IFolder group) {
				return folderIcon;
			} else if (element instanceof IFile item) {
				return srcIcon;
			}
			return super.getImage(element);
		}

		@Override
		public void dispose() {
			// garbage collect system resources
			if (resourceManager != null) {
				resourceManager.dispose();
				resourceManager = null;
			}
		}

		/**
		 * Utility RM accessor
		 */
		protected static ResourceManager getResourceManager() {
			if (resourceManager == null) {
				resourceManager = new LocalResourceManager(JFaceResources.getResources());
			}
			return resourceManager;
		}

		@Override
		public String getToolTipText(Object element) {
			return null;
		}
	}

	protected static ImageDescriptor SOURCE_IMAGE_DESCRIPTOR;
	protected static ImageDescriptor FODLER_IMAGE_DESCRIPTOR;

	private Map<String, Boolean> enableOpt;
	private Map<String, List<IFile>> filesToOptimize;

	private Map<String, Set<String>> selectedFiles;
	private Map<String, List<String>> preferencesSourceList;
	private Map<String, List<String>> preferencesHeadersList;
	private Map<String, List<IResource>> selected;
	private Map<String, List<String>> listOfFilesFromPrefs;

	public OptimizationPropertyPage() {
		super();

		SOURCE_IMAGE_DESCRIPTOR = createImageDescriptor("c_file_obj.png"); //$NON-NLS-1$
		FODLER_IMAGE_DESCRIPTOR = createImageDescriptor("folder.png"); //$NON-NLS-1$

		enableOpt = new HashMap<String, Boolean>();
		filesToOptimize = new HashMap<String, List<IFile>>();

		selectedFiles = new HashMap<String, Set<String>>();
		preferencesSourceList = new HashMap<String, List<String>>();
		preferencesHeadersList = new HashMap<String, List<String>>();
		selected = new HashMap<String, List<IResource>>();
		listOfFilesFromPrefs = new HashMap<String, List<String>>();
	}

	@Override
	public void setElement(IAdaptable element) {
		super.setElement(element);

		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			for (String target : this.targetsNames) {
				Preferences ts = this.targetStore.node(target);

				if (!ts.get(PreferenceConstants.SRC_FILES, "").isBlank()) { //$NON-NLS-1$
					preferencesSourceList.put(target, new ArrayList<String>(Arrays.asList(
							ts.get(PreferenceConstants.SRC_FILES, "").split(PreferenceConstants.FIELD_SEPARATOR)))); //$NON-NLS-1$
				} else {
					preferencesSourceList.put(target, new ArrayList<String>());
				}
				if (!ts.get(PreferenceConstants.FILES_TO_OPTIMIZE, "").isBlank()) { //$NON-NLS-1$
					listOfFilesFromPrefs.put(target,
							new ArrayList<String>(List.of(ts.get(PreferenceConstants.FILES_TO_OPTIMIZE, "") //$NON-NLS-1$
									.split(PreferenceConstants.FIELD_SEPARATOR))));
				} else {
					listOfFilesFromPrefs.put(target, new ArrayList<String>());
				}

				selected.put(target, new ArrayList<IResource>());

				this.enableOpt.put(target, ts.getBoolean(PreferenceConstants.ENABLE_OPT, false));

				this.filesToOptimize.put(target, new ArrayList<IFile>());
				this.selectedFiles.put(target, new HashSet<String>());
				Stream.of(ts.get(PreferenceConstants.FILES_TO_OPTIMIZE, "") //$NON-NLS-1$
						.split(PreferenceConstants.FIELD_SEPARATOR)).filter(str -> !str.isEmpty()).forEach(file -> {
							if (!file.isBlank()) {
								this.filesToOptimize.get(target).add(project.getFile(file));
							}

							this.selectedFiles.get(target).add(file);
						});
			}

			this.enableOpt.put("", false); //$NON-NLS-1$
		}
	}

	@Override
	protected Control createContents(Composite parent) {
		Composite common = createCommon(parent);
		Combo target = createTargetCombo(common);

		Composite composite_1 = new Composite(common, SWT.NONE);
		composite_1.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		composite_1.setLayout(new GridLayout(1, false));

		Button optEnable = new Button(composite_1, SWT.CHECK);
		optEnable.setText("Использовать программно-аппаратную оптимизацию");

		CheckboxTreeViewer treeViewer = new CheckboxTreeViewer(composite_1, SWT.BORDER | SWT.CHECK);
		treeViewer.setContentProvider(new ITreeContentProvider() {

			@Override
			public Object[] getElements(Object inputElement) {
				if (inputElement instanceof IProject tProject) {
					Set<IResource> set = new HashSet<IResource>();
					if (preferencesSourceList.get(selectedTargetName) != null) {
						for (String ts : preferencesSourceList.get(selectedTargetName)) {
							IFile tf = tProject.getFile(ts);
							IPath tp = tf.getFullPath();
							tp.makeRelativeTo(project.getFullPath());
							IFolder tFolder = tProject.getFolder(tp.segment(1));
							if (tFolder.exists()) {
								set.add(tProject.getFolder(tp.segment(1)));
							} else {
								set.add(tf);
							}
						}
					}
					return set.toArray();
				} else if (inputElement instanceof IFolder tFolder) {
					Set<IResource> set = new HashSet<IResource>();
					if (preferencesSourceList.get(selectedTargetName) != null) {
						for (String ts : preferencesSourceList.get(selectedTargetName)) {
							IFile tf = tFolder.getFile(ts);
							IPath tp = tf.getFullPath();
							tp = tp.makeRelativeTo(project.getFullPath());
							IFolder retFolder = tFolder.getFolder(tp.segment(1));
							if (retFolder.exists()) {
								set.add(retFolder);
							} else {
								set.add(tf);
							}
						}
					}
					return set.toArray();
				}
				return null;
			}

			@Override
			public Object[] getChildren(Object parentElement) {
				if (parentElement instanceof IProject tProject) {
					return preferencesSourceList.get(selectedTargetName).toArray();
				} else if (parentElement instanceof String tString) {
					return null;
				} else if (parentElement instanceof IFolder tFolder) {
					Set<IResource> set = new HashSet<IResource>();
					if (preferencesSourceList.get(selectedTargetName) != null) {
						for (String ts : preferencesSourceList.get(selectedTargetName)) {
							IFile tf = project.getFile(ts);
							if (tFolder.getFullPath().isPrefixOf(tf.getFullPath())) {
								IPath tp = tf.getFullPath().makeRelativeTo(tFolder.getFullPath());
								IFolder retFolder = tFolder.getFolder(tp.segment(0));
								if (retFolder.exists()) {
									set.add(retFolder);
								} else {
									if (selectedFiles.get(selectedTargetName)
											.contains(tf.getProjectRelativePath().toPortableString())) {
										selected.get(selectedTargetName).add(tf);
									}
									set.add(tf);
								}
							}
						}
					}
					treeViewer.setCheckedElements(selected.get(selectedTargetName).toArray());
					return set.toArray();
				}
				return null;
			}

			@Override
			public Object getParent(Object element) {
				return null;
			}

			@Override
			public boolean hasChildren(Object element) {
				if (element instanceof IFolder) {
					return true;
				}
				return false;
			}

		});
		treeViewer.setLabelProvider(new DelegatingStyledCellLabelProvider(new ViewLabelProvider()));
		treeViewer.addCheckStateListener(new ICheckStateListener() {

			@Override
			public void checkStateChanged(CheckStateChangedEvent event) {
				treeViewer.setSubtreeChecked(event.getElement(), event.getChecked());

				filesToOptimize.put(selectedTargetName, new ArrayList<IFile>());
				for (Object o : treeViewer.getCheckedElements()) {
					if (o instanceof IFile file) {
						filesToOptimize.get(selectedTargetName).add(file);
					}
				}
			}

		});
		treeViewer.setCheckStateProvider(new ICheckStateProvider() {
			@Override
			public boolean isChecked(Object element) {
				if (element instanceof IFile file) {
					if (listOfFilesFromPrefs.get(selectedTargetName)
							.contains(file.getProjectRelativePath().toPortableString())) {
						listOfFilesFromPrefs.remove(file.getProjectRelativePath().toPortableString());
						return true;
					}
				}
				return false;
			}

			@Override
			public boolean isGrayed(Object element) {
				return false;
			}
		});

		optEnable.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				if (e.getSource() instanceof Button b) {
					enableOpt.put(selectedTargetName, b.getSelection());
					treeViewer.getTree().setEnabled(b.getSelection());
				}

			}
		});
		optEnable.setSelection(this.enableOpt.get(selectedTargetName));

		Tree tree = treeViewer.getTree();
		tree.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));
		tree.setEnabled(this.enableOpt.get(selectedTargetName));

		treeViewer.setInput(project);
		if (this.filesToOptimize.get(selectedTargetName) != null) {
			treeViewer.setCheckedElements(this.filesToOptimize.get(selectedTargetName).toArray());
		}

		target.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				selectedTargetName = target.getText();

				optEnable.setSelection(enableOpt.get(selectedTargetName));
				tree.setEnabled(enableOpt.get(selectedTargetName));

				treeViewer.setInput(project);
				treeViewer.setCheckedElements(filesToOptimize.get(selectedTargetName).toArray());
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				widgetSelected(e);
			}

		});

		return common;

	}

	@Override
	public boolean performOk() {
		for (String target : this.targetsNames) {
			Preferences ts = this.targetStore.node(target);

			Set<String> listToAdd = new HashSet<String>();
			for (IFile file : this.filesToOptimize.get(target)) {
				listToAdd.add(file.getProjectRelativePath().toPortableString());
			}
			listToAdd.addAll(listOfFilesFromPrefs.get(target));

			String toAdd = listToAdd.stream().collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR));
			ts.put(PreferenceConstants.FILES_TO_OPTIMIZE, toAdd);

			ts.putBoolean(PreferenceConstants.ENABLE_OPT, this.enableOpt.get(target));
		}

		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}
		return true;
	}

}
