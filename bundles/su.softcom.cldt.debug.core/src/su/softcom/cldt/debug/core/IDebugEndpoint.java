package su.softcom.cldt.debug.core;

import java.net.URI;

public interface IDebugEndpoint {

	int getPort();

	URI getDebugProgram();
	
	String getHost();

	boolean isAlive();

}
