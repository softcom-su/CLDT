package su.softcom.cldt.internal.core.cmake;

import java.util.List;
import java.util.Optional;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;

import su.softcom.cldt.core.cmake.ICMakeGenerator;
import su.softcom.cldt.core.cmake.ICMakePlatform;

public class CMakeGenerator implements ICMakeGenerator {

	@JsonProperty("extraGenerators")
	String[] extraGenerators;
	@JsonProperty("name")
	String name;
	@JsonProperty("platformSupport")
	boolean platformSupport;
	@JsonProperty("supportedPlatforms")
	@JsonDeserialize(using = CMakePlatformDeserializer.class)
	Optional<List<ICMakePlatform>> supportedPlatforms;
	@JsonProperty("toolsetSupport")
	boolean toolsetSupport;

	@Override
	public String getName() {
		return name;
	}

	@Override
	public boolean isPlatformSupport() {
		return platformSupport;
	}

	@Override
	public List<ICMakePlatform> getPlatforms() {
		if (isPlatformSupport()) {
			return supportedPlatforms.get();
		}
		throw new UnsupportedOperationException("platforms is not supported");
	}

}
