package su.softcom.cldt.internal.core;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.preferences.DefaultScope;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.lsp4j.InitializeParams;
import org.eclipse.lsp4j.jsonrpc.Launcher;
import org.eclipse.lsp4j.launch.LSPLauncher;
import org.eclipse.lsp4j.services.LanguageServer;
import org.osgi.framework.FrameworkUtil;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.ProjectDependency;
import su.softcom.cldt.core.cmake.CMakeParser;
import su.softcom.cldt.core.cmake.CMakeParser.UnexpectedTokenException;
import su.softcom.cldt.core.cmake.CMakeRoot;
import su.softcom.cldt.core.cmake.ICMakeGenerator;
import su.softcom.cldt.core.cmake.ICMakeInstance;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.core.exceptions.BuildFolderException;
import su.softcom.cldt.core.source.ISourceFile;
import su.softcom.cldt.core.source.SourceFile;
import su.softcom.cldt.internal.core.lsp.ClangdLanguageClient;
import su.softcom.cldt.internal.core.lsp.LanguageServerWrapper;

/**
 * CLDT implementation of CMake Project
 */
public class CMakeProject implements ICMakeProject {

	private static final String DEFAULT_BUILD_FOLDER = "build"; //$NON-NLS-1$

	private class InitLSPLauncherJob extends Job {

		public InitLSPLauncherJob(ICMakeProject prj) {
			super("LSP launcher for %s project".formatted(prj.getProject().getName()));
		}

		@Override
		protected IStatus run(IProgressMonitor monitor) {
			List<String> commands = new ArrayList<>();
			commands.add("clangd");
			commands.add("--compile-commands-dir=%s".formatted(getBuildFolder().getLocation().toOSString()));
			ProcessBuilder lsBuilder = new ProcessBuilder(commands);
			lsBuilder.directory(project.getLocation().toFile());
			Process lsProcess;
			try {
				lsProcess = lsBuilder.start();
				launchServer(lsProcess);
			} catch (IOException e) {
				e.printStackTrace();
				return Status.CANCEL_STATUS;
			}
			return Status.OK_STATUS;
		}

		private void launchServer(Process lsProcess) {
			InputStream inputStream = lsProcess.getInputStream();
			OutputStream outputStream = lsProcess.getOutputStream();
			Launcher<LanguageServer> launcher = LSPLauncher.createClientLauncher(new ClangdLanguageClient(),
					inputStream, outputStream);
			wrapper = new LanguageServerWrapper(project, launcher);
			launcher.startListening();
			InitializeParams params = LanguageServerWrapper.getInitParams();
			launcher.getRemoteProxy().initialize(params);
		}

	}

	private final IProject project;
	private final List<Target> targets = new ArrayList<>();
	private CMakeRoot cmakeTree;
	private List<ProjectDependency> dependencies = new ArrayList<>();
	private IFolder buildFolder;
	private ICMakeInstance cmakeInstance;
	private Preferences properties;
	private Map<String, String> buildArguments = new HashMap<>();
	private LanguageServerWrapper wrapper;

	private boolean configStatus;
	private boolean buildStatus;

	/**
	 * Создает объект CMakeProject
	 * 
	 * @param project
	 */
	public CMakeProject(IProject project) {
		this.project = project;
		cmakeInstance = CMakeCorePlugin.getDefault().getCMakeInstance();
		properties = new ProjectScope(project).getNode(CMakeCorePlugin.PLUGIN_ID);
		IEclipsePreferences defaultPrefs = DefaultScope.INSTANCE.getNode(CMakeCorePlugin.PLUGIN_ID);
		setBuildFolder(properties.get(BUILD_FOLDER, defaultPrefs.get(BUILD_FOLDER, DEFAULT_BUILD_FOLDER)));
		Stream.of(properties.get(TARGETS_LIST, "").split(";")) //$NON-NLS-1$ //$NON-NLS-2$
				.filter(str -> !str.isEmpty()).forEach(targetName -> {
					Preferences tgtsPrefs = new ProjectScope(project.getProject()).getNode(CMakeCorePlugin.PLUGIN_ID)
							.node(ICMakeProject.TARGET_CONTAINER).node(targetName);
					addTarget(new Target(targetName, this, tgtsPrefs));
				});

		Stream.of(properties.get(DEPENDENCIES_LIST, "").split(";")).filter(str -> !str.isEmpty()) //$NON-NLS-1$//$NON-NLS-2$
				.forEach(dependencyName -> addDependency(new ProjectDependency(dependencyName, this)));

		Stream.of(properties.get(BUILD_ARGUMENTS_LIST, "").split(";")) //$NON-NLS-1$ //$NON-NLS-2$
				.filter(argumentName -> !argumentName.isBlank())
				.forEach(argumentName -> addArgument(argumentName, buildArguments));

		IFile cmakeLists = project.getFile(CMAKE_LISTS_NAME);
		if (cmakeLists.exists()) {
			CMakeParser parser;
			try (InputStream cmakeContents = cmakeLists.getContents()) {
				parser = new CMakeParser(new String(cmakeContents.readAllBytes()), false);
				cmakeTree = parser.parse();
			} catch (IOException | UnexpectedTokenException | CoreException e) {
				e.printStackTrace();
			}
		}
		new InitLSPLauncherJob(this).schedule();
	}

	@Override
	public IProject getProject() {
		return project;
	}

	@Override
	public List<Target> getTargets() {
		return Collections.unmodifiableList(targets);
	}

	private void addTargets(Collection<Target> tgts) {
		this.targets.addAll(tgts);
	}

	@Override
	public void setTargets(List<Target> targets) {
		this.targets.clear();
		addTargets(targets);
	}

	@Override
	public void addTarget(Target target) {
		if (target == null) {
			throw new IllegalArgumentException("target must be not null");
		}
		Target existsTarget = getTarget(target.getName());
		if (existsTarget != null) {
			throw new IllegalArgumentException("target with name %s already exists".formatted(target.getName()));
		}
		targets.add(target);
	}

	@Override
	public SourceFile getSourceFile(IFile iFile) {
		return targets.stream().flatMap(tgt -> tgt.getSources().stream())
				.filter(source -> source.getFile() != null && source.getFile().equals(iFile))
				.map(SourceFile.class::cast).findFirst().orElse(null);
	}

	@Override
	public Target getTarget(String name) {
		Target result = null;
		for (Target target : targets) {
			if (target.getName().equals(name)) {
				result = target;
				break;
			}
		}
		return result;
	}

	@Override
	public IFolder getBuildFolder() {
		if (buildFolder == null) {
			setBuildFolder(DEFAULT_BUILD_FOLDER);
		}
		return buildFolder;
	}

	@Override
	public void setBuildFolder(String buildFolderName) throws BuildFolderException {
		if (buildFolderName == null || buildFolderName.trim().isEmpty()) {
			throw new IllegalArgumentException("Папка сборки не может быть null или пустым");
		}
		String folderPath = null;
		try {
			folderPath = buildFolderName.trim() + IPath.SEPARATOR + project.getActiveBuildConfig().getName();
			IFolder finalBuildFolder = project.getFolder(folderPath);
			if (finalBuildFolder.exists() && finalBuildFolder.getType() != IResource.FOLDER) {
				throw new IllegalStateException("Ресурс '" + folderPath + "' не является папкой");
			}
			this.buildFolder = finalBuildFolder;
		} catch (CoreException e) {
			throw new BuildFolderException("Ошибка при создании папки сборки: " + folderPath, e);
		}
	}

	@Override
	public void addDependency(ProjectDependency dependency) {
		if (dependency == null) {
			throw new IllegalArgumentException("Dependency must be not null");
		}
		ProjectDependency existingDependency = getDependency(dependency.getName());
		if (existingDependency != null) {
			throw new IllegalArgumentException(
					"Dependency with name %s already exists".formatted(existingDependency.getName()));
		}
		dependencies.add(dependency);
	}

	@Override
	public void setDependencies(List<ProjectDependency> dependencies) {
		this.dependencies = dependencies;
	}

	@Override
	public IFile getCache() {
		return buildFolder.getFile("CMakeCache.txt"); //$NON-NLS-1$
	}

	@Override
	public ICMakeGenerator getGenerator() {
		return cmakeInstance.getInfo().getGenerators().stream().filter(e -> e.getName().equalsIgnoreCase("ninja")) //$NON-NLS-1$
				.toList().get(0);
	}

	@Override
	public Path getToolchain() {
		String pathStr = ""; //$NON-NLS-1$
		try {
			pathStr = FileLocator
					.toFileURL(FrameworkUtil.getBundle(getClass())
							.getEntry("resources" + IPath.SEPARATOR + "toolchains" + IPath.SEPARATOR //$NON-NLS-1$ //$NON-NLS-2$
									+ "toolchain-llvm.cmake")) //$NON-NLS-1$
					.getPath();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return new Path(pathStr);
	}

	@Override
	public ICMakeInstance getCmakeInstance() {
		return CMakeCorePlugin.getDefault().getCMakeInstance();
	}

	/**
	 * @throws BackingStoreException
	 */
	public void save() throws BackingStoreException {
		properties.put(BUILD_FOLDER, buildFolder.getProjectRelativePath().segment(0));
		properties.put(TARGETS_LIST, targets.stream().map(Target::getName).collect(Collectors.joining(";"))); //$NON-NLS-1$
		properties.put(DEPENDENCIES_LIST,
				dependencies.stream().map(ProjectDependency::getName).collect(Collectors.joining(";"))); //$NON-NLS-1$
		properties.put(ICMakeProject.BUILD_ARGUMENTS_LIST,
				buildArguments.keySet().stream().collect(Collectors.joining(";"))); //$NON-NLS-1$
		Preferences argumentsContainer = properties.node(ICMakeProject.BUILD_ARGUMENTS_CONTAINER);
		buildArguments.entrySet().forEach(entry -> argumentsContainer.node(entry.getKey())
				.put(ICMakeProject.BUILD_ARGUMENT_VALUE, entry.getValue()));
		for (Target target : targets) {
			target.save();
		}
		for (ProjectDependency dependency : dependencies) {
			dependency.save();
		}

		properties.flush();
	}

	@Override
	public List<ProjectDependency> getDependencies() {
		return dependencies;
	}

	/**
	 * @return cmake tree associated with this project. May return {@code null} if
	 *         the project does not contain CMakeLists.txt
	 */
	@Override
	public CMakeRoot getCMakeTree() {
		return cmakeTree;
	}

	/**
	 * Returns a project build arguments map copy.
	 *
	 * @return build arguments
	 */
	@Override
	public Map<String, String> getBuildArguments() {
		return new HashMap<>(buildArguments);
	}

	/**
	 * Takes argument name, retrieves argument value from eclipse preferences and
	 * adds this pair to specified arguments list.
	 *
	 * @param argumentName
	 * @param arguments    map
	 */
	private void addArgument(String argumentName, Map<String, String> arguments) {
		String argumentValue = properties.node(ICMakeProject.BUILD_ARGUMENTS_CONTAINER).node(argumentName)
				.get(ICMakeProject.BUILD_ARGUMENT_VALUE, ""); //$NON-NLS-1$
		arguments.put(argumentName, argumentValue);
	}

	@Override
	public ProjectDependency getDependency(String name) {
		for (var dependency : dependencies) {
			if (dependency.getName().contains(name)) {
				return dependency;
			}
		}
		return null;
	}

	@Override
	public void setBuildArguments(Map<String, String> buildArguments) {
		this.buildArguments = buildArguments;
	}

	/**
	 * Установить результат конфигурации проекта.
	 * 
	 * @param status
	 */
	public void setConfigurationStatus(boolean status) {
		this.configStatus = status;
	}

	/**
	 * Установить результат сборки проекта.
	 * 
	 * @param status
	 */
	public void setBuildStatus(boolean status) {
		this.buildStatus = status;
	}

	@Override
	public boolean getConfigurationStatus() {
		return configStatus;
	}

	@Override
	public void removeSourceFile(ISourceFile source) {
		Assert.isNotNull(source);
		targets.forEach(target -> target.removeSourceFile(source));
	}

	@Override
	public boolean removeTarget(Target target) {
		return targets.remove(target);
	}

	@Override
	public boolean getBuildStatus() {
		return buildStatus;
	}

	@Override
	public String toString() {
		return "CMakeProject: %s".formatted(this.getProject().getName());
	}

	@Override
	@SuppressWarnings("unchecked")
	public <T> T getAdapter(Class<T> adapter) {
		Assert.isNotNull(adapter);
		if (IProject.class.equals(adapter)) {
			return (T) project;
		}
		return null;
	}

	@Override
	public List<ISourceFile> getSources() {
		List<ISourceFile> result = new ArrayList<>();
		for (ISourceFile src : targets.stream().flatMap(tgt -> tgt.getSources().stream()).toList()) {
			if (!result.contains(src)) {
				result.add(src);
			}
		}
		return result;
	}

	@Override
	public LanguageServerWrapper getLSPLauncher() {
		return wrapper;
	}

}
