package su.softcom.cldt.internal.core.cmake.tokens;

public class SemicolonToken extends CMakeToken {

	static final String TYPE_NAME = "SEMICOLON"; //$NON-NLS-1$

	public SemicolonToken(int offset, String value) {
		super(offset);
		setValue(";"); //$NON-NLS-1$
	}

	@Override
	public String getTypeName() {
		return TYPE_NAME;
	}

}
