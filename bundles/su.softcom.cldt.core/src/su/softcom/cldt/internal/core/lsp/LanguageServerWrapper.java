package su.softcom.cldt.internal.core.lsp;

import java.util.concurrent.CancellationException;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Function;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.Platform;
import org.eclipse.lsp4e.server.StreamConnectionProvider;
import org.eclipse.lsp4j.ClientCapabilities;
import org.eclipse.lsp4j.ClientInfo;
import org.eclipse.lsp4j.InitializeParams;
import org.eclipse.lsp4j.ServerCapabilities;
import org.eclipse.lsp4j.WindowClientCapabilities;
import org.eclipse.lsp4j.jsonrpc.Launcher;
import org.eclipse.lsp4j.jsonrpc.validation.NonNull;
import org.eclipse.lsp4j.services.LanguageServer;

import su.softcom.cldt.core.CMakeCorePlugin;

/**
 * 
 */
public class LanguageServerWrapper {

	protected StreamConnectionProvider lspStreamProvider;
	private Launcher<LanguageServer> launcher;
	private ServerCapabilities serverCapabilities;
	private final ExecutorService dispatcher;
	private IProject initialProject;

	/** Unified private constructor to set sensible defaults in all cases */
	public LanguageServerWrapper(IProject project, Launcher<LanguageServer> launcher) {
		this.launcher = launcher;
		this.initialProject = project;
		this.dispatcher = Executors.newSingleThreadExecutor(new ThreadFactory() {
			@Override
			public Thread newThread(Runnable r) {
				Thread thread = new Thread(r, "LSP thread");
				thread.setPriority(Thread.MAX_PRIORITY);
				return thread;
			}
		});
	}

	/**
	 * Runs a request on the language server
	 *
	 * @param <T> LS response type
	 * @param fn  Code block that will be supplied the LS in a state where it is
	 *            guaranteed to have been initialized. This should usually be simply
	 *            invoking a method of LSP4E; more complex work like result
	 *            transformation should be avoided here, because
	 *            <ul>
	 *            <li>LSP4E will cancel those futures when necessary, and the
	 *            cancellation is expected to also cancel the LSP request; so this
	 *            method makes LSP4E support proper cancellation only when the
	 *            future here originates from LSP4J API (or transitively cancels the
	 *            related future from LSP4J API)
	 *            <li>This work will run on the Language Server dispatcher thread;
	 *            so extraneous work will block the thread for other work, like
	 *            other pending LSP requests</li>
	 *            </ul>
	 *
	 * @return Async result
	 */
	public <T> CompletableFuture<T> execute(Function<LanguageServer, ? extends CompletableFuture<T>> fn) {
		// Send the request on the dispatch thread
		CompletableFuture<T> lsRequest = executeImpl(fn);
		// then additionally make sure the response is delivered on a thread from the
		// default ForkJoinPool.
		// This makes sure the user can't chain on an arbitrary
		// long-running block of code that would tie up the server response listener and
		// prevent any more
		// inbound messages being read
		CompletableFuture<T> future = lsRequest.thenApplyAsync(Function.identity());
		// and ensure cancellation of the returned future cancels the LS request (send
		// cancel event via
		// LSP4J)
		future.exceptionally(t -> {
			if (t instanceof CancellationException) {
				lsRequest.cancel(true);
			}
			return (T) null;
		});
		return future;
	}

	/**
	 * Runs a request on the language server. Internal hook for the LSPexecutor
	 * implementations
	 *
	 * @param <T> LS response type
	 * @param fn  LSP method to invoke. This should usually be simply invoking a
	 *            method of LSP4E; more complex work like result transformation
	 *            should be avoided here, because
	 *            <ul>
	 *            <li>LSP4E will cancel those futures when necessary, and the
	 *            cancellation is expected to also cancel the LSP request; so this
	 *            method makes LSP4E support proper cancellation only when the
	 *            future here originates from LSP4J API (or transitively cancels the
	 *            related future from LSP4J API)
	 *            <li>This work will run on the Language Server dispatcher thread;
	 *            so extraneous work will block the thread for other work, like
	 *            other pending LSP requests</li>
	 *            </ul>
	 * @return Async result
	 */
	@NonNull
	<T> CompletableFuture<T> executeImpl(@NonNull Function<LanguageServer, ? extends CompletableFuture<T>> fn) {
		// Run the supplied function, ensuring that it is enqueued on the dispatch
		// thread associated with the
		// wrapped language server, and is thus guarannteed to be seen in the correct
		// order with respect
		// to e.g. previous document changes
		//
		// Note this doesn't get the .thenApplyAsync(Function.identity()) chained on
		// additionally, unlike
		// the public-facing version of this method, because we trust the LSPExecutor
		// implementations to
		// make sure the server response thread doesn't get blocked by any further work
		AtomicReference<CompletableFuture<T>> request = new AtomicReference<>();
		Function<Launcher<LanguageServer>, CompletableFuture<T>> cancelWrapper = lnchr -> {
			CompletableFuture<T> res = fn.apply(lnchr.getRemoteProxy());
			request.set(res);
			return res;
		};
		CompletableFuture<T> res = getInitializedServer().thenComposeAsync(cancelWrapper, this.dispatcher);
		res.exceptionally(e -> {
			if (e instanceof CancellationException) {
				CompletableFuture<T> stage = request.get();
				if (stage != null) {
					stage.cancel(false);
				}
			}
			return null;
		});
		return res;
	}

	/**
	 * Starts the language server, ensure it's and returns a CompletableFuture
	 * waiting for the server to be initialized and up-to-date (all related pending
	 * document changes notifications are sent).
	 * <p>
	 * If done in the UI stream, a job will be created displaying that the server is
	 * being initialized
	 * </p>
	 *
	 */
	@NonNull
	protected CompletableFuture<Launcher<LanguageServer>> getInitializedServer() {
		return CompletableFuture.completedFuture(this.launcher);
	}

	/**
	 * @return
	 */
	public static InitializeParams getInitParams() {
		final InitializeParams initParams = new InitializeParams();
		final String name = Platform.getProduct() != null ? Platform.getProduct().getName() : "CLDT"; //$NON-NLS-1$

		final var workspaceClientCapabilities = LSPSupportedFeatures.getWorkspaceClientCapabilities();
		final var textDocumentClientCapabilities = LSPSupportedFeatures.getTextDocumentClientCapabilities();

		WindowClientCapabilities windowClientCapabilities = LSPSupportedFeatures.getWindowClientCapabilities();
		initParams.setCapabilities(new ClientCapabilities(workspaceClientCapabilities, textDocumentClientCapabilities,
				windowClientCapabilities, null));
		initParams.setClientInfo(getClientInfo(name));
		return initParams;
	}

	private static ClientInfo getClientInfo(String name) {
		String pluginVersion = Platform.getBundle(CMakeCorePlugin.PLUGIN_ID).getVersion().toString();
		return new ClientInfo(name, pluginVersion);
	}

	public Launcher<LanguageServer> getLauncher() {
		return launcher;
	}

}
