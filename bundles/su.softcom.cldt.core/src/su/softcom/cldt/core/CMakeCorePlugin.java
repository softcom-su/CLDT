package su.softcom.cldt.core;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IConfigurationElement;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IExtensionRegistry;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Plugin;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.IJobChangeEvent;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.jobs.JobChangeAdapter;
import org.osgi.framework.FrameworkUtil;
import org.osgi.framework.ServiceReference;
import org.osgi.service.component.annotations.Activate;
import org.osgi.service.component.annotations.Component;
import org.osgi.service.component.annotations.Deactivate;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.util.tracker.ServiceTracker;

import su.softcom.cldt.analysis.core.AnalysisProvider;
import su.softcom.cldt.analysis.core.IAnalysisTool;
import su.softcom.cldt.core.builders.CMakeProcessMonitor;
import su.softcom.cldt.core.cmake.CMakeToolsUtil;
import su.softcom.cldt.core.cmake.ICMakeInstance;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.llvm.ILLVMInstance;
import su.softcom.cldt.core.llvm.LLVMToolsUtil;
import su.softcom.cldt.internal.core.CMakeProject;
import su.softcom.cldt.internal.core.CppFileManager;
import su.softcom.cldt.internal.core.CppResourceChangeListener;

/**
 * Основной класс core-плагина поддержки CMake в CLDT.
 * <p>
 * Отвечает за интеграцию CMake-функциональности в Eclipse, регистрацию ключевых
 * сервисов, хранение глобальных констант и идентификаторов, а также за
 * старт/остановку плагина.
 * </p>
 *
 * <p>
 * Используется как точка входа OSGi Declarative Services (DS), благодаря
 * аннотации {@code @Component}. Все основные операции и бизнес-логика должны
 * быть вынесены в отдельные сервисы или утилиты (например,
 * {@link CMakeToolsUtil}).
 * </p>
 *
 */
@Component
public class CMakeCorePlugin extends Plugin {

	/**
	 * Идентификатор маркера сборки для CMake (используется в системе маркеров
	 * Eclipse).
	 */
	public static final String BUILD_MARKER = "su.softcom.cldt.common.markers.buildmarker"; //$NON-NLS-1$

	/**
	 * Идентификатор плагина (используется в preferences, логах, регистрациях
	 * сервисов).
	 */
	public static final String PLUGIN_ID = "su.softcom.cldt.core"; //$NON-NLS-1$

	/**
	 * 
	 */
	public static final CMakeProcessMonitor cmakeOutputMonitor = new CMakeProcessMonitor();
	/**
	 * 
	 */
	public static final IInheriterIOStream buildOutputMonitor = new ProcessOutputMonitor();
	/**
	 * 
	 */
	public static final CMakeCorePlugin eINSTANCE = new CMakeCorePlugin();
	private static final Map<IProject, ICMakeProject> cmakeProjects = new ConcurrentHashMap<>();
	private final CppFileManager fileManager = new CppFileManager();
	private AnalysisProvider analyzisProvider;
	ServiceTracker<IWorkspace, IWorkspace> tracker;

	/**
	 * Логирует ошибку в системный журнал Eclipse с указанием сообщения и
	 * исключения.
	 *
	 * @param message   текстовое описание ошибки
	 * @param throwable исключение, связанное с ошибкой (может быть {@code null})
	 */
	public static void logError(String message, Throwable throwable) {
		Platform.getLog(CMakeCorePlugin.class).log(new Status(IStatus.ERROR, PLUGIN_ID, message, throwable));
	}

	/**
	 * Логирует информационное сообщение в системный журнал Eclipse.
	 *
	 * @param message текст сообщения
	 */
	public static void logInfo(String message) {
		Platform.getLog(CMakeCorePlugin.class).log(new Status(IStatus.INFO, PLUGIN_ID, message));
	}

	/**
	 * Логирует предупреждение в системный журнал Eclipse.
	 *
	 * @param message   текст предупреждения
	 * @param throwable исключение (опционально)
	 */
	public static void logWarning(String message, Throwable throwable) {
		Platform.getLog(CMakeCorePlugin.class).log(new Status(IStatus.WARNING, PLUGIN_ID, message, throwable));
	}

	/**
	 * 
	 */
	public CMakeCorePlugin() {
		analyzisProvider = new AnalysisProvider();
	}

	/**
	 * Монитор сборки CMake
	 * 
	 * @return CMakeProcessMonitor
	 */
	public static CMakeProcessMonitor getCMakeBuildMonitor() {
		return cmakeOutputMonitor;
	}

	/**
	 * @return IInheriterIOStream
	 */
	public static IInheriterIOStream getBuildOutputMonitor() {
		return buildOutputMonitor;
	}

	/**
	 * Возвращает глобальный экземпляр {@link CMakeCorePlugin} (singleton).
	 * 
	 * @return eINSTANCE
	 */
	public static CMakeCorePlugin getDefault() {
		return eINSTANCE;
	}

	/**
	 * Возвращает AnalysisProvider.
	 *
	 * @return экземпляр {@link AnalysisProvider}
	 */
	public AnalysisProvider getAnalyzisProvider() {
		return analyzisProvider;
	}

	private void initAnalyzeTools() {
		IExtensionRegistry registry = Platform.getExtensionRegistry();
		IConfigurationElement[] config = registry.getConfigurationElementsFor(IAnalysisTool.I_ANALYZIS_TOOL_ID);
		try {
			for (IConfigurationElement e : config) {
				final Object o = e.createExecutableExtension("class"); //$NON-NLS-1$
				if (o instanceof IAnalysisTool analyzisTool) {
					analyzisProvider.reportAnalyzisTool(analyzisTool);
				}
			}
		} catch (CoreException e) {
			logError("Ошибка инициализации инструментов анализа: " + e.getLocalizedMessage(), e);
		}
	}

	/**
	 * Method to activate with validation manager.
	 * 
	 */
	@Activate
	public void start() {

		LLVMToolsUtil.loadLLVMPathsFromPreferences(PLUGIN_ID);
		LLVMToolsUtil.scheduleLLVMInitJob();
		disableAutoBuild();
		Job cmakeJob = CMakeToolsUtil.getCMakeInitJob();
		cmakeJob.addJobChangeListener(new JobChangeAdapter() {
			@Override
			public void done(IJobChangeEvent event) {
				for (IProject prj : ResourcesPlugin.getWorkspace().getRoot().getProjects()) {
					CMakeProject cmakePrj = new CMakeProject(prj);
					cmakeProjects.put(prj, cmakePrj);
				}
			}
		});
		cmakeJob.schedule();
		Job.create("Инициализация инструментов анализа", new ICoreRunnable() {

			@Override
			public void run(IProgressMonitor monitor) throws CoreException {
				initAnalyzeTools();

			}
		}).schedule();

		tracker = new ServiceTracker<>(FrameworkUtil.getBundle(getClass()).getBundleContext(), IWorkspace.class, null) {

			@Override
			public IWorkspace addingService(ServiceReference<IWorkspace> reference) {
				IWorkspace result = super.addingService(reference);
				result.addResourceChangeListener(new CppResourceChangeListener());
				return result;
			}

		};
		tracker.open();
		Platform.getLog(getClass()).info("CLDT core plugin activated");
	}

	/**
	 * @throws BackingStoreException
	 */
	@Deactivate
	public void stop() throws BackingStoreException {
		for (Map.Entry<IProject, ICMakeProject> e : cmakeProjects.entrySet()) {
			((CMakeProject) e.getValue()).save();
		}
		LLVMToolsUtil.saveLLVMPathsToPreferences(PLUGIN_ID);
		tracker.close();
	}

	/**
	 * Возвращает экземпляр {@link ICMakeProject} для указанного Eclipse-проекта.
	 * <p>
	 * Если проект обладает одной из CMake-природ ({@code CMakeUpdateNature} или
	 * {@code CmakeProjectNature}), то возвращается существующий объект из кеша или
	 * создаётся новый. В случае ошибки или отсутствия CMake-природы возвращается
	 * {@code null}.
	 * </p>
	 *
	 * @param project Eclipse-проект, для которого требуется получить CMake-обёртку
	 * @return объект {@link ICMakeProject}, либо {@code null}, если проект не
	 *         является CMake или при ошибке
	 */
	public ICMakeProject getProject(IProject project) {
		try {
			if (project.hasNature(CMakeUpdateNature.ID) || project.hasNature(CmakeProjectNature.ID)) {
				ICMakeProject result = cmakeProjects.get(project);
				if (result == null) {
					result = new CMakeProject(project);
					cmakeProjects.put(project, result);
				}
				return result;
			}
		} catch (CoreException e) {
			CMakeCorePlugin.logError(
					"Ошибка при определении природы проекта или создании CMakeProject для: " + project.getName(), e);
		}
		return null;
	}

	/**
	 * Возвращает экземпляр {@link ICMakeProject} по имени проекта в рабочем
	 * пространстве Eclipse.
	 * <p>
	 * Является обёрткой над {@link #getProject(IProject)}. Если проект с заданным
	 * именем существует и является CMake-проектом, возвращается его
	 * {@link ICMakeProject}. В противном случае возвращается {@code null}.
	 * </p>
	 *
	 * @param name имя проекта в Eclipse workspace
	 * @return объект {@link ICMakeProject}, либо {@code null}, если проект не
	 *         найден или не является CMake-проектом
	 */
	public ICMakeProject getProject(String name) {
		IProject prj = ResourcesPlugin.getWorkspace().getRoot().getProject(name);
		return getProject(prj);
	}

	/**
	 * Возвращает экземпляр {@link ILLVMInstance} для запрошенной версии LLVM.
	 * <p>
	 * Гарантирует, что инициализация инструментов LLVM завершена до возврата
	 * результата, блокируя поток при необходимости. В случае прерывания потока
	 * возвращает {@code null}.
	 * </p>
	 *
	 * @param version версия LLVM, для которой требуется получить экземпляр; если
	 *                {@code null}, возвращается экземпляр по умолчанию
	 * @return объект {@link ILLVMInstance} или {@code null}, если поток был прерван
	 */
	public ILLVMInstance getLLVMInstance(Version version) {
		try {
			LLVMToolsUtil.awaitInit();
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
			return null;
		}
		return LLVMToolsUtil.doGetLLVMInstance(version);
	}

	/**
	 * Возвращает экземпляр {@link ICMakeInstance} (обычно — по умолчанию).
	 * <p>
	 * Блокирует поток до окончания инициализации CMake-инстансов, чтобы
	 * гарантировать корректное состояние данных. В случае прерывания потока
	 * возвращает {@code null}.
	 * </p>
	 *
	 * @return объект {@link ICMakeInstance} или {@code null}, если поток был
	 *         прерван
	 */
	public ICMakeInstance getCMakeInstance() {
		try {
			CMakeToolsUtil.awaitInit();
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
			return null;
		}
		return CMakeToolsUtil.getCMakeInstance();
	}

	/**
	 * Delete cmakeProject from projects map
	 * 
	 * @param cmakeProject
	 */
	public void deleteCMakeProject(ICMakeProject cmakeProject) {
		cmakeProjects.remove(cmakeProject.getProject());
	}

	/**
	 * Возвращает менеджер файлов Сpp проектов.
	 * 
	 * @return менеджер файлов для операций с кастомными файлами Cpp проектов
	 */
	public CppFileManager getFileManager() {
		return fileManager;
	}

	/**
	 * Выключает auto-building, перехватывая возможное CoreException.
	 * 
	 **/
	private static void disableAutoBuild() {
		try {
			var ws = ResourcesPlugin.getWorkspace();
			var desc = ws.getDescription();
			desc.setAutoBuilding(false);
			ws.setDescription(desc);
		} catch (CoreException e) {
			CMakeCorePlugin.logError("Ошибка при установке флага auto-building", e);
		}
	}

}
