package su.softcom.cldt.ui.views;

import org.eclipse.core.resources.IFolder;

import su.softcom.cldt.core.operations.ObfuscationOperation;

/**
 * View for obfuscation statistics.
 */
public class ObfuscationStatisticView extends AbstractObfuscationView {

	public static final String ID = "su.softcom.cldt.ui.views.obfstatistics"; //$NON-NLS-1$

	/**
	 * Refreshes viewer.
	 */
	@Override
	public void refresh() {
		if (folderForInfo != null && folderForInfo.exists()) {
			viewer.setInput(getInput(ObfuscationOperation.STAT_FILE_NAME));
		}
		viewer.refresh();
	}

	/**
	 * Sets folder for files with obfuscation info.
	 * 
	 * @param folderForInfo folder with obfuscation info
	 */
	public static void setFolderForInfo(IFolder folderForInfo) {
		ObfuscationStatisticView.folderForInfo = folderForInfo;
	}

	/**
	 * Creates columns of table viewer.
	 */
	@Override
	protected void createColumns() {
		createColumn("Характеристика", 0);
		createColumn("Значение", 1);
	}

}
