package su.softcom.cldt.debug.internal.core.dwarf;

import java.nio.file.Path;
import java.nio.file.Paths;

public class FolderDescriptor {

	final Path path;

	public FolderDescriptor(String str) {
		this.path = Paths.get(str).normalize();
	}

	public FolderDescriptor(FolderDescriptor folder, String relatedPath) {
		this.path = Paths.get(folder.getPath() + "/" + relatedPath).normalize();
	}

	public Path getPath() {
		return path;
	}
}
