package su.softcom.cldt.core.cmake;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.eclipse.core.runtime.Assert;

public class CMakeNode {

	private final List<CMakeNode> children;
	private CMakeNode root;

	public CMakeNode(CMakeNode root) {
		this.root = root;
		children = new ArrayList<CMakeNode>();
	}

	public void accept(ICMakeVisitor visitor) {
		// Base class, do nothing
	}

	public boolean hasChildren() {
		return children.size() > 0;
	}

	public List<CMakeNode> getChildren() {
		return Collections.unmodifiableList(children);
	}

	public void addChild(CMakeNode child) {
		Assert.isNotNull(child);
		children.add(child);
	}

	public void removeChild(CMakeNode child) {
		if (children.contains(child)) {
			children.remove(child);
		}
	}

	/**
	 * Get parent node.
	 * 
	 * @return parent CMakeNode or {@code null} if this is root node of tree.
	 */
	public CMakeNode getRoot() {
		return root;
	}

	public void addChild(int position, CMakeNode child) {
		Assert.isNotNull(child);
		children.add(position, child);
	}

	/**
	 * Get text representation for current node. Default implementation returns text
	 * representation for all children.
	 * 
	 * @return
	 */
	public CharSequence toText() {
		StringBuilder sb = new StringBuilder();
		for (CMakeNode child : children) {
			sb.append(child.toText());
		}
		return sb;
	}
}
