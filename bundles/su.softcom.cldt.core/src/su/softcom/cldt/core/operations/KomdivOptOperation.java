package su.softcom.cldt.core.operations;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.PartInitException;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.ide.IDE;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/**
 * Operation for optimization on Komdiv. It works only on Windows and only for
 * bcvs.
 */
public class KomdivOptOperation implements ICoreRunnable {

	/** Path to bash. */
	private static final String BASH_PATH = CInitializer.getPreferenceStore().getString(CInitializer.BASH_PATH);
	/** Path to clang. */
	private static final String CLANG_PATH = Path.of("C:", "excellence", "clang-komdiv-win", "bin", "clang.exe") //$NON-NLS-1$//$NON-NLS-2$//$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$
			.toString();
	/** Path to directory with optimization scripts. */
	private static final String OPT_PATH = Path.of("C:", "excellence", "crc.for.eclipse", "O3").toString(); //$NON-NLS-1$ //$NON-NLS-2$//$NON-NLS-3$ //$NON-NLS-4$
	/** Name of obfuscated file. */
	private static final String OBFUSCATED_FILE = "CRC.6.S"; //$NON-NLS-1$
	/** Path to image script. */
	private static final String IMAGE_SCRIPT_PATH = "/mnt/c/excellence/crc.v3/src/generate_image.sh"; //$NON-NLS-1$
	/** Path to directory with optimization scripts in wsl style. */
	private static final String WSL_SCRIPT_PATH = "/mnt/c/excellence/crc.for.eclipse/O3"; //$NON-NLS-1$
	/** Name of obfuscated json file. */
	private static final String OBFUSCATED_JSON_FILE = "CRC.p6.json"; //$NON-NLS-1$

	/** Selected resource. */
	private IResource selected;

	/**
	 * Constructs a new komdiv optimization operation.
	 *
	 * @param selected selected resource
	 */
	public KomdivOptOperation(IResource selected) {
		this.selected = selected;
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {

		try {
			if (selected == null) {
				return;
			}

			buildWithObfuscation();
			generateImage();

			// Opening log information
			Display.getDefault().asyncExec(new Runnable() {
				@Override
				public void run() {
					// Project of selected resource
					IProject project = selected.getProject();
					IFolder obfFolder = project.getFolder("build").getFolder("opt");
					IFile log = obfFolder.getFile("log.txt");

					// Getting workbench page
					IWorkbenchPage workbenchPage = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage();

					try {
						IDE.openEditor(workbenchPage, log);
					} catch (PartInitException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			});

			// Opening image
			Display.getDefault().asyncExec(new Runnable() {
				@Override
				public void run() {
					// Project of selected resource
					IProject project = selected.getProject();
					IFolder obfFolder = project.getFolder("build").getFolder("opt");
					IFile image = obfFolder.getFile("result.htm");

					// Getting workbench page
					IWorkbenchPage workbenchPage = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage();

					try {
						IDE.openEditor(workbenchPage, image);
					} catch (PartInitException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			});
		} catch (IOException | InterruptedException e) {
			Platform.getLog(getClass()).error(e.getLocalizedMessage());
		}
	}

	/**
	 * Builds project with obfuscation.
	 * 
	 * @throws CoreException
	 * @throws IOException
	 * @throws InterruptedException
	 */
	private void buildWithObfuscation() throws CoreException, IOException, InterruptedException {
		// Project of selected resource
		IProject project = selected.getProject();

		// Path to script for build
		String pathToScript = project.getFolder("MSPO").getFolder("v1.0").getFolder("vm6").getFile("clang.sh") //$NON-NLS-1$ //$NON-NLS-2$//$NON-NLS-3$ //$NON-NLS-4$
				.getLocation().toOSString().replace("\\", "/"); //$NON-NLS-1$ //$NON-NLS-2$

		// Path to clang
		String pathToClang = getClangPath().replace("\\", "/"); //$NON-NLS-1$ //$NON-NLS-2$

		// Path to folder for build
		String pathToBuild = project.getFolder("build").getFolder("opt").getLocation().toOSString().replace("\\", "/"); //$NON-NLS-1$//$NON-NLS-2$ //$NON-NLS-3$

		// Creating build folder if it doesn't exist
		IFolder buildFolder = project.getFolder("build"); //$NON-NLS-1$
		if (!buildFolder.exists()) {
			buildFolder.create(IResource.FORCE, true, new NullProgressMonitor());
		}

		// Creating obfuscation folder if it doesn't exist
		IFolder obfFolder = buildFolder.getFolder("opt"); //$NON-NLS-1$
		if (!obfFolder.exists()) {
			obfFolder.create(IResource.FORCE, true, new NullProgressMonitor());
		}

		// Path to directory with optimization scripts
		String pathToOptDir = getOptPath().replace("\\", "/"); //$NON-NLS-1$ //$NON-NLS-2$

		// Command
		String command = pathToScript + " " + pathToClang + " " + pathToOptDir + " " + OBFUSCATED_FILE; //$NON-NLS-1$//$NON-NLS-2$ //$NON-NLS-3$

		// Runs in git bash, -c means terminate after this run
		String commands[] = { getShellPath(), "-c", command }; //$NON-NLS-1$
		ProcessBuilder processBuilder = new ProcessBuilder(commands);
		processBuilder.directory(new File(pathToBuild));
		processBuilder.inheritIO();
		Process process = processBuilder.start();

		// Printing output and errors
		BufferedReader outputReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
		BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()));

		while (process.isAlive()) {
			if (outputReader.ready()) {
				String outline = outputReader.readLine();
				if (outline != null) {
					System.out.println(outline);
				}
			}
			if (errorReader.ready()) {
				String errorLine = errorReader.readLine();
				if (errorLine != null) {
					System.out.println(errorLine);
				}
			}
		}

		// Printing exit code
		int exitCode = process.waitFor();
		System.out.println("\nExited with error code : " + exitCode);

		// Refreshing project view
		project.refreshLocal(IResource.DEPTH_INFINITE, new NullProgressMonitor());
	}

	/**
	 * Generates images with obfuscation in build directory.
	 * 
	 * @throws CoreException
	 * @throws IOException
	 * @throws InterruptedException
	 */
	private void generateImage() throws CoreException, IOException, InterruptedException {
		// Project of selected resource
		IProject project = selected.getProject();

		// Path to folder for build
		String pathToBuild = project.getFolder("build").getFolder("opt").getLocation().toOSString().replace("\\", "/"); //$NON-NLS-1$//$NON-NLS-2$ //$NON-NLS-3$

		// Creating build folder if it doesn't exist
		IFolder buildFolder = project.getFolder("build"); //$NON-NLS-1$
		if (!buildFolder.exists()) {
			buildFolder.create(IResource.FORCE, true, new NullProgressMonitor());
		}

		// Creating obfuscation folder if it doesn't exist
		IFolder obfFolder = buildFolder.getFolder("opt"); //$NON-NLS-1$
		if (!obfFolder.exists()) {
			obfFolder.create(IResource.FORCE, true, new NullProgressMonitor());
		}

		// Runs in wsl
		String command[] = { "wsl", IMAGE_SCRIPT_PATH, WSL_SCRIPT_PATH, OBFUSCATED_JSON_FILE }; //$NON-NLS-1$
		ProcessBuilder processBuilder = new ProcessBuilder(command);
		processBuilder.directory(new File(pathToBuild));
		processBuilder.inheritIO();
		Process process = processBuilder.start();

		// Printing output and errors
		BufferedReader outputReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
		BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()));

		while (process.isAlive()) {
			if (outputReader.ready()) {
				String outline = outputReader.readLine();
				if (outline != null) {
					System.out.println(outline);
				}
			}
			if (errorReader.ready()) {
				String errorLine = errorReader.readLine();
				if (errorLine != null) {
					System.out.println(errorLine);
				}
			}
		}

		// Printing exit code
		int exitCode = process.waitFor();
		System.out.println("\nExited with error code : " + exitCode);

		// Refreshing project view
		project.refreshLocal(IResource.DEPTH_INFINITE, new NullProgressMonitor());

	}

	/**
	 * Get bash shell path from preferences.
	 * 
	 * @return absolute bash shell path
	 * 
	 * @throws CoreException if shell not found or not executable
	 */
	private String getShellPath() throws CoreException {
		Path path = Paths.get(BASH_PATH);

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			throw new CoreException(new Status(IStatus.ERROR, getClass(), "Bash shell not found"));
		}

		return path.toString();
	}

	/**
	 * Get clang path from preferences.
	 * 
	 * @return absolute clang path
	 * 
	 * @throws CoreException if clang not found or not executable
	 */
	private String getClangPath() throws CoreException {
		Path path = Paths.get(CLANG_PATH);

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			throw new CoreException(new Status(IStatus.ERROR, getClass(), "Clang not found"));
		}

		return path.toString();
	}

	/**
	 * Get optimization scripts path from preferences.
	 * 
	 * @return absolute optimization scripts path
	 * 
	 * @throws CoreException if optimization scripts not found
	 */
	private String getOptPath() throws CoreException {
		Path path = Paths.get(OPT_PATH);

		if (!Files.exists(path)) {
			throw new CoreException(new Status(IStatus.ERROR, getClass(), "Optimization scripts not found"));
		}

		return path.toString();
	}

}
