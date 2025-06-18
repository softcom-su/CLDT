package su.softcom.cldt.core.cmake;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IAdaptable;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.lsp4j.jsonrpc.Launcher;
import org.osgi.framework.FrameworkUtil;

import su.softcom.cldt.core.ProjectDependency;
import su.softcom.cldt.core.exceptions.BuildFolderException;
import su.softcom.cldt.core.source.ISourceFile;
import su.softcom.cldt.core.source.SourceFile;
import su.softcom.cldt.internal.core.lsp.LanguageServerWrapper;

public interface ICMakeProject extends IAdaptable {

	/**
	 * Id for request node from project scope with exists targets.
	 */
	final String TARGET_CONTAINER = "targets-container"; //$NON-NLS-1$

	final String TARGETS_LIST = "targets-list"; //$NON-NLS-1$

	final String DEPENDENCIES_CONTAINER = "dependencies-container"; //$NON-NLS-1$

	final String CMAKE_LISTS_NAME = "CMakeLists.txt"; //$NON-NLS-1$

	final String DEPENDENCIES_LIST = "dependencies-container"; //$NON-NLS-1$

	final String BUILD_ARGUMENTS_LIST = "arguments-list"; //$NON-NLS-1$

	final String BUILD_ARGUMENTS_CONTAINER = "arguments-container"; //$NON-NLS-1$

	final String BUILD_ARGUMENT_VALUE = "argument-value"; //$NON-NLS-1$

	final String BUILD_TYPE = "build-type"; //$NON-NLS-1$

	final String BUILD_FOLDER = "build-folder"; //$NON-NLS-1$

	final String TOOLCHAIN = "toolchain"; //$NON-NLS-1$

	final String PROJECT_SELECTED_LANGUAGE = "project_selected_language"; //$NON-NLS-1$

	final String LANGUAGE_C = "language_c"; //$NON-NLS-1$

	final String DEFAULT_TARGET_LANGUAGE_STANDARD = "default_target_language_standard"; //$NON-NLS-1$

	final String LANGUAGE_CXX = "language_cxx"; //$NON-NLS-1$

	/**
	 * Regex for CMakeCache variable matching
	 */
	final String CACHE_ARGUMENT_REGEX = "^([a-zA-Z0-9/\\\\\\\\.\\\\\\\\\\\\\\\\_-]+):([A-Z]+)=([a-zA-Z0-9/\\\\.\\\\\\\\_-]*)$"; //$NON-NLS-1$

	/**
	 * CMakeCache file line after which internal variables are listed
	 */
	final String INTERNAL_CACHE_VARIABLES_LINE = "# INTERNAL cache entries"; //$NON-NLS-1$

	/**
	 * Папка установки для cmake --install
	 */
	final String DEFAULT_INSTALL_PATH_POSTFIX = "_install"; //$NON-NLS-1$

	/**
	 * 
	 * @return Возвращает представление {@link org.eclipse.core.resources.IProject
	 *         IProject} этого CMakeProject
	 */
	public IProject getProject();

	/**
	 * 
	 * @return Возвращает список таргетов этого проекта.
	 * @see su.softcom.cldt.core.cmake.Target Target
	 */
	public List<Target> getTargets();

	/**
	 * 
	 * @param name - имя таргета внутри проекта
	 * @return Ищет тагрет с переданным именем внутри проекта. Возвращает null, если
	 *         такого Target с таким именем не существует.
	 */
	public Target getTarget(String name);

	/**
	 * 
	 * Передается в cmake с флагом -B
	 * 
	 * @return Директорию сборки проекта.
	 */
	public IFolder getBuildFolder();

	/**
	 * Получить результат последней попытки конфигурации проекта.
	 * 
	 * @return {@code true} если конфигурация завершилась успешно. {@code false} в
	 *         противном случае.
	 * 
	 */
	public boolean getConfigurationStatus();

	/**
	 * Получить статус последней попытки сборки проекта.
	 * 
	 * @return {@code true} если сборка завершилась успешно. {@code false} в
	 *         противном случае.
	 */
	public boolean getBuildStatus();

	/**
	 * 
	 * @return IFile кэш
	 */
	public IFile getCache();

	/**
	 * 
	 * @return выбранный генератор для сборки
	 * @see su.softcom.cldt.core.cmake.ICMakeGenerator ICMakeGenerator
	 */
	public ICMakeGenerator getGenerator();

	/**
	 * 
	 * @return выбранный тулчейн для сборки
	 */
	public Path getToolchain();

	/**
	 * 
	 * @return список ProjectDependency
	 */
	public List<ProjectDependency> getDependencies();

	/**
	 * Sets external dependency. If a dependency with the specified name already
	 * exists, it will be replaced by the newly created one.
	 * 
	 * @param dependency
	 * @see su.softcom.cldt.core.ProjectDependency ProjectDependency
	 */
	public void addDependency(ProjectDependency dependency);

	/**
	 * Sets newDependencies as a new dependencies list for this project.
	 * 
	 * @param newDependencies
	 */
	public void setDependencies(List<ProjectDependency> newDependencies);

	/**
	 * @param name
	 * @return project dependency with the specified name or {@code null} if no such
	 *         dependency exists
	 */
	public ProjectDependency getDependency(String name);

	public ICMakeInstance getCmakeInstance();

	public void addTarget(Target target);

	public void setTargets(List<Target> targets);

	public CMakeRoot getCMakeTree();

	public Map<String, String> getBuildArguments();

	/**
	 * Sets new project build arguments.
	 * 
	 * @param buildArguments
	 */
	public void setBuildArguments(Map<String, String> buildArguments);

	/**
	 * Parses the provided CMakeCache.txt file and adds all variables (excluding the
	 * internal ones) to a <name, value> map and returns it.
	 * 
	 * @param cmakeCache
	 * @return cached variables map
	 */
	public static Map<String, String> parseCMakeCache(IFile cmakeCache) {
		final Pattern cacheVariablePattern = Pattern.compile(CACHE_ARGUMENT_REGEX);
		Map<String, String> cachedVariables = new HashMap<>();
		try (BufferedReader reader = new BufferedReader(new InputStreamReader(cmakeCache.getContents()))) {
			String line;
			// CMakeCache is structured in such way that internal variables are separated
			// from external
			// ones with a special line which equals INTERNAL_CACHE_VARIABLES_LINE. User
			// does not need internal ones.
			while (!(line = reader.readLine()).equals(INTERNAL_CACHE_VARIABLES_LINE)) {
				Matcher matcher = cacheVariablePattern.matcher(line); // 1 -- variable name, 3 --
																		// variable value
				if (matcher.matches()) {
					cachedVariables.put(matcher.group(1), matcher.group(3));
				}
			}
		} catch (IOException | CoreException e) {
			Platform.getLog(FrameworkUtil.getBundle(ICMakeProject.class)).error(e.getMessage());
		}
		return cachedVariables;
	}

	/**
	 * Устанавливает имя папки, которая будет использоваться для размещения
	 * артефактов сборки проекта.
	 * <p>
	 * Папка сборки будет создана внутри корня проекта, если она ещё не существует.
	 * Имя не должно содержать относительный путь или разделители каталогов, а
	 * должно быть только именем одной директории (например, {@code "build"} или
	 * {@code "out"}). Если также используется активная конфигурация сборки, папка
	 * для неё будет создаваться вложенно автоматически.
	 * <p>
	 * Если указанный ресурс уже существует и не является папкой, или при создании
	 * папки возникла ошибка, выбрасывается {@link BuildFolderException}.
	 * </p>
	 *
	 * @param buildFolderName имя папки сборки (без относительных путей)
	 * @throws BuildFolderException если папка не может быть создана или указан
	 *                              некорректный путь
	 */
	public void setBuildFolder(String buildFolderName) throws BuildFolderException;

	/**
	 * Возвращает исходный файл по имени.
	 * 
	 * @param iFile объект эклипсового файла
	 * @return объект {@link SourceFile} или {@code null}, если файл не найден
	 */
	SourceFile getSourceFile(IFile iFile);

	/**
	 * Возвращает список всех исходных файлов проекта.
	 * 
	 * @return неизменяемый список {@link SourceFile} (пустой список, если файлов
	 *         нет)
	 */
	public List<ISourceFile> getSources();

	/**
	 * Удаляет исходный файл из проекта.
	 * 
	 * @param src {@link SourceFile сущность} для удаления (если {@code null},
	 *            операция игнорируется)
	 */
	public void removeSourceFile(ISourceFile src);

	/**
	 * Удаляет цель сборки из проекта.
	 * 
	 * @param tgt цель для удаления (если не существует, операция игнорируется)
	 * @return true, если цель сборки была удалена.
	 */
	public boolean removeTarget(Target tgt);

	/**
	 * Возвращает экземпляр {@link Launcher} для запуска Language Server Protocol
	 * (LSP).
	 *
	 * @return {@link Launcher} объекта, управляющего запуском и коммуникацией с
	 *         Language Server.
	 */
	LanguageServerWrapper getLSPLauncher();

}
