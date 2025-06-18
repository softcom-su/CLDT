package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;

import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.model.IDebugElement;
import org.eclipse.debug.core.model.IDebugTarget;

/**
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public abstract class DwarfElement implements IDebugElement {
	
	protected boolean DWARF32 = true;
	
	protected long getLength(ByteBuffer buffer) {
		long result = Integer.toUnsignedLong(buffer.getInt());
		if((result == 0) || (result == 0xffffffff)) {
			buffer.position(buffer.position() - 4);
			result = buffer.getLong();
			DWARF32 = false;
		}
		return result;
	}

	protected static long readLeb128(ByteBuffer buffer){
		long result = 0;
		int shift = 0;
		byte b;
		while(true) {
			b = buffer.get();
			result |= ((b & 0x7f) << shift);
			shift +=7;
			if((b & 0x80) == 0) {
				break;
			}
		}
		if((shift<64) && ((b&0x40) != 0)) {
			result |= (~0 << shift);
		}
		return result;
	}
	
	protected static long readUleb128(ByteBuffer buffer) {
		long result = 0;
		int i = 0;
		byte b;
		while(true) {
			b = buffer.get();
			result |= (b & 0x7f) << (i * 7);
			if((b & 0x80) == 0) {
				break;
			}
			i++;
		}
		return result;
	}
	
	protected static long readSleb128(ByteBuffer buffer) {
		long result = 0;
		int i = 0;
        int signBits = -1;
		byte b;
		while(true) {
			b = buffer.get();
			result |= (b & 0x7f) << (i * 7);
			signBits <<= 7;
			if((b & 0x80) == 0) {
				break;
			}
			i++;
		}
        // Sign extend if appropriate
        if (((signBits >> 1) & result) != 0 ) {
            result |= signBits;
        }
		return result;
	}
	
	public boolean isDWARF32() {
		return DWARF32;
	}
	
	public String getModelIdentifier() {
		return "";
	}
	
	public IDebugTarget getDebugTarget() {
		return null;
	}
	
	public ILaunch getLaunch() {
		return null;
	}
	
	public <T> T getAdapter(Class<T> adapter) {
		return null;
	}
}
