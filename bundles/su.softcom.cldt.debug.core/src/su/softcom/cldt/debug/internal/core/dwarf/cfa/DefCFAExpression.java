package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.CFARule;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;

public class DefCFAExpression extends ADefCFA {

	byte[] expression;
	
	/**
	 * Def CFA выражение
	 * @param expression
	 */
	public DefCFAExpression(byte[] expression) {
		this.expression = expression;
	}

	public void updateRow(TableRow row) {
		CFARule rule = row.getCFARule();
		rule.setExpression(expression);
	}

}
