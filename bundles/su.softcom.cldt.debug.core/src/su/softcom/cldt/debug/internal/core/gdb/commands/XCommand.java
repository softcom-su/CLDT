package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.HexFormat;

import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.IGdbCommand;

public class XCommand extends GDBCommand implements IGdbCommand {

	byte[] value;
	long length;
	
	public XCommand(long address, long length) {
		super("x%s,%s".formatted(HexFormat.of().toHexDigits(address),
				HexFormat.of().toHexDigits(length)));
		this.length = length;
	}

	public byte[] getValue() {
		return this.value;
	}
	
	@Override
	protected void doHandleResult(byte[] result) {
	   this.value = result;
	}

}
