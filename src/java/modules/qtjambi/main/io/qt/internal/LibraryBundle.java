/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.internal;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import io.qt.QLibraryNotLoadedError;
import io.qt.core.QPair;

/**
 * @hidden
 */
final class LibraryBundle {
    private static final Map<String, Library> libraryMap = Collections.synchronizedMap(new HashMap<String, Library>());
    
    private LibraryBundle(){}
    
    static void deployQml() {
    	for(Library e : new ArrayList<>(libraryMap.values())) {
        	if(e.isLoaded() && e.isQmlExtracting()) {
        		try {
					e.extractQml();
				} catch (Throwable e1) {
					throw new QLibraryNotLoadedError("Unable to extract library "+e.getName(), e1);
				}
        	}
		}
    }
    
    static Library findLibrary(String lib) {
    	Library entry = libraryMap.get(lib);
    	while(entry instanceof Symlink) {
        	String target = ((Symlink)entry).getTarget();
        	if(target.startsWith("lib/") || target.startsWith("bin/")) {
        		target = target.substring(4);
        	}
        	Library eTarget = libraryMap.get(target);
        	if(eTarget==null)
        		break;
        	else
        		entry = eTarget;
        }
    	return entry;
    }
    
    static void deploy(String path) {
    	Library entry = libraryMap.get(path);
    	if(entry!=null) {
    		try {
				entry.extract();
			} catch (Throwable e) {
				Logger.getLogger("io.qt.internal").throwing(LibraryUtility.class.getName(), "extracting library", e);
			}
    	}
    }
    
    private String compiler;
	private Configuration configuration;
    private String system;
    private String version;
    private String module;
	private URL url;
    private File extractionDir;
    private boolean hasPluginPaths;
    private boolean hasQmlPaths;
    private boolean hasTrPaths;
    private boolean hasSourcePaths;
    private boolean isDebuginfo;
    private List<Library> libraries;
    private List<QPair<String,Boolean>> files;
    private List<String> qmlLibraries;

    private void addQmlLibrary(String name) {
        if (qmlLibraries == null)
        	qmlLibraries = new ArrayList<>();
    	qmlLibraries.add(name);
    }
    
    private void addLibrary(Library e) {
        if (libraries == null)
        	libraries = new ArrayList<>();
        libraries.add(e);
    }

    private void addFile(String direntAsString, boolean isExecutable) {
        if (files == null)
            files = new ArrayList<>();
        files.add(new QPair<>(direntAsString, isExecutable));
    }

    URL url() {
        return url;
    }
    
    public static LibraryBundle read(URL sourceUrl) throws ParserConfigurationException, IOException, SAXException {
    	LibraryBundle depl = new LibraryBundle();
    	depl.url = sourceUrl;
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        factory.setValidating(false);
        DocumentBuilder builder = factory.newDocumentBuilder();

        Document doc;
        try(InputStream inStream = sourceUrl.openStream()){
        	doc = builder.parse(inStream);
        }
        depl.system = doc.getDocumentElement().getAttribute("system");
        if (depl.system == null || depl.system.length() == 0) {
            throw new SpecificationException("<qtjambi-deploy> element missing required attribute 'system'");
        } else if (!depl.system.equals(LibraryUtility.osArchName)) {
            throw new WrongSystemException(String.format("Expected version: %1$s, found: %2$s.", LibraryUtility.osArchName, depl.system));
        }
        depl.version = doc.getDocumentElement().getAttribute("version");
        if (depl.version == null || depl.version.isEmpty())
            throw new SpecificationException("<qtjambi-deploy> element missing required attribute 'version'");
        depl.module = doc.getDocumentElement().getAttribute("module");
        depl.compiler = doc.getDocumentElement().getAttribute("compiler");
        if(depl.compiler!=null && depl.compiler.isEmpty())
        	depl.compiler = null;
        if(depl.module!=null && depl.module.isEmpty())
        	depl.module = null;
        String configuration = doc.getDocumentElement().getAttribute("configuration");
        if("debuginfo".equals(configuration)) {
        	depl.configuration = LibraryBundle.Configuration.Release;
        	depl.isDebuginfo = true;
        }else if(LibraryBundle.Configuration.Release.toString().equalsIgnoreCase(configuration))
        	depl.configuration = LibraryBundle.Configuration.Release;
        else if(LibraryBundle.Configuration.Debug.toString().equalsIgnoreCase(configuration))
        	depl.configuration = LibraryBundle.Configuration.Debug;
        else
        	depl.configuration = LibraryBundle.Configuration.Release;
        NodeList childNodes = doc.getDocumentElement().getChildNodes();
		for (int i = 0; i < childNodes.getLength(); i++) {
			Node child = childNodes.item(i);
			if(child instanceof Element) {
				Element element = (Element)child;
				String elementName = element.getLocalName();
				if(elementName==null)
					elementName = element.getNodeName();
				String name = element.getAttribute("name");
				if (name==null || name.isEmpty())
                    throw new SpecificationException(String.format("<%1$s> element missing required attribute \"name\"", elementName));
				if(name.startsWith("qml/")) {
					depl.hasQmlPaths = true;
                }else if(name.startsWith("plugins/")) {
                	depl.hasPluginPaths = true;
                }else if(name.startsWith("translations/")) {
                	depl.hasTrPaths = true;
                }else if(name.startsWith("sources/")) {
                	depl.hasSourcePaths = true;
                }
				Library libraryEntry;
				switch(elementName) {
				case "library":
					libraryEntry = new Library(name, depl);
					depl.addLibrary(libraryEntry);
					if(name.startsWith("plugins/containeraccess/")) {
    	                if (libraryMap.get(name)==null)
    		                libraryMap.put(name, libraryEntry);
                	}else {
	                	if(name.startsWith("lib/") || name.startsWith("bin/"))
	                		name = name.substring(4);
	                	Library old = libraryMap.get(name);
	                	if (old != null && !old.depl.url.equals(sourceUrl)) {
		                    throw new SpecificationException(String.format(
		                    		"<library> '%1$s' is duplicated. Present in both '%2$s' and '%3$s'.", 
		                    		name, sourceUrl, old.depl.url));
		                }
		                if (old==null)
			                libraryMap.put(name, libraryEntry);
                	}
					break;
				case "qmllib":
					depl.addQmlLibrary(name);
					break;
				case "file":
					depl.addFile(name, "true".equalsIgnoreCase(element.getAttribute("executable")));
					break;
				case "symlink":
					String target = element.getAttribute("target");
					if(target==null || target.isEmpty())
						throw new SpecificationException("<symlink> element missing required attribute \"target\"");
	                libraryEntry = new Symlink(name, target, depl);
	                depl.addLibrary(libraryEntry);
                	if(name.startsWith("lib/") || name.startsWith("bin/"))
                		name = name.substring(4);
	                if (libraryMap.get(name)==null)
		                libraryMap.put(name, libraryEntry);
					break;
				}
			}
		}
		return depl;
    }

    public File extractionDir() {
        return extractionDir;
    }
    public void setExtractionDir(File extractionDir) throws MalformedURLException {
        this.extractionDir = extractionDir;
    }

    List<Library> libraries() {
        return libraries == null ? Collections.emptyList() : Collections.unmodifiableList(libraries);
    }

    public List<QPair<String,Boolean>> files() {
        return files == null ? Collections.emptyList() : Collections.unmodifiableList(files);
    }

	public List<String> qmlLibraries() {
		return qmlLibraries==null ? Collections.emptyList() : Collections.unmodifiableList(qmlLibraries);
	}
	
    public String compiler() {
		return compiler;
	}

	Configuration configuration() {
		return configuration;
	}

    public String version() {
		return version;
	}

	public String module() {
		return module;
	}

	public boolean hasPluginPaths() {
		return hasPluginPaths;
	}

	public boolean hasQmlPaths() {
		return hasQmlPaths;
	}

	public boolean hasTrPaths() {
		return hasTrPaths;
	}
	
	static class Library {
	    private final String name;
	    private final LibraryBundle depl;

		Library(String name, LibraryBundle depl) {
			super();
			this.name = name;
			this.depl = depl;
		}
		
		URL source() {
			return depl.url;
		}
		
		LibraryBundle bundle() {
			return depl;
		}
	    
	    public String getName() {
			return name;
		}

	    public File extractionPath() {
	        if(depl.extractionDir() == null)
	            return null;
	        return new File(depl.extractionDir(), name);
	    }

		private boolean loaded;
	    
	    public boolean isExtracting() {
	    	synchronized(this.extractionFunctions) {
	    		return !extractionFunctions.isEmpty();
	    	}
	    }
	    
	    public boolean isQmlExtracting() {
	    	synchronized(this.qmlExtractionFunctions) {
	    		return !qmlExtractionFunctions.isEmpty();
	    	}
	    }
	    
	    public boolean isLoaded() {
	        return loaded;
	    }
	    void setLoaded(boolean loaded) {
	        this.loaded = loaded;
	        if(loaded) {
//	        	synchronized(this.extractionFunctions) {
//	        		extractionFunctions.clear();
//	        	}
//	        	synchronized(this.qmlExtractionFunctions) {
//	        		qmlExtractionFunctions.clear();
//	        	}
	        }
	    }
	    
	    interface ExtractionFunction{
	    	void extract() throws Throwable;
	    }

	    private final List<ExtractionFunction> extractionFunctions = new LinkedList<>();
	    private final List<ExtractionFunction> qmlExtractionFunctions = new LinkedList<>();

		void addExtractionFunction(ExtractionFunction loadFunction) {
			addExtractionFunctions(Collections.singletonList(loadFunction));
		}
		
		void addQmlExtractionFunction(ExtractionFunction loadFunction) {
			addQmlExtractionFunctions(Collections.singletonList(loadFunction));
		}
		
		void addExtractionFunctions(Collection<ExtractionFunction> loadFunctions) {
			synchronized(this.extractionFunctions) {
				if(this.extractionFunctions.isEmpty()) {
					this.extractionFunctions.add(()->Logger.getLogger("io.qt.internal").log(Level.FINEST, ()->String.format("extracting %1$s", name)));
				}
				this.extractionFunctions.addAll(loadFunctions);
			}
		}
		
		void addQmlExtractionFunctions(Collection<ExtractionFunction> loadFunctions) {
			synchronized(this.qmlExtractionFunctions) {
				if(this.qmlExtractionFunctions.isEmpty()) {
					this.qmlExtractionFunctions.add(()->Logger.getLogger("io.qt.internal").log(Level.FINEST, ()->String.format("extracting qml of %1$s", name)));
				}
				this.qmlExtractionFunctions.addAll(loadFunctions);
			}
		}
		
		public void extract() throws Throwable {
			ExtractionFunction first = null;
			while(true){
				synchronized(this.extractionFunctions) {
					this.extractionFunctions.remove(first);
					if(this.extractionFunctions.isEmpty())
						break;
					else
						first = this.extractionFunctions.get(0);
				}
				first.extract();
			}
		}
		
		public void extractQml() throws Throwable {
			ExtractionFunction first = null;
			while(true){
				synchronized(this.qmlExtractionFunctions) {
					this.qmlExtractionFunctions.remove(first);
					if(this.qmlExtractionFunctions.isEmpty())
						break;
					else
						first = this.qmlExtractionFunctions.get(0);
				}
				first.extract();
			}
		}
	}
	
	static class Symlink extends Library {
		Symlink(String name, String target, LibraryBundle depl) {
			super(name, depl);
			this.target = target;
		}

		private final String target;

		public String getTarget() {
			return target;
		}
	}

	/** 
	 * Enum for defining whether Qt is build in Release or Debug. 
	 */
	enum Configuration {
	    Release,
	    Debug
	}
	
	static class SpecificationException extends RuntimeException {
		private static final long serialVersionUID = 1L;

		SpecificationException(String msg) {
			super(msg);
		}
	}

	static class WrongSystemException extends SpecificationException {
		private static final long serialVersionUID = 1L;

		WrongSystemException(String msg) {
			super(msg);
		}
	}
	
	static class WrongBuildException extends SpecificationException {
		private static final long serialVersionUID = 1L;

		WrongBuildException(String msg) {
			super(msg);
		}
	}
	
	static class WrongConfigurationException extends SpecificationException {
		private static final long serialVersionUID = 1L;

		WrongConfigurationException(String msg) {
			super(msg);
		}
	}

	static class WrongVersionException extends SpecificationException {
		private static final long serialVersionUID = 1L;

		WrongVersionException(String msg) {
			super(msg);
		}
	}

	public boolean hasSourcePaths() {
		return hasSourcePaths;
	}

	public boolean isDebuginfo() {
		return isDebuginfo;
	}
}
