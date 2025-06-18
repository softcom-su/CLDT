package su.softcom.cldt.internal.core.lsp;

import java.util.Arrays;

import org.eclipse.lsp4j.DocumentSymbolCapabilities;
import org.eclipse.lsp4j.ShowDocumentCapabilities;
import org.eclipse.lsp4j.SymbolKind;
import org.eclipse.lsp4j.SymbolKindCapabilities;
import org.eclipse.lsp4j.TextDocumentClientCapabilities;
import org.eclipse.lsp4j.WindowClientCapabilities;
import org.eclipse.lsp4j.WorkspaceClientCapabilities;

/**
 *
 */
public class LSPSupportedFeatures {

	public static TextDocumentClientCapabilities getTextDocumentClientCapabilities() {
		final var textDocumentClientCapabilities = new TextDocumentClientCapabilities();
		// TODO it
//		final var codeAction = new CodeActionCapabilities(new CodeActionLiteralSupportCapabilities(
//				new CodeActionKindCapabilities(Arrays.asList(CodeActionKind.QuickFix, CodeActionKind.Refactor,
//						CodeActionKind.RefactorExtract, CodeActionKind.RefactorInline, CodeActionKind.RefactorRewrite,
//						CodeActionKind.Source, CodeActionKind.SourceOrganizeImports))),
//				true);
//		codeAction.setDataSupport(true);
//		codeAction.setResolveSupport(new CodeActionResolveSupportCapabilities(List.of("edit"))); //$NON-NLS-1$
//		textDocumentClientCapabilities.setCodeAction(codeAction);
//		textDocumentClientCapabilities.setCodeLens(new CodeLensCapabilities());
//		textDocumentClientCapabilities.setInlayHint(new InlayHintCapabilities());
//		textDocumentClientCapabilities.setColorProvider(new ColorProviderCapabilities());
//		final var completionItemCapabilities = new CompletionItemCapabilities(Boolean.TRUE);
//		completionItemCapabilities.setDocumentationFormat(Arrays.asList(MarkupKind.MARKDOWN, MarkupKind.PLAINTEXT));
//		completionItemCapabilities.setInsertTextModeSupport(new CompletionItemInsertTextModeSupportCapabilities(
//				List.of(InsertTextMode.AsIs, InsertTextMode.AdjustIndentation)));
//		completionItemCapabilities.setResolveSupport(new CompletionItemResolveSupportCapabilities(
//				List.of("documentation", "detail", "additionalTextEdits"))); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
//		final var completionCapabilities = new CompletionCapabilities(completionItemCapabilities);
//		completionCapabilities.setContextSupport(Boolean.TRUE);
//		textDocumentClientCapabilities.setCompletion(completionCapabilities);
//		final var definitionCapabilities = new DefinitionCapabilities();
//		definitionCapabilities.setLinkSupport(Boolean.TRUE);
//		textDocumentClientCapabilities.setDefinition(definitionCapabilities);
//		final var typeDefinitionCapabilities = new TypeDefinitionCapabilities();
//		typeDefinitionCapabilities.setLinkSupport(Boolean.TRUE);
//		textDocumentClientCapabilities.setTypeDefinition(typeDefinitionCapabilities);
//		textDocumentClientCapabilities.setDocumentHighlight(new DocumentHighlightCapabilities());
//		textDocumentClientCapabilities.setDocumentLink(new DocumentLinkCapabilities());
		final var documentSymbol = new DocumentSymbolCapabilities();
		documentSymbol.setHierarchicalDocumentSymbolSupport(true);
		documentSymbol.setSymbolKind(new SymbolKindCapabilities(Arrays.asList(SymbolKind.Array, SymbolKind.Boolean,
				SymbolKind.Class, SymbolKind.Constant, SymbolKind.Constructor, SymbolKind.Enum, SymbolKind.EnumMember,
				SymbolKind.Event, SymbolKind.Field, SymbolKind.File, SymbolKind.Function, SymbolKind.Interface,
				SymbolKind.Key, SymbolKind.Method, SymbolKind.Module, SymbolKind.Namespace, SymbolKind.Null,
				SymbolKind.Number, SymbolKind.Object, SymbolKind.Operator, SymbolKind.Package, SymbolKind.Property,
				SymbolKind.String, SymbolKind.Struct, SymbolKind.TypeParameter, SymbolKind.Variable)));
		textDocumentClientCapabilities.setDocumentSymbol(documentSymbol);
//		textDocumentClientCapabilities.setFoldingRange(new FoldingRangeCapabilities());
//		textDocumentClientCapabilities.setFormatting(new FormattingCapabilities(Boolean.TRUE));
//		final var hoverCapabilities = new HoverCapabilities();
//		hoverCapabilities.setContentFormat(Arrays.asList(MarkupKind.MARKDOWN, MarkupKind.PLAINTEXT));
//		textDocumentClientCapabilities.setHover(hoverCapabilities);
//		textDocumentClientCapabilities.setRangeFormatting(new RangeFormattingCapabilities());
//		textDocumentClientCapabilities.setReferences(new ReferencesCapabilities());
//		final var renameCapabilities = new RenameCapabilities();
//		renameCapabilities.setPrepareSupport(true);
//		textDocumentClientCapabilities.setRename(renameCapabilities);
//		textDocumentClientCapabilities.setSignatureHelp(new SignatureHelpCapabilities());
//		textDocumentClientCapabilities
//				.setSynchronization(new SynchronizationCapabilities(Boolean.TRUE, Boolean.TRUE, Boolean.TRUE));
//		SelectionRangeCapabilities selectionRange = new SelectionRangeCapabilities();
//		textDocumentClientCapabilities.setSelectionRange(selectionRange);
		return textDocumentClientCapabilities;
	}

	public static WorkspaceClientCapabilities getWorkspaceClientCapabilities() {
		final var workspaceClientCapabilities = new WorkspaceClientCapabilities();
//		workspaceClientCapabilities.setApplyEdit(Boolean.TRUE);
//		workspaceClientCapabilities.setConfiguration(Boolean.TRUE);
//		workspaceClientCapabilities.setExecuteCommand(new ExecuteCommandCapabilities(Boolean.TRUE));
//		workspaceClientCapabilities.setSymbol(new SymbolCapabilities(Boolean.TRUE));
//		workspaceClientCapabilities.setWorkspaceFolders(Boolean.TRUE);
//		WorkspaceEditCapabilities editCapabilities = new WorkspaceEditCapabilities();
//		editCapabilities.setDocumentChanges(Boolean.TRUE);
//		editCapabilities.setResourceOperations(Arrays.asList(ResourceOperationKind.Create, ResourceOperationKind.Delete,
//				ResourceOperationKind.Rename));
//		editCapabilities.setFailureHandling(FailureHandlingKind.Undo);
//		workspaceClientCapabilities.setWorkspaceEdit(editCapabilities);
//		CodeLensWorkspaceCapabilities codeLensWorkspaceCapabilities = new CodeLensWorkspaceCapabilities(true);
//		workspaceClientCapabilities.setCodeLens(codeLensWorkspaceCapabilities);
		return workspaceClientCapabilities;
	}

	public static WindowClientCapabilities getWindowClientCapabilities() {
		final var windowClientCapabilities = new WindowClientCapabilities();
		windowClientCapabilities.setShowDocument(new ShowDocumentCapabilities(true));
		// windowClientCapabilities.setWorkDoneProgress(true);
		// windowClientCapabilities.setShowMessage(new
		// WindowShowMessageRequestCapabilities());
		return windowClientCapabilities;
	}

}
