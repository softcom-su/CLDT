package su.softcom.cldt.core.operations;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.jobs.ISchedulingRule;
import org.eclipse.core.runtime.jobs.Job;

import su.softcom.cldt.analysis.core.IAnalysisTool;
import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.builders.CMakeBuilderArgs;

/**
 * Implementation AnalysisTool extension point for clang-check tool.
 */
public class CLangStaticAnalysisTool implements IAnalysisTool {

	private static final String ID = "su.softcom.cldt.core.anlyzis.clang"; //$NON-NLS-1$
	private static final String UI_NAME = "Статический анализ clang-check";

	private Map<IProject, Set<IFile>> projectFileMap;

	public CLangStaticAnalysisTool() {
		projectFileMap = new HashMap<IProject, Set<IFile>>();
	}

	@Override
	public String getID() {
		return ID;
	}

	@Override
	public String getUIName() {
		return UI_NAME;
	}

	private Set<IFile> getFilesFromResource(IResource resource) throws CoreException {
		HashSet<IFile> ret = new HashSet<IFile>();

		if (resource instanceof IFolder folder) {
			for (IResource innerResource : folder.members()) {
				ret.addAll(getFilesFromResource(innerResource));
			}
		} else if (resource instanceof IProject project) {
			if (!projectFileMap.containsKey(project)) {
				projectFileMap.put(project, new HashSet<IFile>());
			}
			for (IResource innerResource : project.members()) {
				projectFileMap.get(project).addAll(getFilesFromResource(innerResource));
			}
		} else if (resource instanceof IFile file) {
			ret.add(file);
		}

		return ret;
	}

	private void initProjectFileMap(List<IResource> resources) throws CoreException {
		for (IResource resource : resources) {
			Set<IFile> tSet = getFilesFromResource(resource);
			if (!tSet.isEmpty()) {
				IProject tProject = tSet.iterator().next().getProject();
				if (!projectFileMap.containsKey(tProject)) {
					projectFileMap.put(tProject, new HashSet<IFile>());
				}
				projectFileMap.get(tProject).addAll(tSet);
			}
		}
		projectFileMap.entrySet().stream().forEach(entry -> {
			String targets = new ProjectScope(entry.getKey()).getNode(PreferenceConstants.NODE)
					.get(PreferenceConstants.TARGETS_LIST, "");
			if (targets.isBlank()) {
				targets = entry.getKey().getName();
			}
			Set<IFile> fileSet = Stream.of(targets.split(PreferenceConstants.FIELD_SEPARATOR)).map(targetName -> {
				return Set.of(Stream
						.of(new ProjectScope(entry.getKey()).getNode(PreferenceConstants.NODE)
								.node(PreferenceConstants.NODE_OF_TARGETS).node(targetName)
								.get(PreferenceConstants.SRC_FILES, "").split(PreferenceConstants.FIELD_SEPARATOR))
						.filter(t -> !t.isBlank()).map(srcFilePathString -> entry.getKey().getFile(srcFilePathString))
						.toList().toArray(new IFile[0]));
			}).flatMap(Set::stream).collect(Collectors.toSet());

			entry.getValue().retainAll(fileSet);
		});
	}

	IPath getBuildRoot(Map<String, String> args, IProject project) {
		String buildRoot = args.get(CMakeBuilderArgs.ROOT_DIR);
		if (buildRoot == null) {
			return project.getLocation();
		}
		return project.getFolder(buildRoot).getLocation();
	}

	@Override
	public void execute(List<IResource> resources, IProgressMonitor monitor) {
		Set<IFile> files;
		try {
			initProjectFileMap(resources);

			List<CLangCheckOperation> сLangCheckOperationList1 = new ArrayList<CLangCheckOperation>();
			List<CLangCheckOperation> сLangCheckOperationList2 = new ArrayList<CLangCheckOperation>();

			CLangCheckOperation сLangCheckOperation;

			for (Entry<IProject, Set<IFile>> entry : projectFileMap.entrySet()) {

				entry.getKey().build(IncrementalProjectBuilder.INCREMENTAL_BUILD, monitor.slice(1));

				String buildFolder = new ProjectScope(entry.getKey()).getNode(PreferenceConstants.NODE)
						.get(PreferenceConstants.BUILD_FOLDER, PreferenceConstants.DEFAULT_BUILD_FOLDER);

				сLangCheckOperation = new CLangCheckOperation(entry.getKey(), entry.getValue(), null,
						new ArrayList<String>(), buildFolder); // $NON-NLS-1$
				сLangCheckOperationList1.add(сLangCheckOperation);

				сLangCheckOperation = new CLangCheckOperation(entry.getKey(), entry.getValue(), null,
						List.of("--analyze"), buildFolder); //$NON-NLS-1$
				сLangCheckOperationList2.add(сLangCheckOperation);
			}

			for (int i = 0; i < сLangCheckOperationList1.size(); i++) {
				final int tint = i;
				ISchedulingRule rule = new ISchedulingRule() {

					@Override
					public boolean contains(ISchedulingRule rule) {
						return (rule == this);
					}

					@Override
					public boolean isConflicting(ISchedulingRule rule) {
						return (rule == this);
					}

				};

				Job job1 = Job.create("Статический анализ clang: этап 1", сLangCheckOperationList1.get(tint));
				job1.setRule(rule);
				Job job2 = Job.create("Статический анализ clang: этап 2", сLangCheckOperationList2.get(tint));
				job2.setRule(rule);

				job1.schedule();
				job2.schedule();
			}
		} catch (CoreException e) {
			monitor.done();
			Platform.getLog(getClass()).error(e.getLocalizedMessage());
		}
		monitor.done();
	}

}
