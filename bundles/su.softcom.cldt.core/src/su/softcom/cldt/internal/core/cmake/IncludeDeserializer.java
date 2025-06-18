package su.softcom.cldt.internal.core.cmake;

import java.io.IOException;

import com.fasterxml.jackson.core.JacksonException;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;

import su.softcom.cldt.core.cmake.IncludeObject;

public class IncludeDeserializer extends StdDeserializer<IncludeObject> {

	public IncludeDeserializer() {
		this(null);
	}

	protected IncludeDeserializer(Class<?> vc) {
		super(vc);
	}

	@Override
	public IncludeObject deserialize(JsonParser p, DeserializationContext ctxt) throws IOException, JacksonException {
		JsonNode node = p.getCodec().readTree(p);
		String path = node.get("path").asText();
		boolean system = false;
		if (node.has("isSystem")) {
			system = node.get("isSystem").asBoolean();
		}
		return new IncludeObject(path, system);
	}

}
