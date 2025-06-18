package su.softcom.cldt.core.cmake;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.Map;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IIncrementalProjectBuilder2;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IConfigurationElement;
import org.eclipse.core.runtime.IExtensionRegistry;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.ProjectDependency;
import su.softcom.cldt.core.cmake.CMakeParser.UnexpectedTokenException;

public class CMakeModifier extends IncrementalProjectBuilder implements IIncrementalProjectBuilder2 {

	/**
	 * public ID constant for references to current builder with ID.
	 */
	public static final String ID = "su.softcom.cldt.core.builder.cmakeModifier"; //$NON-NLS-1$

	public static final String CMAKE_MODIFIER_EP = "su.softcom.cldt.core.cmake.modifier"; //$NON-NLS-1$

	static final String CMAKE_LISTS_NAME = "CMakeLists.txt"; //$NON-NLS-1$
	static final String CMAKE_FOLDER_NAME = "cmake"; //$NON-NLS-1$
	static final String CMAKE_CONFIG_NAME = "Config.cmake.in"; //$NON-NLS-1$
	static final String CMAKE_ROOT_TEMPLATE = "cmake_minimum_required (VERSION 3.20.0)\n\nproject (%s C CXX ASM)\n\nstring(COMPARE EQUAL ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} TEST_IS_TOP_LEVEL)\n" //$NON-NLS-1$
			+ "if(TEST_IS_TOP_LEVEL)\n\tinclude(CMakePackageConfigHelpers)\n" //$NON-NLS-1$
			+ "\tconfigure_package_config_file(\"cmake/Config.cmake.in\" \"TestConfig.cmake\" INSTALL_DESTINATION \"${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}\")\n" //$NON-NLS-1$
			+ "endif()\n"; //$NON-NLS-1$

	@Override
	protected IProject[] build(int kind, Map<String, String> args, IProgressMonitor monitor) throws CoreException {
		IFile rootFile = getProject().getFile(CMAKE_LISTS_NAME);
		IFolder cmakeFolder = getProject().getFolder(CMAKE_FOLDER_NAME);
		try {
			String rootContent = readFromFile(rootFile, CMAKE_ROOT_TEMPLATE.formatted(getProject().getName()));
			if (!cmakeFolder.exists()) {
				cmakeFolder.create(false, false, monitor.slice(1));
			}
			IFile configFile = cmakeFolder.getFile(CMAKE_CONFIG_NAME);
			String configContent = readFromFile(configFile, "");
			CMakeParser parser = new CMakeParser(rootContent, false);
			CMakeRoot tree = parser.parse();
			ICMakeProject cmakeProject = CMakeCorePlugin.getDefault().getProject(getProject());
			updateTree(tree, cmakeProject);
			executeExternalModifiers(tree, cmakeProject);
			tree.writeToFile(rootFile, monitor.slice(1));
			if (configFile.exists()) {
				configFile.refreshLocal(IResource.DEPTH_ONE, monitor.slice(1));
				configFile.setContents(new ByteArrayInputStream(configContent.getBytes()), false, true,
						monitor.slice(1));
			} else {
				configFile.create(new ByteArrayInputStream(configContent.getBytes()), false, monitor.slice(1));
			}

		} catch (IOException | UnexpectedTokenException e) {
			throw new CoreException(new Status(IStatus.CANCEL, CMakeCorePlugin.PLUGIN_ID, "", e));
		}
		return new IProject[] { getProject() };
	}

	private void executeExternalModifiers(CMakeRoot tree, ICMakeProject cmakeProject) {
		IExtensionRegistry registry = Platform.getExtensionRegistry();
		IConfigurationElement[] config = registry.getConfigurationElementsFor(CMAKE_MODIFIER_EP);
		try {
			for (IConfigurationElement e : config) {
				final Object o = e.createExecutableExtension("class"); //$NON-NLS-1$
				if (o instanceof ICMakeTreeModifier modifier) {
					modifier.modifyTree(cmakeProject, tree);
				}
			}
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.getLocalizedMessage());
		}
	}

	protected void updateTree(CMakeRoot tree, ICMakeProject project) {
		for (Target tgt : project.getTargets()) {
			if (tgt.isDirty()) {
				ICMakeVisitor visitor = new TargetSynchronizer(tgt);
				tree.accept(visitor);
			}

		}
	}

	@Override
	public void clean(Map<String, String> args, IProgressMonitor monitor) throws CoreException {
		IFile rootFile = getProject().getFile(CMAKE_LISTS_NAME);
		if (rootFile.exists()) {
			rootFile.delete(false, monitor.slice(1));
		}
		IFolder cmakeFolder = getProject().getFolder(CMAKE_FOLDER_NAME);
		if (cmakeFolder.exists()) {
			IFile configFile = getProject().getFile(CMAKE_CONFIG_NAME);
			if (configFile.exists()) {
				configFile.delete(false, monitor.slice(1));
			}
			cmakeFolder.delete(false, monitor.slice(1));
		}
	}

	/**
	 * Modifies CMake tree adding {@code find_package} commands after "project"
	 * command in the CMakeLists.txt file. It uses CMakeProject's dependencies
	 * field.
	 * 
	 * @throws CoreException
	 */
	public void generateFindPackage() throws CoreException {
		IFile rootFile = getProject().getFile(CMAKE_LISTS_NAME);
		ICMakeProject cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(getProject());
		List<ProjectDependency> packages = cmakeProject.getDependencies();

		CMakeRoot tree = cmakeProject.getCMakeTree();
		if (tree == null) {
			throw new CoreException(new Status(IStatus.CANCEL, CMakeCorePlugin.PLUGIN_ID, "")); //$NON-NLS-1$
		}

		CommandVisitor commandVisitor = new CommandVisitor("project"); //$NON-NLS-1$
		tree.accept(commandVisitor);
		List<CommandNode> commands = commandVisitor.getResult();

		int insertIndex = tree.getChildren().indexOf(commands.get(0)) + 1; // get the insert position

		for (var pac : packages) {
			CommandNode findPackageNode = new CommandNode("find_package", tree); //$NON-NLS-1$
			findPackageNode.addArg(pac.getName());
			tree.addChild(insertIndex, findPackageNode);
		}
		tree.writeToFile(rootFile, null);
	}

	private String readFromFile(IFile file, String def) throws IOException, CoreException {
		String result = def;
		if (file.exists()) {
			try (InputStream stream = file.getContents()) {
				result = new String(stream.readAllBytes());
			}
		}
		return result;
	}
}
