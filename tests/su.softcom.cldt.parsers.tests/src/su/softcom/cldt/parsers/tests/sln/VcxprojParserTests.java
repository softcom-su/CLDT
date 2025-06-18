package su.softcom.cldt.parsers.tests.sln;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;
import org.osgi.framework.FrameworkUtil;
import org.yaml.snakeyaml.Yaml;

import su.softcom.cldt.msvc.IVcxprojParser;
import su.softcom.cldt.msvc.ParserPlugin;


/**
 * @author petrb
 *
 */
public class VcxprojParserTests {
	
	@ParameterizedTest
	@MethodSource("getCases")
	final void test(VcxprojTestCase testCase) throws URISyntaxException, IOException, CoreException {
		URL slnFileURL = FileLocator.toFileURL(FrameworkUtil.getBundle(getClass()).getEntry(testCase.getPath().toString()));
		Path path = Paths.get(slnFileURL.toURI());
		IVcxprojParser p = ParserPlugin.getInstance().getVcxprojParser(path);
		p.run(new NullProgressMonitor());
	}
	
	static List<VcxprojTestCase> getCases() throws IOException, URISyntaxException {
		URL slnFileURL = FileLocator.toFileURL(FrameworkUtil.getBundle(SlnParserTests.class).getEntry("resources" + IPath.SEPARATOR + "cases" + IPath.SEPARATOR + "vcxproj" + IPath.SEPARATOR + "VcxprojParserTests.yaml"));
		Path path = Paths.get(slnFileURL.toURI());
		Yaml yaml = new Yaml();
		List<VcxprojTestCase> result = new ArrayList<VcxprojTestCase>();
		try (InputStream in = new FileInputStream(path.toFile())) {
			Iterable<Object> yamlContent = yaml.loadAll(in);
			for(Object o : yamlContent) {
				if(o instanceof Iterable cases) {
					for(Object caseObj : cases) {
						result.add(readTestCase(caseObj));
					}
				}
			}
		}
		return result;
	}

	static VcxprojTestCase readTestCase(Object caseObj) throws URISyntaxException {
		if(caseObj instanceof Map<?, ?> c) {
			Object pathStr = c.get("path");
			Object sources = c.get("sources");
			VcxprojTestCase result = null;

			if(pathStr instanceof String pathURI) {
				result = new VcxprojTestCase(new URI(pathURI));
			} else {
				throw new IllegalArgumentException("Expected instance of String class");
			}

			return result;
		}
		throw new IllegalArgumentException("Expected instance of Map interface");
	}

}
