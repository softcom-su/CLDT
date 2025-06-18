package su.softcom.cldt.ui;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Map;

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.lsp4j.SymbolKind;
import org.osgi.framework.Bundle;

public class CLDTPluginImages {

	private static final String F_ROOT = ""; //$NON-NLS-1$
	private static final String F_SRC = "srcelem16"; //$NON-NLS-1$
	private static final String F_TGT = "tgt16"; //$NON-NLS-1$

	public static final IPath ICONS_PATH_16 = new Path("icons" + IPath.SEPARATOR + "view16"); //$NON-NLS-1$ //$NON-NLS-2$

	public static final ImageDescriptor CMAKE_ICON = createUnManaged(F_ROOT, "cmake.png"); //$NON-NLS-1$
	public static final ImageDescriptor OBFUC_SHIELD_ICON = createUnManaged(F_ROOT, "shield.png"); //$NON-NLS-1$
	// Target types
	public static final ImageDescriptor TARGETS_FOLDER_ICON = createUnManaged(F_TGT, "build.png"); //$NON-NLS-1$
	public static final ImageDescriptor EXECUTABLE_ICON = createUnManaged(F_TGT, "exec.png"); //$NON-NLS-1$
	public static final ImageDescriptor SHARED_LIB_ICON = createUnManaged(F_TGT, "internal_libraries.png"); //$NON-NLS-1$
	public static final ImageDescriptor STATIC_LIB_ICON = createUnManaged(F_TGT, "lib.png"); //$NON-NLS-1$
	// Source elements
	public static final ImageDescriptor C_FILE_ICON = createUnManaged(F_SRC, "C_FILE.png"); //$NON-NLS-1$
	public static final ImageDescriptor FUNC_ICON = createUnManaged(F_SRC, "function.png"); //$NON-NLS-1$
	public static final ImageDescriptor CLASS_ICON = createUnManaged(F_SRC, "class.png"); //$NON-NLS-1$
	public static final ImageDescriptor METHOD_ICON = createUnManaged(F_SRC, "Method.png"); //$NON-NLS-1$
	public static final ImageDescriptor VAR_ICON = createUnManaged(F_SRC, "LocalVariable.png"); //$NON-NLS-1$
	public static final ImageDescriptor FIELD_ICON = createUnManaged(F_SRC, "Field.png"); //$NON-NLS-1$
	public static final ImageDescriptor NAMECAPSE_ICON = createUnManaged(F_SRC, "Namespace.png"); //$NON-NLS-1$
	public static final ImageDescriptor PROPERTY_ICON = createUnManaged(F_SRC, "Property.png"); //$NON-NLS-1$
	public static final ImageDescriptor STRING_ICON = createUnManaged(F_SRC, "String.png"); //$NON-NLS-1$
	public static final ImageDescriptor CONST_ICON = createUnManaged(F_SRC, "Constant.png"); //$NON-NLS-1$

	public static final Map<SymbolKind, ImageDescriptor> symbolKindImageMap = Map.ofEntries(
			Map.entry(SymbolKind.Function, FUNC_ICON), Map.entry(SymbolKind.Variable, VAR_ICON),
			Map.entry(SymbolKind.Class, CLASS_ICON), Map.entry(SymbolKind.Struct, CLASS_ICON),
			Map.entry(SymbolKind.Method, METHOD_ICON), Map.entry(SymbolKind.Constructor, METHOD_ICON),
			Map.entry(SymbolKind.Namespace, NAMECAPSE_ICON), Map.entry(SymbolKind.Property, PROPERTY_ICON),
			Map.entry(SymbolKind.String, STRING_ICON), Map.entry(SymbolKind.Constant, CONST_ICON),
			Map.entry(SymbolKind.Field, FIELD_ICON));

	/*
	 * Creates an image descriptor for the given prefix and name in the JDT UI
	 * bundle. The path can contain variables like $NL$. If no image could be found,
	 * <code>useMissingImageDescriptor</code> decides if either the 'missing image
	 * descriptor' is returned or <code>null</code>. or <code>null</code>.
	 */
	private static ImageDescriptor create(String prefix, String name, boolean useMissingImageDescriptor) {
		IPath path = ICONS_PATH_16.append(prefix).append(name);
		return createImageDescriptor(CMakeUIPlugin.getDefault().getBundle(), path, useMissingImageDescriptor);
	}

	/*
	 * Creates an image descriptor for the given prefix and name in the JDT UI
	 * bundle. The path can contain variables like $NL$. If no image could be found,
	 * the 'missing image descriptor' is returned.
	 */
	private static ImageDescriptor createUnManaged(String prefix, String name) {
		return create(prefix, name, true);
	}

	/*
	 * Creates an image descriptor for the given path in a bundle. The path can
	 * contain variables like $NL$. If no image could be found,
	 * <code>useMissingImageDescriptor</code> decides if either the 'missing image
	 * descriptor' is returned or <code>null</code>
	 */
	public static ImageDescriptor createImageDescriptor(Bundle bundle, IPath path, boolean useMissingImageDescriptor) {
		IPath uriPath = new Path("/plugin").append(bundle.getSymbolicName()).append(path); //$NON-NLS-1$
		URL url = null;
		try {
			URI uri = new URI("platform", null, uriPath.toString(), null); //$NON-NLS-1$
			url = uri.toURL();
		} catch (MalformedURLException | URISyntaxException e) {
			// no image
		}
		URL foundUrl = FileLocator.find(url);
		if (foundUrl != null) {
			return ImageDescriptor.createFromURL(url);
		}
		if (useMissingImageDescriptor) {
			return ImageDescriptor.getMissingImageDescriptor();
		}
		return null;
	}

	private CLDTPluginImages() {
		// utility class
	}

}