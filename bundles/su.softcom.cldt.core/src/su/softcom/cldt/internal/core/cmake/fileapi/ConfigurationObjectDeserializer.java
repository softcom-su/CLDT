package su.softcom.cldt.internal.core.cmake.fileapi;

import java.io.IOException;

import com.fasterxml.jackson.core.JacksonException;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.ObjectCodec;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import com.fasterxml.jackson.databind.node.ArrayNode;

import su.softcom.cldt.core.cmake.Target;

public class ConfigurationObjectDeserializer extends StdDeserializer<ConfigurationObject> {

	public ConfigurationObjectDeserializer() {
		this(null);
	}

	public ConfigurationObjectDeserializer(Class<?> vc) {
		super(vc);
		// TODO Auto-generated constructor stub
	}

	@Override
	public ConfigurationObject deserialize(JsonParser p, DeserializationContext ctxt)
			throws IOException, JacksonException {
		ObjectCodec codec = p.getCodec();
		JsonNode node = codec.readTree(p);
		JsonNode targets = node.get("targets");
		if (targets instanceof ArrayNode array) {
			for (JsonNode target : array) {
				Target tgt = ctxt.readTreeAsValue(target, Target.class);
			}
		}
		return null;
	}

}
