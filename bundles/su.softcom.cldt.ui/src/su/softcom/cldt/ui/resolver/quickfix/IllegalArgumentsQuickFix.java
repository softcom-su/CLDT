package su.softcom.cldt.ui.resolver.quickfix;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.jface.text.BadLocationException;
import org.eclipse.jface.text.FindReplaceDocumentAdapter;
import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.IRegion;

public class IllegalArgumentsQuickFix extends AbstractQuickFix {

	public IllegalArgumentsQuickFix(String label) {
		super(label);
	}

	@Override
	protected void fixProblem(IDocument document, int lineNum) {
		FindReplaceDocumentAdapter dad = new FindReplaceDocumentAdapter(document);
		try {
			IRegion region = dad.find(0, "buffer_sprintf_s_text", true, false, false, false);
			boolean textExist = (region != null);
			int lineOffset = document.getLineOffset(lineNum - 1);
			IRegion region1 = dad.find(lineOffset, "sprintf_s", true, false, false, false);
			if (region1 == null) {
				log.error(String.format("Функция sprintf_s не найдена в строке %d", lineNum));
				return;
			}
			IRegion region2 = dad.find(lineOffset, ";", true, false, false, false);
			if (region2 == null) {
				return;
			}
			int startPosition = region1.getOffset();
			int endPosition = region2.getOffset();
			int lenght = endPosition - startPosition;
			String data = getFixString(document.get(startPosition, lenght), textExist);
			document.replace(startPosition, lenght, data);
		} catch (BadLocationException e) {
			log.error(String.format(
					"Проблема при поиске информации об ошибке IllegalArgumentsException в файле %s в строке %d",
					fileName, lineNum));
		}
	}

	private String getFixString(String data, boolean textExist) {

		String printedData = new String();
		String result = new String();

		Pattern pattern = Pattern.compile("\\(.*\\)");
		Matcher matcher = pattern.matcher(data);
		while (matcher.find()) {
			printedData = matcher.group();
		}
		printedData = printedData.substring(1, printedData.length() - 1);
		printedData = checkForExtraData(printedData);
		if (printedData.startsWith("\"") && printedData.endsWith("\"")) {
			result = getTextFixString(printedData, textExist);
		} else {
			result = getVariableFixString(printedData);
		}
		return result;
	}

	private String checkForExtraData(String printedData) {
		String[] splitedData = printedData.split(",");
		if (splitedData.length <= 1) {
			return printedData;
		}
		String result = splitedData[splitedData.length - 1];
		result = result.trim();
		return result;
	}

	private String getTextFixString(String printedData, boolean textExist) {
		String name = "text";
		String newTextBuffer = String.format("char buffer_sprintf_s_%s[256];\n", name);
		String newTextSprintfS = new String();
		newTextSprintfS = String.format("sprintf_s(buffer_sprintf_s_%s, _T(\"%s\"), %s)", name, "%s", printedData);
		return textExist ? newTextSprintfS : (newTextBuffer + "\t" + newTextSprintfS);
	}

	private String getVariableFixString(String printedData) {
		String name = new String();
		String newTextBuffer = new String();
		String newTextSprintfS = new String();
		name = printedData;
		newTextBuffer = String.format("char buffer_sprintf_s_%s[256];\n", name);
		newTextSprintfS = String.format("\tsprintf_s(buffer_sprintf_s_%s, _T(\"%s\"), (LPCTSTR)%s)", name, "%s", name);
		return newTextBuffer + newTextSprintfS;
	}

}
