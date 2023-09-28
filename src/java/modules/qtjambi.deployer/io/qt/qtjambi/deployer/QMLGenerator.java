/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.qtjambi.deployer;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.AbstractMap.SimpleEntry;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import io.qt.core.QCommandLineOption;
import io.qt.core.QCommandLineParser;
import io.qt.core.QDir;
import io.qt.core.QStringList;

final class QMLGenerator {
	static void generate(QCommandLineParser parser, String[] args, QCommandLineOption platformOption, QCommandLineOption dirOption, QCommandLineOption classPathOption, QCommandLineOption configurationOption) throws InterruptedException, IOException {
	    QCommandLineOption qmlTargetOption = new QCommandLineOption(QStringList.of("library"), "Java library for qml.", "jar");
	    QCommandLineOption qmlLibraryOption = new QCommandLineOption(QStringList.of("jarimport"), "Path to jarimport library.\nExamples:\n--jarimport=path"+File.separator+"jarimport.dll\n--jarimport=macos"+File.pathSeparator+"path"+File.separator+"jarimport.dylib", "file");
	    QCommandLineOption qmlLibraryLocationOption = new QCommandLineOption(QStringList.of("jarimport-location"), "Directory containing jarimport library", "path");
	    QCommandLineOption qmlNativeLibraryPathOption = new QCommandLineOption(QStringList.of("native-library-path"), "Native library path", "path");
	    
	    parser.addOptions(Arrays.asList(
	    		platformOption,
	    		configurationOption,
	    		qmlTargetOption,
	    		qmlLibraryOption,
	    		qmlLibraryLocationOption,
	    		qmlNativeLibraryPathOption,
	    		dirOption,
	    		classPathOption
			));
		if(args.length==1)
			parser.showHelp();
    	parser.process(new QStringList(args));
		QStringList unusedArguments = new QStringList(parser.positionalArguments());
		
		if(unusedArguments.size()==1)
			throw new Error("QML import library generation, illegal argument: "+unusedArguments.join(", "));
		if(unusedArguments.size()>1)
			throw new Error("QML import library generation, illegal argument: "+unusedArguments.join(", "));
		
		File library = null;
		if(parser.isSet(qmlTargetOption))
			library = new File(parser.value(qmlTargetOption));
		
		String platform = null;
		if(parser.isSet(platformOption))
			platform = parser.value(platformOption);

		List<String> classPaths = new ArrayList<>();
		if(parser.isSet(classPathOption))
			classPaths.addAll(Arrays.asList(parser.value(classPathOption).split(File.pathSeparator)));
		
		List<String> natives = new ArrayList<>();
		if(parser.isSet(qmlNativeLibraryPathOption))
			classPaths.addAll(Arrays.asList(parser.value(qmlNativeLibraryPathOption).split(File.pathSeparator)));
		
		File dir = null;
		if(parser.isSet(dirOption))
			dir = new File(parser.value(dirOption));
		
        Boolean isDebug = null;
        if(parser.isSet(configurationOption))
        	isDebug = "debug".equals(parser.value(configurationOption));
        
        List<Map.Entry<String,URL>> libraries = new ArrayList<>();
        if(parser.isSet(qmlLibraryOption)) {
        	String[] libinfo = parser.value(qmlLibraryOption).split(File.pathSeparator);
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
                }else if(libinfo[0].endsWith("_x86_64.so")) {
                    os = "android-x64";
                }else if(libinfo[0].endsWith("_x86.so")) {
                    os = "android-x86";
                }else if(libinfo[0].endsWith("_arm64-v8a.so")) {
                    os = "android-arm64";
                }else if(libinfo[0].endsWith("_armeabi-v7a.so")) {
                    os = "android-arm";
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
        }else if(parser.isSet(qmlLibraryLocationOption)) {
        	QDir location = new QDir(parser.value(qmlLibraryLocationOption));
            if(isDebug==null){
                throw new Error("Please specify --configuration=debug or --configuration=release prior to --jarimport-location");
            }
			for(String entry : location.entryList(QDir.Filter.Files)) {
                String os = null;
                if(isDebug){
                    if(entry.equals("jarimportd.dll")) {
                        os = "windows";
                    }else if(entry.equals("libjarimport_debug.dylib") || entry.equals("libjarimport.dylib")) {
                        os = "macos";
                    }else if(entry.equals("libjarimport_debug_x86_64.so") || entry.equals("libjarimport_x86_64.so")) {
                        os = "android-x64";
                    }else if(entry.equals("libjarimport_debug_x86.so") || entry.equals("libjarimport_x86.so")) {
                        os = "android-x86";
                    }else if(entry.equals("libjarimport_debug_arm64-v8a.so") || entry.equals("libjarimport_arm64-v8a.so")) {
                        os = "android-arm64";
                    }else if(entry.equals("libjarimport_debug_armeabi-v7a.so") || entry.equals("libjarimport_armeabi-v7a.so")) {
                        os = "android-arm";
                    }else if(entry.equals("libjarimport_debug.so") || entry.equals("libjarimport.so")) {
                        os = "linux";
                    }
                }else{
                    if(entry.equals("jarimport.dll")) {
                        os = "windows";
                    }else if(entry.equals("libjarimport.dylib")) {
                        os = "macos";
                    }else if(entry.equals("libjarimport_x86_64.so")) {
                        os = "android-x64";
                    }else if(entry.equals("libjarimport_x86.so")) {
                        os = "android-x86";
                    }else if(entry.equals("libjarimport_arm64-v8a.so")) {
                        os = "android-arm64";
                    }else if(entry.equals("libjarimport_armeabi-v7a.so")) {
                        os = "android-arm";
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
			Enumeration<URL> specsFound = Main.findSpecs();
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
		                        			URL libraryURL = new URL(eform.substring(0, end+2)+libraryName);
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
			Enumeration<JarEntry> enries = jarFile.entries();
			while (enries.hasMoreElements()) {
				JarEntry entry = (JarEntry) enries.nextElement();
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
		File parentDir = dir;
		dir = new File(dir, "qml"+File.separatorChar+packageName.replace('/', File.separatorChar));
		dir.mkdirs();
		if(dir.isDirectory()) {
			boolean found = false;
			for(Map.Entry<String,URL> entry : libraries) {
				String os = entry.getKey();
				boolean isAndroid = false;
				if(os!=null && (platform==null || platform.startsWith(os))) {
					File targetLibFile;
					URL debuginfoURL = null;
					File targetDebuginfoFile = null;
                    switch(os.toLowerCase()) {
					case "win32":
					case "win64":
					case "windows":
					case "windows-aarch64":
					case "windows-arm64":
					case "windows-x86":
					case "windows-x64":
                        if(isDebug==null){
							targetLibFile = new File(dir, "jarimport" + (entry.getValue().toExternalForm().endsWith("d.dll") ? "d.dll" : ".dll"));
						}else{
							targetLibFile = new File(dir, "jarimport" + (isDebug ? "d.dll" : ".dll"));
                        }
                        if(Boolean.FALSE.equals(isDebug) || !entry.getValue().toExternalForm().endsWith("d.dll")) {
                        	URL url = entry.getValue();
                        	switch(url.getProtocol()) {
        					case "file":
        						try {
        							File rfile = new File(url.toURI());
        							File dfile = new File(rfile.getParentFile(), rfile.getName().substring(0, rfile.getName().length()-3) + "pdb");
        							if(!dfile.exists()) {
        								dfile = new File(rfile.getAbsolutePath()+".debug");
        							}
        							if(dfile.exists()) {
        								debuginfoURL = dfile.toURI().toURL();
        								targetDebuginfoFile = new File(dir, "jarimport"+(dfile.getName().endsWith(".pdb") ? ".pdb" : ".dll.debug"));
        							}
        						} catch (URISyntaxException e1) {
        						}
        						break;
        					case "jar":
        						String path = url.toString();
        						int idx = path.indexOf("!/");
        						if(idx>0 && (path.endsWith(".dll") || path.endsWith(".exe"))) {
        							String jarFileURL = path.substring(4, idx);
        							String filePath = path.substring(idx);
        							try {
        								debuginfoURL = new URL(jarFileURL.replace("-native-", "-debuginfo-"));
        								if(!new File(debuginfoURL.toURI()).exists()) {
        									debuginfoURL = new URL(jarFileURL.replace("-native-", "-debuginfo-").replace("/native/", "/debuginfo/"));
        									if(!new File(debuginfoURL.toURI()).exists()) {
        										debuginfoURL = null;
        									}
        								}
        								if(debuginfoURL!=null) {
        									debuginfoURL = new URL("jar:"+debuginfoURL.toString()+filePath.substring(0, filePath.length()-3)+"pdb");
        									try(InputStream s = debuginfoURL.openStream()){
        										targetDebuginfoFile = new File(dir, "jarimport.pdb");
        									} catch (IOException e) {
        										debuginfoURL = new URL("jar:"+debuginfoURL.toString()+filePath+".debug");
        										try(InputStream s = debuginfoURL.openStream()){
        											targetDebuginfoFile = new File(dir, "jarimport.dll.debug");
        										} catch (IOException e2) {
        											debuginfoURL = null;
        										}
        									}
        								}
        							} catch (URISyntaxException e) {
        							}
        						}
        						break;
        					}
                        }
						System.gc();
						break;
					case "android-x64":
						File _dir = new File(parentDir, "lib/x86_64");
						_dir.mkdirs();
						targetLibFile = new File(_dir, "libqml_"+packageName.replace('/', '_')+"_jarimport_x86_64.so");
						isAndroid = true;
						break;
					case "android-x86":
						_dir = new File(parentDir, "lib/x86");
						_dir.mkdirs();
						targetLibFile = new File(_dir, "libqml_"+packageName.replace('/', '_')+"_jarimport_x86.so");
						isAndroid = true;
						break;
					case "android-arm64":
						_dir = new File(parentDir, "lib/arm64-v8a");
						_dir.mkdirs();
						targetLibFile = new File(_dir, "libqml_"+packageName.replace('/', '_')+"_jarimport_arm64-v8a.so");
						isAndroid = true;
						break;
					case "android-arm":
						_dir = new File(parentDir, "lib/armeabi-v7a");
						_dir.mkdirs();
						targetLibFile = new File(_dir, "libqml_"+packageName.replace('/', '_')+"_jarimport_armeabi-v7a.so");
						isAndroid = true;
						break;
					case "macos":
					case "osx":
						targetLibFile = new File(dir, "libjarimport.dylib");
						if(Boolean.FALSE.equals(isDebug)) {
                        	URL url = entry.getValue();
							switch(url.getProtocol()) {
							case "file":
								try {
									File rfile = new File(url.toURI());
									File dfile = new File(rfile.getAbsolutePath()+".dSYM/Contents/Resources/DWARF/libjarimport.dylib");
									if(dfile.exists()) {
										debuginfoURL = dfile.toURI().toURL();
										targetDebuginfoFile = new File(dir, "libjarimport.dylib.dSYM/Contents/Resources/DWARF/libjarimport.dylib");
									}
									dfile = new File(rfile.getAbsolutePath()+".dSYM/Contents/Info.plist");
									if(dfile.exists()) {
										try(InputStream s = new FileInputStream(dfile)){
											File targetDebuginfoFile2 = new File(dir, "libjarimport.dylib.dSYM/Contents/Info.plist");
											targetDebuginfoFile2.getParentFile().mkdirs();
				                    		Files.copy(s, targetDebuginfoFile2.toPath(), StandardCopyOption.REPLACE_EXISTING);
										} catch (IOException e) {
										}
									}
								} catch (URISyntaxException e1) {
								}
								break;
							case "jar":
								String path = url.toString();
								int idx = path.indexOf("!/");
								if(idx>0) {
									String jarFileURL = path.substring(4, idx);
									String filePath = path.substring(idx);
									try {
										debuginfoURL = new URL(jarFileURL.replace("-native-", "-debuginfo-"));
										if(!new File(debuginfoURL.toURI()).exists()) {
											debuginfoURL = new URL(jarFileURL.replace("-native-", "-debuginfo-").replace("/native/", "/debuginfo/"));
											if(!new File(debuginfoURL.toURI()).exists()) {
												debuginfoURL = null;
											}
										}
										if(debuginfoURL!=null) {
											URL debuginfoBaseURL = debuginfoURL;
											debuginfoURL = new URL("jar:"+debuginfoBaseURL.toString()+filePath+".dSYM/Contents/Resources/DWARF/libjarimport.dylib");
											try(InputStream s = debuginfoURL.openStream()){
												targetDebuginfoFile = new File(dir, "libjarimport.dylib.dSYM/Contents/Resources/DWARF/libjarimport.dylib");
											} catch (IOException e) {
												debuginfoURL = null;
											}
											URL debuginfoURL2 = new URL("jar:"+debuginfoBaseURL.toString()+filePath+".dSYM/Contents/Info.plist");
											try(InputStream s = debuginfoURL2.openStream()){
												File targetDebuginfoFile2 = new File(dir, "libjarimport.dylib.dSYM/Contents/Info.plist");
												targetDebuginfoFile2.getParentFile().mkdirs();
					                    		Files.copy(s, targetDebuginfoFile2.toPath(), StandardCopyOption.REPLACE_EXISTING);
											} catch (IOException e) {
												debuginfoURL2 = null;
											}
										}
									} catch (URISyntaxException e) {
									}
								}
								break;
							}
                        }
						break;
					case "linux":
					case "linux32":
					case "linux64":
					case "linux-arm64":
					case "linux-aarch64":
					case "linux-x86":
					case "linux-x64":
						targetLibFile = new File(dir, "libjarimport.so");
						if(Boolean.FALSE.equals(isDebug)) {
							URL url = entry.getValue();
							switch(url.getProtocol()) {
							case "file":
								try {
									File rfile = new File(url.toURI());
									File dfile = new File(rfile.getAbsolutePath()+".debug");
									if(dfile.exists()) {
										debuginfoURL = dfile.toURI().toURL();
										targetDebuginfoFile = new File(dir, "libjarimport.so.debug");
									}
								} catch (URISyntaxException e1) {
								}
								break;
							case "jar":
								String path = url.toString();
								int idx = path.indexOf("!/");
								if(idx>0) {
									String jarFileURL = path.substring(4, idx);
									String filePath = path.substring(idx);
									try {
										debuginfoURL = new URL(jarFileURL.replace("-native-", "-debuginfo-"));
										if(!new File(debuginfoURL.toURI()).exists()) {
											debuginfoURL = new URL(jarFileURL.replace("-native-", "-debuginfo-").replace("/native/", "/debuginfo/"));
											if(!new File(debuginfoURL.toURI()).exists()) {
												debuginfoURL = null;
											}
										}
										if(debuginfoURL!=null) {
											debuginfoURL = new URL("jar:"+debuginfoURL.toString()+filePath+".debug");
											try(InputStream s = debuginfoURL.openStream()){
												targetDebuginfoFile = new File(dir, "libjarimport.so.debug");
											} catch (IOException e) {
												debuginfoURL = null;
											}
										}
									} catch (URISyntaxException e) {
									}
								}
								break;
							}
                        }
						break;
						default: continue;
					}
                    try(InputStream in = entry.getValue().openStream()){
                    	targetLibFile.getParentFile().mkdirs();
                    	Files.copy(in, targetLibFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
                    	found = true;
					}catch(IOException e) {
                        throw new Error("Unable to write file "+targetLibFile, e);
					}
                    if(debuginfoURL!=null && targetDebuginfoFile!=null) {
                    	try(InputStream in = debuginfoURL.openStream()){
                    		targetDebuginfoFile.getParentFile().mkdirs();
                        	Files.copy(in, targetDebuginfoFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
    					}catch(IOException e) {
    					}
                    }
				}
				if(found) {
					try{
                    	Files.copy(library.toPath(), new File(dir, library.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
					}catch(IOException e) {
                        throw new Error("Unable to write file "+new File(dir, library.getName()), e);
					}
                    if(!classPaths.isEmpty()) {
                    	for(String cp : classPaths) {
                    		if(cp.endsWith(".jar")) {
                        		File jar = new File(cp);
                        		if(jar.exists()) {
                        			try{
        	                        	Files.copy(jar.toPath(), new File(dir, jar.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
        							}catch(IOException e) {
        	                            throw new Error("Unable to write file "+new File(dir, jar.getName()), e);
        							}
                        		}
                    		}
                    	}
                    }
                    try(PrintWriter writer = new PrintWriter(new File(dir, "qmldir"))){
                    	writer.print("module ");
                    	writer.println(packageName.replace('/', '.'));
                    	if(isAndroid) {
                    		writer.println("plugin qml_"+packageName.replace('/', '_')+"_jarimport");
                    	}else {
                    		writer.println("plugin jarimport");
                    	}
                    	if(!natives.isEmpty()) {
                    		writer.print("librarypath ");
                        	writer.println(new QStringList(natives).join(","));
                    	}
                    }catch(IOException e) {
                        throw new Error("Unable to write file "+new File(dir, "qmldir"), e);
					}
                    System.out.println("Plugin written to "+dir.getAbsolutePath());
				}
			}
		}else {
			throw new Error(String.format("Unable to create directory %1$s.", dir.getAbsolutePath()));
		}
	}
}
