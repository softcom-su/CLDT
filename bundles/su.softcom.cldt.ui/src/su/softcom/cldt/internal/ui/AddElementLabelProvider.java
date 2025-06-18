package su.softcom.cldt.internal.ui;

import java.net.URL;

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.resource.FontDescriptor;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.resource.JFaceResources;
import org.eclipse.jface.resource.LocalResourceManager;
import org.eclipse.jface.resource.ResourceManager;
import org.eclipse.jface.viewers.CellLabelProvider;
import org.eclipse.jface.viewers.ILabelProvider;
import org.eclipse.jface.viewers.ViewerCell;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.Image;
import org.osgi.framework.Bundle;
import org.osgi.framework.FrameworkUtil;

public class AddElementLabelProvider extends CellLabelProvider implements ILabelProvider {

	final Image elementIcon;
	static final FontDescriptor NEW_ELEMENT_FONT_DESCRIPTOR = FontDescriptor.createFrom(JFaceResources.getDefaultFont())
			.setStyle(SWT.ITALIC | SWT.BOLD);
	static final FontDescriptor DEFAULT_FONT_DESCRIPTOR = FontDescriptor.createFrom(JFaceResources.getDefaultFont())
			.setStyle(SWT.NONE);

	static final Image IMAGE_ADD = getResourceManager().createImage(createImageDescriptor("add.png")); //$NON-NLS-1$
	private Font font;

	public AddElementLabelProvider(Image elementImage) {
		elementIcon = elementImage;
	}

	private static ResourceManager resourceManager;

	protected static ResourceManager getResourceManager() {
		if (resourceManager == null) {
			resourceManager = new LocalResourceManager(JFaceResources.getResources());
		}
		return resourceManager;
	}

	static ImageDescriptor createImageDescriptor(String fileName) {
		Bundle bundle = FrameworkUtil.getBundle(AddElementLabelProvider.class);
		URL url = FileLocator.find(bundle, new Path("icons" + IPath.SEPARATOR + "view16" + IPath.SEPARATOR + fileName), //$NON-NLS-1$//$NON-NLS-2$
				null);
		return ImageDescriptor.createFromURL(url);
	}

	@Override
	public void update(ViewerCell cell) {
		Object element = cell.getElement();
		cell.setText(getText(element));
		Image image = getImage(element);
		cell.setImage(image);
		FontDescriptor fDesc;
		if (element instanceof AddElementContentProvider.AddNewElement) {
			fDesc = NEW_ELEMENT_FONT_DESCRIPTOR;
		} else {
			fDesc = DEFAULT_FONT_DESCRIPTOR;
		}
		font = fDesc.createFont(cell.getItem().getDisplay());
		cell.setFont(font);
		fDesc.destroyFont(font);
	}

	@Override
	public String getText(Object element) {
		return element == null ? "" : element.toString();//$NON-NLS-1$
	}

	@Override
	public Image getImage(Object element) {
		if (element instanceof AddElementContentProvider.AddNewElement) {
			return IMAGE_ADD;
		}
		return elementIcon;
	}

	@Override
	public void dispose() {
		super.dispose();
		font.dispose();
	}

}
