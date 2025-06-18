package su.softcom.cldt.core.operations;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;

/**
 * Runs clang-refactor with the provided parameters
 */
public class ClangRefactorOperation extends LLVMOperation {

	private static final String EXECUTABLE_NAME = "clang-refactor"; //$NON-NLS-1$
	IProject project;
	IFile file;
	IMarker marker;
	final ClangRefactorOutputParser parser;

	/**
	 * Create new operation instance for refactor passed files located in the passed
	 * project. Arguments will be passed "as is" into process stdin without
	 * additional handling.
	 * 
	 * @param project
	 * @param files
	 * @param args
	 * @param marker
	 */
	public ClangRefactorOperation(final IProject project, final Set<IFile> files, List<String> args, IMarker marker) {
		super(EXECUTABLE_NAME);
		this.project = project;
		this.marker = marker;
		this.file = files.iterator().next();
		List<String> localArgs = args == null ? new ArrayList<>() : new ArrayList<>(args);
		configure(files, localArgs);
		parser = new ClangRefactorOutputParser();
		outputMonitor.addListener(parser);

	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		super.run(monitor);

		outputMonitor.flushOutStream();
		outputMonitor.removeListener(parser);
		String clangRefactorOutput = parser.sb.toString();

		if (exitCode != 0) {
			String msg = "Refactoring failed with exit code %d.%nOutput:%n%s".formatted(exitCode, clangRefactorOutput);
			Platform.getLog(this.getClass()).log(new Status(IStatus.ERROR, "su.softcom.cldt.core.operations", msg)); //$NON-NLS-1$
			throw new CoreException(new Status(IStatus.ERROR, "su.softcom.cldt.core.operations", //$NON-NLS-1$
					"Refactoring operation failed. See Error Log for details."));
		}

		boolean inplaceEdit = arguments.contains("-i"); //$NON-NLS-1$
		if (!inplaceEdit) {

			InputStream refactoredContent = new ByteArrayInputStream(clangRefactorOutput.getBytes());
			file.setContents(refactoredContent, IResource.FORCE, monitor);
		}
		if (marker != null && marker.exists()) {
			marker.delete();
		}

		marker.getResource().touch(monitor);
	}

	public Object getStatus() {
		return null;
	}

}
