package su.softcom.cldt.internal.core;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IResourceChangeEvent;
import org.eclipse.core.resources.IResourceChangeListener;
import org.eclipse.core.resources.IResourceDelta;
import org.eclipse.core.resources.IResourceDeltaVisitor;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.source.SourceFile;

/**
 * 
 */
public class CppResourceChangeListener implements IResourceChangeListener {

	private final CppFileManager fileManager = CMakeCorePlugin.getDefault().getFileManager();

	@Override
	public void resourceChanged(IResourceChangeEvent event) {
		if (event.getType() == IResourceChangeEvent.PRE_DELETE) {
			IProject prj = (IProject) event.getResource();
			fileManager.handleDeletedProject(prj);
		} else if (event.getType() == IResourceChangeEvent.POST_CHANGE) {
			try {
				CppResourceVisitor visitor = new CppResourceVisitor();
				event.getDelta().accept(visitor);
			} catch (CoreException e) {
				Platform.getLog(getClass()).error("Error during post-change resource event"); //$NON-NLS-1$
			}
		}
	}

	private class CppResourceVisitor implements IResourceDeltaVisitor {

		@Override
		public boolean visit(IResourceDelta delta) throws CoreException {
			IResource res = delta.getResource();
			if (delta.getKind() == IResourceDelta.ADDED) {
				return false;
			}
			if (delta.getKind() == IResourceDelta.REMOVED) {
				return handleResourceDeletion(res);
			}
			if ((delta.getKind() == IResourceDelta.CHANGED) && (res instanceof IFile file)) {
				IProject project = res.getProject();
				if (project.exists()) {
					ICMakeProject cmakeProject = CMakeCorePlugin.getDefault().getProject(project);
					if (cmakeProject != null) {
						SourceFile src = cmakeProject.getSourceFile(file);
						if (src != null) {
							src.setDirty(true);
						}

					}
				}
				return false;
			}
			return true;
		}

		private boolean handleResourceDeletion(IResource resource) {
			IProject project = resource.getProject();
			if (!project.exists()) {
				return false;
			}
			ICMakeProject cmakeProject = CMakeCorePlugin.getDefault().getProject(project);
			if (cmakeProject == null) {
				return false;
			}
			if (cmakeProject.getSources().isEmpty()) {
				return false;
			}
			if (resource instanceof IFile file) {
				fileManager.handleDeletedFile(file, cmakeProject);
				return false;
			} else if (resource instanceof IFolder) {
				return true;
			} else if (resource instanceof IProject) {
				return true;
			}
			return true;
		}
	}
}