package su.softcom.cldt.debug.internal.core.lldb;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import su.softcom.cldt.debug.core.IDebugEndpoint;
import su.softcom.cldt.common.preferences.PreferenceConstants;

import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.jobs.Job;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.model.IStreamsProxy;

/**
 * Abstract Job for wrapping LLDB process. Clients must create new pipe with passed pipe name.
 */
public abstract class AbstractLLDBJob extends Job implements IDebugEndpoint {

	static final Pattern LAUNCHED = Pattern.compile("Launched '(.*)' as process (\\d+)");
	static final String PROCESS_NAME = "LLDB Process";
	protected String pipeName;
	protected List<String> programArgs;
	protected int port;
	protected final ILog LOG = Platform.getLog(getClass());
	private URI debugProgram;
	private List<IDebugServerListener> serverListeners = new ArrayList<>();
	
	private ILaunch launch;
	private Process process;
	private IStreamsProxy proxy;
	
	public abstract void createPipe();

	public abstract String getLLDBPath(String llvmPath);

	public abstract int readPort();

	protected AbstractLLDBJob() {
		super(PROCESS_NAME);
	}

	@Override
	public final IStatus run(IProgressMonitor monitor) {
		IEclipsePreferences preferences = InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE);
		createPipe();
		List<String> args = new ArrayList<>();
		args.add(getLLDBPath(preferences.get(PreferenceConstants.CLANG_PATH, "")));
		args.add("g");
		args.add("--named-pipe");
		args.add(pipeName);
		args.add("localhost:0");
		// Временное решение для Windows FIXME 
		String debugStr = System.getProperty("os.name").contains("Win")
				? debugProgram.getPath().substring(1)
				: debugProgram.getPath();
		args.add("--");
		args.add(debugStr);
		args.addAll(programArgs);
		ProcessBuilder builder = new ProcessBuilder(args);
		LOG.info("Starting lldb-server process...");
		try {
			process = builder.start();
		} catch (IOException e) {
			e.printStackTrace();
		}
		port = readPort();
		LOG.info("Started lldb-server at port %d with arguments: %s".formatted(port,
				args.stream().collect(Collectors.joining(" "))));
		for (IDebugServerListener listener: serverListeners) {
			listener.serverStarted("localhost", port);
		}
		BufferedReader outputReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
		BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()));
		try{
			while (process.isAlive()) {
				if (monitor.isCanceled()) {
					process.destroy();
				}
			}
		}catch (Exception e) {
			Platform.getLog(getClass()).error(e.getMessage());
		}
		
		LOG.info("lldb-server exited with exit code %d".formatted(process.exitValue()));
		for (IDebugServerListener listener: serverListeners) {
			listener.serverFinished(process.exitValue());
		}
		return Status.OK_STATUS;
	}

	public final int getPort() {
		return port;
	}

	public final String getHost() {
		return "localhost";
	}

	@Override
	public final boolean isAlive() {
		return process.isAlive();
	}

	public URI getDebugProgram() {
		return debugProgram;
	}

	public void setDebugProgram(URI debugProgram) {
		Assert.isNotNull(debugProgram);
		this.debugProgram = debugProgram;
	}

	public String getPipeName() {
		return pipeName;
	}

	public void setPipeName(String pipeName) {
		Assert.isNotNull(pipeName);
		this.pipeName = pipeName;
	}
	
	public void setLaunch(ILaunch launch) {
		Assert.isNotNull(launch);
		this.launch = launch;
	}

	public List<String> getProgramArgs() {
		return programArgs;
	}

	public void setProgramArgs(List<String> programArgs) {
		Assert.isNotNull(programArgs);
		this.programArgs = programArgs;
	}

	public void addServerListener(IDebugServerListener serverListener) {
		Assert.isNotNull(serverListener);
		this.serverListeners.add(serverListener);
	}
	
	public void removeServerListener(IDebugServerListener serverListener) {
		Assert.isNotNull(serverListener);
		this.serverListeners.remove(serverListener);
	}
}
