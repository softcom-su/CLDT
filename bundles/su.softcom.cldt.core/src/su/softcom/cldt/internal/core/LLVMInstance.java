package su.softcom.cldt.internal.core;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import su.softcom.cldt.core.Version;
import su.softcom.cldt.core.llvm.ILLVMInstance;

/**
 * Представление экземпляра LLVM, содержащего список инструментов и
 * поддерживаемые архитектуры. Позволяет определять версию, получать инструменты
 * по имени и проверять их наличие.
 */
public class LLVMInstance implements ILLVMInstance {

	private static final Pattern VERSION = Pattern.compile("version (\\d+\\.\\d+(?:\\.\\d+)?)", //$NON-NLS-1$
			Pattern.CASE_INSENSITIVE);

	private static final Pattern TARGET = Pattern.compile("^\\s+(\\S+)\\s+- (.+)$"); //$NON-NLS-1$
	private List<File> toolList = new ArrayList<>();
	private final List<Architecture> targets;
	private final Version versionObj;

	/**
	 * Парсит версию LLVM из строки.
	 *
	 * @param str строка, содержащая информацию о версии LLVM (например, вывод clang
	 *            --version)
	 * @return объект {@link Version}, если версия найдена
	 * @throws IllegalArgumentException если версия не найдена в строке
	 */
	public static Version getVersion(String str) throws IllegalArgumentException {
		Matcher m = VERSION.matcher(str);
		if (m.find()) {
			return new Version(m.group(1));
		}
		throw new IllegalArgumentException("Cannot find version in string \"%s\"".formatted(str));
	}

	/**
	 * Создаёт новый экземпляр LLVMInstance с заданной версией.
	 *
	 * @param version версия LLVM
	 */
	public LLVMInstance(Version version) {
		this.versionObj = version;
		targets = new ArrayList<>();
	}

	/**
	 * Добавляет инструмент (файл) к данному экземпляру LLVM.
	 *
	 * @param tool файл исполняемого инструмента
	 */
	public void addTool(File tool) {
		toolList.add(tool);
	}

	/**
	 * Проверяет, имеется ли инструмент с заданным именем среди добавленных файлов.
	 *
	 * @param name имя инструмента без расширения (например, "clang" или "clang++")
	 * @return {@code true}, если инструмент найден; иначе {@code false}
	 */
	@Override
	public boolean hasTool(String name) {
		return !toolList.stream().map(File::getName).filter(e -> {
			if (e.contains(".")) { //$NON-NLS-1$
				return e.substring(0, e.lastIndexOf('.')).equals(name);
			} else {
				return e.equals(name);
			}
		}).toList().isEmpty();
	}

	/**
	 * Возвращает список поддерживаемых архитектур (по умолчанию пустой).
	 *
	 * @return список архитектур, поддерживаемых данным LLVMInstance
	 */
	@Override
	public List<Architecture> getTargets() {
		return Collections.emptyList(); // TODO refactor it
	}

	/**
	 * Получает файл инструмента по имени, если он был ранее добавлен.
	 *
	 * @param name имя инструмента без расширения (например, "clang")
	 * @return {@link File} инструмента, если найден; иначе {@code null}
	 */
	@Override
	public File getTool(String name) {
		if (hasTool(name)) {
			return toolList.stream().filter(e -> {
				if (e.getName().contains(".")) { //$NON-NLS-1$
					return e.getName().substring(0, e.getName().lastIndexOf('.')).equals(name);
				} else {
					return e.getName().equals(name);
				}
			}).toList().get(0);
		}
		return null;
	}

	/**
	 * Возвращает версию данного экземпляра LLVM.
	 *
	 * @return версия LLVM
	 */
	@Override
	public Version getVersion() {
		return versionObj;
	}
}
