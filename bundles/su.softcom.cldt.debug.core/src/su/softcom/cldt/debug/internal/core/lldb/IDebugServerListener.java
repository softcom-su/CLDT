package su.softcom.cldt.debug.internal.core.lldb;

public interface IDebugServerListener {

	void serverStarted(String host, long port);
	
	void serverFinished(int exitCode);
}
