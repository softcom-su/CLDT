package su.softcom.cldt.core.tests.clangtidyquickfixes;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.junit.Before;
import org.junit.Test;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.ui.resolvers.quickfixes.ClangTidyRenameQuickFix;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import static org.junit.Assert.assertEquals;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;

import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.OperationCanceledException;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.junit.After;

public class TestClangTidyRenameQuickFix {

	private IProject project;
	private IProgressMonitor monitor;
	private IEclipsePreferences instancePreferences;

	@Before
	public void setUp() throws CoreException {
		IWorkspace workspace = ResourcesPlugin.getWorkspace();
		IWorkspaceRoot root = workspace.getRoot();
		project = root.getProject("TestProject"); //$NON-NLS-1$
		monitor = new NullProgressMonitor();
		
		instancePreferences = InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE);
		instancePreferences.put(PreferenceConstants.CLANG_PATH, "/usr/lib/llvm-14"); //$NON-NLS-1$

		if (!project.exists()) {
			project.create(monitor);
		}
		
		project.open(monitor);
	}

	@Test
	public void testRenameFixApplication() throws CoreException, OperationCanceledException, InterruptedException {
		String originalCode =
				"""
				int main() {
				    int oldName = 0;
				    return oldName;
				}
				"""; //$NON-NLS-1$
		
		String expectedCode =
				"""
				int main() {
				    int newName = 0;
				    return newName;
				}
				"""; //$NON-NLS-1$

		IFile tempSourceFile = project.getFile("rename_test.cpp"); //$NON-NLS-1$
		tempSourceFile.create(new ByteArrayInputStream(originalCode.getBytes()), true, monitor);

		Map<String, Object> attributes = new HashMap<>();
		attributes.put(IMarker.LOCATION, tempSourceFile.getLocation().toOSString() + ":2:9"); //$NON-NLS-1$
		attributes.put(IMarker.LINE_NUMBER, 2);
		attributes.put(IMarker.SEVERITY, IMarker.SEVERITY_WARNING);
		attributes.put(IMarker.MESSAGE, "Variable 'oldName' has a bad name"); //$NON-NLS-1$
		attributes.put("checkId", "readability-identifier-naming"); //$NON-NLS-1$ //$NON-NLS-2$

		IMarker testMarker = tempSourceFile.createMarker("su.softcom.cldt.core.marker.clangTidyMarker", attributes); //$NON-NLS-1$

		ClangTidyRenameQuickFix quickFix = new ClangTidyRenameQuickFix() {
			@Override
			protected String getNewName(IMarker marker) {
				return "newName"; //$NON-NLS-1$
			}
		};

		quickFix.run(testMarker);

		Job.getJobManager().join(ResourcesPlugin.FAMILY_AUTO_BUILD, null);
		Job.getJobManager().join(ResourcesPlugin.FAMILY_MANUAL_REFRESH, null);

		tempSourceFile.refreshLocal(IResource.DEPTH_ZERO, monitor);

		try (InputStream is = tempSourceFile.getContents()) {
			String fixedContent = new String(is.readAllBytes());

			assertEquals("Код должен быть переименован", expectedCode, fixedContent); //$NON-NLS-1$
		} catch (IOException e) {
			throw new AssertionError("Ошибка ввода-вывода при тестировании файла", e); //$NON-NLS-1$
		}

		tempSourceFile.delete(true, null);
	}

	@After
	public void tearDown() throws CoreException, BackingStoreException {
		instancePreferences.clear();
		project.delete(true, true, monitor);
	}
}