package su.softcom.cldt.core.preferences;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import org.eclipse.core.runtime.preferences.AbstractPreferenceInitializer;

/** Abstract path searching preferences. */
public abstract class AbstractInitializer extends AbstractPreferenceInitializer {

	public static final boolean IS_WINDOWS = System.getProperty("os.name").startsWith("Windows"); //$NON-NLS-1$ //$NON-NLS-2$

	public static final String EXECUTABLE_EXTENSION = IS_WINDOWS ? ".exe" : ""; //$NON-NLS-1$ //$NON-NLS-2$

	public static final String INSTALL_DIRECTORY = "kpac"; //$NON-NLS-1$

	private Set<Path> dirs;

	/**
	 * Initialize basic paths set to search.
	 */
	public AbstractInitializer() {
		final String home = System.getProperty("user.home"); //$NON-NLS-1$
		dirs = new HashSet<Path>();
		addInstallDirectory();

		if (IS_WINDOWS) {
			dirs.add(Paths.get("C:")); //$NON-NLS-1$
			dirs.add(Paths.get("C:", "excellence")); //$NON-NLS-1$ //$NON-NLS-2$

			dirs.add(Paths.get("D:")); //$NON-NLS-1$

			dirs.add(Paths.get("D:", "Desktop")); //$NON-NLS-1$ //$NON-NLS-2$
			dirs.add(Paths.get("D:", "Documents")); //$NON-NLS-1$ //$NON-NLS-2$
			dirs.add(Paths.get("D:", "Downloads")); //$NON-NLS-1$ //$NON-NLS-2$

			dirs.add(Paths.get("D:", "Загрузки")); //$NON-NLS-1$ //$NON-NLS-2$
			dirs.add(Paths.get("D:", "Документы")); //$NON-NLS-1$ //$NON-NLS-2$
			dirs.add(Paths.get("D:", "Рабочий стол")); //$NON-NLS-1$ //$NON-NLS-2$
		} else {
			dirs.add(Paths.get("/", "usr", "bin")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
		}

		dirs.add(Paths.get(home));
		dirs.add(Paths.get(home, "excellence")); //$NON-NLS-1$

		dirs.add(Paths.get(home, "Desktop")); //$NON-NLS-1$
		dirs.add(Paths.get(home, "Documents")); //$NON-NLS-1$
		dirs.add(Paths.get(home, "Downloads")); //$NON-NLS-1$

		dirs.add(Paths.get(home, "Загрузки")); //$NON-NLS-1$
		dirs.add(Paths.get(home, "Документы")); //$NON-NLS-1$
		dirs.add(Paths.get(home, "Рабочий стол")); //$NON-NLS-1$

		dirs.removeIf(path -> !Files.isDirectory(path));

		parseEnvironmentVariable("PATH"); //$NON-NLS-1$
		parseEnvironmentVariable("RPATH"); //$NON-NLS-1$
	}

	/**
	 * Search directory into default paths by predefined patterns. You can use file
	 * path patterns to search folder contains them.
	 * 
	 * @param patterns path segments to search
	 * 
	 * @return required directory path, or empty string
	 */
	protected String searchDirectory(final String... patterns) {
		return searchRegEx(patternsToPaths(patterns)).map(Paths::get).map(Path::toAbsolutePath).filter(Files::exists)
				.map(path -> Files.isDirectory(path) ? path.toString() : path.getParent().toString()).findFirst()
				.orElse(""); //$NON-NLS-1$
	}

	/**
	 * Search file into default paths by predefined patterns.
	 * 
	 * @param patterns path segments to search
	 * 
	 * @return required file path, or empty string
	 */
	protected String searchFile(final String... patterns) {
		return searchRegEx(patternsToPaths(patterns)).map(Paths::get).map(Path::toAbsolutePath)
				.filter(Files::isRegularFile).map(Object::toString).findFirst().orElse(""); //$NON-NLS-1$
	}

	/**
	 * Search paths by regular expression patterns.
	 * 
	 * @param patterns paths with '*'
	 * 
	 * @return paths stream
	 */
	private Stream<String> searchRegEx(final Stream<String> patterns) {
		return patterns.flatMap(pattern -> {
			try {
				final int index = pattern.indexOf('*');
				return index != -1
						&& (index == 0 || pattern.charAt(index - 1) == '\\' || pattern.charAt(index - 1) == '/')
						&& (pattern.charAt(index + 1) == '\\' || pattern.charAt(index + 1) == '/')
								? searchRegEx(Files.list(Paths.get(pattern.substring(0, index)))
										.filter(dir -> Files.isDirectory(dir))
										.map(dir -> dir.toString() + pattern.substring(index + 1)))
								: Stream.of(pattern);
			} catch (IOException e) {
				return null;
			}
		});
	}

	/**
	 * Append path segments to full paths.
	 * 
	 * @param patterns patterns path segments
	 * 
	 * @return full paths stream
	 */
	private Stream<String> patternsToPaths(final String... patterns) {
		return dirs.stream()
				.flatMap(path -> Arrays.stream(patterns).map(pattern -> path.toString() + File.separator + pattern));
	}

	/**
	 * Parse OS Environment Variable
	 * 
	 * @param variable OS Variable
	 */
	private void parseEnvironmentVariable(final String variable) {
		final String value = System.getenv(variable);
		if (value == null) {
			return;
		}

		dirs.addAll(Arrays.stream(value.split(";")).map(path -> Paths.get(path)).filter(path -> Files.isDirectory(path)) //$NON-NLS-1$
				.collect(Collectors.toSet()));
	}

	/**
	 * Add KPAC install directory to search paths.
	 */
	private void addInstallDirectory() {
		try {
			Path path = Paths
					.get(AbstractInitializer.class.getProtectionDomain().getCodeSource().getLocation().toURI());

			while (!INSTALL_DIRECTORY.equals(path.getFileName().toString())) {
				path = path.getParent();
				if (path == null || path.equals(path.getRoot())) {
					return;
				}
			}

			dirs.add(path.getParent());
		} catch (URISyntaxException e) {
			// there are no install directory, ignore
		}
	}

}
