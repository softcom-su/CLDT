package su.softcom.cldt.internal.ui.properties;

import org.eclipse.ui.IWorkbenchPropertyPage;

import su.softcom.cldt.common.preferences.PreferenceConstants;

/**
 * Property page for build arguments.
 * 
 * @author Dmitrii Davladov - creator
 * @author Vlaislav Ershov - update code style
 * @author Petr Babanov
 */
public class CompilerPropertyPage extends AbstractOptionPropertyPage implements IWorkbenchPropertyPage {

	public CompilerPropertyPage() {
		super(PreferenceConstants.COMPILER_ARGUMENTS, PreferenceConstants.COMPILER_DEFINITIOINS);
	}
}
