package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.HexFormat;

import su.softcom.cldt.debug.core.IGdbCommand;

public class PCommand extends GDBCommand implements IGdbCommand {

	long value;
	
	public PCommand(int regNum) {
		super("p%s".formatted(HexFormat.of().toHexDigits(regNum)));
	}

	public long getValue() {
		return this.value;
	}
	
	@Override
	protected void doHandleResult(byte[] result) {
		this.value = Long.reverseBytes(Long.parseUnsignedLong(new String(result), 16));
	}

}
