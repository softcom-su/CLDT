package su.softcom.cldt.debug.core;


/**
 * Interface IGDBReply defines a method to handle replies from a LLDB server.
 * 
 * @author maxim.drumov@softcom.su
 */
public interface IGDBReply {

	//TODO: add other types
	public enum ReplyType {
		T_REPLY, O_REPLY, E_REPLY
	}
	
    /**
     * Handles the reply string from LLDB server by parsing and storing relevant information.
     * 
     * @param reply the input string to be parsed
     */
	void handleReply(String reply);
	
	ReplyType getReplyType();
	
	
}
