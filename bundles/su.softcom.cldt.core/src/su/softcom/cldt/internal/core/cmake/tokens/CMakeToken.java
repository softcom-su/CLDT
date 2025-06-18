package su.softcom.cldt.internal.core.cmake.tokens;

import org.eclipse.core.runtime.Assert;

public abstract class CMakeToken {

	private final int offset;
	private String value;

	CMakeToken(int offset) {
		Assert.isTrue(offset >= 0);
		this.offset = offset;
	}

	protected void setValue(String value) {
		this.value = value;
	}

	public String getValue() {
		return value;
	}

	public int getOffset() {
		return offset;
	}

	public abstract String getTypeName();
}
