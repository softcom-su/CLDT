package su.softcom.cldt.internal.core.cmake.fileapi;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.Path;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.ObjectCodec;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.ArrayNode;

import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.core.source.ISourceFile;
import su.softcom.cldt.core.source.SourceFile;

public class TargetFileDeserializer extends JsonDeserializer<Target> {

	final Target target;

	public TargetFileDeserializer(Target target) {
		Assert.isNotNull(target);
		this.target = target;
	}

	@Override
	public Target deserialize(JsonParser p, DeserializationContext ctxt) throws IOException {
		ObjectCodec codec = p.getCodec();
		JsonNode node = codec.readTree(p);
		JsonNode artifactsNode = node.get("artifacts"); //$NON-NLS-1$
		if (artifactsNode instanceof ArrayNode array) {
			List<IFile> artifactList = new ArrayList<>(array.size());
			for (JsonNode artifactNode : array) {
				artifactList.add(target.getProject().getBuildFolder().getFile(artifactNode.get("path").asText())); //$NON-NLS-1$
			}
			target.setArtifacts(artifactList);
		}
		JsonNode sourcesNode = node.get("sources"); //$NON-NLS-1$
		if (sourcesNode instanceof ArrayNode array) {
			List<ISourceFile> filesList = new ArrayList<>(array.size());
			for (JsonNode sourceNode : array) {
				if (sourceNode.has("path")) { //$NON-NLS-1$
					ISourceFile sourceFile = new SourceFile(new Path(sourceNode.get("path").asText()), //$NON-NLS-1$
							target);
					sourceFile.addTarget(target);
					filesList.add(sourceFile);
				}
			}
			target.setSources(filesList);
		}
		target.setName(node.get("name").asText()); //$NON-NLS-1$
		String typeString = node.get("type").asText(); //$NON-NLS-1$
		switch (typeString) {
		case "EXECUTABLE": //$NON-NLS-1$
			target.setType(Target.Type.EXECUTABLE);
			break;
		case "SHARED_LIBRARY": //$NON-NLS-1$
			target.setType(Target.Type.SHARED_LIBRARY);
			break;
		case "STATIC_LIBRARY": //$NON-NLS-1$
			target.setType(Target.Type.STATIC_LIBRARY);
			break;
		case "OBJECT_LIBRARY": //$NON-NLS-1$
			target.setType(Target.Type.OBJECT_LIBRARY);
			break;
		case "MODULE_LIBRARY": //$NON-NLS-1$
			target.setType(Target.Type.MODULE_LIBRARY);
			break;
		case "INTERFACE_LIBRARY": //$NON-NLS-1$
			target.setType(Target.Type.INTERFACE_LIBRARY);
			break;
		case "UTILITY": //$NON-NLS-1$
			target.setType(Target.Type.UTILITY);
			break;
		default:
			throw new IllegalArgumentException("Unexpected type %s".formatted(typeString));
		}
		return target;
	}

}
