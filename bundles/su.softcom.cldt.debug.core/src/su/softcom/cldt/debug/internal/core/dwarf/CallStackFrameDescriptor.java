package su.softcom.cldt.debug.internal.core.dwarf;

import java.util.List;

public class CallStackFrameDescriptor {
	
	static public class VariableInfo {
		public String type;
		public String name;
		public String value;
		public List<VariableInfo> insideVariables;
	}

	public long adderes;
	public long address2;
	public String name;
	public long offset;
	public String srcPath;
	public int line;
	
	public List<VariableInfo> variablesData;

}
