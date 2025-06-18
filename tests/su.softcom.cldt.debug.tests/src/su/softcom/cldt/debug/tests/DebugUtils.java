package su.softcom.cldt.debug.tests;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.junit.jupiter.api.Assertions;
import org.osgi.framework.Bundle;

import su.softcom.cldt.debug.core.ELF;
import su.softcom.cldt.debug.internal.core.ELFReader;

/**
 * Utils for tests of debug
 * 
 * @author: maxim.drumov@softcom.su
 */
public class DebugUtils {

	public static final String EXEC_PATH = "resources" + IPath.SEPARATOR + "executable" + IPath.SEPARATOR + "linux"
			+ IPath.SEPARATOR;
	
	public static ELF readElf(Bundle bundle, String path) throws FileNotFoundException, IOException {
		String resoursePath = bundle.getResource(path).getFile();
		String bundlePath = FileLocator.getBundleFileLocation(bundle).get().getAbsolutePath();
		String execFilePath = bundlePath + resoursePath;

		File file = new File(execFilePath);
		return ELFReader.readELFFile(file);
		
	}
	
}
