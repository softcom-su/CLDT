package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.Arrays;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.model.IMemoryBlock;
import su.softcom.cldt.debug.core.ELF;
import su.softcom.cldt.debug.core.IProcMapsFile;
import su.softcom.cldt.debug.core.IProcMapsFile.IProcMapsEntry;
import su.softcom.cldt.debug.core.linux.ProcMapsFile;
import su.softcom.cldt.debug.internal.core.MemoryBlock;

/**
 * 
 * Read data from an opened file descriptor.
 * <p>
 * 
 * @author rinat.nizamov@softcom.su
 */
public class VFilePreadCommand extends AbstractVFileCommand {

	private ELF elf = null;
	private IProcMapsFile procMapsFile = null;
	private IMemoryBlock stack;
	private long mainInitAdress = 0;
	private FileDescriptor fileDescriptor;
	private Pattern pattern = Pattern.compile("/proc/.+/maps");

	/**
	 * File Descriptor class.
	 * 
	 * @see https://en.wikipedia.org/wiki/File_descriptor
	 */
	public static class FileDescriptor {

		private long fs;
		private String path;

		/**
		 * @param fs
		 * @param path
		 */
		public FileDescriptor(long fs, String path) {
			this.fs = fs;
			this.path = path;
		}

		/**
		 * @return fs value
		 */
		public long getFs() {
			return fs;
		}

		/**
		 * @return path value
		 */
		public String getPath() {
			return path;
		}

	}

	/**
	 * @param fileDescriptor
	 * @param size
	 * @param offset
	 */
	public VFilePreadCommand(FileDescriptor fileDescriptor, long size, long offset) {
		super("pread", "%s,%s,%s".formatted(Long.toHexString(fileDescriptor.getFs()), Long.toHexString(size),
				Long.toHexString(offset)));
		this.fileDescriptor = fileDescriptor;
	}

	@Override
	protected void doHandleResult(byte[] packet) {
		if ((char) packet[0] == 'F') {
			int resOffset = 0;
			for (byte resByte : packet) {
				resOffset++;
				if (resByte == 59) {
					break;
				}
			}
			Matcher matcher = pattern.matcher(fileDescriptor.getPath());
			byte[] resBytes = Arrays.copyOfRange(packet, resOffset, packet.length);
			if (resBytes[0] == 127) {
				elf = new ELF(resBytes);
			} else if (matcher.find()) {
				procMapsFile = new ProcMapsFile(resBytes);
				readInitAddress();
				readStackBlock();
			}
		} else {
			Platform.getLog(getClass()).warn("Not an F packet resived");
		}
	}

	private void readInitAddress() {
		List<IProcMapsEntry> entries= procMapsFile.getProcMapsEntries();
		if (!entries.isEmpty()) {
			IProcMapsEntry firstEntry = entries.get(0);
			mainInitAdress = firstEntry.getAddressStart();
		}
	}

	private void readStackBlock() {
		List<IProcMapsEntry> stacksList= procMapsFile.getProcMapsEntries("[stack]");
		if (!stacksList.isEmpty()) {
			IProcMapsEntry stackFirstEntry = stacksList.get(0);
			long length = stackFirstEntry.getAddressEnd() - stackFirstEntry.getAddressStart();
			this.stack = new MemoryBlock(null, stackFirstEntry.getAddressStart(), length);
		}
	}

	/**
	 * @return elf
	 */
	public ELF getElfFile() {
		Assert.isNotNull(elf);
		return elf;
	}

	/**
	 * @return stack
	 */
	public IMemoryBlock getStackMemoryBlock() {
		Assert.isNotNull(stack);
		return stack;
	}

	/**
	 * @return mainInitAdress
	 */
	public long getInitAdress() {
		Assert.isTrue(mainInitAdress != 0);
		return mainInitAdress;
	}
	
	/**
	 * @return procMapsFile
	 */
	public IProcMapsFile getProcMapsFile() {
		Assert.isNotNull(procMapsFile);
		return  procMapsFile;
	}

}
