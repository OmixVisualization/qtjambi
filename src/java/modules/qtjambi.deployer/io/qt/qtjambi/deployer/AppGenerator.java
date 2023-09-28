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

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
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
import java.util.function.IntFunction;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import io.qt.core.QByteArray;
import io.qt.core.QCborArray;
import io.qt.core.QCborMap;
import io.qt.core.QCborValue;
import io.qt.core.QCommandLineOption;
import io.qt.core.QCommandLineParser;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileDevice;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QStringList;
import io.qt.qtjambi.deployer.Main.JVMDetectionModes;
import io.qt.qtjambi.deployer.Main.Parameters;

final class AppGenerator {
	static void generate(QCommandLineParser parser, String[] args, QCommandLineOption platformOption, QCommandLineOption dirOption, QCommandLineOption classPathOption, QCommandLineOption configurationOption) throws InterruptedException, IOException{
	    QCommandLineOption applicationJNIMinVersionOption = new QCommandLineOption(QStringList.of("jni-minimum-version"), "Minimum version for the required JNI", "version");
	    QCommandLineOption applicationNameOption = new QCommandLineOption(QStringList.of("application", "application-name"), "Application name", "name");
	    QCommandLineOption applicationIcoOption = new QCommandLineOption(QStringList.of("ico", "application-icon"), "Application icon", "file");
	    QCommandLineOption applicationMPOption = new QCommandLineOption(QStringList.of("mp", "module-path"), "Module path for application execution", "path");
	    QCommandLineOption applicationLPOption = new QCommandLineOption(QStringList.of("lp", "library-path"), "Library path for application execution", "path");
	    QCommandLineOption applicationMainClassOption = new QCommandLineOption(QStringList.of("main-class"), "Main class", "class");
	    QCommandLineOption applicationAutodetectJvmOption = new QCommandLineOption(QStringList.of("autodetect-jvm"), "Autodetect Java Virtual Machine at runtime");
	    QCommandLineOption applicationJVMMinVersionOption = new QCommandLineOption(QStringList.of("minversion-jvm", "jvm-minimum-version"), "Minimum version for the autodetected Java Virtual Machine", "version");
	    QCommandLineOption applicationJVMPathOption = new QCommandLineOption(QStringList.of("jvm-path"), "Path to Java Virtual Machine (absolute or relative to app binary)", "path");
	    QCommandLineOption applicationExeOption = new QCommandLineOption(QStringList.of("executable"), "Path to executable file.\nExamples:\n--executable=path"+File.separator+"QtJambiLauncher.exe\n--executable=macos"+File.pathSeparator+"path"+File.separator+"QtJambiLauncher.app", "file");
	    QCommandLineOption applicationExeLocationOption = new QCommandLineOption(QStringList.of("executable-location"), "Directory containing QtJambiLauncher executable", "path");
	    QCommandLineOption applicationJVMArgOption = new QCommandLineOption(QStringList.of("jvmarg"), "JVM argument", "arg");
	    QCommandLineOption applicationJVMArgFiltersOption = new QCommandLineOption(QStringList.of("filter", "jvm-argument-filter"), "JVM argument filter", "filter");
	    parser.addOptions(Arrays.asList(
	    		platformOption,
	    		configurationOption,
	    		
	    		applicationJNIMinVersionOption,
	    		applicationNameOption,
	    		applicationIcoOption,
	    		applicationMPOption,
	    		applicationLPOption,
	    		applicationMainClassOption,
	    		applicationAutodetectJvmOption,
	    		applicationJVMMinVersionOption,
	    		applicationJVMPathOption,
	    		applicationExeOption,
	    		applicationExeLocationOption,
	    		applicationJVMArgFiltersOption,
	    		applicationJVMArgOption,
	    		
	    		dirOption,
	    		classPathOption
			));
	    parser.addPositionalArgument("jvmargs", "Argument for Java Virtual Machine");
	    
		if(args.length==1)
			parser.showHelp();
    	parser.process(new QStringList(args));
		
		QStringList additionalArguments = new QStringList(parser.positionalArguments());
		
		String platform = null;
		if(parser.isSet(platformOption))
			platform = parser.value(platformOption);

		String appName = null;
		if(parser.isSet(applicationNameOption))
			appName = parser.value(applicationNameOption);
		
		String mainClass = null;
		if(parser.isSet(applicationMainClassOption))
			mainClass = parser.value(applicationMainClassOption);
		
		String jvmPath = null;
		if(parser.isSet(applicationJVMPathOption))
			jvmPath = parser.value(applicationJVMPathOption);

		QDir dir = null;
		if(parser.isSet(dirOption))
			dir = new QDir(QDir.fromNativeSeparators(parser.value(dirOption)));
		
		QFile ico = null;
		if(parser.isSet(applicationIcoOption)) {
			ico = new QFile(QDir.fromNativeSeparators(parser.value(applicationIcoOption)));
			System.out.println("Setting application icon is deprecated.");
		}
		
		Integer jniMinimumVersion = null;
		if(parser.isSet(applicationJNIMinVersionOption))
			jniMinimumVersion = Integer.parseInt(parser.value(applicationJNIMinVersionOption), 16);
		
		List<String> classPaths = new ArrayList<>();
		if(parser.isSet(classPathOption))
			classPaths.addAll(Arrays.asList(parser.value(classPathOption).split(File.pathSeparator)));
		
		List<String> modulePaths = new ArrayList<>();
		if(parser.isSet(applicationMPOption))
			modulePaths.addAll(Arrays.asList(parser.value(applicationMPOption).split(File.pathSeparator)));
		
		List<String> libraryPaths = new ArrayList<>();
		if(parser.isSet(applicationLPOption))
			libraryPaths.addAll(Arrays.asList(parser.value(applicationLPOption).split(File.pathSeparator)));

		List<Map.Entry<String,URL>> executables = new ArrayList<>();
		if(parser.isSet(applicationExeOption)) {
			String[] exeinfo = parser.value(applicationExeOption).split(File.pathSeparator);
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
				}else if(exeinfo[0].endsWith("_x86_64")) {
					if(!exeFile.isDirectory()) {
						throw new Error("Specified launcher executable does not exist: "+exeinfo[0]);
					}
					os = "android-x64";
				}else if(exeinfo[0].endsWith("_x86")) {
					if(!exeFile.isDirectory()) {
						throw new Error("Specified launcher executable does not exist: "+exeinfo[0]);
					}
					os = "android-x86";
				}else if(exeinfo[0].endsWith("_arm64-v8a")) {
					if(!exeFile.isDirectory()) {
						throw new Error("Specified launcher executable does not exist: "+exeinfo[0]);
					}
					os = "android-arm64";
				}else if(exeinfo[0].endsWith("_armeabi-v7a")) {
					if(!exeFile.isDirectory()) {
						throw new Error("Specified launcher executable does not exist: "+exeinfo[0]);
					}
					os = "android-arm";
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
		}else if(parser.isSet(applicationExeLocationOption)) {
			QDir location = new QDir(parser.value(applicationExeLocationOption));
			for(String entry : location.entryList(QDir.Filter.Files)) {
                String os = null;
                if(entry.equals("QtJambiLauncher.exe")) {
                    os = "windows";
                }else if(entry.equals("QtJambiLauncher.app")) {
                    os = "macos";
                }else if(entry.equals("QtJambiLauncher_x86_64")) {
                    os = "android-x64";
                }else if(entry.equals("QtJambiLauncher_x86")) {
                    os = "android-x86";
                }else if(entry.equals("QtJambiLauncher_armeabi-v7a")) {
                    os = "android-arm";
                }else if(entry.equals("QtJambiLauncher_arm64-v8a")) {
                    os = "android-arm64";
                }else if(entry.equals("QtJambiLauncher")) {
                    os = "linux";
                }
                if(os!=null && entry.contains("QtJambiLauncher")) {
                	File libFile = new File(location.absoluteFilePath(entry));
                	executables.add(new SimpleEntry<>(os, libFile.toURI().toURL()));
                }
			}
		}
		
        boolean autoDetect = parser.isSet(applicationAutodetectJvmOption);
        
        int minimumJVMVersion = 11;
		if(parser.isSet(applicationJVMMinVersionOption))
			minimumJVMVersion = Integer.parseInt(parser.value(applicationJVMMinVersionOption));
		
		List<String> filters = new ArrayList<>();
		if(parser.isSet(applicationJVMArgFiltersOption))
			filters.addAll(parser.values(applicationJVMArgFiltersOption));
		
		List<String> arguments = new ArrayList<>(additionalArguments);
		if(parser.isSet(applicationJVMArgOption))
			arguments.addAll(parser.values(applicationJVMArgOption));

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
//                        String configuration = doc.getDocumentElement().getAttribute("configuration");
	                        for(int i=0; i<doc.getDocumentElement().getChildNodes().getLength(); ++i) {
	                        	Node child = doc.getDocumentElement().getChildNodes().item(i);
	                        	if(child instanceof Element && (child.getNodeName().equals("library") || child.getNodeName().equals("file"))) {
	                        		Element element = (Element)child;
	                        		String library = element.getAttribute("name");
	                        		if(library.endsWith("QtJambiLauncher") 
	                        				|| library.endsWith("QtJambiLauncher_debug")
	                        				|| library.endsWith(".exe")) {
	                        			URL libraryURL = new URL(eform.substring(0, end+2)+library);
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
		IntFunction<QCborValue[]> arrayFactory = QCborValue[]::new;
		cborValue.setValue(Parameters.MainClass.value(), new QCborValue(mainClass));
		QCborArray argumentArray = new QCborArray(arguments.stream().map(QByteArray::new).map(QCborValue::new).toArray(arrayFactory));
		cborValue.setValue(Parameters.JVMArguments.value(), new QCborValue(argumentArray));
		QCborArray filtersArray = new QCborArray(filters.stream().map(QByteArray::new).map(QCborValue::new).toArray(arrayFactory));
		cborValue.setValue(Parameters.JVMArgumentFilters.value(), new QCborValue(filtersArray));
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
			if(os!=null && (platform==null || platform.startsWith(os))) {
                URL url = entry.getValue();
				QFile newFile;
				URL debuginfoURL = null;
				QFile debuginfoFile = null;
                switch(os.toLowerCase()) {
				case "linux":
				case "linux32":
				case "linux64":
				case "linux-arm":
				case "linux-arm32":
				case "linux-arm64":
				case "linux-aarch64":
				case "linux-x86":
				case "linux-x64":
					newFile = new QFile(dir.absoluteFilePath(appName));
					switch(url.getProtocol()) {
					case "file":
						try {
							File rfile = new File(url.toURI());
							File dfile = new File(rfile.getAbsolutePath()+".debug");
							if(dfile.exists()) {
								debuginfoURL = dfile.toURI().toURL();
								debuginfoFile = new QFile(dir.absoluteFilePath(appName+".debug"));
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
										debuginfoFile = new QFile(dir.absoluteFilePath(appName+".debug"));
									} catch (IOException e) {
										debuginfoURL = null;
									}
								}
							} catch (URISyntaxException e) {
							}
						}
						break;
					}
//					debuginfoFile = new QFile(dir.absoluteFilePath(appName+".debug"));
					break;
				case "android-arm":
				case "android-arm32":
					newFile = new QFile(dir.absoluteFilePath(appName+"_armeabi-v7a"));
					break;
				case "android-arm64":
				case "android-aarch64":
					newFile = new QFile(dir.absoluteFilePath(appName+"_arm64-v8a"));
					break;
				case "android-x86":
					newFile = new QFile(dir.absoluteFilePath(appName+"_x86"));
					break;
				case "android-x64":
					newFile = new QFile(dir.absoluteFilePath(appName+"_x86_64"));
					break;
//				default: continue;
				case "win32":
				case "win64":
				case "windows":
				case "windows-arm":
				case "windows-arm32":
				case "windows-aarch64":
				case "windows-arm64":
				case "windows-x86":
				case "windows-x64":
					newFile = new QFile(dir.absoluteFilePath(appName + ".exe"));
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
								debuginfoFile = new QFile(dir.absoluteFilePath(appName + (dfile.getName().endsWith(".pdb") ? ".pdb" : ".exe.debug")));
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
										debuginfoFile = new QFile(dir.absoluteFilePath(appName+".pdb"));
									} catch (IOException e) {
										debuginfoURL = new URL("jar:"+debuginfoURL.toString()+filePath+".debug");
										try(InputStream s = debuginfoURL.openStream()){
											debuginfoFile = new QFile(dir.absoluteFilePath(appName+filePath.substring(filePath.length()-4)+".debug"));
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
					System.gc();
					break;
				case "macos":
				case "osx":
					String fileBase = url.toExternalForm();
					if(fileBase.endsWith("/"))
						fileBase = fileBase.substring(0, fileBase.length()-1);
					{
						int idx = fileBase.lastIndexOf(".app/");
						if(idx>0)
							fileBase = fileBase.substring(0, idx+4);
					}
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
					}catch(IOException e) {
						e.printStackTrace();
					}
                    try {
                    	URL pkgInfoUrl = new URL(fileBase + "/Contents/PkgInfo");
						QIODevice device = QIODevice.fromInputStream(pkgInfoUrl.openStream());
                        QByteArray data = device.readAll();
                        device.close();
                        dir.mkpath(appName + ".app/Contents");
                        QFile pkgInfo = new QFile(dir.absoluteFilePath(appName + ".app/Contents/PkgInfo"));
						pkgInfo.remove();
						if(pkgInfo.open(QIODevice.OpenModeFlag.WriteOnly)) {
							pkgInfo.write(data);
							pkgInfo.close();
						}
					}catch(IOException e) {
					}
                    try {
	                    URL emptylprojUrl = new URL(fileBase + "/Contents/Resources/empty.lproj");
	                    QIODevice device = QIODevice.fromInputStream(emptylprojUrl.openStream());
                        QByteArray data = device.readAll();
                        device.close();
                        dir.mkpath(appName + ".app/Contents/Resources");
                        QFile emptylproj = new QFile(dir.absoluteFilePath(appName + ".app/Contents/Resources/empty.lproj"));
						emptylproj.remove();
						if(emptylproj.open(QIODevice.OpenModeFlag.WriteOnly)) {
							emptylproj.write(data);
							emptylproj.close();
						}
					}catch(IOException e) {
					}
                    try {
	                    URL executableUrl = new URL(fileBase + "/Contents/MacOS/"+baseName);
	                    QIODevice device = QIODevice.fromInputStream(executableUrl.openStream());
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
					
					switch(url.getProtocol()) {
					case "file":
						try {
							File rfile = new File(new URL(fileBase).toURI());
							File dfile = new File(rfile.getAbsolutePath()+".dSYM/Contents/Resources/DWARF/QtJambiLauncher");
							if(dfile.exists()) {
								debuginfoURL = dfile.toURI().toURL();
								File targetDebuginfoFile = new File(dir.absolutePath(), appName+".app.dSYM/Contents/Resources/DWARF/"+appName);
								try(InputStream in = debuginfoURL.openStream()){
		                    		targetDebuginfoFile.getParentFile().mkdirs();
		                        	Files.copy(in, targetDebuginfoFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
		    					}catch(IOException e) {
		    					}
							}
							dfile = new File(rfile.getAbsolutePath()+".dSYM/Contents/Info.plist");
							if(dfile.exists()) {
								try {
									QByteArray data = new QByteArray(Files.readAllBytes(dfile.toPath()));
									File targetDebuginfoFile = new File(dir.absolutePath(), appName+".app.dSYM/Contents/Info.plist");
		                    		targetDebuginfoFile.getParentFile().mkdirs();
		                        	data = data.replace("QtJambiLauncher", appName);
		                        	Files.copy(new ByteArrayInputStream(data.toArray()), targetDebuginfoFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
		    					}catch(IOException e) {
		    					}
							}
						} catch (URISyntaxException e1) {
						}
						break;
					case "jar":
						int idx = fileBase.indexOf("!/");
						if(idx>0) {
							String jarFileURL = fileBase.substring(4, idx);
							String filePath = fileBase.substring(idx);
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
									debuginfoURL = new URL("jar:"+debuginfoBaseURL.toString()+filePath+".dSYM/Contents/Resources/DWARF/QtJambiLauncher");
									try(InputStream s = debuginfoURL.openStream()){
										File targetDebuginfoFile = new File(dir.absolutePath(), appName+".app.dSYM/Contents/Resources/DWARF/"+appName);
										targetDebuginfoFile.getParentFile().mkdirs();
			                        	Files.copy(s, targetDebuginfoFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
									} catch (IOException e) {
										debuginfoURL = null;
									}
									URL debuginfoURL2 = new URL("jar:"+debuginfoBaseURL.toString()+filePath+".dSYM/Contents/Info.plist");
									try(InputStream s = debuginfoURL2.openStream()){
										QIODevice device = QIODevice.fromInputStream(s);
										File targetDebuginfoFile2 = new File(dir.absolutePath(), appName+".app.dSYM/Contents/Info.plist");
										targetDebuginfoFile2.getParentFile().mkdirs();
			                        	QByteArray data = device.readAll();
			                        	data = data.replace("QtJambiLauncher", appName);
										Files.copy(new ByteArrayInputStream(data.toArray()), targetDebuginfoFile2.toPath(), StandardCopyOption.REPLACE_EXISTING);
									} catch (IOException e) {
										debuginfoURL2 = null;
									}
								}
							} catch (URISyntaxException e) {
							}
						}
						break;
					}
					
                    continue;
                    default: continue;
				}
                try {
                    QIODevice device = QIODevice.fromInputStream(url.openStream());
                    QByteArray exeData = device.readAll();
                    device.close();
                    int idx = (int)exeData.indexOf(QTJAMBI_LAUNCHER);
                    if(idx>0) {
                    	new QFileInfo(newFile).absoluteDir().mkpath(".");
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
        						case "windows-arm":
        						case "windows-arm32":
        						case "windows-aarch64":
        						case "windows-amd64":
        						case "windows-x64":
        						case "windows-x86":
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
                        throw new Error("Unable to find \""+QTJAMBI_LAUNCHER+"\" in file "+url.toExternalForm());
                    }
				}catch(IOException e) {
                    throw new Error("Unable to read file "+url.toExternalForm(), e);
				}
                if(debuginfoURL!=null && debuginfoFile!=null) {
                	try {
                        QIODevice device = QIODevice.fromInputStream(debuginfoURL.openStream());
                        QByteArray debugData = device.readAll();
                        device.close();
                        debuginfoFile.remove();
                        new QFileInfo(debuginfoFile).absoluteDir().mkpath(".");
                        if(debuginfoFile.open(QIODevice.OpenModeFlag.WriteOnly)) {
                        	debuginfoFile.write(debugData);
                        	debuginfoFile.close();
                        }
                	}catch(IOException e) {}
                }
			}
		}
	}
}
