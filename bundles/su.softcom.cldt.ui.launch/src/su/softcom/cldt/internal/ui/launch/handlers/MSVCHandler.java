package su.softcom.cldt.internal.ui.launch.handlers;

import java.util.ResourceBundle;

import org.eclipse.core.resources.IResource;

import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.core.runners.ProblemsEmitter;
import su.softcom.cldt.internal.core.preferences.CInitializer;
import su.softcom.cldt.internal.core.runners.MSVCCompilerRunner;
import su.softcom.cldt.internal.ui.console.ClangConsoleFactory;
import su.softcom.cldt.ui.launch.handlers.CompileHandler;

/** Abstract Microsoft Visual C++ compiling command handler. */
public abstract class MSVCHandler extends CompileHandler {

	protected static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	@Override
	protected void compile(IResource resource) {
		final ProblemsEmitter problems = new ProblemsEmitter();
		problems.deleteMarkers();

		String toolset = getToolsetPath();
		if (toolset == null || toolset.isEmpty()) {
			problems.emitWarning(RESOURCE_BUNDLE.getString("cmds.warnings.toolsetNotFound")); //$NON-NLS-1$
		}

		String msvc = getCompilerPath();
		if (msvc == null || msvc.isEmpty()) {
			problems.emitWarning(RESOURCE_BUNDLE.getString("cmds.warnings.msvcNotFound")); //$NON-NLS-1$
		}

		if (msvc == null || msvc.isEmpty() || toolset == null || toolset.isEmpty()) {
			problems.emitInfo(RESOURCE_BUNDLE.getString("cmds.infos.msvcUseDefault")); //$NON-NLS-1$
			problems.emitInfo(RESOURCE_BUNDLE.getString("cmds.infos.toolsetUseDefault")); //$NON-NLS-1$

			msvc = CInitializer.getPreferenceStore().getString(CInitializer.MSVC_DEFAULT);
			toolset = CInitializer.getPreferenceStore().getString(CInitializer.TOOLSET_DEFAULT);
		}

		final ClangConsoleFactory factory = new ClangConsoleFactory();
		final IRunner compiler = new MSVCCompilerRunner(msvc, toolset);

		compiler.setConsole(factory.getConsole());
		compiler.addResource(resource.getProject());
		compiler.start();
	}

	/**
	 * Get Microsoft Visual C++ compiler path
	 * 
	 * @return MSVC compiler path
	 */
	protected abstract String getCompilerPath();

	/**
	 * Get Microsoft C++ toolset path
	 * 
	 * @return Visual Studio variables BAT file path
	 */
	protected abstract String getToolsetPath();

}
