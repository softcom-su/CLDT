package su.softcom.cldt.ui.wizards;

import org.eclipse.osgi.util.NLS;

@Deprecated
public class Messages extends NLS {
	private static final String BUNDLE_NAME = Messages.class.getPackageName() + ".messages"; //$NON-NLS-1$
	public static String AbstractNewFileWizard_FileCreation;
	public static String AbstractNewFileWizardPage_Browse;
	public static String AbstractNewFileWizardPage_ChooseFolder;
	public static String AbstractNewFileWizardPage_ChooseProject;
	public static String AbstractNewFileWizardPage_EnterFileExtension;
	public static String AbstractNewFileWizardPage_EnterFileName;
	public static String AbstractNewFileWizardPage_FileCreationFolder;
	public static String AbstractNewFileWizardPage_FileName;
	public static String AbstractNewFileWizardPage_FolderSelection;
	public static String AbstractNewFileWizardPage_IncorrectFileName;
	public static String AbstractNewFileWizardPage_NoFolderSelected;
	public static String AbstractNewFileWizardPage_NoProjectSelected;
	public static String AbstractNewFileWizardPage_Project;
	public static String AbstractNewFileWizardPage_ProjectSelection;
	public static String AbstractNewFileWizardPage_SelectFolder;
	public static String AbstractNewFileWizardPage_SelectProject;
	public static String NewHeaderFileWizard_NewHeaderFile;
	public static String NewSourceFileWizard_NewSourceFile;
	public static String NewHeaderFileWizardPage_PossibleFileExtensions;
	public static String NewSourceFileWizardPage_PossibleFileExtensions;
	public static String OptimizationWizardPage_NotSelectedFiles;
	public static String OptimizationWizardPage_SelectFiles;
	static {
		// initialize resource bundle
		NLS.initializeMessages(BUNDLE_NAME, Messages.class);
	}

	private Messages() {
	}
}
