package su.softcom.cldt.core.llvm;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.preferences.ConfigurationScope;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.Version;
import su.softcom.cldt.internal.core.LLVMInstance;

/**
 * <p>
 * Утилитный класс для поиска, регистрации, хранения и получения путей к
 * инструментам LLVM в среде Eclipse RCP. Управляет списком известных папок,
 * обнаруживает LLVM-инструменты, парсит их версии, а также синхронизирует
 * состояние между настройками пользователя (preferences) и памятью.
 * </p>
 * <p>
 * Данный класс не предполагает создания экземпляра. Все методы и поля
 * статические.
 * </p>
 *
 */
public final class LLVMToolsUtil {

	private static ILLVMInstance defaultLLVMInstance;
	private static final String LLVM_PREFERENCES_LIST = "llvm-instances"; //$NON-NLS-1$
	private static final List<ILLVMInstance> LLVM_INSTANCES = new ArrayList<>();
	private static final List<String> LLVM_TOOLS = List.of("clang", "clang++", "clangd", "lldb", "lldb-server"); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$
	private static final List<Path> LLVM_SEARCH_LIST = Collections.synchronizedList(new ArrayList<>());
	private static final CountDownLatch llvmInitLatch = new CountDownLatch(1);

	static {
		List<Path> defLLVMPath = new ArrayList<>();

		if (Platform.getOS().equals(Platform.OS_LINUX)) {
			defLLVMPath.add(Path.of("/usr/bin")); //$NON-NLS-1$
			Path llvmRoot = Path.of("/usr/lib"); //$NON-NLS-1$
			try {
				if (Files.isDirectory(llvmRoot)) {
					try (var dirStream = Files.newDirectoryStream(llvmRoot, "llvm-*")) { //$NON-NLS-1$
						for (Path llvmFolder : dirStream) {
							Path binFolder = llvmFolder.resolve("bin"); //$NON-NLS-1$
							if (Files.isDirectory(binFolder)) {
								defLLVMPath.add(binFolder);
							}
						}
					}
				}
			} catch (IOException e) {
				CMakeCorePlugin.logWarning("Ошибка при поиске LLVM в /usr/lib", e);
			}
		} else if (Platform.getOS().equals(Platform.OS_WIN32)) {
			Path cRoot = null;
			for (File root : File.listRoots()) {
				String name = root.toString().toLowerCase();
				if (name.startsWith("c")) { //$NON-NLS-1$
					cRoot = root.toPath();
					break;
				}
			}
			if (cRoot != null) {
				defLLVMPath.add(cRoot.resolve("Program Files").resolve("LLVM").resolve("bin")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				defLLVMPath.add(cRoot.resolve("Program Files (x86)").resolve("LLVM").resolve("bin")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
			}
		}

		LLVM_SEARCH_LIST.addAll(defLLVMPath);
		CMakeCorePlugin.logInfo(
				"LLVM search list: " + LLVM_SEARCH_LIST.stream().map(Path::toString).collect(Collectors.joining(":"))); //$NON-NLS-1$ //$NON-NLS-2$
	}

	/**
	 * Асинхронно запускает инициализацию списка инструментов LLVM через Eclipse
	 * Job. После завершения job снимает блокировку (latch), чтобы другие части кода
	 * могли использовать инструменты.
	 */
	public static void scheduleLLVMInitJob() {
		Job llvmInitJob = Job.create("Инициализация инструментов LLVM", new ICoreRunnable() { //$NON-NLS-1$
			@Override
			public void run(IProgressMonitor monitor) throws CoreException {
				initLLVMToolkits(); //
			}
		});
		llvmInitJob.schedule();
	}

	private LLVMToolsUtil() {
	}

	private static void initLLVMToolkits() {
		List<File> folders = searchFolders();
		for (File folder : folders) {
			File[] tools = searchLLVMInFolder(folder);
			for (File tool : tools) {
				try {
					Version v = getVersion(tool);
					if (v != null) {
						LLVMInstance instance = (LLVMInstance) doGetLLVMInstance(v);
						if (instance == null) {
							instance = new LLVMInstance(v);
							LLVM_INSTANCES.add(instance);
						}
						instance.addTool(tool);
					}
				} catch (IOException e) {
					// ignore
				}
			}
		}

		LLVM_INSTANCES.sort(Comparator.comparing(ILLVMInstance::getVersion));
		CMakeCorePlugin.logInfo("Найдено LLVM инструментов: %d".formatted(LLVM_INSTANCES.size()));

		if (!LLVM_INSTANCES.isEmpty()) {
			defaultLLVMInstance = LLVM_INSTANCES.get(0);
		}
		if (llvmInitLatch.getCount() > 0) {
			llvmInitLatch.countDown();
		}
	}

	private static List<File> searchFolders() {
		return LLVM_SEARCH_LIST.stream().map(java.nio.file.Path::toFile).filter(File::exists).filter(File::isDirectory)
				.toList();
	}

	private static File[] searchLLVMInFolder(File folder) {
		Assert.isTrue(folder.isDirectory());
		String extPattern = "(?<!^)[.].*"; //$NON-NLS-1$
		return folder.listFiles(e -> (LLVM_TOOLS.contains(e.getName().replaceAll(extPattern, ""))) && e.canExecute()); //$NON-NLS-1$
	}

	private static Version getVersion(File executable) throws IOException {
		if (!executable.exists() || !executable.canExecute()) {
			throw new IOException("File not found or not executable: " + executable); //$NON-NLS-1$
		}
		ProcessBuilder builder = new ProcessBuilder(executable.getAbsolutePath(), "--version"); //$NON-NLS-1$
		Process p = builder.start();

		try (BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()))) {
			if (!p.waitFor(3, TimeUnit.SECONDS)) {
				p.destroy();
				throw new IOException("Timeout 3 seconds"); //$NON-NLS-1$
			}
			if (p.exitValue() == 0) {
				String line;
				while ((line = reader.readLine()) != null) {
					Version version = tryParseVersion(line);
					if (version != null) {
						return version;
					}
				}
			} else {
				throw new IOException("Process exited with code " + p.exitValue()); //$NON-NLS-1$
			}
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
			throw new IOException("Interrupted", e); //$NON-NLS-1$
		}
		return null;
	}

	private static Version tryParseVersion(String line) {
		try {
			return LLVMInstance.getVersion(line);
		} catch (IllegalArgumentException e) {
			return null;
		}
	}

	/**
	 * Возвращает {@link ILLVMInstance} для запрошенной версии LLVM, если такая
	 * версия обнаружена. Если версия {@code null}, возвращает экземпляр по
	 * умолчанию (наиболее раннюю/базовую найденную версию).
	 *
	 * @param version версия инструмента LLVM (или {@code null} для экземпляра по
	 *                умолчанию)
	 * @return подходящий экземпляр {@link ILLVMInstance}, либо {@code null}, если
	 *         не найден
	 */
	public static ILLVMInstance doGetLLVMInstance(Version version) {
		if (version == null) {
			return defaultLLVMInstance;
		}
		return LLVM_INSTANCES.stream().filter(i -> i.getVersion().equals(version)).findFirst().orElse(null);
	}

	/**
	 * Блокирует выполнение до окончания инициализации списка LLVM-инстансов.
	 * Используется для гарантии, что все инструменты найдены и готовы к
	 * использованию.
	 *
	 * @throws InterruptedException если поток был прерван во время ожидания
	 */
	public static void awaitInit() throws InterruptedException {
		llvmInitLatch.await();
	}

	/**
	 * Загружает список путей к папкам с инструментами LLVM из пользовательских
	 * настроек (preferences). Ожидает строку, в которой пути разделены точкой с
	 * запятой. Если значение отсутствует — список путей не изменяется.
	 *
	 * @param pluginId идентификатор плагина
	 */
	public static void loadLLVMPathsFromPreferences(String pluginId) {
		IEclipsePreferences scope = ConfigurationScope.INSTANCE.getNode(pluginId);
		String[] llvmPaths = scope.get(LLVM_PREFERENCES_LIST, "").split(";"); //$NON-NLS-1$ //$NON-NLS-2$
		for (String path : llvmPaths) {
			if (!path.isBlank()) {
				LLVM_SEARCH_LIST.add(Paths.get(path));
			}
		}
	}

	/**
	 * Сохраняет текущий список путей к папкам с инструментами LLVM в
	 * пользовательские настройки (preferences). Пути сериализуются одной строкой с
	 * разделителем ";".
	 *
	 * @param pluginId идентификатор плагина (обычно PLUGIN_ID)
	 * @throws BackingStoreException если произошла ошибка записи в preferences
	 */
	public static void saveLLVMPathsToPreferences(String pluginId) throws BackingStoreException {
		IEclipsePreferences scope = ConfigurationScope.INSTANCE.getNode(pluginId);
		String llvmPaths = LLVM_SEARCH_LIST.stream().map(Path::toAbsolutePath).map(Path::toString)
				.collect(Collectors.joining("")); //$NON-NLS-1$
		scope.put(LLVM_PREFERENCES_LIST, llvmPaths);
		scope.flush();
	}

}
