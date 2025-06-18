package su.softcom.cldt.debug.tests;

import static org.junit.jupiter.api.Assertions.assertNotNull;

import java.io.FileInputStream;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.concurrent.TimeUnit;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IConfigurationElement;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.Timeout;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;
import su.softcom.cldt.debug.internal.core.lldb.AbstractLLDBJob;

@TestInstance(Lifecycle.PER_CLASS)
class LLDBJobTests {
	//TODO: do more tests for LLDB Job
	static final String projectName = "test_project";
	static IProject project = null;

	@BeforeAll
	public static void beforeAll() throws CoreException {

		project = ResourcesPlugin.getWorkspace().getRoot().getProject("lldb-test");
		project.create(new NullProgressMonitor());
	}

	@AfterAll
	public void afterAll() throws CoreException {
		project.delete(true, new NullProgressMonitor());
	}

	@Test
	@Timeout(unit = TimeUnit.MINUTES, value = 5)
	void testCreatePipe() throws CoreException, IOException, URISyntaxException {

		project.open(new NullProgressMonitor());
		Bundle bundle = FrameworkUtil.getBundle(this.getClass());
		assertNotNull(bundle, "Bundle is null");

		String resoursePath = bundle.getResource(DebugUtils.EXEC_PATH + projectName).getFile();
		String bundlePath = FileLocator.getBundleFileLocation(bundle).get().getAbsolutePath();
		String execFilePath = bundlePath + resoursePath;
		FileInputStream stream = new FileInputStream(execFilePath);

		IFile program = project.getFile("test_project");
		program.create(stream, false, new NullProgressMonitor());
		program.getLocation().toFile().setExecutable(true);
		IConfigurationElement[] elements = Platform.getExtensionRegistry()
				.getConfigurationElementsFor("su.softcom.cldt.debug.core.interal.core.lldb");
		String pipeName = "/tmp/lldb-server-%s-in-test";
		for (IConfigurationElement extension_element : elements) {
			Object obj = extension_element.createExecutableExtension("class");
			AbstractLLDBJob lldbDebugJob = ((AbstractLLDBJob) obj);
			lldbDebugJob.setDebugProgram(program.getLocationURI());
			lldbDebugJob.setProgramArgs(new ArrayList<String>());
			lldbDebugJob.setPipeName(pipeName);
			lldbDebugJob.createPipe();
		}

	}

}
