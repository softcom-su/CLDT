package su.softcom.cldt.core.tests.clangtidyquickfixes;

import static org.junit.Assert.*;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Stream;

import org.eclipse.core.resources.*;
import org.eclipse.core.runtime.*;
import org.eclipse.core.runtime.jobs.Job;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import su.softcom.cldt.ui.resolvers.quickfixes.ClangTidyAutoQuickFix;


public class TestAutoQuickFix {

	private IProject project;
	private IProgressMonitor monitor;

	@BeforeEach
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

	private IMarker addMarkerForError(IFile cppFile, MarkerInfo markerAttributes) throws CoreException {
		Map<String, Object> attributes = new HashMap<>();
		attributes.put(IMarker.LOCATION,
				cppFile.getLocation().toOSString() + ":" + markerAttributes.lineNumber + ":" + markerAttributes.column); //$NON-NLS-1$ //$NON-NLS-2$
		attributes.put(IMarker.LINE_NUMBER, markerAttributes.lineNumber);
		attributes.put(IMarker.SEVERITY, IMarker.SEVERITY_WARNING);
		attributes.put(IMarker.MESSAGE, markerAttributes.message);
		attributes.put("checkId", markerAttributes.checkId); //$NON-NLS-1$

		return cppFile.createMarker("su.softcom.cldt.core.marker.clangTidyMarker", attributes); //$NON-NLS-1$
	}
	
	public static Stream<Arguments> testData() {
        return Stream.of(
            Arguments.of(
                "cppcoreguidelines-init-variables", //$NON-NLS-1$
                """
                int main() {
                    int x;
                    return x;
                }
                """, //$NON-NLS-1$
                "int x = 0;", //$NON-NLS-1$
                new MarkerInfo(2, 8, "Variable 'x' is not initialized", "cppcoreguidelines-init-variables") //$NON-NLS-1$ //$NON-NLS-2$
            ),
            Arguments.of(
                "modernize-use-auto", //$NON-NLS-1$
                """
                #include <vector>
                int main() {
                    std::vector<int>::iterator it = std::vector<int>().begin();
                    return 0;
                }
                """, //$NON-NLS-1$
                "auto it", //$NON-NLS-1$
                new MarkerInfo(3, 4, "use auto when declaring iterators", "modernize-use-auto") //$NON-NLS-1$ //$NON-NLS-2$
            ),
            Arguments.of(
                "modernize-use-nullptr", //$NON-NLS-1$
                """
                #include <iostream>
                int main() {
                    int* p = NULL;
                    return 0;
                }
                """, //$NON-NLS-1$
                "int* p = nullptr", //$NON-NLS-1$
                new MarkerInfo(3, 13, "use nullptr instead of NULL", "modernize-use-nullptr") //$NON-NLS-1$ //$NON-NLS-2$
            )
        );
    }

	@ParameterizedTest
	@MethodSource("testData")
	public void testQuickFixApplication(String testName, String sourceCode, String expectedFix, MarkerInfo markerInfo) throws CoreException, OperationCanceledException, InterruptedException {
		IPath filePath = new Path("auto_quick_fix_" + testName + "_test.cpp"); //$NON-NLS-1$ //$NON-NLS-2$
		IFile tempSourceFile = project.getFile(filePath);

		tempSourceFile.create(new ByteArrayInputStream(sourceCode.getBytes()), true, monitor);

		IMarker marker = addMarkerForError(tempSourceFile, markerInfo);

		new ClangTidyAutoQuickFix(marker).run(marker);

		Job.getJobManager().join(ResourcesPlugin.FAMILY_AUTO_BUILD, null);
		Job.getJobManager().join(ResourcesPlugin.FAMILY_MANUAL_REFRESH, null);

		tempSourceFile.refreshLocal(IResource.DEPTH_ZERO, new NullProgressMonitor());

		try (InputStream is = tempSourceFile.getContents()) {
			String fixedContent = new String(is.readAllBytes());

			assertTrue("Файл должен содержать: " + expectedFix, fixedContent.contains(expectedFix)); //$NON-NLS-1$
		} catch (IOException e) {
			throw new AssertionError("Ошибка ввода-вывода при тестировании файла", e); //$NON-NLS-1$
		}

		tempSourceFile.delete(true, null);
	}

	@Test
	public void testQuickFixWithMultipleMarkers()
			throws CoreException, OperationCanceledException, InterruptedException {
		
		String sourceCode = """
				#include <vector>
				std::vector<int> createVec() { return {}; }
				int main() {
				    std::vector<int>::iterator it = createVec().begin();
				    return *it;
				}
				"""; //$NON-NLS-1$

		String expectedFix = "auto it = createVec().begin()"; //$NON-NLS-1$

		MarkerInfo primaryMarker = new MarkerInfo(4, 4, "use auto when declaring iterators", "modernize-use-auto"); //$NON-NLS-1$ //$NON-NLS-2$
		MarkerInfo secondaryMarker = new MarkerInfo(4, 4, "iterator could be const", "misc-const-correctness"); //$NON-NLS-1$ //$NON-NLS-2$

		IPath filePath = new Path("multi_marker_test.cpp"); //$NON-NLS-1$
		IFile tempSourceFile = project.getFile(filePath);

		tempSourceFile.create(new ByteArrayInputStream(sourceCode.getBytes()), true, monitor);

		IMarker autoMarker = addMarkerForError(tempSourceFile, primaryMarker);
		addMarkerForError(tempSourceFile, secondaryMarker);

		new ClangTidyAutoQuickFix(autoMarker).run(autoMarker);

		Job.getJobManager().join(ResourcesPlugin.FAMILY_AUTO_BUILD, null);
		tempSourceFile.refreshLocal(IResource.DEPTH_ZERO, monitor);

		try (InputStream is = tempSourceFile.getContents()) {
			String fixedContent = new String(is.readAllBytes());

			assertTrue("Должен быть применен modernize-use-auto-фикс", fixedContent.contains(expectedFix)); //$NON-NLS-1$

			assertFalse("Не должен быть применен misc-const-correctness-фикс", fixedContent.contains("const iterator")); //$NON-NLS-1$ //$NON-NLS-2$
		} catch (IOException e) {
			throw new AssertionError("Ошибка ввода-вывода при тестировании файла", e); //$NON-NLS-1$
		}

		tempSourceFile.delete(true, null);
	}

	@AfterEach
	public void tearDown() throws CoreException {
		project.delete(true, true, monitor);
	}

	private static class MarkerInfo {
		int lineNumber;
		int column;
		String message;
		String checkId;

		public MarkerInfo(int lineNumber, int column, String message, String checkId) {
			this.lineNumber = lineNumber;
			this.column = column;
			this.message = message;
			this.checkId = checkId;
		}
	}
}