package su.softcom.cldt.internal.ui.properties;

import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.JFaceResources;
import org.eclipse.jface.resource.LocalResourceManager;
import org.eclipse.jface.resource.ResourceManager;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.DelegatingStyledCellLabelProvider;
import org.eclipse.jface.viewers.DelegatingStyledCellLabelProvider.IStyledLabelProvider;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.IToolTipProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.ITreeSelection;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.StructuredSelection;
import org.eclipse.jface.viewers.StyledString;
import org.eclipse.jface.viewers.TreePath;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.ElementTreeSelectionDialog;
import org.eclipse.ui.model.WorkbenchLabelProvider;
import org.eclipse.ui.views.navigator.ResourceComparator;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.build.BuildPathGroup;
import su.softcom.cldt.core.build.ComponentBuildPathGroup;
import su.softcom.cldt.core.build.HeaderBuildPathGroup;
import su.softcom.cldt.core.build.IBuildPathGroup;
import su.softcom.cldt.core.build.SourceBuildPathGroup;
import su.softcom.cldt.core.build.StandartLibBuildPathGroup;
import su.softcom.cldt.internal.ui.preferences.Messages;
import su.softcom.cldt.ui.dialogs.ComponentSelectionDialog;
import su.softcom.cldt.ui.dialogs.ExternalLibrarySelectionDialog;
import su.softcom.cldt.ui.dialogs.FileSelectionDialog;
import su.softcom.cldt.ui.dialogs.FolderSelectionDialog;

public class LibraryPropertyPage extends AbstractTargetPropertyPage implements IWorkbenchPropertyPage {

	public class TreeContentProvider implements ITreeContentProvider {
		@Override
		public boolean hasChildren(Object element) {
			return element instanceof IBuildPathGroup;
		}

		@Override
		public Object getParent(Object element) {
			if (sourceGroup.get(selectedTargetName).getResources().contains(element)) {
				return sourceGroup;
			}
			if (headerGroup.get(selectedTargetName).getResources().contains(element)) {
				return headerGroup;
			}
			if (librariesGroup.get(selectedTargetName).getResources().contains(element)) {
				return librariesGroup;
			}
			if (externalGroup.get(selectedTargetName).getResources().contains(element)) {
				return externalGroup;
			}
			if (driversGroup.get(selectedTargetName).getResources().contains(element)) {
				return driversGroup;
			}
			return null;
		}

		@Override
		public Object[] getElements(Object inputElement) {
			return ArrayContentProvider.getInstance().getElements(inputElement);
		}

		@Override
		public Object[] getChildren(Object parentElement) {
			if (parentElement instanceof StandartLibBuildPathGroup group) {
				return group.getStringResources().toArray();
			} else if (parentElement instanceof IBuildPathGroup group) {
				ArrayList<Object> resultList = new ArrayList<>(group.getResources());
				return resultList.toArray();
			}
			return null;
		}
	}

	class ViewLabelProvider extends LabelProvider implements IStyledLabelProvider, IToolTipProvider {

		private static ResourceManager resourceManager;

		static Image folderIcon = getResourceManager().createImage(createImageDescriptor("fldr_obj.png")); //$NON-NLS-1$
		static Image srcIcon = getResourceManager().createImage(createImageDescriptor("c_file_obj.png")); //$NON-NLS-1$
		static Image hIcon = getResourceManager().createImage(createImageDescriptor("h_file_obj.png")); //$NON-NLS-1$
		static Image libIcon = getResourceManager().createImage(createImageDescriptor("library.png")); //$NON-NLS-1$

		@Override
		public StyledString getStyledText(Object element) {

			if (element instanceof IBuildPathGroup group) {
				return new StyledString(group.getName());
			} else if (element instanceof IResource item) {
				return new StyledString(item.getName());
			} else if (element instanceof String item) {
				return new StyledString(item);
			}
			return new StyledString(element.toString());
		}

		@Override
		public Image getImage(Object element) {
			if (element instanceof IBuildPathGroup group) {
				return getResourceManager().createImage(groupIcons.get(group.getClass()));
			} else if (element instanceof IResource item) {
				PlatformUI.getWorkbench().getEditorRegistry().getImageDescriptor(item.getName());
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

	private static final String LIBRARIES_GROUP = "Библиотеки";
	private static final String DRIVERS_GROUP = "Драйверы";

	private Map<String, IBuildPathGroup> sourceGroup;
	private Map<String, IBuildPathGroup> headerGroup;
	private Map<String, IBuildPathGroup> librariesGroup;
	private Map<String, IBuildPathGroup> externalGroup;
	private Map<String, IBuildPathGroup> driversGroup;

	@SuppressWarnings("rawtypes")
	private Map<Class, ImageDescriptor> groupIcons;

	/**
	 * Default constructor
	 */
	@SuppressWarnings("rawtypes")
	public LibraryPropertyPage() {
		super();

		this.sourceGroup = new HashMap<String, IBuildPathGroup>();
		this.headerGroup = new HashMap<String, IBuildPathGroup>();
		this.librariesGroup = new HashMap<String, IBuildPathGroup>();
		this.driversGroup = new HashMap<String, IBuildPathGroup>();
		this.externalGroup = new HashMap<String, IBuildPathGroup>();

		groupIcons = new HashMap<Class, ImageDescriptor>();
		groupIcons.put(SourceBuildPathGroup.class, createImageDescriptor("source_group.png")); //$NON-NLS-1$
		groupIcons.put(HeaderBuildPathGroup.class, createImageDescriptor("header_group.png")); //$NON-NLS-1$
		groupIcons.put(ComponentBuildPathGroup.class, createImageDescriptor("local_libs.png")); //$NON-NLS-1$
		groupIcons.put(ComponentBuildPathGroup.class, createImageDescriptor("drivers.png")); //$NON-NLS-1$
		groupIcons.put(StandartLibBuildPathGroup.class, createImageDescriptor("external.png")); //$NON-NLS-1$
	}

	static ImageDescriptor createImageDescriptor(String fileName) {
		Bundle bundle = FrameworkUtil.getBundle(ViewLabelProvider.class);
		URL url = FileLocator.find(bundle, new Path("icons" + IPath.SEPARATOR + "view16" + IPath.SEPARATOR + fileName), //$NON-NLS-1$//$NON-NLS-2$
				null);
		return ImageDescriptor.createFromURL(url);
	}

	@Override
	public void setElement(IAdaptable element) {
		super.setElement(element);

		IProject prj = element.getAdapter(IProject.class);
		if (prj != null) {
			this.sourceGroup.put("", new SourceBuildPathGroup()); //$NON-NLS-1$
			this.headerGroup.put("", new HeaderBuildPathGroup()); //$NON-NLS-1$
			this.librariesGroup.put("", new ComponentBuildPathGroup(LIBRARIES_GROUP)); //$NON-NLS-1$
			this.driversGroup.put("", new ComponentBuildPathGroup(DRIVERS_GROUP)); //$NON-NLS-1$
			this.externalGroup.put("", new StandartLibBuildPathGroup()); //$NON-NLS-1$

			for (String target : this.targetsNames) {
				Preferences ts = this.targetStore.node(target);

				IBuildPathGroup source = new SourceBuildPathGroup();
				IBuildPathGroup header = new HeaderBuildPathGroup();
				IBuildPathGroup libraries = new ComponentBuildPathGroup(LIBRARIES_GROUP);
				IBuildPathGroup drivers = new ComponentBuildPathGroup(DRIVERS_GROUP);
				StandartLibBuildPathGroup external = new StandartLibBuildPathGroup();

				Stream.of(ts.get(PreferenceConstants.SRC_FILES, "") //$NON-NLS-1$
						.split(PreferenceConstants.FIELD_SEPARATOR)).filter(str -> !str.isEmpty()).forEach(path -> {
							if (path.length() > 0) {
								source.addResource(Path.fromOSString(path));
							}
						});

				Stream.of(ts.get(PreferenceConstants.LOCAL_INC_FILES, "") //$NON-NLS-1$
						.split(PreferenceConstants.FIELD_SEPARATOR)).filter(str -> !str.isEmpty()).forEach(path -> {
							try {
								header.addResource(Path.fromOSString(path));
							} catch (IllegalArgumentException e) {
								Platform.getLog(getClass()).error(e.getLocalizedMessage());
								e.printStackTrace();
							}
						});

				Stream.of(ts.get(PreferenceConstants.EXTERNAL_LIBRARIES, "") //$NON-NLS-1$
						.split(PreferenceConstants.FIELD_SEPARATOR)).filter(str -> !str.isEmpty()).forEach(path -> {
							if (path.length() > 0) {
								external.addResource(path);
							}
						});

				Stream.of(ts.get(PreferenceConstants.LIBRARIES, "") //$NON-NLS-1$
						.split(PreferenceConstants.FIELD_SEPARATOR)).filter(str -> !str.isEmpty()).forEach(path -> {
							if (path.length() > 0) {
								libraries.addResource(Path.fromOSString(path));
							}
						});

				Stream.of(ts.get(PreferenceConstants.DRIVERS, "") //$NON-NLS-1$
						.split(PreferenceConstants.FIELD_SEPARATOR)).filter(str -> !str.isEmpty()).forEach(path -> {
							if (path.length() > 0) {
								drivers.addResource(Path.fromOSString(path));
							}
						});

				this.sourceGroup.put(target, source);
				this.headerGroup.put(target, header);
				this.librariesGroup.put(target, libraries);
				this.driversGroup.put(target, drivers);
				this.externalGroup.put(target, external);
			}
		}
	}

	@Override
	protected Control createContents(Composite parent) {
		Composite common = createCommon(parent);
		Combo target = createTargetCombo(common);

		Group group = new Group(common, SWT.NONE);
		group.setLayout(new GridLayout(2, false));
		group.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));

		TreeViewer treeViewer = createTree(group);

		Composite buttonsComposite = createButtonsComposite(group);
		Button addFromComponent = createButton(buttonsComposite, "Компонент");
		Button addLocal = createButton(buttonsComposite, "Компонент решения");
		Button addExternal = createButton(buttonsComposite, "Из файловой системы");
		Button delete = createButton(buttonsComposite, "Удалить");

		addFromComponent.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				if (event.type == SWT.Selection) {
					if (((StructuredSelection) treeViewer.getSelection())
							.getFirstElement() instanceof StandartLibBuildPathGroup) {
						ExternalLibrarySelectionDialog dialog = new ExternalLibrarySelectionDialog(getShell(),
								"Выбор стандартных библиотек",
								"Укажите названия стандартных библиотек, используя разделитель ':'");

						if (dialog.open() == IDialogConstants.OK_ID) {
							if (treeViewer.getSelection() instanceof StructuredSelection strSel) {
								for (String lib : dialog.getResult()) {
									if (strSel.getFirstElement() instanceof StandartLibBuildPathGroup selGroup) {
										selGroup.addResource(lib);
									}
								}
								treeViewer.refresh();
							}
						}
					} else {
						ElementTreeSelectionDialog dialog = null;
						if (((StructuredSelection) treeViewer.getSelection())
								.getFirstElement() instanceof SourceBuildPathGroup) {
							dialog = new FileSelectionDialog(getShell());
							dialog.setTitle("Выбор файлов исходного кода");
							dialog.setMessage("Укажите файлы исходного кода");
							dialog.setInput(project);
						} else if (((StructuredSelection) treeViewer.getSelection())
								.getFirstElement() instanceof HeaderBuildPathGroup) {
							dialog = new FolderSelectionDialog(getShell());
							dialog.setTitle("Выбор заголовочных файлов");
							dialog.setMessage("Укажите папки с заголовочными файлами");
							dialog.setInput(
									new FolderSelectionDialog.WorkspaceProjectRecord(project.getWorkspace(), project));
						}

						if (dialog != null) {
							dialog.setComparator(new ResourceComparator(ResourceComparator.NAME));
							if (dialog.open() == IDialogConstants.OK_ID) {
								if (treeViewer.getSelection() instanceof StructuredSelection strSel) {
									for (Object o : dialog.getResult()) {
										if (o instanceof IResource resource) {
											if (strSel.getFirstElement() instanceof IBuildPathGroup selGroup) {
												IPath path = ((StructuredSelection) treeViewer.getSelection())
														.getFirstElement() instanceof HeaderBuildPathGroup
																? resource.getFullPath()
																		.makeRelativeTo(ResourcesPlugin.getWorkspace()
																				.getRoot().getFullPath())
																: resource.getFullPath()
																		.makeRelativeTo(project.getFullPath());

												if (path != null && !selGroup.getResources().contains(path)) {
													selGroup.addResource(path);
												}
											}
										}
									}
									treeViewer.refresh();
								}
							}
						}
					}
				}
			}
		});

		addLocal.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				if (event.type == SWT.Selection) {
					ElementTreeSelectionDialog dialog = new ComponentSelectionDialog(getShell(),
							new WorkbenchLabelProvider(), new ITreeContentProvider() {

								@Override
								public Object[] getElements(Object inputElement) {
									return getChildren(inputElement);
								}

								@Override
								public Object[] getChildren(Object parentElement) {
									if (parentElement instanceof IWorkspaceRoot root) {
										IProject[] projects = root.getProjects();

										List<Object> sortedProjects = new ArrayList<Object>();
										for (IProject proj : projects) {
											if (!proj.getName().startsWith(".") //$NON-NLS-1$
													&& !proj.getName().equals(project.getName())) {
												sortedProjects.add(proj);
											}
										}

										int i = 0;
										Object[] result = new Object[sortedProjects.size()];
										for (Object proj : sortedProjects) {
											result[i++] = proj;
										}
										return result;
									}
									return null;
								}

								@Override
								public Object getParent(Object element) {
									return ResourcesPlugin.getWorkspace().getRoot();
								}

								@Override
								public boolean hasChildren(Object element) {
									return element instanceof IWorkspaceRoot;
								}

							});
					dialog.setTitle(Messages.BuildFolderFieldEditor_ChooseFolderForBuild);
					dialog.setMessage(Messages.BuildFolderFieldEditor_ChooseFolder);
					dialog.setInput(ResourcesPlugin.getWorkspace().getRoot());
					dialog.setComparator(new ResourceComparator(ResourceComparator.NAME));
					if (dialog.open() == IDialogConstants.OK_ID) {
						if (treeViewer.getSelection() instanceof StructuredSelection strSel) {
							for (Object o : dialog.getResult()) {
								if (o instanceof IResource resource) {
									if (strSel.getFirstElement() instanceof IBuildPathGroup selGroup) {
										IPath path = resource.getFullPath().makeRelativeTo(project.getFullPath());
										if (!selGroup.getResources().contains(path)) {
											selGroup.addResource(path);
										}
									}
								}
							}
							treeViewer.refresh();
						}
					}
				}
			}
		});

		addExternal.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				if (event.type == SWT.Selection) {
					String filename = null;
					if (((StructuredSelection) treeViewer.getSelection())
							.getFirstElement() instanceof HeaderBuildPathGroup) {
						DirectoryDialog dialog = new DirectoryDialog(getShell(), SWT.SHEET);
						dialog.setText(""); //$NON-NLS-1$
						filename = dialog.open();
					} else {
						FileDialog dialog = new FileDialog(getShell(), SWT.SHEET);
						dialog.setText(""); //$NON-NLS-1$
						filename = dialog.open();
					}
					if (filename != null) {
						ISelection s = treeViewer.getSelection();
						if (s instanceof StructuredSelection strSel) {
							if (strSel.getFirstElement() instanceof IBuildPathGroup pathGroup) {
								pathGroup.addResource(Path.fromOSString(filename));
								treeViewer.refresh();
							}
						}
					}
				}
			}
		});

		delete.addListener(SWT.Selection, new Listener() {

			@Override
			public void handleEvent(Event event) {
				if (event.type == SWT.Selection) {
					ISelection s = treeViewer.getSelection();
					if (s instanceof ITreeSelection treeSelection) {
						if (treeSelection.getFirstElement() instanceof String item) {
							TreePath[] o = treeSelection.getPathsFor(treeSelection.getFirstElement());
							StandartLibBuildPathGroup pathGroup = (StandartLibBuildPathGroup) o[0].getParentPath()
									.getLastSegment();
//							pathGroup.deleteResource(item);
							treeViewer.refresh();
						} else if (treeSelection.getFirstElement() instanceof IPath item) {
							TreePath[] o = treeSelection.getPathsFor(treeSelection.getFirstElement());
							BuildPathGroup pathGroup = (BuildPathGroup) o[0].getParentPath().getLastSegment();
							pathGroup.deleteResource(item);
							treeViewer.refresh();
						}
					}
				}
			}
		});

		treeViewer.addSelectionChangedListener(new ISelectionChangedListener() {

			@Override
			public void selectionChanged(SelectionChangedEvent event) {
				if (event.getSelection() instanceof ITreeSelection treeSelection) {
					if (treeSelection.getFirstElement() instanceof IBuildPathGroup pathGroup) {
						delete.setEnabled(false);
						setButtonEnablement(pathGroup);
					} else if (treeSelection.getFirstElement() instanceof IResource item) {
						setDeleteEnabled();
						TreePath[] o = treeSelection.getPathsFor(treeSelection.getFirstElement());
						o[0].getParentPath().getLastSegment();
					} else if (treeSelection.getFirstElement() instanceof String item) {
						setDeleteEnabled();
						TreePath[] o = treeSelection.getPathsFor(treeSelection.getFirstElement());
						o[0].getParentPath().getLastSegment();
					} else if (treeSelection.getFirstElement() instanceof Path item) {
						setDeleteEnabled();
						TreePath[] o = treeSelection.getPathsFor(treeSelection.getFirstElement());
						o[0].getParentPath().getLastSegment();
					}
				}

			}

			void setButtonEnablement(IBuildPathGroup pathGroup) {
				switch (pathGroup.getName()) {
				case "Стандартные библиотеки":
					addLocal.setEnabled(false);
					addExternal.setEnabled(true);
					addFromComponent.setEnabled(false);
					break;
				case "Библиотеки":
				case "Драйверы":
					addLocal.setEnabled(true);
					addExternal.setEnabled(false);
					addFromComponent.setEnabled(false);
					break;
				case "Папки с заголовочными файлами":
					addLocal.setEnabled(false);
					addExternal.setEnabled(true);
					addFromComponent.setEnabled(true);
					break;
				default:
					addLocal.setEnabled(false);
					addExternal.setEnabled(false);
					addFromComponent.setEnabled(true);
				}
			}

			void setDeleteEnabled() {
				addLocal.setEnabled(false);
				addExternal.setEnabled(false);
				addFromComponent.setEnabled(false);
				delete.setEnabled(true);
			}
		});

		setInput(treeViewer);

		target.addSelectionListener(new org.eclipse.swt.events.SelectionListener() {

			@Override
			public void widgetSelected(SelectionEvent e) {
				selectedTargetName = target.getText();

				setInput(treeViewer);

				addLocal.setEnabled(false);
				addExternal.setEnabled(false);
				addFromComponent.setEnabled(false);
				delete.setEnabled(false);
			}

			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				widgetSelected(e);
			}

		});

		return common;
	}

	private TreeViewer createTree(Composite parent) {
		Tree tree = new Tree(parent, SWT.BORDER);
		tree.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true, 1, 1));

		TreeViewer treeViewer = new TreeViewer(tree);
		treeViewer.setLabelProvider(new DelegatingStyledCellLabelProvider(new ViewLabelProvider()));
		treeViewer.setContentProvider(new TreeContentProvider());

		return treeViewer;
	}

	private void setInput(TreeViewer treeViewer) {
		treeViewer.setInput(new IBuildPathGroup[] { this.sourceGroup.get(selectedTargetName),
				this.headerGroup.get(selectedTargetName), this.librariesGroup.get(selectedTargetName),
				this.driversGroup.get(selectedTargetName), this.externalGroup.get(selectedTargetName) });
	}

	private Composite createButtonsComposite(Composite parent) {
		Composite composite = new Composite(parent, SWT.None);
		composite.setLayout(new GridLayout(1, false));
		GridData buttonData = new GridData();
		buttonData.verticalAlignment = SWT.TOP;
		composite.setLayoutData(buttonData);

		return composite;
	}

	private Button createButton(Composite parent, String label) {
		Button button = new Button(parent, SWT.NONE);
		button.setText(label);
		GridData gridData = new GridData(SWT.FILL);
		gridData.horizontalAlignment = SWT.FILL;
		button.setLayoutData(gridData);

		return button;
	}

	@Override
	public boolean performOk() {
		for (String target : this.targetsNames) {
			Preferences ts = this.targetStore.node(target);

			ts.put(PreferenceConstants.SRC_FILES, sourceGroup.get(target).getResources().stream()
					.map(e -> e.toPortableString()).collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));

			List<String> incFiles = new ArrayList<>(headerGroup.get(target).getResources().stream().map(e -> {
				return e.toPortableString();
			}).toList());
			ts.put(PreferenceConstants.LOCAL_INC_FILES,
					incFiles.stream().collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));

			ts.put(PreferenceConstants.EXTERNAL_LIBRARIES,
					((StandartLibBuildPathGroup) externalGroup.get(target)).getStringResources().stream().map(e -> e)
							.collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));

			ts.put(PreferenceConstants.LIBRARIES,
					librariesGroup.get(target).getResources().stream().map(e -> e.lastSegment().toString())
							.collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));

			ts.put(PreferenceConstants.DRIVERS, driversGroup.get(target).getResources().stream()
					.map(e -> e.toPortableString()).collect(Collectors.joining(PreferenceConstants.FIELD_SEPARATOR)));
		}

		return super.performOk();
	}

}
