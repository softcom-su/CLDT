package su.softcom.cldt.internal.ui.handlers;

import org.eclipse.osgi.util.NLS;

public class Messages extends NLS {
	private static final String BUNDLE_NAME = Messages.class.getPackageName() + ".messages"; //$NON-NLS-1$
	public static String TechnologicalHandler_ErrorsDuringTranslation;
	public static String TechnologicalHandler_TechnologicalBuild;
	public static String TechnologicalHandler_TechnologicalTranslation;
	public static String TechnologicalHandler_TranslationCompletedSuccessfully;
	static {
		// initialize resource bundle
		NLS.initializeMessages(BUNDLE_NAME, Messages.class);
	}

	private Messages() {
	}
}
