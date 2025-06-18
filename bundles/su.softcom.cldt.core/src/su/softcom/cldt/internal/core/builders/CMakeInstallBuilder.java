package su.softcom.cldt.internal.core.builders;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;

public class CMakeInstallBuilder extends CMakeBuilder {

	public static final String ID = "su.softcom.cldt.core.builder.install"; //$NON-NLS-1$

	private static final String MESSAGE_STRING = "Установка";

	@Override
	protected IProject[] build(int kind, Map<String, String> args, IProgressMonitor monitor) throws CoreException {
		List<String> commands = new ArrayList<>();
		commands.add("--install"); //$NON-NLS-1$
		commands.add(getBuildFolder().getLocation().toOSString());
		if (getCMakeProject().getBuildStatus()) {
			startCMakeProcess(commands, monitor, MESSAGE_STRING);
		}
		return new IProject[] { getProject() };
	}

}
