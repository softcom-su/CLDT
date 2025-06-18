package su.softcom.cldt.core.cmake;

public class ArgumentNode extends CMakeNode {

	boolean quoted = false;

	boolean bracketed = false;

	private String value;

	/**
	 * Create simple argument
	 * 
	 * @param root  (parent node)
	 * @param value of argument
	 */
	public ArgumentNode(CMakeNode root, String value) {
		super(root);
		this.value = value;
	}

	/**
	 * @return value of this argument node
	 */
	public String getValue() {
		return value;
	}

	@Override
	public void accept(ICMakeVisitor visitor) {
		visitor.visitArgument(this);
	}

	@Override
	public CharSequence toText() {
		String result = "%s"; //$NON-NLS-1$
		if (bracketed) {
			result = result.formatted("[[%s]]"); //$NON-NLS-1$
		}
		if (quoted) {
			result = result.formatted("\"%s\""); //$NON-NLS-1$
		}
		return result.formatted(this.value);
	}

	public void makeQuoted() {
		this.quoted = true;
	}

	public void makeBracketed() {
		this.bracketed = true;
	}

	public void unquote() {
		this.quoted = false;
	}

	public void unbracket() {
		this.bracketed = false;
	}

	@Override
	public String toString() {
		return "ArgNode: " + value; //$NON-NLS-1$
	}

}
