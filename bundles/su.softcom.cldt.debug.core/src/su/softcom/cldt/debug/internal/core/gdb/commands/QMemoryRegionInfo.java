package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.HexFormat;

import su.softcom.cldt.debug.core.IGdbCommand;
import su.softcom.cldt.debug.core.Utils;
import su.softcom.cldt.debug.internal.core.DebugTarget;

/**
 * qMemoryRegionInfo: command implementation. Get information about the address
 * range that contains {@link QMemoryRegionInfo#address address}
 * 
 * @author rinat.nizamov@softcom.su
 */
public class QMemoryRegionInfo extends GDBCommand{

	private long start;
	private long size;
	private String permissions;
	private String flags;
	private String name;
	private long address;
	
	
	public QMemoryRegionInfo(long address) {
		super("qMemoryRegionInfo:%s".formatted(Utils.removeLeadZero(HexFormat.of().toHexDigits(address))));
		this.address = address;
	}

	@Override
	protected void doHandleResult(byte[] result) {
		String resultStr = new String(result);
		String[] arrayStr = resultStr.split(";");
		for (String entry: arrayStr) {
			
			String[] entrySplit = entry.split(":");
			String field = "";
			String value = "";
			if (entrySplit.length > 1){
				field = entry.split(":")[0];
				value = entry.split(":")[1];
			}
			switch (field) {
				case "start":
					start = Long.parseLong(value,16);
					break;
				case "size":
					size = Long.parseLong(value,16);
					break;
				case "permissions":
					permissions = value;
					break;
				case "flags":
					flags = value;
					break;
				case "name":
					name = value;
					break;
			}
		}
	}

	public long getStart() {
		return start;
	}


	public long getSize() {
		return size;
	}


	public String getPerms() {
		return permissions;
	}


	public String getName() {
		return name;
	}

	public String getFlags() {
		return flags;
	}

}
