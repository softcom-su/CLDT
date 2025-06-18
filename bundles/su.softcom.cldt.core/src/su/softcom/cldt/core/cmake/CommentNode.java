package su.softcom.cldt.core.cmake;

import org.eclipse.core.runtime.Assert;

import su.softcom.cldt.internal.core.cmake.tokens.CommentToken;

/**
 * @author Petr Babanov (petr.babanov@softcom.su) - initial implementation
 */
public class CommentNode extends CMakeNode {

	final CommentToken token;

	/**
	 * Basic constructor
	 * 
	 * @param token - to set
	 */
	public CommentNode(CommentToken token) {
		super(null);
		Assert.isNotNull(token);
		this.token = token;
	}

	@Override
	public void accept(ICMakeVisitor visitor) {
		visitor.visitComment(this);
	}

}
