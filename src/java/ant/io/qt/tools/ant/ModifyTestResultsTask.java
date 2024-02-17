package io.qt.tools.ant;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class ModifyTestResultsTask extends Task {
    
	@Override
    public void execute() throws BuildException {
		String nbsp = "_";
		String suffix = nbsp+"("+version+","+nbsp+mode;
		if(jdk!=null && !jdk.isEmpty())
			suffix += ","+nbsp+jdk;
		suffix += ")";
		try {
			java.io.File directory = new java.io.File(dir);
			if(!directory.isDirectory())
				return;
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			factory.setValidating(false);
			DocumentBuilder builder = factory.newDocumentBuilder();
			Transformer transformer = TransformerFactory.newInstance().newTransformer();
			transformer.setOutputProperty(OutputKeys.INDENT, "yes");
			transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
			transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
			loop: for(String file : directory.list()){
				if(file.startsWith("TEST") && file.endsWith(".xml")) {
					java.io.File xml = new java.io.File(directory, file);
					Document doc;
					try(FileInputStream fis = new FileInputStream(xml)){
						doc = builder.parse(fis);
					}catch(org.xml.sax.SAXParseException e) {
						System.err.println(xml.getAbsolutePath());
						e.printStackTrace(System.err);
						continue;
					}catch(IOException e) {
						throw new org.xml.sax.SAXParseException(xml.getName()+"; "+e.getMessage(), null, e);
					}
					NodeList childNodes = doc.getChildNodes();
					for(int i=0, length=childNodes.getLength(); i<length; ++i) {
						Node item = childNodes.item(i);
						if("testsuite".equals(item.getNodeName()) && item instanceof Element) {
							Element child = (Element)item;
							if(child.hasAttribute("name")) {
								String name = child.getAttribute("name");
								if(!name.endsWith(suffix))
									child.setAttribute("name", name+suffix);
								else continue loop;
							}
							
							NodeList childNodes2 = child.getChildNodes();
							for(int j=0, length2=childNodes2.getLength(); j<length2; ++j) {
								Node item2 = childNodes2.item(j);
								if("testcase".equals(item2.getNodeName()) && item2 instanceof Element) {
									Element child2 = (Element)item2;
									if(child2.hasAttribute("classname")) {
										String name = child2.getAttribute("classname");
										if(!name.endsWith(suffix))
											child2.setAttribute("classname", name+suffix);
										else continue loop;
									}
								}
							}
						}
					}
					try(FileOutputStream fos = new FileOutputStream(xml)){
						StreamResult result = new StreamResult(fos);
						transformer.transform(new DOMSource(doc), result);
					}
				}
			}
		}catch(RuntimeException | Error e){
			throw e;
		}catch(Exception e){
			throw new BuildException(e);
		}
	}
	
	private String dir, mode, jdk, version;

	public String getDir() {
		return dir;
	}

	public void setDir(String dir) {
		this.dir = dir;
	}

	public String getMode() {
		return mode;
	}

	public void setMode(String mode) {
		this.mode = mode;
	}

	public String getJdk() {
		return jdk;
	}

	public void setJdk(String jdk) {
		this.jdk = jdk;
	}

	public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version.replace(".", Character.toString(0x2219));
	}
}
