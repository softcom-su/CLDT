package su.softcom.cldt.debug.internal.core;

import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IMemoryBlock;

public class MemoryBlock extends CLDTDebugElement implements IMemoryBlock {

	private ILaunch launch;
	private long startAddress;
	private long length;
	private byte[] content;
	
	public MemoryBlock(DebugTarget target, long startAddress, long length) {
		super(target);
		if (target != null) {			
			this.launch = target.getLaunch();
		}
		this.startAddress = startAddress;
		this.length = length;
	}

	@Override
	public ILaunch getLaunch() {
		return launch;
	}

	@Override
	public <T> T getAdapter(Class<T> adapter) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public long getStartAddress() {
		return startAddress;
	}

	@Override
	public long getLength() {
		return length;
	}

	@Override
	public byte[] getBytes() throws DebugException {
		return this.content;
	}

	@Override
	public boolean supportsValueModification() {
		return false;
	}

	@Override
	public void setValue(long offset, byte[] bytes) throws DebugException {
		this.content = bytes;
		fireContentChangeEvent();
	}


	public void updateContent(int startAddress, byte[] data) {
		int fromOffset;
		int toOffset;
		int length;

		if (startAddress <= getStartAddress()) {
			fromOffset = (int) (getStartAddress() - startAddress);
			toOffset = 0;
		} else {
			fromOffset = 0;
			toOffset = (int) (startAddress - getStartAddress());
		}

		length = (int) Math.min(getLength() - toOffset - fromOffset, data.length);

		System.arraycopy(data, fromOffset, content, toOffset, length);
	}
	
	private void fireContentChangeEvent()
	{
		DebugEvent event = new DebugEvent(this, DebugEvent.CHANGE);
		fireEvent(event);	
	}
	
}
