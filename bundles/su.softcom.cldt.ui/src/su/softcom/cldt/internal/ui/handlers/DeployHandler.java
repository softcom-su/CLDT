package su.softcom.cldt.internal.ui.handlers;

import org.eclipse.core.commands.AbstractHandler;
import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;

import su.softcom.cldt.core.preferences.PreferenceConstants;

public class DeployHandler extends AbstractHandler {

	public static final String ERROR_MESSAGE = "Запуск не удался";

	@Override
	public Object execute(ExecutionEvent event) throws ExecutionException {

		IEclipsePreferences scope = InstanceScope.INSTANCE.getNode("su.softcom.cldt.nativebuild");

		String script = scope.get(PreferenceConstants.NATIVEBUILD_DEPLOY_SCRIPT, "");

		HandlerLaunch handlerLaunch = new HandlerLaunch();
		try {
			handlerLaunch.scriptLaunch(script, null);
		} catch (CoreException e) {
			Platform.getLog(getClass()).error(e.getMessage());
			throw new ExecutionException(ERROR_MESSAGE);
		}

		return null;
	}

}
