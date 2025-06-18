package su.softcom.cldt.internal.core.lsp;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CompletableFuture;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.lsp4j.DidOpenTextDocumentParams;
import org.eclipse.lsp4j.DocumentSymbol;
import org.eclipse.lsp4j.DocumentSymbolParams;
import org.eclipse.lsp4j.SymbolInformation;
import org.eclipse.lsp4j.TextDocumentIdentifier;
import org.eclipse.lsp4j.TextDocumentItem;
import org.eclipse.lsp4j.jsonrpc.Launcher;
import org.eclipse.lsp4j.jsonrpc.messages.Either;
import org.eclipse.lsp4j.services.LanguageServer;
import org.eclipse.lsp4j.services.TextDocumentService;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.source.ISourceFile;

/**
 * 
 */
public class DocumentSymbolHandler {

	private LanguageServerWrapper wrapper = new LanguageServerWrapper(null, null);

	private DocumentSymbolHandler() {
		// utility class
	}

	/**
	 * @param variants
	 * @return DocumentSymbol
	 */
	@SuppressWarnings("deprecation") // its intended
	public static DocumentSymbol toDocumentSymbol(Either<SymbolInformation, DocumentSymbol> variants) {
		if (variants.isRight()) {
			return variants.getRight();
		}
		SymbolInformation left = variants.getLeft();
		return new DocumentSymbol(left.getName(), left.getKind(), left.getLocation().getRange(),
				left.getLocation().getRange(), null, Collections.emptyList());
	}

	/**
	 * @param file
	 * @return List of DocumentSymbol of this {@code file}
	 */
	public static List<DocumentSymbol> getSymbols(ISourceFile file) {
		IFile eclipseFile = file.getFile();
		LanguageServerWrapper wrapper = CMakeCorePlugin.getDefault().getProject(file.getProject()).getLSPLauncher();
		Launcher<LanguageServer> launcher = wrapper.getLauncher();
		final var params = new DocumentSymbolParams(
				new TextDocumentIdentifier(eclipseFile.getLocationURI().toASCIIString()));
		TextDocumentService docServ = launcher.getRemoteProxy().getTextDocumentService();

		try (InputStream content = eclipseFile.getContents()) {
			docServ.didOpen(new DidOpenTextDocumentParams(new TextDocumentItem(
					eclipseFile.getLocationURI().toASCIIString(), "c++", 0, new String(content.readAllBytes())))); //$NON-NLS-1$
		} catch (IOException | CoreException e) {
			e.printStackTrace();
		}
		CompletableFuture<List<Either<SymbolInformation, DocumentSymbol>>> symbols = wrapper
				.execute(ls -> ls.getTextDocumentService().documentSymbol(params));

		List<DocumentSymbol> result = new ArrayList<>();
		for (Either<SymbolInformation, DocumentSymbol> elem : symbols.join()) {
			result.add(toDocumentSymbol(elem));
		}
		return result;
	}

}
