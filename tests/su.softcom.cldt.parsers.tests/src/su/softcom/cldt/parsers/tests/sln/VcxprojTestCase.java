package su.softcom.cldt.parsers.tests.sln;

import java.net.URI;
import java.nio.file.Path;
import java.util.List;

public class VcxprojTestCase {
	
	final URI sourseURI;
	List<Path> sources;
	
	public VcxprojTestCase(URI pathURI) {
		sourseURI = pathURI;
	}
	
	public URI getPath() {
		return sourseURI;
	}
}
