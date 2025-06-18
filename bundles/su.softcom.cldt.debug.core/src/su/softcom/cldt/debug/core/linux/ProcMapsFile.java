package su.softcom.cldt.debug.core.linux;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import su.softcom.cldt.debug.core.IProcMapsFile;

public class ProcMapsFile implements IProcMapsFile {

	private class ProcMapsEntry implements IProcMapsEntry {

		private final long startAddress;
		private final long endAddress;
		private final String modes;
		private final String pathName;
		
		public ProcMapsEntry(String line) {
			String[] columns = Arrays.stream(line.split(" ")).filter(x -> !x.isBlank()).toArray(String[]::new);
			String[] addressStrs = columns[0].split("-");
			this.startAddress = new BigInteger(addressStrs[0], 16).longValue();
			this.endAddress = new BigInteger(addressStrs[1], 16).longValue();
			this.modes = columns[1];
			this.pathName = columns.length > 5 ? columns[5] : "";

		}
		
		@Override
		public long getAddressStart() {
			return startAddress;
		}

		@Override
		public long getAddressEnd() {
			return endAddress;
		}

		@Override
		public String getModes() {
			return modes;
		}

		@Override
		public String getPathName() {
			return pathName;
		}

		@Override
		public String toString() {
			StringBuilder sb = new StringBuilder();
			sb.append(this.startAddress+ "-" + this.endAddress);
			sb.append(" ");
			sb.append(modes);
			sb.append(" ");
			sb.append(pathName);
			return sb.toString();
		}
	}

	private List<IProcMapsEntry> entries = new ArrayList<>();

	public ProcMapsFile(byte[] rawBytesFile) {
		String resStr = new String(rawBytesFile);
		String[] procMapsLines = resStr.split(System.lineSeparator());
		for (String line: procMapsLines) {
			IProcMapsEntry entry = new ProcMapsEntry(line);
			entries.add(entry);
		}
	}
	
	@Override
	public List<IProcMapsEntry> getProcMapsEntries() {
		return entries;
	}

	@Override
	public List<IProcMapsEntry> getProcMapsEntries(String path) {
		return entries.stream().filter(entry -> entry.getPathName().contains(path)).toList();
	}
	
}
