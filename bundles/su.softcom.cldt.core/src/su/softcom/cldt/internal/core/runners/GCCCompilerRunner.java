package su.softcom.cldt.internal.core.runners;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.MessageFormat;

import su.softcom.cldt.internal.core.preferences.CInitializer;

/** GCC compiler runner. */
public class GCCCompilerRunner extends ClangCompilerRunner {

	@Override
	protected String getCompilerPath() {
		final Path path = Paths.get(CInitializer.getPreferenceStore().getString(CInitializer.GCC_PATH),
				CInitializer.IS_WINDOWS ? isCpp ? "g++.exe" : "gcc.exe" : isCpp ? "g++" : "gcc"); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$

		if (!Files.exists(path) || !Files.isExecutable(path)) {
			problems.emitError(
					MessageFormat.format(RESOURCE_BUNDLE.getString("errors.compilerNotFound"), "GCC")); //$NON-NLS-1$ //$NON-NLS-2$
			return null;
		}

		return path.toAbsolutePath().toString();
	}

}
