package su.softcom.cldt.debug.internal.core.model;

import java.util.Objects;
import su.softcom.cldt.debug.core.ELF;
import su.softcom.cldt.debug.core.IModule;
import su.softcom.cldt.debug.core.info.IDebugInfo;

public abstract class Module implements IModule {

	/**
	 * The full name of the loaded shared object.
	 */
	private final String name;
	/**
	 * The base address of the object loaded into memory.
	 */
	private final long addr;
	/**
	 * Pointer to .dynamic in memory
	 */
	private long ld;
	/**
	 * elfFile of this Module
	 */
	private ELF elfFile;

	private IDebugInfo debugInfo;
	
	/**
	 * @param name
	 * @param addr
	 */
	protected Module(String name, long addr) {
		this.name = name;
		this.addr = addr;
	}

	/**
	 * @param elfFile
	 */
	public void setElfFile(ELF elfFile) {
		this.elfFile = elfFile;
		// TODO: получить дебаг инфу
	}

	/**
	 * @param addr
	 */
	public void setDynamicAddress(long addr) {
		this.ld = addr;
	}

	/**
	 * @return dwarf info 
	 */
	public ELF getElfFile() {
		return elfFile;
	}

	/**
	 * @return name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @return addr value
	 */
	public long getBaseAddress() {
		return addr;
	}

	/**
	 * @return ld value
	 */
	public long getDynamicAddress() {
		return ld;
	}
	
	public IDebugInfo getDebugInfo() {
		return debugInfo;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}

		if (obj == null || getClass() != obj.getClass()) {
			return false;
		}
		Module module = (Module) obj;
		if (this.name.equals(module.getName()) && this.addr == module.getBaseAddress()) {
			return true;
		}
		return super.equals(obj);
	}

	@Override
	public int hashCode() {
		int result = Objects.isNull(ld) ? 0 : Long.hashCode(ld);
		result = elfFile == null ? result : result * 31 + elfFile.getPath().toOSString().hashCode();
		result = 31 * result + name.hashCode();
		result = 31 * result + Long.hashCode(addr);
		return result;
	}

	@Override
	public String toString() {
		return name;
	}

}