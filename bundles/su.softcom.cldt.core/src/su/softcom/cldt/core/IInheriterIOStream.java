package su.softcom.cldt.core;

import java.io.IOException;
import java.io.OutputStream;

public interface IInheriterIOStream {

	void addListener(OutputStream listener);

	void addErrorListener(OutputStream listener);

	void addSuccessListener(OutputStream systemStream);

	String getContent();

	String getErrorContent();

	String getSuccsessContent();

	void clear();

	void removeListener(OutputStream listener);

	void addContent(String str) throws IOException;

	void addErrorContent(String str) throws IOException;

	void addSuccessContent(String str) throws IOException;

	void flushOutStream();

	void flushErrorStream();

	void flushSuccessStream();

}
