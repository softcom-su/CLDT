package su.softcom.cldt.core.source;

import java.util.Collections;
import java.util.List;
import java.util.Optional;

import org.eclipse.core.runtime.Assert;
import org.eclipse.lsp4j.DocumentSymbol;

/**
 * 
 */
public final class SourcePartFactory {

	private static SourcePartFactory eInstance;

	/**
	 * @return this SourcePartFactory
	 */
	public static SourcePartFactory getDefault() {

		if (eInstance == null) {
			eInstance = new SourcePartFactory();
		}
		return eInstance;

	}

	private SourcePartFactory() {

	}

	/**
	 * Рекурсивно создает элемент исходного кода ({@link SourceElement}) на основе
	 * символа документа и файла.
	 * 
	 * @param docSymbol символ документа, для которого создается элемент (не может
	 *                  быть {@code null})
	 * @param file      файл, с которым связан элемент (не может быть {@code null})
	 * @return новый экземпляр {@link SourceElement} с дочерними элементами
	 * @throws NullPointerException если любой из параметров равен {@code null}
	 */
	public SourceElement getSourceElement(DocumentSymbol docSymbol, ISourceFile file) {
		Assert.isNotNull(docSymbol, "DocumentSymbol не может быть null");
		Assert.isNotNull(file, "ISourceFile не может быть null");
		List<SourceElement> srcChildren = Optional.ofNullable(docSymbol.getChildren()).orElseGet(Collections::emptyList)
				.stream().map(symbol -> getSourceElement(symbol, file)).toList();
		return new SourceElement(docSymbol, srcChildren, file);
	}
}
