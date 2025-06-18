package su.softcom.cldt.internal.ui.console.cmake;

import java.io.IOException;
import java.util.List;

import org.eclipse.core.runtime.Platform;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsoleManager;
import org.eclipse.ui.console.MessageConsole;
import org.eclipse.ui.console.MessageConsoleStream;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.builders.CMakeProcessMonitor.OUTPUT_TYPE;
import su.softcom.cldt.core.builders.IProjectOutputListener;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.internal.core.cmake.CMakeProjectDocument.DocumentLine;
import su.softcom.cldt.ui.CLDTPluginImages;
import su.softcom.cldt.ui.CMakeUIPlugin;

/**
 * Реализция консоли сборки CMake. Создается в момент начала сборки в UI потоке.
 * 
 * @author rinat.nizamov
 */
public class CMakeConsole extends MessageConsole implements IProjectOutputListener {

	static final String DEFAULT_CONSOLE_NAME = "Консоль сборки CMake"; //$NON-NLS-1$

	private ICMakeProject project;
	private Color outColor;
	private Color systemColor;
	private Color errorColor;
	private MessageConsoleStream outStream;
	private MessageConsoleStream systemStream;
	private MessageConsoleStream errorStream;
	private IConsoleManager consoleManager;
	private volatile boolean initialized = false;
	private volatile boolean visible = false;

	/**
	 * 
	 * @param project
	 */
	public CMakeConsole(ICMakeProject project) {
		super(createConsoleName(project), CLDTPluginImages.CMAKE_ICON);
		this.project = project;
		this.consoleManager = ConsolePlugin.getDefault().getConsoleManager();
	}

	private static String createConsoleName(ICMakeProject project) {
		if (project != null) {
			return "Консоль сборки проекта %s".formatted(project.getProject().getName()); //$NON-NLS-1$
		}
		return DEFAULT_CONSOLE_NAME;
	}

	@Override
	protected void init() {
		super.init();
		CMakeUIPlugin.getStandardDisplay().asyncExec(this::initializeStreams);

	}

	/*
	 * Initialize streams of the console. Must be called from the UI thread.
	 */
	private void initializeStreams() {
		if (!Display.isSystemDarkTheme()) {
			setBackground(new Color(new RGB(255, 255, 255))); // set background to white
			outColor = new Color(new RGB(75, 75, 75));// dim black
			systemColor = new Color(new RGB(5, 5, 5)); // black
			errorColor = new Color(new RGB(255, 0, 0)); // bright red
		} else {
			setBackground(new Color(new RGB(50, 50, 50))); // set background to black
			outColor = new Color(new RGB(180, 180, 180));// dim white
			systemColor = new Color(new RGB(255, 255, 255)); // white
			errorColor = new Color(new RGB(255, 0, 0)); // dark red
		}
		if (!initialized) {
			outStream = newMessageStream();
			errorStream = newMessageStream();
			systemStream = newMessageStream();
			outStream.setColor(outColor);
			systemStream.setColor(systemColor);
			errorStream.setColor(errorColor);
			initialized = true;
			showConsole();
			List<DocumentLine> addedLines = CMakeCorePlugin.getCMakeBuildMonitor().addListener(this, project);
			for (DocumentLine docLine : addedLines) {
				appendLine(docLine.getType(), docLine.getLine());
			}
		}
	}

	private void appendLine(OUTPUT_TYPE type, String line) {
		if (!visible) {
			showConsole();
		}
		switch (type) {
		case OUT:
			outStream.print(line);
			break;
		case SYSTEM:
			systemStream.print(line);
			break;
		case ERROR:
			errorStream.print(line);
			break;
		default:
			Platform.getLog(getClass()).error("error while appendLine"); //$NON-NLS-1$
		}
	}

	/**
	 * Отобразить консоль
	 */
	public void showConsole() {
		consoleManager.showConsoleView(this);
		visible = true;
	}

	@Override
	protected void dispose() {
		super.dispose();
		CMakeCorePlugin.getCMakeBuildMonitor().removeListener(this, project);
		try {
			outStream.close();
			errorStream.close();
			systemStream.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (outColor != null) {
			outColor.dispose();
		}
		if (systemColor != null) {
			systemColor.dispose();
		}
		if (errorColor != null) {
			errorColor.dispose();
		}

	}

	@Override
	public void outLineReceived(String line) {
		appendLine(OUTPUT_TYPE.OUT, line);
	}

	@Override
	public void systemLineReceived(String line) {
		appendLine(OUTPUT_TYPE.SYSTEM, line);
	}

	@Override
	public void errorLineReceived(String line) {
		appendLine(OUTPUT_TYPE.ERROR, line);
	}

	@Override
	public ICMakeProject getListenersProject() {
		return project;
	}

}
