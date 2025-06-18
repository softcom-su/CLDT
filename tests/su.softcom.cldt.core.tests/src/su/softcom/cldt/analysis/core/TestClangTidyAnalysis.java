package su.softcom.cldt.analysis.core;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Stream;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.IWorkspaceDescription;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.osgi.service.prefs.BackingStoreException;
import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.operations.ClangTidyAnalysisTool;

public class TestClangTidyAnalysis {

	private ClangTidyAnalysisTool analysisTool;
	private IProject testProject;
	private IProgressMonitor monitor = new NullProgressMonitor();
	private IWorkspaceRoot root;
	private String projectName = "ClangTidyAnalysisTestProject"; //$NON-NLS-1$
	private String projectPath = "./resources/TestProjects/ClangTidyAnalysisTestProject"; //$NON-NLS-1$
	private IFile mainFile;
	private IFile anotherFile;
	private IFile inappropriateFile;
	private IEclipsePreferences store;
	private String checks = "checks";

	@BeforeEach
	public void setUp() throws CoreException {
		analysisTool = new ClangTidyAnalysisTool();

		IWorkspace workspace = ResourcesPlugin.getWorkspace();
		IWorkspaceDescription desc = workspace.getDescription();
		desc.setAutoBuilding(false);
		workspace.setDescription(desc);

		root = workspace.getRoot();

		copyProjectToWorkspace(projectPath, root.getLocation().toOSString());

		testProject = createAndImportProject();

		root.refreshLocal(IResource.DEPTH_INFINITE, monitor);

		testProject = root.getProject(projectName);
		store = (IEclipsePreferences) new ProjectScope(testProject).getNode(PreferenceConstants.NODE)
				.node("clang-tidy"); //$NON-NLS-1$
		mainFile = testProject.getFile("src/main.cpp"); //$NON-NLS-1$
		anotherFile = testProject.getFile("additionalDirectory/anotherFile.cpp"); //$NON-NLS-1$
		inappropriateFile = testProject.getFile("additionalDirectory/inappropriateFile.txt"); //$NON-NLS-1$
	}

	@Test
	public void testRunningOnFileWithoutCertainErrorsDoesNotGenerateMarkers() throws CoreException {
		if (!testProject.exists() || !testProject.isOpen()) {
			throw new IllegalStateException("Project does not exist or is not open."); //$NON-NLS-1$
		}

		try {
			store.clear();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		// The file does not contain these issues
		store.put(checks,
				"performance-no-int-to-ptr,bugprone-assert-side-effect,readability-braces-around-statements"); //$NON-NLS-1$

		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		List<IResource> resources = List.of(mainFile);

		analysisTool.execute(resources, new NullProgressMonitor());

		testProject.refreshLocal(IResource.DEPTH_INFINITE, null);

		boolean markersNotExist = !checkForMarkers(mainFile, null);
		assertTrue("Markers should not be found in main.cpp after analysis.", markersNotExist); //$NON-NLS-1$
	}

	@Test
	public void testGenerateCorrectMarkers() throws CoreException, InterruptedException {
		if (!testProject.exists() || !testProject.isOpen()) {
			throw new IllegalStateException("Project does not exist or is not open."); //$NON-NLS-1$
		}

		try {
			store.clear();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		// The files contains these issues
		store.put(checks,
				"readability-magic-numbers,cppcoreguidelines-init-variables"); //$NON-NLS-1$
		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		List<IResource> resources = List.of(mainFile);

		analysisTool.execute(resources, monitor);

		Job.getJobManager().join(ResourcesPlugin.FAMILY_AUTO_BUILD, null);
		Job.getJobManager().join(ResourcesPlugin.FAMILY_MANUAL_REFRESH, null);

		testProject.refreshLocal(IResource.DEPTH_ZERO, null);

		Set<String[]> expectedMarkers = new HashSet<>();

		expectedMarkers
				.add(new String[] { "3", "1", "10 is a magic number; consider replacing it with a named constant" }); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
		expectedMarkers.add(new String[] { "2", "1", "variable 'a' is not initialized" }); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$

		boolean markersExist = checkForMarkers(mainFile, expectedMarkers);
		assertTrue("Markers should be found in main.cpp after analysis.", markersExist); //$NON-NLS-1$
	}

	@Test
	public void testCompileCommandsJsonGeneration() throws CoreException {
		if (!testProject.exists() || !testProject.isOpen()) {
			throw new IllegalStateException("Project does not exist or is not open."); //$NON-NLS-1$
		}

		IFile compileCommandsFile = testProject.getFile("build/compile_commands.json"); //$NON-NLS-1$
		if (compileCommandsFile.exists()) {
			compileCommandsFile.delete(true, null);
		}

		testProject.refreshLocal(IResource.DEPTH_INFINITE, null);

		List<IResource> resources = List.of(mainFile);

		analysisTool.execute(resources, monitor);

		testProject.refreshLocal(IResource.DEPTH_INFINITE, null);

		compileCommandsFile = testProject.getFile("build/compile_commands.json"); //$NON-NLS-1$
		boolean compileCommandsExists = compileCommandsFile.exists();
		assertTrue("compile_commands.json should be generated after analysis.", compileCommandsExists); //$NON-NLS-1$
	}

	@Test
	public void testRunOnProject() throws CoreException, InterruptedException {
		if (!testProject.exists() || !testProject.isOpen()) {
			throw new IllegalStateException("Project does not exist or is not open."); //$NON-NLS-1$
		}

		try {
			store.clear();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		// The files contains these issues
		store.put(checks, "readability-magic-numbers,cppcoreguidelines-init-variables"); //$NON-NLS-1$//$NON-NLS-2$
		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		List<IResource> resources = List.of(testProject);

		analysisTool.execute(resources, monitor);
		Job.getJobManager().join(ResourcesPlugin.FAMILY_AUTO_BUILD, null);
		Job.getJobManager().join(ResourcesPlugin.FAMILY_MANUAL_REFRESH, null);

		testProject.refreshLocal(IResource.DEPTH_INFINITE, null);

		boolean markersExist1 = checkForMarkers(mainFile, null);
		boolean markersExist2 = checkForMarkers(anotherFile, null);
		assertTrue("Markers should be found in main.cpp after analysis.", markersExist1); //$NON-NLS-1$
		assertTrue("Markers should be found in anotherFile.cpp after analysis.", markersExist2); //$NON-NLS-1$
	}

	@Test
	public void testRunWithInappropriateFileShouldNotThrowException() throws CoreException {
		if (!testProject.exists() || !testProject.isOpen()) {
			throw new IllegalStateException("Project does not exist or is not open."); //$NON-NLS-1$
		}

		try {
			store.clear();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		store.put(checks, "readability-magic-numbers"); //$NON-NLS-1$
		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		List<IResource> resources = List.of(inappropriateFile);

		try {
			analysisTool.execute(resources, monitor);
		} catch (Exception e) {
			fail("Method threw an exception: " + e.getMessage()); //$NON-NLS-1$
		}

		testProject.refreshLocal(IResource.DEPTH_INFINITE, null);
	}

	private void copyProjectToWorkspace(String projectDir, String workspaceDir) {
		Path source = Paths.get(projectDir);
		Path destination = Paths.get(workspaceDir).resolve(source.getFileName());

		try (Stream<Path> stream = Files.walk(source)) {
			stream.forEach(sourcePath -> {
				Path destPath = destination.resolve(source.relativize(sourcePath));
				try {
					if (Files.isDirectory(sourcePath)) {
						if (!Files.exists(destPath)) {
							Files.createDirectories(destPath);
						}
					} else {
						Files.copy(sourcePath, destPath, java.nio.file.StandardCopyOption.REPLACE_EXISTING);
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			});
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private IProject createAndImportProject() throws CoreException {
		IWorkspace workspace = root.getWorkspace();
		IProject project = root.getProject(projectName);

		if (!project.exists()) {
			IProjectDescription description = workspace.newProjectDescription(projectName);

			description.setLocation(root.getLocation().append(projectName));
			description.setNatureIds(new String[]{"su.softcom.cldt.core.cnature"}); //$NON-NLS-1$

			project.create(description, null);

			project.open(null);
		} else {
			if (!project.isOpen()) {
				project.open(monitor);
			}
		}

		return project;
	}

	private boolean checkForMarkers(IFile file, Set<String[]> expectedMarkers) throws CoreException {
		IMarker[] markers = file.findMarkers("su.softcom.cldt.core.marker.clangTidyMarker", true, IResource.DEPTH_ZERO); //$NON-NLS-1$
		boolean hasCorrectMarkers = true;

		if (expectedMarkers != null) {
			for (IMarker marker : markers) {
				boolean fileContainsCorrectMarker = false;

				for (String[] expectedMarker : expectedMarkers) {
					if (markerInExpected(marker, expectedMarker)) {
						fileContainsCorrectMarker = true;
						break;
					}
				}

				if (!fileContainsCorrectMarker) {
					hasCorrectMarkers = false;
					break;
				}
			}
		}

		return markers.length > 0 && hasCorrectMarkers;
	}

	private boolean markerInExpected(IMarker marker1, String[] marker2) throws CoreException {
		return marker1.getAttribute(IMarker.LINE_NUMBER).toString().equals(marker2[0])
				&& marker1.getAttribute(IMarker.SEVERITY).toString().equals(marker2[1])
				&& marker1.getAttribute(IMarker.MESSAGE).equals(marker2[2]);
	}
}