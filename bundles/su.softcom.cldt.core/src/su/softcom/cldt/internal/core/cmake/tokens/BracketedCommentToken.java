package su.softcom.cldt.internal.core.cmake.tokens;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.core.runtime.Assert;

public class BracketedCommentToken extends CommentToken {

	static final Pattern pattern = Pattern.compile("#\\[(?<COMMENTFILL>=*)\\[(.*?)\\]\\k<COMMENTFILL>\\]"); //$NON-NLS-1$

	static final String TYPE_NAME = "BRACKETCOMMENT"; //$NON-NLS-1$

	public BracketedCommentToken(int offset, String value) {
		super(offset);
		Assert.isNotNull(value);
		Matcher matcher = pattern.matcher(value);
		if (matcher.find()) {
			setValue(matcher.group(2));
		} else {
			throw new IllegalArgumentException(
					"Argument 'value' must be matched by regex '#\\[(?<BRACKETFILL>=*)\\[(.*?)\\]\\k<BRACKETFILL>\\]'. Value is '%s'"
							.formatted(value));
		}
	}

	@Override
	public String getTypeName() {
		return TYPE_NAME;
	}

}
