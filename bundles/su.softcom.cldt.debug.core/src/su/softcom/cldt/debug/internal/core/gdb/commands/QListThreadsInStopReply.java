package su.softcom.cldt.debug.internal.core.gdb.commands;

import org.eclipse.core.runtime.Platform;

/**
 * Enable the threads: and thread-pcs: data in the question-mark packet (“T
 * packet”) responses when the stub reports that a program has stopped
 * executing.
 * 
 * @author rinat.nizamov@softcom.su
 */
public class QListThreadsInStopReply extends GDBCommand {
	
	public QListThreadsInStopReply() {
		super("QListThreadsInStopReply");
	}

	@Override
	protected void doHandleResult(byte[] result) {
		String resultStr = new String(result);
		if (resultStr.equals("OK")){
			//OK
		}
		else {
			Platform.getLog(getClass()).warn("Error in %s".formatted(this.getCommandString()));
		}
	}

}
