package su.softcom.cldt.debug.internal.core.gdb;

import org.eclipse.core.runtime.ListenerList;
import org.eclipse.core.runtime.SafeRunner;
import org.eclipse.debug.core.IStreamListener;
import org.eclipse.debug.core.model.IFlushableStreamMonitor;

class DebugStreamMonitor implements IFlushableStreamMonitor  {

	private ListenerList<IStreamListener> listeners = new ListenerList<>();
	private StringBuffer stream = new StringBuffer();
	private boolean buffer;

	@Override
	public String getContents() {
		return stream.toString();
	}

	@Override
	public void addListener(IStreamListener listener) {
		listeners.add(listener);
	}

	@Override
	public void removeListener(IStreamListener listener) {
		listeners.remove(listener);
	}

	public void append(String text) {
		if (buffer && text != null) {
				stream.append(text);
			}
		
		notifyAppend(text);
	}

	public void notifyAppend(String text) {
		if (text == null) {
			return;
		}
		for (IStreamListener listener : listeners) {
			SafeRunner.run(() ->  listener.streamAppended(text, this));
		}
	}

	@Override
	public void flushContents() {
		stream.setLength(0);
		stream.trimToSize();
	}

	@Override
	public void setBuffered(boolean buffer1) {
		this.buffer = buffer1;
	}

	@Override
	public boolean isBuffered() {
		return buffer;
	}
}