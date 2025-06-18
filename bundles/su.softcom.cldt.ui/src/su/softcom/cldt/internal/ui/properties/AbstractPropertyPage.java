package su.softcom.cldt.internal.ui.properties;

import java.net.URL;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.JFaceResources;
import org.eclipse.jface.resource.LocalResourceManager;
import org.eclipse.jface.resource.ResourceManager;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.eclipse.ui.dialogs.PropertyPage;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;
import org.osgi.service.prefs.BackingStoreException;

import su.softcom.cldt.internal.ui.AddElementLabelProvider;

public class AbstractPropertyPage extends PropertyPage implements IWorkbenchPropertyPage {

	protected IProject project;
	protected IEclipsePreferences store;
	protected IEclipsePreferences defaults;
	private static ResourceManager resourceManager;

	static ImageDescriptor createImageDescriptor(String fileName) {
		Bundle bundle = FrameworkUtil.getBundle(AddElementLabelProvider.class);
		URL url = FileLocator.find(bundle, new Path("icons" + IPath.SEPARATOR + "view16" + IPath.SEPARATOR + fileName), //$NON-NLS-1$//$NON-NLS-2$
				null);
		return ImageDescriptor.createFromURL(url);
	}

	protected static ResourceManager getResourceManager() {
		if (resourceManager == null) {
			resourceManager = new LocalResourceManager(JFaceResources.getResources());
		}
		return resourceManager;
	}

	protected Composite createCommon(Composite parent) {
		parent.setLayout(new GridLayout(1, false));

		GridData commonData = new GridData();
		commonData.grabExcessVerticalSpace = true;
		commonData.verticalAlignment = SWT.FILL;
		commonData.grabExcessHorizontalSpace = true;
		commonData.horizontalAlignment = SWT.FILL;

		Composite common = new Composite(parent, SWT.None);
		common.setLayout(new GridLayout(1, false));
		common.setLayoutData(commonData);

		return common;
	}

	@Override
	protected Control createContents(Composite parent) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean performOk() {
		try {
			store.flush();
		} catch (BackingStoreException e) {
			e.printStackTrace();
			return false;
		}

		return true;
	}

}
