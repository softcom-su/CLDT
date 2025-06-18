/**
 * 
 */
package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.io.IOException;
import java.nio.ByteOrder;

import su.softcom.cldt.debug.core.AuxVector;

/**
 * qXfer:auxv:read command implementation. 
 * 
 * @author rinat.nizamov@softcom.su
 */
public class AuxvReadCommand extends AbstractQXferCommand {
	
	private ByteOrder endian;
	private AuxVector auxVector;
	
	public AuxvReadCommand(long offset, long length, ByteOrder endian) {
		super("auxv","","%s,%s".formatted(Long.toString(offset), Long.toString(length)));
		this.endian = endian;
	}
	
	@Override
	public void doHandleResult(byte[] result) {
		try {
			auxVector = new AuxVector(result, endian);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public AuxVector getAuxVector() {
		return auxVector;
	}
}
