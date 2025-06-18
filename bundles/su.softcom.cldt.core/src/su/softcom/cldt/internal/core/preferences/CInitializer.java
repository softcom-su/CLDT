package su.softcom.cldt.internal.core.preferences;

import org.eclipse.core.runtime.preferences.InstanceScope;
import org.eclipse.jface.preference.IPreferenceStore;
import org.eclipse.ui.preferences.ScopedPreferenceStore;

import su.softcom.cldt.core.preferences.AbstractInitializer;

/** C/C++ path preferences. */
public class CInitializer extends AbstractInitializer {

	public static final String STORE_QUALIFIER = "su.softcom.cldt.core"; //$NON-NLS-1$
	/**
	 * Path to bin folder of Clang compiler
	 */
	public static final String CLANG_PATH = "clangPath"; //$NON-NLS-1$
	public static final String CMAKE_PATH = "cmakePath"; //$NON-NLS-1$
	public static final String WSL_PATH = "wslPath"; //$NON-NLS-1$
	public static final String INTEL_PATH = "intelPath"; //$NON-NLS-1$

	public static final String NINJA_PATH = "ninjaPath"; //$NON-NLS-1$

	public static final String GCC_PATH = "gccPath"; //$NON-NLS-1$
	public static final String QMAKE_PATH = "qmakePath"; //$NON-NLS-1$

	public static final String MSVC_DEFAULT = "msvcDefault"; //$NON-NLS-1$
	public static final String MSVC_2013 = "msvc2013"; //$NON-NLS-1$
	public static final String MSVC_2010 = "msvc2010"; //$NON-NLS-1$
	public static final String MSVC_2008 = "msvc2008"; //$NON-NLS-1$
	public static final String MSVC_2003 = "msvc2003"; //$NON-NLS-1$

	public static final String TOOLSET_DEFAULT = "toolsetDefault"; //$NON-NLS-1$
	public static final String TOOLSET_2013 = "toolset2013"; //$NON-NLS-1$
	public static final String TOOLSET_2010 = "toolset2010"; //$NON-NLS-1$
	public static final String TOOLSET_2008 = "toolset2008"; //$NON-NLS-1$
	public static final String TOOLSET_2003 = "toolset2003"; //$NON-NLS-1$

	public static final String MIPS_TOOLCHAIN_PATH = "mipsToolchainPath"; //$NON-NLS-1$
	public static final String IS_SOFT_FLOAT = "isSoftFloat"; //$NON-NLS-1$

	public static final String ARM_TOOLCHAIN_PATH = "armToolchainPath"; //$NON-NLS-1$
	public static final String BASH_PATH = "bashPath"; //$NON-NLS-1$

	@Override
	@SuppressWarnings("nls")
	public void initializeDefaultPreferences() {
		IPreferenceStore store = getPreferenceStore();

		store.setDefault(CLANG_PATH, searchDirectory("clang-komdiv/bin", "clang-komdiv-win/bin",
				"Deploy.v1/Release/bin", "Program Files/LLVM/bin", IS_WINDOWS ? "clang.exe" : "clang"));
		store.setDefault(CMAKE_PATH,
				searchFile("Program Files/CMake/bin/cmake.exe", IS_WINDOWS ? "cmake.exe" : "cmake"));

		store.setDefault(WSL_PATH, IS_WINDOWS ? "ubuntu1604.exe" : "/bin/bash");
		store.setDefault(INTEL_PATH, searchDirectory("intel"));

		store.setDefault(NINJA_PATH,
				IS_WINDOWS ? searchFile("ninja.exe", "ninja/ninja.exe") : searchFile("ninja", "ninja/ninja"));

		final String QT_MAKE = searchDirectory("Qt/Tools/*/bin/mingw32-make.exe");
		store.setDefault(GCC_PATH, !QT_MAKE.isEmpty() ? QT_MAKE
				: searchDirectory("MinGW/bin", "msys64/usr/bin", "cygwin/bin", IS_WINDOWS ? "gcc.exe" : "gcc"));
		store.setDefault(QMAKE_PATH,
				searchFile("Qt/*/mingw53_32/bin/qmake.exe", "Qt/*/mingw73_32/bin/qmake.exe",
						"Qt/*/mingw73_64/bin/qmake.exe", "Qt/*/mingw81_64/bin/qmake.exe",
						"Qt/*/mingw81_32/bin/qmake.exe", IS_WINDOWS ? "qmake.exe" : "qmake"));

		store.setDefault(MSVC_DEFAULT,
				searchFile("Program Files (x86)/Microsoft Visual Studio/*/BuildTools/VC/Tools/MSVC/*/bin/*/*/cl.exe",
						"Program Files (x86)/Microsoft Visual Studio/*/Community/VC/Tools/MSVC/*/bin/*/*/cl.exe",
						"Program Files (x86)/Microsoft Visual Studio/*/Professional/VC/Tools/MSVC/*/bin/*/*/cl.exe",
						"Program Files (x86)/Microsoft Visual Studio/*/Enterprise/VC/Tools/MSVC/*/bin/*/*/cl.exe"));

		store.setDefault(TOOLSET_DEFAULT,
				searchFile("Program Files (x86)/Microsoft Visual Studio/*/BuildTools/VC/Auxiliary/Build/vcvars64.bat",
						"Program Files (x86)/Microsoft Visual Studio/*/BuildTools/VC/Auxiliary/Build/vcvars32.bat",
						"Program Files (x86)/Microsoft Visual Studio/*/Community/VC/Auxiliary/Build/vcvars64.bat",
						"Program Files (x86)/Microsoft Visual Studio/*/Community/VC/Auxiliary/Build/vcvars32.bat",
						"Program Files (x86)/Microsoft Visual Studio/*/Professional/VC/Auxiliary/Build/vcvars64.bat",
						"Program Files (x86)/Microsoft Visual Studio/*/Professional/VC/Auxiliary/Build/vcvars32.bat",
						"Program Files (x86)/Microsoft Visual Studio/*/Enterprise/VC/Auxiliary/Build/vcvars64.bat",
						"Program Files (x86)/Microsoft Visual Studio/*/Enterprise/VC/Auxiliary/Build/vcvars32.bat"));

		store.setDefault(MSVC_2013, searchFile("Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/amd64/cl.exe",
				"Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/cl.exe"));
		store.setDefault(TOOLSET_2013,
				searchFile("Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/amd64/vcvars64.bat",
						"Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/vcvars32.bat"));

		store.setDefault(MSVC_2010, searchFile("Program Files (x86)/Microsoft Visual Studio 10.0/VC/bin/amd64/cl.exe",
				"Program Files (x86)/Microsoft Visual Studio 10.0/VC/bin/cl.exe"));
		store.setDefault(TOOLSET_2010,
				searchFile("Program Files (x86)/Microsoft Visual Studio 10.0/VC/bin/amd64/vcvars64.bat",
						"Program Files (x86)/Microsoft Visual Studio 10.0/VC/bin/vcvars32.bat"));

		store.setDefault(MSVC_2008, searchFile("Program Files (x86)/Microsoft Visual Studio 9.0/VC/bin/cl.exe"));
		store.setDefault(TOOLSET_2008,
				searchFile("Program Files (x86)/Microsoft Visual Studio 9.0/VC/bin/vcvars32.bat"));

		store.setDefault(MSVC_2003, searchFile("Program Files (x86)/Microsoft Visual Studio .NET 2003/Vc7/bin/cl.exe"));
		store.setDefault(TOOLSET_2003,
				searchFile("Program Files (x86)/Microsoft Visual Studio .NET 2003/Vc7/bin/vcvars32.bat"));

		store.setDefault(MIPS_TOOLCHAIN_PATH, searchDirectory("mips-mti-linux-gnu/2019.02-01",
				"mips-mti-linux-gnu/2018.11-01", "mips-mti-linux-gnu/2018.09-02"));
		store.setDefault(IS_SOFT_FLOAT, false);

		store.setDefault(ARM_TOOLCHAIN_PATH, searchDirectory("gcc-arm-none-eabi-10.3-2021.07"));
		store.setDefault(BASH_PATH, searchFile("Program Files/Git/bin/bash.exe"));
	}

	/**
	 * Returns the preference store of this preference initializer.
	 * 
	 * @return the preference store
	 */
	public static IPreferenceStore getPreferenceStore() {
		return new ScopedPreferenceStore(InstanceScope.INSTANCE, STORE_QUALIFIER);
	}
}
