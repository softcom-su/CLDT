package su.softcom.cldt.ui.views;

import org.eclipse.core.resources.IFolder;

import su.softcom.cldt.core.operations.ObfuscationOperation;

/**
 * View for obfuscation log.
 */
public class ObfuscationLogView extends AbstractObfuscationView {

	public static final String ID = "su.softcom.cldt.ui.views.obflog"; //$NON-NLS-1$

	/**
	 * Refreshes viewer.
	 */
	@Override
	public void refresh() {
		if (folderForInfo != null && folderForInfo.exists()) {
			viewer.setInput(getInput(ObfuscationOperation.LOG_FILE_NAME));
		}
		viewer.refresh();
	}

	/**
	 * Sets folder for files with obfuscation info.
	 * 
	 * @param folderForInfo folder with obfuscation info
	 */
	public static void setFolderForInfo(IFolder folderForInfo) {
		ObfuscationLogView.folderForInfo = folderForInfo;
	}

	/**
	 * Creates columns of table viewer.
	 */
	@Override
	protected void createColumns() {
		createColumn("Функция", 0);
		createColumn("Кол-во участков до оптимизации", 1);
		createColumn("Кол-во участков после оптимизации", 2);
		createColumn("Кол-во машинных команд до оптимизации", 3);
		createColumn("Кол-во машинных команд после оптимизации", 4);
		createColumn("Кол-во внешних дуг до оптимизации", 5);
		createColumn("Кол-во внешних дуг после оптимизации", 6);
	}

}
