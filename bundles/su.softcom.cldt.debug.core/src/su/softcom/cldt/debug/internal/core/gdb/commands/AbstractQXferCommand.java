package su.softcom.cldt.debug.internal.core.gdb.commands;

/**
 * Abstract class for qXfer commands 
 * 
 * @author rinat.nizamov@softcom.su
 */
public abstract class AbstractQXferCommand extends GDBCommand {
	
	protected AbstractQXferCommand(String object, String annex, String args) {
		super("qXfer:%s:read:%s:%s".formatted(object, 
				annex.isEmpty() ? "" : annex,
				args.isEmpty() ? "" : args));
	}
		
}
