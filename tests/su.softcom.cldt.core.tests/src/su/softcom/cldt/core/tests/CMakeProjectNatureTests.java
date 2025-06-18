package su.softcom.cldt.core.tests;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.fail;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.eclipse.core.resources.ICommand;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Order;
import org.junit.jupiter.api.Test;

import su.softcom.cldt.core.CmakeProjectNature;
import su.softcom.cldt.internal.core.builders.CMakeConfigureBuilder;
import su.softcom.cldt.internal.core.builders.CMakeProjectBuilder;

/**
 * 
 * TODO: test for repeated adding nature
 */
public class CMakeProjectNatureTests {
	
	static IProject testProject;
	
	@BeforeAll
	public static void setupBeforeTests() throws CoreException {
		testProject = ResourcesPlugin.getWorkspace().getRoot().getProject("testProject"); //$NON-NLS-1$
		testProject.create(new NullProgressMonitor());
		testProject.open(new NullProgressMonitor());
	}
	
	@AfterAll
	public static void cleanAfterTests() throws CoreException {
		testProject.delete(false, new NullProgressMonitor());
	}
	
	@Test
	@Order(1)
	public void checkSetupDeleteNature() throws CoreException {
		IProjectDescription description = testProject.getDescription();
		List<String> natures = new ArrayList<>(Arrays.asList(description.getNatureIds()));
		natures.add(CmakeProjectNature.ID);
		description.setNatureIds(natures.toArray(new String[description.getNatureIds().length]));
		testProject.setDescription(description, new NullProgressMonitor());
		description = testProject.getDescription();
		ICommand[] buildCommands = description.getBuildSpec();
		assertEquals(buildCommands[buildCommands.length - 1].getBuilderName(), CMakeProjectBuilder.ID);
		assertEquals(buildCommands[buildCommands.length - 2].getBuilderName(), CMakeConfigureBuilder.ID);
		int valueOfBuilders = description.getBuildSpec().length;
		natures.remove(CmakeProjectNature.ID);
		description.setNatureIds(natures.toArray(new String[natures.size()]));
		testProject.setDescription(description, new NullProgressMonitor());
		description = testProject.getDescription();
		assertEquals(valueOfBuilders - description.getBuildSpec().length, 2);
	}
}
