package su.softcom.cldt.internal.core.cmake.tokens;

public class RParenToken extends CMakeToken {

	static final String TYPE_NAME = "RPAREN"; //$NON-NLS-1$

	public RParenToken(int offset, String value) {
		this(offset);
	}

	public RParenToken(int offset) {
		super(offset);
		setValue(")"); //$NON-NLS-1$
	}

	@Override
	public String getTypeName() {
		return TYPE_NAME;
	}

}
