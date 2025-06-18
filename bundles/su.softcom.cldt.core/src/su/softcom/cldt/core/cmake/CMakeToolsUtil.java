package su.softcom.cldt.core.cmake;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.jobs.IJobChangeEvent;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.jobs.JobChangeAdapter;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;

import com.fasterxml.jackson.databind.ObjectMapper;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.core.CMakeCorePlugin;

/**
 * Утилита для обнаружения и работы с установками CMake.
 * <p>
 * Собирает список генераторов, хранит {@link ICMakeInstance}, синхронизирует
 * состояние с Eclipse preferences.
 * </p>
 */
public final class CMakeToolsUtil {

	static final String CMAKE_DEFAULT_BASE_PATH = Platform.getOS().equals(Platform.OS_WIN32) ? "C:/Program Files/CMake" //$NON-NLS-1$
			: "/usr"; //$NON-NLS-1$
	private static ICMakeInstance defaultCmakeInstance;
	private static List<ICMakeGenerator> generators;
	private static final CountDownLatch cmakeInitLatch = new CountDownLatch(1);

	private CMakeToolsUtil() {
	}

	/**
	 * Синхронно (с таймаутом) ждёт окончания инициализации.
	 * 
	 * @throws InterruptedException если поток прервали
	 */
	public static void awaitInit() throws InterruptedException {
		cmakeInitLatch.await();
	}

	/**
	 * Асинхронно инициализирует CMake-инструменты, а по завершении отключает
	 * auto-building и создаёт кэш {@code CMakeProject}.
	 * <p>
	 * Вызовите один раз при старте плагина.
	 * </p>
	 * 
	 * @return Job
	 */
	public static Job getCMakeInitJob() {

		Job job = Job.create("Инициализация инструментов CMake", (ICoreRunnable) monitor -> initCMakeTools());
		job.addJobChangeListener(new JobChangeAdapter() {
			@Override
			public void done(IJobChangeEvent event) {
				cmakeInitLatch.countDown();
			}
		});
		return job;
	}

	/**
	 * Получить экземпляр CMake нужной версии или дефолтный, если
	 * {@code version == null}.
	 * 
	 * @return ICMakeInstance
	 */
	public static ICMakeInstance getCMakeInstance() {
		return defaultCmakeInstance;
	}

	/**
	 * @return generators ICMakeGenerator
	 */
	public List<ICMakeGenerator> getGenerators() {
		return generators;
	}

	/**
	 * Обновить параметры конфигурации используемого инструмента CMake.
	 */
	public static void initCMakeTools() {
		ProcessBuilder pb = new ProcessBuilder(getCMakeExecPath(), "-E", "capabilities"); //$NON-NLS-1$ //$NON-NLS-2$
		try {
			Process p = pb.start();
			StringBuilder resultBuilder = new StringBuilder();
			try (InputStream stream = p.getInputStream();
					BufferedReader reader = new BufferedReader(new InputStreamReader(stream))) {
				String line;
				while ((line = reader.readLine()) != null) {
					resultBuilder.append(line).append('\n');
				}

				String result = resultBuilder.toString();
				ObjectMapper mapper = new ObjectMapper();
				CMakeInfo info = mapper.readValue(result.getBytes(), CMakeInfo.class);
				List<? extends ICMakeGenerator> list = info.getGenerators();
				generators = (list != null) ? new ArrayList<>(list) : new ArrayList<>();
				defaultCmakeInstance = new ICMakeInstance() {
					@Override
					public Path getPath() {
						return new Path(getCMakeExecPath());
					}

					@Override
					public String getName() {
						return "Default";
					}

					@Override
					public CMakeInfo getInfo() {
						return info;
					}

					@Override
					public boolean supportsToolchainArgument() {
						return info.getVersion().compareTo(new CMakeInfo.Version(false, 3, 21, 0, "", "")) >= 0; //$NON-NLS-1$ //$NON-NLS-2$
					}
				};
			}
		} catch (IOException e) {
			CMakeCorePlugin.logError("Ошибка при инициализации CMake (capabilities): " + e.getLocalizedMessage(), e);
		}
	}

	/**
	 * Получить абсолютный путь до исполняемого файла CMake
	 * 
	 * @return - Полное имя исполняемого файла CMake
	 */
	private static String getCMakeExecPath() {
		IEclipsePreferences preferences = InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE);
		String basePath = preferences.get(PreferenceConstants.CMAKE_PATH, CMAKE_DEFAULT_BASE_PATH); // $NON-NLS-1$
		return basePath.concat("/bin/cmake"); //$NON-NLS-1$
	}

}
