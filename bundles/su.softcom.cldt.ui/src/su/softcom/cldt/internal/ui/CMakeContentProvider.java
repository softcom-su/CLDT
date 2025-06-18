package su.softcom.cldt.internal.ui;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IResourceDelta;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.jface.viewers.StructuredViewer;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.widgets.Control;
import org.eclipse.ui.IMemento;
import org.eclipse.ui.model.WorkbenchContentProvider;
import org.eclipse.ui.navigator.ICommonContentExtensionSite;
import org.eclipse.ui.navigator.IPipelinedTreeContentProvider;
import org.eclipse.ui.navigator.PipelinedShapeModification;
import org.eclipse.ui.navigator.PipelinedViewerUpdate;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.core.source.ISourceElement;
import su.softcom.cldt.core.source.ISourceFile;
import su.softcom.cldt.core.source.SourceElement;
import su.softcom.cldt.core.source.SourceFile;

/**
 * Представляет content provider для CMake файлов. Выводит в Project Explorer
 * вид файлов {@link SourceFile}, {@link CMakeTargetsFolder} и {@link Target}
 * 
 * @see https://help.eclipse.org/latest/index.jsp?topic=%2Forg.eclipse.platform.doc.isv%2Freference%2Fextension-points%2Forg_eclipse_ui_navigator_navigatorContent.html
 */
public class CMakeContentProvider extends WorkbenchContentProvider implements IPipelinedTreeContentProvider {

	protected static final Object[] NO_CHILDREN = new Object[0];
	public static final String CMAKE_CP_ID = "su.softcom.cldt.ui.navigatorContent.cmake"; //$NON-NLS-1$
	private Viewer fViewer;

	@Override
	public void init(ICommonContentExtensionSite commonContentExtensionSite) {
		IMemento memento = commonContentExtensionSite.getMemento();
		restoreState(memento);
	}

	@Override
	public void inputChanged(Viewer viewer, Object oldInput, Object newInput) {
		this.fViewer = viewer;
		super.inputChanged(viewer, oldInput, newInput);
	}

	@Override
	public Object[] getElements(Object inputElement) {
		List<Object> result = new ArrayList<>();
		if (inputElement instanceof IWorkspaceRoot root) {
			return root.getProjects();
		}
		if (inputElement instanceof IProject project) {
			ICMakeProject cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
			result.add(cmakeProject);
			return result.toArray(new Object[0]);
		}
		return NO_CHILDREN;
	}

	@Override
	public Object[] getChildren(Object parentElement) {
		List<Object> result = new ArrayList<>();
		if (parentElement instanceof IProject project) {
			ICMakeProject cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
			if (cmakeProject == null) {
				return new Object[0];
			}
			List<ISourceFile> sources = cmakeProject.getSources();
			for (ISourceFile src : sources) {
				result.add(src);
			}
			if (!cmakeProject.getTargets().isEmpty()) {
				CMakeTargetsFolder tgtFolder = new CMakeTargetsFolder(cmakeProject.getTargets(), project);
				result.add(tgtFolder);
			}
			return result.toArray(new Object[0]);
		}
		if (parentElement instanceof IFolder folder) {
			ICMakeProject cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(folder.getProject());
			List<ISourceFile> sources = cmakeProject.getSources();
			for (ISourceFile src : sources) {
				result.add(src);
			}
			return result.toArray(new Object[0]);
		}
		if (parentElement instanceof CMakeTargetsFolder fldr) {
			return fldr.getTargets().toArray();
		}
		if (parentElement instanceof Target target) {
			return target.getSources().toArray();
		}
		if (parentElement instanceof ISourceFile src) {
			return src.getElements().toArray(new Object[0]);
		}
		if (parentElement instanceof SourceElement srcElem) {
			return srcElem.getElements().toArray(new Object[0]);
		}
		return new Object[0];
	}

	@Override
	public Object getParent(Object element) {
		if (element instanceof CMakeTargetsFolder fldr) {
			return fldr.getProject();
		}
		return null;
	}

	@Override
	public boolean hasChildren(Object element) {
		if (element instanceof IProject project) {
			return project.isAccessible();
		}
		if (element instanceof CMakeTargetsFolder fldr) {
			return !fldr.getTargets().isEmpty();
		}
		if (element instanceof Target tgt) {
			return !tgt.getSources().isEmpty();
		}
		if (element instanceof ISourceFile) {
			// FIXME
			return true;
		}
		if (element instanceof ISourceElement srcElem) {
			return !srcElem.getElements().isEmpty();
		}
		return false;
	}

	@Override
	public void restoreState(IMemento aMemento) {
		// empty
	}

	@Override
	public void saveState(IMemento aMemento) {
		// empty
	}

	@Override
	public void getPipelinedChildren(Object parent, Set currentChildren) {
		customize(getChildren(parent), currentChildren, parent);
	}

	@Override
	public void getPipelinedElements(Object input, Set currentElements) {
		customize(getElements(input), currentElements, input);
	}

	@Override
	public Object getPipelinedParent(Object object, Object suggestedParent) {
		return getParent(object);
	}

	@Override
	public PipelinedShapeModification interceptAdd(PipelinedShapeModification addModification) {
		Object parent = addModification.getParent();
		if (parent instanceof IProject project) {
			addModification.setParent(project);
		}
		if (parent instanceof IWorkspaceRoot root) {
			addModification.setParent(root);
		}
		return addModification;
	}

	@Override
	public PipelinedShapeModification interceptRemove(PipelinedShapeModification removeModification) {
		return removeModification;
	}

	@Override
	public boolean interceptRefresh(PipelinedViewerUpdate aRefreshSynchronization) {
		return false;
	}

	@Override
	public boolean interceptUpdate(PipelinedViewerUpdate anUpdateSynchronization) {
		return false;
	}

	/**
	 * Process the resource delta.
	 *
	 * @param delta delta to process; not <code>null</code>
	 */
	@Override
	protected void processDelta(IResourceDelta delta) {
		int status = delta.getKind();
		IResource resource = delta.getResource();
		if (resource == null) {
			return;
		}
		Control ctrl = fViewer.getControl();
		if (status == IResourceDelta.CHANGED) {
			// Are we in the UIThread? If so spin it until we are done
			if (ctrl.getDisplay().getThread() == Thread.currentThread()) {
				((StructuredViewer) fViewer).refresh(resource);
			} else {
				ctrl.getDisplay().asyncExec(() -> {
					// Abort if this happens after disposes
					Control ctrl1 = fViewer.getControl();
					if (ctrl1 == null || ctrl1.isDisposed()) {
						return;
					}
					((StructuredViewer) fViewer).refresh(resource);
				});
			}
		}
		super.processDelta(delta);
	}

	/**
	 * Adapted from the Common Navigator Content Provider
	 *
	 * @param cppElements      the cpp elements
	 * @param proposedChildren the proposed children
	 */
	private void customize(Object[] cppElements, Set<Object> proposedChildren, Object parent) {
		for (Object element : cppElements) {
			if (element instanceof SourceFile src) {
				IResource resource = src.getFile();
				if (resource != null && resource.getParent().equals(parent)) {
					proposedChildren.remove(resource);
					proposedChildren.add(element);
				}
			} else if (element != null) {
				proposedChildren.add(element);
			}
		}

	}

}
