package su.softcom.cldt.core.tests;

import org.junit.Assert;
import org.junit.jupiter.api.Test;

import su.softcom.cldt.core.CMakeCorePlugin;

public class CMakeCorePluginTests {
	
	@Test
	public void getDefaultTest() {
		Assert.assertNotNull(CMakeCorePlugin.getDefault());
	}
	
	@Test
	public void cmakeInstanceTest() {
		CMakeCorePlugin.getDefault();
		Assert.assertNotNull(CMakeCorePlugin.getDefault().getCMakeInstance());
	}
	
	@Test
	public void llvmInstanceTest() {
		Assert.assertNotNull(CMakeCorePlugin.getDefault().getLLVMInstance(null));
	}
}
