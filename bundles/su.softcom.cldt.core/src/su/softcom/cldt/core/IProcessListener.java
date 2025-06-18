package su.softcom.cldt.core;

import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * Описывает слушателя сборки CMake.
 */
public interface IProcessListener {

	/**
	 * Сообщает слушателю, что процесс сборки запущен.
	 * 
	 * @param project проект CMake, который собираем.
	 */
	void processStarted(ICMakeProject project);

}
