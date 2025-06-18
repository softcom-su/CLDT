package su.softcom.cldt.internal.core.cmake.fileapi;

import java.io.IOException;
import java.io.InputStream;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.runtime.CoreException;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.ObjectCodec;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JavaType;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import com.fasterxml.jackson.databind.module.SimpleModule;

import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;

public class TargetDeserializer extends StdDeserializer<Target> {

	final ICMakeProject project;
	final IFolder replyFolder;

	public TargetDeserializer(ICMakeProject project, IFolder replyFolder) {
		this(null, project, replyFolder);
	}

	public TargetDeserializer(JavaType valueType, ICMakeProject project, IFolder replyFolder) {
		super(valueType);
		this.project = project;
		this.replyFolder = replyFolder;
	}

	@Override
	public Target deserialize(JsonParser p, DeserializationContext ctxt) throws IOException {
		ObjectCodec codec = p.getCodec();
		JsonNode node = codec.readTree(p);
		String targetName = node.get("name").asText(); //$NON-NLS-1$
		Target target = project.getTarget(targetName);
		if (target == null) {
			target = new Target(targetName, project); // $NON-NLS-1$
			project.addTarget(target);
		}
		String fileName = node.get("jsonFile").asText(); //$NON-NLS-1$
		IFile targetFile = replyFolder.getFile(fileName);
		try (InputStream stream = targetFile.getContents()) {
			ObjectMapper mapper = new ObjectMapper();
			SimpleModule module = new SimpleModule();
			module.addDeserializer(Target.class, new TargetFileDeserializer(target));
			mapper.registerModule(module);
			mapper.readValue(stream.readAllBytes(), Target.class);
		} catch (CoreException e) {
			e.printStackTrace();
		}
		return null;
	}

}
