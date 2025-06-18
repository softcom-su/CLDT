package su.softcom.cldt.core.operations;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.URL;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.internal.core.preferences.CInitializer;

/**
 * Operation for obfuscation several source files.
 */
public class ObfuscationOperation implements ICoreRunnable {
	/** Name of obfuscated file. */
	public static final String OBFUSCATED_FILE_NAME = "all.7.S"; //$NON-NLS-1$
	/** Name of file with linear segments. */
	public static final String LS_FILE_NAME = "all.1.S"; //$NON-NLS-1$
	/** Name of obfuscation build folder. */
	public static final String OBFUSCATION_BUILD_FOLDER = "opt"; //$NON-NLS-1$
	/** Name of obfuscation info folder. */
	public static final String OBFUSCATION_INFO_FOLDER = "optInfo"; //$NON-NLS-1$
	/** Name of obfuscation temporary folder. */
	public static final String OBFUSCATION_TEMPORARY_FOLDER = "tmp"; //$NON-NLS-1$
	/** Name of info files before obfuscation. */
	public static final String RESULT_BEFORE = "result_before"; //$NON-NLS-1$
	/** Name of info files after obfuscation. */
	public static final String RESULT_AFTER = "result_after"; //$NON-NLS-1$
	/** Json for obfuscated file. */
	public static final String OBFUSCATED_JSON_NAME = "build/opt_tmp/all.pm.json"; //$NON-NLS-1$
	/** Log file name. */
	public static final String LOG_FILE_NAME = "log.txt"; //$NON-NLS-1$
	/** Statistics file name. */
	public static final String STAT_FILE_NAME = "stat.txt"; //$NON-NLS-1$

	/**
	 * Name of functions in obfuscated files.
	 */
	private static List<String> functions = new ArrayList<String>();

	/** Path to python. */
	private static final String PYTHON_PATH = Paths.get(System.getProperty("pythonPath"), "python.exe").toString(); //$NON-NLS-1$ //$NON-NLS-2$
	/** Path to clang. */
	private static final String CLANG_PATH = Paths
			.get(CInitializer.getPreferenceStore().getString(CInitializer.CLANG_PATH)).getParent().toString();

	/** Json for not obfuscated file. */
	private static final String NOT_OBFUSCATED_JSON_NAME = "build/opt_tmp/all.p.json"; //$NON-NLS-1$

	/** Array of files for obfuscation. */
	private List<IFile> filesForObfuscation;
	/** Folder for putting obfuscated assembler file. */
	private IFolder tmpFolder;
	/** Folder for putting info about obfuscated and artifacts. */
	private IFolder infoFolder;
	/** Mode for obfuscation. */
	private String mode;

	private final OutputStream stream;

	private final IProject project;

	/**
	 * Constructs a new obfuscation operation.
	 * 
	 * @param ioConsoleInputStream
	 *
	 * @param filesForObfuscation  array of files for obfuscation
	 * @param infoFolder           folder for putting info about obfuscated
	 */
	public ObfuscationOperation(IProject project, OutputStream ioConsoleInputStream) {
		stream = ioConsoleInputStream;
		IEclipsePreferences projectPreferences = new ProjectScope(project).getNode(PreferenceConstants.NODE);
		infoFolder = project.getFolder(projectPreferences.get(PreferenceConstants.OPT_FOLDER, ".opt_info"));
		tmpFolder = infoFolder;
		this.project = project;
		if (!infoFolder.exists()) {
			try {
				infoFolder.create(false, true, new NullProgressMonitor());
			} catch (CoreException e) {
				// Ignore
			}
		}
		String optFiles = projectPreferences.get(PreferenceConstants.FILES_TO_OPTIMIZE, "");
		filesForObfuscation = Arrays.asList(optFiles.split(PreferenceConstants.FIELD_SEPARATOR)).stream()
				.map(project::getFile).toList();
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		try {
			Platform.getLog(getClass()).info("cleaning markers");
			clearMarkers();
			Platform.getLog(getClass()).info("obfuscation start");
			obfuscate();
			Platform.getLog(getClass()).info("obfuscation end, generating report");
			// Setting functions list
			IFile obfuscated_json = tmpFolder.getFile("all.pm.json");
			try (InputStream contents = obfuscated_json.getContents()) {
				ObjectMapper mapper = new ObjectMapper();
				JsonNode json = mapper.readTree(contents);

				Iterable<String> functionsIter = () -> json.get("free").fieldNames(); //$NON-NLS-1$
				List<String> functionsList = new ArrayList<String>();
				for (String function : functionsIter) {
					functionsList.add(function);
				}
				ObfuscationOperation.functions = functionsList;

			} catch (IOException | CoreException e) {
				Platform.getLog(getClass()).error(e.getLocalizedMessage(), e);
			}

			generateImage(tmpFolder.getFile("all.p.json"), RESULT_BEFORE, 1);
			generateImage(tmpFolder.getFile("all.pm.json"), RESULT_AFTER, 2);
		} catch (IOException | InterruptedException e) {
			Platform.getLog(getClass()).error(e.getLocalizedMessage(), e);
		}
	}

	/**
	 * Returns list of name of obfuscated functions.
	 * 
	 * @return list of name of obfuscated functions
	 */
	public static List<String> getObfuscatedFunctions() {
		return functions;
	}

	/**
	 * Obfuscates files.
	 * 
	 * @throws IOException
	 * @throws InterruptedException
	 * @throws CoreException
	 */
	private void obfuscate() throws IOException, InterruptedException, CoreException {
		Platform.getLog(getClass()).info("ObfuscationOperation.obfuscate() started");
		// Getting URIs for scripts
		Bundle bundle = FrameworkUtil.getBundle(getClass());
		Platform.getLog(getClass()).info("translation files");
		List<IFile> translatedFiles = translate(filesForObfuscation);
		IEclipsePreferences projectPreferences = new ProjectScope(project).getNode(PreferenceConstants.NODE);
		String arch = projectPreferences.get(PreferenceConstants.TARGET_ARCH, PreferenceConstants.TARGET_ARCH_HOST);
		if (arch.equals(PreferenceConstants.TARGET_ARCH_HOST)) {
			String optPath = FileLocator
					.toFileURL(bundle.getEntry("resources" + IPath.SEPARATOR + "optimizator" + IPath.SEPARATOR))
					.getPath().substring(1);
			List<String> args = new ArrayList<String>(
					Arrays.asList(System.getProperty("pythonPath", "") + "python.exe", optPath + "main.py")); //$NON-NLS-1$ //$NON-NLS-2$//$NON-NLS-3$
			args.add("-t");
			args.add("x64");
			args.add("-o");
			args.add(project.getFile("opt.S").getLocation().toOSString());
			args.add("-m");
			args.add("-l");
			args.add("8");
			args.add("-dc");
			args.add(translatedFiles.get(0).getLocation().toOSString());
			ProcessBuilder pb = new ProcessBuilder(args);
			Map<String, String> env = pb.environment();
			String pythonPath = optPath.substring(0, optPath.length() - 1); // removes trailing PathSeparator
			if (env.containsKey("PYTHONPATH")) {
				pythonPath += ";" + env.get("PYTHONPATH");
			}
			pb.directory(new File(optPath));
			env.put("PYTHONPATH", pythonPath);
			Platform.getLog(getClass())
					.info("Start intel optimization\nWorking directory: %s\nPYTHONPATH:%s\nCommand^%s".formatted(
							new File(optPath).getAbsolutePath(), pythonPath,
							args.stream().collect(Collectors.joining(" "))));
			Platform.getLog(getClass()).info("");
			Process p = pb.start();
			BufferedReader output = new BufferedReader(new InputStreamReader(p.getInputStream()));
			BufferedReader error = new BufferedReader(new InputStreamReader(p.getErrorStream()));
			while (p.isAlive()) {
				if (output.ready()) {
					stream.write(output.lines().collect(Collectors.joining("\n")).getBytes());
				}
				if (error.ready()) {
					stream.write(error.lines().collect(Collectors.joining("\n")).getBytes());
				}
			}
			if (output.ready()) {
				stream.write(output.lines().collect(Collectors.joining("\n")).getBytes());
			}
			if (error.ready()) {
				stream.write(error.lines().collect(Collectors.joining("\n")).getBytes());
			}
			return;
		} else {
			Platform.getLog(getClass()).info("generation f objects");
			List<IFile> fFiles = generateFObjects(translatedFiles);
			Platform.getLog(getClass()).info("generation l objects");
			List<IFile> lFiles = generateLObjects(fFiles);
			Platform.getLog(getClass()).info("merge l objects");
			IFile mergedLFile = mergeLFiles(lFiles);
			Platform.getLog(getClass()).info("generation p objects");
			IFile pFile = generatePFile(mergedLFile);
			Platform.getLog(getClass()).info("generation ps objects");
			IFile psFile = generatePSFile(pFile);
			Platform.getLog(getClass()).info("insert dead data");
			IFile deadData = insertDeadData(psFile);
			Platform.getLog(getClass()).info("split LU");
			IFile splitLu = splitLU(deadData);
			Platform.getLog(getClass()).info("obfuscation CFG");
			IFile obfCFG = obfCFG(splitLu);
			Platform.getLog(getClass()).info("insert dead code");
			IFile deadCode = insertDeadCode(obfCFG);
			Platform.getLog(getClass()).info("mix LU");
			IFile mixCodeLu = mixCodeLU(deadCode);
			Platform.getLog(getClass()).info("write ASM");
			writeASM(mixCodeLu);
		}

	}

	private void writeASM(IFile mixCodeLu) throws IOException, CoreException {
		IFile result = tmpFolder.getProject().getFile("opt.S"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "p2S.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(mixCodeLu.getLocation().toOSString());
		args.add(result.getLocation().toOSString());
		Platform.getLog(getClass())
				.info("translation %s to %s by command: %s".formatted(mixCodeLu.getLocation().toOSString(),
						result.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		result.getProject().refreshLocal(IResource.DEPTH_ONE, null);
	}

	private IFile mixCodeLU(IFile deadCode) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("all.pm.json"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "p2pm01.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(deadCode.getLocation().toOSString());
		args.add(result.getLocation().toOSString());
		args.add("100");
		args.add("all");
		Platform.getLog(getClass())
				.info("translation %s to %s by command: %s".formatted(deadCode.getLocation().toOSString(),
						result.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private IFile insertDeadCode(IFile obfCFG) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("all.pc.json"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "p2pc.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(obfCFG.getLocation().toOSString());
		args.add(result.getLocation().toOSString());
		args.add("50");
		args.add("all");
		Platform.getLog(getClass())
				.info("translation %s to %s by command: %s".formatted(obfCFG.getLocation().toOSString(),
						result.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private IFile obfCFG(IFile splitLu) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("all.zp.json"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "p2zp32.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(splitLu.getLocation().toOSString());
		args.add(result.getLocation().toOSString());
		Platform.getLog(getClass())
				.info("translation %s to %s by command: %s".formatted(splitLu.getLocation().toOSString(),
						result.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private void execute(List<String> args, IProgressMonitor monitor) throws IOException, CoreException {
		ProcessBuilder builder = new ProcessBuilder(args);
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts");
		String path = builder.environment().get("PYTHONPATH");
		if (path == null) {
			path = FileLocator.toFileURL(scriptURL).getPath().substring(1);
		} else {
			path.concat(";%s".formatted(FileLocator.toFileURL(scriptURL).getPath().substring(1)));
		}
		builder.environment().put("PYTHONPATH", path);
		Process p = builder.start();
		while (p.isAlive()) {
			if (monitor.isCanceled()) {
				p.destroy();
			}
		}
		tmpFolder.refreshLocal(IResource.DEPTH_INFINITE, null);
	}

	private IFile splitLU(IFile deadData) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("all.pdd.json"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "p2pd.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(deadData.getLocation().toOSString());
		args.add(result.getLocation().toOSString());
		args.add("5");
		Platform.getLog(getClass())
				.info("translation %s to %s by command: %s".formatted(deadData.getLocation().toOSString(),
						result.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private IFile insertDeadData(IFile psFile) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("all.pdd.json"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "pdd.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(psFile.getLocation().toOSString());
		args.add(result.getLocation().toOSString());
		args.add("10");
		args.add("10");
		Platform.getLog(getClass())
				.info("translation %s to %s by command: %s".formatted(psFile.getLocation().toOSString(),
						result.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private IFile generatePSFile(IFile pFile) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("all.ps.json"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "p2ps03.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(pFile.getLocation().toOSString());
		args.add(result.getLocation().toOSString());
		Platform.getLog(getClass())
				.info("translation %s to %s by command: %s".formatted(pFile.getLocation().toOSString(),
						result.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private IFile generatePFile(IFile mergedLFile) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("all.p.json"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "l2p.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(mergedLFile.getLocation().toOSString());
		args.add(result.getLocation().toOSString());
		Platform.getLog(getClass())
				.info("translation %s to %s by command: %s".formatted(mergedLFile.getLocation().toOSString(),
						result.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private IFile mergeLFiles(List<IFile> lFiles) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("all.l.json"); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "l2l.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(result.getLocation().toOSString());
		for (IFile f : lFiles) {
			args.add(f.getLocation().toOSString());
		}
		Platform.getLog(getClass()).info("translation %s to %s by command: %s".formatted(lFiles.stream().map(e -> {
			return e.getLocation().toOSString();
		}).collect(Collectors.joining(", ")), result.getLocation().toOSString(),
				args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private List<IFile> generateLObjects(List<IFile> fFiles) throws IOException, CoreException {
		List<IFile> result = new ArrayList<IFile>();
		for (IFile f : fFiles) {
			List<String> args = new ArrayList<String>();
			args.add("python");
			URL scriptURL = FrameworkUtil.getBundle(getClass())
					.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "f2l.py");
			args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
			args.add(f.getLocation().toOSString());
			IFile newFile = tmpFolder.getFile(f.getName() + ".l");
			args.add(newFile.getLocation().toOSString());
			Platform.getLog(getClass())
					.info("translation %s to %s by command: %s".formatted(f.getLocation().toOSString(),
							newFile.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
			execute(args, new NullProgressMonitor());
			result.add(newFile);
		}
		tmpFolder.refreshLocal(IResource.DEPTH_INFINITE, null);
		return result;
	}

	private List<IFile> generateFObjects(List<IFile> translatedFiles) throws IOException, CoreException {
		List<IFile> result = new ArrayList<IFile>();
		for (IFile f : translatedFiles) {
			List<String> args = new ArrayList<String>();
			args.add("python");
			URL scriptURL = FrameworkUtil.getBundle(getClass())
					.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "s2f14.py");
			args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
			args.add(f.getLocation().toOSString());
			IFile newFile = tmpFolder.getFile(f.getName() + ".f");
			args.add(newFile.getLocation().toOSString());
			Platform.getLog(getClass())
					.info("translation %s to %s by command: %s".formatted(f.getLocation().toOSString(),
							newFile.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
			execute(args, new NullProgressMonitor());
			result.add(newFile);
		}
		tmpFolder.refreshLocal(IResource.DEPTH_INFINITE, null);
		return result;
	}

	private List<String> getCompileCommand(IFile file) {
		ObjectMapper mapper = new ObjectMapper();
		String str;
		try {
			str = new String(project.getFolder("build").getFile("compile_commands.json").getContents().readAllBytes());
			List<HashMap<String, String>> res = mapper.readValue(str,
					new TypeReference<List<HashMap<String, String>>>() {
					});
			for (HashMap<String, String> obj : res) {
				if (obj.get("file").equals(file.getLocation().toOSString())) {
					String command = obj.get("command");
					List<String> args = new ArrayList<String>(Arrays.asList(command.split(" ")));
					args.set(args.indexOf("-c"), "-S");
					for (int i = 0; i < args.size(); i++) {
						if (args.get(i).equals("-o")) {
							String outputFile = args.get(i + 1);
							String fileName = Paths.get(outputFile).getFileName().toString();
							outputFile = tmpFolder.getFile(fileName).getLocation().removeFileExtension()
									.addFileExtension("S").toOSString();
							args.set(i + 1, outputFile);
							break;
						}
					}
					args.add("-nostdlib");
					return args;
				}
			}
			HashMap<String, String> map = res.get(0);
			map.containsKey(file.getLocation().toOSString());
		} catch (IOException | CoreException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return new ArrayList<>();
	}

	private List<IFile> translate(List<IFile> files) throws IOException, CoreException {
		List<IFile> result = new ArrayList<IFile>();
		for (IFile f : files) {
			List<String> args = getCompileCommand(f);
			Platform.getLog(getClass())
					.info("translation %s to %s by command: %s".formatted(f.getLocation().toOSString(),
							tmpFolder.getFile(f.getName().concat(".S")).getLocation().toOSString(),
							args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
			Process p = new ProcessBuilder(args).start();
			while (p.isAlive()) {
				// waiting
				;
			}
			if (p.exitValue() == 0) {
				result.add(tmpFolder.getFile(f.getName().concat(".S")));
			}
		}
		tmpFolder.refreshLocal(IResource.DEPTH_INFINITE, null);
		return result;
	}

	/**
	 * Generates image for obfuscated files.
	 * 
	 * @param jsonFile name of json for image
	 * @param resName  name for result json and htm file
	 * @param postfix  number for postfix for scripts
	 * 
	 * @throws IOException
	 * @throws InterruptedException
	 * @throws CoreException
	 */
	private void generateImage(IFile jsonFile, String resName, int postfix)
			throws IOException, InterruptedException, CoreException {
		generateCFG(jsonFile, postfix);
		List<IFile> jsons = convertCFGtoJSON(postfix);
		IFile json = mergeJSON(jsons, resName);
		URL htmURL = FrameworkUtil.getBundle(getClass()).getEntry(
				"resources" + IPath.SEPARATOR + "optimize_templates" + IPath.SEPARATOR + "%s.htm".formatted(resName)); //$NON-NLS-1$ //$NON-NLS-2$
		IFile file = tmpFolder.getFile("%s.htm".formatted(resName));
		if (file.exists()) {
			file.delete(false, null);
		}
		file.create(new FileInputStream(FileLocator.toFileURL(htmURL).getFile()), false, null);
	}

	private IFile mergeJSON(List<IFile> jsons, String outputName) throws IOException, CoreException {
		IFile result = tmpFolder.getFile("%s.json".formatted(outputName)); //$NON-NLS-1$
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "add711.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(result.getLocation().toOSString());
		for (IFile f : jsons) {
			args.add(f.getLocation().toOSString());
		}
		Platform.getLog(getClass()).info("translation %s to %s by command: %s".formatted(jsons.stream().map(e -> {
			return e.getLocation().toOSString();
		}).collect(Collectors.joining(", ")), result.getLocation().toOSString(),
				args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
		execute(args, new NullProgressMonitor());
		return result;
	}

	private List<IFile> convertCFGtoJSON(int postfix) throws CoreException, IOException {
		List<IFile> result = new ArrayList<IFile>();
		for (String fileName : ObfuscationOperation.functions) {
			IFile f = tmpFolder.getFile("%s_%d.cfg".formatted(fileName, postfix));
			List<String> args = new ArrayList<String>();
			args.add("python");
			URL scriptURL = FrameworkUtil.getBundle(getClass())
					.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "json71.py");
			args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
			args.add(f.getLocation().toOSString());
			IFile newFile = tmpFolder.getFile("%s_%d.json".formatted(fileName, postfix));
			args.add(newFile.getLocation().toOSString());
			args.add(Integer.toString(postfix));
			Platform.getLog(getClass())
					.info("translation %s to %s by command: %s".formatted(f.getLocation().toOSString(),
							newFile.getLocation().toOSString(), args.stream().collect(Collectors.joining(" ")))); //$NON-NLS-1$
			execute(args, new NullProgressMonitor());
			result.add(newFile);
		}
		tmpFolder.refreshLocal(IResource.DEPTH_INFINITE, null);
		return result;
	}

	private void generateCFG(IFile jsonFile, int postfix) throws IOException, CoreException {
		List<String> args = new ArrayList<String>();
		args.add("python");
		URL scriptURL = FrameworkUtil.getBundle(getClass())
				.getEntry("resources" + IPath.SEPARATOR + "optimize_scripts" + IPath.SEPARATOR + "p2cfg.py");
		args.add(FileLocator.toFileURL(scriptURL).getPath().substring(1));
		args.add(jsonFile.getLocation().toOSString());
		args.add(tmpFolder.getLocation().toOSString());
		args.add(Integer.toString(postfix));
		Platform.getLog(getClass())
				.info("generation cfg file from %s to directory %s via command %s".formatted(
						jsonFile.getLocation().toOSString(), tmpFolder.getLocation().toOSString(),
						args.stream().collect(Collectors.joining(" ")))); // $NON-NLS-1$
		execute(args, new NullProgressMonitor());
		tmpFolder.refreshLocal(IResource.DEPTH_ONE, null);
	}

	/**
	 * The function of clearing all markers
	 */
	private void clearMarkers() {
		IWorkspace workspace = ResourcesPlugin.getWorkspace();

		try {
			IMarker[] markers = workspace.getRoot().findMarkers(IMarker.MARKER, true, IResource.DEPTH_INFINITE);

			if (markers.length != 0) {
				for (IMarker marker : markers) {
					marker.delete();
				}
			}
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.toString());
		}
	}

	/**
	 * Runs command with required environment
	 * 
	 * @param command command
	 * 
	 * @throws IOException
	 * @throws InterruptedException
	 * @throws CoreException
	 */
	private void runCommand(String command) throws IOException, InterruptedException, CoreException {
		Platform.getLog(getClass()).info("Running command:\n%s".formatted(command));
		// Getting scripts folder location
		Bundle bundle = FrameworkUtil.getBundle(getClass());
		URL scriptsFolder = bundle.getEntry("resources/su/softcom/cldt/core/operations/obfuscation/komdiv/obfuscation"); //$NON-NLS-1$
		String scriptsFolderSystemPath = FileLocator.toFileURL(scriptsFolder).getPath().substring(1).replace("/", "\\"); //$NON-NLS-1$ //$NON-NLS-2$

		// Runs cmd, /C means terminate after this run
		String commands[] = { "cmd", "/C", command }; //$NON-NLS-1$ //$NON-NLS-2$
		ProcessBuilder processBuilder = new ProcessBuilder(commands);
		processBuilder.directory(new File(infoFolder.getLocation().toOSString()));
		processBuilder.inheritIO();

		// Setting environment variables as at set.cmd
		processBuilder.environment().put("PYTHON", PYTHON_PATH); //$NON-NLS-1$
		processBuilder.environment().put("P", scriptsFolderSystemPath); //$NON-NLS-1$
		processBuilder.environment().put("N", "all"); //$NON-NLS-1$ //$NON-NLS-2$

		Platform.getLog(getClass())
				.info("Process builder enviroment variables are as folowes:\nPYTON %s\nP %s\nN all\ndirectory %s"
						.formatted(PYTHON_PATH, scriptsFolderSystemPath, infoFolder.getLocation().toOSString()));

		Process process = processBuilder.start();

		// Printing output and errors
		BufferedReader outputReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
		BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()));

		while (process.isAlive()) {
			if (outputReader.ready()) {
				String outline = outputReader.readLine();
				if (outline != null) {
					Platform.getLog(getClass()).info("Meesage from process\n%s".formatted(outline));
					System.out.println(outline);
				}
			}
			if (errorReader.ready()) {
				String errorLine = errorReader.readLine();
				if (errorLine != null) {
					Platform.getLog(getClass()).error("Error message from process\n%s".formatted(errorLine));
					System.out.println(errorLine);
				}
			}
		}

		// Handles output that appeared since last attempt at it's handling, yet after
		// process ceased to run
		if (outputReader.ready()) {
			String outline = outputReader.readLine();
			if (outline != null) {
				Platform.getLog(getClass()).info("Meesage from process\n%s".formatted(outline));
				System.out.println(outline);
			}
		}
		if (errorReader.ready()) {
			String errorLine = errorReader.readLine();
			if (errorLine != null) {
				Platform.getLog(getClass()).error("Error message from process\n%s".formatted(errorLine));
				System.out.println(errorLine);
			}
		}

		// Printing exit code
		int exitCode = process.waitFor();
		Platform.getLog(getClass()).info("Exited with error code : %s".formatted(exitCode));
		System.out.println("\nExited with error code : " + exitCode);

		try {
			filesForObfuscation.get(0).getProject().refreshLocal(IResource.DEPTH_INFINITE, new NullProgressMonitor());
		} catch (CoreException e) {
			Platform.getLog(getClass())
					.error("CoreException while refreshing project:\n%s".formatted(e.getStackTrace()));
			throw e;
		}
	}

}
