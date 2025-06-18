package su.softcom.cldt.internal.ui.launch.handlers;

import org.eclipse.core.resources.IResource;

import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.internal.core.runners.MakeRunner;
import su.softcom.cldt.internal.core.runners.QMakeRunner;
import su.softcom.cldt.internal.ui.console.ClangConsoleFactory;
import su.softcom.cldt.ui.launch.handlers.CompileHandler;

/** Qt 5 compiling command handler. */
public class QtCompileHandler extends CompileHandler {

	@Override
	protected void compile(IResource resource) {
		final ClangConsoleFactory factory = new ClangConsoleFactory();
		final IRunner compiler = new QMakeRunner();

		compiler.setConsole(factory.getConsole());
		compiler.addResource(resource.getProject());

		compiler.next(new MakeRunner());
		compiler.start();
	}

}
