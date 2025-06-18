package su.softcom.cldt.internal.core.analysis.valgrind;

import java.util.ArrayList;
import java.util.List;

/**
 * класс для хранения одной ошибки, которую выявил valgrind
 */
public class ValgrindError {
	public String kind;
	public String lineError;
	public String description;
	public String xwhatText;
	public ArrayList<String> stackText;
	public List<ValgrindFrame> stackTrace;

	public ValgrindError() {
		stackTrace = new ArrayList<>();
	}
}
