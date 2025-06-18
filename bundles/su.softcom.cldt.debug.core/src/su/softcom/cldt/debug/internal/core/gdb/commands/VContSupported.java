package su.softcom.cldt.debug.internal.core.gdb.commands;


public class VContSupported extends GDBCommand {

	public VContSupported() {
		super("vCont?");
	}

	@Override
	public void doHandleResult(byte[] result) {
		//TODO: send it and check for supported
	}

}
