package su.softcom.cldt.ui.resolver.quickfix;

import org.eclipse.jface.text.BadLocationException;
import org.eclipse.jface.text.FindReplaceDocumentAdapter;
import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.IRegion;

public class MainReturnValueQuickFix extends AbstractQuickFix {

	public MainReturnValueQuickFix() {
		super("Быстрое исправление для возвращаемого значения функции main");
	}

	@Override
	protected void fixProblem(IDocument document, int lineNum) {
		FindReplaceDocumentAdapter dad = new FindReplaceDocumentAdapter(document);
		try {
			int startPosition = document.getLineOffset(lineNum - 1);
			IRegion region1 = dad.find(startPosition, "main", true, false, false, false);
			if (region1 == null) {
				log.error(String.format("Функция main не найдена в строке %d", lineNum));
				return;
			}

			int endPosition = region1.getOffset();
			int lenght = endPosition - startPosition;
			String mainData = document.get(startPosition, lenght);
			while (mainData.endsWith(" ")) {
				mainData = mainData.substring(0, mainData.length() - 1);
			}
			int spaceIndex = mainData.lastIndexOf(" ");
			startPosition = spaceIndex == -1 ? startPosition : startPosition + spaceIndex;
			document.replace(startPosition, endPosition - startPosition, "int ");
		} catch (BadLocationException e) {
			log.error(String.format("Проблема при обработке ошибки MainReturnValue в файле %s в строке %d", fileName,
					lineNum));
		}
	}

}
