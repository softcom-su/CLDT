package su.softcom.cldt.core.cmake;

import java.util.List;

import com.fasterxml.jackson.annotation.JsonProperty;

import su.softcom.cldt.internal.core.cmake.CMakeGenerator;

public class CMakeInfo {

	public static record Version(boolean isDirty, int major, int minor, int patch, String string, String suffix)
			implements Comparable<Version> {

		@Override
		public int compareTo(Version o) {
			if (major != o.major) {
				return major - o.major;
			}
			if (minor != o.minor) {
				return minor - o.minor;
			}
			return patch - o.patch;
		}
	};

	@JsonProperty("debugger")
	boolean debugger;
	@JsonProperty("fileApi")
	Object fileApi;
	@JsonProperty("generators")
	List<? extends CMakeGenerator> generators;
	@JsonProperty("serverMode")
	boolean serverMode;
	@JsonProperty("version")
	Version version;
	@JsonProperty("tls")
	boolean tls;

	public List<? extends ICMakeGenerator> getGenerators() {
		return generators;
	}

	public Version getVersion() {
		return version;
	}

}
