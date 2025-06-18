package su.softcom.cldt.debug.internal.core;

import java.io.File;
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IncrementalProjectBuilder;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IConfigurationElement;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.core.Launch;
import org.eclipse.debug.core.model.ILaunchConfigurationDelegate2;
import org.eclipse.debug.core.model.RuntimeProcess;
import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.debug.core.DebugConstants;
import su.softcom.cldt.debug.core.IDebugEventBus;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfSourceLocator;
import su.softcom.cldt.debug.internal.core.lldb.AbstractLLDBJob;

public class HostLaunchDelegate implements ILaunchConfigurationDelegate2 {

	private String projectName;
	private String targetName;
	private ICMakeProject cmakeProject;
	private Target target;
	private IFile executableFile = null;
	
	@Override
	public void launch(ILaunchConfiguration configuration, String mode, ILaunch launch, IProgressMonitor monitor)
			throws CoreException {

		String argsStr = configuration.getAttribute(DebugConstants.LAUNCH_ARGUMENTS, "");
		String[] args = Arrays.stream(argsStr.split("\\R"))
			    .filter(line -> !line.isEmpty())
			    .toArray(String[]::new);
		boolean mainBreak = configuration.getAttribute(DebugConstants.MAIN_BREAK, true);
		boolean wrongBuildType = configuration.getAttribute(DebugConstants.WRONG_BUILD_TYPE_FOR_DEBUG, false);

		if (ILaunchManager.DEBUG_MODE.equals(mode)) {
			debugLaunch(launch, args, mainBreak, wrongBuildType);
		} else if (ILaunchManager.RUN_MODE.equals(mode)) {
			ProcessBuilder builder = new ProcessBuilder();
			List<String> runArgs = new ArrayList<>();
			runArgs.add(executableFile.getLocation().toOSString());
			runArgs.addAll(Arrays.asList(args));
			builder.command(runArgs);
			try {
				new RuntimeProcess(launch, builder.start(), argsStr, null);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * Launch debug Job
	 *
	 * @param launch
	 * @param programArgs
	 * @param executableFile
	 *
	 */
	private void debugLaunch(ILaunch launch, String[] args, boolean mainBreak, boolean wrongBuildType) {
		IConfigurationElement[] elements = Platform.getExtensionRegistry()
				.getConfigurationElementsFor("su.softcom.cldt.debug.core.interal.core.lldb");
		//TODO: try to get $TMPDIR first
		String pipeName = "/tmp/lldb-server-%s".formatted(executableFile.getName());
		IConfigurationElement extensionElement = elements[0];
		try {
			Platform.getLog(getClass()).info("Launching all debug jobs...");
			Object obj = extensionElement.createExecutableExtension("class");
			AbstractLLDBJob lldbServer = ((AbstractLLDBJob) obj);
			IDebugEventBus eventBus = new DebugEventBus(executableFile.getName());
			// TODO: GDBClient debugger = new GDBClient(lldbServer, eventBus, mainBreak, wrongBuildType);
			// use it to disable most of debug functionality
			GDBClient debugger = new GDBClient(lldbServer, eventBus, mainBreak, launch);
			lldbServer.setLaunch(launch);
			// TODO: fix URI path here for windows debug to work
			lldbServer.setDebugProgram(executableFile.getLocationURI());
			lldbServer.setProgramArgs(Arrays.asList(args));
			lldbServer.setPipeName(pipeName);
			lldbServer.addServerListener(debugger);
			lldbServer.setPriority(Job.SHORT);
			lldbServer.schedule();

			eventBus.setDebugger(new DebugTarget(launch, eventBus));
			eventBus.setModel(debugger);
			eventBus.start();
			Platform.getLog(getClass()).info("All Jobs scheduled...");
		} catch (CoreException e1) {
			e1.printStackTrace();
		} catch (UnknownHostException e1) {
			try {
				launch.terminate();
			} catch (DebugException e2) {
				e1.printStackTrace();
				e2.printStackTrace();
			}
		}
	}

	@Override
	public ILaunch getLaunch(ILaunchConfiguration configuration, String mode) throws CoreException {

		this.projectName = configuration.getAttribute(DebugConstants.PROJECT_NAME, "");
		this.targetName = configuration.getAttribute(DebugConstants.TARGET_NAME, "");
		if (projectName.length() == 0 || targetName.length() == 0) {
			throw new CoreException(Status.CANCEL_STATUS);
		}
		return new Launch(configuration, mode, new DwarfSourceLocator());
	}

	@Override
	public boolean preLaunchCheck(ILaunchConfiguration configuration, String mode, IProgressMonitor monitor)
			throws CoreException {
		return true;
	}

	@Override
	public boolean buildForLaunch(ILaunchConfiguration configuration, String mode, IProgressMonitor monitor)
			throws CoreException {
		IProject project = ResourcesPlugin.getWorkspace().getRoot().getProject(projectName);
		this.cmakeProject = CMakeCorePlugin.eINSTANCE.getProject(project);
		this.target = cmakeProject.getTarget(targetName);
		for (IFile ifile : target.getArtifacts()) {
			File file = ifile.getRawLocation().makeAbsolute().toFile();
			if (file.canExecute()) {
				this.executableFile = ifile;
				break;
			}
		}
		if (!this.executableFile.exists()) {
			project.build(IncrementalProjectBuilder.FULL_BUILD, monitor);
		}
		return false;
	}

	@Override
	public boolean finalLaunchCheck(ILaunchConfiguration configuration, String mode, IProgressMonitor monitor)
			throws CoreException {
		return true;
	}

}
