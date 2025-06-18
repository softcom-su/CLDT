package su.softcom.cldt.core.operations;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.ui.console.MessageConsole;
import org.eclipse.ui.console.MessageConsoleStream;

import su.softcom.cldt.core.ProcessOutputMonitor;
import su.softcom.cldt.internal.core.builders.CMakeConfigureBuilder;

/**
 * Operation for run <code>clang-tidy</code> tool. Operation will generate
 * problem markers for every found error, warning and note.
 * 
 * @see org.eclipse.core.resources.IMarker
 * 
 */
public class ClangTidyOperation implements ICoreRunnable {

	private List<String> launchArguments;
	private static final String EXECUTABLE_NAME = "clang-tidy"; //$NON-NLS-1$
	private final String COMPILE_COMMANDS = "compile_commands.json"; //$NON-NLS-1$
	private final IProject project;
	private final ProcessOutputMonitor outputMonitor;
	private final MessageConsole outputConsole;
	private final String buildFolder;
	private final String ID = "su.softcom.cldt.core.anlysis.clang-tidy"; //$NON-NLS-1$

	/**
	 * Creates an instance of the clang-tidy operation to analyze the source files
	 * located in the project.
	 * 
	 * @param project
	 * @param files
	 * @param outputConsole
	 * @param args
	 * @param buildFolder
	 */
	public ClangTidyOperation(final IProject project, final Set<IFile> files, final MessageConsole outputConsole,
			List<String> args, String buildFolder) {
		this.project = project;
		this.outputMonitor = new ProcessOutputMonitor();
		this.outputConsole = outputConsole;
		this.buildFolder = buildFolder;

		configure(files, args);
	}

	private void configure(Set<IFile> targetFiles, List<String> arguments) {
		this.launchArguments = new ArrayList<String>();

		String pathToClangTidyFolder = "/usr"; // TODO что делать если не в usr/bin? //$NON-NLS-1$
		this.launchArguments.add(new Path("%s/bin/%s%s".formatted(pathToClangTidyFolder, EXECUTABLE_NAME, //$NON-NLS-1$
				Platform.getOS().contains("win") ? ".exe" : "")).toString()); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$

		for (IFile targetFile : targetFiles) {
			this.launchArguments.add(targetFile.getLocation().toPortableString());
		}

		arguments.add("-p"); //$NON-NLS-1$
		arguments.add(project.getFolder(buildFolder).getLocation().toOSString());

		this.launchArguments.addAll(arguments);
	}

	private boolean compileCommandsExists() {
		String buildFolderPath = project.getFolder(buildFolder).getLocation().toOSString();
		File compileCommandsFile = new File(buildFolderPath, COMPILE_COMMANDS);

		return compileCommandsFile.exists();
	}

	@Override
	public void run(IProgressMonitor monitor) {

		if (!compileCommandsExists()) {
			try {
				project.build(IncrementalProjectBuilder.INCREMENTAL_BUILD, CMakeConfigureBuilder.ID, null, null);
			} catch (CoreException e) {
				e.printStackTrace();
			}
		}

		runClangTidy();
	}

	private void runClangTidy() {
		ProcessBuilder pb = new ProcessBuilder(launchArguments);
		ILog log = Platform.getLog(this.getClass());

		try {
			writeToConsole("Project: " + project.getName() + "\n"); //$NON-NLS-1$ //$NON-NLS-2$
			Process proc = pb.start();
			log.log(new Status(IStatus.INFO, ID, "Clang-tidy started")); //$NON-NLS-1$
			InputStream is = proc.getInputStream();
			InputStream es = proc.getErrorStream();

			BufferedReader inputStreamReader = new BufferedReader(new InputStreamReader(is));
			BufferedReader errorStreamReader = new BufferedReader(new InputStreamReader(es));

			do {
				readFromStream(inputStreamReader);
				readFromStream(errorStreamReader);
			} while (proc.isAlive());

			readFromStream(inputStreamReader);
			readFromStream(errorStreamReader);
			int exitCode = proc.exitValue();
			writeToConsole("\n\n"); //$NON-NLS-1$

			is.close();
			es.close();
			log.log(new Status(IStatus.INFO, ID, "Clang-tidy finished with exit code: " + exitCode)); //$NON-NLS-1$
		} catch (IOException e) {
			log.log(new Status(IStatus.ERROR, ID, "Failed to start Clang-tidy", e)); //$NON-NLS-1$
		}
	}

	private void readFromStream(BufferedReader reader) throws IOException {
		StringBuilder sb = new StringBuilder();
		while (reader.ready()) {
			char readChar = (char) reader.read();
			sb.append(readChar);
			if (readChar == '\n') {
				outputMonitor.addContent(sb.toString());
				writeToConsole(sb.toString());
				sb.setLength(0);
			}
		}
	}

	private void writeToConsole(String message) {
		try {
			if (outputConsole != null) {
				MessageConsoleStream out = outputConsole.newMessageStream();
				out.print(message);
				out.close();
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}