package su.softcom.cldt.debug.internal.core.gdb.commands;


/**
 * Abstract class for VFile commands 
 * 
 * @author rinat.nizamov@softcom.su
 */
public abstract class AbstractVFileCommand extends GDBCommand {

	protected AbstractVFileCommand(String operation, String args) {
		super("vFile:%s:%s".formatted(operation, 
				args.isEmpty() ? "" : args));
	}

}
