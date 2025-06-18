package su.softcom.cldt.debug.internal.core.lldb;

import java.io.IOException;
import java.util.Arrays;

import com.sun.jna.ptr.IntByReference;
import com.sun.jna.platform.win32.Kernel32;
import com.sun.jna.platform.win32.WinBase;
import com.sun.jna.platform.win32.WinNT.HANDLE;

public class LLDBJob extends AbstractLLDBJob {

	static final int BUFFER_SIZE = 1024;

	HANDLE pipe;

	public LLDBJob() throws IOException {
		System.out.println("Starting WINDOWS LLDBJob");
		// super(program, args,
		// "%s-lldb-debug".formatted(program.getName().substring(0,
		// program.getName().lastIndexOf(".") > 0 ? program.getName().lastIndexOf(".")
		// : program.getName().length())));
	}

	public String getLLDBPath(String llvmPath) {
		String lldbPath = llvmPath.length() > 0 ? llvmPath + "/bin/" : "";
		return "%slldb-server.exe".formatted(lldbPath);
	}

	public int readPort() {
		byte[] buf = new byte[BUFFER_SIZE];
		IntByReference bytesRead = new IntByReference(0);
		while (bytesRead.getValue() == 0) {
			Kernel32.INSTANCE.ReadFile(pipe, buf, BUFFER_SIZE, bytesRead, null);
		}
		byte[] result = Arrays.copyOfRange(buf, 0, bytesRead.getValue());
		return Integer.valueOf(new String(result).trim());
	}

	public void createPipe() {
		pipe = Kernel32.INSTANCE.CreateNamedPipe("\\\\.\\pipe\\%s".formatted(getPipeName()), WinBase.PIPE_ACCESS_DUPLEX, // dwOpenMode
				WinBase.PIPE_TYPE_BYTE | WinBase.PIPE_READMODE_BYTE | WinBase.PIPE_WAIT, // dwPipeMode
				1, // nMaxInstances,
				Byte.MAX_VALUE, // nOutBufferSize,
				Byte.MAX_VALUE, // nInBufferSize,
				1000, // nDefaultTimeOut,
				null);
		if (WinBase.INVALID_HANDLE_VALUE.equals(pipe)) {
			Kernel32.INSTANCE.GetLastError();
			throw new RuntimeException("Can't create pipe with name %s".formatted(getPipeName()));
		}
	}

}
