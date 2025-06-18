package su.softcom.cldt.core.cmake;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.resources.IFolder;

import su.softcom.cldt.core.source.ISourceFile;

public class TargetSynchronizer implements ICMakeVisitor {

	final List<ISourceFile> sources;
	final List<IFolder> includes;
	final List<String> compileOptions;
	final List<String> linkOptions;
	final List<String> dependencies;

	boolean foundDefenition = false;
	boolean foundSources = false;
	boolean foundIncludes = false;
	boolean foundCompileOptions = false;
	boolean foundLinkOptions = false;
	boolean foundDependencies = false;

	public TargetSynchronizer(Target target) {
		sources = new ArrayList<>(target.getSources());
		includes = new ArrayList<>(target.includes());
		linkOptions = new ArrayList<>(target.linkOptions());
		compileOptions = new ArrayList<>(target.getCompileOptions());
		dependencies = new ArrayList<>(target.dependencies());
	}

	@Override
	public void visitRoot(CMakeRoot node) {
		// TODO Auto-generated method stub
	}

	@Override
	public void visitArgument(ArgumentNode node) {
		// TODO Auto-generated method stub

	}

	@Override
	public void visitCommand(CommandNode node) {
		// TODO Auto-generated method stub

	}

	@Override
	public void visitConditionCommand(ConditionCommand node) {
		// TODO Auto-generated method stub

	}

	@Override
	public void visitComment(CommentNode node) {
		// TODO Auto-generated method stub

	}

}
