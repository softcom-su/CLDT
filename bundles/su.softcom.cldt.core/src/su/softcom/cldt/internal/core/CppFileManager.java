package su.softcom.cldt.internal.core;

import java.util.List;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.Assert;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.source.ISourceFile;
import su.softcom.cldt.core.source.SourceFile;

/**
 * Менеджер для управления файловыми операциями в контексте CMake-проекта.
 * Обеспечивает синхронизацию CMake метаданных проекта при удалении файлов
 * {@link SourceFile} и проектов {@link CMakeProject}.
 */
public class CppFileManager {

	/**
	 * Обрабатывает удаление исходного файла из проекта. Удаляет все связанные с
	 * файлом метаданные: - Удаляет {@link SourceFile} из списка источников проекта -
	 * Удаляет связи файла со всеми целями сборки (Targets)
	 * 
	 * @param file         удаляемый файл (не может быть {@code null})
	 * @param cmakeProject проект удаляемого файла
	 */
	public void handleDeletedFile(IFile file, ICMakeProject cmakeProject) {
		Assert.isNotNull(file);
		List<ISourceFile> toRemove = cmakeProject.getSources().stream().filter(src -> file.equals(src.getFile()))
				.toList();
		toRemove.forEach(cmakeProject::removeSourceFile);
	}

	/**
	 * Обрабатывает удаление всего проекта
	 * 
	 * @param prj удаляемый проект (не может быть {@code null})
	 */
	public void handleDeletedProject(IProject prj) {
		Assert.isNotNull(prj);
		ICMakeProject cmakePrj = CMakeCorePlugin.getDefault().getProject(prj);
		CMakeCorePlugin.getDefault().deleteCMakeProject(cmakePrj);
	}

}