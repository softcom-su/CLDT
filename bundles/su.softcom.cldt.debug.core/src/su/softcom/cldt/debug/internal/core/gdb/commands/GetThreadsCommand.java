package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import org.eclipse.core.runtime.Platform;

/**
 * 
 * Obtain a list of all active thread IDs from the target (OS). 
 * Since there may be too many active threads to fit into one reply packet, this query works iteratively:
 *  it may require more than one query/reply sequence to obtain the entire list of threads.
 * The first query of the sequence will be the ‘qfThreadInfo’ query;
 *  subsequent queries in the sequence will be the ‘qsThreadInfo’ query. 
 * 
 *
 * @param DebugTarget target
 * 
 * @author rinat.nizamov@softcom.su
 */
public class GetThreadsCommand extends GDBCommand {

	public List<Long> firstActiveThreadIds;
	public GetThreadsCommand() {
		super("qfThreadInfo");
	}

	@Override
	public void doHandleResult(byte[] packet) {
		String handeledPacket = new String(packet);
		if (handeledPacket.startsWith("m"))
		{
			String threadOutStr = handeledPacket.substring(1);
			firstActiveThreadIds = Arrays.asList(threadOutStr.split(",")).stream().map(x -> {
				return Long.parseLong(x, 16);
			}).collect(Collectors.toList());
		} else {
			Platform.getLog(getClass()).warn("Not an 'm' packet received");
		}
		
	}

	public List<Long> getFirstActiveThreadIds() {
		return firstActiveThreadIds;
	}


}
