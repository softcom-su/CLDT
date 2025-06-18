package su.softcom.cldt.core.tests.cmake;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

import java.io.File;

import java.io.IOException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.IWorkspaceDescription;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.ILogListener;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Order;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.osgi.framework.FrameworkUtil;
import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.internal.core.builders.CMakeConfigureBuilder;

@TestInstance(Lifecycle.PER_CLASS)
public class CMakeConfigureBuilderParametersTest {

	private IProject testProject;
	private List<String> capturedLogMessages = new ArrayList<>();

	@BeforeAll
	public void setUp() throws CoreException, URISyntaxException, IOException {
		File projectSourcesFolder = new File(
				FileLocator.toFileURL(FrameworkUtil.getBundle(CMakeConfigureBuilderParametersTest.class)
						.getEntry("resources" + IPath.SEPARATOR + "TestProjects" //$NON-NLS-1$ //$NON-NLS-2$
								+ IPath.SEPARATOR + "CMakeConfigureBuilderTestProject")) //$NON-NLS-1$
						.toURI());
		importProject(projectSourcesFolder);

		// Prevent auto-build after tearDown is called
		IWorkspaceDescription workspaceDescription = ResourcesPlugin.getWorkspace().getDescription();
		workspaceDescription.setAutoBuilding(false);
		ResourcesPlugin.getWorkspace().setDescription(workspaceDescription);

		ICMakeProject cmakeProject = CMakeCorePlugin.getDefault().getProject(testProject);
		Map<String, String> arguments = Map.of("CMAKE_C_COMPILER", "clang", //$NON-NLS-1$ //$NON-NLS-2$
				"TEST_VAR", "ON"); //$NON-NLS-1$ //$NON-NLS-2$
		cmakeProject.setBuildArguments(arguments);

		// Capture log messages that represent CMake configure command
		ILogListener logListener = (status, plugin) -> {
		    if (status.getSeverity() == IStatus.INFO && !status.getMessage().startsWith("Start")) { //$NON-NLS-1$
		        capturedLogMessages.add(status.getMessage());
		    }
		};


		Platform.getLog(CMakeConfigureBuilder.class).addLogListener(logListener);
	}

	@AfterAll
	public void tearDown() throws CoreException {
		testProject.build(IncrementalProjectBuilder.CLEAN_BUILD, new NullProgressMonitor());
	}

	@Order(1)
	@Test
	public void shouldWriteParametersToConsole() throws CoreException {
		testProject.build(IncrementalProjectBuilder.FULL_BUILD, new NullProgressMonitor());
		assertTrue(capturedLogMessages.get(0).contains("-DCMAKE_C_COMPILER=")); //$NON-NLS-1$
		assertTrue(capturedLogMessages.get(0).contains("-DTEST_VAR=")); //$NON-NLS-1$
	}

	@Order(2)
	@Test
	public void shouldNOTWriteParametersToConsole() throws CoreException {
		testProject.build(IncrementalProjectBuilder.FULL_BUILD, new NullProgressMonitor());
		assertFalse(capturedLogMessages.get(1).contains("-DCMAKE_C_COMPILER")); //$NON-NLS-1$
		assertFalse(capturedLogMessages.get(1).contains("-DTEST_VAR=")); //$NON-NLS-1$
	}

	private void importProject(final File baseDirectory) throws CoreException {
		IProjectDescription projectDescription = ResourcesPlugin.getWorkspace()
				.loadProjectDescription(new Path(baseDirectory.getAbsolutePath() + "/.project")); //$NON-NLS-1$
		testProject = ResourcesPlugin.getWorkspace().getRoot().getProject(projectDescription.getName());
		testProject.create(projectDescription, new NullProgressMonitor());
		testProject.open(new NullProgressMonitor());
	}

}
