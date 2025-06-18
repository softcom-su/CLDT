package su.softcom.cldt.debug.core;

import java.util.List;

/**
 * 
 * 
 * @see <a href="https://www.baeldung.com/linux/proc-id-maps"> usefull link </a>
 */
public interface IProcMapsFile {

	public interface IProcMapsEntry {

		/**
		 * "address start" – "address end" is the start and end address of that mapping.
		 * 
		 * @return address start
		 */
		public long getAddressStart();

		/**
		 * "address start" – "address end" is the start and end address of that mapping.
		 * 
		 * @return address end
		 */
		public long getAddressEnd();

		/**
		 * mode (permissions) specifies which actions are available on this mapping and
		 * if it’s private or shared.
		 * 
		 * @return modes string
		 */
		public String getModes();

		/**
		 * The file path of the file for that mapping. In the event that this is not a
		 * file mapping, that field is empty.
		 * 
		 * @return path name
		 */
		public String getPathName();
		
	}

	/**
	 * Возвращает все IProcMapsEntry
	 * 
	 * @return
	 */
	public List<IProcMapsEntry> getProcMapsEntries();

	/**
	 * Возвращает IProcMapsEntry по строке пути
	 * 
	 * @param name
	 * @return
	 */
	public List<IProcMapsEntry> getProcMapsEntries(String path);

}
