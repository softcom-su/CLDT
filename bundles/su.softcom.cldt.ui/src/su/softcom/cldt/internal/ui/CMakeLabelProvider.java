package su.softcom.cldt.internal.ui;

import java.util.HashMap;
import java.util.Map;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.viewers.ILabelProvider;
import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.Resource;

import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.core.source.ISourceElement;
import su.softcom.cldt.core.source.SourceElement;
import su.softcom.cldt.core.source.SourceFile;
import su.softcom.cldt.ui.CLDTPluginImages;

public class CMakeLabelProvider implements ILabelProvider {

	private Map<ImageDescriptor, Image> iconImageMap = new HashMap<>();

	@Override
	public void addListener(ILabelProviderListener listener) {
		// no
	}

	@Override
	public void dispose() {
		iconImageMap.values().stream().forEach(Resource::dispose);
	}

	@Override
	public boolean isLabelProperty(Object element, String property) {
		return false;
	}

	@Override
	public void removeListener(ILabelProviderListener listener) {
		//
	}

	@Override
	public Image getImage(Object element) {
		if (element instanceof CMakeTargetsFolder) {
			return getIconImage(CLDTPluginImages.TARGETS_FOLDER_ICON);
		}
		if (element instanceof SourceElement srcElem) {
			return getIconImage(CLDTPluginImages.symbolKindImageMap.get(srcElem.getSymbolKind()));
		}
		if (element instanceof Target target) {
			switch (target.getType()) {
			case EXECUTABLE:
				return getIconImage(CLDTPluginImages.EXECUTABLE_ICON);
			case SHARED_LIBRARY:
				return getIconImage(CLDTPluginImages.SHARED_LIB_ICON);
			default:
				return getIconImage(CLDTPluginImages.STATIC_LIB_ICON);
			}
		}
		if (element instanceof SourceFile) {
			return getIconImage(CLDTPluginImages.C_FILE_ICON);
		}
		return null;
	}

	private Image getIconImage(ImageDescriptor desc) {
		return iconImageMap.computeIfAbsent(desc, ImageDescriptor::createImage);
	}

	@Override
	public String getText(Object element) {
		if (element instanceof Target target) {
			return target.getName();
		} else if (element instanceof SourceFile source) {
			return source.getName();
		}
		if (element instanceof IFile file) {
			return file.getName();
		}
		if (element instanceof IFolder folder) {
			return folder.getName();
		}
		if (element instanceof SourceFile src) {
			return src.getName() + "%s".formatted(src.getTargets()); //$NON-NLS-1$
		}
		if (element instanceof ISourceElement elem) {
			return elem.getName();
		}
		return element.toString();
	}

}
