package su.softcom.cldt.ui.resolver.quickfix;

import org.eclipse.jface.text.BadLocationException;
import org.eclipse.jface.text.IDocument;

public class ExpectedSemicolonQuickFix extends AbstractQuickFix {
	
	public ExpectedSemicolonQuickFix() {
		super("Быстрое исправление для пропущенного символа ';'");
	}

	@Override
	protected void fixProblem(IDocument document, int lineNum) {
		try {
			int endPosition = document.getLineOffset(lineNum);			
			document.replace(endPosition - 1, 0, ";");
		} catch (BadLocationException e) {
			log.error(String.format(
					"Проблема при исправлении ошибки отстствия ';' в файле %s в строке %d",
					fileName, lineNum));
		}
	}

}
