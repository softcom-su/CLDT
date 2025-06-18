package su.softcom.cldt.core.source;

import org.eclipse.lsp4j.Position;
import org.eclipse.lsp4j.SymbolKind;

/**
 * Представляет элемент исходного кода, связанный с конкретным файлом и
 * описываемый символом из Language Server Protocol (LSP).
 * <p>
 * Расширяет {@link ISourcePart} и добавляет методы для получения файла, имени и
 * LSP-символа, соответствующего этому элементу.
 * <p>
 * Используется для описания структурных элементов исходного кода C++, например,
 * функций, классов, переменных
 */
public interface ISourceElement extends ISourcePart {

	/**
	 * Возвращает файл, в котором находится данный элемент исходного кода.
	 *
	 * @return объект {@link ISourceFile}, содержащий этот элемент
	 */
	ISourceFile getFile();

	/**
	 * @return parent
	 */
	ISourceElement getParent();

	/**
	 * Возвращает имя данного элемента исходного кода.
	 *
	 * @return имя элемента
	 */
	String getName();

	/**
	 * The kind of this symbol.
	 * 
	 * @return symbol kind
	 */
	SymbolKind getSymbolKind();

	/**
	 * The range's start position
	 * 
	 * @return value of start position of this symbol
	 */
	Position getStartPosition();

	/**
	 * The range's end position
	 * 
	 * @return value of end position of this symbol
	 */
	Position getEndPosition();

	/**
	 * More detail for this symbol, e.g the signature of a function. If not provided
	 * the name is used.
	 * 
	 * @return string of detail
	 */
	String getDetail();

}
