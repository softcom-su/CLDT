package su.softcom.cldt.ui;

import java.util.List;
import java.util.Map;

import su.softcom.cldt.core.cmake.Target.Type;

/**
 * Предоставляет строки для отображения типов целей CMake в UI. Связан с
 * {@link Type}.
 */
public class TargetTypeDisplay {

	private static final Map<Type, String> TYPE_UI_MAP = Map.ofEntries(Map.entry(Type.EXECUTABLE, "Исполняемый файл"),
			Map.entry(Type.STATIC_LIBRARY, "Статическая библиотека"),
			Map.entry(Type.OBJECT_LIBRARY, "Объектная библиотека"),
			Map.entry(Type.INTERFACE_LIBRARY, "Интерфейсная библиотека"),
			Map.entry(Type.MODULE_LIBRARY, "Модульная библиотека"),
			Map.entry(Type.SHARED_LIBRARY, "Динамическая библиотека"), Map.entry(Type.UTILITY, "Кастомная цель"));

	/**
	 * Возвращает локализованное имя типа цели сборки.
	 * 
	 * @param type тип цели из enum {@link Type}
	 * @return строка для отображения в UI
	 */
	public static String getDisplayName(Type type) {
		return TYPE_UI_MAP.getOrDefault(type, "Неизвестный тип цели %s".formatted(type));
	}

	/**
	 * Возвращает тип цели сборки по UI имени.
	 * 
	 * @param name строка UI
	 * @return тип по строке или {@code Type.UTILITY}, если тип не найден
	 */
	public static Type getType(String name) {
		return TYPE_UI_MAP.entrySet().stream().filter(entry -> entry.getValue().equals(name)).map(Map.Entry::getKey)
				.findFirst().orElse(Type.UTILITY);
	}

	/**
	 * @return UI строки для всех доступных типов в порядке объявления
	 */
	public static List<String> getSupportedTypes() {
		return TYPE_UI_MAP.values().stream().toList();
	}

	private TargetTypeDisplay() {
		// utility class
	}
}