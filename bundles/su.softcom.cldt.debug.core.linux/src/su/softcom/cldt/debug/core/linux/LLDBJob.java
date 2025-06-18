package su.softcom.cldt.debug.core.linux;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import su.softcom.cldt.debug.internal.core.lldb.AbstractLLDBJob;

public class LLDBJob extends AbstractLLDBJob {

	public LLDBJob() throws IOException {
		LOG.info("Starting LINUX LLDBJob");
	}

	// @Override
	public int readPort() {
		LOG.info("Reading port...");
		try (BufferedReader bufferReader = new BufferedReader(
				new InputStreamReader(new FileInputStream(new File(pipeName))))) {
			String line = bufferReader.readLine().trim();
			LOG.info("Got port %s ".formatted(line));
			return Integer.valueOf(line);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return -1;
	}

	// @Override
	public String getLLDBPath(String llvmPath) {
		String lldbPath = llvmPath.length() > 0 ? llvmPath + "/bin/" : "";
		return lldbPath + "lldb-server";
	}

	// @Override
	public void createPipe() {
		LOG.info("Creating pipe...");
		File pipe = new File(pipeName);
		ProcessBuilder builder = new ProcessBuilder("mkfifo", pipeName);
		builder.inheritIO();
		if (!pipe.exists()) {
			try {
				LOG.info("New pipe has  been created.");
				builder.start();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		else
		{
			LOG.info("Pipe has already been created. Skiping creation...");
		}
	}

}
