package su.softcom.cldt.internal.core.cmake.tokens;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author Petr Babanov (petr.babanov@softcom.su) - initial implementation
 *
 */
public class CMakeLexer {

	private final InputStream symbolStream;

	boolean readArgument = false;

	int line = 1;
	int column = 1;
	int offset = 0;

	final static String regex = "(\\s+)|(\\()|(\\))|(;)|(\\\"(?:\\\\.|[^\\\"])*\\\")|" //$NON-NLS-1$
			+ "(#\\[(?<COMMENTFILL>=*)\\[.*?\\]\\k<COMMENTFILL>\\])|"//$NON-NLS-1$
			+ "(\\[(?<BRACKETFILL>=*)\\[.*?\\]\\k<BRACKETFILL>\\])|" //$NON-NLS-1$
			+ "(#(?!\\[).*?$)|((?:\\\\.|[^\\\\\\(\\)\\\"# \\t\\r\\n;])+)"; //$NON-NLS-1$
	final static Pattern pattern = Pattern.compile(regex, Pattern.MULTILINE | Pattern.DOTALL | Pattern.UNIX_LINES);
	final static Map<Integer, Class<? extends CMakeToken>> groupTokenTypeMap = Map.of(2, LParenToken.class, 3,
			RParenToken.class, 4, SemicolonToken.class, 5, QuotedToken.class, 6, BracketedCommentToken.class, 8,
			BracketedToken.class, 10, LineCommentToken.class, 11, RawToken.class);

	/**
	 * Basic constructor
	 * 
	 * @param stream - stream to parse
	 * @throws IOException - from InputStream.read()
	 */
	public CMakeLexer(InputStream stream) throws IOException {
		symbolStream = stream;
	}

	/**
	 * Read all tokens from symbolStream.
	 * 
	 * @return queue of tokens
	 * @throws IOException
	 * @throws SecurityException
	 * @throws NoSuchMethodException
	 * @throws InvocationTargetException
	 * @throws IllegalArgumentException
	 * @throws IllegalAccessException
	 * @throws InstantiationException
	 */
	public Queue<CMakeToken> read() throws IOException, NoSuchMethodException, SecurityException,
			InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException {
		String str = new String(symbolStream.readAllBytes());
		Matcher matcher = pattern.matcher(str);
		Queue<CMakeToken> result = new LinkedBlockingQueue<CMakeToken>();
		while (matcher.find()) {
			for (int i = 1; i <= matcher.groupCount(); i++) {
				if (matcher.group(i) != null) {
					Class<? extends CMakeToken> clazz = groupTokenTypeMap.get(i);
					if (clazz == null) {
						continue;
					}
					Constructor<? extends CMakeToken> constructor = groupTokenTypeMap.get(i)
							.getConstructor(Integer.TYPE, String.class);
					result.add(constructor.newInstance(matcher.start(), matcher.group(i)));
					break;
				}
			}
		}
		return result;
	}
}