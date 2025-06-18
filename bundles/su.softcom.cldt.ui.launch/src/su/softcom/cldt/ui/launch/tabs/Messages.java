package su.softcom.cldt.ui.launch.tabs;

import org.eclipse.osgi.util.NLS;

public class Messages extends NLS {

	private static final String BUNDLE_NAME = "su.softcom.cldt.ui.launch.tabs.messages"; //$NON-NLS-1$

	public static String BashMainTab_Script_Group;
	public static String BashMainTab_Search_Button;
	public static String BashMainTab_Selection_Message;
	public static String BashMainTab_Selection_Title;
	public static String BashMainTab_Wrong_Script;

	public static String EnvTab_Import_Button;
	public static String EnvTab_Intersection_Message;
	public static String EnvTab_Intersection_Title;
	public static String EnvTab_Merge_Button;
	public static String EnvTab_Replace_Button;

	public static String ImportDialog_amd64;
	public static String ImportDialog_amd64_arm;
	public static String ImportDialog_amd64_arm64;
	public static String ImportDialog_amd64_x86;
	public static String ImportDialog_Architecture_Label;
	public static String ImportDialog_Browse_Button;
	public static String ImportDialog_Import_Title;
	public static String ImportDialog_MSVC;
	public static String ImportDialog_Script_Label;
	public static String ImportDialog_User_Script;
	public static String ImportDialog_Variables_Label;
	public static String ImportDialog_x86;
	public static String ImportDialog_x86_amd64;
	public static String ImportDialog_x86_arm;
	public static String ImportDialog_x86_arm64;

	public static String MainTab_Arguments_Group;
	public static String MainTab_Browse_Button;
	public static String MainTab_Name;
	public static String MainTab_Project_Group;
	public static String MainTab_Project_Not_Exist;
	public static String MainTab_Project_Not_Specified;
	public static String MainTab_Selection_Message;
	public static String MainTab_Selection_Title;

	static {
		// initialize resource bundle
		NLS.initializeMessages(BUNDLE_NAME, Messages.class);
	}

	private Messages() {
	}
}
