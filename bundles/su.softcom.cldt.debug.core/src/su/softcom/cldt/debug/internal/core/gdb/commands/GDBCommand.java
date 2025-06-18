package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.function.Consumer;

import su.softcom.cldt.debug.core.IGdbCommand;

public abstract class GDBCommand implements IGdbCommand {

	protected String command;

	protected GDBCommand(String command) {
		this.command = command;
	}

	@Override
	public String getCommandString() {
		return command;
	}
	
	
	protected abstract void doHandleResult(byte[] result);

	@Override
	public final void handleResult(byte[] result) {
		doHandleResult(decode(result));
	};

	protected byte[] encode(byte[] result) {
		//TODO: encode command
		return result;
	}
	
	
	/**
	 * Декодирует ответ от сервера, убирая байты "}" = 0x7d и заменяя последующий на (byte XOR 0x20).
	 * @param src - source array received from GDB server.
	 * @return decoded byte array.
	 */
	private byte[] decode(byte[] src) {
		int resSize = src.length;
		ByteBuffer resBuffer = ByteBuffer.allocate(resSize);
		boolean lastEscaped = false;
		int escapedNumber = 0;
		for (byte b: src) {
			if (lastEscaped) {
				resBuffer.put((byte) (b ^ 0x20));
				lastEscaped = false;
				escapedNumber++;
			}
			else {
				if(b == 0x7d) {
					// Escape it
					lastEscaped = true;
				} else {
					resBuffer.put(b);
				}
			}
		}
		return Arrays.copyOfRange(resBuffer.array(), 0 , resSize - escapedNumber);
	}
    
	
}
