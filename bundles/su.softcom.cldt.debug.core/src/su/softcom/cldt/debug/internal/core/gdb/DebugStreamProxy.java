package su.softcom.cldt.debug.internal.core.gdb;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.Charset;

import org.eclipse.debug.core.IStreamListener;
import org.eclipse.debug.core.model.IFlushableStreamMonitor;
import org.eclipse.debug.core.model.IStreamMonitor;
import org.eclipse.debug.core.model.IStreamsProxy;
import org.eclipse.debug.core.model.IStreamsProxy2;

/**
 * Standard implementation of a streams proxy for IStreamsProxy.
 */
public class DebugStreamProxy implements IStreamsProxy {
	/**
	 * The monitor for the output stream (connected to standard out of the process)
	 */
	private DebugStreamMonitor fOutputMonitor;
	/**
	 * The monitor for the error stream (connected to standard error of the process)
	 */
	private DebugStreamMonitor fErrorMonitor;

	/**
	 * Records the open/closed state of communications with
	 * the underlying streams.  Note: fClosed is initialized to
	 * <code>false</code> by default.
	 */
	private boolean fClosed;

	public DebugStreamProxy() {
		this.fErrorMonitor = new DebugStreamMonitor();
		this.fOutputMonitor = new DebugStreamMonitor();
	}
	
	/**
	 * Causes the proxy to close all
	 * communications between it and the
	 * underlying streams after all remaining data
	 * in the streams is read.
	 */
	public void close() {
		if (!isClosed(true)) {

		}
	}

	/**
	 * Returns whether the proxy is currently closed.  This method
	 * synchronizes access to the <code>fClosed</code> flag.
	 *
	 * @param setClosed If <code>true</code> this method will also set the
	 * <code>fClosed</code> flag to true.  Otherwise, the <code>fClosed</code>
	 * flag is not modified.
	 * @return Returns whether the stream proxy was already closed.
	 */
	private synchronized boolean isClosed(boolean setClosed) {
	    boolean closed = fClosed;
	    if (setClosed) {
	        fClosed = true;
	    }
	    return closed;
	}

	/**
	 * @see IStreamsProxy#getErrorStreamMonitor()
	 */
	@Override
	public IStreamMonitor getErrorStreamMonitor() {
		return fErrorMonitor;
	}

	/**
	 * @see IStreamsProxy#getOutputStreamMonitor()
	 */
	@Override
	public IStreamMonitor getOutputStreamMonitor() {
		return fOutputMonitor;
	}

	/**
	 * @see IStreamsProxy#write(String)
	 */
	@Override
	public void write(String input) throws IOException {
		fOutputMonitor.append(input);
	} 

	public void writeToErrorStream(String input) throws IOException {
		fErrorMonitor.append(input);
	}

}