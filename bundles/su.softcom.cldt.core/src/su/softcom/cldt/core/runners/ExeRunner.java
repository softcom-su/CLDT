package su.softcom.cldt.core.runners;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ResourceBundle;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IResource;
import org.eclipse.ui.console.IOConsole;

/** Executable files runner. */
public class ExeRunner extends ProcessRunner implements IRunner {

	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	private IFile toExecute;

	/**
	 * Create Executable runner.
	 */
	public ExeRunner() {
		super();
	}

	/**
	 * Create Executable runner with specified I/O console.
	 * 
	 * @param console I/O console
	 */
	public ExeRunner(final IOConsole console) {
		super(console);
	}

	@Override
	public void addResource(final IResource resource) {
		if (resource == null || !resource.exists() || !(resource instanceof IFile)) {
			return;
		}

		toExecute = (IFile) resource;
	}

	@Override
	protected Process buildProcess() {
		if (toExecute == null) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.executableNotFound")); //$NON-NLS-1$
			return null;
		}
		problems.setDefault(toExecute.getProject());

		final ProcessBuilder processBuilder = new ProcessBuilder(toExecute.getLocation().toString());

		try {
			return processBuilder.start();
		} catch (IOException e) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.cannotStartExecution")); //$NON-NLS-1$
			return null;
		}
	}

	@Override
	protected void streamsRedirector(final Process process) throws IOException {
		if (console == null) {
			return;
		}

		console.clearConsole();
		final Charset cs = console.getCharset();
		final OutputStream out = console.newOutputStream();

		final BufferedReader reader = new BufferedReader(
				new InputStreamReader(process.getInputStream(), StandardCharsets.UTF_8));

		while (process.isAlive() || reader.ready()) {
			int ch = reader.read();
			while (ch != -1) {
				out.write(cs.encode(Character.toString(ch)).array());
				ch = reader.read();
			}

			out.flush();
		}

		reader.close();
		out.close();
	}

}
