package su.softcom.cldt.core.exceptions;

import org.eclipse.core.runtime.CoreException;

/**
 * Исключение, выбрасываемое при ошибках, связанных с созданием или настройкой
 * папки сборки проекта.
 *
 * Используется для индикации специфических проблем с ресурсами build-папки
 * внутри проектной инфраструктуры Eclipse.
 */
public class BuildFolderException extends RuntimeException {

	private static final long serialVersionUID = 1L;

	/**
	 * Создаёт исключение с заданным сообщением.
	 *
	 * @param message описание причины ошибки, которая возникла при работе с папкой
	 *                сборки.
	 */
	public BuildFolderException(String message) {
		super(message);
	}

	/**
	 * Создаёт исключение с заданным сообщением и исходной причиной (другим
	 * исключением).
	 *
	 * @param message описание причины ошибки, которая возникла при работе с папкой
	 *                сборки.
	 * @param cause   исходное исключение, вызвавшее эту ошибку (например,
	 *                {@link CoreException})
	 */
	public BuildFolderException(String message, Throwable cause) {
		super(message, cause);
	}

}