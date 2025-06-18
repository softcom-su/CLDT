package su.softcom.cldt.debug.internal.core.gdb.commands;

import org.eclipse.core.runtime.Platform;
import su.softcom.cldt.debug.internal.core.DebugTarget;

/**
 * VFile:close: command implementation.
 * 
 * @author rinat.nizamov@softcom.su
 */
public class VFileCloseCommand extends AbstractVFileCommand {

	public VFileCloseCommand(int fileDescriptor) {
		super("close", Integer.toHexString(fileDescriptor));
	}

	@Override
	protected void doHandleResult(byte[] result) {
		String resultStr = new String(result);
		if (resultStr.charAt(0) == 'F') {
			String resF = resultStr.split("F")[1];
			if (resF.charAt(0) == '0') {
			} else {
				Platform.getLog(getClass()).warn("ERROR: file is not closed");
			}
		} else {
			Platform.getLog(getClass()).warn("ERROR: not an F packet recieved");
		}
	}

}