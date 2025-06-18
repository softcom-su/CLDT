package su.softcom.cldt.core.operations;

import java.io.File;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.analysis.core.IAnalysisTool;
import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.internal.core.CMakeProject;
import su.softcom.cldt.internal.core.builders.CMakeConfigureBuilder;

/**
 * Implementation AnalysisTool extension point for clang-tidy.
 */
public class ClangTidyAnalysisTool implements IAnalysisTool {

	private static final String ID = "su.softcom.cldt.core.anlyzis.clang-tidy"; //$NON-NLS-1$
	private static final String UI_NAME = "Статический анализ clang-tidy"; //$NON-NLS-1$
	private static final String CLANG_TIDY = "clang-tidy"; //$NON-NLS-1$
	private static final String CLANG_TIDY_MARKER_ID = "su.softcom.cldt.core.marker.clangTidyMarker"; //$NON-NLS-1$
	private static final String COMPILE_COMMANDS = "compile_commands.json"; //$NON-NLS-1$

	private ICMakeProject project;
	private IEclipsePreferences store;
	private IEclipsePreferences globalPreferences;
	private String buildFolder;

	private HashSet<IFile> projectSourceFiles = new HashSet<>();

	@Override
	public String getID() {
		return ID;
	}

	@Override
	public String getUIName() {
		return UI_NAME;
	}

	private Set<IFile> addSourceFiles(IResource resource) throws CoreException {
		HashSet<IFile> ret = new HashSet<>();

		if (resource instanceof IFolder folder) {
			if (resource.getName().equals(buildFolder)) {
				return new HashSet<>();
			}

			for (IResource innerResource : folder.members()) {
				ret.addAll(addSourceFiles(innerResource));
			}
		} else if (resource instanceof IProject proj) {
			for (IResource innerResource : proj.members()) {
				Set<IFile> tSet = addSourceFiles(innerResource);
				ret.addAll(tSet);
			}
		} else if (resource instanceof IFile file) {
			String fileName = file.getName();
			if (fileName.endsWith(".c") || fileName.endsWith(".cpp")) { //$NON-NLS-1$ //$NON-NLS-2$
				ret.add(file);
			}
		}

		return ret;
	}

	private void initProjectSourceFiles(List<IResource> resources) throws CoreException {
		for (IResource resource : resources) {
			projectSourceFiles.addAll(addSourceFiles(resource));
		}
	}

	@Override
	public void execute(List<IResource> resources, IProgressMonitor monitor) {
		try {
			for (IResource resource : resources) {
				projectSourceFiles.clear();
				project = new CMakeProject(resource.getProject());
				store = (IEclipsePreferences) new ProjectScope(project.getProject()).getNode(PreferenceConstants.NODE)
						.node(CLANG_TIDY);
				globalPreferences = (IEclipsePreferences) InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE)
						.node(CLANG_TIDY);
				buildFolder = project.getBuildFolder().getLocation().toOSString();

				initProjectSourceFiles(List.of(resource));

				prepareCompilationDatabase();

				if (projectSourceFiles.isEmpty()) {
					return;
				}

				for (IFile file : projectSourceFiles) {
					file.deleteMarkers(CLANG_TIDY_MARKER_ID, false, 0);
				}

				ClangTidyOperation clangTidyOperation = new ClangTidyOperation(projectSourceFiles,
						getClangTidyParameters(), buildFolder);
				Job job = Job.create(UI_NAME, clangTidyOperation);
				job.setRule(project.getProject());
				job.schedule();
			}

		} catch (CoreException e) {
			e.printStackTrace();
		}
	}

	private List<String> getClangTidyParameters() {
		ArrayList<String> parameters = new ArrayList<>();
		StringBuilder sb = new StringBuilder();

		if (store.get("checks", null) == null && globalPreferences.get("checks", null) != null) {
			sb.append("--").append("checks").append("=").append(globalPreferences.get("checks", null));
			parameters.add(sb.toString());
			sb.setLength(0);
		}

		try {
			for (String key : store.keys()) {
				String value = store.get(key, null);

				if (value != null && !value.isEmpty()) {
					sb.append("--").append(key).append("=").append(value); //$NON-NLS-1$ //$NON-NLS-2$
					parameters.add(sb.toString());
					sb.setLength(0);
				}
			}
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		return parameters;
	}

	private void prepareCompilationDatabase() {
		File compileCommandsFile = new File(buildFolder, COMPILE_COMMANDS);

		if (!compileCommandsFile.exists()) {
			try {
				project.getProject().build(IncrementalProjectBuilder.INCREMENTAL_BUILD, CMakeConfigureBuilder.ID, null,
						new NullProgressMonitor());
			} catch (CoreException e) {
				e.printStackTrace();
			}
		}
	}

}
