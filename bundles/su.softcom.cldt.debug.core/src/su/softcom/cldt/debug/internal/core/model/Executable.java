package su.softcom.cldt.debug.internal.core.model;

public class Executable extends Module {

	/**
	 * Executable File type of Module
	 * @param execName
	 * @param initAddr
	 */
	public Executable(String execName, long initAddr) {
		super(execName, initAddr);
	}

}
