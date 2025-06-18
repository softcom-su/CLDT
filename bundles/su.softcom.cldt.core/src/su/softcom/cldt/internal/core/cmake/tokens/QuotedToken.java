package su.softcom.cldt.internal.core.cmake.tokens;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.core.runtime.Assert;

public class QuotedToken extends CMakeToken {

	static final String TYPE_NAME = "QUOTED"; //$NON-NLS-1$

	static final Pattern pattern = Pattern.compile("\\\"((?:\\\\.|[^\\\\\"])*)\\\""); //$NON-NLS-1$

	public QuotedToken(int offset, String value) {
		super(offset);
		Assert.isNotNull(value);
		Matcher matcher = pattern.matcher(value);
		if (matcher.find()) {
			setValue(matcher.group(1));
		} else {
			throw new IllegalArgumentException(
					"Argument 'value' must be matched by regex '\\\\\\\"((?:\\\\\\\\.|[^\\\\\\\\\\\"])*)\\\\\\\"'. Value is '%s'"
							.formatted(value));
		}
	}

	@Override
	public String getTypeName() {
		return TYPE_NAME;
	}
}
