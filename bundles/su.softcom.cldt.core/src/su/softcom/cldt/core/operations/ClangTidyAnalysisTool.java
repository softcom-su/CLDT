package su.softcom.cldt.core.operations;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.MessageConsole;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.analysis.core.IAnalysisTool;
import su.softcom.cldt.common.preferences.PreferenceConstants;

/**
 * Implementation AnalysisTool extension point for clang-tidy.
 */
public class ClangTidyAnalysisTool implements IAnalysisTool {

	private static final String ID = "su.softcom.cldt.core.anlyzis.clang-tidy"; //$NON-NLS-1$
	private static final String UI_NAME = "Статический анализ clang-tidy"; //$NON-NLS-1$
	private static final String CLANG_TIDY = "clang-tidy"; //$NON-NLS-1$

	private IProject project;
	private IEclipsePreferences store;
	private String buildFolder;

	private HashSet<IFile> projectSourceFiles = new HashSet<IFile>();

	@Override
	public String getID() {
		return ID;
	}

	@Override
	public String getUIName() {
		return UI_NAME;
	}

	private Set<IFile> addSourceFiles(IResource resource) throws CoreException {
		HashSet<IFile> ret = new HashSet<IFile>();

		if (resource instanceof IFolder folder) {
			if (resource.getName().equals(buildFolder)) {
				return new HashSet<IFile>();
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
		Job job = new Job(UI_NAME) {
			@Override
			protected IStatus run(IProgressMonitor subMonitor) {
				return executeClangTidy(resources, subMonitor);
			}
		};

		job.schedule();
	}

	private IStatus executeClangTidy(List<IResource> resources, IProgressMonitor monitor) {
		MessageConsole console = findConsole("Clang-Tidy Console"); //$NON-NLS-1$

		try {
			for (IResource resource : resources) {
				projectSourceFiles.clear();
				project = resource.getProject();
				store = (IEclipsePreferences) new ProjectScope(project).getNode(PreferenceConstants.NODE)
						.node(CLANG_TIDY);
				buildFolder = new ProjectScope(project).getNode(PreferenceConstants.NODE)
						.get(PreferenceConstants.BUILD_FOLDER, PreferenceConstants.DEFAULT_BUILD_FOLDER);

				initProjectSourceFiles(List.of(resource));

				for (IFile file : projectSourceFiles) {
					file.deleteMarkers("su.softcom.cldt.marker.analysisMarker", false, 0); //$NON-NLS-1$
				}

				ClangTidyOperation clangTidyOperation = new ClangTidyOperation(project, projectSourceFiles, console,
						getClangTidyParameters(), buildFolder);
				clangTidyOperation.run(monitor);
			}
		} catch (CoreException e) {
			return new Status(IStatus.ERROR, getClass().getName(), e.getLocalizedMessage(), e);
		} finally {
			monitor.done();
		}
		return Status.OK_STATUS;
	}

	private List<String> getClangTidyParameters() {
		ArrayList<String> parameters = new ArrayList<String>();
		StringBuilder sb = new StringBuilder();

		try {
			for (String key : store.keys()) {
				String value = store.get(key, null);

				if (value != null && !value.isEmpty()) {
					sb.append("-").append(key).append("=").append(value); //$NON-NLS-1$ //$NON-NLS-2$
					parameters.add(sb.toString());
					sb.setLength(0);
				}
			}
		} catch (BackingStoreException e) {
			e.printStackTrace();
		}

		return parameters;
	}

	private MessageConsole findConsole(String name) {
		ConsolePlugin plugin = ConsolePlugin.getDefault();
		MessageConsole console = null;
		IConsole[] consoles = plugin.getConsoleManager().getConsoles();
		for (IConsole c : consoles) {
			if (c.getName().equals(name)) {
				console = (MessageConsole) c;
				console.clearConsole();
				break;
			}
		}
		if (console == null) {
			console = new MessageConsole(name, null);
			plugin.getConsoleManager().addConsoles(new IConsole[] { console });
		}
		return console;
	}
}