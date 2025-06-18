package su.softcom.cldt.core.tests.cmake;

import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.osgi.framework.FrameworkUtil;
import org.yaml.snakeyaml.LoaderOptions;
import org.yaml.snakeyaml.Yaml;
import org.yaml.snakeyaml.constructor.Constructor;

import su.softcom.cldt.core.cmake.CMakeParser;
import su.softcom.cldt.core.cmake.CMakeParser.UnexpectedTokenException;
import su.softcom.cldt.core.cmake.CMakeRoot;

public class CMakeParserTests {
	
	@ParameterizedTest
	@MethodSource("getSources")
	public void test(String value, List<String> expectedTokens) throws IOException, NoSuchMethodException, SecurityException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, UnexpectedTokenException, CoreException {
		CMakeParser parser = new CMakeParser(new String(value.getBytes()), false);
		CMakeRoot root = parser.parse();
	}

	
	public static Stream<Arguments> getSources() throws IOException, URISyntaxException {
		List<Arguments> arguments = new ArrayList<>();
		File testDataFolder = new File(FileLocator
				.toFileURL(FrameworkUtil.getBundle(CMakeLexerTests.class)
						.getEntry("resources" + IPath.SEPARATOR + "cmake" //$NON-NLS-1$ //$NON-NLS-2$
								+ IPath.SEPARATOR + "CMakeFiles")) //$NON-NLS-1$
				.toURI());
		File[] cases = testDataFolder.listFiles(new FilenameFilter() {
			
			@Override
			public boolean accept(File dir, String name) {
				// TODO Auto-generated method stub
				return name.endsWith(".yml");
			}
		});
		Yaml yml = new Yaml(new Constructor(TestCaseObject.class, new LoaderOptions()));
		for(File caseFile : cases) {
			TestCaseObject testCase = yml.load(new FileInputStream(caseFile));
			arguments.add(Arguments.of(testCase.content, testCase.tokens));
		}
		return arguments.stream();
	}

}
