package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.ArrayList;
import java.util.HexFormat;
import java.util.List;
import java.util.function.Consumer;

import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.Utils;


/**
 * VFile:open: command implementation. 
 * 
 * @author rinat.nizamov@softcom.su
 */
public class VFileOpenCommand extends AbstractVFileCommand{
	
	protected int fileDescriptor;
	
	public VFileOpenCommand(String filename, long flagsToOpenCall, long modeBits) {
		super("open", "%s,%s,%s".formatted(Utils.strToHex(filename),Long.toString(flagsToOpenCall), Long.toString(modeBits)));
	}

	@Override
	protected void doHandleResult(byte[] result) {
		String resultStr = new String(result);
		if (resultStr.charAt(0) == 'F') {
			String fileDesctiptor = resultStr.split("F")[1];
			if ( fileDesctiptor.startsWith("-1")) {
				Platform.getLog(getClass()).warn("Error in F packet, ERROR VALUE : %s".formatted(fileDesctiptor.split(",")[1]));
			}
			else{
				this.setFileDescriptor(HexFormat.fromHexDigits(fileDesctiptor));
			}
		}
	}

	
	public int getFileDescriptor() {
		return fileDescriptor;
	}

	protected void setFileDescriptor(int fileDescriptor) {
		this.fileDescriptor = fileDescriptor;
	}
		
	
}
