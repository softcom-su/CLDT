package su.softcom.cldt.internal.core.cmake;

import java.util.Arrays;
import java.util.List;

import su.softcom.cldt.core.builders.CMakeProcessMonitor.OUTPUT_TYPE;
import su.softcom.cldt.core.cmake.ICMakeProject;

/**
 * Документ проекта. Нужен для синхронизации и буферизации записей в слушателя.
 * 
 * @author rinat.nizamov
 */
public class CMakeProjectDocument {

	private static final int BUFFER_SIZE = 4000; // TODO: read from workspace

	private ICMakeProject project;
	private OUTPUT_TYPE[] lineTypes;
	private String[] lines;
	private int writeIndex = 0;

	/**
	 * Считать строки для определенного слушателя.
	 * 
	 * @param index
	 * @return возвращает считанные в документ строки
	 */
	public List<DocumentLine> readLines(int index) {
		DocumentLine[] docLines = new DocumentLine[index > writeIndex ? BUFFER_SIZE : writeIndex];
		if (isEmpty(index)) {
			return Arrays.asList(docLines);
		}
		for (int i = index; i < docLines.length; i++) {
			docLines[i] = new DocumentLine(lines[i], lineTypes[i]);
		}
		return Arrays.asList(docLines);
	}

	/**
	 * @return true означает, что для этого индекса документ уже рассказал всё
	 *         содержимое.
	 */
	private boolean isEmpty(int index) {
		return writeIndex == index;
	}

	/**
	 * Представление строки документа.
	 */
	public class DocumentLine {

		private String line;
		private OUTPUT_TYPE type;

		DocumentLine(String line, OUTPUT_TYPE type) {
			this.line = line;
			this.type = type;
		}

		@Override
		public String toString() {
			return line;
		}

		/**
		 * @return строку
		 */
		public String getLine() {
			return line;
		}

		/**
		 * @return тип
		 */
		public OUTPUT_TYPE getType() {
			return type;
		}
	}

	/**
	 * Creates an empty console document. Каждый документ связан со своим project-ом
	 * 
	 * @param prject
	 */
	public CMakeProjectDocument(ICMakeProject prject) {
		this.project = prject;
	}

	/**
	 * @return проект, к которому этот документ привязан
	 */
	public ICMakeProject getProject() {
		return project;
	}

	/**
	 * Appends a line of the specified type to the end of the console.
	 * 
	 * @param type тип строки
	 * @param line строка
	 */
	public void appendConsoleLine(OUTPUT_TYPE type, String line) {
		if (lines == null) {
			lines = new String[BUFFER_SIZE];
			lineTypes = new OUTPUT_TYPE[BUFFER_SIZE];
		}
		lines[writeIndex] = line;
		lineTypes[writeIndex] = type;

		if (++writeIndex >= BUFFER_SIZE) {
			writeIndex = 0;
		}
	}

	/**
	 * Clears content of this Document
	 */
	public void clear() {
		lineTypes = null;
		lines = null;
		writeIndex = 0;
	}
}
