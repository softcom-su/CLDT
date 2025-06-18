package su.softcom.cldt.internal.core.analysis.valgrind;

/**
 * разбор одного фрейма из одной ошибки
 */
public class ValgrindFrame {
	public String ip;
	public String obj;
	public String fn;
	public String dir;
	public String file;
	public String line;
}