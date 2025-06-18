package su.softcom.cldt.core.cmake;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import org.eclipse.core.runtime.CoreException;

import su.softcom.cldt.internal.core.cmake.tokens.CMakeLexer;
import su.softcom.cldt.internal.core.cmake.tokens.CMakeToken;
import su.softcom.cldt.internal.core.cmake.tokens.CommentToken;
import su.softcom.cldt.internal.core.cmake.tokens.LParenToken;
import su.softcom.cldt.internal.core.cmake.tokens.RParenToken;
import su.softcom.cldt.internal.core.cmake.tokens.RawToken;
import su.softcom.cldt.internal.core.cmake.tokens.SemicolonToken;

public class CMakeParser {

	public class UnexpectedTokenException extends Exception {
		final CMakeToken token;

		public UnexpectedTokenException(CMakeToken token) {
			this.token = token;
		}
	}

	static final Map<String, Class<? extends CMakeNode>> COMMAND_MAP = Map.of("if", ConditionCommand.class, "elseif",
			ConditionCommand.class);

	final Queue<CMakeToken> tokens;
	final boolean skipComments;

	public CMakeParser(String content, boolean skipComments) throws IOException {
		CMakeLexer tokenParser = new CMakeLexer(new ByteArrayInputStream(content.getBytes()));
		this.skipComments = skipComments;
		try {
			tokens = tokenParser.read();
		} catch (NoSuchMethodException | SecurityException | InstantiationException | IllegalAccessException
				| IllegalArgumentException | InvocationTargetException | IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			throw new IOException();
		}
	}

	public CMakeRoot parse() throws UnexpectedTokenException, IOException, CoreException {
		Queue<CMakeNode> raw = parseRaw();
		CMakeRoot root = new CMakeRoot();
		parseSubTree(raw, root, new ArrayList<>());
		return root;
	}

	private void parseSubTree(Queue<CMakeNode> rawCommands, CMakeNode tree, List<String> until) {
		while (!rawCommands.isEmpty()) {
			CMakeNode node = rawCommands.peek();
			if (node instanceof CommentNode) {
				tree.addChild(rawCommands.poll());
			} else if (node instanceof CommandNode command) {
				if (until.contains(command.getName())) {
					return;
				}
				if (command.getName().equals("if") | command.getName().equals("elseif")
						| command.getName().equals("else")) {
					ConditionCommand cmd = new ConditionCommand((CommandNode) rawCommands.poll(), tree);
					parseSubTree(rawCommands, cmd, List.of("elseif", "else", "endif")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
					tree.addChild(cmd);
					continue;
				} else {
					tree.addChild(rawCommands.poll());
				}

			}
		}
	}

	private Queue<CMakeNode> parseRaw() throws UnexpectedTokenException {
		CMakeToken token = tokens.poll();
		Queue<CMakeNode> result = new ConcurrentLinkedQueue<>();
		while (token != null) {
			if (token instanceof CommentToken commentToken) {
				if (!skipComments) {
					result.add(new CommentNode(commentToken));
				}
				token = tokens.poll();
				continue;
			}
			if (token instanceof RawToken raw) {
				CMakeToken lparen = tokens.poll();
				while (!(lparen instanceof LParenToken)) {
					if (lparen instanceof CommentToken comment) {
						if (!skipComments) {
							result.add(new CommentNode(comment));
						}
						lparen = tokens.poll();
					} else {
						throw new UnexpectedTokenException(lparen);
					}
				}
				List<CMakeToken> args = parseArgs();
				CMakeToken rparen = args.get(args.size() - 1);
				if (!(rparen instanceof RParenToken)) {
					throw new UnexpectedTokenException(lparen);
				}
				args.remove(args.size() - 1);
				result.add(new CommandNode(raw, args));
				token = tokens.poll();
				continue;
			}
			throw new UnexpectedTokenException(token);
		}
		return result;
	}

	private List<CMakeNode> parseBlock(Queue<CMakeNode> raw, CMakeNode root, List<String> until) {
		List<CMakeNode> result = new ArrayList<>();
		while (!raw.isEmpty()) {
			CMakeNode node = raw.peek();
			if (node instanceof CommentNode) {
				root.addChild(raw.poll());
				continue;
			}
			if (node instanceof CommandNode cmd) {
				if (until.contains(cmd.getName())) {
					return result;
				} else {
					result.add(raw.poll());
				}
			}
		}
		throw new IllegalStateException("");
	}

	private List<CMakeToken> parseArgs() throws UnexpectedTokenException {
		List<CMakeToken> result = new ArrayList<>();
		CMakeToken token = tokens.poll();
		while (token != null) {
			if (token instanceof LParenToken) {
				result.addAll(parseArgs());
			}
			if (token instanceof RParenToken) {
				result.add(token);
				return result;
			}
			if (!(token instanceof SemicolonToken)) {
				result.add(token);
			}
			token = tokens.poll();
		}
		throw new UnexpectedTokenException(null);
	}
}
