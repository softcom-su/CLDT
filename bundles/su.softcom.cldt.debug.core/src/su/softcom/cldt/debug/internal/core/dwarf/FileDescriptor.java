package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.file.Paths;
import java.nio.file.Path;

import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.Platform;

public class FileDescriptor{

	private Path path;
	private String md5;
	
	public FileDescriptor(String name) {
		this.path = Paths.get(name).normalize();
//		Platform.getLog(getClass()).info(this.path == null  ? "Is local file" : "File found");
	}
	
	public FileDescriptor(FolderDescriptor folder, String name) {
		this.path = Paths.get(folder.getPath() + "/" + name).normalize();
//		Platform.getLog(getClass()).info("Read file by folder path: " + folder.path + " and file path: " + name);
//		Platform.getLog(getClass()).info(this.path == null ? "File not found" : "File found " + this.path);
	}
	
	public Path getPath() {
		return path;
	}

	public void setMD5(String md5) {
		this.md5 = md5;
	}
}