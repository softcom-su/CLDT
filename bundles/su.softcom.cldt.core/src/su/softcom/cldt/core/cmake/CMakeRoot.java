package su.softcom.cldt.core.cmake;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;

public class CMakeRoot extends CMakeNode {

	protected boolean finished = false;

	IFile file; // FIXME: need to either delete this field or use it in writeToFile() method
				// (delete its file parameter then)

	/**
	 * Constructs a CMake tree root. By conventions, it should be the only node with
	 * {@code null} as its root.
	 * 
	 * @throws IOException   ~
	 * @throws CoreException ~
	 */
	public CMakeRoot() throws IOException, CoreException {
		super(null);
	}

	/**
	 * Rewrites a given file with the tree contents.
	 * 
	 * @param file
	 * @param monitor
	 * @throws CoreException
	 */
	public void writeToFile(IFile file, IProgressMonitor monitor) throws CoreException {
		InputStream s = new ByteArrayInputStream(toText().toString().getBytes());
		if (!file.exists()) {
			file.create(s, false, monitor.slice(1));
		} else {
			file.refreshLocal(IResource.DEPTH_ONE, monitor.slice(1));
			file.setContents(s, false, false, monitor.slice(1));
		}
		// file.refreshLocal(IResource.DEPTH_ONE, monitor.slice(1));
		try {
			s.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void accept(ICMakeVisitor visitor) {
		visitor.visitRoot(this);
	}

}
