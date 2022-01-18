package io.qt.tools.ant;

import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.Task;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class CreatePOMTask extends Task {

	public String getLibraries() {
		return libraries;
	}
	public void setLibraries(String libraries) {
		this.libraries = libraries;
	}
	
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public String getOutputDirectory() {
		return outputDirectory;
	}
	public void setOutputDirectory(String outputDirectory) {
		this.outputDirectory = outputDirectory;
	}

	public void setDependencies(String dependencies) {
		this.dependencies = dependencies;
	}

	public String getModule() {
		return moduleName;
	}
	public void setModule(String moduleName) {
		this.moduleName = moduleName;
	}

    private String msg = "";
    
	@Override
    public void execute() throws BuildException {
        getProject().log(this, msg, Project.MSG_INFO);
		try {
			String qtjambiVersion = getProject().getProperty("qtjambi.jar.version");
			if("true".equalsIgnoreCase(getProject().getProperty("qtjambi-deploy-snapshot"))
					|| "yes".equalsIgnoreCase(getProject().getProperty("qtjambi-deploy-snapshot"))) {
				qtjambiVersion += "-SNAPSHOT";
			}
			java.io.File directory = new java.io.File(outputDirectory);
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			factory.setValidating(false);
			DocumentBuilder builder = factory.newDocumentBuilder();
			Transformer transformer = TransformerFactory.newInstance().newTransformer();
			transformer.setOutputProperty(OutputKeys.INDENT, "yes");
			transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
			transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
			if("parent".equals(moduleName)){
				Document doc = builder.getDOMImplementation().createDocument(
						"http://maven.apache.org/POM/4.0.0",
						"project",
						null
					);
				doc.getDocumentElement().setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
				doc.getDocumentElement().setAttribute("xsi:schemaLocation", "http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd");
				Element modelVersion = doc.createElement("modelVersion");
				modelVersion.setTextContent("4.0.0");
				doc.getDocumentElement().appendChild(modelVersion);
				Element groupId = doc.createElement("groupId");
				groupId.setTextContent("io.qtjambi");
				doc.getDocumentElement().appendChild(groupId);
				Element artifactId = doc.createElement("artifactId");
				artifactId.setTextContent("qtjambi-parent");
				doc.getDocumentElement().appendChild(artifactId);
				Element name = doc.createElement("name");
				name.setTextContent("QtJambi Parent");
				doc.getDocumentElement().appendChild(name);
				Element version = doc.createElement("version");
				version.setTextContent(qtjambiVersion);
				doc.getDocumentElement().appendChild(version);
				Element description = doc.createElement("description");
				description.setTextContent("QtJambi");
				doc.getDocumentElement().appendChild(description);				
				Element url = doc.createElement("url");
				url.setTextContent("https://www.qtjambi.io");
				doc.getDocumentElement().appendChild(url);
				Element packaging = doc.createElement("packaging");
				packaging.setTextContent("pom");
				doc.getDocumentElement().appendChild(packaging);
				Element licenses = doc.createElement("licenses");
				Element license = doc.createElement("license");
				Element lname = doc.createElement("name");
				lname.setTextContent("GNU Lesser General Public License 2.1");
				license.appendChild(lname);
				Element lurl = doc.createElement("url");
				lurl.setTextContent("http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html");
				license.appendChild(lurl);
				licenses.appendChild(license);
				license = doc.createElement("license");
				lname = doc.createElement("name");
				lname.setTextContent("GNU General Public License 3");
				license.appendChild(lname);
				lurl = doc.createElement("url");
				lurl.setTextContent("http://www.gnu.org/copyleft/gpl.html");
				license.appendChild(lurl);
				licenses.appendChild(license);
				doc.getDocumentElement().appendChild(licenses);
				Element scm = doc.createElement("scm");
				Element connection = doc.createElement("connection");
				connection.setTextContent("scm:git:git@github.com:OmixVisualization/qtjambi.git");
				scm.appendChild(connection);
				Element developerConnection = doc.createElement("developerConnection");
				developerConnection.setTextContent("scm:git:ssh://github.com:OmixVisualization/qtjambi.git");
				scm.appendChild(developerConnection);
				Element surl = doc.createElement("url");
				surl.setTextContent("https://github.com/OmixVisualization/qtjambi/tree/master");
				scm.appendChild(surl);
				doc.getDocumentElement().appendChild(scm);
				
				Element developers = doc.createElement("developers");
				Element developer = doc.createElement("developer");
				Element id = doc.createElement("id");
				id.setTextContent("docjambi");
				developer.appendChild(id);
				Element devname = doc.createElement("name");
				devname.setTextContent("Dr. Peter Droste");
				developer.appendChild(devname);
				Element email = doc.createElement("email");
				email.setTextContent("info@qtjambi.io");
				developer.appendChild(email);
				developers.appendChild(developer);
				doc.getDocumentElement().appendChild(developers);
				
				Element modules = doc.createElement("modules");
				for(java.io.File file : directory.listFiles()) {
					if(file.getName().endsWith("-"+qtjambiVersion+".pom") && !file.getName().startsWith("qtjambi-parent-")) {
						Element moduleEl = doc.createElement("module");
						moduleEl.setTextContent(file.getName().substring(0, file.getName().length()-("-"+qtjambiVersion+".pom").length()));
						modules.appendChild(moduleEl);
					}
				}
				if(modules.hasChildNodes())
					doc.getDocumentElement().appendChild(modules);
								
				try(FileOutputStream fos = new FileOutputStream(new java.io.File(directory, "qtjambi-parent-"+qtjambiVersion+".pom"))){
					StreamResult result = new StreamResult(fos);
					transformer.transform(new DOMSource(doc), result);
				}
			}else{
				List<String> libraries = new ArrayList<>();
				{
					if(this.libraries!=null && !this.libraries.isEmpty()) {
						for(String line : this.libraries.split(",")) {
							libraries.add(line.trim());
						}
					}
				}
				String _moduleId = moduleName.replace('.', '-');
				String _moduleName = "${project.groupId}:${project.artifactId}";
				if(getProject().getProperty("module-name")!=null && !getProject().getProperty("module-name").isEmpty()) {
					_moduleName = getProject().getProperty("module-name");
				}else {
					for(String line : libraries) {
						if(line.startsWith("QtJambi"))
							_moduleName = line.replace("QtJambi", "QtJambi ");
						else
							_moduleName = line;
						break;
					}
				}
				{
					Document doc = builder.getDOMImplementation().createDocument(
							"http://maven.apache.org/POM/4.0.0",
							"project",
							null
						);
					doc.getDocumentElement().setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
					doc.getDocumentElement().setAttribute("xsi:schemaLocation", "http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd");
					Element modelVersion = doc.createElement("modelVersion");
					modelVersion.setTextContent("4.0.0");
					doc.getDocumentElement().appendChild(modelVersion);
					Element groupId = doc.createElement("groupId");
					groupId.setTextContent("io.qtjambi");
					doc.getDocumentElement().appendChild(groupId);
					Element artifactId = doc.createElement("artifactId");
					artifactId.setTextContent(_moduleId);
					doc.getDocumentElement().appendChild(artifactId);
					Element name = doc.createElement("name");
					name.setTextContent(_moduleName);
					doc.getDocumentElement().appendChild(name);
					Element version = doc.createElement("version");
					version.setTextContent(qtjambiVersion);
					doc.getDocumentElement().appendChild(version);
					if(this.description!=null) {
						Element description = doc.createElement("description");
						this.description = this.description.trim();
						if(this.description.startsWith("\"") && this.description.endsWith("\"")) {
							this.description = this.description.substring(1, this.description.length()-1);
						}
						if(this.description.startsWith("<p>")) {
							this.description = this.description.substring(3);
							int idx = this.description.indexOf("</p>");
							if(idx>=0) {
								this.description = this.description.substring(0, idx);
							}
						}
						description.setTextContent(this.description);
						doc.getDocumentElement().appendChild(description);				
					}
					Element url = doc.createElement("url");
					url.setTextContent("https://www.qtjambi.io");
					doc.getDocumentElement().appendChild(url);
					Element packaging = doc.createElement("packaging");
					packaging.setTextContent("jar");
					doc.getDocumentElement().appendChild(packaging);
					Element licenses = doc.createElement("licenses");
					Element license = doc.createElement("license");
					Element lname = doc.createElement("name");
					lname.setTextContent("GNU Lesser General Public License 2.1");
					license.appendChild(lname);
					Element lurl = doc.createElement("url");
					lurl.setTextContent("http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html");
					license.appendChild(lurl);
					licenses.appendChild(license);
					license = doc.createElement("license");
					lname = doc.createElement("name");
					lname.setTextContent("GNU General Public License 3");
					license.appendChild(lname);
					lurl = doc.createElement("url");
					lurl.setTextContent("http://www.gnu.org/copyleft/gpl.html");
					license.appendChild(lurl);
					licenses.appendChild(license);
					doc.getDocumentElement().appendChild(licenses);
					Element scm = doc.createElement("scm");
					Element connection = doc.createElement("connection");
					connection.setTextContent("scm:git:git@github.com:OmixVisualization/qtjambi.git");
					scm.appendChild(connection);
					Element developerConnection = doc.createElement("developerConnection");
					developerConnection.setTextContent("scm:git:ssh://github.com:OmixVisualization/qtjambi.git");
					scm.appendChild(developerConnection);
					Element surl = doc.createElement("url");
					surl.setTextContent("https://github.com/OmixVisualization/qtjambi/tree/master");
					scm.appendChild(surl);
					doc.getDocumentElement().appendChild(scm);
					
					Element developers = doc.createElement("developers");
					Element developer = doc.createElement("developer");
					Element id = doc.createElement("id");
					id.setTextContent("docjambi");
					developer.appendChild(id);
					Element devname = doc.createElement("name");
					devname.setTextContent("Dr. Peter Droste");
					developer.appendChild(devname);
					Element email = doc.createElement("email");
					email.setTextContent("info@qtjambi.io");
					developer.appendChild(email);
					developers.appendChild(developer);
					doc.getDocumentElement().appendChild(developers);
					
					if(!libraries.isEmpty()){
						Element profiles = doc.createElement("profiles");
						{
							Element profile = doc.createElement("profile");
							Element pid = doc.createElement("id");
							pid.setTextContent("windows");
							profile.appendChild(pid);
							Element activation = doc.createElement("activation");
							Element os = doc.createElement("os");
							Element family = doc.createElement("family");
							family.setTextContent("Windows");
							os.appendChild(family);
							Element arch = doc.createElement("arch");
							arch.setTextContent("amd64");
							os.appendChild(arch);
							activation.appendChild(os);
							profile.appendChild(activation);
							Element dependencies = doc.createElement("dependencies");
							profile.appendChild(dependencies);
							Element dependencyEl = doc.createElement("dependency");
							Element dgroupId = doc.createElement("groupId");
							dgroupId.setTextContent("io.qtjambi");
							dependencyEl.appendChild(dgroupId);
							Element dartifactId = doc.createElement("artifactId");
							dartifactId.setTextContent(_moduleId+"-native-windows-x64");
							dependencyEl.appendChild(dartifactId);
							Element dversion = doc.createElement("version");
							dversion.setTextContent(qtjambiVersion);
							dependencyEl.appendChild(dversion);
							Element opt = doc.createElement("optional");
							opt.setTextContent("true");
							dependencyEl.appendChild(opt);
							dependencies.appendChild(dependencyEl);
							profiles.appendChild(profile);
						}
						{
							Element profile = doc.createElement("profile");
							Element pid = doc.createElement("id");
							pid.setTextContent("linux64");
							profile.appendChild(pid);
							Element activation = doc.createElement("activation");
							Element os = doc.createElement("os");
							Element family = doc.createElement("name");
							family.setTextContent("Linux");
							os.appendChild(family);
							Element arch = doc.createElement("arch");
							arch.setTextContent("amd64");
							os.appendChild(arch);
							activation.appendChild(os);
							profile.appendChild(activation);
							Element dependencies = doc.createElement("dependencies");
							profile.appendChild(dependencies);
							Element dependencyEl = doc.createElement("dependency");
							Element dgroupId = doc.createElement("groupId");
							dgroupId.setTextContent("io.qtjambi");
							dependencyEl.appendChild(dgroupId);
							Element dartifactId = doc.createElement("artifactId");
							dartifactId.setTextContent(_moduleId+"-native-linux-x64");
							dependencyEl.appendChild(dartifactId);
							Element dversion = doc.createElement("version");
							dversion.setTextContent(qtjambiVersion);
							dependencyEl.appendChild(dversion);
							Element opt = doc.createElement("optional");
							opt.setTextContent("true");
							dependencyEl.appendChild(opt);
							dependencies.appendChild(dependencyEl);
							profiles.appendChild(profile);
						}
						{
							Element profile = doc.createElement("profile");
							Element pid = doc.createElement("id");
							pid.setTextContent("mac");
							profile.appendChild(pid);
							Element activation = doc.createElement("activation");
							Element os = doc.createElement("os");
							Element family = doc.createElement("family");
							family.setTextContent("mac");
							os.appendChild(family);
							activation.appendChild(os);
							profile.appendChild(activation);
							Element dependencies = doc.createElement("dependencies");
							profile.appendChild(dependencies);
							Element dependencyEl = doc.createElement("dependency");
							Element dgroupId = doc.createElement("groupId");
							dgroupId.setTextContent("io.qtjambi");
							dependencyEl.appendChild(dgroupId);
							Element dartifactId = doc.createElement("artifactId");
							dartifactId.setTextContent(_moduleId+"-native-macos");
							dependencyEl.appendChild(dartifactId);
							Element dversion = doc.createElement("version");
							dversion.setTextContent(qtjambiVersion);
							dependencyEl.appendChild(dversion);
							Element opt = doc.createElement("optional");
							opt.setTextContent("true");
							dependencyEl.appendChild(opt);
							dependencies.appendChild(dependencyEl);
							profiles.appendChild(profile);
						}
						doc.getDocumentElement().appendChild(profiles);
					}
		
					List<String> dependenciesList = new ArrayList<>();
					List<Element> dependencyElements = new ArrayList<>();
					Element dependencies = doc.createElement("dependencies");
					if(this.dependencies!=null && !this.dependencies.isEmpty()){
						for(String dependency : this.dependencies.split(",")) {
							dependency = dependency.trim().replace('.', '-');
							if(!dependency.isEmpty() && !dependency.equals(_moduleId) && !dependenciesList.contains(dependency)) {
								dependenciesList.add(dependency);
								Element dependencyEl = doc.createElement("dependency");
								Element dgroupId = doc.createElement("groupId");
								dgroupId.setTextContent("io.qtjambi");
								dependencyEl.appendChild(dgroupId);
								Element dartifactId = doc.createElement("artifactId");
								dartifactId.setTextContent(dependency);
								dependencyEl.appendChild(dartifactId);
								dependencyElements.add(dartifactId);
								Element dversion = doc.createElement("version");
								dversion.setTextContent(qtjambiVersion);
								dependencyEl.appendChild(dversion);
								if(!"qtjambi".equals(dependency) && !"qtjambi-jre8".equals(dependency)) {
									Element opt = doc.createElement("optional");
									opt.setTextContent("true");
									dependencyEl.appendChild(opt);
								}
								dependencies.appendChild(dependencyEl);
							}
						}
					}
					if(dependencies.hasChildNodes())
						doc.getDocumentElement().appendChild(dependencies);
					
					Element parent = doc.createElement("parent");
					Element pgroupId = doc.createElement("groupId");
					pgroupId.setTextContent("io.qtjambi");
					parent.appendChild(pgroupId);
					Element partifactId = doc.createElement("artifactId");
					partifactId.setTextContent("qtjambi-parent");
					parent.appendChild(partifactId);
					Element dversion = doc.createElement("version");
					dversion.setTextContent(qtjambiVersion);
					parent.appendChild(dversion);
					doc.getDocumentElement().appendChild(parent);
					
					try(FileOutputStream fos = new FileOutputStream(new java.io.File(directory, _moduleId+"-"+qtjambiVersion+".pom"))){
						StreamResult result = new StreamResult(fos);
						transformer.transform(new DOMSource(doc), result);
					}
					
					artifactId.setTextContent(_moduleId+"-jre8");
					name.setTextContent(_moduleName + " for Java 8");
					for(Element element : dependencyElements) {
						element.setTextContent(element.getTextContent()+"-jre8");
					}
					try(FileOutputStream fos = new FileOutputStream(new java.io.File(directory, _moduleId+"-jre8-"+qtjambiVersion+".pom"))){
						StreamResult result = new StreamResult(fos);
						transformer.transform(new DOMSource(doc), result);
					}
				}
				
				/*platform*/{
					Document doc = builder.getDOMImplementation().createDocument(
							"http://maven.apache.org/POM/4.0.0",
							"project",
							null
						);
					doc.getDocumentElement().setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
					doc.getDocumentElement().setAttribute("xsi:schemaLocation", "http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd");
					Element modelVersion = doc.createElement("modelVersion");
					modelVersion.setTextContent("4.0.0");
					doc.getDocumentElement().appendChild(modelVersion);
					Element groupId = doc.createElement("groupId");
					groupId.setTextContent("io.qtjambi");
					doc.getDocumentElement().appendChild(groupId);
					Element artifactId = doc.createElement("artifactId");
					artifactId.setTextContent(_moduleId+"-native-windows-x64");
					doc.getDocumentElement().appendChild(artifactId);
					Element name = doc.createElement("name");
					name.setTextContent(_moduleName+" native components for Windows");
					doc.getDocumentElement().appendChild(name);
					Element version = doc.createElement("version");
					version.setTextContent(qtjambiVersion);
					doc.getDocumentElement().appendChild(version);
					Element description = doc.createElement("description");
					description.setTextContent("Native components for Windows");
					doc.getDocumentElement().appendChild(description);				
					Element url = doc.createElement("url");
					url.setTextContent("https://www.qtjambi.io");
					doc.getDocumentElement().appendChild(url);
					Element packaging = doc.createElement("packaging");
					packaging.setTextContent("jar");
					doc.getDocumentElement().appendChild(packaging);
					Element licenses = doc.createElement("licenses");
					Element license = doc.createElement("license");
					Element lname = doc.createElement("name");
					lname.setTextContent("GNU Lesser General Public License 2.1");
					license.appendChild(lname);
					Element lurl = doc.createElement("url");
					lurl.setTextContent("http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html");
					license.appendChild(lurl);
					licenses.appendChild(license);
					license = doc.createElement("license");
					lname = doc.createElement("name");
					lname.setTextContent("GNU General Public License 3");
					license.appendChild(lname);
					lurl = doc.createElement("url");
					lurl.setTextContent("http://www.gnu.org/copyleft/gpl.html");
					license.appendChild(lurl);
					licenses.appendChild(license);
					doc.getDocumentElement().appendChild(licenses);
					Element scm = doc.createElement("scm");
					Element connection = doc.createElement("connection");
					connection.setTextContent("scm:git:git@github.com:OmixVisualization/qtjambi.git");
					scm.appendChild(connection);
					Element developerConnection = doc.createElement("developerConnection");
					developerConnection.setTextContent("scm:git:ssh://github.com:OmixVisualization/qtjambi.git");
					scm.appendChild(developerConnection);
					Element surl = doc.createElement("url");
					surl.setTextContent("https://github.com/OmixVisualization/qtjambi/tree/master");
					scm.appendChild(surl);
					doc.getDocumentElement().appendChild(scm);
					
					Element developers = doc.createElement("developers");
					Element developer = doc.createElement("developer");
					Element id = doc.createElement("id");
					id.setTextContent("docjambi");
					developer.appendChild(id);
					Element devname = doc.createElement("name");
					devname.setTextContent("Dr. Peter Droste");
					developer.appendChild(devname);
					Element email = doc.createElement("email");
					email.setTextContent("info@qtjambi.io");
					developer.appendChild(email);
					developers.appendChild(developer);
					doc.getDocumentElement().appendChild(developers);
									
					Element parent = doc.createElement("parent");
					Element pgroupId = doc.createElement("groupId");
					pgroupId.setTextContent("io.qtjambi");
					parent.appendChild(pgroupId);
					Element partifactId = doc.createElement("artifactId");
					partifactId.setTextContent("qtjambi-parent");
					parent.appendChild(partifactId);
					Element dversion = doc.createElement("version");
					dversion.setTextContent(qtjambiVersion);
					parent.appendChild(dversion);
					doc.getDocumentElement().appendChild(parent);
					
					try(FileOutputStream fos = new FileOutputStream(new java.io.File(directory, _moduleId+"-native-windows-x64-"+qtjambiVersion+".pom"))){
						StreamResult result = new StreamResult(fos);
						transformer.transform(new DOMSource(doc), result);
					}
					
					artifactId.setTextContent(_moduleId+"-native-linux-x64");
					name.setTextContent(_moduleName+" native components for Linux x64");
					description.setTextContent("Native components for Linux x64");
					try(FileOutputStream fos = new FileOutputStream(new java.io.File(directory, _moduleId+"-native-linux-x64-"+qtjambiVersion+".pom"))){
						StreamResult result = new StreamResult(fos);
						transformer.transform(new DOMSource(doc), result);
					}
					
					artifactId.setTextContent(_moduleId+"-native-macos");
					name.setTextContent(_moduleName+" native components for macOS");
					description.setTextContent("Native components for macOS");
					try(FileOutputStream fos = new FileOutputStream(new java.io.File(directory, _moduleId+"-native-macos-"+qtjambiVersion+".pom"))){
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
	
	private String description;
	private String libraries;
	private String moduleName;
	private String outputDirectory;
	private String dependencies;
}
