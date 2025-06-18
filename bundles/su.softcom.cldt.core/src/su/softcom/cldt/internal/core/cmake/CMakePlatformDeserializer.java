package su.softcom.cldt.internal.core.cmake;

import java.io.IOException;
import java.util.List;
import java.util.Optional;

import com.fasterxml.jackson.core.JacksonException;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;

import su.softcom.cldt.core.cmake.ICMakePlatform;

public class CMakePlatformDeserializer extends JsonDeserializer<Optional<List<ICMakePlatform>>> {

	@Override
	public Optional<List<ICMakePlatform>> deserialize(JsonParser p, DeserializationContext ctxt)
			throws IOException, JacksonException {
		List<String> names = p.readValueAs(new TypeReference<List<String>>() {
			// do nothing
		});
		return Optional.of(names.stream().map(e -> {
			return (ICMakePlatform) new CMakePlatform(e);
		}).toList());
	}

}
