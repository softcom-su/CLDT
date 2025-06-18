package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.io.IOException;
import java.io.StringReader;
import java.util.List;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.dataformat.xml.XmlMapper;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlRootElement;

import org.eclipse.core.runtime.Platform;

import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;

public class FeaturesReadCommand extends AbstractQXferCommand {

	private static final String START_TARGET_TAG = "<target";
	private static final String END_TARGET_TAG = "</target>";

	Target target = null;

	@JacksonXmlRootElement(localName = "target")
	public static class Target {

		@JsonIgnore
		@JacksonXmlProperty(localName = "version")
		private String version;

		@JacksonXmlProperty(localName = "architecture")
		private String architecture;

		@JacksonXmlProperty(localName = "feature")
		private List<Reg> feature;

		public Target() {
		}

		public String getArchitecture() {
			return architecture;
		}

		public void setArchitecture(String architecture) {
			this.architecture = architecture;
		}

		public List<Reg> getFeature() {
			return feature;
		}

		public void setFeature(List<Reg> feature) {
			this.feature = feature;
		}

		@Override
		public String toString() {
			return "Target [architecture=" + architecture + ", feature=" + feature + "]";
		}

		@JsonIgnoreProperties(ignoreUnknown = true)
		public static class Reg {

			@JacksonXmlProperty(localName = "name", isAttribute = true)
			private String name;

			@JacksonXmlProperty(localName = "bitsize", isAttribute = true)
			private Integer bitSize;

			@JacksonXmlProperty(localName = "regnum", isAttribute = true)
			private Integer regnum;

			@JsonIgnore
			@JacksonXmlProperty(localName = "offset", isAttribute = true)
			private Integer offset;

			@JsonIgnore
			@JacksonXmlProperty(localName = "encoding", isAttribute = true)
			private String encoding;

			@JsonIgnore
			@JacksonXmlProperty(localName = "format", isAttribute = true)
			private String format;

			@JacksonXmlProperty(localName = "group", isAttribute = true)
			private String group;

			@JsonIgnore
			@JacksonXmlProperty(localName = "ehframe_regnum", isAttribute = true)
			private Integer ehframeRegnum;

			@JacksonXmlProperty(localName = "dwarf_regnum", isAttribute = true)
			private Integer dwarfRegNum;

			@JsonIgnore
			@JacksonXmlProperty(localName = "generic", isAttribute = true)
			private String generic;

			@JsonIgnore
			@JacksonXmlProperty(localName = "value_regnums", isAttribute = true)
			private Integer valueRegnums;

			@JsonIgnore
			@JacksonXmlProperty(localName = "invalidate_regnums", isAttribute = true)
			private String invalidateRegnums;

			public String getName() {
				return name;
			}

			public void setName(String name) {
				this.name = name;
			}

			public Integer getBitSize() {
				return bitSize;
			}

			public void setBitSize(Integer bitSize) {
				this.bitSize = bitSize;
			}

			public Integer getRegnum() {
				return regnum;
			}

			public void setRegnum(Integer regnum) {
				this.regnum = regnum;
			}

			public String getGroup() {
				return group;
			}

			public void setGroup(String group) {
				this.group = group;
			}

			public Integer getDwarfRegNum() {
				return dwarfRegNum;
			}

			public void setDwarfRegNum(Integer dwarfRegNum) {
				this.dwarfRegNum = dwarfRegNum;
			}

			@Override
			public String toString() {
				return "\nRegister [name=" + name + ", bitSize=" + bitSize + ", group=" + group + "]";
			}
		}
	}

	public FeaturesReadCommand(String annex, long offset, long length) {
		super("features", annex, "%s,%s".formatted(Long.toString(offset), Long.toString(length)));
	}

	@Override
	protected void doHandleResult(byte[] result) {
		String xml = new String(result);
		int startTargetIndex = xml.indexOf(START_TARGET_TAG);
		int endTargetIndex = xml.indexOf(END_TARGET_TAG) + END_TARGET_TAG.length();

		xml = xml.substring(startTargetIndex, endTargetIndex);

		XmlMapper xmlMapper = new XmlMapper();
		XMLInputFactory factory = XMLInputFactory.newDefaultFactory();

		try (StringReader reader = new StringReader(xml)) {
			XMLStreamReader xmlReader = factory.createXMLStreamReader(reader);

			target = xmlMapper.readValue(xmlReader, Target.class);
			Platform.getLog(getClass()).info("Parsed Target object:" + target);
		} catch (XMLStreamException | IOException e) {
			Platform.getLog(getClass()).error("Error parsing XML", e);
		}

	}

	public Target getTarget() {
		return target;
	}

}
