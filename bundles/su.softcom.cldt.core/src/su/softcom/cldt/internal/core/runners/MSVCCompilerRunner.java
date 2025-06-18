package su.softcom.cldt.internal.core.runners;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.osgi.framework.FrameworkUtil;

/** Microsoft Visual C++ compiler runner. */
public class MSVCCompilerRunner extends ClangCompilerRunner {

	private final String COMPILER_PATH;
	private final String TOOLSET_PATH;

	/**
	 * Create Microsoft Visual C++ compiler runner.
	 * 
	 * @param msvc    Microsoft Visual C++ compiler path
	 * @param toolset Microsoft C++ toolset path
	 */
	public MSVCCompilerRunner(final String msvc, final String toolset) {
		super();
		COMPILER_PATH = msvc;
		TOOLSET_PATH = toolset;
	}

	@Override
	protected Process buildProcess() {
		problems.deleteMarkers();

		final String compilerPath = getCompilerPath();
		if (compilerPath == null) {
			return null;
		}

		final String buildFolder = setBuildFolder();
		if (buildFolder == null) {
			return null;
		}

		final List<String> toRun = new ArrayList<String>();
		toRun.add(compilerPath);

		ILog log = Platform.getLog(FrameworkUtil.getBundle(getClass()));
		log.info(toRun.stream().reduce((fst, snd) -> fst + " " + snd).orElse("NON")); //$NON-NLS-1$//$NON-NLS-2$

		final ProcessBuilder processBuilder = new ProcessBuilder(toRun);
		processBuilder.directory(new File(buildFolder));

		try {
			return processBuilder.start();
		} catch (IOException e) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.cannotStartCompiling")); //$NON-NLS-1$
			return null;
		}
	}

	@Override
	protected String getCompilerPath() {
		final Path path = Paths.get(COMPILER_PATH);
		if (!Files.exists(path) || !Files.isExecutable(path)) {
			problems.emitError(MessageFormat.format(RESOURCE_BUNDLE.getString("errors.compilerNotFound"), //$NON-NLS-1$
					"Microsoft Visual C++")); //$NON-NLS-1$
			return null;
		}

		final Path vcvars = Paths.get(TOOLSET_PATH);
		if (!Files.exists(vcvars) || !Files.isExecutable(vcvars)) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.toolsetNotFound")); //$NON-NLS-1$
			return null;
		}

		final String buildFolder = setBuildFolder();
		if (buildFolder == null) {
			return null;
		}

		final Path slnFile = Paths.get(buildFolder);
		File f = new File(slnFile.getParent().toString());
		List<File> lst = new ArrayList<>();
		for (File file : f.listFiles()) {
			if (file.toString().endsWith(".sln")) {
				lst.add(file);
			}
		}

		if (lst.isEmpty()) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.cannotStartCompiling")); //$NON-NLS-1$
			return null;
		} else {
			String ext = lst.toString();
			try {
				FileWriter fw = new FileWriter(buildFolder + File.separator + "msvc.bat", false); //$NON-NLS-1$
				fw.write("@echo off\n\"" + vcvars.toAbsolutePath().toString() + "\""); //$NON-NLS-1$ //$NON-NLS-2$
				fw.write(" && " + "msbuild " + slnFile.getParent() + "\\" + ext
						+ " /t:rebuild /p:Platform=win32 /p:Configuration=Debug");
				fw.close();
			} catch (IOException e) {
				problems.emitError(RESOURCE_BUNDLE.getString("errors.cannotStartCompiling")); //$NON-NLS-1$
				return null;
			}

			return buildFolder + File.separator + "msvc.bat"; //$NON-NLS-1$
		}
	}
//ушла эпоха и народное творчество

	@Override
	protected void streamsRedirector(final Process process) throws IOException {
		final BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream(), "CP866")); //$NON-NLS-1$

		while (process.isAlive() || reader.ready()) {
			if (reader.ready()) {
				final String line = reader.readLine();

				// Just for Windows paths, like "C:\..."
				final int firstColon = line.indexOf(':', 2); // $NON-NLS-1$
				final int lastColon = line.lastIndexOf(':'); // $NON-NLS-1$
				if (firstColon == -1 || firstColon == lastColon || lastColon == line.length() - 1) {
					continue;
				}

				String message = line.substring(line.indexOf(' ', lastColon) + 1); // $NON-NLS-1$
				message = message.substring(0, 1).toUpperCase() + message.substring(1);

				String path = line.substring(0, firstColon);
				final int bracket = path.lastIndexOf('(');
				if (bracket != -1) {
					path = path.substring(0, bracket);
				}

				final int secondColon = line.indexOf(':', firstColon + 1); // $NON-NLS-1$
				final int lineNumber = secondColon != lastColon && Character.isDigit(line.charAt(firstColon + 1))
						? Integer.parseInt(line.substring(firstColon + 1, secondColon))
						: -1;

				if (line.contains("error")) { //$NON-NLS-1$
					problems.emitError(path, lineNumber, message);
				} else if (line.contains("warning")) { //$NON-NLS-1$
					problems.emitWarning(path, lineNumber, message);
				} else if (line.contains("note")) { //$NON-NLS-1$
					problems.emitInfo(path, lineNumber, message);
				}
			}
		}

		reader.close();
	}

}
