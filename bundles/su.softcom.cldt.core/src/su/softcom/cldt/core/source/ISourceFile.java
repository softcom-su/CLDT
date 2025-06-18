package su.softcom.cldt.core.source;

import java.util.List;

import org.eclipse.core.resources.IFile;

import su.softcom.cldt.core.cmake.Target;

/**
 * Представляет исходный файл или ресурс.
 * <p>
 * Обеспечивает доступ к метаинформации о файлах, участвующих в процессе сборки,
 * включая как пользовательские исходные файлы, так и автоматически
 * сгенерированные артефакты.
 * </p>
 */
public interface ISourceFile extends ISourcePart {

	/**
	 * Возвращает файл ресурса в рабочем пространстве Eclipse.
	 * 
	 * @return объект {@link IFile}, связанный с этим источником CMake. Вернет
	 *         {@code null}, если файл не существует в рабочем пространстве.
	 */
	IFile getFile();

	/**
	 * Возвращает имя исходного файла.
	 * <p>
	 * Пример: для цели {@code add_executable(my_app ...)} вернёт "my_app".
	 * </p>
	 * 
	 * @return уникальное имя источника
	 */
	String getName();

	/**
	 * Определяет, является ли файл автоматически сгенерированным.
	 * 
	 * 
	 * @return {@code true} если файл создан автоматически, {@code false} если это
	 *         пользовательский исходный файл
	 */
	boolean isGenerated();

	/**
	 * Возвращает список всех целей сборки к которым принадлежит этот исходный файл
	 * 
	 * @return список целей сборки
	 */
	List<Target> getTargets();

	/**
	 * @param target
	 */
	void addTarget(Target target);
}
