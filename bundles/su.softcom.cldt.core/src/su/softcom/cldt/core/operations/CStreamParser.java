package su.softcom.cldt.core.operations;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.nio.file.InvalidPathException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.OperationCanceledException;
import org.eclipse.core.runtime.Platform;

/**
 * Parser for C-like compilers output. Produce {@link IMarker} for parsed
 * errors. Supports GCC, Clang, Clang-CL & MSVC.
 */
public class CStreamParser extends OutputStream implements ICoreRunnable {

	private static final String pathPattern = "(?<path>\\S:?[^:]*?)"; //$NON-NLS-1$
	private static final String lineCharacterPattern = ":\\s?(?<line>\\d+)(?:\\s?:\\s?(?<character>\\d+))?"; //$NON-NLS-1$
	private static final String windowsLineCharacterPattern = "\\((?<windowsLine>\\d+)(?:\\W+(?<windowsCharacter>\\d+))?\\)"; //$NON-NLS-1$
	private static final String locationPattern = pathPattern + "(?:" + lineCharacterPattern + "|" //$NON-NLS-1$ //$NON-NLS-2$
			+ windowsLineCharacterPattern + ")?+\\W*?:"; //$NON-NLS-1$

	private static final String codePattern = "(?:\\s(?<code>\\w+))?"; //$NON-NLS-1$
	private static final String colorPattern = "(?:\\x1B\\[(?:\\d+;)*\\d+m)?"; //$NON-NLS-1$
	private static final String messagePattern = "\\s*+" + colorPattern + "(?<message>.*?)"; //$NON-NLS-1$ //$NON-NLS-2$
	private static final String vcxprojPattern = "(?:\\s*\\[(?<vcxproj>[^-].*)\\])?+"; //$NON-NLS-1$
	private static final String progressRegex = "\\[(?<current>\\d+)/(?<full>\\d+)\\] (?<step>\\w+).+$"; //$NON-NLS-1$

	private final IResource root;
	private final String markerType;
	@Deprecated
	private final InputStream stream;
	@Deprecated
	private final Charset cs;
	private final Pattern pattern;
	private final Pattern progressPattern = Pattern.compile(progressRegex);

	private Map<Integer, List<String>> problemKeys = new HashMap<Integer, List<String>>();

	IProgressMonitor subMonitor;
	private boolean monitorСonfigured = false;
	private int currentStep = 0;
	private int stepsRemaining = 0;

	/**
	 * Create parser for C-compiler stream, with default workspace root resource.
	 * 
	 * @param monitor a progress monitor, or {@code null} if progress reporting and
	 *                cancellation are not desired
	 */
	public CStreamParser(IProgressMonitor monitor) {
		this(ResourcesPlugin.getWorkspace().getRoot(), monitor);
	}

	/**
	 * Create parser for C-compiler stream, with default resource.
	 * 
	 * @param root    default resource for {@link IMarker}
	 * @param monitor a progress monitor, or {@code null} if progress reporting and
	 *                cancellation are not desired
	 */
	public CStreamParser(IResource root, IProgressMonitor monitor) {
		this(root, monitor, IMarker.PROBLEM);
	}

	/**
	 * Create parser for C-compiler stream, with default resource.
	 * 
	 * @param root       default resource for {@link IMarker}
	 * @param monitor    a progress monitor, or {@code null} if progress reporting
	 *                   and cancellation are not desired
	 * @param markerType Marker type to generate ({@link IMarker}
	 */
	public CStreamParser(IResource root, IProgressMonitor monitor, String markerType) {
		this.root = root;
		this.markerType = markerType;
		this.stream = null;
		this.cs = null;
		this.pattern = Pattern.compile(locationPattern + getTypePattern() + messagePattern + vcxprojPattern);
		this.subMonitor = monitor;
		fillProblemKeys();
	}

	/**
	 * Create parser for C-compiler stream, with default workspace root resource.
	 * 
	 * @param stream compiler process stream
	 * @param cs     stream encoding
	 */
	@Deprecated
	public CStreamParser(InputStream stream, Charset cs) {
		this(ResourcesPlugin.getWorkspace().getRoot(), stream, cs);
	}

	/**
	 * Create parser for C-compiler stream, with default resource.
	 * 
	 * @param root   default resource for {@link IMarker}
	 * @param stream compiler process stream
	 * @param cs     stream encoding
	 */
	@Deprecated
	public CStreamParser(IResource root, InputStream stream, Charset cs) {
		this.root = root;
		this.markerType = IMarker.PROBLEM;
		this.stream = stream;
		this.cs = cs;
		this.pattern = Pattern.compile(locationPattern + getTypePattern() + messagePattern + vcxprojPattern);
		fillProblemKeys();
	}

	@Override
	@Deprecated
	public void run(IProgressMonitor monitor) throws CoreException {
		BufferedReader reader = new BufferedReader(new InputStreamReader(stream, cs));

		reader.lines().peek(System.out::println).map(pattern::matcher).filter(Matcher::matches).forEach(matcher -> {
			if (monitor != null && monitor.isCanceled()) {
				throw new OperationCanceledException();
			} else {
				match(matcher);
			}
		});
	}

	/**
	 * Parse line of C-compiler stream.
	 * 
	 * @param line C-compiler output line
	 */
	public void parse(String line) {
		increaseProgress(line);

		Matcher matcher = pattern.matcher(line);
		if (matcher.matches()) {
			match(matcher);
		}
	}

	/**
	 * Create {@link IMarker} with given parameters.
	 * 
	 * @param type        severity marker attribute
	 * @param file        file to create marker
	 * @param line        line in file to create marker
	 * @param character   character in line to create marker
	 * @param message     marker message
	 * @param problemType error type code
	 */
	protected void match(int type, IFile file, int line, int character, String message, int problemType) {
		int charStart = 0;
		try {
			InputStream input = file.getContents();

			boolean utf8 = true;
			int skipCounter = 0;
			int positionUTF8 = 0;
			int positionASCII = 0;

			byte[] bytes = input.readNBytes(1);

			int curLine = 1;
			int curCharacterUTF8 = 0;
			int curCharacterASCII = 0;
			while ((curLine < line || (curCharacterASCII < character || (utf8 && curCharacterUTF8 < character)))
					&& bytes.length != 0) {
				byte ch = bytes[0];

				curLine += ch == 0b00001010 ? 1 : 0;

				positionASCII += curCharacterASCII < character ? 1 : 0;
				curCharacterASCII += curLine == line ? 1 : 0;

				int checkRes = checkByte(ch);
				if (checkRes == -1 && skipCounter == 0) {
					positionUTF8++;
					curCharacterUTF8 += curLine == line ? 1 : 0;
				} else if (checkRes == 0 && skipCounter > 0) {
					skipCounter--;
				} else if (checkRes > 0 && skipCounter <= 0) {
					skipCounter = checkByte(ch);
					positionUTF8++;
					curCharacterUTF8 += curLine == line ? 1 : 0;
				} else {
					utf8 = false;
				}

				bytes = input.readNBytes(1);
			}

			charStart = utf8 ? positionUTF8 : positionASCII;

			input.close();
		} catch (CoreException | IOException e) {
			Platform.getLog(getClass()).error(e.toString());
		}

		try {
			for (IMarker marker : file.findMarkers(markerType, true, IResource.DEPTH_INFINITE)) {
				if (marker.getAttribute(IMarker.LINE_NUMBER).equals(line)
						&& marker.getAttribute(IMarker.CHAR_START).equals(charStart)
						&& marker.getAttribute(IMarker.CHAR_END).equals(charStart + 1)
						&& marker.getAttribute(IMarker.MESSAGE).equals(message)
						&& marker.getAttribute(IMarker.SEVERITY).equals(type)
						&& marker.getAttribute("error_type").equals(problemType)) { //$NON-NLS-1$
					return;
				}
			}

			IMarker marker = file.createMarker(markerType);
			marker.setAttribute(IMarker.LINE_NUMBER, line);
			marker.setAttribute(IMarker.CHAR_START, charStart);
			marker.setAttribute(IMarker.CHAR_END, charStart + 1);
			marker.setAttribute(IMarker.MESSAGE, message);
			marker.setAttribute(IMarker.SEVERITY, type);
			marker.setAttribute("error_type", problemType); //$NON-NLS-1$
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.toString());
		}
	}

	/**
	 * Create {@link IMarker} with given parameters.
	 * 
	 * @param type        severity marker attribute
	 * @param file        file to create marker
	 * @param line        line in file to create marker
	 * @param message     marker message
	 * @param problemType error type code
	 */
	protected void match(int type, IFile file, int line, String message, int problemType) {
		try {
			for (IMarker marker : file.findMarkers(markerType, true, IResource.DEPTH_INFINITE)) {
				if (marker.getAttribute(IMarker.LINE_NUMBER).equals(line)
						&& marker.getAttribute(IMarker.MESSAGE).equals(message)
						&& marker.getAttribute(IMarker.SEVERITY).equals(type)
						&& marker.getAttribute("error_type").equals(problemType)) { //$NON-NLS-1$
					return;
				}
			}

			IMarker marker = file.createMarker(markerType);
			marker.setAttribute(IMarker.LINE_NUMBER, line);
			marker.setAttribute(IMarker.MESSAGE, message);
			marker.setAttribute(IMarker.SEVERITY, type);
			marker.setAttribute("error_type", problemType); //$NON-NLS-1$
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.toString());
		}
	}

	/**
	 * Create {@link IMarker} with given parameters.
	 * 
	 * @param type        severity marker attribute
	 * @param resource    resource to create marker
	 * @param message     marker message
	 * @param problemType error type code
	 */
	protected void match(int type, IResource resource, String message, int problemType) {
		try {
			for (IMarker marker : resource.findMarkers(markerType, true, IResource.DEPTH_INFINITE)) {
				if (marker.getAttribute(IMarker.MESSAGE).equals(message)
						&& marker.getAttribute(IMarker.SEVERITY).equals(type)
						&& marker.getAttribute("error_type").equals(problemType)) { //$NON-NLS-1$
					return;
				}
			}

			IMarker marker = resource.createMarker(markerType);
			marker.setAttribute(IMarker.MESSAGE, message);
			marker.setAttribute(IMarker.SEVERITY, type);
			marker.setAttribute("error_type", problemType); //$NON-NLS-1$
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.toString());
		}
	}

	/**
	 * Get a table of severity marker attributes keyed by problem type.
	 * 
	 * @return a table of problem types
	 */
	protected Map<String, Integer> getProblemTypes() {
		return Map.ofEntries(Map.entry("fatal error", IMarker.SEVERITY_ERROR), //$NON-NLS-1$
				Map.entry("error", IMarker.SEVERITY_ERROR), Map.entry("warning", IMarker.SEVERITY_WARNING), //$NON-NLS-1$ //$NON-NLS-2$
				Map.entry("note", IMarker.SEVERITY_INFO), Map.entry("message", IMarker.SEVERITY_INFO)); //$NON-NLS-1$ //$NON-NLS-2$
	}

	private void increaseProgress(String line) {
		Matcher progress = progressPattern.matcher(line);

		if (subMonitor != null && !subMonitor.isCanceled() && progress.matches()) {
			int newCurrentStep = Integer.parseInt(progress.group("current")); //$NON-NLS-1$

			if (!monitorСonfigured) {
				stepsRemaining = Integer.parseInt(progress.group("full")); //$NON-NLS-1$
				monitorСonfigured = true;
			}

			subMonitor.worked(newCurrentStep - currentStep);
			currentStep = newCurrentStep;
		}
	}

	private int checkByte(byte ch) {
		return (ch & 0b10000000) != 0
				? (ch & 0b01000000) != 0 ? (ch & 0b00100000) != 0 ? (ch & 0b00010000) != 0 ? 3 : 2 : 1 : 0
				: -1;
	}

	private String getTypePattern() {
		return "\\s?" + colorPattern + "(?<type>" //$NON-NLS-1$ //$NON-NLS-2$
				+ getProblemTypes().keySet().stream().reduce((fst, snd) -> fst + "|" + snd).get() //$NON-NLS-1$
				+ ")" + codePattern + "\\s?:"; //$NON-NLS-1$ //$NON-NLS-2$
	}

	/**
	 * Get the workspace analog of file system file, which contains an error.
	 * 
	 * @param path path in file system to file, which contains an error
	 * @return workspace analog of file system file
	 */
	private IResource getResource(String path) {
		try {
			IFile[] files = ResourcesPlugin.getWorkspace().getRoot().findFilesForLocationURI(new File(path).toURI());
			return files.length != 0 ? files[0] : root;
		} catch (InvalidPathException e) {
			return root;
		}
	}

	private void match(Matcher matcher) {
		int type = getProblemTypes().get(matcher.group("type")); //$NON-NLS-1$
		IResource resource = getResource(matcher.group("path")); //$NON-NLS-1$
		String message = matcher.group("message"); //$NON-NLS-1$
		int problemType = parseMarkerMessage(message);

		String code = matcher.group("code"); //$NON-NLS-1$
		if (code != null) {
			message = code + ": " + message; //$NON-NLS-1$
		}

		if (!(resource instanceof IFile)) {
			match(type, resource, message, 4);
			return;
		}

		String line = matcher.group("line"); //$NON-NLS-1$
		String character = matcher.group("character"); //$NON-NLS-1$
		if (line == null) {
			line = matcher.group("windowsLine"); //$NON-NLS-1$
			character = matcher.group("windowsCharacter"); //$NON-NLS-1$
		}

		if (line == null) {
			match(type, resource, message, problemType);
		} else if (character == null) {
			match(type, (IFile) resource, Integer.parseInt(line), message, problemType);
		} else {
			match(type, (IFile) resource, Integer.parseInt(line), Integer.parseInt(character), message, problemType);
		}
	}

	private int parseMarkerMessage(String message) {
		double probability = 0.5;
		int result = -1;
		for (Entry<Integer, List<String>> item : problemKeys.entrySet()) {
			double curProbability = 0;
			for (String str : item.getValue()) {
				if (message.contains(str)) {
					curProbability += 1.0;
				}
			}
			curProbability /= item.getValue().size();
			if (curProbability > probability) {
				probability = curProbability;
				result = item.getKey();
			}
		}
		return result;
	}

	private void fillProblemKeys() {
		int problemIllegalArgument = IProblemType.ILLEGAL_ARGUMENTS;
		List<String> keysIllegalArgument = new ArrayList<String>(Arrays.asList("no matching", "function", "call"));
		problemKeys.put(problemIllegalArgument, keysIllegalArgument);

		int problemMainReturnValue = IProblemType.MAIN_RETURN_VALUE;
		List<String> keysMainReturnValue = new ArrayList<String>(Arrays.asList("main", "must", "return", "int"));
		problemKeys.put(problemMainReturnValue, keysMainReturnValue);

		int problemExpectedSemicolon = IProblemType.EXPECTED_SEMICOLON;
		List<String> keysExpectedSemicolon = new ArrayList<String>(Arrays.asList("expected", ";"));
		problemKeys.put(problemExpectedSemicolon, keysExpectedSemicolon);
	}

	@Override
	public void write(int b) throws IOException {
		// TODO Auto-generated method stub

	}

	@Override
	public void write(byte[] b) throws IOException {
		for (String s : new String(b).split("\n")) {
			parse(s);
		}
	}

}
