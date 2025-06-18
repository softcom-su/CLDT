package su.softcom.cldt.debug.internal.core.dwarf.values;

import java.nio.ByteBuffer;
import java.util.Objects;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.dwarf.DwarfInfo;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;

public class Exprloc extends Value {
	
	private byte[] operations;

	public Exprloc(DwarfInfo info, ByteBuffer data, ICompileUnit cu) {
		super(info, cu, DwarfConstants.DW_FORM_exprloc, data);
	}
	
	@Override
	public int hashCode() {
		return Objects.hash(o, operations, form);
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {			
			return true;
		}
		if (obj == null) {			
			return false;
		}
		if (getClass() != obj.getClass()) {			
			return false;
		}
		Value other = (Value) obj;
		return Objects.equals(o, other.o) && form == other.form;
	}
	
	@Override
	protected void readValue(ByteBuffer buffer, ICompileUnit cu, DwarfInfo info) {
		long size = readUleb128(buffer);
		byte[] data = new byte[(int) size];
		buffer.get(data);
		operations = data;
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder("DW_FORM_exprloc");
		return sb.toString();
	}
	
	@Override
	public Object getValue() {
		return operations;
	}

}
