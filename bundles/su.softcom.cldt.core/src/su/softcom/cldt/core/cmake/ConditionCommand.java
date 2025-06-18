package su.softcom.cldt.core.cmake;

public class ConditionCommand extends CommandNode {

	public ConditionCommand(CommandNode rawCommand, CMakeNode parent) {
		super(rawCommand.getName(), parent);
		addAllArg(rawCommand.getArgs());
	}

	@Override
	public void accept(ICMakeVisitor visitor) {
		visitor.visitConditionCommand(this);
	}

	@Override
	public CharSequence toText() {
		StringBuilder result = new StringBuilder();
		result.append(super.toText());
		for (CMakeNode node : getChildren()) {
			result.append("\t%s".formatted(node.toText())); //$NON-NLS-1$
		}
		return result;
	}

}
