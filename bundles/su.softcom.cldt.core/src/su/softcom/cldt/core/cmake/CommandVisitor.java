package su.softcom.cldt.core.cmake;

import java.util.ArrayList;
import java.util.List;

/**
 * Traverses CMake tree and collects every command node with the provided name.
 */
public class CommandVisitor implements ICMakeVisitor {

	private String commandName;
	private List<CommandNode> commands = new ArrayList<>();

	/**
	 * Constructs a visitor with the command name to search for.
	 * 
	 * @param commandName
	 */
	public CommandVisitor(String commandName) {
		this.commandName = commandName;
	}

	@Override
	public void visitRoot(CMakeRoot root) {
		root.getChildren().forEach(child -> child.accept(this));

	}

	@Override
	public void visitArgument(ArgumentNode argument) {
		// Do nothing
	}

	@Override
	public void visitCommand(CommandNode command) {
		if (command.getName().equals(commandName)) {
			commands.add(command);
		}
	}

	@Override
	public void visitConditionCommand(ConditionCommand conditionCommand) {
		conditionCommand.getChildren().forEach(child -> child.accept(this));
	}

	@Override
	public void visitComment(CommentNode comment) {
		// Do nothing
	}

	/**
	 * @return result of the visitor's traverse.
	 */
	public List<CommandNode> getResult() {
		return commands;
	}

}
