package su.softcom.cldt.debug.internal.core.gdb.commands;

import org.eclipse.core.runtime.Platform;
import su.softcom.cldt.debug.internal.core.DebugTarget;

public class KillCommand extends GDBCommand {

	private int exitCode;
	
	public KillCommand() {
		super("k");
	}

	@Override
	public void doHandleResult(byte[] result) {
		String resultStr = new String(result);
		// FIXME
		exitCode = Integer.parseInt(resultStr.split("X")[1]);
		Platform.getLog(getClass())
				.info("LLDB process exited with status %s".formatted(resultStr.split("X")[1]));
	}

	public int getExitCode() {
		return exitCode;
	}
	
}
