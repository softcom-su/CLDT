package su.softcom.cldt.internal.core.cmake.fileapi;

import java.util.Optional;

public class Version {
	public Optional<String> major;
	public Optional<String> minor;
	public Optional<Boolean> isDirty;
	public Optional<Integer> patch;
	public Optional<String> string;
	public Optional<String> suffix;

	public void setIsDirty(Boolean dirty) {
		isDirty = Optional.of(dirty);
	}

	public void setPatch(Integer integer) {
		patch = Optional.of(integer);
	}

	public void setString(String str) {
		string = Optional.of(str);
	}

	public void setSuffix(String str) {
		suffix = Optional.of(str);
	}

	public void setMajor(String str) {
		major = Optional.of(str);
	}

	public void setMinor(String str) {
		minor = Optional.of(str);
	}
}
