package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.util.HexFormat;

import su.softcom.cldt.debug.core.IGdbCommand;


public class BreakpointCommand extends GDBCommand implements IGdbCommand {
	
	public enum BreakpointType {
		SOFTWARE(0), HARDWARE(1), WRITE(2), READ(3), ACCESS(4);

		private final int num;

		BreakpointType(int i) {
			this.num = i;
		}

		public int getNum() {
			return num;
		}
	};

	
	public enum BreakpointAction {
		ADD(0),
		REMOVE(1);
	  
		private final int num;
		
		BreakpointAction(int i) { 
			this.num = i; 
		}
		
		public String getCommand () { 
			return num == 0 ? "Z" : "z"; 
		} 
	};
	
	private boolean status = false;
	
	public BreakpointCommand(BreakpointAction action, BreakpointType type, int kind, long addr, String conditions, String cmds) {
		super("%s%s,%s,%s%s%s".formatted(action.getCommand(), 
				type.getNum(), 
				HexFormat.of().toHexDigits(addr),
				HexFormat.of().toHexDigits(kind, 4),
				conditions.isEmpty() ? "" : ";%s".formatted(conditions),
				cmds.isEmpty() ? "" : ";cmds:persist,%s".formatted(cmds)));		
	}
	
	@Override
	public void doHandleResult(byte[] result) {
		String resultStr = new String(result);
		if (resultStr.equals("OK")) {
			status = true;
		}
	}

	public boolean getStatus() {
		return status;
	}
	
}
