package su.softcom.cldt.internal.ui.properties;

import org.eclipse.ui.IWorkbenchPropertyPage;

import su.softcom.cldt.common.preferences.PreferenceConstants;

/**
 * Property page for build arguments.
 * 
 * @author Dmitrii Davladov - creator
 */
public class LinkerPropertyPage extends AbstractOptionPropertyPage implements IWorkbenchPropertyPage {

	public LinkerPropertyPage() {
		super(PreferenceConstants.LINKER_ARGUMENTS, PreferenceConstants.LINKER_DEFENITIONS);
	}
}
