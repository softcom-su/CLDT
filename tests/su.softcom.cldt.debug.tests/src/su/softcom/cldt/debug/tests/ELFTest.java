package su.softcom.cldt.debug.tests;

import java.io.IOException;
import java.util.List;

import org.eclipse.core.runtime.CoreException;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;

import su.softcom.cldt.debug.core.ELF;

/**
 * Tests for reading elf files
 * 
 * @author maxim.drumov@softcom.su
 */
@TestInstance(Lifecycle.PER_CLASS)
public class ELFTest {
	
	static Bundle bundle = null;
	
	@Test
	public void testReadElf() throws IOException, CoreException {
		bundle = FrameworkUtil.getBundle(getClass());
		List<String> projectNames = List.of("linpack", "curl");
		for (String projectName : projectNames) {
			ELF elf = DebugUtils.readElf(bundle, DebugUtils.EXEC_PATH + projectName);
			Assertions.assertNotNull(elf);
		}
	}

}
