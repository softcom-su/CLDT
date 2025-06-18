package su.softcom.cldt.debug.internal.core;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;

import org.eclipse.core.resources.IStorage;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.PlatformObject;

public class FileStorage extends PlatformObject implements IStorage {
	
	IPath path;
	InputStream in = null;

	@Override
	public InputStream getContents() throws CoreException {
		if (in == null) {
			try {
				return new FileInputStream(path.toFile());
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			}
		}
		return in;
	}

	@Override
	public IPath getFullPath() {
		return this.path;
	}

	@Override
	public String getName() {
		return this.path.lastSegment();
	}

	@Override
	public boolean isReadOnly() {
		return true;
	}

	public FileStorage(IPath path) {
		this.path = path;
	}

	public FileStorage(InputStream in, IPath path) {
		this.path = path;
		this.in = in;
	}

	@Override
	public String toString() {
		return path.toOSString();
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj instanceof IStorage) {
			IPath p = getFullPath();
			IPath objPath = ((IStorage) obj).getFullPath();
			if (p != null && objPath != null)
				return p.equals(objPath);
		}
		return super.equals(obj);
	}
}
