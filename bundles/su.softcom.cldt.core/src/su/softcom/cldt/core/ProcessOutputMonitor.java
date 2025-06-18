package su.softcom.cldt.core;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.runtime.Platform;

/**
 * Сущность для связи CORE и UI. UI регестрируется как Listener. Core добавляет
 * content.
 * 
 */
public class ProcessOutputMonitor implements IInheriterIOStream {

	List<OutputStream> contentListeners = new ArrayList<>();
	List<OutputStream> errorListeners = new ArrayList<>();
	List<OutputStream> successListeners = new ArrayList<>();
	StringBuilder content = new StringBuilder();
	StringBuilder errorContent = new StringBuilder();
	StringBuilder successContent = new StringBuilder();
	StringBuilder outMessage = new StringBuilder();
	StringBuilder errorMessage = new StringBuilder();
	StringBuilder successMessage = new StringBuilder();

	@Override
	public void addListener(OutputStream listener) {
		if (!contentListeners.contains(listener)) {
			contentListeners.add(listener);
		}
	}

	@Override
	public void addErrorListener(OutputStream listener) {
		if (!errorListeners.contains(listener)) {
			errorListeners.add(listener);
		}
	}

	@Override
	public void addSuccessListener(OutputStream listener) {
		if (!successListeners.contains(listener)) {
			successListeners.add(listener);
		}
	}

	@Override
	public String getContent() {
		return content.toString();
	}

	@Override
	public void addContent(String str) throws IOException {
		outMessage.append(str);
	}

	private void notifyListeners(byte[] bytes) {
		for (OutputStream stream : contentListeners) {
			try {
				stream.write(bytes);
			} catch (IOException e) {
				Platform.getLog(getClass()).warn("Cannot write to console str: %s".formatted(new String(bytes)));
			}
		}
	}

	private void notifyErrorListeners(byte[] bytes) {
		for (OutputStream stream : errorListeners) {
			try {
				stream.write(bytes);
			} catch (IOException e) {
				Platform.getLog(getClass()).warn("Cannot write to console str: %s".formatted(new String(bytes)));
			}
		}
	}

	private void notifySystemListeners(byte[] bytes) {
		for (OutputStream stream : successListeners) {
			try {
				stream.write(bytes);
			} catch (IOException e) {
				Platform.getLog(getClass()).warn("Cannot write to console str: %s".formatted(new String(bytes)));
			}
		}
	}

	public void addContent(char c) {
		content.append(c);
	}

	@Override
	public void clear() {
		content = new StringBuilder();
		errorContent = new StringBuilder();
	}

	@Override
	public void removeListener(OutputStream listener) {
		if (contentListeners.contains(listener)) {
			contentListeners.remove(listener);
		} else {
			Platform.getLog(getClass()).warn("Listener not registered");
		}
	}

	@Override
	public String getErrorContent() {
		return errorContent.toString();
	}

	@Override
	public void addErrorContent(String str) throws IOException {
		errorMessage.append(str);
	}

	@Override
	public void addSuccessContent(String str) throws IOException {
		successMessage.append(str);
	}

	@Override
	public String getSuccsessContent() {
		return successMessage.toString();
	}

	@Override
	public void flushOutStream() {
		notifyListeners(outMessage.toString().getBytes());
		content.append(outMessage);
		outMessage = new StringBuilder();
	}

	@Override
	public void flushErrorStream() {
		notifyErrorListeners(errorMessage.toString().getBytes());
		errorContent.append(errorMessage);
		errorMessage = new StringBuilder();
	}

	@Override
	public void flushSuccessStream() {
		notifySystemListeners(successMessage.toString().getBytes());
		successContent.append(successMessage);
		successMessage = new StringBuilder();
	}

}
