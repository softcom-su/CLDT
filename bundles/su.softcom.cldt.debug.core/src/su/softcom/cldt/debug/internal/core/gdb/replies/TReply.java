package su.softcom.cldt.debug.internal.core.gdb.replies;

import java.math.BigInteger;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HexFormat;
import java.util.List;
import java.util.Map;
import org.eclipse.core.runtime.Platform;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import su.softcom.cldt.debug.core.IGDBReply;
import su.softcom.cldt.debug.core.Utils;

/**
 * Class TReply represents a T reply packet from the LLDB server and processes
 * it accordingly.
 * 
 * @author maxim.drumov@softcom.su
 */
public class TReply implements IGDBReply {

	public static class StopInfo {

		private String name;
		private String reason;
		private int signal;
		private long tid;

		public long getTid() {
			return tid;
		}

		public void setTid(long tid) {
			this.tid = tid;
		}

		public int getSignal() {
			return signal;
		}

		public void setSignal(int signal) {
			this.signal = signal;
		}

		public String getReason() {
			return reason;
		}

		public void setReason(String reason) {
			this.reason = reason;
		}

		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}

	}

	Map<Integer, BigInteger> registers;

	private long stoppedThreadId;
	private List<Long> threadIds = new ArrayList<>();
	private List<Long> threadPcs = new ArrayList<>();
	private List<StopInfo> stopInfos = new ArrayList<>();
	private String core;
	private String reason;
	private String errorDescription;
	private ByteOrder order = ByteOrder.BIG_ENDIAN;

	/**
	 * Default constructor initializes the TReply with empty values.
	 */
	public TReply() {
		this.registers = new HashMap<>();
		this.stoppedThreadId = 0;
		this.core = null;
		this.reason = null;
	}

	/**
	 * Handles the reply string from LLDB server by parsing and storing relevant
	 * information.
	 * 
	 * @param inputStr the input string to be parsed
	 */
	@Override
	public void handleReply(String inputStr) {
		String dataPart = inputStr.substring(3);// skip T sig
		for (String pair : dataPart.split(";")) {
			String[] keyValue = pair.split(":");
			if (keyValue.length == 2) {
				String key = keyValue[0].trim();
				String value = keyValue[1].trim();
				if (isTwoDigitHexadecimal(key)) {
					readSingleRegister(key, value);
				} else if (key.equals("core")) {
					setCore(value);
				} else if (key.equals("thread")) {
					setStopedThread(Long.parseLong(value, 16));
				} else if (key.equals("threads")) {
					setThreads(value);
				} else if (key.equals("jstopinfo")) {
					parseStopInfoJson(value);
				} else if (key.equals("thread-pcs")) {
					setThreadPcs(value);
				} else if (key.equals("reason")) {
					setReason(value);
				} else if (key.equals("description")) {
					setErrorDescriprion(value);
				}
			}
		}
	}

	private void readSingleRegister(String key, String value) {
		BigInteger regValue;
		if (order == ByteOrder.BIG_ENDIAN) {
			regValue = new BigInteger(value, 16);
		} else {
			regValue = BigInteger.valueOf(Long.reverseBytes(new BigInteger(value, 16).longValue()));
		}
		registers.put(Integer.parseInt(key, 16), regValue);
	}

	private void parseStopInfoJson(String value) {
		String jsonInfo = Utils.hexToAscii(value);
		ObjectMapper mapper = new ObjectMapper();
		try {
			JsonNode arrayNode = mapper.readTree(jsonInfo);
			for (JsonNode node : arrayNode) {
				StopInfo stopInfo = mapper.treeToValue(node, StopInfo.class);
				getStopInfos().add(stopInfo);
			}
		} catch (JsonProcessingException e) {
			e.printStackTrace();
		}
	}

	@Override
	public ReplyType getReplyType() {
		return ReplyType.T_REPLY;
	}

	/**
	 * Checks if a given input string is a two-digit hexadecimal number.
	 * 
	 * @param input the input string to be checked
	 * @return true if the input is a two-digit hexadecimal number, false otherwise
	 */
	private boolean isTwoDigitHexadecimal(String input) {
		String regex = "^[0-9A-Fa-f]{2}$";
		return input.matches(regex);
	}

	/**
	 * Prints the stored information in a formatted log message.
	 */
	public void print() {
		StringBuilder logMessage = new StringBuilder();

		if (stoppedThreadId != 0) {
			logMessage.append("Stopped thread: ").append(stoppedThreadId).append("\n");
		}
		if (!threadIds.isEmpty()) {
			logMessage.append("All thread IDs: ").append(threadIds).append("\n");
		}
		if (!threadPcs.isEmpty()) {
			logMessage.append("Thread-pcs: ").append(threadPcs).append("\n");
		}
		if (core != null) {
			logMessage.append("Core number: ").append(core).append("\n");
		}
		if (reason != null) {
			logMessage.append("Reason: ").append(reason).append("\n");
		}

		Platform.getLog(getClass()).info(logMessage.toString());
	}

	/**
	 * Converts the registers map to a formatted string.
	 * 
	 * @return a formatted string representing the registers
	 */
	public String registersToString() {
		StringBuilder strBuilder = new StringBuilder();
		for (int i = 0; i < registers.size(); i++) {
			strBuilder.append(String.format("%-2s = %d%n", i, registers.get(i)));
		}
		return strBuilder.toString();
	}

	public ByteOrder getOrder() {
		return order;
	}

	public void setOrder(ByteOrder order) {
		this.order = order;
	}

	public String getReason() {
		return reason;
	}

	private void setReason(String reason) {
		this.reason = reason;
	}

	public String getCore() {
		return core;
	}

	private void setCore(String core) {
		this.core = core;
	}

	private void setErrorDescriprion(String desc) {
		this.errorDescription = new String(HexFormat.of().parseHex(desc));
	}

	public List<Long> getAllThreadIds() {
		return threadIds;
	}

	private void setThreads(String value) {
		String[] threads = value.split(",");
		for (String thread : threads) {
			this.threadIds.add(Long.parseLong(thread, 16));
		}
	}

	private void setThreadPcs(String value) {
		String[] threads = value.split(",");
		for (String thread : threads) {
			this.threadPcs.add(Long.parseLong(thread, 16));
		}
	}

	public List<Long> getThreadPcs() {
		return threadPcs;
	}

	public Map<Integer, BigInteger> getRegisters() {
		return registers;
	}

	public long getStopedThreadId() {
		return stoppedThreadId;
	}

	private void setStopedThread(long stopedThread) {
		this.stoppedThreadId = stopedThread;
	}

	public String getErrorDescriprion() {
		return errorDescription;
	}

	public List<StopInfo> getStopInfos() {
		return stopInfos;
	}

}
