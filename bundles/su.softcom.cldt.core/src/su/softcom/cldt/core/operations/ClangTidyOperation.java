package su.softcom.cldt.core.operations;

import java.io.IOException;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;

/**
 * Operation for running <code>clang-tidy</code> tool.
 * 
 * @see org.eclipse.core.resources.IMarker
 * 
 */
public class ClangTidyOperation extends LLVMOperation {

	private static final String EXECUTABLE_NAME = "clang-tidy"; //$NON-NLS-1$

	private final String buildFolder;
	private final ClangTidyOutputParser parser;

	/**
	 * Creates an instance of the clang-tidy operation to analyze the source files
	 * located in the project.
	 * 
	 * @param files       source files to be analyzed.
	 * @param args        arguments for clang-tidy.
	 * @param buildFolder build folder.
	 */
	public ClangTidyOperation(final Set<IFile> files, List<String> args, String buildFolder) {
		super(EXECUTABLE_NAME);
		this.buildFolder = buildFolder;

		configure(files, args);
		parser = new ClangTidyOutputParser(!args.contains("--fix")); //$NON-NLS-1$
		outputMonitor.addListener(parser);
	}

	@Override
	protected void configure(Set<IFile> targetFiles, List<String> parameters) {
		parameters.add("-p"); //$NON-NLS-1$
		parameters.add(buildFolder);
		super.configure(targetFiles, parameters);
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		super.run(monitor);
		outputMonitor.flushOutStream();

		if (exitCode != 0) {
			String clangTidyOutput = parser.getOutput();

			outputMonitor.removeListener(parser);

			String msg = String.format("Clang-tidy failed with exit code %d.%nOutput:%n%s", exitCode, clangTidyOutput); //$NON-NLS-1$
			Platform.getLog(this.getClass()).log(new Status(IStatus.ERROR, "su.softcom.cldt.core.operations", msg)); //$NON-NLS-1$

			throw new CoreException(new Status(IStatus.ERROR, "su.softcom.cldt.core.operations", //$NON-NLS-1$
					"Clang-tidy operation failed with exit code: " + exitCode + "\nSee Error Log for details.")); //$NON-NLS-1$ //$NON-NLS-2$
		}

		try {
			parser.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}

		outputMonitor.removeListener(parser);
	}
}
