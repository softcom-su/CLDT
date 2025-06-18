package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.nio.ByteOrder;

import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.IDebugEventBus;
import su.softcom.cldt.debug.core.IGDBReply;
import su.softcom.cldt.debug.core.debugger.events.ConsoleOutputEvent;
import su.softcom.cldt.debug.internal.core.gdb.replies.EReply;
import su.softcom.cldt.debug.internal.core.gdb.replies.TReply;

/**
 * Resume the inferior, specifying different actions for each thread. <br>
 * Currently supported actions are: {@link VContAction#CONTINUE CONTINUE} and
 * {@link VContAction#STEP STEP}
 * 
 * @author rinat.nizamov@softcom.su
 */
public class VContCommand extends GDBCommand {

	public enum VContAction {
		CONTINUE("c"), STEP("s");

		private final String command;

		VContAction(String command) {
			this.command = command;
		}

		public String getCommand() {
			return command;
		}

	}

	private VContAction action;
	private ByteOrder order;
	private IGDBReply reply = null;
	long threadId = -1;

	public VContCommand(VContAction action, ByteOrder order) {
		this(action, -1, order);
	}

	public VContCommand(VContAction action, long threadId, ByteOrder order) {
		// FIXME: В случае continue запускаются все потоки с continue, так как у нас
		// реализован all-stop mode. При поддержке non-stop mode поддержать вариацию с
		// добавлением действия для каждого потока
		super("vCont;%s%s".formatted(action.getCommand(), threadId == -1 || action.equals(VContAction.CONTINUE) ? ""
				: ":%s".formatted(Long.toHexString(threadId))));
		this.action = action;
		this.threadId = threadId;
		this.order = order;
	}

	@Override
	public void doHandleResult(byte[] result) {
		String resultStr = new String(result);
		String hexStr = resultStr.substring(1);
		switch (resultStr.charAt(0)) {
		case 'W':
			Platform.getLog(getClass()).info("The debugged programm exited with status: %s".formatted(hexStr));
			break;

		case 'O':
			Platform.getLog(getClass()).info("Redirecting output to console...");
			break;
		case 'T':
			TReply tReply = new TReply();
			tReply.setOrder(order);
			tReply.handleReply(resultStr);
			tReply.print();
			this.reply = tReply;
			break;
		case 'E':
			EReply eReply = new EReply();
			eReply.handleReply(hexStr);
			this.reply = eReply;
			Platform.getLog(getClass())
					.error("The debugged programm encountered an error with code: %s".formatted(hexStr));
			break;
		default:
			Platform.getLog(getClass()).warn("Cannot parse response: %s".formatted(resultStr));
			break;
		}

	}

	public VContAction getAction() {
		return action;
	}

	public IGDBReply getReply() {
		return reply;
	}

}
