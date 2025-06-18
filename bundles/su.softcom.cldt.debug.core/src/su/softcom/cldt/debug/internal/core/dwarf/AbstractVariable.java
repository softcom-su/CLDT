package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.ByteBuffer;

import su.softcom.cldt.debug.core.IDebugVariable;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Attribute;
import su.softcom.cldt.debug.internal.core.dwarf.attr.Location;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Tag;

public abstract class AbstractVariable extends Tag implements IDebugVariable {

	protected String name;
	protected int offsetAbstractOrigin = -1;
	protected int fileIndex = -1;
	protected Tag type;
	protected int line = -1;
	protected long address = 0;
	protected boolean isArtificial = false;
	protected Location location;

	/**
	 * 
	 * @param prototype
	 */
	protected AbstractVariable(Tag prototype) {
		super(prototype);
	}

	/**
	 * 
	 * @param code
	 * @param data
	 */
	protected AbstractVariable(int code, ByteBuffer data) {
		super(code, data);
	}

//
//	@Override
//	public long getAddress(GDBClient client, IFunction function, ICompileUnit cu) {
//		// TODO Auto-generated method stub
//		return 0;
//	}

	public int getLine() {
		return line;
	}

	public int getFileIndex() {
		return fileIndex;
	}

	public boolean isArtificial() {
		return isArtificial;
	}

	public void resolve(Attribute[] attrs) {
		super.resolve(attrs);
	}

	@Override
	public long getAddress() {
		return address != 0 ? address : 0;
	}

}
