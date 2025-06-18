package su.softcom.cldt.core.operations;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.osgi.framework.FrameworkUtil;

/**
 * Generates emul.dv.h and manages availability of emul.dv.3S
 */
public class ExtabLibOperation implements ICoreRunnable {
	/**
	 * The name of generated header file
	 */
	public static String H_FILE = "emul.dv.h"; //$NON-NLS-1$
	/**
	 * The name of copied .S file
	 */
	public static String S_FILE = "emul.dv3.S"; //$NON-NLS-1$
	/**
	 * The name of folder where controlled files reside
	 */
	public static String FOLDER_NAME = ".uemu"; //$NON-NLS-1$

	private static String PRE_FORMATED_H_FILE_LINE = "set_api \t%s\n"; //$NON-NLS-1$
	private static String ORIGINAL_S_FILE_LOCATION = "resources" + IPath.SEPARATOR + "toolchains" + IPath.SEPARATOR //$NON-NLS-1$ //$NON-NLS-2$
			+ "emul.dv3.S";

	private IProject project;
	private List<String> names;

	/**
	 * @param project - project for which operation needs to be performed
	 * @param names   - list of words to add to header file
	 */
	public ExtabLibOperation(IProject project, List<String> names) {
		this.project = project;
		this.names = names.stream().filter(e -> {
			return e.length() > 0;
		}).toList();
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		IFolder folder = project.getFolder(FOLDER_NAME);
		if (!folder.exists()) {
			folder.create(false, true, monitor);
		}
		IFile sFile = folder.getFile(S_FILE);
		if (!sFile.exists()) {
			createSFile(sFile, monitor);
		}
		IFile hFile = folder.getFile(H_FILE);
		manageHFile(hFile, monitor);
	}

	private ByteArrayInputStream generateHFileInputStream() {
		return new ByteArrayInputStream(names.stream().map((name) -> {
			return PRE_FORMATED_H_FILE_LINE.formatted(name);
		}).collect(Collectors.joining()).getBytes());
	}

	private void manageHFile(IFile file, IProgressMonitor monitor) throws CoreException {
		if (file.exists()) {
			file.delete(false, monitor);
		}
		file.create(generateHFileInputStream(), false, monitor);
		file.refreshLocal(0, monitor);
	}

	private void createSFile(IFile file, IProgressMonitor monitor) throws CoreException {
		String path;
		try {
			path = FileLocator.toFileURL(FrameworkUtil.getBundle(getClass()).getEntry((ORIGINAL_S_FILE_LOCATION)))
					.getPath();
			if (!Platform.getOS().equals(Platform.OS_LINUX)) {
				path = path.substring(1);
			}
			InputStream is = new ByteArrayInputStream(Files.readAllBytes(Paths.get(path)));
			file.create(is, false, monitor);
			file.refreshLocal(0, monitor);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
