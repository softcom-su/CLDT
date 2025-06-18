package su.softcom.cldt.internal.core.runners;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ResourceBundle;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;

import su.softcom.cldt.core.runners.IRunner;
import su.softcom.cldt.internal.core.preferences.CInitializer;

/** Intel optimization script runner. */
public class IntelRunner extends WSLRunner implements IRunner {

	private static final ResourceBundle RESOURCE_BUNDLE = ResourceBundle.getBundle("OSGI-INF.l10n.bundle"); //$NON-NLS-1$

	private static final String DEFAULT_BUILD_FOLDER = "build"; //$NON-NLS-1$

	private IFile source;

	@Override
	public void addResource(final IResource resource) {
		problems.setDefault(resource.getProject());

		if (resource instanceof IFile) {
			source = (IFile) resource;
		}
	}

	@Override
	protected String getExecutable() {
		try {
			setBuildFolder();
		} catch (NullPointerException e) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.sourcesNotFound")); //$NON-NLS-1$
			return null;
		} catch (CoreException e) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.cannotCreateBuildFolder")); //$NON-NLS-1$
			return null;
		}

		final Path path = Paths.get(CInitializer.getPreferenceStore().getString(CInitializer.INTEL_PATH), "intel.sh"); //$NON-NLS-1$

		if (!Files.exists(path)) {
			problems.emitError(RESOURCE_BUNDLE.getString("errors.optimizerNotFound")); //$NON-NLS-1$
			return null;
		}

		return toUnixPath(path.toAbsolutePath().toString());
	}

	protected void setBuildFolder() throws CoreException {
		final IFolder buildFolder = source.getProject().getFolder(DEFAULT_BUILD_FOLDER);

		if (!buildFolder.exists()) {
			buildFolder.create(IResource.DERIVED, true, null);
		}

		IFile copy = buildFolder.getFile(source.getName());
		if (copy.exists()) {
			copy.delete(true, null);
		}

		source.copy(copy.getFullPath(), true, null);
		addArg(toUnixPath(copy.getLocation().toString()));
	}

}
