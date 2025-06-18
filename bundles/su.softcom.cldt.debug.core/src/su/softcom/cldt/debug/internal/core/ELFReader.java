package su.softcom.cldt.debug.internal.core;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import su.softcom.cldt.debug.core.ELF;
import su.softcom.cldt.debug.core.ELF.TableSectionHeaderItem;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo;

/**
 * 
 */
public class ELFReader {
	
	private ELFReader () {
		// empty
	}
 
	/**
	 * 
	 * @param file java.io ELF файл, который нужно считать. 
	 * @return  {@link ELF} объект
	 * @throws IOException если файл не существует или не найден в системе
	 */
	public static ELF readELFFile(File file) throws IOException {
		if (!file.exists()) {
			throw new FileNotFoundException();
		}
		ELF elfFile;
		try (FileInputStream fis = new FileInputStream(file)) {
			// Чтение ELF-заголовка
			byte[] headerBuffer = new byte[64];
			fis.read(headerBuffer, 0, headerBuffer.length);
			elfFile = new ELF(headerBuffer);
			elfFile.readSectionHeaders(readSections(elfFile, fis, headerBuffer));
			elfFile.readSectionHeaderNames(readShstrtab(elfFile, fis));
			elfFile.setDebugSections(readDebugSections(elfFile, fis));
			elfFile.setFrameSection(readFrameSection(elfFile, fis));
			readStrTabs(elfFile, fis);
			readSymTabSections(elfFile, fis);
		}
		return elfFile;
	}

	private static void readSymTabSections(ELF elfFile, FileInputStream fis) throws IOException {
		List<TableSectionHeaderItem> symTabSectionsHeaders = elfFile.getSymTabSections();
		for (TableSectionHeaderItem item : symTabSectionsHeaders) {
			fis.getChannel().position(item.getOffset());
			byte[] symTabData = new byte[(int) item.getSize()];
			fis.read(symTabData, 0, symTabData.length);
			elfFile.addSymTabSection(item.getLink(), symTabData);
		}
	}

	private static byte[] readSections(ELF elfFile, FileInputStream fis, byte[] headerBuffer) throws IOException {
		int sectionTableSize = elfFile.calculateTableSectionSize();
		long sectionTableOffset = elfFile.getSectionHeaderOffset();
		fis.skip(sectionTableOffset - headerBuffer.length);
		byte[] sectionTableBuffer = new byte[sectionTableSize];
		fis.read(sectionTableBuffer, 0, sectionTableSize);
		return sectionTableBuffer;
	}

	private static byte[] readShstrtab(ELF elfFile, FileInputStream fis) throws IOException {
		TableSectionHeaderItem shstrtab = elfFile.getSectionHeaderStrTab();
		fis.getChannel().position(shstrtab.getOffset());
		byte[] shstrtabBuffer = new byte[(int) shstrtab.getSize()];
		fis.read(shstrtabBuffer, 0, shstrtabBuffer.length);
		return shstrtabBuffer;
	}

	private static Map<String, byte[]> readDebugSections(ELF elfFile, FileInputStream fis) throws IOException {
		List<TableSectionHeaderItem> debugSectionsHeaders = elfFile.readDebugSections();
		Map<String, byte[]> debugSections = new HashMap<>();
		for (TableSectionHeaderItem item : debugSectionsHeaders) {
			fis.getChannel().position(item.getOffset());
			byte[] debugSectionData = new byte[(int) item.getSize()];
			fis.read(debugSectionData, 0, debugSectionData.length);
			debugSections.put(item.getName(), debugSectionData);
		}
		return debugSections;
	}
	
//  TODO: Использовaть .eh_frame_hdr. Позволит быстро искать FDE.
//  ByteBuffer frameHeader = wrapOptionalSection(".eh_frame_hdr");
//  readFrameHeader(frameHeader);
	private static byte[] readFrameSection(ELF elfFile, FileInputStream fis) throws IOException {
		List<TableSectionHeaderItem> frameSectionsHeaders = elfFile.readFrameSections();
		byte[] ehFreameSection = new byte[0];
		if (frameSectionsHeaders.isEmpty()) {
			return ehFreameSection;
		}
		for (TableSectionHeaderItem item : frameSectionsHeaders) {
			fis.getChannel().position(item.getOffset());
			ehFreameSection = new byte[(int) item.getSize()];
			fis.read(ehFreameSection, 0, ehFreameSection.length);
		}
		long startEHFrameSection = frameSectionsHeaders.get(0).getAddress();
		ByteBuffer data = ByteBuffer.wrap(ehFreameSection).order(elfFile.getEndian());
		elfFile.setStackInfo(new CallFrameInfo(data, startEHFrameSection));
		return ehFreameSection;
	}


	private static void readStrTabs(ELF elfFile, FileInputStream fis) throws IOException {
		List<TableSectionHeaderItem> strSections = elfFile.getStrTabSections();
		for (TableSectionHeaderItem strSection: strSections) {
			long index = strSection.getIndex();
			fis.getChannel().position(strSection.getOffset());
			byte[] strtabBuffer = new byte[(int) strSection.getSize()];
			fis.read(strtabBuffer, 0, strtabBuffer.length);
			elfFile.addStrTabSection(index, strtabBuffer);
		}
	}

}
