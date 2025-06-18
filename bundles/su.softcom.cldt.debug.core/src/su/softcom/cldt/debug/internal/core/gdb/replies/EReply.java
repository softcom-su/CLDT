package su.softcom.cldt.debug.internal.core.gdb.replies;

import su.softcom.cldt.debug.core.IGDBReply;

public class EReply implements IGDBReply {

	private int errorCode = 0;
	
	@Override
	public void handleReply(String reply) {
		errorCode = Integer.parseInt(reply);
	}

	@Override
	public ReplyType getReplyType() {
		return ReplyType.E_REPLY;
	}

	public int getErrorCode() {
		return this.errorCode;
	}
	
}
