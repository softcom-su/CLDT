package su.softcom.cldt.core.source;

import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Objects;

import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.IPath;
import org.eclipse.lsp4j.DocumentSymbol;
import org.eclipse.lsp4j.Position;
import org.eclipse.lsp4j.SymbolKind;

/**
 * Реализация {@link ISourceElement}, представляющая элемент исходного кода,
 * описываемый символом из Language Server Protocol (LSP).
 * <p>
 * Каждый объект {@code SourceElement} связан с файлом {@link ISourceFile},
 * содержит имя, LSP-символ {@link DocumentSymbol} и может иметь вложенные
 * дочерние элементы.
 * <p>
 * Используется для представления структурных элементов C++ кода (например,
 * классов, функций, переменных) в рамках интеграции с clangd.
 */
public class SourceElement extends AbstractSourcePart implements ISourceElement {

	private final String name;
	private final ISourceFile file;
	private final SymbolKind kind;
	private final Position startPos;
	private final Position endPos;
	private final String detail;
	private ISourceElement parent;
	private List<SourceElement> elements = new LinkedList<>();

	/**
	 * Создаёт новый элемент исходного кода.
	 *
	 * @param docSymbol символ LSP, описывающий этот элемент
	 * @param children  список дочерних элементов (вложенных структур)
	 * @param file      файл {@link ISourceFile}, к которому принадлежит элемент
	 */
	public SourceElement(DocumentSymbol docSymbol, List<SourceElement> children, ISourceFile file) {
		super(file.getProject());
		this.file = file;
		this.name = docSymbol.getName();
		this.kind = docSymbol.getKind();
		this.startPos = docSymbol.getRange().getStart();
		this.endPos = docSymbol.getRange().getEnd();
		this.detail = docSymbol.getDetail();
		children.stream().forEach(srcElem -> srcElem.setParent(this));
		this.elements = children;
	}

	/**
	 * @param parent
	 */
	public void setParent(ISourceElement parent) {
		this.parent = parent;
	}

	/**
	 * Возвращает список дочерних элементов исходного кода.
	 * <p>
	 * Список является неизменяемым.
	 *
	 * @return список дочерних элементов {@link ISourcePart}
	 */
	@Override
	public List<ISourcePart> getElements() {
		return Collections.unmodifiableList(elements);
	}

	/**
	 * Возвращает имя данного элемента исходного кода.
	 *
	 * @return имя элемента
	 */
	@Override
	public String getName() {
		return name;
	}

	/**
	 * Возвращает файл, в котором находится данный элемент.
	 *
	 * @return файл {@link ISourceFile}
	 */
	@Override
	public ISourceFile getFile() {
		return file;
	}

	/**
	 * Возвращает уникальное имя-идентификатор для данного элемента, состоящее из
	 * идентификатора файла, имени элемента и номера строки начала диапазона
	 * символа.
	 *
	 * @return уникальный идентификатор элемента
	 */
	@Override
	public String getIdentifierName() {
		String parentName = ""; //$NON-NLS-1$
		if (parent != null) {
			parentName = parent.getIdentifierName() + IPath.SEPARATOR;
		}
		return parentName + name + "[" + kind.toString() + "]"; //$NON-NLS-1$ //$NON-NLS-2$
	}

	@Override
	public SymbolKind getSymbolKind() {
		return kind;
	}

	@Override
	public Position getStartPosition() {
		return startPos;
	}

	@Override
	public Position getEndPosition() {
		return endPos;
	}

	@Override
	public String getDetail() {
		return detail;
	}

	@Override
	public <T> T getAdapter(Class<T> adapter) {
		Assert.isNotNull(adapter);
		return null;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null || getClass() != obj.getClass())
			return false;

		SourceElement other = (SourceElement) obj;
		return Objects.equals(name, other.name) && Objects.equals(file, other.file)
				&& Objects.equals(detail, other.detail) && kind == other.kind
				&& Objects.equals(startPos, other.startPos) && Objects.equals(endPos, other.endPos);

	}

	@Override
	public int hashCode() {
		return Objects.hash(name, file, kind, startPos, endPos, detail);
	}

	@Override
	public ISourceElement getParent() {
		return parent;
	}

}
