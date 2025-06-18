package su.softcom.cldt.internal.core.lsp;

import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;

import org.eclipse.lsp4e.server.ProcessStreamConnectionProvider;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.llvm.ILLVMInstance;

/**
 * Language server protocol for eclipse stream connection provider
 * {@link org.eclipse.lsp4e}.
 */
public class CStreamConnectionProvider extends ProcessStreamConnectionProvider {

	private static final String CLANGD_STR = "clangd"; //$NON-NLS-1$

	/**
	 * Configure stream connection provider.
	 * 
	 * @throws MalformedURLException
	 */
	public CStreamConnectionProvider() throws MalformedURLException {

		ILLVMInstance llvmInstance = CMakeCorePlugin.getDefault().getLLVMInstance(null);
		final List<String> commands = new ArrayList<>();
		if (llvmInstance.hasTool(CLANGD_STR)) {
			commands.add(llvmInstance.getTool(CLANGD_STR).getAbsolutePath());
		} else {
			commands.add(CLANGD_STR);
		}
		setCommands(commands);
	}

}
