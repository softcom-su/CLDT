/**
 * 
 */
package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.HexFormat;

import su.softcom.cldt.debug.core.ELF;
import su.softcom.cldt.debug.core.Utils;
import su.softcom.cldt.debug.internal.core.DebugTarget;

/**
 * “x” - Binary memory read
 * Like the m (read) and M (write) packets, this is a partner to the X (write binary data) packet, x.
 * 
 */
public class ExamineMemoryCommand extends GDBCommand {

	
	public ExamineMemoryCommand(long address, long length) {
		super("x%s,%s".formatted(Utils.removeLeadZero(HexFormat.of().toHexDigits(address)),
				Utils.removeLeadZero(HexFormat.of().toHexDigits(length))));
	}

	@Override
	public void doHandleResult(byte[] result) {

	}
	
}
