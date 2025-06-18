package su.softcom.cldt.core.cmake;

public interface ICMakeVisitor {

	void visitRoot(CMakeRoot root);

	void visitArgument(ArgumentNode argument);

	void visitCommand(CommandNode command);

	void visitConditionCommand(ConditionCommand conditionCommand);

	void visitComment(CommentNode comment);

}
