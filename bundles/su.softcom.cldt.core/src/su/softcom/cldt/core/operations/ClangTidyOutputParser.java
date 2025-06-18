package su.softcom.cldt.core.operations;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.MessageConsole;
import org.eclipse.ui.console.MessageConsoleStream;

/**
 * A parser for reading clang-tidy output and generating analysis markers.
 */
public class ClangTidyOutputParser extends OutputStream {
	private static final String WIN_PATH_REGEX = "([a-zA-Z]:[\\\\\\/](?:[a-zA-Z0-9-._]+[\\\\\\/]?)*)"; //$NON-NLS-1$
	private static final String LINUX_PATH_REGEX = "((?:\\/[\\w-.]+)+)"; //$NON-NLS-1$
	private static final String FIX_IT = "FIX-IT"; //$NON-NLS-1$

	private static final Map<String, Integer> SEVERITY_MAP = new HashMap<>();

	static {
		SEVERITY_MAP.put("warning", IMarker.SEVERITY_WARNING); //$NON-NLS-1$
		SEVERITY_MAP.put("error", IMarker.SEVERITY_ERROR); //$NON-NLS-1$
		SEVERITY_MAP.put("note", IMarker.SEVERITY_INFO); //$NON-NLS-1$
	}

	private static final Pattern markerPattern = Pattern.compile("%s:(\\d+):(\\d+):\\s(\\w+): (.*?)(?: \\[(.*?)\\])?%n" //$NON-NLS-1$
			.formatted(Platform.getOS().equals(Platform.OS_WIN32) ? WIN_PATH_REGEX : LINUX_PATH_REGEX));

	private static final Pattern errorPattern = Pattern.compile("^Error:.*"); //$NON-NLS-1$

	private static final String CLANG_TIDY_MARKER_ID = "su.softcom.cldt.core.marker.clangTidyMarker"; //$NON-NLS-1$

	private StringBuilder markersSb = new StringBuilder();
	private StringBuilder outputSb = new StringBuilder();

	private MessageConsole outputConsole;

	/**
	 * @param showOutput show clang-tidy output in the console.
	 */
	public ClangTidyOutputParser(boolean showOutput) {
		outputConsole = showOutput ? findConsole("Clang-tidy") : null; //$NON-NLS-1$
	}

	/**
	 * Tries to parse given line and create a marker or error message.
	 * 
	 * @param str string to be parsed.
	 * @throws CoreException thrown if the marker could not be generated.
	 */
	public void parse(String str) throws CoreException {
		if (str.contains(FIX_IT)) {
			return;
		}

		Matcher m = markerPattern.matcher(str);
		if (m.matches()) {
			createMarker(m);
		}
	}

	private void createMarker(Matcher m) throws CoreException {
		String strPath = m.group(1);

		IFile[] files = ResourcesPlugin.getWorkspace().getRoot().findFilesForLocationURI(Paths.get(strPath).toUri());

		if (files.length == 0) {
			return;
		}

		IFile file = files[0];

		Map<String, Object> attributes = new HashMap<>();
		String checkId = m.group(6);

		attributes.put(IMarker.LOCATION, strPath + ":" + m.group(2) + ":" + m.group(3)); //$NON-NLS-1$ //$NON-NLS-2$
		attributes.put(IMarker.LINE_NUMBER, Integer.valueOf(m.group(2)));
		attributes.put(IMarker.SEVERITY, SEVERITY_MAP.get(m.group(4)));
		attributes.put(IMarker.MESSAGE, MessageProvider.getMessage("", m.group(5))); //$NON-NLS-1$
		if (checkId != null) {
			attributes.put("checkId", checkId.replaceAll("[\\[\\]]", "")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
		}

		String type = CLANG_TIDY_MARKER_ID;
		IMarker[] markers = file.findMarkers(type, false, 0);
		boolean exists = false;
		for (IMarker marker : markers) {
			if (attributes.get(IMarker.LINE_NUMBER).equals(marker.getAttribute(IMarker.LINE_NUMBER))
					&& attributes.get(IMarker.SEVERITY).equals(marker.getAttribute(IMarker.SEVERITY))
					&& attributes.get(IMarker.MESSAGE).equals(marker.getAttribute(IMarker.MESSAGE))
					&& attributes.get(IMarker.LOCATION).equals(marker.getAttribute(IMarker.LOCATION))) {
				exists = true;
				break;
			}
		}

		if (!exists) {
			file.createMarker(type, attributes);
		}

	}

	@Override
	public void write(int b) throws IOException {
		markersSb.append((char) b);
		outputSb.append((char) b);
		if (b == '\n') {
			try {
				this.parse(markersSb.toString());
			} catch (CoreException e) {
				Platform.getLog(getClass())
						.error("Error while creating IMarker from string %s".formatted(markersSb.toString())); //$NON-NLS-1$
			}

			markersSb.setLength(0);
		}
	}

	@Override
	public void write(byte[] b, int off, int len) throws IOException {
		for (int i = off; i < off + len; ++i) {
			char c = (char) b[i];
			markersSb.append(c);
			outputSb.append(c);

			if (c == '\n') {
				try {
					this.parse(markersSb.toString());
				} catch (CoreException e) {
					Platform.getLog(getClass())
							.error("Error while creating IMarker from string %s".formatted(markersSb.toString())); //$NON-NLS-1$
				}

				markersSb.setLength(0);
			}
		}
	}

	/**
	 * @return String containing clang-tidy output.
	 */
	public String getOutput() {
		String output = outputSb.toString();
		outputSb.setLength(0);

		Matcher errorMatcher = errorPattern.matcher(output);
		if (errorMatcher.find()) {
			return errorMatcher.group();
		}

		return output;
	}

	@Override
	public void flush() throws IOException {
		markersSb.setLength(0);

		writeToConsole(outputSb.toString());
		outputSb.setLength(0);
	}

	private MessageConsole findConsole(String name) {
		ConsolePlugin plugin = ConsolePlugin.getDefault();
		MessageConsole console = null;
		IConsole[] consoles = plugin.getConsoleManager().getConsoles();
		for (IConsole c : consoles) {
			if (c.getName().equals(name)) {
				console = (MessageConsole) c;
				console.clearConsole();
				break;
			}
		}
		if (console == null) {
			console = new MessageConsole(name, null);
			plugin.getConsoleManager().addConsoles(new IConsole[] { console });
		}

		return console;
	}

	private void writeToConsole(String message) {
		try {
			if (outputConsole != null) {
				MessageConsoleStream out = outputConsole.newMessageStream();
				out.print(message);
				out.close();
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
