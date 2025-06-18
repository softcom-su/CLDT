package su.softcom.cldt.core.tests.cmake;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.stream.Stream;

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.osgi.framework.FrameworkUtil;
import org.yaml.snakeyaml.LoaderOptions;
import org.yaml.snakeyaml.Yaml;
import org.yaml.snakeyaml.constructor.Constructor;

import su.softcom.cldt.core.tests.cmake.TestCaseObject.TokenDescription;
import su.softcom.cldt.internal.core.cmake.tokens.CMakeLexer;
import su.softcom.cldt.internal.core.cmake.tokens.CMakeToken;

public class CMakeLexerTests {

	@ParameterizedTest
	@MethodSource("getSources")
	public void test(TestCaseObject testCase) throws IOException, NoSuchMethodException, SecurityException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException {
		CMakeLexer lexer = new CMakeLexer(new ByteArrayInputStream(testCase.content.getBytes()));
		Queue<CMakeToken> tokens = lexer.read();
		for(TokenDescription desc : testCase.tokens) {
			CMakeToken token = tokens.poll();
			assertEquals(token.getValue(), desc.value);
			assertEquals(token.getTypeName(), desc.type);
		}
		
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
				return name.endsWith(".yml");
			}
		});
		Yaml yml = new Yaml(new Constructor(TestCaseObject.class, new LoaderOptions()));
		for(File caseFile : cases) {
			TestCaseObject testCase = yml.load(new FileInputStream(caseFile));
			arguments.add(Arguments.of(testCase));
		}
		return arguments.stream();
	}

}
