package su.softcom.cldt.ui;

import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.Plugin;
import org.eclipse.swt.widgets.Display;
import org.osgi.service.component.annotations.Activate;
import org.osgi.service.component.annotations.Component;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.internal.ui.console.cmake.CMakeProcessListener;

@Component
public class CMakeUIPlugin extends Plugin {

	public static final String PLUGIN_ID = "su.softcom.cldt.ui"; //$NON-NLS-1$
	public static CMakeUIPlugin eINSTANCE = null;

	/**
	 * Returns the standard display to be used. The method first checks, if the
	 * thread calling this method has an associated display. If so, this display is
	 * returned. Otherwise the method returns the default display.
	 */
	public static Display getStandardDisplay() {
		Display display = Display.getCurrent();
		if (display == null) {
			display = Display.getDefault();
		}
		return display;
	}

	public CMakeUIPlugin() {
		eINSTANCE = this;
	}

	public static CMakeUIPlugin getDefault() {
		if (eINSTANCE == null) {
			synchronized (eINSTANCE) {
				if (eINSTANCE == null) {
					eINSTANCE = new CMakeUIPlugin();
				}
			}
		}
		return eINSTANCE;
	}

	/**
	 * Method to activate with validation manager.
	 * 
	 */
	@Activate
	public void start() {
		CMakeCorePlugin.getCMakeBuildMonitor().addProcesListener(new CMakeProcessListener());
		Platform.getLog(getClass()).info("CLDT UI plugin activated"); //$NON-NLS-1$
	}

}
