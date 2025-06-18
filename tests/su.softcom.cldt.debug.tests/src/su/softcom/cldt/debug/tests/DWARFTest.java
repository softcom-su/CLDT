package su.softcom.cldt.debug.tests;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

import org.eclipse.core.runtime.CoreException;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;

import su.softcom.cldt.debug.core.ELF;
import su.softcom.cldt.debug.core.dwarf.DwarfInfo;

/**
 * Tests for reading Dwarf info
 * 
 * @author maxim.drumov@softcom.su
 */
@TestInstance(Lifecycle.PER_CLASS)
public class DWARFTest {

	static Bundle bundle = null;
	
	@ParameterizedTest
	@MethodSource("getArguments")
	public void testReadDwarf(ELF elf) throws IOException, CoreException {
		DwarfInfo dwarfInfo = new DwarfInfo(elf);
		Assertions.assertNotNull(dwarfInfo);
	}
	
	public static Stream<Arguments> getArguments() throws FileNotFoundException, IOException{
		List<String> listFileName = List.of("linpack", "curl");
		List<Arguments> args = new ArrayList<>();
		bundle = FrameworkUtil.getBundle(DWARFTest.class);
		for (String fileName : listFileName) {
			ELF elf = DebugUtils.readElf(bundle, DebugUtils.EXEC_PATH + fileName);
			Assertions.assertNotNull(elf);
			args.add(Arguments.of(elf));
		}
		return args.stream();
	}

}
