package su.softcom.cldt.internal.core.analysis.valgrind;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class ValgrindXmlParser {

	/**
	 * @param filePath
	 * @param tempExecutable
	 * @return List<ValgrindError>
	 */
	public static List<ValgrindError> parseValgrindXml(String filePath, File tempExecutable) {
		List<ValgrindError> errors = new ArrayList<>();
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			Document document = builder.parse(new File(filePath));
			document.getDocumentElement().normalize();

			NodeList errorNodes = document.getElementsByTagName("error"); //$NON-NLS-1$
			for (int i = 0; i < errorNodes.getLength(); i++) {
				Node errorNode = errorNodes.item(i);
				if (errorNode.getNodeType() == Node.ELEMENT_NODE) {
					Element errorElement = (Element) errorNode;
					ValgrindError error = new ValgrindError();
					error.stackText = new ArrayList<>();
					error.kind = getTagValue("kind", errorElement); //$NON-NLS-1$
					error.description = getTagValue("what", errorElement); //$NON-NLS-1$
					Element xwhatElement = getFirstChildElementByTagName(errorElement, "what"); //$NON-NLS-1$
					error.xwhatText = (xwhatElement != null) ? getTagValue("text", xwhatElement) : null; //$NON-NLS-1$

					NodeList frames = errorElement.getElementsByTagName("frame"); //$NON-NLS-1$

					for (int j = 0; j < frames.getLength(); j++) {
						Node frameNode = frames.item(j);
						String stack = ""; //$NON-NLS-1$
						if (j == 0) {
							stack += "at "; //$NON-NLS-1$
						} else {
							stack += "by "; //$NON-NLS-1$
						}

						if (frameNode.getNodeType() == Node.ELEMENT_NODE) {
							Element frameElement = (Element) frameNode;

							ValgrindFrame frame = new ValgrindFrame();
							frame.ip = getTagValue("ip", frameElement); //$NON-NLS-1$
							frame.obj = getTagValue("obj", frameElement); //$NON-NLS-1$
							frame.fn = getTagValue("fn", frameElement); //$NON-NLS-1$
							frame.dir = getTagValue("dir", frameElement); //$NON-NLS-1$
							frame.file = getTagValue("file", frameElement); //$NON-NLS-1$
							frame.line = getTagValue("line", frameElement); //$NON-NLS-1$
							stack += frame.ip + ": " + frame.fn + " (" + frame.dir + "/" + frame.file + ":" + frame.line //$NON-NLS-1$ //$NON-NLS-2$//$NON-NLS-3$ //$NON-NLS-4$
									+ ")"; //$NON-NLS-1$
							error.stackText.add(stack);
							if (frame.obj.equals(tempExecutable.toString())) {
								error.lineError = frame.line;
							}
							error.stackTrace.add(frame);
						}
					}
					errors.add(error);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return errors;
	}

	private static String getTagValue(String tag, Element element) {
		NodeList nodeList = element.getElementsByTagName(tag);
		if (nodeList.getLength() > 0) {
			Node node = nodeList.item(0);
			if (node != null && node.getNodeType() == Node.ELEMENT_NODE) {
				return node.getTextContent();
			}
		}
		return null;
	}

	private static Element getFirstChildElementByTagName(Element parent, String tagName) {
		NodeList nodeList = parent.getElementsByTagName(tagName);
		if (nodeList.getLength() > 0) {
			Node node = nodeList.item(0);
			if (node.getNodeType() == Node.ELEMENT_NODE) {
				return (Element) node;
			}
		}
		return null;
	}
}