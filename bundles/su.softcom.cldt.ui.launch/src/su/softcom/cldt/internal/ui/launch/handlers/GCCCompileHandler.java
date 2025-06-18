package su.softcom.cldt.internal.ui.launch.handlers;

import org.eclipse.core.resources.IResource;

import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.internal.core.runners.GCCCompilerRunner;
import su.softcom.cldt.internal.ui.console.ClangConsoleFactory;
import su.softcom.cldt.ui.launch.handlers.CompileHandler;

/** GCC compiling command handler. */
public class GCCCompileHandler extends CompileHandler {

	@Override
	protected void compile(IResource resource) {
		final ClangConsoleFactory factory = new ClangConsoleFactory();
		final IRunner compiler = new GCCCompilerRunner();

		compiler.setConsole(factory.getConsole());
		compiler.addResource(resource.getProject());
		compiler.start();
	}

}
