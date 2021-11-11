package io.qt.qtjambi.deployer;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.AbstractMap.SimpleEntry;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.IntFunction;
import java.util.jar.Attributes;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import io.qt.QtLongEnumerator;
import io.qt.core.QByteArray;
import io.qt.core.QCborArray;
import io.qt.core.QCborMap;
import io.qt.core.QCborValue;
import io.qt.core.QCoreApplication;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileDevice;
import io.qt.core.QIODevice;
import io.qt.core.QJsonDocument;
import io.qt.core.QJsonObject;
import io.qt.core.QJsonParseError.ParseError;
import io.qt.core.QStringList;
import io.qt.internal.QtJambiPlugins;

public class Main {
	
	private static class PluginClassLoader extends URLClassLoader{
		public PluginClassLoader() {
			super(new URL[0]);
		}
		protected void addURLs(Collection<URL> urls) {
			for(URL url : urls) {
				super.addURL(url);
			}
		}
	}
	
	enum JVMDetectionModes implements QtLongEnumerator{
	    AutoDetect,
	    UseRelativePath
	};

	enum Parameters implements QtLongEnumerator{
	    JVMDetectionMode,
	    JVMPath,
	    MinimumJVM,
	    MainClass,
	    ClassPath,
	    ModulePath,
	    LibraryPath,
	    JVMArguments,
	    JNIMinimumVersion
	};
	
	public static void main(String[] args) throws InterruptedException, IOException {
		QCoreApplication.initialize("QtJambiDeployer", args);
		{
			if(args.length==0) {
				System.err.println("No arguments given.");
				showInfo();
				System.exit(-1);
			}
			switch(args[0]) {
			case "plugin":
			{
				String iid = null;
				String className = null;
				String pluginName = null;
				File dir = null;
				QJsonObject metaData = null;
				List<String> classPath = new ArrayList<>();
				List<Map.Entry<String,URL>> libraries = new ArrayList<>();
	            Boolean isDebug = null;
				if(args.length==0) {
					System.err.println("No arguments given.");
					showInfo();
					return;
				}
				for (int i = 1; i < args.length; i++) {
					if(args[i].startsWith("--help") 
							|| args[i].startsWith("-h")
							|| args[i].startsWith("-?")) {
						showInfo();
						return;
					}else if(args[i].startsWith("--iid=")) {
						iid = args[i].substring(6);
					}else if(args[i].startsWith("--configuration=")) {
						String conf = args[i].substring(16);
	                    isDebug = "debug".equals(conf);
					}else if(args[i].startsWith("--dir=")) {
						dir = new File(args[i].substring(6));
					}else if(args[i].startsWith("--class-name=")) {
						className = args[i].substring(13);
					}else if(args[i].startsWith("--plugin-name=")) {
						pluginName = args[i].substring(14);
					}else if(args[i].startsWith("--class-path=")) {
						Collections.addAll(classPath, args[i].substring(13).split(File.pathSeparator));
					}else if(args[i].startsWith("--plugin-library=")) {
						String[] libinfo = args[i].substring(19).split(File.pathSeparator);
						if(libinfo.length==2){
							File libFile = new File(libinfo[1]);
							if(!libFile.isFile()) {
								throw new Error("Specified qtjambiplugin library does not exist: "+libinfo[1]);
							}
							libraries.add(new SimpleEntry<>(libinfo[0], libFile.toURI().toURL()));
						}else {
							String os = null;
							if(libinfo[0].endsWith(".dll")) {
								os = "windows";
							}else if(libinfo[0].endsWith(".dylib")) {
								os = "macos";
							}else if(libinfo[0].endsWith(".so")) {
								os = "linux";
							}
							if(os!=null) {
								File libFile = new File(libinfo[0]);
								if(!libFile.isFile()) {
									throw new Error("Specified qtjambiplugin library does not exist: "+libinfo[0]);
								}
								libraries.add(new SimpleEntry<>(os, libFile.toURI().toURL()));
							}else {
								throw new Error("Unable to determine platform for library "+libinfo[0]+". Please use --plugin-library=<platform>"+File.pathSeparatorChar+"<path to library>");
							}
						}
					}else if(args[i].startsWith("--plugin-library-location=")) {
						File location = new File(args[i].substring(26));
	                    if(isDebug==null){
	                        throw new Error("Please specify --configuration=debug or --configuration=release prior to --plugin-library-location");
	                    }
	                    if(location.isDirectory())
						for(File entry : location.listFiles()) {
							if(!entry.isFile())
								continue;
	                        String os = null;
	                        if(isDebug){
	                            if(entry.getName().equals("qtjambiplugind.dll")) {
	                                os = "windows";
	                            }else if(entry.getName().equals("libqtjambiplugin_debug.dylib")) {
	                                os = "macos";
	                            }else if(entry.getName().equals("libqtjambiplugin_debug.so")) {
	                                os = "linux";
	                            }
	                        }else{
	                            if(entry.getName().equals("qtjambiplugin.dll")) {
	                                os = "windows";
	                            }else if(entry.getName().equals("libqtjambiplugin.dylib")) {
	                                os = "macos";
	                            }else if(entry.getName().equals("libqtjambiplugin.so")) {
	                                os = "linux";
	                            }
	                        }
	                        if(os!=null && entry.getName().contains("qtjambiplugin")) {
	                            libraries.add(new SimpleEntry<>(os, entry.toURI().toURL()));
	                        }
						}
					}else if(args[i].startsWith("--meta-data=")) {
						String _metaData = args[i].substring(12);
						if(_metaData.startsWith("\"") && _metaData.endsWith("\"")) {
							_metaData = _metaData.substring(1, _metaData.length()-2);
						}
						QByteArray data;
						if(_metaData.startsWith("{") && _metaData.endsWith("}")) {
							data = new QByteArray(_metaData.replace("''", "\""));
						}else{
							File file = new File(_metaData);
							if(file.exists()) {
								data = new QByteArray(Files.readAllBytes(file.toPath()));
							}else {
								data = null;
							}
						}
						if(data!=null) {
							QJsonDocument.FromJsonResult result = QJsonDocument.fromJson(data);
							if(result.error.error()==ParseError.NoError && result.document.isObject()) {
								metaData = result.document.object();
							}
						}
					}else {
						showInfo();
						throw new Error("Unknown arguments "+args[i]);
					}
				}
				if(dir==null) {
					throw new Error("Missing target directory. Please use --dir=...");
				}
				if(classPath.isEmpty()) {
					throw new Error("Missing classpath. Please use --class-path=...");
				}
				if(className==null || metaData==null || pluginName==null) {
					try(JarFile jarFile = new JarFile(new File(classPath.get(0)))){
						Attributes mainAttributes = jarFile.getManifest().getMainAttributes();
						if(metaData==null) {
							String _metaData = mainAttributes.getValue("QtJambi-Plugin-Metadata");
							QByteArray data = new QByteArray(_metaData.replace("''", "\""));
							QJsonDocument.FromJsonResult result = QJsonDocument.fromJson(data);
							if(result.error.error()==ParseError.NoError && result.document.isObject()) {
								metaData = result.document.object();
							}
						}
						if(className==null) {
							className = mainAttributes.getValue("QtJambi-Plugin-Class");
						}
						if(pluginName==null) {
							pluginName = mainAttributes.getValue("QtJambi-Plugin-Name");
						}
					}
				}
				if(className==null || className.isEmpty()) {
					throw new Error("Missing class name. Please use --class-name=...");
				}
				if(metaData==null) {
					throw new Error("Missing metadata. Please use --meta-data=...");
				}
				if(libraries.isEmpty()) {
					Enumeration<URL> specsFound = Collections.emptyEnumeration();
		            try {
						specsFound = Main.class.getClassLoader().getResources("qtjambi-utilities.xml");
					} catch (IOException e) {
						Logger.getLogger("io.qt").log(Level.WARNING, "", e);
					}
		            Set<URL> specsUrls = new HashSet<>();
		            while (specsFound.hasMoreElements()) {
		                URL url = specsFound.nextElement();
		                if(specsUrls.contains(url))
		                	continue;
		                specsUrls.add(url);
		                String protocol = url.getProtocol();
		                if (protocol.equals("jar")) {
		                	String eform = url.toExternalForm();
		                	int start = 4;
		                	int end = eform.indexOf("!/", start);
		                	if (end != -1) {
		                		try {
			                        Document doc;
			                        try(InputStream inStream = url.openStream()){
			                        	DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			                        	factory.setValidating(false);
			                			DocumentBuilder builder = factory.newDocumentBuilder();
			                			doc = builder.parse(inStream);
			                        }
			                        String system = doc.getDocumentElement().getAttribute("system");
			                        String configuration = doc.getDocumentElement().getAttribute("configuration");
			                        if(isDebug==null || isDebug.booleanValue()=="debug".equals(configuration)) {
				                        for(int i=0; i<doc.getDocumentElement().getChildNodes().getLength(); ++i) {
				                        	Node child = doc.getDocumentElement().getChildNodes().item(i);
				                        	if(child instanceof Element && child.getNodeName().equals("library")) {
				                        		Element element = (Element)child;
				                        		String library = element.getAttribute("name");
				                        		if(library.contains("qtjambiplugin")) {
				                        			URL libraryURL = new URL(eform.substring(0, end-2)+library);
				                        			libraries.add(new SimpleEntry<>(system, libraryURL));
				                        		}
				                        	}
				                        }
			                        }
		                		}catch (Exception e) {
		            				Logger.getLogger("io.qt").log(Level.WARNING, "", e);
		            			}
		                	}
		                }
		            }
				}
				if(libraries.isEmpty()) {
					throw new Error("Missing paths to qtjambiplugin library. Please use --plugin-library=<path to library>, --plugin-library=<platform>"+File.pathSeparatorChar+"<path to library> or --plugin-library-location=<path> in combination with --configuration=debug/release");
				}
				if(pluginName==null) {
					if(classPath.size()==1) {
						File file = new File(classPath.get(0));
						String path = file.getName();
						if(path.endsWith(".jar")) {
							pluginName = path.substring(0, path.length()-4);
						}
					}
				}
				if(pluginName==null) {
					throw new Error("Missing plugin name. Please use --plugin-name=...");
				}
				if(iid==null) {
					//System.out.println("Analyzing IID...");
					//System.out.flush();
					List<URL> urls = new ArrayList<>();
					List<File> libraryPath = new ArrayList<>();
					for(String path : classPath) {
						File file = new File(path);
						if(file.exists()) {
							if(file.isFile() && !file.getName().endsWith(".jar")) {
								libraryPath.add(file.getParentFile());
							}else {
								urls.add(file.toURI().toURL());
							}
						}else {
							throw new Error("Classpath not a file or directory: "+path);
						}
					}
					StringBuilder lp = new StringBuilder(System.getProperty("java.library.path", ""));
					if(!libraryPath.isEmpty()) {
						for(File file : libraryPath) {
							if(lp.length()!=0) {
								lp.append(File.pathSeparator);
							}
							lp.append(file.getAbsolutePath());
						}
						System.setProperty("java.library.path", lp.toString());
					}
					
					@SuppressWarnings("resource")
					PluginClassLoader pluginClassLoader = new PluginClassLoader();
					pluginClassLoader.addURLs(urls);
					Class<?> cls;
					try {
						cls = pluginClassLoader.loadClass(className);
					} catch (ClassNotFoundException | NoClassDefFoundError e) {
						System.err.println("Loading class "+className+" failed. Trying to extend classpath.");
						URL url = QFile.class.getResource("QFile.class");
						if(url!=null) {
							String path = url.toString();
							int idx = -1;
							if(path.startsWith("jar:file:") && (idx = path.indexOf("!/")) >= 0) {
								path = path.substring(4, idx);
								try {
									File jarFile = new File(new URL(path).toURI());
									if(jarFile.getParentFile().isDirectory())
									for(File other : jarFile.getParentFile().listFiles()) {
										if(other.isFile() && other.getName().startsWith("qtjambi")
												 && !other.getName().endsWith("javadoc.jar")
												 && !other.getName().endsWith("sources.jar")
												 && other.getName().endsWith(".jar")) {
											urls.add(other.toURI().toURL());
										}
									}
									pluginClassLoader.close();
									pluginClassLoader = new PluginClassLoader();
									pluginClassLoader.addURLs(urls);
									cls = pluginClassLoader.loadClass(className);
								} catch (Exception e1) {
									System.err.println(url);
									e1.printStackTrace();
									throw new Error("Unable to find class: "+className);
								}
							}else {
								System.err.println(url);
								throw new Error("Unable to find class: "+className);
							}
						}else {
							System.err.println(url);
							throw new Error("Unable to find class: "+className);
						}
					}
					iid = QtJambiPlugins.getInterfaceIID(cls);
					if(iid==null) {
						throw new Error("Unable to detect IID from class: "+cls.getName());
					}
				}
				if(iid!=null && className!=null) {
					//System.out.println("Collecting metadata...");
					//System.out.flush();
					QCborMap cborValue = new QCborMap();
					cborValue.setValue(/*QtPluginMetaDataKeys::IID*/ 2, new QCborValue(iid));
					cborValue.setValue(/*QtPluginMetaDataKeys::ClassName*/ 3, new QCborValue(className.replace(".", "::")));
					cborValue.setValue(/*QtPluginMetaDataKeys::MetaData*/ 4, new QCborValue(QCborMap.fromJsonObject(metaData)));
					cborValue.setValue(0x0_CAFEBABE_0L, new QCborValue(pluginName));
					QByteArray cborData = cborValue.toCborValue().toCbor();
					if(cborData.size()>1024) {
						throw new Error("Plugin metadata exceeds maximum size of 1024 byte.");
					}
					System.gc();
					if(classPath.size()==1) {
						File file = new File(classPath.get(0));
						Files.copy(file.toPath(), new File(dir, pluginName+".jar").toPath(), StandardCopyOption.REPLACE_EXISTING);
					}else {
						File subdir = new File(dir, pluginName);
						subdir.mkdirs();
						for (String path : classPath) {
							File target = new File(subdir, path);
							target.getParentFile().mkdirs();
							Files.copy(new File(path).toPath(), target.toPath(), StandardCopyOption.REPLACE_EXISTING);
						}
					}
					
					final QByteArray QTMETADATA = new QByteArray("QTMETADATA !");
					
					for(Map.Entry<String,URL> entry : libraries) {
						String os = entry.getKey();
						if(os!=null) {
							File newFile;
	                        switch(os.toLowerCase()) {
							case "win32":
							case "win64":
							case "windows":
	                            if(isDebug==null){
									newFile = new File(dir, pluginName + (entry.getValue().toExternalForm().endsWith("d.dll") ? "d.dll" : ".dll"));
								}else{
									newFile = new File(dir, pluginName + (isDebug ? "d.dll" : ".dll"));
	                            }
								System.gc();
								break;
							case "macos":
							case "osx":
	                            if(isDebug==null){
									newFile = new File(dir, "lib" + pluginName + (entry.getValue().toExternalForm().endsWith("_debug.dylib") ? "_debug.dylib" : ".dylib"));
								}else{
									newFile = new File(dir, "lib" + pluginName + (isDebug ? "_debug.dylib" : ".dylib"));
	                            }
	                            break;
							case "linux":
							case "linux32":
							case "linux64":
	                            if(isDebug==null){
									newFile = new File(dir, "lib" + pluginName + (entry.getValue().toExternalForm().endsWith("_debug.so") ? "_debug.so" : ".so"));
								}else{
									newFile = new File(dir, "lib" + pluginName + (isDebug ? "_debug.so" : ".so"));
	                            }
								break;
								default: continue;
							}
	                        try{
								QIODevice device = QIODevice.fromInputStream(entry.getValue().openStream());
								if(device.open(QIODevice.OpenModeFlag.ReadOnly)) {
		                            QByteArray libData = device.readAll();
		                            device.close();
		                            int idx = (int)libData.indexOf(QTMETADATA);
		                            if(idx>0) {
		                            	QFile _newFile = new QFile(QDir.fromNativeSeparators(newFile.getAbsolutePath()));
		                                if(_newFile.open(QIODevice.OpenModeFlag.WriteOnly)) {
		                                    _newFile.write(libData);
	                                    	while(idx>0) {
			                                    _newFile.seek(idx+16);
			                                    _newFile.write(cborData);		                                    		
		                                    	idx = (int)libData.indexOf(QTMETADATA, idx+16);
	                                    	}
		                                    _newFile.close();
		                                    if(_newFile.fileName().endsWith("_debug.dylib")) {
		                                    	QFile.copy(_newFile.fileName(), _newFile.fileName().replace("_debug.dylib", ".dylib"));
		                                    }
		                                }else{
		                                    throw new Error("Unable to write file "+_newFile.fileName());
		                                }
	                                }else{
	                                    throw new Error("Unable to find \""+QTMETADATA+"\" in file "+entry.getValue().toExternalForm());
		                            }
		                        }else{
		                            throw new Error("Unable to read file "+entry.getValue().toExternalForm());
		                        }
							}catch(IOException e) {
	                            throw new Error("Unable to read file "+entry.getValue().toExternalForm(), e);
							}
						}
					}
				}
			}
				break;
			case "app":
			case "application":
			{
				String appName = null;
				String mainClass = null;
				String jvmPath = null;
				QDir dir = null;
				QFile ico = null;
				Integer jniMinimumVersion = null;
				List<String> classPaths = new ArrayList<>();
				List<String> modulePaths = new ArrayList<>();
				List<String> libraryPaths = new ArrayList<>();
				List<String> arguments = new ArrayList<>();
				List<Map.Entry<String,URL>> executables = new ArrayList<>();
	            boolean autoDetect = false;
	            int minimumJVMVersion = 11;
				for (int i = 1; i < args.length; i++) {
					if(args[i].startsWith("--help") 
							|| args[i].startsWith("-h")
							|| args[i].startsWith("-?")) {
						showInfo();
						return;
					}else if(args[i].startsWith("--jni-minimum-version=")) {
						jniMinimumVersion = Integer.parseInt(args[i].substring(22), 16);
					}else if(args[i].startsWith("--application=")) {
						appName = args[i].substring(14);
					}else if(args[i].startsWith("--dir=")) {
						dir = new QDir(QDir.fromNativeSeparators(args[i].substring(6)));
					}else if(args[i].startsWith("--ico=")) {
						ico = new QFile(QDir.fromNativeSeparators(args[i].substring(6)));
					}else if(args[i].startsWith("--class-path=")) {
						classPaths.addAll(Arrays.asList(args[i].substring(13).split(File.pathSeparator)));
					}else if(args[i].startsWith("--module-path=")) {
						modulePaths.addAll(Arrays.asList(args[i].substring(14).split(File.pathSeparator)));
					}else if(args[i].startsWith("--library-path=")) {
						libraryPaths.addAll(Arrays.asList(args[i].substring(15).split(File.pathSeparator)));
					}else if(args[i].startsWith("--main-class=")) {
						mainClass = args[i].substring(13);
					}else if(args[i].startsWith("--autodetect-jvm")) {
	                    autoDetect = true;
					}else if(args[i].startsWith("--minversion-jvm=")) {
						minimumJVMVersion = Integer.parseInt(args[i].substring(17));
					}else if(args[i].startsWith("--jvm-path=")) {
						jvmPath = args[i].substring(11);
					}else if(args[i].startsWith("--executable=")) {
						String[] exeinfo = args[i].substring(13).split(File.pathSeparator);
						if(exeinfo.length==2){
							File exeFile = new File(exeinfo[1]);
							if(!exeFile.isFile()) {
								throw new Error("Specified launcher executable does not exist: "+exeinfo[1]);
							}
							executables.add(new SimpleEntry<>(exeinfo[0], exeFile.toURI().toURL()));
						}else {
							String os = null;
							File exeFile = new File(exeinfo[0]);
							if(exeinfo[0].endsWith(".exe")) {
								os = "windows";
								if(!exeFile.isFile()) {
									throw new Error("Specified launcher executable does not exist: "+exeinfo[0]);
								}
							}else if(exeinfo[0].endsWith(".app")) {
								if(!exeFile.isDirectory()) {
									throw new Error("Specified launcher executable does not exist: "+exeinfo[0]);
								}
								os = "macos";
							}else {
								if(!exeFile.isFile()) {
									throw new Error("Specified launcher executable does not exist: "+exeinfo[0]);
								}
								os = "linux";
							}
							if(os!=null) {
								executables.add(new SimpleEntry<>(os, exeFile.toURI().toURL()));
							}
						}
					}else if(args[i].startsWith("--executable-location=")) {
						QDir location = new QDir(args[i].substring(26));
						for(String entry : location.entryList(QDir.Filter.Files)) {
	                        String os = null;
	                        if(entry.equals("QtJambiLauncher.exe")) {
	                            os = "windows";
	                        }else if(entry.equals("QtJambiLauncher.app")) {
	                            os = "macos";
	                        }else if(entry.equals("QtJambiLauncher")) {
	                            os = "linux";
	                        }
	                        if(os!=null && entry.contains("QtJambiLauncher")) {
	                        	File libFile = new File(location.absoluteFilePath(entry));
	                        	executables.add(new SimpleEntry<>(os, libFile.toURI().toURL()));
	                        }
						}
					}else {
						arguments.add(args[i]);
					}
				}
				if(appName==null || appName.isEmpty()) {
					throw new Error("Missing application name. Please use --application=...");
				}
				if(!autoDetect && (jvmPath==null || jvmPath.isEmpty())) {
					throw new Error("Missing JVM path. Please use --jvm-path=...");
				}
				if(dir==null) {
					throw new Error("Missing target directory. Please use --dir=...");
				}
				if(mainClass==null || mainClass.isEmpty()) {
					throw new Error("Missing main class. Please use --main-class=...");
				}
				if(executables.isEmpty()) {
					Enumeration<URL> specsFound = Collections.emptyEnumeration();
		            try {
						specsFound = Main.class.getClassLoader().getResources("qtjambi-utilities.xml");
					} catch (IOException e) {
						Logger.getLogger("io.qt").log(Level.WARNING, "", e);
					}
		            Set<URL> specsUrls = new HashSet<>();
		            while (specsFound.hasMoreElements()) {
		                URL url = specsFound.nextElement();
		                if(specsUrls.contains(url))
		                	continue;
		                specsUrls.add(url);
		                String protocol = url.getProtocol();
		                if (protocol.equals("jar")) {
		                	String eform = url.toExternalForm();
		                	int start = 4;
		                	int end = eform.indexOf("!/", start);
		                	if (end != -1) {
		                		try {
			                        Document doc;
			                        try(InputStream inStream = url.openStream()){
			                        	DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			                        	factory.setValidating(false);
			                			DocumentBuilder builder = factory.newDocumentBuilder();
			                			doc = builder.parse(inStream);
			                        }
			                        String system = doc.getDocumentElement().getAttribute("system");
//			                        String configuration = doc.getDocumentElement().getAttribute("configuration");
			                        for(int i=0; i<doc.getDocumentElement().getChildNodes().getLength(); ++i) {
			                        	Node child = doc.getDocumentElement().getChildNodes().item(i);
			                        	if(child instanceof Element && child.getNodeName().equals("library")) {
			                        		Element element = (Element)child;
			                        		String library = element.getAttribute("name");
			                        		if(library.endsWith("QtJambiLauncher") 
			                        				|| library.endsWith("QtJambiLauncher_debug")
			                        				|| library.endsWith(".exe")) {
			                        			URL libraryURL = new URL(eform.substring(0, end-2)+library);
			                        			executables.add(new SimpleEntry<>(system, libraryURL));
			                        		}
			                        	}
			                        }
		                		}catch (Exception e) {
		            				Logger.getLogger("io.qt").log(Level.WARNING, "", e);
		            			}
		                	}
		                }
		            }
				}
				if(executables.isEmpty()) {
					throw new Error("Missing paths to QtJambiLauncher executables. Please use --executable=<path to executable>, --executable=<platform>"+File.pathSeparatorChar+"<path to executable> or --executable-location=<path>");
				}
				//System.out.println("Collecting metadata...");
				//System.out.flush();
				QCborMap cborValue = new QCborMap();
				cborValue.setValue(Parameters.JVMDetectionMode.value(), new QCborValue(autoDetect ? JVMDetectionModes.AutoDetect.value() : JVMDetectionModes.UseRelativePath.value()));
				if(autoDetect) {
					cborValue.setValue(Parameters.MinimumJVM.value(), new QCborValue(minimumJVMVersion));
				}else {
					cborValue.setValue(Parameters.JVMPath.value(), new QCborValue(jvmPath));				
				}
				IntFunction<QCborValue[]> arrayFactory = length->new QCborValue[length];
				cborValue.setValue(Parameters.MainClass.value(), new QCborValue(mainClass));
				QCborArray argumentArray = new QCborArray(arguments.stream().map(QByteArray::new).map(QCborValue::new).toArray(arrayFactory));
				cborValue.setValue(Parameters.JVMArguments.value(), new QCborValue(argumentArray));
				QCborArray classPathArray = new QCborArray(classPaths.stream().map(QCborValue::new).toArray(arrayFactory));
				cborValue.setValue(Parameters.ClassPath.value(), new QCborValue(classPathArray));
				QCborArray modulePathArray = new QCborArray(modulePaths.stream().map(QCborValue::new).toArray(arrayFactory));
				cborValue.setValue(Parameters.ModulePath.value(), new QCborValue(modulePathArray));
				QCborArray libraryPathArray = new QCborArray(libraryPaths.stream().map(QCborValue::new).toArray(arrayFactory));
				cborValue.setValue(Parameters.LibraryPath.value(), new QCborValue(libraryPathArray));
				if(jniMinimumVersion!=null) {
					cborValue.setValue(Parameters.JNIMinimumVersion.value(), new QCborValue(jniMinimumVersion));
				}
				QByteArray cborData = cborValue.toCborValue().toCbor();
				if(cborData.size()>16384) {
					throw new Error("Launcher metadata exceeds maximum size of 16384 byte.");
				}
				System.gc();
				final QByteArray QTJAMBI_LAUNCHER = new QByteArray("QTJAMBI_LAUNCHER!");
				
				for(Map.Entry<String,URL> entry : executables) {
					String os = entry.getKey();
					if(os!=null) {
	                    URL file = entry.getValue();
						QFile newFile;
	                    switch(os.toLowerCase()) {
						case "linux":
						case "linux32":
						case "linux64":
						case "linux-arm64":
						case "linux-aarch64":
						case "linux-x86":
						case "linux-x64":
							newFile = new QFile(dir.absoluteFilePath(appName));
							break;
//							default: continue;
						case "win32":
						case "win64":
						case "windows":
						case "windows-aarch64":
						case "windows-arm64":
						case "windows-x86":
						case "windows-x64":
							newFile = new QFile(dir.absoluteFilePath(appName + ".exe"));
							System.gc();
							break;
						case "macos":
						case "osx":
							String fileBase = file.toExternalForm();
							if(fileBase.endsWith("/"))
								fileBase = fileBase.substring(0, fileBase.length()-1);
							String baseName = fileBase;
							{
								int idx = baseName.lastIndexOf('/');
								if(idx>0)
									baseName = baseName.substring(idx+1);
							}
							if(baseName.endsWith(".app"))
								baseName = baseName.substring(0, baseName.length()-4);
							dir.mkpath(appName + ".app/Contents/MacOS");
							dir.mkpath(appName + ".app/Contents/Resources");
							try {
								URL infoPListUrl = new URL(fileBase + "/Contents/Info.plist");
								QIODevice device = QIODevice.fromInputStream(infoPListUrl.openStream());
			                    if(device.open(QIODevice.OpenModeFlag.ReadOnly)) {
			                        QByteArray data = device.readAll();
			                        device.close();
			                        int idx = (int)data.indexOf("<key>CFBundleIdentifier</key>");
									if(idx>0) {
										idx = (int)data.indexOf("<string>", idx);
										if(idx>0) {
											idx += 8;
											int idxEnd = (int)data.indexOf("</string>", idx);
											if(idxEnd>idx) {
												data = data.remove(idx, idxEnd-idx);
												data.insert(idx, mainClass);
											}
										}
									}
									data = data.replace(baseName, appName);
									data = data.replace("QtJambiLauncher", appName);
									QFile infoPList = new QFile(dir.absoluteFilePath(appName + ".app/Contents/Info.plist"));
									infoPList.remove();
									if(infoPList.open(QIODevice.OpenModeFlag.WriteOnly)) {
										infoPList.write(data);
										infoPList.close();
									}
			                    }
							}catch(IOException e) {
								e.printStackTrace();
							}
		                    try {
		                    	URL pkgInfoUrl = new URL(fileBase + "/Contents/PkgInfo");
								QIODevice device = QIODevice.fromInputStream(pkgInfoUrl.openStream());
			                    if(device.open(QIODevice.OpenModeFlag.ReadOnly)) {
			                        QByteArray data = device.readAll();
			                        device.close();
			                        dir.mkpath(appName + ".app/Contents");
			                        QFile pkgInfo = new QFile(dir.absoluteFilePath(appName + ".app/Contents/PkgInfo"));
									pkgInfo.remove();
									if(pkgInfo.open(QIODevice.OpenModeFlag.WriteOnly)) {
										pkgInfo.write(data);
										pkgInfo.close();
									}
			                    }
							}catch(IOException e) {
								e.printStackTrace();
							}
		                    try {
			                    URL emptylprojUrl = new URL(fileBase + "/Contents/Resources/empty.lproj");
			                    QIODevice device = QIODevice.fromInputStream(emptylprojUrl.openStream());
			                    if(device.open(QIODevice.OpenModeFlag.ReadOnly)) {
			                        QByteArray data = device.readAll();
			                        device.close();
			                        dir.mkpath(appName + ".app/Contents/Resources");
			                        QFile emptylproj = new QFile(dir.absoluteFilePath(appName + ".app/Contents/Resources/empty.lproj"));
									emptylproj.remove();
									if(emptylproj.open(QIODevice.OpenModeFlag.WriteOnly)) {
										emptylproj.write(data);
										emptylproj.close();
									}
			                    }
							}catch(IOException e) {
								e.printStackTrace();
							}
		                    try {
			                    URL executableUrl = new URL(fileBase + "/Contents/MacOS/"+baseName);
			                    QIODevice device = QIODevice.fromInputStream(executableUrl.openStream());
			                    if(device.open(QIODevice.OpenModeFlag.ReadOnly)) {
			                        QByteArray data = device.readAll();
			                        device.close();
			                        dir.mkpath(appName + ".app/Contents/MacOS");
			                        QFile executable = new QFile(dir.absoluteFilePath(appName + ".app/Contents/MacOS/"+appName));
									executable.remove();
									if(executable.open(QIODevice.OpenModeFlag.WriteOnly)) {
										executable.write(data);
										executable.close();
										executable.setPermissions(executable.permissions()
									                                		.combined(QFileDevice.Permission.ExeGroup)
									                                		.combined(QFileDevice.Permission.ExeOther)
									                                		.combined(QFileDevice.Permission.ExeOwner)
									                                		.combined(QFileDevice.Permission.ExeUser));
									}
			                    }
							}catch(IOException e) {
								e.printStackTrace();
							}
		                    
		                    dir.mkpath(appName + ".app/Contents/Resources");
	                        QFile params = new QFile(dir.absoluteFilePath(appName + ".app/Contents/Resources/params.cbor"));
							params.remove();
							if(params.open(QIODevice.OpenModeFlag.WriteOnly)) {
								params.write(cborData);
								params.close();
							}
	                        continue;
	                        default: continue;
						}
	                    try {
		                    QIODevice device = QIODevice.fromInputStream(file.openStream());
		                    if(device.open(QIODevice.OpenModeFlag.ReadOnly)) {
		                        QByteArray exeData = device.readAll();
		                        device.close();
		                        int idx = (int)exeData.indexOf(QTJAMBI_LAUNCHER);
		                        if(idx>0) {
		                        	newFile.remove();
		                            if(newFile.open(QIODevice.OpenModeFlag.WriteOnly)) {
		                                try{
			                                newFile.write(exeData);
			                                while(idx>0) {
				                                newFile.seek((int)idx);
				                                newFile.write(cborData);
				                                idx = (int)exeData.indexOf(QTJAMBI_LAUNCHER, idx + cborData.length());
			                                }
			                                switch(os.toLowerCase()) {
			        						case "win32":
			        						case "win64":
			        						case "windows":
			                                	idx = (int)exeData.indexOf(new QByteArray("QTJAMBI_ICO!"));
			                                	if(idx>254) {
			                                        if(ico!=null) {
			        									if(ico.open(QIODevice.OpenModeFlag.ReadOnly)) {
			        										QByteArray icoData = ico.readAll();
			        										if(icoData.size()>=92482) {
			        			                                throw new Error("Icon size too big.");
			        										}
			        	                                    newFile.seek(idx-254);
			        	                                    newFile.write(icoData);
			        									}
			                                        }else {
			    	        							try(InputStream stream = Main.class.getResourceAsStream("icon.ico");
			    	        									ByteArrayOutputStream buffer = new ByteArrayOutputStream()){
			    	        								byte[] data = new byte[10240];
			    	        								int length = stream.read(data);
			    	        								while(length>0) {
			    	        									buffer.write(data, 0, length);
			    	        									length = stream.read(data);
			    	        								}
			    	        								data = buffer.toByteArray();
			    	        								if(data.length<92482) {
				    	        								newFile.seek(idx-254);
				        	                                    newFile.write(data);
			    	        								}
			    	        							}
			                                        }
			                                	}
			        							break;
			        						default: 
			        							break;
			                                }
		                                }finally {
		                                	newFile.close();
		                                }
		                                newFile.setPermissions(newFile.permissions()
								                                		.combined(QFileDevice.Permission.ExeGroup)
								                                		.combined(QFileDevice.Permission.ExeOther)
								                                		.combined(QFileDevice.Permission.ExeOwner)
								                                		.combined(QFileDevice.Permission.ExeUser));
		                            }else{
		                                throw new Error("Unable to write file "+newFile.fileName());
		                            }
		                        }else {
			                        throw new Error("Unable to find \""+QTJAMBI_LAUNCHER+"\" in file "+file.toExternalForm());
		                        }
		                    }else{
		                        throw new Error("Unable to read file "+file.toExternalForm());
		                    }
						}catch(IOException e) {
	                        throw new Error("Unable to read file "+file.toExternalForm(), e);
						}
					}
				}
			}
				break;
			case "qml":
			{
				File library = null;
				List<String> classPaths = new ArrayList<>();
				List<String> natives = new ArrayList<>();
				File dir = null;
				List<Map.Entry<String,URL>> libraries = new ArrayList<>();
	            Boolean isDebug = null;
				for (int i = 1; i < args.length; i++) {
					if(args[i].startsWith("--help") 
							|| args[i].startsWith("-h")
							|| args[i].startsWith("-?")) {
						showInfo();
						return;
					}else if(args[i].startsWith("--library=")) {
						library = new File(args[i].substring(10));
					}else if(args[i].startsWith("--classpath=")) {
						classPaths.addAll(Arrays.asList(args[i].substring(12).split(File.pathSeparator)));
					}else if(args[i].startsWith("--native-library-path=")) {
						natives.addAll(Arrays.asList(args[i].substring(22).split(File.pathSeparator)));
					}else if(args[i].startsWith("--configuration=")) {
						String conf = args[i].substring(16);
	                    isDebug = "debug".equals(conf);
					}else if(args[i].startsWith("--dir=")) {
						dir = new File(args[i].substring(6));
					}else if(args[i].startsWith("--jarimport=")) {
						String[] libinfo = args[i].substring(12).split(File.pathSeparator);
						if(libinfo.length==2){
							File libFile = new File(libinfo[1]);
							if(!libFile.isFile()) {
								throw new Error("Specified jarimport library does not exist: "+libinfo[1]);
							}
							libraries.add(new SimpleEntry<>(libinfo[0], libFile.toURI().toURL()));
						}else {
							String os = null;
							if(libinfo[0].endsWith(".dll")) {
								os = "windows";
							}else if(libinfo[0].endsWith(".dylib")) {
								os = "macos";
							}else if(libinfo[0].endsWith(".so")) {
								os = "linux";
							}
							if(os!=null) {
								File libFile = new File(libinfo[0]);
								if(!libFile.isFile()) {
									throw new Error("Specified jarimport library does not exist: "+libinfo[0]);
								}
								libraries.add(new SimpleEntry<>(os, libFile.toURI().toURL()));
							}else {
								throw new Error("Unable to determine platform for library "+libinfo[0]+". Please use --jarimport=<platform>"+File.pathSeparatorChar+"<path to jarimport library>");
							}
						}
					}else if(args[i].startsWith("--jarimport-location=")) {
						QDir location = new QDir(args[i].substring(21));
	                    if(isDebug==null){
	                        throw new Error("Please specify --configuration=debug or --configuration=release prior to --jarimport-location");
	                    }
						for(String entry : location.entryList(QDir.Filter.Files)) {
	                        String os = null;
	                        if(isDebug){
	                            if(entry.equals("jarimportd.dll")) {
	                                os = "windows";
	                            }else if(entry.equals("libjarimport_debug.dylib")) {
	                                os = "macos";
	                            }else if(entry.equals("libjarimport_debug.so")) {
	                                os = "linux";
	                            }
	                        }else{
	                            if(entry.equals("jarimport.dll")) {
	                                os = "windows";
	                            }else if(entry.equals("libjarimport.dylib")) {
	                                os = "macos";
	                            }else if(entry.equals("libjarimport.so")) {
	                                os = "linux";
	                            }
	                        }
	                        if(os!=null && entry.contains("jarimport")) {
	                        	File libFile = new File(location.absoluteFilePath(entry));
	                            libraries.add(new SimpleEntry<>(os, libFile.toURI().toURL()));
	                        }
						}
					}
				}
				if(library==null) {
					throw new Error("Missing qml library. Please use --library=...");
				}
				if(!library.isFile() || !library.getName().endsWith(".jar")) {
					throw new Error("Qml library is not a jar file.");
				}
				if(dir==null) {
					throw new Error("Missing target directory. Please use --dir=...");
				}
				if(dir.isFile()) {
					throw new Error("File not allowed as target directory. Please specify a directory --dir=...");
				}
				if(libraries.isEmpty()) {
					Enumeration<URL> specsFound = Collections.emptyEnumeration();
		            try {
						specsFound = Main.class.getClassLoader().getResources("qtjambi-utilities.xml");
					} catch (IOException e) {
						Logger.getLogger("io.qt").log(Level.WARNING, "", e);
					}
		            Set<URL> specsUrls = new HashSet<>();
		            while (specsFound.hasMoreElements()) {
		                URL url = specsFound.nextElement();
		                if(specsUrls.contains(url))
		                	continue;
		                specsUrls.add(url);
		                String protocol = url.getProtocol();
		                if (protocol.equals("jar")) {
		                	String eform = url.toExternalForm();
		                	int start = 4;
		                	int end = eform.indexOf("!/", start);
		                	if (end != -1) {
		                		try {
			                        Document doc;
			                        try(InputStream inStream = url.openStream()){
			                        	DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			                        	factory.setValidating(false);
			                			DocumentBuilder builder = factory.newDocumentBuilder();
			                			doc = builder.parse(inStream);
			                        }
			                        String system = doc.getDocumentElement().getAttribute("system");
			                        String configuration = doc.getDocumentElement().getAttribute("configuration");
			                        if(isDebug==null || isDebug.booleanValue()=="debug".equals(configuration)) {
				                        for(int i=0; i<doc.getDocumentElement().getChildNodes().getLength(); ++i) {
				                        	Node child = doc.getDocumentElement().getChildNodes().item(i);
				                        	if(child instanceof Element && child.getNodeName().equals("library")) {
				                        		Element element = (Element)child;
				                        		String libraryName = element.getAttribute("name");
				                        		if(libraryName.contains("jarimport")) {
				                        			URL libraryURL = new URL(eform.substring(0, end-2)+libraryName);
				                        			libraries.add(new SimpleEntry<>(system, libraryURL));
				                        		}
				                        	}
				                        }
			                        }
		                		}catch (Exception e) {
		            				Logger.getLogger("io.qt").log(Level.WARNING, "", e);
		            			}
		                	}
		                }
		            }
				}
				if(libraries.isEmpty()) {
					throw new Error("Missing paths to jarimport library. Please use --jarimport=<path to jarimport library>, --jarimport=<platform>"+File.pathSeparatorChar+"<path to jarimport library> or --jarimport-location=<path> in combination with --configuration=debug/release");
				}
				String packageName = null;
				try (JarFile jarFile = new JarFile(library)) {
					for(JarEntry entry : (Iterable<JarEntry>)()->jarFile.entries().asIterator()) {
						if(!entry.isDirectory() && entry.getName().endsWith(".class")) {
							int idx = entry.getName().lastIndexOf('/');
							if(idx>=0) {
								packageName = entry.getName();
								packageName = packageName.substring(packageName.startsWith("/") ? 1 : 0, idx);
								break;
							}
						}
					}
				}catch(IOException e) {
					throw new Error("Unable to read jar file.", e);
				}
				if(packageName==null) {
					throw new Error(String.format("Unable to detect package in jar file %1$s.", library.getName()));
				}
				dir = new File(dir, packageName.replace('/', File.separatorChar));
				dir.mkdirs();
				if(dir.isDirectory()) {
					for(Map.Entry<String,URL> entry : libraries) {
						String os = entry.getKey();
						if(os!=null) {
							File newFile;
	                        switch(os.toLowerCase()) {
							case "win32":
							case "win64":
							case "windows":
	                            if(isDebug==null){
									newFile = new File(dir, "jarimport" + (entry.getValue().toExternalForm().endsWith("d.dll") ? "d.dll" : ".dll"));
								}else{
									newFile = new File(dir, "jarimport" + (isDebug ? "d.dll" : ".dll"));
	                            }
								System.gc();
								break;
							case "macos":
							case "osx":
								newFile = new File(dir, "libjarimport.dylib");
	                            break;
							case "linux":
							case "linux32":
							case "linux64":
								newFile = new File(dir, "libjarimport.so");
								break;
								default: continue;
							}
	                        try(InputStream in = entry.getValue().openStream()){
	                        	Files.copy(in, newFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
							}catch(IOException e) {
	                            throw new Error("Unable to write file "+newFile, e);
							}
	                        try{
	                        	Files.copy(library.toPath(), new File(dir, library.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
							}catch(IOException e) {
	                            throw new Error("Unable to write file "+newFile, e);
							}
	                        if(!classPaths.isEmpty()) {
	                        	for(String cp : classPaths) {
	                        		if(cp.endsWith(".jar")) {
		                        		File jar = new File(cp);
		                        		if(jar.exists()) {
		                        			try{
		        	                        	Files.copy(jar.toPath(), new File(dir, jar.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
		        							}catch(IOException e) {
		        	                            throw new Error("Unable to write file "+newFile, e);
		        							}
		                        		}
	                        		}
	                        	}
	                        }
	                        try(PrintWriter writer = new PrintWriter(new File(dir, "qmldir"))){
	                        	writer.print("module ");
	                        	writer.println(packageName.replace('/', '.'));
	                        	writer.println("plugin jarimport");
	                        	if(!natives.isEmpty()) {
	                        		writer.print("librarypath ");
		                        	writer.println(new QStringList(natives).join(","));
	                        	}
	                        }catch(IOException e) {
	                            throw new Error("Unable to write file "+new File(dir, "qmldir"), e);
							}
						}
					}
				}else {
					throw new Error(String.format("Unable to create directory %1$s.", dir.getAbsolutePath()));
				}
			}
				break;
			case "--help":
			case "-h":
			case "-?":
				showInfo();
				return;
			default:
				System.err.println("Unexpected argument "+args[0]);
				showInfo();
				throw new Error("Unexpected argument "+args[0]);
			}
		}
		QCoreApplication.shutdown();
	}

	private static void showInfo() {
	}

}
