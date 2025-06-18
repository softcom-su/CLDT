package su.softcom.cldt.core.operations;

import java.util.HashMap;
import java.util.Map;

/**
 * Provider for localization clang output markers.
 */
public class MessageProvider {

	private static final Map<String, String> LocalizedMessages = new HashMap<>();

	static {
		LocalizedMessages.put("core.uninitialized.Branch", Messages.ClangMarkerID0); //$NON-NLS-1$
	}

	/**
	 * Get localized message for passed clang problem id. Returns default value if
	 * message doesn't localized yet.
	 * 
	 * @param clangID
	 * @param defaultValue
	 * @return
	 */
	public static String getMessage(String clangID, String defaultValue) {
		return LocalizedMessages.containsKey(clangID) ? LocalizedMessages.get(clangID) : defaultValue;
	}

}
