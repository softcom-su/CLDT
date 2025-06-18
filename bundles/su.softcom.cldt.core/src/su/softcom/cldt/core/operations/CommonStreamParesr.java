package su.softcom.cldt.core.operations;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.Charset;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ICoreRunnable;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;

public abstract class CommonStreamParesr extends OutputStream implements ICoreRunnable {

	private static final String pathPattern = "(?<path>\\S:?[^:]*?)"; //$NON-NLS-1$
	private static final String lineCharacterPattern = ":\\s?(?<line>\\d+)(?:\\s?:\\s?(?<character>\\d+))?"; //$NON-NLS-1$
	private static final String windowsLineCharacterPattern = "\\((?<windowsLine>\\d+)(?:\\W+(?<windowsCharacter>\\d+))?\\)"; //$NON-NLS-1$
	private static final String locationPattern = pathPattern + "(?:" + lineCharacterPattern + "|" //$NON-NLS-1$ //$NON-NLS-2$
			+ windowsLineCharacterPattern + ")?+\\W*?:"; //$NON-NLS-1$

	private static final String codePattern = "(?:\\s(?<code>\\w+))?"; //$NON-NLS-1$
	private static final String colorPattern = "(?:\\x1B\\[(?:\\d+;)*\\d+m)?"; //$NON-NLS-1$
	private static final String messagePattern = "\\s*+" + colorPattern + "(?<message>.*?)"; //$NON-NLS-1$ //$NON-NLS-2$
	private static final String vcxprojPattern = "(?:\\s*\\[(?<vcxproj>[^-].*)\\])?+"; //$NON-NLS-1$
	private static final String progressRegex = "\\[(?<current>\\d+)/(?<full>\\d+)\\] (?<step>\\w+).+$"; //$NON-NLS-1$

	private final InputStream inputStream;
	private final InputStream errorStream;
	private final Charset charset;

	private boolean done;

	public CommonStreamParesr(InputStream inputStream, InputStream errorStream, Charset charset) {
		this.inputStream = inputStream;
		this.errorStream = errorStream;
		this.charset = charset;
		done = false;
	}

	public void processFinishedNotififcation() {
		done = true;
	}

	@Override
	public void run(IProgressMonitor monitor) throws CoreException {
		BufferedReader inputReader = new BufferedReader(new InputStreamReader(inputStream, charset));
		BufferedReader errorReader = new BufferedReader(new InputStreamReader(errorStream, charset));

		try {
			while (!done) {
				while (inputReader.ready()) {
					parse(inputReader.readLine());
				}
			}
		} catch (IOException e) {
			throw new CoreException(
					new Status(IStatus.ERROR, "su.softcom.cldt.core.operations", e.getLocalizedMessage(), e));
		}
	}

	@Override
	public void write(int arg0) throws IOException {
		// TODO Auto-generated method stub

	}

	abstract public void parse(String line);

	public void reportProcessFinished() {

	}
}
