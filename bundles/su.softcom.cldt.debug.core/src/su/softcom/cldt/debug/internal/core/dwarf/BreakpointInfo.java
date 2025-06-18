package su.softcom.cldt.debug.internal.core.dwarf;

public class BreakpointInfo {

	private int currentLineNumber;
	private long address;
	private long modulesBaseAddress;
	
	public BreakpointInfo(int lineNumber, long address) {
		this.currentLineNumber = lineNumber;
		this.address = address;
	}
	
	public int getCurrentLine() {
		return this.currentLineNumber;
	}
	
	public long getAddress() {
		return this.address;
	}

	public long getModulesBaseAddress() {
		return modulesBaseAddress;
	}

	public void setModulesBaseAddress(long modulesBaseAddress) {
		this.modulesBaseAddress = modulesBaseAddress;
	}
}
