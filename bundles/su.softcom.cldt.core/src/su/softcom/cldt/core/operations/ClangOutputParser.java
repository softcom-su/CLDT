package su.softcom.cldt.core.operations;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.Paths;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;

/**
 * Parser for read clang output and generate markers. May generate analysis and
 * build markers. Parser has state and may not be reused between processes.
 */
public class ClangOutputParser extends OutputStream {

	static final String WIN_PATH_REGEX = "([a-zA-Z]:[\\\\\\/](?:[a-zA-Z0-9-._]+[\\\\\\/]?)*)"; //$NON-NLS-1$
	static final String LINUX_PATH_REGEX = "((?:\\/[\\w-.]+)+)"; //$NON-NLS-1$

	static final Map<String, Integer> SEVERITY_MAP = new HashMap<>();
	static {
		SEVERITY_MAP.put("warning", IMarker.SEVERITY_WARNING); //$NON-NLS-1$
		SEVERITY_MAP.put("info", IMarker.SEVERITY_INFO); //$NON-NLS-1$
		SEVERITY_MAP.put("error", IMarker.SEVERITY_ERROR); //$NON-NLS-1$
	}

	static final Map<String, String> SUPPORTED_MARKERS_ID;
	static {
		Map<String, String> tmp = new HashMap<>();
		tmp.put("core.uninitialized.ArraySubscript", "su.softcom.cldt.core.marker.uninitialized"); //$NON-NLS-1$ //$NON-NLS-2$
		tmp.put("core.uninitialized.Assign", "su.softcom.cldt.core.marker.uninitialized"); //$NON-NLS-1$ //$NON-NLS-2$
		tmp.put("core.uninitialized.Branch", "su.softcom.cldt.core.marker.uninitialized"); //$NON-NLS-1$ //$NON-NLS-2$
		tmp.put("core.uninitialized.CapturedBlockVariable", "su.softcom.cldt.core.marker.uninitialized"); //$NON-NLS-1$ //$NON-NLS-2$
		tmp.put("core.uninitialized.UndefReturn", "su.softcom.cldt.core.marker.uninitialized"); //$NON-NLS-1$ //$NON-NLS-2$
		tmp.put("core.uninitialized.NewArraySize", "su.softcom.cldt.core.marker.uninitialized"); //$NON-NLS-1$ //$NON-NLS-2$
		SUPPORTED_MARKERS_ID = Collections.unmodifiableMap(tmp);
	}

	StringBuilder sb = new StringBuilder();

	Pattern p = Pattern.compile("%s:(\\d+):(\\d+):\\s(\\w+): ([^\\[\\]]*) \\[(\\S+)]" //$NON-NLS-1$
			.formatted(Platform.getOS().equals(Platform.OS_WIN32) ? WIN_PATH_REGEX : LINUX_PATH_REGEX));

	final String defaultMarkerId;

	/**
	 * Create new clang output parser.
	 * 
	 * @param defaultMarker - default marker ID which will be generated for not
	 *                      presented in
	 *                      {@link ClangOutputParser#SUPPORTED_MARKERS_ID} markers
	 */
	public ClangOutputParser(String defaultMarker) {
		if ((defaultMarker == null) || (defaultMarker.length() == 0)) {
			throw new IllegalArgumentException("default marker ID must not be null or empty");
		}
		defaultMarkerId = defaultMarker;
	}

	private void parse(String str) throws CoreException {
		Matcher m = p.matcher(str.strip());
		if (m.matches()) {
			createMarker(m);
		}
	}

	/**
	 * Extract data from {@link Matcher} and generate marker.
	 * 
	 * @param m - matcher for extract data. Must have 6 groups.
	 * @throws CoreException
	 */
	private void createMarker(Matcher m) throws CoreException {
		String strPath = m.group(1);
		IFile[] files = ResourcesPlugin.getWorkspace().getRoot().findFilesForLocationURI(Paths.get(strPath).toUri());
		Map<String, Object> attributes = new HashMap<>();
		attributes.put(IMarker.LOCATION, strPath + ":" + m.group(2) + ":" + m.group(3)); //$NON-NLS-1$ //$NON-NLS-2$
		attributes.put(IMarker.LINE_NUMBER, Integer.valueOf(m.group(2)));
		attributes.put(IMarker.SEVERITY, SEVERITY_MAP.get(m.group(4)));
		attributes.put(IMarker.MESSAGE, MessageProvider.getMessage(m.group(6), m.group(5)));
		String type = SUPPORTED_MARKERS_ID.containsKey(m.group(6)) ? SUPPORTED_MARKERS_ID.get(m.group(6))
				: defaultMarkerId;
		for (IFile file : files) { // FIXME: in case of nested projects will has many IFile per on file on FS
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
	}

	@Override
	public void write(int b) throws IOException {
		if (((char) b) == '\n') {
			try {
				parse(sb.toString());
				sb = new StringBuilder();
			} catch (CoreException e) {
				Platform.getLog(getClass())
						.error("error while creating IMarker from string %s".formatted(sb.toString()));
				sb = new StringBuilder();
			}
		} else {
			sb.append((char) b);
		}
	}

}
