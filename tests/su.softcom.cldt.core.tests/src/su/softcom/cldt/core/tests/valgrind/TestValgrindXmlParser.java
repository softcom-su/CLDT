package su.softcom.cldt.core.tests.valgrind;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Path;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import su.softcom.cldt.internal.core.analysis.valgrind.ValgrindError;
import su.softcom.cldt.internal.core.analysis.valgrind.ValgrindXmlParser;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class TestValgrindXmlParser {

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
	public void testValgrindXmlParsing() throws Exception {
		String holoFile = "/valgrindFiles/holo.xml"; //$NON-NLS-1$

		InputStream resourceStream = getClass().getResourceAsStream(holoFile);

		assertNotNull("Файл holo.xml не найден в ресурсах", resourceStream); //$NON-NLS-1$

		IPath tempFilePath = new Path("holo.xml"); //$NON-NLS-1$
		IFile tempFile = project.getFile(tempFilePath);

		if (!tempFile.exists()) {
			tempFile.create(resourceStream, true, monitor);
		} else {
			tempFile.setContents(resourceStream, true, false, monitor);
		}

		assertEquals(true, tempFile.exists());

		File tempExecutable = File.createTempFile("holo", ".c"); //$NON-NLS-1$ //$NON-NLS-2$
		tempExecutable.deleteOnExit();

		List<ValgrindError> errors = ValgrindXmlParser.parseValgrindXml(tempFile.getLocation().toOSString(),
				tempExecutable);

		assertNotNull(errors);

		ValgrindError error = errors.get(0);
		assertEquals("UninitCondition", error.kind); //$NON-NLS-1$
		assertEquals("Conditional jump or move depends on uninitialised value(s)", error.description); //$NON-NLS-1$
	}
}