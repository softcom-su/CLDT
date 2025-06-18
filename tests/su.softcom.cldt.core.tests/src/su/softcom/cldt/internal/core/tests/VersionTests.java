package su.softcom.cldt.internal.core.tests;

import java.util.stream.Stream;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import org.junit.Assert;
import su.softcom.cldt.core.Version;
import su.softcom.cldt.internal.core.LLVMInstance;

public class VersionTests {

	@ParameterizedTest
	@MethodSource("getVersionString")
	public void versionTest(String str, int major, int minor, int patch, String suffix) {
		Version v = new Version(str);
		Assert.assertEquals(major, v.major());
		Assert.assertEquals(minor, v.minor());
		Assert.assertEquals(patch, v.patch());
		Assert.assertEquals(suffix, v.suffix());
	}

	public static Stream<Arguments> getVersionString() {
	    return Stream.of(
	        Arguments.of("0.0.0", 0, 0, 0, null), //$NON-NLS-1$
	        Arguments.of("0.0.0-STABLE", 0, 0, 0, "STABLE"), //$NON-NLS-1$ //$NON-NLS-2$
	        Arguments.of("0.0", 0, 0, -1, null), //$NON-NLS-1$
	        Arguments.of("1.2.3", 1, 2, 3, null), //$NON-NLS-1$
	        Arguments.of("1.2-rc1", 1, 2, -1, "rc1"), //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-2
	        Arguments.of("2.5-BETA", 2, 5, -1, "BETA"), //$NON-NLS-1$ //$NON-NLS-2$
	        Arguments.of("7", 7, -1, -1, null), //$NON-NLS-1$
	        Arguments.of("42-ALPHA", 42, -1, -1, "ALPHA"), //$NON-NLS-1$ //$NON-NLS-2$
	        Arguments.of("0.0.0-TEST", 0, 0, 0, "TEST"), //$NON-NLS-1$ //$NON-NLS-2$
	        Arguments.of("10.20.30", 10, 20, 30, null), //$NON-NLS-1$
	        Arguments.of("3.2.1-custom-build", 3, 2, 1, "custom-build"), //$NON-NLS-1$ //$NON-NLS-2$
	        Arguments.of("4.0.1-rc2", 4, 0, 1, "rc2"), //$NON-NLS-1$ //$NON-NLS-2$
	        Arguments.of(" 1.2.3 ", 1, 2, 3, null), //$NON-NLS-1$
	        Arguments.of("2.0.0- SNAPSHOT ", 2, 0, 0, "SNAPSHOT"), //$NON-NLS-1$ //$NON-NLS-2$
	        Arguments.of("0.0-EXPERIMENTAL", 0, 0, -1, "EXPERIMENTAL"), //$NON-NLS-1$ //$NON-NLS-2$
	        Arguments.of("5.6", 5, 6, -1, null),//$NON-NLS-1$
	        Arguments.of("8", 8, -1, -1, null) //$NON-NLS-1$
	    );
	}

}
