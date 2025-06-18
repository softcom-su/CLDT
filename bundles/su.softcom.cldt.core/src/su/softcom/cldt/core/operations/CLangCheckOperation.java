package su.softcom.cldt.core.operations;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.ui.console.MessageConsole;

/**
 * Operation for run <code>clang-check</code> tool. Operation will generate
 * problem markers for every found error, warning and info.
 * 
 * @see org.eclipse.core.resources.IMarker
 * 
 */
public class CLangCheckOperation extends LLVMOperation {

	private static final String EXECUTABLE_NAME = "clang-check"; //$NON-NLS-1$
	IProject project;

	final ClangOutputParser parser;

	/**
	 * Create new operation instance for analysis passed files located in the passed
	 * project. Arguments will be passed "as is" into process stdin without
	 * additional handling.
	 * 
	 * @param project
	 * @param files
	 * @param outputConsole
	 * @param args
	 * @param buildFolder
	 */
	public CLangCheckOperation(final IProject project, final Set<IFile> files, final MessageConsole outputConsole,
			List<String> args, String buildFolder) {
		super(EXECUTABLE_NAME);
		this.project = project;
		List<String> arguments = args == null ? new ArrayList<>() : new ArrayList<>(args);
		arguments.add("-p"); //$NON-NLS-1$
		arguments.add(project.getFolder(buildFolder).getLocation().toOSString());
		configure(files, arguments);
		parser = new ClangOutputParser("su.softcom.cldt.marker.analysisMarker"); //$NON-NLS-1$
		outputMonitor.addListener(parser);
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		super.run(monitor);
		outputMonitor.removeListener(parser);
	}
}
