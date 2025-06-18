package su.softcom.cldt.internal.ui.launch;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Stream;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationType;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.core.model.ILaunchConfigurationDelegate;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.debug.ui.ILaunchShortcut2;

import su.softcom.cldt.core.runners.ExeRunner;
import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.internal.core.runners.CMakeRunner;
import su.softcom.cldt.internal.ui.console.ClangConsoleFactory;
import su.softcom.cldt.ui.launch.AbstractLaunchDelegate;
import su.softcom.cldt.ui.launch.handlers.BuildHandler;
import su.softcom.cldt.ui.launch.tabs.MainTab;

/** CMake launch delegate. */
public class CMakeLaunchDelegate extends AbstractLaunchDelegate
		implements ILaunchShortcut, ILaunchShortcut2, ILaunchConfigurationDelegate {

	private static final String LAUNCH_CONFIGURATION_TYPE = "su.softcom.cldt.ui.launch.cmake"; //$NON-NLS-1$

	private static final String CMAKE_LISTS = "CMakeLists.txt"; //$NON-NLS-1$

	@Override
	protected ILaunchConfigurationType getLaunchConfigurationType() {
		return DebugPlugin.getDefault().getLaunchManager().getLaunchConfigurationType(LAUNCH_CONFIGURATION_TYPE);
	}

	@Override
	protected void launch(final IResource resource, final ILaunchConfiguration configuration, final String mode,
			final ILaunch launch, final IProgressMonitor monitor) {

		final ClangConsoleFactory factory = new ClangConsoleFactory();
		final IRunner builder = new CMakeRunner();
		builder.setConsole(factory.getConsole());
		builder.addResource(resource.getProject());

		final String[] args = getArguments(configuration);
		if (args != null) {
			builder.addArgs(args);
		}

		final IRunner compiler = new CMakeRunner();
		builder.next(compiler);

		if (ILaunchManager.DEBUG_MODE.equals(mode)) {
			builder.addArg("-DCMAKE_BUILD_TYPE=Debug"); //$NON-NLS-1$
			compiler.addArg("--config Debug"); //$NON-NLS-1$
		} else if (ILaunchManager.RUN_MODE.equals(mode)) {
			builder.addArg("-DCMAKE_BUILD_TYPE=Release"); //$NON-NLS-1$
			compiler.addArg("--config Release"); //$NON-NLS-1$
		}

		if (!(resource instanceof IProject)) {
			try (Stream<String> lines = Files
					.lines(Paths.get(((IContainer) resource).findMember(CMAKE_LISTS).getLocation().toString()))) {
				Pattern setPattern = Pattern
						.compile("(set|SET)[\\s\\t]*\\\\([\\s\\t]*PROJECT_NAME[\\s\\t]*([^\\s\\t]+)[\\s\\t]*\\\\)"); //$NON-NLS-1$
				Pattern projectPattern = Pattern
						.compile("(project|PROJECT)[\\s\\t]*\\\\([\\s\\t]*([^\\s\\t]+)[\\s\\t]*\\\\)"); //$NON-NLS-1$

				String target = lines.flatMap(line -> Stream.of(setPattern.matcher(line), projectPattern.matcher(line)))
						.filter(Matcher::find).map(matcher -> matcher.group(2)).findFirst().orElse(null);

				if (target != null) {
					compiler.addArg("--target " + target); //$NON-NLS-1$
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		if (!BuildHandler.BUILD_MODE.equals(mode)) {
			final IRunner executor = new ExeRunner();
			compiler.next(executor);
		}

		builder.start();
	}

	/**
	 * Get program arguments form launch configuration
	 * 
	 * @param configuration current launch configuration
	 * 
	 * @return array of arguments, or {@code null}
	 */
	protected String[] getArguments(final ILaunchConfiguration configuration) {
		if (configuration == null) {
			return null;
		}

		try {
			return configuration.getAttribute(MainTab.ARGUMENTS, "").split("\\s+"); //$NON-NLS-1$ //$NON-NLS-2$
		} catch (CoreException e) {
			e.printStackTrace();
		}

		return null;
	}

	@Override
	protected IResource getLaunchableResource(final IResource resource) {
		IContainer container = resource instanceof IContainer ? (IContainer) resource : resource.getParent();

		while (container.findMember(CMAKE_LISTS) == null && !(container instanceof IProject)) {
			container = container.getParent();
		}
		return container;
	}
}
