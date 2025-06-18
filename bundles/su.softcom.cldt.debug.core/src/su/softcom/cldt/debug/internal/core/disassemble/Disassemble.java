package su.softcom.cldt.debug.internal.core.disassemble;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;

import su.softcom.cldt.common.preferences.PreferenceConstants;
import su.softcom.cldt.debug.core.IAddressable;
import su.softcom.cldt.debug.core.IDebugEndpoint;
import su.softcom.cldt.debug.internal.core.disassemble.Section.Subsection;
import su.softcom.cldt.debug.internal.core.disassemble.Section.Subsection.Instruction;

public class Disassemble {

	List<Section> sections;
	IDebugEndpoint endpoint;

	public Disassemble(IDebugEndpoint endpoint) {
		this.endpoint = endpoint;
		this.sections = new ArrayList<>();
	}

	public String getLlvmObjdumpPath(String llvmPath) {
		String lldbPath = llvmPath.length() > 0 ? llvmPath + "/bin/" : "";
		return lldbPath + "llvm-objdump";
	}

	public void readInstructions() throws IOException {
		IEclipsePreferences preferences = InstanceScope.INSTANCE.getNode(PreferenceConstants.NODE);

		List<String> args = new ArrayList<>();
		Platform.getLog(getClass()).info(getLlvmObjdumpPath(preferences.get(PreferenceConstants.CLANG_PATH, "")));
		args.add(getLlvmObjdumpPath(preferences.get(PreferenceConstants.CLANG_PATH, "")));
		args.add("-d");
		args.add(endpoint.getDebugProgram().getPath());
		ProcessBuilder builder = new ProcessBuilder(args);
		Process process = builder.start();

		Section section = null;
		Subsection currentSubsection = null;
		boolean isTextSection = false;

		Pattern sectionPattern = Pattern.compile("Disassembly of section (.+):");
		Pattern instructionPattern = Pattern.compile("\\s*([0-9a-fA-F]+):\\s+([0-9a-fA-F ]+)\\s+(.*)");

		try (BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()))) {
			String line;

			while ((line = reader.readLine()) != null) {
				if (line.startsWith("Disassembly of section")) {
					Matcher sectionMatcher = sectionPattern.matcher(line);
					if (sectionMatcher.find()) {
						String sectionName = sectionMatcher.group(1);

						if (".text".equals(sectionName)) {
							isTextSection = true;
							section = new Section(sectionName);
							sections.add(section);
							currentSubsection = null;
						} else if (isTextSection) {
							break;
						}
					}
					continue;
				}

				if (isTextSection) {
					if (line.endsWith(":") && line.matches("^[0-9a-fA-F]+.*")) {
						String[] parts = line.split("\\s+", 2);
						Long startAddress = Long.parseLong(parts[0], 16);
						String name = parts[1].substring(0, parts[1].length() - 1);

						if (section.getStartAddress() == null) {
							section.setStartAddress(startAddress);
						}

						currentSubsection = section.new Subsection(section, startAddress, name);
						section.addSubsection(startAddress, currentSubsection);
					} else {
						Matcher instructionMatcher = instructionPattern.matcher(line);
						if (instructionMatcher.matches() && currentSubsection != null) {
							Long instructionAddress = Long.parseLong(instructionMatcher.group(1), 16);
							String opcodesStr = instructionMatcher.group(2).replaceAll(" ", "");
							int[] opcodes = hexStringToIntArray(opcodesStr);

							String fullInstruction = instructionMatcher.group(3);
							String instructionName = fullInstruction.split("\\s+")[0];

							Instruction instruction = currentSubsection.new Instruction(opcodes, instructionName,
									fullInstruction);
							currentSubsection.addInstruction(instructionAddress, instruction);
						}
					}
				}
			}
		}
	}

	private <K extends IAddressable> K getObjectByAddress(List<K> list, Long address) {
		for (K object : list) {
			if ((object.getStartAddress() <= address) && (object.getEndAddress() > address)) {
				return object;
			}
		}
		return null;
	}

	public Subsection getSubsection(Long address) {
		Section section = getObjectByAddress(sections, address);
		if (section != null) {
			List<Subsection> subsections = section.getSubsections().values().stream().collect(Collectors.toList());
			return getObjectByAddress(subsections, address);
		}
		return null;
	}

	public Entry<Long, Instruction> getNextEntry(Long address) {
		Subsection subsection = getSubsection(address);
		if (subsection != null) {
			return subsection.getNextEntry(address);
		}
		return null;
	}

	public Entry<Long, Instruction> getCurrentEntry(Long address) {
		Subsection subsection = getSubsection(address);
		if (subsection != null) {
			return subsection.getCurrentEntry(address);
		}
		return null;
	}

	public static int[] hexStringToIntArray(String s) {
		int len = s.length();
		int[] data = new int[len / 2];
		for (int i = 0; i < len; i += 2) {
			data[i / 2] = Integer.parseInt(s.substring(i, i + 2), 16);
		}
		return data;
	}

	@Override
	public String toString() {
		return "Disassemble [sections=" + sections + "]";
	}

}
