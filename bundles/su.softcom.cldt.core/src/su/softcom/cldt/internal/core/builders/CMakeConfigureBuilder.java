package su.softcom.cldt.internal.core.builders;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IIncrementalProjectBuilder2;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Status;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeInstance;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.internal.core.cmake.fileapi.CodemodelFile;
import su.softcom.cldt.internal.core.cmake.fileapi.ConfigurationObject;
import su.softcom.cldt.internal.core.cmake.fileapi.ConfigurationObjectDeserializer;
import su.softcom.cldt.internal.core.cmake.fileapi.TargetDeserializer;

/**
 * Builder for generating configuration files.
 */
public class CMakeConfigureBuilder extends CMakeBuilder implements IIncrementalProjectBuilder2 {

	/**
	 * builder ID
	 */
	public static final String ID = "su.softcom.cldt.core.builder.configure"; //$NON-NLS-1$

	private static final String MESSAGE_STRING = "Конфигурация";

	@Override
	protected IProject[] build(int kind, Map<String, String> args, IProgressMonitor monitor) throws CoreException {
		ICMakeProject cmakeProject = getCMakeProject();
		proxy.notifyProcessListeners(project);
		List<String> params = new ArrayList<>();
		params.add(getProject().getLocation().toOSString());
		params.add("-B"); //$NON-NLS-1$
		params.add(getBuildFolder().getLocation().toOSString());
		params.add("-S"); //$NON-NLS-1$
		params.add(getProject().getLocation().toOSString());
		params.add("-G"); //$NON-NLS-1$
		params.add(cmakeProject.getGenerator().getName());
		Map<String, String> buildArguments = cmakeProject.getBuildArguments();
		IFile cmakeCache = cmakeProject.getCache();
		if (cmakeCache.exists()) {
			removeCachedArguments(cmakeCache, buildArguments);
		}
		ICMakeInstance cmake = cmakeProject.getCmakeInstance();
		Path toolchainPath;
		if ((toolchainPath = cmakeProject.getToolchain()) != null) {
			if (cmake.supportsToolchainArgument()) {
				params.add("--toolchain"); //$NON-NLS-1$
				params.add(toolchainPath.toOSString());
			} else {
				buildArguments.put("CMAKE_TOOLCHAIN_FILE", cmakeProject.getToolchain().toOSString()); //$NON-NLS-1$
			}
		}
		// FIXME: когда сделаем выбор версии и самого компилятора. пока только clang
		buildArguments.put("CMAKE_EXPORT_COMPILE_COMMANDS", "ON"); //$NON-NLS-1$ //$NON-NLS-2$
		buildArguments.put("CMAKE_BUILD_TYPE", getProject().getActiveBuildConfig().getName()); //$NON-NLS-1$
		buildArguments.put("CMAKE_C_COMPILER", "clang"); //$NON-NLS-1$ //$NON-NLS-2$
		buildArguments.put("CMAKE_CXX_COMPILER", "clang++"); //$NON-NLS-1$ //$NON-NLS-2$
		buildArguments.put("CMAKE_ASM_COMPILER", "clang"); //$NON-NLS-1$ //$NON-NLS-2$
		handleProjectRefereces(buildArguments);
		buildArguments.entrySet()
				.forEach(argumentEntry -> params.add("-D" + argumentEntry.getKey() + "=" + argumentEntry.getValue())); //$NON-NLS-1$ //$NON-NLS-2$
		IFolder fileApiFolder = createCMakeAPIFile(monitor.slice(1));
		ProcessBuilder builder = new ProcessBuilder(params);
		Map<String, String> env = builder.environment();
		cmakeProject.getDependencies().stream().filter(dependency -> dependency.getPath() != null).forEach(
				dependency -> env.put("%s_DIR".formatted(dependency.getName()), dependency.getPath().toOSString())); //$NON-NLS-1$
		startCMakeProcess(params, monitor, MESSAGE_STRING);
		boolean configProcessStatus = !monitor.isCanceled();
		if (configProcessStatus) {
			readFileApi(fileApiFolder);
		}
		getCMakeProject().setConfigurationStatus(configProcessStatus);
		return new IProject[] { getProject() };

	}

	private void handleProjectRefereces(Map<String, String> buildArguments) {
		try {
			String refListStr = Arrays.stream(project.getProject().getReferencedProjects())
					.filter(prj -> prj.exists() && CMakeCorePlugin.eINSTANCE.getProject(prj) != null)
					.map(prj -> CMakeCorePlugin.eINSTANCE.getProject(prj)).map(cmakePrj -> cmakePrj.getBuildFolder()
							.getFolder(ICMakeProject.DEFAULT_INSTALL_PATH_POSTFIX).getLocation().toOSString())
					.collect(Collectors.joining(";")); //$NON-NLS-1$

			if (!refListStr.isEmpty()) {
				buildArguments.put("CMAKE_PREFIX_PATH", refListStr); //$NON-NLS-1$
			}
		} catch (CoreException e) {
			e.printStackTrace();
		}

	}

	private void readFileApi(IFolder folder) throws CoreException {
		IFolder replyFolder = folder.getFolder("reply"); //$NON-NLS-1$
		if (!replyFolder.exists()) {
			throw new CoreException(Status.CANCEL_STATUS);
		}
		List<IResource> resList = Arrays.stream(replyFolder.members()).filter(e -> e.getName().startsWith("codemodel")) //$NON-NLS-1$
				.toList();
		if (resList.size() != 1) {
			throw new CoreException(Status.CANCEL_STATUS);
		}
		IResource codemodelFile = resList.get(0);
		if (codemodelFile instanceof IFile file) {
			try (InputStream stream = file.getContents()) {
				ObjectMapper mapper = new ObjectMapper();
				SimpleModule module = new SimpleModule();
				module.addDeserializer(ConfigurationObject.class, new ConfigurationObjectDeserializer());
				module.addDeserializer(Target.class, new TargetDeserializer(getCMakeProject(), replyFolder));
				mapper.registerModule(module);
				mapper.readValue(stream.readAllBytes(), CodemodelFile.class);
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
	}

	@Override
	public void clean(Map<String, String> args, IProgressMonitor monitor) throws CoreException {
		getBuildFolder().refreshLocal(IResource.DEPTH_INFINITE, monitor.slice(1));
		getBuildFolder().delete(false, monitor.slice(1));
	}

	private IFolder createCMakeAPIFile(IProgressMonitor monitor) throws CoreException {
		IFolder buildFolder = getBuildFolder();
		createFolderRecursively(buildFolder, monitor);
		IFolder cmakeFolder = createFolder(monitor, getBuildFolder(), ".cmake"); //$NON-NLS-1$
		IFolder apiFolder = createFolder(monitor, cmakeFolder, "api"); //$NON-NLS-1$
		IFolder versionFolder = createFolder(monitor, apiFolder, "v1"); //$NON-NLS-1$
		IFolder queryFolder = createFolder(monitor, versionFolder, "query"); //$NON-NLS-1$
		IFile queryFile = queryFolder.getFile("codemodel-v2"); //$NON-NLS-1$
		if (!queryFile.exists()) {
			queryFile.create(new ByteArrayInputStream("".getBytes()), false, monitor.slice(1)); //$NON-NLS-1$
		}
		return versionFolder;
	}

	private void createFolderRecursively(IFolder folder, IProgressMonitor monitor) throws CoreException {
		if (!folder.exists()) {
			IContainer parent = folder.getParent();
			if (parent instanceof IFolder paretFolder) {
				createFolderRecursively(paretFolder, monitor);
			}
			folder.create(true, true, monitor);
		}
	}

	private IFolder createFolder(IProgressMonitor monitor, IFolder rootFolder, String name) throws CoreException {
		IFolder folder = rootFolder.getFolder(name);
		if (!folder.exists()) {
			folder.create(false, false, monitor.slice(1));
		}
		return folder;
	}

	/**
	 * Removes every argument that is already cached (and the values are equal or
	 * the last path element of cache value equals to user value (this is because
	 * CMake can compute paths itself)) from the {@code buildArguments} map. <br>
	 * This is to save console space (no need to pass {@code -DARG_NAME=ARG_VALUE}
	 * if it's already cached). </br>
	 * 
	 * @param cmakeCache
	 */
	private void removeCachedArguments(IFile cmakeCache, Map<String, String> buildArguments) {
		Map<String, String> cachedArguments = ICMakeProject.parseCMakeCache(cmakeCache);
		Iterator<String> iterator = buildArguments.keySet().iterator();

		while (iterator.hasNext()) {
			String buildArgumentName = iterator.next();
			String cachedArgValue = cachedArguments.get(buildArgumentName);

			if (cachedArgValue == null) {
				continue;
			}

			String cachedArgValueToCompare = cachedArgValue.contains(File.separator)
					? cachedArgValue.substring(cachedArgValue.lastIndexOf(File.separator) + 1)
					: cachedArgValue;

			if (cachedArgValueToCompare.equals(buildArguments.get(buildArgumentName))) {
				iterator.remove();
			}
		}
	}

}
