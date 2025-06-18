package su.softcom.cldt.core.cmake;

import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.Path;

import com.fasterxml.jackson.databind.annotation.JsonDeserialize;

import su.softcom.cldt.internal.core.cmake.IncludeDeserializer;

@JsonDeserialize(using = IncludeDeserializer.class)
public class IncludeObject implements IAdaptable {

	final IFolder folder;
	final boolean isSystem;

	public IncludeObject(String path, boolean isSystem) {
		folder = ResourcesPlugin.getWorkspace().getRoot().getFolder(new Path(path));
		this.isSystem = isSystem;
	}

	@Override
	public <T> T getAdapter(Class<T> adapter) {
		if (adapter == IFolder.class) {
			return (T) folder;
		}
		return null;
	}
}