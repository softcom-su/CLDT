package su.softcom.cldt.internal.core.cmake.tokens;

public class RawToken extends CMakeToken {

	static final String TYPE_NAME = "RAW"; //$NON-NLS-1$

	public RawToken(int offset, String value) {
		super(offset);
		setValue(value);
	}

	@Override
	public String getTypeName() {
		return TYPE_NAME;
	}
}
