package su.softcom.cldt.internal.core.cmake.tokens;

public class LParenToken extends CMakeToken {

	static final String TYPE_NAME = "LPAREN"; //$NON-NLS-1$

	public LParenToken(int offset, String value) {
		this(offset);
	}

	public LParenToken(int offset) {
		super(offset);
		setValue("("); //$NON-NLS-1$
	}

	@Override
	public String getTypeName() {
		return TYPE_NAME;
	}

}
