package su.softcom.cldt.core.tests.valgrind;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Status;
import org.junit.Before;
import org.junit.Test;

import su.softcom.cldt.internal.core.analysis.valgrind.ValgrindJob;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;

import org.eclipse.core.runtime.NullProgressMonitor;

import org.junit.After;

public class TestValgrindJob {

	private IProject project;
	private IProgressMonitor monitor;

	@Before
	public void setUp() throws CoreException {
		IWorkspace workspace = ResourcesPlugin.getWorkspace();
		IWorkspaceRoot root = workspace.getRoot();
		project = root.getProject("TestProject"); //$NON-NLS-1$
		monitor = new NullProgressMonitor();

		if (!project.exists()) {
			project.create(monitor);
		}
		project.open(monitor);
	}

	@After
	public void tearDown() throws CoreException {
		project.delete(true, true, monitor);
	}

	@Test
	public void testValgrindJobUninitializedMemory() throws IOException, InterruptedException, CoreException {

		// ------------------CREATE FILE-------------------- //
		IPath filePath = new Path("uninitialized_memory_test.cpp"); //$NON-NLS-1$
		IFile tempSourceFile = project.getFile(filePath);
		String sourceCode = "#include <iostream>\n" //$NON-NLS-1$
				+ "int main() {\n" //$NON-NLS-1$
				+ "    char uninitialized;\n" //$NON-NLS-1$
				+ "    std::cout << uninitialized << std::endl;\n" //$NON-NLS-1$
				+ "    return 0;\n" //$NON-NLS-1$
				+ "}\n"; //$NON-NLS-1$
		tempSourceFile.create(new ByteArrayInputStream(sourceCode.getBytes()), true, monitor);

		// ------------------COMPILE FILE-------------------- //

		IFile tempExecutable = project.getFile("uninitialized_memory_test_executable"); //$NON-NLS-1$

		ProcessBuilder compileProcessBuilder = new ProcessBuilder("clang++", "-gdwarf-4", "-o", //$NON-NLS-1$//$NON-NLS-2$//$NON-NLS-3$
				tempExecutable.getLocation().toString(), tempSourceFile.getLocation().toString());
		Process compileProcess = compileProcessBuilder.start();

		int compileExitCode = compileProcess.waitFor();
		if (compileExitCode != 0) {
			try (BufferedReader errorReader = new BufferedReader(
					new InputStreamReader(compileProcess.getErrorStream()))) {
				StringBuilder errorMessage = new StringBuilder();
				String line;
				while ((line = errorReader.readLine()) != null) {
					errorMessage.append(line).append("\n"); //$NON-NLS-1$
				}
				fail("Compilation failed:\n" + errorMessage); //$NON-NLS-1$
			}
		}

		// ------------------START TEST ValgrindJob-------------------- //

		ValgrindJob valgrindJob = new ValgrindJob(project, tempExecutable.getLocation().toString(), "--leak-check=yes"); //$NON-NLS-1$

		Status status = (Status) valgrindJob.run(monitor);

		if (status.isOK()) {
			fail("Expected Valgrind to report an error for uninitialized memory, but it passed."); //$NON-NLS-1$
		}

		String errorMessage = status.getMessage();
		System.out.println("Valgrind error: " + errorMessage); //$NON-NLS-1$

		assertEquals(IStatus.ERROR, status.getSeverity());
		tempExecutable.delete(false, monitor);
		tempSourceFile.delete(false, monitor);
	}

	@Test
	public void testValgrindJobSuccess() throws IOException, InterruptedException, CoreException {

		// ------------------CREATE FILE-------------------- //

		IPath filePath = new Path("success_test.cpp"); //$NON-NLS-1$
		IFile tempSourceFile = project.getFile(filePath);
		String sourceCode = "#include <iostream>\n" //$NON-NLS-1$
				+ "int main() {\n" //$NON-NLS-1$
				+ "    return 0;\n" //$NON-NLS-1$
				+ "}\n"; //$NON-NLS-1$
		tempSourceFile.create(new ByteArrayInputStream(sourceCode.getBytes()), true, monitor);

		// ------------------COMPILE FILE-------------------- //

		IFile tempExecutable = project.getFile("success_test"); //$NON-NLS-1$

		ProcessBuilder compileProcessBuilder = new ProcessBuilder("clang++", "-gdwarf-4", "-o", //$NON-NLS-1$//$NON-NLS-2$//$NON-NLS-3$
				tempExecutable.getLocation().toString(), tempSourceFile.getLocation().toString());

		Process compileProcess = compileProcessBuilder.start();

		int compileExitCode = compileProcess.waitFor();
		if (compileExitCode != 0) {
			try (BufferedReader errorReader = new BufferedReader(
					new InputStreamReader(compileProcess.getErrorStream()))) {
				StringBuilder errorMessage = new StringBuilder();
				String line;
				while ((line = errorReader.readLine()) != null) {
					errorMessage.append(line).append("\n"); //$NON-NLS-1$
				}
				fail("Compilation failed:\n" + errorMessage); //$NON-NLS-1$
			}
		}

		// ------------------START TEST ValgrindJob-------------------- //

		ValgrindJob valgrindJob = new ValgrindJob(project, tempExecutable.getLocation().toString(), "--leak-check=yes"); //$NON-NLS-1$

		Status status = (Status) valgrindJob.run(monitor);

		if (!status.isOK()) {
			fail("Expected Valgrind to pass."); //$NON-NLS-1$
		}

		assertEquals(IStatus.OK, status.getSeverity());
		
		tempExecutable.delete(false, monitor);
		tempSourceFile.delete(false, monitor);
		}
}
