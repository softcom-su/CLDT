package su.softcom.cldt.internal.ui.console.cmake;

import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;

import su.softcom.cldt.core.IProcessListener;
import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * Реализция Listner процесса сборки CMake. Регистрируется с помощью
 * CMakeUIplugin во время активации. <br>
 * Создает новую или открывает существующую консоль в момент начала сборки
 * 
 * @author rinat.nizamov
 */
public class CMakeProcessListener implements IProcessListener {

	@Override
	public void processStarted(ICMakeProject project) {
		openProjectConsole(project);
	}

	private void openProjectConsole(ICMakeProject project) {
		IConsole[] consoles = ConsolePlugin.getDefault().getConsoleManager().getConsoles();
		for (IConsole cnsl : consoles) {
			if (cnsl instanceof CMakeConsole cmakeCnls && cmakeCnls.getListenersProject().equals(project)) {
				cmakeCnls.clearConsole();
				cmakeCnls.showConsole();
				return;
			}
		}
		CMakeConsole console = new CMakeConsole(project);
		ConsolePlugin.getDefault().getConsoleManager().addConsoles(new IConsole[] { console });
	}

}
