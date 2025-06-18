package su.softcom.cldt.parsers.tests.sln;

import su.softcom.cldt.internal.msvc.sln.*;
import su.softcom.cldt.msvc.ISlnParser;
import su.softcom.cldt.msvc.ParserPlugin;
import su.softcom.cldt.msvc.ISlnParser.IProjectDescriptor;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

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
import java.util.UUID;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;
import org.osgi.framework.FrameworkUtil;
import org.yaml.snakeyaml.Yaml;

import su.softcom.cldt.parsers.tests.sln.SlnTestCase.Project;

class SlnParserTests {

	@BeforeAll
	static void setUpBeforeClass() throws Exception {
	}

	@AfterAll
	static void tearDownAfterClass() throws Exception {
	}

	@BeforeEach
	void setUp() throws Exception {
	}

	@Test
	final void testRegexp() {
		assertTrue(SlnParser.checkProjectPattern(
				"Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"NetWorkLib\", \"BION\\NetWorkLib\\NetWorkLib.vcxproj\", \"{FF86DD2F-1069-4B36-BE2D-087C0B0214E9}\"\n"
						+ "EndProject"));
		assertTrue(SlnParser.checkProjectPattern(
				"Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"OEK\", \"OEK\\OEK.vcxproj\", \"{9AB7BE5E-42BB-447B-A08D-E283D6A8B311}\"\n"
						+ "	ProjectSection(ProjectDependencies) = postProject\n"
						+ "		{08EF88BD-AFBD-4DD8-8F84-73BBBFC2FB61} = {08EF88BD-AFBD-4DD8-8F84-73BBBFC2FB61}\n"
						+ "		{B32D68CA-317D-45C1-8E5C-688645987559} = {B32D68CA-317D-45C1-8E5C-688645987559}\n"
						+ "		{FA4A68E1-EEC5-4823-BE52-E0AD499A0CBA} = {FA4A68E1-EEC5-4823-BE52-E0AD499A0CBA}\n"
						+ "	EndProjectSection\n" + "EndProject"));
		assertTrue(SlnParser.checkProjectPattern(
				"Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"NetWorkLib\", \"BION\\NetWorkLib\\NetWorkLib.vcxproj\", \"{FF86DD2F-1069-4B36-BE2D-087C0B0214E9}\"\n"
						+ "EndProject\n"
						+ "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"OEK\", \"OEK\\OEK.vcxproj\", \"{9AB7BE5E-42BB-447B-A08D-E283D6A8B311}\"\n"
						+ "	ProjectSection(ProjectDependencies) = postProject\n"
						+ "		{08EF88BD-AFBD-4DD8-8F84-73BBBFC2FB61} = {08EF88BD-AFBD-4DD8-8F84-73BBBFC2FB61}\n"
						+ "		{B32D68CA-317D-45C1-8E5C-688645987559} = {B32D68CA-317D-45C1-8E5C-688645987559}\n"
						+ "		{FA4A68E1-EEC5-4823-BE52-E0AD499A0CBA} = {FA4A68E1-EEC5-4823-BE52-E0AD499A0CBA}\n"
						+ "	EndProjectSection\n" + "EndProject"));
	}

	@ParameterizedTest
	@MethodSource("getCases")
	final void test(SlnTestCase testCase) throws URISyntaxException, IOException, CoreException {
		URL slnFileURL = FileLocator
				.toFileURL(FrameworkUtil.getBundle(getClass()).getEntry(testCase.getPath().toString()));
		Path path = Paths.get(slnFileURL.toURI());
		ISlnParser p = ParserPlugin.getInstance().getSlnParser(path);
		p.run(null);
		assertEquals(p.getProjects().size(), testCase.getProjects().size());
		for (IProjectDescriptor project : p.getProjects()) {
			boolean foundProject = false;
			for (Project refProject : testCase.getProjects()) {
				if (project.getId().equals(refProject.id())) {
					if (project.getName().equals(refProject.name())) {
						if (project.getVcxProj().toString().replace('/', '\\').endsWith(refProject.vcxprojPath())) {
							for (UUID dependency : project.getDependencies()) {
								boolean found = false;
								for (UUID refDependency : refProject.dependencies()) {
									if (dependency.equals(refDependency)) {
										found = true;
										break;
									}
								}
								if (!found) {
									fail("ref dependency for UUID: %s not found".formatted(dependency));
								}
							}
							foundProject = true;
							break;
						}
					}
				}
			}
			if (!foundProject) {
				// FIXME
				fail("reference project not found");
			}
		}
	}

	public static List<SlnTestCase> getCases() throws IOException, URISyntaxException {
		URL slnFileURL = FileLocator.toFileURL(FrameworkUtil.getBundle(SlnParserTests.class).getEntry("resources"
				+ IPath.SEPARATOR + "cases" + IPath.SEPARATOR + "sln" + IPath.SEPARATOR + "SlnParserTests.yaml"));
		Path path = Paths.get(slnFileURL.toURI());
		Yaml yaml = new Yaml();
		List<SlnTestCase> result = new ArrayList<SlnTestCase>();
		try (InputStream in = new FileInputStream(path.toFile())) {
			Iterable<Object> yamlContent = yaml.loadAll(in);
			for (Object o : yamlContent) {
				if (o instanceof Iterable cases) {
					for (Object caseObj : cases) {
						result.add(readTestCase(caseObj));
					}
				}
			}
		}
		return result;
	}

	static SlnTestCase readTestCase(Object caseObj) throws URISyntaxException {
		if (caseObj instanceof Map<?, ?> c) {
			Object pathStr = c.get("path");
			Object projects = c.get("projects");
			SlnTestCase result = null;

			if (pathStr instanceof String pathURI) {
				result = new SlnTestCase(new URI(pathURI));
			} else {
				throw new IllegalArgumentException("Expected instance of String class");
			}

			if (projects instanceof List projectsList) {
				for (Object projectObj : projectsList) {
					if (projectObj instanceof Map proj) {
						Project p = result.addProject(UUID.fromString((String) proj.get("id")),
								(String) proj.get("name"), (String) proj.get("path"));
						Object dependenciesObject = proj.get("dependencies");
						if (dependenciesObject instanceof List depList) {
							for (Object dep : depList) {
								if (dep instanceof String depString) {
									p.addDependency(UUID.fromString(depString));
								}
							}
						}
					}
				}
			}
			return result;
		}
		throw new IllegalArgumentException("Expected instance of Map interface");
	}

}
