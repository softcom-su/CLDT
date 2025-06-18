package su.softcom.cldt.core.runners;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;

/**
 * Generates problem after build.
 */
public final class ProblemsEmitter {

	private String markerType;
	private IResource defaultResource;

	/**
	 * Default constructor
	 */
	public ProblemsEmitter() {
		this.defaultResource = ResourcesPlugin.getWorkspace().getRoot();
		this.markerType = IMarker.PROBLEM;
	}

	/**
	 * @param defaultResource resource for which marker will be emitted
	 */
	public ProblemsEmitter(final IResource defaultResource) {
		this.defaultResource = defaultResource;
		this.markerType = IMarker.PROBLEM;
	}

	/**
	 * @param type marker type
	 */
	public ProblemsEmitter(final String type) {
		this.defaultResource = ResourcesPlugin.getWorkspace().getRoot();
		this.markerType = type;
	}

	/**
	 * @param defaultResource resource for which marker will be emitted
	 * @param type            marker type
	 */
	public ProblemsEmitter(final IResource defaultResource, final String type) {
		this.defaultResource = defaultResource;
		this.markerType = type;
	}

	/**
	 * @param resource default resource
	 */
	public void setDefault(final IResource resource) {
		defaultResource = resource;
	}

	/**
	 * @return default resource
	 */
	public IResource getDefault() {
		return defaultResource;
	}

	/**
	 * @param type marker type
	 */
	public void setType(final String type) {
		markerType = type;
	}

	/**
	 * @return marker type
	 */
	public String getType() {
		return markerType;
	}

	/**
	 * Creates error marker with given message
	 * 
	 * @param message error message
	 */
	public void emitError(final String message) {
		emitMarker(defaultResource, message, IMarker.SEVERITY_ERROR);
	}

	/**
	 * Creates error marker with given message
	 * 
	 * @param resource resource for which the marker shall be created
	 * @param message  error message
	 */
	public void emitError(final IResource resource, final String message) {
		emitMarker(resource, message, IMarker.SEVERITY_ERROR);
	}

	/**
	 * Creates error marker with given message
	 * 
	 * @param path    path to resource for which the marker shall be created
	 * @param message error message
	 */
	public void emitError(final IPath path, final String message) {
		emitError(findMember(path), message);
	}

	/**
	 * Creates error marker with given message
	 * 
	 * @param path    path to resource for which the marker shall be created as a
	 *                String
	 * @param message error message
	 */
	public void emitError(final String path, final String message) {
		emitError(findMember(path), message);
	}

	/**
	 * Creates error marker with given message
	 * 
	 * @param file    file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitError(final IResource file, final int line, final String message) {
		emitMarker(file, line, message, IMarker.SEVERITY_ERROR);
	}

	/**
	 * Creates error marker with given message
	 * 
	 * @param file    path to file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitError(final IPath file, final int line, final String message) {
		emitError(findMember(file), line, message);
	}

	/**
	 * Creates error marker with given message
	 * 
	 * @param file    of a file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitError(final String file, final int line, final String message) {
		emitError(findMember(file), line, message);
	}

	/**
	 * Creates warning marker with given message
	 * 
	 * @param message error message
	 */
	public void emitWarning(final String message) {
		emitMarker(defaultResource, message, IMarker.SEVERITY_WARNING);
	}

	/**
	 * Creates warning marker with given message
	 * 
	 * @param resource resource for which the marker shall be created
	 * @param message  error message
	 */
	public void emitWarning(final IResource resource, final String message) {
		emitMarker(resource, message, IMarker.SEVERITY_WARNING);
	}

	/**
	 * Creates warning marker with given message
	 * 
	 * @param path    path to resource for which the marker shall be created
	 * @param message error message
	 */
	public void emitWarning(final IPath path, final String message) {
		emitWarning(findMember(path), message);
	}

	/**
	 * Creates warning marker with given message
	 * 
	 * @param path    path to resource for which the marker shall be created as a
	 *                String
	 * @param message error message
	 */
	public void emitWarning(final String path, final String message) {
		emitWarning(findMember(path), message);
	}

	/**
	 * Creates warning marker with given message
	 * 
	 * @param file    file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitWarning(final IResource file, final int line, final String message) {
		emitMarker(file, line, message, IMarker.SEVERITY_WARNING);
	}

	/**
	 * Creates warning marker with given message
	 * 
	 * @param file    path to file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitWarning(final IPath file, final int line, final String message) {
		emitWarning(findMember(file), line, message);
	}

	/**
	 * Creates warning marker with given message
	 * 
	 * @param file    of a file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitWarning(final String file, final int line, final String message) {
		emitWarning(findMember(file), line, message);
	}

	/**
	 * Creates info marker with given message
	 * 
	 * @param message error message
	 */
	public void emitInfo(final String message) {
		emitMarker(defaultResource, message, IMarker.SEVERITY_INFO);
	}

	/**
	 * Creates info marker with given message
	 * 
	 * @param resource resource for which the marker shall be created
	 * @param message  error message
	 */
	public void emitInfo(final IResource resource, final String message) {
		emitMarker(resource, message, IMarker.SEVERITY_INFO);
	}

	/**
	 * Creates info marker with given message
	 * 
	 * @param path    path to resource for which the marker shall be created
	 * @param message error message
	 */
	public void emitInfo(final IPath path, final String message) {
		emitInfo(findMember(path), message);
	}

	/**
	 * Creates info marker with given message
	 * 
	 * @param path    path to resource for which the marker shall be created as a
	 *                String
	 * @param message error message
	 */
	public void emitInfo(final String path, final String message) {
		emitInfo(findMember(path), message);
	}

	/**
	 * Creates info marker with given message
	 * 
	 * @param file    file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitInfo(final IResource file, final int line, final String message) {
		emitMarker(file, line, message, IMarker.SEVERITY_INFO);
	}

	/**
	 * Creates info marker with given message
	 * 
	 * @param file    path to file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitInfo(final IPath file, final int line, final String message) {
		emitInfo(findMember(file), line, message);
	}

	/**
	 * Creates info marker with given message
	 * 
	 * @param file    of a file for which the marker shall be created
	 * @param line    line of given file for marker to be created at
	 * @param message error message
	 */
	public void emitInfo(final String file, final int line, final String message) {
		emitInfo(findMember(file), line, message);
	}

	/**
	 * Deletes all markers of default resource
	 */
	public void deleteMarkers() {
		try {
			defaultResource.deleteMarkers(null, true, IResource.DEPTH_INFINITE);
		} catch (CoreException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Deletes all markers from workspace
	 */
	public static void deleteAllMarkers() {
		try {
			ResourcesPlugin.getWorkspace().getRoot().deleteMarkers(null, true, IResource.DEPTH_INFINITE);
		} catch (CoreException e) {
			e.printStackTrace();
		}
	}

	private IResource findMember(final String path) {
		return findMember(new Path(path));
	}

	private IResource findMember(final IPath path) {
		return findMember(defaultResource, path);
	}

	private IResource findMember(final IResource resource, final IPath path) {
		if (resource.getLocation().equals(path)) {
			return resource;
		}

		if (resource instanceof IContainer) {
			try {
				for (final IResource member : ((IContainer) resource).members()) {
					IResource result = findMember(member, path);
					if (result != null) {
						return result;
					}
				}
			} catch (CoreException e) {
				e.printStackTrace();
			}
		}

		return null;
	}

	private void emitMarker(final IResource resource, final String message, final int type) {
		if (resource != null) {
			try {
				IMarker marker = resource.createMarker(markerType);
				marker.setAttribute(IMarker.MESSAGE, message);
				marker.setAttribute(IMarker.SEVERITY, type);
			} catch (CoreException e) {
				if (!resource.equals(defaultResource)) {
					emitMarker(defaultResource, message, type);
				} else if (!ResourcesPlugin.getWorkspace().getRoot().equals(defaultResource)) {
					emitMarker(ResourcesPlugin.getWorkspace().getRoot(), message, type);
				} else {
					e.printStackTrace();
				}
			}
		} else {
			emitMarker(defaultResource, message, type);
		}
	}

	private void emitMarker(final IResource file, final int line, final String message, final int type) {
		if (file != null && file instanceof IFile && line > 0) {
			try {
				IMarker marker = file.createMarker(markerType);
				marker.setAttribute(IMarker.LINE_NUMBER, line);
				marker.setAttribute(IMarker.MESSAGE, message);
				marker.setAttribute(IMarker.SEVERITY, type);
			} catch (CoreException e) {
				emitMarker(defaultResource, message, type);
			}
		} else {
			emitMarker(file, message, type);
		}
	}

}
