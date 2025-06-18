package su.softcom.cldt.core.operations;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Constructs clang-refactor output
 */
public class ClangRefactorOutputParser extends OutputStream {

	/**
	 * Default constructor
	 */
	public ClangRefactorOutputParser() {
	}

	StringBuilder sb = new StringBuilder();

	@Override
	public void write(int b) throws IOException {
		sb.append((char) b);
	}

}
