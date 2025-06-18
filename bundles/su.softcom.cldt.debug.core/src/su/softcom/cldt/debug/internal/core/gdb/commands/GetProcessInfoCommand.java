package su.softcom.cldt.debug.internal.core.gdb.commands;

/**
 * qProcessInfo: command implementation. Get the first process info for one or
 * more processes on the remote platform.
 * 
 * @author rinat.nizamov@softcom.su
 */
public class GetProcessInfoCommand extends GDBCommand {
	
	private String infoStr;
	
	public GetProcessInfoCommand() {
		super("qProcessInfo");
	}

	@Override
	public void doHandleResult(byte[] result) {
		infoStr = new String(result);
	}

	public String getProcInfo() {
		return infoStr;
	}

}
