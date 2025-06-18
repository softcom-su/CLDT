package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HexFormat;
import java.util.List;
import java.util.stream.Collectors;


/**
 * qSupported command implementation. 
 * 
 * @author rinat.nizamov@softcom.su
 */
public class QSupported extends GDBCommand {

	
	private List<String> features;
	private long packetSize;
	
	/**
	 * Query the GDB remote server for features it supports
	 * @param client
	 * @param features
	 */
	public QSupported(List<String> features) {
		super("qSupported%s".formatted(
				features.isEmpty() ? "" : ":%s".formatted(features.stream().collect(Collectors.joining(";")))));
	}

	public QSupported() {
		super("qSupported");
	}
	
	@Override
	public void doHandleResult(byte[] packet) {
		String strResult = new String(packet);
		configure(strResult);
	}

	/**
	 * Awaits cleared packet string to configure available server features
	 */
	private void configure(String allFeatures) {
		List<String> result = new ArrayList<>();
		for (String feature : allFeatures.split(";")) {
			if (feature.contains("PacketSize")) {
				setPacketSize(feature);
				continue;
			} else {
				if(feature.endsWith("+")) {
					result.add(feature.substring(0, feature.length() -1));
				}
			}
		}
		setFeatures(Collections.unmodifiableList(result));
	}
	
	private void setFeatures(List<String> features) {
		this.features = features;
	}

	private void setPacketSize(String str) {
		packetSize = HexFormat.fromHexDigits(str.split("=")[1]);
	}

	public long getPacketSize() {
		return packetSize;
	}
	
	public List<String> getFeatures() {
		return features;
		
	}
	


}
