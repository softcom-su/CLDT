package su.softcom.cldt.internal.ui.preferences;

import org.eclipse.osgi.util.NLS;

/**
 * Messages for internal prefernces.
 * 
 * @author ershov_vladislav@softcom.su
 */
public class Messages extends NLS {
	private static final String BUNDLE_NAME = Messages.class.getPackageName() + ".messages"; //$NON-NLS-1$
	public static String ArgumentsPropertyPage_ProgramCallParameters;
	public static String BuildFolderFieldEditor_ChooseFolder;
	public static String BuildFolderFieldEditor_ChooseFolderForBuild;
	public static String BuildFolderFieldEditor_ChooseNotFolder;
	public static String BuildFolderFieldEditor_FolderNotFound;
	public static String BuildFolderSelectionDialog_InvalidFolderName;
	public static String BuildFolderFieldEditor_PathMustStartWith;
	public static String BuildFolderSelectionDialog_CreateFolder;
	public static String BuildFolderSelectionDialog_NoRecordsAvailable;
	public static String CompilerPropertyPage_BuildFolder;
	public static String CompilerPropertyPage_LibTypeDynamic;
	public static String CompilerPropertyPage_LibTypeStatic;
	public static String CompilerPropertyPage_LibTypeObject;
	public static String CompilerPropertyPage_LibTypeInterface;
	public static String CompilerPropertyPage_LibTypeModule;
	public static String CompilerPropertyPage_BuildDebug;
	public static String CompilerPropertyPage_BuildMinSizeRel;
	public static String CompilerPropertyPage_BuildRelease;
	public static String CompilerPropertyPage_BuildRelWithDebInfo;
	public static String CompilerPropertyPage_FolderBuild;
	public static String CompilerPropertyPage_PerformanceBuild;
	public static String CMakePreferences_DisableCMakeTest;
	public static String CreateFolderDialog_CreationFolder;
	public static String CreateFolderDialog_EnterFolderName;
	public static String CreateFolder_InvalidFolderName;
	public static String ExecutionFileFieldEditor_NotEnteredFileName;
	public static String ExecutionFileFieldEditor_UncorrectFileName;
	public static String LinkerPreferencePage_LinkerArguments;
	static {
		// initialize resource bundle
		NLS.initializeMessages(BUNDLE_NAME, Messages.class);
	}

	private Messages() {
	}
}
