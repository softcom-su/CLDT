package su.softcom.cldt.core.operations;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.ProcessOutputMonitor;

public abstract class LLVMOperation implements ICoreRunnable {

	protected List<String> arguments;
	IPath filePath;

	private final IPath pathToLLVMFolder;

	protected final IEclipsePreferences instancePreferences;
	protected ProcessOutputMonitor outputMonitor;
	private String fileName;
	protected int exitCode;

	protected LLVMOperation(String fileName) {
		outputMonitor = (ProcessOutputMonitor) CMakeCorePlugin.getBuildOutputMonitor();
		instancePreferences = InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE);
		pathToLLVMFolder = new Path(instancePreferences.get(PreferenceConstants.CLANG_PATH, "/usr")); //$NON-NLS-1$
		this.fileName = fileName;
	}

	protected void configure(Set<IFile> targetFiles, List<String> args) {
		this.arguments = new ArrayList<>();

		this.arguments.add(new Path(
				"%s/bin/%s%s".formatted(pathToLLVMFolder, fileName, Platform.getOS().contains("win") ? ".exe" : "")) //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
				.toString());

		this.arguments.addAll(args);
		if (!targetFiles.isEmpty()) {
			for (IFile targetFile : targetFiles) {
				this.arguments.add(targetFile.getLocation().toPortableString());
			}
		}

	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		ProcessBuilder pb = new ProcessBuilder(arguments);
		try {
			Process proc = pb.start();
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
			exitCode = proc.exitValue();
			is.close();
			es.close();
		} catch (IOException e) {
			Platform.getLog(this.getClass()).error(e.getLocalizedMessage());
		}
	}

	private void readFromStream(BufferedReader reader) throws IOException {
		StringBuilder sb = new StringBuilder();
		while (reader.ready()) {
			char readChar = (char) reader.read();
			sb.append(readChar);
			if (readChar == '\n') {
				outputMonitor.addContent(sb.toString());
				sb = new StringBuilder();
			}
		}

		outputMonitor.addContent(sb.toString());
	}
}
