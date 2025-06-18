package su.softcom.cldt.debug.internal.core.gdb.commands;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;
import org.eclipse.core.runtime.Platform;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.dataformat.xml.XmlMapper;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlElementWrapper;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlProperty;
import com.fasterxml.jackson.dataformat.xml.annotation.JacksonXmlRootElement;


public class DylibReadCommand extends AbstractQXferCommand {

	public static class LibraryXML {
		
		@Override
		public String toString() {
			return "\n Library [name=" + getName() + ", lm=" + getLm() + ", l_addr=" + getAddr() + "l_ld=" + getLd() +"]";
		}

		public String getLd() {
			return ld;
		}

		public String getAddr() {
			return addr;
		}

		public String getLm() {
			return lm;
		}

		public String getName() {
			return name;
		}

		@JacksonXmlProperty(localName="name", isAttribute = true)
		private String name;
		
		@JacksonXmlProperty(localName="lm", isAttribute = true)
		private String lm;
		
		@JacksonXmlProperty(localName="l_addr", isAttribute = true)
		private String addr;
		
		@JacksonXmlProperty(localName="l_ld", isAttribute = true)
		private String ld;
	}
	
	@JacksonXmlRootElement(localName = "library-list-svr4")
	public static class LibraryDTO {
		
	    @JsonIgnore
	    @JacksonXmlProperty(localName="version")
		private String version;
		
	    @JacksonXmlElementWrapper(useWrapping = false)
	    @JacksonXmlProperty(localName = "library")
	    private List<LibraryXML> libraries;

	    public List<LibraryXML> getLibraries() {
	    	return libraries;
	    }
	}
	
	private LibraryDTO libraries = null;
	
	public DylibReadCommand() {
		super("libraries-svr4","","%s,%s".formatted(Long.toString(0), Long.toString(13000))); //FIXME packet size
	}

	@Override
	protected void doHandleResult(byte[] result) {
		String xml = new String(result);
		xml = xml.substring(1);
        XmlMapper xmlMapper = new XmlMapper();
        XMLInputFactory factory = XMLInputFactory.newDefaultFactory();
        try (StringReader reader = new StringReader(xml)) {
            XMLStreamReader xmlReader = factory.createXMLStreamReader(reader);
            LibraryDTO libDTO = xmlMapper.readValue(xmlReader, LibraryDTO.class);
            if (libDTO != null) {
            	libraries = libDTO;
            	// Add logging if needed
//            	for (LibraryXML lib: libDTO.libraries) {
//                    Platform.getLog(getClass()).info("Parsed Library object:" + lib.toString());
//            	}            	
            }
        } catch (XMLStreamException | IOException e) {
            Platform.getLog(getClass()).error("Error parsing XML", e);
        }
	}

	public LibraryDTO getDTO() {
		return libraries;
	}
	
}

