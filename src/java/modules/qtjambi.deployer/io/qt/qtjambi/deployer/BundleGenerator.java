package io.qt.qtjambi.deployer;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.function.Function;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.jar.JarInputStream;
import java.util.jar.JarOutputStream;
import java.util.jar.Manifest;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import io.qt.QtUtilities;
import io.qt.core.QCommandLineOption;
import io.qt.core.QCommandLineParser;
import io.qt.core.QDir;
import io.qt.core.QLibraryInfo;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QPair;
import io.qt.core.QProcess;
import io.qt.core.QStringList;
import io.qt.core.QVersionNumber;

final class BundleGenerator {
	
	private static Map<String,Set<String>> pluginsByModules;
	private static Map<String,Set<String>> pluginsByModules(String osArchName){
		if(pluginsByModules==null) {
			Map<String,String> _pluginsModules = new TreeMap<>();
			_pluginsModules.put("3dinputdevices", "qt3dinput");
			_pluginsModules.put("renderplugins", "qt3drender");
			_pluginsModules.put("geometryloaders", "qt3drender");
			_pluginsModules.put("sceneparsers", "qt3drender");
			_pluginsModules.put("renderers", "qt3drender");
			_pluginsModules.put("geometryloaders", "qt3drender");
			_pluginsModules.put("accessible", "gui");
			_pluginsModules.put("accessiblebridge", "gui");
			_pluginsModules.put("iconengines", "gui");
			_pluginsModules.put("imageformats", "gui");
			_pluginsModules.put("generic", "gui");
			_pluginsModules.put("platforminputcontexts", "gui");
			if(osArchName.startsWith("android-")) {
				_pluginsModules.put("platforms", "core");
			}else {
				_pluginsModules.put("platforms", "gui");
			}
			_pluginsModules.put("platformthemes", "gui");
			_pluginsModules.put("networkaccess", "network");
			_pluginsModules.put("networkinformation", "network");
			_pluginsModules.put("tls", "network");
			_pluginsModules.put("egldeviceintegrations", "gui");
			_pluginsModules.put("xcbglintegrations", "gui");
			_pluginsModules.put("printsupport", "printsupport");
			_pluginsModules.put("sqldrivers", "sql");
			_pluginsModules.put("styles", "widgets");
			_pluginsModules.put("qmltooling", "qml");
			_pluginsModules.put("qmllint", "qml");
			_pluginsModules.put("qmlls", "qml");
			_pluginsModules.put("scenegraph", "quick");
			_pluginsModules.put("opcua", "opcua");
			_pluginsModules.put("position", "positioning");
			_pluginsModules.put("assetimporters", "quick3d");
			_pluginsModules.put("scxmldatamodel", "scxml");
			_pluginsModules.put("canbus", "serialbus");
			_pluginsModules.put("virtualkeyboard", "virtualkeyboard");
			_pluginsModules.put("wayland-graphics-integration-client", "waylandclient");
			_pluginsModules.put("wayland-inputdevice-integration", "waylandclient");
			_pluginsModules.put("wayland-decoration-client", "waylandclient");
			_pluginsModules.put("wayland-shell-integration", "waylandclient");
			_pluginsModules.put("wayland-graphics-integration-server", "waylandcompositor");
			_pluginsModules.put("wayland-hardware-layer-integration", "waylandcompositor");
			_pluginsModules.put("wayland-graphics-integration-server", "waylandcompositor");
			_pluginsModules.put("webview", "webview");
			_pluginsModules.put("gamepads", "gamepad");
			_pluginsModules.put("geoservices", "location");
			_pluginsModules.put("texttospeech", "texttospeech");
			_pluginsModules.put("designer", "designer");
			_pluginsModules.put("sensors", "sensors");
			_pluginsModules.put("sensorgestures", "sensors");
			_pluginsModules.put("video", "multimedia");
			_pluginsModules.put("audio", "multimedia");
			_pluginsModules.put("mediaservice", "multimedia");
			_pluginsModules.put("resourcepolicy", "multimedia");
			_pluginsModules.put("playlistformats", "multimedia");
			_pluginsModules.put("multimedia", "multimedia");
			_pluginsModules.put("bearer", "network");
			_pluginsModules.put("qtwebengine", "webenginecore");
			_pluginsModules.put("help", "help");
			Map<String,Set<String>> _pluginsByModules = new TreeMap<>();
			Function<String,Set<String>> containerFactory = key->new TreeSet<>();
			for(Map.Entry<String,String> entry : _pluginsModules.entrySet()) {
				_pluginsByModules.computeIfAbsent(entry.getValue(), containerFactory).add(entry.getKey());
			}
			for(Map.Entry<String,Set<String>> entry : _pluginsByModules.entrySet()) {
				entry.setValue(Collections.unmodifiableSet(entry.getValue()));
			}
//			pluginsModules = Collections.unmodifiableMap(_pluginsModules);
			pluginsByModules = Collections.unmodifiableMap(_pluginsByModules);
		}
		return pluginsByModules;
	}
	
	static void generate(QCommandLineParser parser, String[] args, QCommandLineOption platformOption, QCommandLineOption dirOption, QCommandLineOption configurationOption) throws InterruptedException, IOException{
		QCommandLineOption qtdirOption = new QCommandLineOption(QStringList.of("qtdir"), "Directory containing Qt (Qt prefix)", "directory");
		QCommandLineOption binsOption = new QCommandLineOption(QStringList.of("bins"), "Directory containing Qt binaries", "directory");
		QCommandLineOption libsOption = new QCommandLineOption(QStringList.of("libs"), "Directory containing Qt libraries", "directory");
		QCommandLineOption libexecsOption = new QCommandLineOption(QStringList.of("libexecs"), "Directory containing Qt library executables", "directory");
		QCommandLineOption pluginsOption = new QCommandLineOption(QStringList.of("plugins"), "Directory containing Qt plugins", "directory");
		QCommandLineOption qmlOption = new QCommandLineOption(QStringList.of("qml"), "Directory containing Qt qml libraries", "directory");
		QCommandLineOption resourcesOption = new QCommandLineOption(QStringList.of("resources"), "Directory containing Qt resources", "directory");
		QCommandLineOption translationsOption = new QCommandLineOption(QStringList.of("translations"), "Directory containing Qt translations", "directory");
		QCommandLineOption qtbaseOption = new QCommandLineOption(QStringList.of("qtbase"), "Installation directory of Qt", "directory");
		QCommandLineOption versionOption = new QCommandLineOption(QStringList.of("target-version"), "Qt version", "version");
	    QCommandLineOption utilitiesOption = new QCommandLineOption(QStringList.of("utilities"), "Path to android utilities.\nExamples:\n--utilities=utilities/QtAndroidBindings.jar", "file");
	    QCommandLineOption utilitiesLocationOption = new QCommandLineOption(QStringList.of("utilities-location"), "Directory containing android utilities", "path");
	    QCommandLineOption forceDebugInfoOption = new QCommandLineOption(QStringList.of("force-debug-info"), "Forces to also bundle release debug info if available");
		parser.addOptions(Arrays.asList(platformOption, dirOption, 
				configurationOption, 
				qtdirOption, 
				qtbaseOption,
				binsOption,
				libsOption,
				libexecsOption,
				pluginsOption,
				qmlOption,
				resourcesOption,
				translationsOption,
				versionOption, 
				utilitiesOption, 
				forceDebugInfoOption,
				utilitiesLocationOption));
    	parser.process(new QStringList(args));
		QStringList unusedArguments = new QStringList(parser.positionalArguments());
		if(unusedArguments.size()==1)
			throw new Error("Qt bundle generation, illegal argument: "+unusedArguments.join(", "));
		if(unusedArguments.size()>1)
			throw new Error("Qt bundle generation, illegal argument: "+unusedArguments.join(", "));
		boolean isDebug = parser.isSet(configurationOption) && parser.value(configurationOption).equals("debug");
		boolean isForceDebugInfo = parser.isSet(forceDebugInfoOption);
		File targetDir = new File(parser.isSet(dirOption) ? parser.value(dirOption) : System.getProperty("user.dir"));
		File qtdir = null;
		File qtbase = null;
		Boolean isMinGWBuilt = null;
		QVersionNumber version = null;
		String osArchName = null;
		URL androidBindingsURL = null;
		
		if(parser.isSet(utilitiesOption)) {
			File utilitiesJar = new File(parser.value(utilitiesOption));
			if(!utilitiesJar.exists())
				throw new Error(String.format("Utilities file does not exist: %1$s", utilitiesJar.getAbsolutePath()));
			androidBindingsURL = utilitiesJar.toURI().toURL();
		}else if(parser.isSet(utilitiesLocationOption)) {
			QDir location = new QDir(parser.value(utilitiesLocationOption));
			File utilitiesJar = new File(location.absoluteFilePath("QtAndroidBindings.jar"));
			if(!utilitiesJar.exists())
				throw new Error("utilities-location does not contain QtAndroidBindings.jar.");
			androidBindingsURL = utilitiesJar.toURI().toURL();
		}
		if(androidBindingsURL==null) {
			Enumeration<URL> specsFound = Main.findSpecs();
            Set<URL> specsUrls = new HashSet<>();
            while (specsFound.hasMoreElements() && androidBindingsURL==null) {
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
	                        if(system!=null && system.startsWith("android")) {
		                        for(int i=0, l = doc.getDocumentElement().getChildNodes().getLength(); i<l; ++i) {
		                        	Node child = doc.getDocumentElement().getChildNodes().item(i);
		                        	if(child instanceof Element && child.getNodeName().equals("file")) {
		                        		Element element = (Element)child;
		                        		String libraryName = element.getAttribute("name");
		                        		if(libraryName.equals("utilities/QtAndroidBindings.jar")) {
		                        			androidBindingsURL = Main.createURL(eform.substring(0, end+2)+libraryName);
		                        			break;
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
		if(parser.isSet(qtdirOption)) {
			if(parser.isSet(qtbaseOption))
				throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", qtbaseOption.valueName(), qtdirOption.valueName()));
			if(parser.isSet(binsOption))
				throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", binsOption.valueName(), qtdirOption.valueName()));
			if(parser.isSet(libsOption))
				throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", libsOption.valueName(), qtdirOption.valueName()));
			if(parser.isSet(libexecsOption))
				throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", libexecsOption.valueName(), qtdirOption.valueName()));
			if(parser.isSet(pluginsOption))
				throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", pluginsOption.valueName(), qtdirOption.valueName()));
			if(parser.isSet(qmlOption))
				throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", qmlOption.valueName(), qtdirOption.valueName()));
			if(parser.isSet(resourcesOption))
				throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", resourcesOption.valueName(), qtdirOption.valueName()));
			if(parser.isSet(translationsOption))
				throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", translationsOption.valueName(), qtdirOption.valueName()));
			qtdir = new File(parser.value(qtdirOption));
		}else {
			if(parser.isSet(versionOption)) {
				version = QVersionNumber.fromString(parser.value(versionOption));
			}
			if(parser.isSet(qtbaseOption)) {
				if(parser.isSet(binsOption))
					throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", binsOption.valueName(), qtbaseOption.valueName()));
				if(parser.isSet(libsOption))
					throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", libsOption.valueName(), qtbaseOption.valueName()));
				if(parser.isSet(libexecsOption))
					throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", libexecsOption.valueName(), qtbaseOption.valueName()));
				if(parser.isSet(pluginsOption))
					throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", pluginsOption.valueName(), qtbaseOption.valueName()));
				if(parser.isSet(qmlOption))
					throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", qmlOption.valueName(), qtbaseOption.valueName()));
				if(parser.isSet(resourcesOption))
					throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", resourcesOption.valueName(), qtbaseOption.valueName()));
				if(parser.isSet(translationsOption))
					throw new Error(String.format("Qt bundle generation: either use --%1$s or --%2$s but not both.", translationsOption.valueName(), qtbaseOption.valueName()));
				qtbase = new File(parser.value(qtbaseOption));
			}else if(!parser.isSet(binsOption)
					&& !parser.isSet(libsOption)
					&& !parser.isSet(libexecsOption)
					&& !parser.isSet(pluginsOption)
					&& !parser.isSet(qmlOption)
					&& !parser.isSet(resourcesOption)
					&& !parser.isSet(translationsOption)){
				String base = null;
				if(QOperatingSystemVersion.current().type()==QOperatingSystemVersion.OSType.Windows) {
					base = "C:\\Qt";
					if(base==null || !new File(base).isDirectory()) {
						base = null;
					}
				}else if(QOperatingSystemVersion.current().type()==QOperatingSystemVersion.OSType.MacOS) {
					base = "/Library/Qt";
					if(base==null || !new File(base).isDirectory()) {
						base = "/System/Qt";
					}
					if(base==null || !new File(base).isDirectory()) {
						base = "/opt/Qt";
					}
					if(base==null || !new File(base).isDirectory()) {
						base = null;
					}
				}else {
					base = "/opt/Qt";
					if(base==null || !new File(base).isDirectory()) {
						base = null;
					}
				}
				if(base==null || !new java.io.File(base).isDirectory()) {
					base = System.getProperty("user.home")+java.io.File.separator+"Qt";
				}
				if(base==null || !new java.io.File(base).isDirectory()) {
					base = System.getProperty("user.dir")+java.io.File.separator+".."+java.io.File.separator+"Qt";
				}
				if(base==null || !(qtbase = new java.io.File(base)).isDirectory()) {
					base = null;
					qtbase = null;
				}
				if(qtbase==null && (version==null || QLibraryInfo.version().equals(version))) {
					File qtLibraryPath = DeployerUtility.qtLibraryPath();
					if(qtLibraryPath!=null && qtLibraryPath.isDirectory()) {
						qtdir = qtLibraryPath.getParentFile();
						isMinGWBuilt = DeployerUtility.isMinGWBuilt();
						osArchName = DeployerUtility.osArchName();
						version = QLibraryInfo.version();
					}
				}
			}
		}
		if(qtbase!=null) {
			int patchVersion = -1;
			java.io.File foundVersionDir = null;
			QVersionNumber qtjambiVersion = QtUtilities.qtjambiVersion();
			if(version==null) {
				for(java.io.File versionDir : qtbase.listFiles()) {
					String qtjambiVersionString = String.format("%1$s.%2$s.", qtjambiVersion.majorVersion(), qtjambiVersion.minorVersion());
					if(versionDir.isDirectory() && versionDir.getName().startsWith(qtjambiVersionString)) {
						try {
							int patch = Integer.parseInt(versionDir.getName().substring(qtjambiVersionString.length()));
							if(patch>patchVersion){
								patchVersion = patch;
								foundVersionDir = versionDir;
							}
						} catch (Exception e) {
						}
					}
				}
			}else {
				foundVersionDir = new File(qtbase, version.toString());
				if(!foundVersionDir.isDirectory())
					throw new Error(String.format("Qt bundle generation: Unknown version '%1$s'.", version));
			}
			if(foundVersionDir!=null) {
				if(version==null)
					version = new QVersionNumber(qtjambiVersion.majorVersion(), qtjambiVersion.minorVersion(), patchVersion);
				isMinGWBuilt = DeployerUtility.isMinGWBuilt();
				List<QPair<String,File>> qtdirs = new ArrayList<>();
				if(parser.isSet(platformOption)) {
					osArchName = parser.value(platformOption);
					File subdir = null;
					switch(osArchName) {
					case "android-arm64":
						if(!(subdir = new File(foundVersionDir, "android")).isDirectory()) {
							subdir = new File(foundVersionDir, "android_arm64_v8a");
						}
						break;
					case "android-arm":
						if(!(subdir = new File(foundVersionDir, "android")).isDirectory()) {
							subdir = new File(foundVersionDir, "android_armv7");
						}
						break;
					case "android-x64":
						if(!(subdir = new File(foundVersionDir, "android")).isDirectory()) {
							subdir = new File(foundVersionDir, "android_x86_64");
						}
						break;
					case "android-x86":
						if(!(subdir = new File(foundVersionDir, "android")).isDirectory()) {
							subdir = new File(foundVersionDir, "android_x86");
						}
						break;
					case "windows-mingw-x64":
						subdir = new File(foundVersionDir, "mingw_64");
						if(!subdir.isDirectory()) {
							File _subdir = new File(foundVersionDir, "mingw81_64");
							if(_subdir.isDirectory())
								subdir = _subdir;
						}
						break;
					case "windows-llvm-mingw-x64":
						subdir = new File(foundVersionDir, "llvm-mingw_64");
						break;
					case "windows-x64":
						subdir = new File(foundVersionDir, isMinGWBuilt ? "mingw_64" : "msvc2019_64");
						if(isMinGWBuilt && !subdir.isDirectory()) {
							File _subdir = new File(foundVersionDir, "mingw81_64");
							if(_subdir.isDirectory())
								subdir = _subdir;
						}
						break;
					case "windows-arm64":
						subdir = new File(foundVersionDir, "msvc2019_arm64");
						break;
					case "linux-x64":
						subdir = new File(foundVersionDir, "gcc_64");
						break;
					case "linux-arm64":
						subdir = new File(foundVersionDir, "gcc_arm64");
						break;
					case "macos":
						if(!(subdir = new File(foundVersionDir, "macos")).isDirectory()) {
							subdir = new File(foundVersionDir, "clang_64");
						}
						break;
					case "ios":
						subdir = new File(foundVersionDir, "ios");
//						if(!(subdir = new File(foundVersionDir, "ios")).isDirectory()) {
//							subdir = new File(foundVersionDir, "clang_64");
//						}
						break;
					default:
						throw new Error(String.format("Qt bundle generation: Unknown target platform '%1$s'.", osArchName));
					}
					if(subdir!=null 
							&& subdir.isDirectory()
							&& (new File(subdir, "bin").isDirectory() || new File(subdir, "lib").isDirectory())
							&& new File(subdir, "plugins").isDirectory())
						qtdirs.add(new QPair<>(osArchName, subdir));
				}else {
					for(File subdir : foundVersionDir.listFiles()) {
						if(subdir.isDirectory()) {
							osArchName = null;
							if(subdir.getName().equals("android")) {
								osArchName = null;
								if((new File(subdir, "bin").isDirectory() || new File(subdir, "lib").isDirectory())
										&& new File(subdir, "plugins").isDirectory()) {
									qtdirs.add(new QPair<>("android-arm64", subdir));
									qtdirs.add(new QPair<>("android-arm", subdir));
									qtdirs.add(new QPair<>("android-x64", subdir));
									qtdirs.add(new QPair<>("android-x86", subdir));
								}
							}else if(subdir.getName().equals("android_arm64_v8a")) {
								osArchName = "android-arm64";
							}else if(subdir.getName().equals("android_armv7")) {
								osArchName = "android-arm";
							}else if(subdir.getName().equals("android_x86_64")) {
								osArchName = "android-x64";
							}else if(subdir.getName().equals("android_x86")) {
								osArchName = "android-x86";
							}else if(subdir.getName().startsWith("mingw")) {
								if(!isMinGWBuilt)
									continue;
								if(subdir.getName().endsWith("_64"))
									osArchName = "windows-mingw-x64";
								else
									osArchName = "windows-mingw-x86";
							}else if(subdir.getName().startsWith("llvm-mingw")) {
								if(!isMinGWBuilt)
									continue;
								if(subdir.getName().endsWith("_64"))
									osArchName = "windows-llvm-mingw-x64";
								else
									continue;
							}else if(subdir.getName().startsWith("msvc2019")) {
								if(isMinGWBuilt)
									continue;
								if(subdir.getName().endsWith("_64"))
									osArchName = "windows-x64";
								else if(subdir.getName().endsWith("_arm64"))
									osArchName = "windows-arm64";
								else
									osArchName = "windows-x86";
							}else if(subdir.getName().startsWith("gcc")) {
								if(subdir.getName().endsWith("_64"))
									osArchName = "linux-x64";
								else
									osArchName = "linux-x86";
							}else if(subdir.getName().equals("clang_64") || subdir.getName().equals("macos")) {
								osArchName = "macos";
							}else if(subdir.getName().equals("ios")) {
								osArchName = "ios";
							}
							if(osArchName!=null
									&& (new File(subdir, "bin").isDirectory() || new File(subdir, "lib").isDirectory())
									&& new File(subdir, "plugins").isDirectory()) {
								qtdirs.add(new QPair<>(osArchName, subdir));
							}
						}
					}
				}
				if(!targetDir.exists())
					targetDir.mkdirs();
				for (QPair<String, File> qPair : qtdirs) {
					generateBundles(targetDir, qPair.second, qPair.first, androidBindingsURL, isDebug, isForceDebugInfo, isMinGWBuilt, version);
				}
			}else if(parser.isSet(qtbaseOption)) {
				throw new Error(String.format("Qt bundle generation: Unable to find Qt in directory specified by --%1$s.", qtbaseOption.valueName()));
			}else {
				throw new Error(String.format("Qt bundle generation: Unable to find Qt. Either use --%1$s or --%2$s.", qtbaseOption.valueName(), qtdirOption.valueName()));
			}
		}else if(qtdir!=null) {
			if(!(new File(qtdir, "bin").isDirectory() || new File(qtdir, "lib").isDirectory())
					|| !new File(qtdir, "plugins").isDirectory()) {
				throw new Error("Qt bundle generation: Specified Qt location does not contain Qt libraries.");
			}
			if(parser.isSet(platformOption))
				osArchName = parser.value(platformOption);
			if(osArchName==null) {
				if(qtdir.getName().equals("android")) {
					osArchName = "android";
				}else if(qtdir.getName().equals("android_arm64_v8a")) {
					osArchName = "android-arm64";
				}else if(qtdir.getName().equals("android_armv7")) {
					osArchName = "android-arm";
				}else if(qtdir.getName().equals("android_x86_64")) {
					osArchName = "android-x64";
				}else if(qtdir.getName().equals("android_x86")) {
					osArchName = "android-x86";
				}else if(qtdir.getName().startsWith("mingw")) {
					if(qtdir.getName().endsWith("_64"))
						osArchName = "windows-mingw-x64";
					else if(qtdir.getName().endsWith("_arm64"))
						osArchName = "windows-mingw-arm64";
					else
						osArchName = "windows-x86";
				}else if(qtdir.getName().startsWith("llvm-mingw")) {
					if(qtdir.getName().endsWith("_64"))
						osArchName = "windows-llvm-mingw-x64";
					else if(qtdir.getName().endsWith("_arm64"))
						osArchName = "windows-llvm-mingw-arm64";
					else
						osArchName = "windows-x86";
				}else if(qtdir.getName().startsWith("msvc20")) {
					if(qtdir.getName().endsWith("_64"))
						osArchName = "windows-x64";
					else if(qtdir.getName().endsWith("_arm64"))
						osArchName = "windows-arm64";
					else
						osArchName = "windows-x86";
				}else if(qtdir.getName().startsWith("gcc")) {
					if(qtdir.getName().endsWith("_64"))
						osArchName = "linux-x64";
					else
						osArchName = "linux-x86";
				}else if(qtdir.getName().equals("clang_64") || qtdir.getName().equals("macos")) {
					osArchName = "macos";
				}else if(qtdir.getName().equals("ios")) {
					osArchName = "ios";
				}
			}
			if(isMinGWBuilt==null) {
				isMinGWBuilt = qtdir.getName().startsWith("mingw");
			}
			if(parser.isSet(platformOption) && !parser.value(platformOption).equals(osArchName))
				throw new Error(String.format("Qt bundle generation: Platform specified by --%1$s is not available in %2$s.", platformOption.valueName(), qtdir));
			if(!targetDir.exists())
				targetDir.mkdirs();
			if(parser.isSet(versionOption))
				version = QVersionNumber.fromString(parser.value(versionOption));
			else
				version = null;
			if("android".equals(osArchName)) {
				generateBundles(targetDir, qtdir, "android-arm64", androidBindingsURL, isDebug, isForceDebugInfo, isMinGWBuilt, version);
				generateBundles(targetDir, qtdir, "android-arm", androidBindingsURL, isDebug, isForceDebugInfo, isMinGWBuilt, version);
				generateBundles(targetDir, qtdir, "android-x64", androidBindingsURL, isDebug, isForceDebugInfo, isMinGWBuilt, version);
				generateBundles(targetDir, qtdir, "android-x86", androidBindingsURL, isDebug, isForceDebugInfo, isMinGWBuilt, version);
			}else {
				generateBundles(targetDir, qtdir, osArchName, androidBindingsURL, isDebug, isForceDebugInfo, isMinGWBuilt, version);
			}
		}else if(parser.isSet(binsOption)
				|| parser.isSet(libsOption)
				|| parser.isSet(libexecsOption)
				|| parser.isSet(pluginsOption)
				|| parser.isSet(qmlOption)
				|| parser.isSet(resourcesOption)
				|| parser.isSet(translationsOption)) {
			File binDir = null;
			File libDir = null;
			File libExecDir = null;
			File pluginsDir = null;
			File qmlDir = null;
			File resourcesDir = null;
			File translationsDir = null;
			if(parser.isSet(binsOption))
				binDir = new File(parser.value(binsOption));
			if(parser.isSet(libsOption))
				libDir = new File(parser.value(libsOption));
			if(parser.isSet(libexecsOption))
				libExecDir = new File(parser.value(libexecsOption));
			if(parser.isSet(pluginsOption))
				pluginsDir = new File(parser.value(pluginsOption));
			if(parser.isSet(qmlOption))
				qmlDir = new File(parser.value(qmlOption));
			if(parser.isSet(resourcesOption))
				resourcesDir = new File(parser.value(resourcesOption));
			if(parser.isSet(translationsOption))
				translationsDir = new File(parser.value(translationsOption));
			if(parser.isSet(platformOption))
				osArchName = parser.value(platformOption);
			if(binDir==null || libDir==null || libExecDir==null || pluginsDir==null || qmlDir==null || resourcesDir==null || osArchName==null)
				throw new Error(String.format("Qt bundle generation: Unable to find Qt. Either use --%1$s or --%2$s, or all of --%3$s, --%4$s, --%5$s, --%6$s, --%7$s, --%8$s, --%9$s, --%10$s.", 
						qtbaseOption.valueName(), 
						qtdirOption.valueName(),
						binsOption.valueName(),
						libsOption.valueName(),
						libexecsOption.valueName(),
						pluginsOption.valueName(),
						qmlOption.valueName(),
						resourcesOption.valueName(),
						translationsOption.valueName(),
						platformOption.valueName()));
			if(isMinGWBuilt==null) {
				isMinGWBuilt = binDir.getParentFile().getName().startsWith("mingw");
			}
			generateBundles(targetDir, binDir, libDir, pluginsDir, qmlDir, libExecDir, resourcesDir, translationsDir, osArchName, androidBindingsURL, isDebug, isForceDebugInfo, isMinGWBuilt, version);
		}else {
			if(args.length==1)
				parser.showHelp();
			throw new Error(String.format("Qt bundle generation: Unable to find Qt. Either use --%1$s or --%2$s, or all of --%3$s, --%4$s, --%5$s, --%6$s, --%7$s, --%8$s, --%9$s, --%10$s.", 
					qtbaseOption.valueName(), 
					qtdirOption.valueName(),
					binsOption.valueName(),
					libsOption.valueName(),
					libexecsOption.valueName(),
					pluginsOption.valueName(),
					qmlOption.valueName(),
					resourcesOption.valueName(),
					translationsOption.valueName(),
					platformOption.valueName()));
		}
	}
	
	private static String findTargetModule(String name) {
		if(name.equals("xcbqpa") || name.startsWith("eglfs")) {
			return "gui";
		}else if(name.startsWith("ffmpegstub")) {
			return "multimedia";
		}else if(name.equals("designercomponents")) {
			return "designer";
		}else if(name.equals("waylandeglclienthwintegration") || name.equals("wlshellintegration")) {
			return "waylandclient";
		}else if(name.equals("waylandeglcompositorhwintegration")) {
			return "waylandcompositor";
		}else if(name.startsWith("quickcontrols2")
				|| name.startsWith("quickdialogs2")
				|| name.startsWith("quicklayout")
				|| name.startsWith("quickparticles")
				|| name.startsWith("quickshapes")
				|| name.startsWith("quicktemplates2")
				|| name.startsWith("quicktimeline")) {
			return "quick";
		}else if(name.startsWith("labs")) {
			return "qml";
		}else if(name.startsWith("qml") && !name.equals("qml")) {
			return "qml";
		}else if(name.startsWith("webenginequick") && !name.equals("webenginequick")) {
			return "webenginequick";
		}else if(name.startsWith("protobuf") && !name.startsWith("protobufquick")) {
			return "protobuf";
		}else if(name.startsWith("quick3d") && !name.equals("quick3d")) {
			return "quick3d";
		}
		return null;
	}
	
	private static void generateBundles(File targetDir, File qtdir, String osArchName, URL androidBindingsURL, boolean isDebug, boolean isForceDebugInfo, boolean isMinGWBuilt, QVersionNumber version) {
		File binDir = new File(qtdir, "bin");
		File libDir = new File(qtdir, "lib");
		File pluginsDir = new File(qtdir, "plugins");
		File qmlDir = new File(qtdir, "qml");
		File libexecDir = new File(qtdir, "libexec");
		File resourcesDir = new File(qtdir, "resources");
		File translationsDir = new File(qtdir, "translations");
		generateBundles(targetDir, binDir, libDir, pluginsDir, qmlDir, libexecDir, resourcesDir, translationsDir, osArchName, androidBindingsURL, isDebug, isForceDebugInfo, isMinGWBuilt, version);
	}
	
	private static void generateBundles(File targetDir, 
										File binDir,
										File libDir,
										File pluginsDir,
										File qmlDir,
										File libexecDir, 
										File resourcesDir,
										File translationsDir,
										String osArchName, 
										URL androidBindingsURL, 
										boolean isDebug,
										boolean isForceDebugInfo, 
										boolean isMinGWBuilt, 
										QVersionNumber version) {
		if(System.getProperty("os.name").toLowerCase().startsWith("windows")) {
			File qmake = new File(binDir, "qmake.exe");
			if(!qmake.exists())
				qmake = new File(binDir, "qmake.bat");
			if(qmake.exists()) {
				QProcess process = new QProcess();
				process.start(qmake.getAbsolutePath(), QStringList.of("-query", "QT_VERSION"));
				process.waitForFinished();
				if(process.exitCode()==0) {
					version = QVersionNumber.fromString(process.readAllStandardOutput().trimmed().toString());
				}
			}
		}else {
			File qmake = new File(binDir, "qmake");
			if(qmake.exists()) {
				QProcess process = new QProcess();
				process.start(qmake.getAbsolutePath(), QStringList.of("-query", "QT_VERSION"));
				process.waitForFinished();
				if(process.exitCode()==0) {
					version = QVersionNumber.fromString(process.readAllStandardOutput().trimmed().toString());
				}
			}
		}
		if(version!=null) {
			Map<String,List<File>> libraries = new TreeMap<>();
			Map<String,File> plugins = new TreeMap<>();
			Map<String,File> qmllibs = new TreeMap<>();
			if(osArchName.startsWith("windows-")) {
				boolean isMingw = osArchName.startsWith("windows-mingw");
				boolean isLlvmMingw = osArchName.startsWith("windows-llvm-mingw");
				String prefix = "Qt" + version.majorVersion();
				if(isDebug) {
					if(isMingw || isLlvmMingw) {
						for(String lib : binDir.list()) {
							if(lib.startsWith(prefix) && lib.endsWith(".dll")) {
								String name = lib.substring(prefix.length(), lib.length()-4).toLowerCase();
								if(name.startsWith("3d"))
									name = "qt"+name;
								boolean prepend = true;
								String targetModule = findTargetModule(name);
								if(targetModule!=null) {
									name = targetModule;
									prepend = false;
								}
								List<File> list = libraries.computeIfAbsent(name, Utility.arrayListFactory());
								if(prepend)
									list.add(0, new File(binDir, lib));
								else
									list.add(new File(binDir, lib));
								File debugLib = new File(libDir, lib+".debug");
								if(debugLib.exists())
									list.add(debugLib);
							}else if(lib.equals("libGLESv2.dll") || lib.equals("libEGL.dll") || lib.equals("opengl32sw.dll")) {
								List<File> list = libraries.computeIfAbsent("gui", Utility.arrayListFactory());
								list.add(new File(binDir, lib));
							}else if(isMingw && (lib.equals("libstdc++-6.dll") || lib.equals("libwinpthread-1.dll") || lib.equals("libgcc_s_seh-1.dll"))) {
								List<File> list = libraries.computeIfAbsent("core", Utility.arrayListFactory());
								list.add(new File(binDir, lib));
							}else if(isLlvmMingw && (lib.equals("libc++.dll") || lib.equals("libunwind.dll"))) {
								List<File> list = libraries.computeIfAbsent("core", Utility.arrayListFactory());
								list.add(new File(binDir, lib));
							}else if((lib.startsWith("av") || lib.startsWith("sw")) && lib.endsWith(".dll")) {
								List<File> list = libraries.computeIfAbsent("multimedia", Utility.arrayListFactory());
								list.add(new File(binDir, lib));
							}
						}
						for(File pluginDir : pluginsDir.listFiles()) {
							if(pluginDir.isDirectory()) {
								boolean found = false;
								for(String lib : pluginDir.list()) {
									if(lib.endsWith(".dll")) {
										String libNoSuffix = lib.substring(0, lib.length()-5);
										if(new File(pluginDir, libNoSuffix+".dll").exists())
											continue;
										found = true;
										break;
									}
								}
								if(found)
									plugins.put(pluginDir.getName(), pluginDir);
							}
						}
					}else {
						for(String lib : binDir.list()) {
							if(lib.startsWith(prefix) && lib.endsWith("d.dll")) {
								String libNoSuffix = lib.substring(0, lib.length()-5);
								if(new File(binDir, libNoSuffix+"dd.dll").exists())
									continue;
								String name = lib.substring(prefix.length(), lib.length()-5).toLowerCase();
								if(name.startsWith("3d"))
									name = "qt"+name;
								boolean prepend = true;
								String targetModule = findTargetModule(name);
								if(targetModule!=null) {
									name = targetModule;
									prepend = false;
								}
								List<File> list = libraries.computeIfAbsent(name, Utility.arrayListFactory());
								if(prepend)
									list.add(0, new File(binDir, lib));
								else
									list.add(new File(binDir, lib));
							}else if((lib.startsWith("libGLESv2") || lib.startsWith("libEGL")) && lib.endsWith("d.dll")) {
								String libNoSuffix = lib.substring(0, lib.length()-5);
								if(new File(binDir, libNoSuffix+"dd.dll").exists())
									continue;
								List<File> list = libraries.computeIfAbsent("gui", Utility.arrayListFactory());
								list.add(new File(binDir, lib));
							}else if((lib.startsWith("av") || lib.startsWith("sw")) && lib.endsWith(".dll")) {
								List<File> list = libraries.computeIfAbsent("multimedia", Utility.arrayListFactory());
								list.add(new File(binDir, lib));
							}
						}
						for(File pluginDir : pluginsDir.listFiles()) {
							if(pluginDir.isDirectory()) {
								boolean found = false;
								for(String lib : pluginDir.list()) {
									if(lib.endsWith("d.dll")) {
										String libNoSuffix = lib.substring(0, lib.length()-5);
										if(new File(pluginDir, libNoSuffix+"dd.dll").exists())
											continue;
										found = true;
										break;
									}
								}
								if(found)
									plugins.put(pluginDir.getName(), pluginDir);
							}
						}
					}
				}else {
					for(String lib : binDir.list()) {
						if(lib.startsWith(prefix) && lib.endsWith(".dll")) {
							if(lib.endsWith("d.dll")) {
								String libNoSuffix = lib.substring(0, lib.length()-5);
								if(new File(binDir, libNoSuffix+".dll").exists())
									continue;
							}
							String name = lib.substring(prefix.length(), lib.length()-4).toLowerCase();
							if(name.startsWith("3d"))
								name = "qt"+name;
							boolean prepend = true;
							String targetModule = findTargetModule(name);
							if(targetModule!=null) {
								name = targetModule;
								prepend = false;
							}
							List<File> list = libraries.computeIfAbsent(name, Utility.arrayListFactory());
							if(prepend)
								list.add(0, new File(binDir, lib));
							else
								list.add(new File(binDir, lib));
						}else if((lib.startsWith("libGLESv2") || lib.startsWith("libEGL")) && lib.endsWith("d.dll")) {
							String libNoSuffix = lib.substring(0, lib.length()-5);
							if(new File(binDir, libNoSuffix+".dll").exists())
								continue;
							List<File> list = libraries.computeIfAbsent("gui", Utility.arrayListFactory());
							list.add(new File(binDir, lib));
						}else if(lib.equals("opengl32sw.dll")) {
							List<File> list = libraries.computeIfAbsent("gui", Utility.arrayListFactory());
							list.add(new File(binDir, lib));
						}else if(isMingw && (lib.equals("libstdc++-6.dll") || lib.equals("libwinpthread-1.dll") || lib.equals("libgcc_s_seh-1.dll"))) {
							List<File> list = libraries.computeIfAbsent("core", Utility.arrayListFactory());
							list.add(new File(binDir, lib));
						}else if(isLlvmMingw && (lib.equals("libc++.dll") || lib.equals("libunwind.dll"))) {
							List<File> list = libraries.computeIfAbsent("core", Utility.arrayListFactory());
							list.add(new File(binDir, lib));
						}else if((lib.startsWith("av") || lib.startsWith("sw")) && lib.endsWith(".dll")) {
							List<File> list = libraries.computeIfAbsent("multimedia", Utility.arrayListFactory());
							list.add(new File(binDir, lib));
						}
					}
					for(File pluginDir : pluginsDir.listFiles()) {
						if(pluginDir.isDirectory()) {
							boolean found = false;
							for(String lib : pluginDir.list()) {
								if(lib.endsWith(".dll")) {
									String libNoSuffix = lib.substring(0, lib.length()-5);
									if(new File(pluginDir, libNoSuffix+".dll").exists())
										continue;
									found = true;
									break;
								}
							}
							if(found)
								plugins.put(pluginDir.getName(), pluginDir);
						}
					}
				}
			}else if(osArchName.startsWith("android-")) {
				if(androidBindingsURL==null) {
					throw new Error("QtAndroidBindings.jar required. Please specify --utilities or --utilities-location.");
				}
				isDebug = false;
				String prefix = "libQt" + version.majorVersion();
				String suffix;
				switch(osArchName) {
				case "android-arm64":
					suffix = "_arm64-v8a.so";
					break;
				case "android-arm":
					suffix = "_armeabi-v7a.so";
					break;
				case "android-x64":
					suffix = "_x86_64.so";
					break;
				default:
					suffix = "_x86.so";
					break;
				}
				for(String lib : libDir.list()) {
					if(lib.startsWith(prefix) && lib.endsWith(suffix)) {
						String name = lib.substring(prefix.length(), lib.length() - suffix.length()).toLowerCase();
						if(name.startsWith("3d"))
							name = "qt"+name;
						boolean prepend = true;
						String targetModule = findTargetModule(name);
						if(targetModule!=null) {
							name = targetModule;
							prepend = false;
						}
						List<File> list = libraries.computeIfAbsent(name, Utility.arrayListFactory());
						if(prepend)
							list.add(0, new File(libDir, lib));
						else
							list.add(new File(libDir, lib));
					}else if((lib.startsWith("libav") || lib.startsWith("libsw")) && lib.endsWith(".so")) {
						List<File> list = libraries.computeIfAbsent("multimedia", Utility.arrayListFactory());
						list.add(new File(libDir, lib));
					}
				}
				for(File pluginDir : pluginsDir.listFiles()) {
					if(pluginDir.isDirectory()) {
						boolean found = false;
						for(String lib : pluginDir.list()) {
							if(lib.startsWith("lib") && lib.endsWith(suffix)) {
								found = true;
								break;
							}
						}
						if(found)
							plugins.put(pluginDir.getName(), pluginDir);
					}
				}
			}else if(osArchName.startsWith("linux-") || osArchName.startsWith("solaris-") || osArchName.contains("bsd-")) {
				isDebug = false;
				String prefix = "libQt" + version.majorVersion();
				String suffix = ".so."+version;
				String suffix2 = ".so."+version.majorVersion()+"."+version.minorVersion();
				String prefix2 = "libicu";
				for(String lib : libDir.list()) {
					if(lib.startsWith(prefix) && lib.endsWith(suffix)) {
						String name = lib.substring(prefix.length(), lib.length() - suffix.length()).toLowerCase();
						if(name.startsWith("3d"))
							name = "qt"+name;
						boolean prepend = true;
						String targetModule = findTargetModule(name);
						if(targetModule!=null) {
							name = targetModule;
							prepend = false;
						}
						List<File> list = libraries.computeIfAbsent(name, Utility.arrayListFactory());
						if(prepend)
							list.add(0, new File(libDir, lib));
						else
							list.add(new File(libDir, lib));
					}else if(lib.startsWith(prefix) && lib.endsWith(suffix2)) {
						String name = lib.substring(prefix.length(), lib.length() - suffix2.length()).toLowerCase();
						if(name.startsWith("3d"))
							name = "qt"+name;
						boolean prepend = true;
						String targetModule = findTargetModule(name);
						if(targetModule!=null) {
							name = targetModule;
							prepend = false;
						}
						Path link = new File(libDir, lib).toPath();
						try {
							if(Files.isSymbolicLink(link)) {
								File libFile = link.toRealPath().toFile();
								if(libFile.getName().startsWith(prefix) && !libFile.getName().endsWith(suffix)) {
									List<File> list = libraries.computeIfAbsent(name, Utility.arrayListFactory());
									if(prepend)
										list.add(0, libFile);
									else
										list.add(libFile);									
								}
							}
						} catch (IOException e) {
						}
					}else if(lib.startsWith(prefix2) && lib.contains(".so.") && !Files.isSymbolicLink(new File(libDir, lib).toPath())) {
						libraries.computeIfAbsent("core", Utility.arrayListFactory()).add(new File(libDir, lib));
					}else if((lib.startsWith("libav") || lib.startsWith("libsw")) && (lib.endsWith(".so") || lib.contains(".so."))) {
						List<File> list = libraries.computeIfAbsent("multimedia", Utility.arrayListFactory());
						list.add(new File(libDir, lib));
					}
				}
				for(File pluginDir : pluginsDir.listFiles()) {
					if(pluginDir.isDirectory()) {
						boolean found = false;
						for(String lib : pluginDir.list()) {
							if(lib.startsWith("lib") && lib.endsWith(".so")) {
								found = true;
								break;
							}
						}
						if(found)
							plugins.put(pluginDir.getName(), pluginDir);
					}
				}
			}else if(osArchName.equals("macos")) {
				isDebug = false;
				String prefix = "Qt";
				String suffix = ".framework";
				for(File lib : libDir.listFiles()) {
					if(lib.isDirectory() && lib.getName().startsWith(prefix) && lib.getName().endsWith(suffix)) {
						String name = lib.getName().substring(prefix.length(), lib.getName().length() - suffix.length()).toLowerCase();
						if(name.startsWith("3d"))
							name = "qt"+name;
						boolean prepend = true;
						String targetModule = findTargetModule(name);
						if(targetModule!=null) {
							name = targetModule;
							prepend = false;
						}
						List<File> list = libraries.computeIfAbsent(name, Utility.arrayListFactory());
						if(prepend)
							list.add(0, lib);
						else
							list.add(lib);
					}else if((lib.getName().startsWith("libav") || lib.getName().startsWith("libsw")) && lib.getName().endsWith(".dylib")) {
						List<File> list = libraries.computeIfAbsent("multimedia", Utility.arrayListFactory());
						list.add(lib);
					}
				}
				for(File pluginDir : pluginsDir.listFiles()) {
					if(pluginDir.isDirectory()) {
						boolean found = false;
						for(String lib : pluginDir.list()) {
							if(lib.startsWith("lib") && lib.endsWith(".dylib")) {
								found = true;
								break;
							}
						}
						if(found)
							plugins.put(pluginDir.getName(), pluginDir);
					}
				}
			}else if(osArchName.equals("ios")) {
				return;
			}
			for(File qmlsubDir : qmlDir.listFiles()) {
				if(qmlsubDir.isDirectory())
					switch(qmlsubDir.getName()) {
					case "QtAudioEngine":
						qmllibs.put("QtMultimedia", qmlsubDir);
						break;
					case "QtQuick":
						qmllibs.put(qmlsubDir.getName(), qmlsubDir);
						for(File quicklib : qmlsubDir.listFiles()) {
							if(quicklib.isDirectory()) {
								qmllibs.put("QtQuick/"+quicklib.getName(), quicklib);
							}
						}
						break;
					case "QtWayland":
						for(File quicklib : qmlsubDir.listFiles()) {
							if(quicklib.isDirectory()) {
								qmllibs.put("QtWayland/"+quicklib.getName(), quicklib);
							}
						}
						break;
					case "Qt3D":
						qmllibs.put(qmlsubDir.getName(), qmlsubDir);
						for(File quicklib : qmlsubDir.listFiles()) {
							if(quicklib.isDirectory()) {
								qmllibs.put("Qt3D/"+quicklib.getName(), quicklib);
							}
						}
						break;
						default:
							qmllibs.put(qmlsubDir.getName(), qmlsubDir);
							break;
					}
			}
			
			try {
				@SuppressWarnings("serial")
				class HasManifestException extends IOException{}
				
				class UnEmptyJarOutputStream extends JarOutputStream{
					private final File utilFile;
					private final Manifest manifest;
					public UnEmptyJarOutputStream(File utilFile, Manifest manifest) throws IOException {
						super(new FileOutputStream(utilFile));
						this.utilFile = utilFile;
						this.manifest = manifest;
					}
					
					boolean hasManifest = false;
					int entries;
					@Override
					public void putNextEntry(ZipEntry ze) throws IOException {
						if(JarFile.MANIFEST_NAME.equals(ze.getName())) {
							if(hasManifest)
								throw new HasManifestException();
							hasManifest = true;
							super.putNextEntry(ze);
						}else if(ze.getName().startsWith("META-INF/") || ze.getName().endsWith(".class")){
							super.putNextEntry(ze);
						}else if(ze.getName().startsWith("qml/")){
							//super.putNextEntry(new JarEntry("assets/qrc/qt-project.org/imports/"+ze.getName().substring(4)));
							super.putNextEntry(new JarEntry("qt-project.org/imports/"+ze.getName().substring(4)));
						}else {
//							super.putNextEntry(new JarEntry("assets/qrc/"+ze.getName()));
							super.putNextEntry(new JarEntry(ze.getName()));
						}
						++entries;
					}

					@Override
					public void close() throws IOException {
						if(!hasManifest && entries>0) {
							super.putNextEntry(new ZipEntry(JarFile.MANIFEST_NAME));
							manifest.write(this);
							super.closeEntry();
						}
						super.close();
						if(entries==0)
							utilFile.delete();
					}
				}
				
				Transformer transformer = TransformerFactory.newInstance().newTransformer();
				transformer.setOutputProperty(OutputKeys.INDENT, "yes");
				transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
				transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
				DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
				factory.setValidating(false);
				DocumentBuilder builder = factory.newDocumentBuilder();
				Manifest manifest = new Manifest();
				manifest.getMainAttributes().putValue("Bundle-Version", version.toString());
				
				for (Map.Entry<String, List<File>> libPair : libraries.entrySet()) {
					File libBundleFile = new File(targetDir, "qt-lib-"+libPair.getKey()+"-native-"+osArchName+(isDebug ? "-debug-" : "-")+version+".jar");
					File libDebugInfoFile = new File(targetDir, "qt-lib-"+libPair.getKey()+"-debuginfo-"+osArchName+(isDebug ? "-debug-" : "-")+version+".jar");
					@SuppressWarnings("resource")
					AutoCloseable closable = ()->{};
					JarOutputStream jarUtilFile = null;
					if(osArchName.startsWith("android")) {
						File utilFile = new File(targetDir, "qt-lib-"+libPair.getKey()+"-native-android-common-"+version+".jar");
						jarUtilFile = new UnEmptyJarOutputStream(utilFile, manifest);
						closable = jarUtilFile;
						if("core".equals(libPair.getKey())) {
							try(JarInputStream is = new JarInputStream(androidBindingsURL.openStream())){
								JarEntry entry = is.getNextJarEntry();
								while(entry!=null) {
									if(!entry.isDirectory() && !entry.getName().equals(JarFile.MANIFEST_NAME)) {
										jarUtilFile.putNextEntry(entry);
										byte[] buffer = new byte[1024];
										int length = is.read(buffer, 0, buffer.length);
										while(length>0) {
											jarUtilFile.write(buffer, 0, length);
											length = is.read(buffer, 0, buffer.length);
										}
										jarUtilFile.closeEntry();
									}
									is.closeEntry();
									entry = is.getNextJarEntry();
								}
							}
						}
					}
					boolean isEmpty = false;
					boolean isDEmpty = false;
					try(JarOutputStream libBundleJarFile = new JarOutputStream(new FileOutputStream(libBundleFile));
						JarOutputStream libDebugInfoJarFile = new JarOutputStream(new FileOutputStream(libDebugInfoFile));
							AutoCloseable _closable = closable){
						if(jarUtilFile==null) {
							jarUtilFile = libBundleJarFile;
						}
						Document libDoc = builder.getDOMImplementation().createDocument(
								null,
								"qtjambi-deploy",
								null
							);
						libDoc.getDocumentElement().setAttribute("module", "qt.lib."+libPair.getKey().replace('-', '.').replace('/', '.'));
						libDoc.getDocumentElement().setAttribute("system", osArchName);
						libDoc.getDocumentElement().setAttribute("version", version.toString());
						libDoc.getDocumentElement().setAttribute("configuration", isDebug ? "debug" : "release");
						Document debugInfoDoc = builder.getDOMImplementation().createDocument(
								null,
								"qtjambi-deploy",
								null
							);
						debugInfoDoc.getDocumentElement().setAttribute("module", "qt.lib."+libPair.getKey().replace('-', '.').replace('/', '.'));
						debugInfoDoc.getDocumentElement().setAttribute("system", osArchName);
						debugInfoDoc.getDocumentElement().setAttribute("version", version.toString());
						debugInfoDoc.getDocumentElement().setAttribute("configuration", "debuginfo");
						
						Document debugDoc = isDebug ? libDoc : debugInfoDoc;
						JarOutputStream debugJarFile = isDebug ? jarUtilFile : libDebugInfoJarFile;
						
						for(File libraryFile : libPair.getValue()) {
							if(osArchName.startsWith("macos")) {
								String libName = libraryFile.getName();
								libName = libName.substring(0, libName.length() - ".framework".length());
								File versionDir = new File(new File(libraryFile, "Versions"), version.majorVersion()==5 ? "5" : "A");
								File libFile = new File(versionDir, libName);
								if(libFile.isFile()) {
									if(isDebug || isForceDebugInfo) {
										File debugSym = new File(libraryFile.getParentFile(), libName+".framework.dSYM");
										if(debugSym.exists() && debugSym.isFile()) {
											debugJarFile.putNextEntry(new ZipEntry(libraryFile.getParentFile().getName()+"/"+libName+".framework.dSYM"));
											Files.copy(debugSym.toPath(), debugJarFile);
											debugJarFile.closeEntry();
											Element libraryElement = libDoc.createElement("file");
											libraryElement.setAttribute("name", libraryFile.getParentFile().getName()+"/"+libName+".framework.dSYM");
											debugDoc.getDocumentElement().appendChild(libraryElement);									
										}else if(debugSym.isDirectory()) {
											copyDirectory(debugSym, libDebugInfoJarFile, libDebugInfoJarFile, libDebugInfoJarFile, libraryFile.getParentFile().getName(), osArchName, isDebug, isForceDebugInfo, debugInfoDoc, debugInfoDoc);
										}
									}
									String libFilePath = libraryFile.getName()+(version.majorVersion()==5 ? "/Versions/5" : "/Versions/A");
									libBundleJarFile.putNextEntry(new ZipEntry(libraryFile.getParentFile().getName()+"/"+libFilePath +"/"+ libName));
									Files.copy(libFile.toPath(), libBundleJarFile);
									libBundleJarFile.closeEntry();
									Element libraryElement = libDoc.createElement("library");
									libraryElement.setAttribute("name", libraryFile.getParentFile().getName()+"/"+libFilePath +"/"+ libName);
									libDoc.getDocumentElement().appendChild(libraryElement);
									File macResourcesDir = new File(versionDir, "Resources");
									if(macResourcesDir.isDirectory())
										copyDirectory(macResourcesDir, libBundleJarFile, libDebugInfoJarFile, jarUtilFile, libraryFile.getParentFile().getName()+"/"+libFilePath, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
									File helpersDir = new File(versionDir, "Helpers");
									if(helpersDir.isDirectory())
										copyDirectory(helpersDir, libBundleJarFile, libDebugInfoJarFile, jarUtilFile, libraryFile.getParentFile().getName()+"/"+libFilePath, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
									Element symlinkElement = libDoc.createElement("symlink");
									symlinkElement.setAttribute("name", libraryFile.getParentFile().getName()+"/"+libraryFile.getName()+"/Versions/Current");
									symlinkElement.setAttribute("target", libraryFile.getParentFile().getName()+"/"+libFilePath);
									libDoc.getDocumentElement().appendChild(symlinkElement);
									if(macResourcesDir.isDirectory()) {
										symlinkElement = libDoc.createElement("symlink");
										symlinkElement.setAttribute("name", libraryFile.getParentFile().getName()+"/"+libraryFile.getName()+"/Resources");
										symlinkElement.setAttribute("target", libraryFile.getParentFile().getName()+"/"+libraryFile.getName()+"/Versions/Current/Resources");
										libDoc.getDocumentElement().appendChild(symlinkElement);
									}
									if(helpersDir.isDirectory()) {
										symlinkElement = libDoc.createElement("symlink");
										symlinkElement.setAttribute("name", libraryFile.getParentFile().getName()+"/"+libraryFile.getName()+"/Helpers");
										symlinkElement.setAttribute("target", libraryFile.getParentFile().getName()+"/"+libraryFile.getName()+"/Versions/Current/Helpers");
										libDoc.getDocumentElement().appendChild(symlinkElement);										
									}
									symlinkElement = libDoc.createElement("symlink");
									symlinkElement.setAttribute("name", libraryFile.getParentFile().getName()+"/"+libraryFile.getName()+"/"+libName);
									symlinkElement.setAttribute("target", libraryFile.getParentFile().getName()+"/"+libraryFile.getName()+"/Versions/Current/"+libName);
									libDoc.getDocumentElement().appendChild(symlinkElement);
								}
							}else if(osArchName.startsWith("linux-") || osArchName.startsWith("solaris-") || osArchName.contains("bsd-")) {
								libBundleJarFile.putNextEntry(new ZipEntry("lib/"+libraryFile.getName()));
								Files.copy(libraryFile.toPath(), libBundleJarFile);
								libBundleJarFile.closeEntry();
								Element libraryElement = libDoc.createElement("library");
								libraryElement.setAttribute("name", "lib/"+libraryFile.getName());
								libDoc.getDocumentElement().appendChild(libraryElement);
								String libName = libraryFile.getName();
								if(libName.endsWith(".so."+version)) {
									if(isDebug || isForceDebugInfo) {
										String _libName;
										File debugSym = new File(libraryFile.getParentFile(), _libName = libName.replace(".so."+version, ".debug"));
										if(!debugSym.exists() && _libName.startsWith("lib")) {
											_libName = _libName.substring(3);
											debugSym = new File(libraryFile.getParentFile(), _libName);
										}
										if(debugSym.exists()) {
											debugJarFile.putNextEntry(new ZipEntry("lib/"+debugSym.getName()));
											Files.copy(debugSym.toPath(), debugJarFile);
											debugJarFile.closeEntry();
											libraryElement = debugDoc.createElement("file");
											libraryElement.setAttribute("name", "lib/"+debugSym.getName());
											debugDoc.getDocumentElement().appendChild(libraryElement);									
										}
									}
								}
								if(libName.contains(".so.")) {
									while(!libName.endsWith(".so")) {
										int idx = libName.lastIndexOf('.');
										libName = libName.substring(0, idx);
										Element symlinkElement = libDoc.createElement("symlink");
										symlinkElement.setAttribute("name", "lib/"+libName);
										symlinkElement.setAttribute("target", "lib/"+libraryFile.getName());
										libDoc.getDocumentElement().appendChild(symlinkElement);
									}
								}
							}else if(osArchName.startsWith("android-")) {
								String targetDirName;
								switch(osArchName) {
								case "android-arm64":
									targetDirName = "lib/arm64-v8a/";
									break;
								case "android-arm":
									targetDirName = "lib/armeabi-v7a/";
									break;
								case "android-x64":
									targetDirName = "lib/x86_64/";
									break;
								default:
									targetDirName = "lib/x86/";
									break;
								}
								libBundleJarFile.putNextEntry(new ZipEntry(targetDirName + libraryFile.getName()));
								Files.copy(libraryFile.toPath(), libBundleJarFile);
								libBundleJarFile.closeEntry();
								Element libraryElement = libDoc.createElement("library");
								libraryElement.setAttribute("name", targetDirName + libraryFile.getName());
								libDoc.getDocumentElement().appendChild(libraryElement);
								String libName = libraryFile.getName();
								if(libName.startsWith("libQt") && libName.endsWith(".so")) {
									File dependencies = new File(libraryFile.getParentFile(), libName.substring(3, libName.length()-3)+"-android-dependencies.xml");
									if(dependencies.exists()) {
										Document depDoc = builder.parse(dependencies);
										Element rulesEl = depDoc.getDocumentElement();
										Set<String> resources = new HashSet<>();
										if(rulesEl.getNodeName().equals("rules")) {
											for(int i=0, length=rulesEl.getChildNodes().getLength(); i<length; ++i) {
												if(rulesEl.getChildNodes().item(i) instanceof Element) {
													Element dependenciesEl = (Element)rulesEl.getChildNodes().item(i);
													if(dependenciesEl.getNodeName().equals("dependencies")) {
														for(int i2=0, length2=dependenciesEl.getChildNodes().getLength(); i2<length2; ++i2) {
															if(dependenciesEl.getChildNodes().item(i2) instanceof Element) {
																Element libEl = (Element)dependenciesEl.getChildNodes().item(i2);
																if(libEl.getNodeName().equals("lib") && libName.substring(3, libName.length()-3).equals(libEl.getAttribute("name"))) {
																	for(int i3=0, length3=libEl.getChildNodes().getLength(); i3<length3; ++i3) {
																		if(libEl.getChildNodes().item(i3) instanceof Element) {
																			Element dependsEl = (Element)libEl.getChildNodes().item(i3);
																			if(dependsEl.getNodeName().equals("depends")) {
																				for(int i4=0, length4=dependsEl.getChildNodes().getLength(); i4<length4; ++i4) {
																					if(dependsEl.getChildNodes().item(i4) instanceof Element) {
																						Element childEl = (Element)dependsEl.getChildNodes().item(i4);
																						switch(childEl.getNodeName()) {
																						case "jar":{
																							File jarResource = new File(libDir.getParentFile(), childEl.getAttribute("file"));
																							if(jarResource.exists() && !resources.contains(childEl.getAttribute("file"))) {
																								resources.add(childEl.getAttribute("file"));
																								try(JarFile jarResourceFile = new JarFile(jarResource)){
																									Enumeration<JarEntry> entries = jarResourceFile.entries();
																									while(entries.hasMoreElements()) {
																										JarEntry entry = entries.nextElement();
																										if(entry.isDirectory())
																											continue;
																										try {
																											jarUtilFile.putNextEntry(entry);
																										}catch(HasManifestException h) {
																											continue;
																										}catch(Throwable e) {
																											e.printStackTrace();
																											continue;
																										}
																										try(InputStream is = jarResourceFile.getInputStream(entry)){
																											byte[] buffer = new byte[1024];
																											int l = is.read(buffer);
																											while(l>0){
																												jarUtilFile.write(buffer, 0, l);
																												l = is.read(buffer);
																											}
																										}
																										jarUtilFile.closeEntry();
																									}
																								}
																							}
																						}
																						break;
																						case "lib":{
																							File libResource = new File(libDir.getParentFile(), childEl.getAttribute("file"));
																							if(libResource.exists() && pluginsDir.equals(libResource.getParentFile().getParentFile())) {
																								String plugin = libResource.getParentFile().getName();
																								File pluginFile = plugins.remove(plugin);
																								if(pluginFile!=null) {
																									copyDirectory(pluginFile, libBundleJarFile, libDebugInfoJarFile, jarUtilFile, "plugins", osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
																								}
																							}
																						}
																						break;
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}else {
								libBundleJarFile.putNextEntry(new ZipEntry(libraryFile.getParentFile().getName()+"/"+libraryFile.getName()));
								Files.copy(libraryFile.toPath(), libBundleJarFile);
								libBundleJarFile.closeEntry();
								Element libraryElement = libDoc.createElement("library");
								libraryElement.setAttribute("name", libraryFile.getParentFile().getName()+"/"+libraryFile.getName());
								libDoc.getDocumentElement().appendChild(libraryElement);
								
								if(isDebug || isForceDebugInfo) {
									File pdb = new File(libraryFile.getParentFile(), libraryFile.getName().substring(0, libraryFile.getName().length()-3)+"pdb");
									if(pdb.exists()) {
										debugJarFile.putNextEntry(new ZipEntry(pdb.getParentFile().getName()+"/"+pdb.getName()));
										Files.copy(pdb.toPath(), debugJarFile);
										debugJarFile.closeEntry();
										libraryElement = debugDoc.createElement("file");
										libraryElement.setAttribute("name", pdb.getParentFile().getName()+"/"+pdb.getName());
										debugDoc.getDocumentElement().appendChild(libraryElement);
									}else if(isDebug){
										pdb = new File(libraryFile.getParentFile(), libraryFile.getName()+".debug");
										if(pdb.exists()) {
											debugJarFile.putNextEntry(new ZipEntry(pdb.getParentFile().getName()+"/"+pdb.getName()));
											Files.copy(pdb.toPath(), debugJarFile);
											debugJarFile.closeEntry();
											libraryElement = debugDoc.createElement("file");
											libraryElement.setAttribute("name", pdb.getParentFile().getName()+"/"+pdb.getName());
											debugDoc.getDocumentElement().appendChild(libraryElement);
										}
								}
							}
							}
							
							switch(libPair.getKey()) {
							case "webenginecore": {
									if(resourcesDir.isDirectory()) {
										copyDirectory(resourcesDir, libBundleJarFile, libDebugInfoJarFile, jarUtilFile, "", osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
									}
									File qtwebengine_locales;
									if(translationsDir.isDirectory() && (qtwebengine_locales = new File(translationsDir, "qtwebengine_locales")).isDirectory()) {
										copyDirectory(qtwebengine_locales, libBundleJarFile, libDebugInfoJarFile, jarUtilFile, "translations/", osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
									}
									File exe = null;
									String path = null;
									if(osArchName.startsWith("windows-")) {
										path = "bin";
										exe = new File(binDir, isDebug ? "QtWebEngineProcessd.exe" : "QtWebEngineProcess.exe");
									}else if(osArchName.startsWith("linux-") || osArchName.startsWith("solaris-") || osArchName.contains("bsd-")) {
										path = "libexec";
										exe = new File(libexecDir, "QtWebEngineProcess");
									}
									if(exe!=null && exe.exists()) {
										libBundleJarFile.putNextEntry(new ZipEntry(path+"/"+exe.getName()));
										Files.copy(exe.toPath(), libBundleJarFile);
										libBundleJarFile.closeEntry();
										Element libraryElement = libDoc.createElement("file");
										libraryElement.setAttribute("name", path+"/"+exe.getName());
										if(exe.canExecute())
											libraryElement.setAttribute("executable", "true");
										libDoc.getDocumentElement().appendChild(libraryElement);									
									}
								}
								break;
							case "svg": {
									for(String subdir : Arrays.asList("imageformats", "iconengines")) {
										for(File pluginFile : new File(pluginsDir, subdir).listFiles()) {
											if(pluginFile.getName().contains("qsvg")) {
												if(pluginFile.isFile()) {
													String targetDirName = "plugins/" + subdir + "/";
													if(osArchName.startsWith("windows-")) {
														if(pluginFile.getName().endsWith(".pdb")) {
															if(isDebug) {
																if(pluginFile.getName().endsWith("d.pdb")) {
																	String libNoSuffix = pluginFile.getName().substring(0, pluginFile.getName().length()-5);
																	if(new File(pluginFile.getParentFile(), libNoSuffix+"dd.pdb").exists())
																		continue;
																}else continue;
															}else if(isForceDebugInfo) {
																if(pluginFile.getName().endsWith("d.pdb")) {
																	String libNoSuffix = pluginFile.getName().substring(0, pluginFile.getName().length()-5);
																	if(new File(pluginFile.getParentFile(), libNoSuffix+".pdb").exists())
																		continue;
																}else continue;
															}else {
																continue;
															}
														}
														if(pluginFile.getName().endsWith(".dll.debug") && !isDebug)
															continue;
														if(isDebug && !pluginFile.getName().endsWith("d.dll"))
															continue;
														if(!isDebug && pluginFile.getName().endsWith("d.dll"))
															continue;
													}else if(osArchName.startsWith("android-")) {
														String suffix;
														switch(osArchName) {
														case "android-arm64":
															suffix = "_arm64-v8a.so";
															targetDirName = "lib/arm64-v8a/";
															break;
														case "android-arm":
															suffix = "_armeabi-v7a.so";
															targetDirName = "lib/armeabi-v7a/";
															break;
														case "android-x64":
															suffix = "_x86_64.so";
															targetDirName = "lib/x86_64/";
															break;
														default:
															suffix = "_x86.so";
															targetDirName = "lib/x86/";
															break;
														}
														if(!pluginFile.getName().endsWith(suffix))
															continue;
													}
													libBundleJarFile.putNextEntry(new ZipEntry(targetDirName + pluginFile.getName()));
													Files.copy(pluginFile.toPath(), libBundleJarFile);
													libBundleJarFile.closeEntry();
													Element libraryElement = libDoc.createElement("library");
													libraryElement.setAttribute("name", targetDirName + pluginFile.getName());
													libDoc.getDocumentElement().appendChild(libraryElement);
												}else if(pluginFile.getName().endsWith("dSYM") && isDebug) {
													copyDirectory(pluginFile, libBundleJarFile, libDebugInfoJarFile, jarUtilFile, "plugins/"+subdir, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
												}
											}
										}
									}
								}
								break;
							case "virtualkeyboard": {
								for(String subdir : Arrays.asList("platforminputcontexts")) {
									for(File pluginFile : new File(pluginsDir, subdir).listFiles()) {
										if(pluginFile.getName().contains("qtvirtualkeyboard")) {
											if(pluginFile.isFile()) {
												String targetDirName = "plugins/" + subdir + "/";
												if(osArchName.startsWith("windows-")) {
													if(pluginFile.getName().endsWith(".pdb")) {
														if(isDebug) {
															if(pluginFile.getName().endsWith("d.pdb")) {
																String libNoSuffix = pluginFile.getName().substring(0, pluginFile.getName().length()-5);
																if(new File(pluginFile.getParentFile(), libNoSuffix+"dd.pdb").exists())
																	continue;
															}else continue;
														}else if(isForceDebugInfo) {
															if(pluginFile.getName().endsWith("d.pdb")) {
																String libNoSuffix = pluginFile.getName().substring(0, pluginFile.getName().length()-5);
																if(new File(pluginFile.getParentFile(), libNoSuffix+".pdb").exists())
																	continue;
															}else continue;
														}else {
															continue;
														}
													}
													if(pluginFile.getName().endsWith(".dll.debug") && !isDebug)
														continue;
													if(isDebug && !pluginFile.getName().endsWith("d.dll"))
														continue;
													if(!isDebug && pluginFile.getName().endsWith("d.dll"))
														continue;
												}else if(osArchName.startsWith("android-")) {
													String suffix;
													switch(osArchName) {
													case "android-arm64":
														suffix = "_arm64-v8a.so";
														targetDirName = "lib/arm64-v8a/";
														break;
													case "android-arm":
														suffix = "_armeabi-v7a.so";
														targetDirName = "lib/armeabi-v7a/";
														break;
													case "android-x64":
														suffix = "_x86_64.so";
														targetDirName = "lib/x86_64/";
														break;
													default:
														suffix = "_x86.so";
														targetDirName = "lib/x86/";
														break;
													}
													if(!pluginFile.getName().endsWith(suffix))
														continue;
												}
												libBundleJarFile.putNextEntry(new ZipEntry(targetDirName + pluginFile.getName()));
												Files.copy(pluginFile.toPath(), libBundleJarFile);
												libBundleJarFile.closeEntry();
												Element libraryElement = libDoc.createElement("library");
												libraryElement.setAttribute("name", targetDirName + pluginFile.getName());
												libDoc.getDocumentElement().appendChild(libraryElement);
											}else if(pluginFile.getName().endsWith("dSYM") && isDebug) {
												copyDirectory(pluginFile, libBundleJarFile, libDebugInfoJarFile, jarUtilFile, "plugins/"+subdir, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
											}
										}
									}
								}
							}
							break;
							}
						}
						
						Set<String> associatedPlugins = pluginsByModules(osArchName).get(libPair.getKey());
						if(associatedPlugins!=null) {
							for(String plugin : associatedPlugins) {
								File pluginFile = plugins.remove(plugin);
								if(pluginFile!=null && pluginFile.isDirectory()) {
									copyDirectory(pluginFile, libBundleJarFile, libDebugInfoJarFile, jarUtilFile, "plugins", osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
								}
							}
						}
						if("pdfquick".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Collections.singletonList("QtQuick/Pdf"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						}else if("waylandclient".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Collections.singletonList("QtWayland/Client"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						}else if("waylandcompositor".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Collections.singletonList("QtWayland/Compositor"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						}else if("virtualkeyboard".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Collections.singletonList("QtQuick/VirtualKeyboard"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						}else if("quickeffects".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Collections.singletonList("QtQuickEffectMaker"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						}else if("qml".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Arrays.asList("QtQuick/LocalStorage",
									"Assets",
									"QtQml",
									"QmlTime",
									"QtQuick/Window",
									"QtQuick/tooling"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
							if(version.majorVersion()==5) {
								copyQmlPaths(qmllibs, Collections.singletonList("QtQuick/LocalStorage"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
							}
						}else if("qt3dcore".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Arrays.asList("QtQuick/Scene2D", "QtQuick/Scene3D"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						}else if("quick".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Arrays.asList(
									"QtQuick/NativeStyle",
									"QtQuick/Particles",
									"Qt",
									"QtQuick",
									"QtCore",
									"QtQuick/Shapes",
									"QtQuick/Templates",
									"QtQuick/Controls",
									"QtQuick/Dialogs",
									"QtQuick/Layouts",
									"QtQuick/Timeline"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
							if(version.majorVersion()==5) {
								copyQmlPaths(qmllibs, Arrays.asList("QtGraphicalEffects",
										"QtQuick/Window.2",
										"QtQuick/Templates.2",
										"QtQuick/Controls.2",
										"QtQuick/Particles.2",
										"QtQuick.2",
										"QtQuick/Extras",
										"QtQuick/PrivateWidgets"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
							}
						}else if("webenginequick".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Collections.singletonList("QtWebEngine"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						}else if("xmlpatterns".equals(libPair.getKey())) {
							copyQmlPaths(qmllibs, Collections.singletonList("QtQuick/XmlListModel"), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						}
						if(!"core".equals(libPair.getKey())) {
							for(String key : new ArrayList<>(qmllibs.keySet())) {
								String libName = key.replace('/', '-').replace(".", "").toLowerCase();
								if(libName.startsWith("qt3d-")) {
									switch(libName) {
									case "qt3d-core":
										libName = "qt3dquick";
										break;
									case "qt3d-logic":
										libName = "qt3dlogic";
										break;
									default: 
										libName = "qt3dquick"+libName.substring(5);
										break;
									}
								}
								if(libName.startsWith("qtquick-")) {
									libName = "quick"+libName.substring(8); 
								}
								if(libName.startsWith("qt")) {
									String _libName = libName.substring(2);
									if(!_libName.isEmpty() && Character.isJavaIdentifierStart(_libName.charAt(0)))
										libName = _libName;
								}
								if(libPair.getKey().equals("core5compat") && libName.equals("qt5compat"))
									libName = libPair.getKey();
								else if(libPair.getKey().equals(libName+"2"))
									libName = libPair.getKey();
								else if(libPair.getKey().endsWith("qml")) {
									String nonQmlLib = libPair.getKey().substring(0, libPair.getKey().length()-3);
									if(libraries.containsKey(nonQmlLib) && libName.equals(nonQmlLib)) {
										libName = libPair.getKey();
									}
								}
								else if(libPair.getKey().endsWith("quick")) {
									String nonQmlLib = libPair.getKey().substring(0, libPair.getKey().length()-5);
									if(libraries.containsKey(nonQmlLib) && libName.equals(nonQmlLib)) {
										libName = libPair.getKey();
									}
								}
								if(libName.equals(libPair.getKey())) {
									if(!libraries.containsKey(libPair.getKey()+"qml")
											&& !libraries.containsKey(libPair.getKey()+"quick")) {
										copyQmlPaths(qmllibs, Collections.singletonList(key), libBundleJarFile, jarUtilFile, libDebugInfoJarFile, osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
									}
								}
							}
						}
						{
							List<File> list = libraries.get(libPair.getKey()+"qml");
							if(list!=null && !list.isEmpty()) {
								Element libraryElement = libDoc.createElement("qmllib");
								if(osArchName.startsWith("macos")) {
									String libName = list.get(0).getName();
									libName = libName.substring(0, libName.length() - ".framework".length());
									libraryElement.setAttribute("name", list.get(0).getName()+"/Versions/"+(version.majorVersion()==5 ? "5/" : "A/")+libName);
								}else {
									libraryElement.setAttribute("name", list.get(0).getParentFile().getName()+"/"+list.get(0).getName());
								}
								libDoc.getDocumentElement().appendChild(libraryElement);																	
							}else {
								if("webenginecore".equals(libPair.getKey())) {
									if(version.majorVersion()==5) {
										list = libraries.get("webengine");
									}else {
										list = libraries.get("webenginequick");
									}
								}else {
									list = libraries.get(libPair.getKey()+"quick");
								}
								if(list!=null && !list.isEmpty()) {
									Element libraryElement = libDoc.createElement("qmllib");
									if(osArchName.startsWith("macos")) {
										String libName = list.get(0).getName();
										libName = libName.substring(0, libName.length() - ".framework".length());
										libraryElement.setAttribute("name", list.get(0).getName()+"/Versions/"+(version.majorVersion()==5 ? "5/" : "A/")+libName);
									}else {
										libraryElement.setAttribute("name", list.get(0).getParentFile().getName()+"/"+list.get(0).getName());
									}
									libDoc.getDocumentElement().appendChild(libraryElement);																	
								}
							}
						}
						
						libBundleJarFile.putNextEntry(new ZipEntry(JarFile.MANIFEST_NAME));
						manifest.write(libBundleJarFile);
						libBundleJarFile.closeEntry();
						libDebugInfoJarFile.putNextEntry(new ZipEntry(JarFile.MANIFEST_NAME));
						manifest.write(libDebugInfoJarFile);
						libDebugInfoJarFile.closeEntry();
						
						isEmpty = !libDoc.getDocumentElement().hasChildNodes();
						isDEmpty = !debugInfoDoc.getDocumentElement().hasChildNodes();
						if(!osArchName.startsWith("android-")) {
							libBundleJarFile.putNextEntry(new ZipEntry("META-INF/qtjambi-deployment.xml"));
							transformer.transform(new DOMSource(libDoc), new StreamResult(libBundleJarFile));
							libBundleJarFile.closeEntry();
							libDebugInfoJarFile.putNextEntry(new ZipEntry("META-INF/qtjambi-deployment.xml"));
							transformer.transform(new DOMSource(debugInfoDoc), new StreamResult(libDebugInfoJarFile));
							libDebugInfoJarFile.closeEntry();
						}
					}
					if(isDEmpty || isDebug || osArchName.startsWith("android") || osArchName.equals("ios"))
						libDebugInfoFile.delete();
					if(isEmpty)
						libBundleFile.delete();
				}
				
				for (Map.Entry<String, File> libPair : plugins.entrySet()) {
					File libBundleFile = new File(targetDir, "qt-plugin-"+libPair.getKey().replace("/", "-")+"-native-"+osArchName+(isDebug ? "-debug-" : "-")+version+".jar");
					File libDebugInfoFile = new File(targetDir, "qt-plugin-"+libPair.getKey().replace("/", "-")+"-debuginfo-"+osArchName+(isDebug ? "-debug-" : "-")+version+".jar");
					@SuppressWarnings("resource")
					AutoCloseable closable = ()->{};
					JarOutputStream jarUtilFile = null;
					if(osArchName.startsWith("android")) {
						File utilFile = new File(targetDir, "qt-plugin-"+libPair.getKey().replace("/", "-")+"-native-android-common-"+version+".jar");
						if(!utilFile.exists()) {
							jarUtilFile = new UnEmptyJarOutputStream(utilFile, manifest);
						}else {
							jarUtilFile = new JarOutputStream(new ByteArrayOutputStream());
						}
						closable = jarUtilFile;
					}
					boolean isEmpty = false;
					boolean isDEmpty = false;
					try(JarOutputStream jarFile = new JarOutputStream(new FileOutputStream(libBundleFile));
							JarOutputStream libDebugInfoJarFile = new JarOutputStream(new FileOutputStream(libDebugInfoFile));
							AutoCloseable _closable = closable){
						if(jarUtilFile==null) {
							jarUtilFile = jarFile;
						}
						Document libDoc = builder.getDOMImplementation().createDocument(
								null,
								"qtjambi-deploy",
								null
							);
						libDoc.getDocumentElement().setAttribute("module", "qt.plugin."+libPair.getKey().replace('-', '.').replace('/', '.'));
						libDoc.getDocumentElement().setAttribute("system", osArchName);
						libDoc.getDocumentElement().setAttribute("version", version.toString());
						libDoc.getDocumentElement().setAttribute("configuration", isDebug ? "debug" : "release");
						Document debugInfoDoc = builder.getDOMImplementation().createDocument(
								null,
								"qtjambi-deploy",
								null
							);
						debugInfoDoc.getDocumentElement().setAttribute("module", "qt.plugin."+libPair.getKey().replace('-', '.').replace('/', '.'));
						debugInfoDoc.getDocumentElement().setAttribute("system", osArchName);
						debugInfoDoc.getDocumentElement().setAttribute("version", version.toString());
						debugInfoDoc.getDocumentElement().setAttribute("configuration", "debuginfo");
						copyDirectory(libPair.getValue(), jarFile, libDebugInfoJarFile, jarUtilFile, "plugins", osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						jarFile.putNextEntry(new ZipEntry(JarFile.MANIFEST_NAME));
						manifest.write(jarFile);
						jarFile.closeEntry();
						
						isEmpty = !libDoc.getDocumentElement().hasChildNodes();
						isDEmpty = !debugInfoDoc.getDocumentElement().hasChildNodes();
						if(!osArchName.startsWith("android-")) {
							jarFile.putNextEntry(new ZipEntry("META-INF/qtjambi-deployment.xml"));
							transformer.transform(new DOMSource(libDoc), new StreamResult(jarFile));
							jarFile.closeEntry();
							libDebugInfoJarFile.putNextEntry(new ZipEntry("META-INF/qtjambi-deployment.xml"));
							transformer.transform(new DOMSource(debugInfoDoc), new StreamResult(libDebugInfoJarFile));
							libDebugInfoJarFile.closeEntry();
						}
					}
					if(isEmpty)
						libBundleFile.delete();
					if(isDEmpty || isDebug || osArchName.startsWith("android") || osArchName.equals("ios"))
						libDebugInfoFile.delete();
				}
				
				for (Map.Entry<String, File> libPair : qmllibs.entrySet()) {
					String libName = libPair.getKey().replace('/', '-').replace(".", "").toLowerCase();
					if(libName.startsWith("qt")) {
						String _libName = libName.substring(2);
						if(!_libName.isEmpty() && Character.isJavaIdentifierStart(_libName.charAt(0)))
							libName = _libName;
					}
					File libBundleFile = new File(targetDir, "qt-qml-"+libName+"-native-"+osArchName+(isDebug ? "-debug-" : "-")+version+".jar");
					File libDebugInfoFile = new File(targetDir, "qt-qml-"+libName+"-debuginfo-"+osArchName+(isDebug ? "-debug-" : "-")+version+".jar");
					boolean isEmpty = false;
					boolean isDEmpty = false;
					@SuppressWarnings("resource")
					AutoCloseable closable = ()->{};
					JarOutputStream jarUtilFile = null;
					if(osArchName.startsWith("android")) {
						File utilFile = new File(targetDir, "qt-qml-"+libName+"-native-android-common-"+version+".jar");
						if(!utilFile.exists()) {
							jarUtilFile = new UnEmptyJarOutputStream(utilFile, manifest);
						}else {
							jarUtilFile = new JarOutputStream(new ByteArrayOutputStream());
						}
						closable = jarUtilFile;
					}
					try(JarOutputStream jarFile = new JarOutputStream(new FileOutputStream(libBundleFile));
							JarOutputStream libDebugInfoJarFile = new JarOutputStream(new FileOutputStream(libDebugInfoFile));
							AutoCloseable _closable = closable){
						if(jarUtilFile==null) {
							jarUtilFile = jarFile;
						}
						Document libDoc = builder.getDOMImplementation().createDocument(
								null,
								"qtjambi-deploy",
								null
							);
						libDoc.getDocumentElement().setAttribute("module", "qt.qml."+libName.replace('-', '.').replace('/', '.'));
						libDoc.getDocumentElement().setAttribute("system", osArchName);
						libDoc.getDocumentElement().setAttribute("version", version.toString());
						libDoc.getDocumentElement().setAttribute("configuration", isDebug ? "debug" : "release");
						Document debugInfoDoc = builder.getDOMImplementation().createDocument(
								null,
								"qtjambi-deploy",
								null
							);
						debugInfoDoc.getDocumentElement().setAttribute("module", "qt.qml."+libName.replace('-', '.').replace('/', '.'));
						debugInfoDoc.getDocumentElement().setAttribute("system", osArchName);
						debugInfoDoc.getDocumentElement().setAttribute("version", version.toString());
						debugInfoDoc.getDocumentElement().setAttribute("configuration", "debuginfo");
						List<String> path = new ArrayList<>();
						path.add("qml");
						path.addAll(Arrays.asList(libPair.getKey().split("/")));
						path.remove(path.size()-1);
						copyDirectory(libPair.getValue(), jarFile, libDebugInfoJarFile, jarUtilFile, String.join("/", path), osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
						jarFile.putNextEntry(new ZipEntry(JarFile.MANIFEST_NAME));
						manifest.write(jarFile);
						jarFile.closeEntry();
						
						isEmpty = !libDoc.getDocumentElement().hasChildNodes();
						isDEmpty = !debugInfoDoc.getDocumentElement().hasChildNodes();
						if(!osArchName.startsWith("android-")) {
							jarFile.putNextEntry(new ZipEntry("META-INF/qtjambi-deployment.xml"));
							transformer.transform(new DOMSource(libDoc), new StreamResult(jarFile));
							jarFile.closeEntry();
							libDebugInfoJarFile.putNextEntry(new ZipEntry("META-INF/qtjambi-deployment.xml"));
							transformer.transform(new DOMSource(debugInfoDoc), new StreamResult(libDebugInfoJarFile));
							libDebugInfoJarFile.closeEntry();
						}
					}
					if(isEmpty)
						libBundleFile.delete();
					if(isDEmpty || isDebug || osArchName.startsWith("android") || osArchName.equals("ios"))
						libDebugInfoFile.delete();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	private static void copyQmlPaths(Map<String,File> qmllibs, Iterable<String> paths, JarOutputStream jarFile, JarOutputStream jarUtilFile, JarOutputStream libDebugInfoJarFile, String osArchName, boolean isDebug, boolean isForceDebugInfo, Document libDoc, Document debugInfoDoc) throws IOException {
		for (String path : paths) {
			File qmlLib = qmllibs.remove(path);
			if(qmlLib!=null && qmlLib.isDirectory()) {
				List<String> _path = new ArrayList<>();
				_path.add("qml");
				_path.addAll(Arrays.asList(path.split("/")));
				_path.remove(_path.size()-1);
				copyDirectory(qmlLib, jarFile, libDebugInfoJarFile, jarUtilFile, String.join("/", _path), osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
			}
		}
	}
	
	private static void copyDirectory(File dir, JarOutputStream jarFile, JarOutputStream libDebugInfoJarFile, JarOutputStream jarUtilFile, String path, String osArchName, boolean isDebug, boolean isForceDebugInfo, Document libDoc, Document debugInfoDoc) throws IOException {
		if(osArchName.equals("macos") && !(isDebug || isForceDebugInfo) && dir.getName().endsWith(".dSYM"))
			return;
		if(path==null)
			path = "";
		if(!path.isEmpty() && !path.endsWith("/")) {
			path += "/";
		}
		boolean noSubdirs = "qml/".equals(path) && ("QtQuick".equals(dir.getName()) || "Qt3D".equals(dir.getName()));
		if("qml/".equals(path) && "QtQml".equals(dir.getName())) {
			File qmltypesFile = new File(dir.getParentFile(), "builtins.qmltypes");
			if(qmltypesFile.exists()) {
				jarUtilFile.putNextEntry(new ZipEntry(path + qmltypesFile.getName()));
				Files.copy(qmltypesFile.toPath(), jarUtilFile);
				jarUtilFile.closeEntry();
				Element element = libDoc.createElement("file");
				element.setAttribute("name", path + dir.getName() + "/" + qmltypesFile.getName());
				libDoc.getDocumentElement().appendChild(element);									
			}
			qmltypesFile = new File(dir.getParentFile(), "jsroot.qmltypes");
			if(qmltypesFile.exists()) {
				jarUtilFile.putNextEntry(new ZipEntry(path + qmltypesFile.getName()));
				Files.copy(qmltypesFile.toPath(), jarUtilFile);
				jarUtilFile.closeEntry();
				Element element = libDoc.createElement("file");
				element.setAttribute("name", path + dir.getName() + "/" + qmltypesFile.getName());
				libDoc.getDocumentElement().appendChild(element);									
			}
		}
		for(File file : dir.listFiles()) {
			JarOutputStream targetJarFile = jarUtilFile;
			Document targetDoc = libDoc;
			if(file.isDirectory()) {
				if(noSubdirs)
					continue;
				if(osArchName.equals("macos") && !(isDebug || isForceDebugInfo) && file.getName().endsWith(".dSYM"))
					continue;
				if(file.getName().endsWith(".dSYM"))
					copyDirectory(file, libDebugInfoJarFile, libDebugInfoJarFile, libDebugInfoJarFile, path + dir.getName() + "/", osArchName, isDebug, isForceDebugInfo, debugInfoDoc, debugInfoDoc);
				else
					copyDirectory(file, jarFile, libDebugInfoJarFile, jarUtilFile, path + dir.getName() + "/", osArchName, isDebug, isForceDebugInfo, libDoc, debugInfoDoc);
			}else {
				if(file.getName().contains("qsvg") || file.getName().contains("qtvirtualkeyboardplugin")) {
					continue;
				}
				boolean isLibrary = false;
				String targetDirName = path + dir.getName() + "/";
				if(osArchName.startsWith("windows-")) {
					if(file.getName().endsWith(".dll.debug")) {
						if(isForceDebugInfo) {
							targetJarFile = libDebugInfoJarFile;
							targetDoc = debugInfoDoc;
						}else if(!isDebug) {
							continue;
						}
					}
					if(file.getName().endsWith(".pdb")) {
						if(isDebug) {
							if(file.getName().endsWith("d.pdb")) {
								String libNoSuffix = file.getName().substring(0, file.getName().length()-5);
								if(new File(file.getParentFile(), libNoSuffix+"dd.pdb").exists())
									continue;
							}else continue;
						}else if(isForceDebugInfo) {
							if(file.getName().endsWith("d.pdb")) {
								String libNoSuffix = file.getName().substring(0, file.getName().length()-5);
								if(new File(file.getParentFile(), libNoSuffix+".pdb").exists())
									continue;
							}
							targetJarFile = libDebugInfoJarFile;
							targetDoc = debugInfoDoc;
						}else {
							continue;
						}
					}
					if(file.getName().endsWith(".dll")) {
						isLibrary = true;
						if(isDebug) {
							if(file.getName().endsWith("d.dll")) {
								String libNoSuffix = file.getName().substring(0, file.getName().length()-5);
								if(new File(dir, libNoSuffix+"dd.dll").exists())
									continue;
							}else continue;
						}else {
							if(file.getName().endsWith("d.dll")) {
								String libNoSuffix = file.getName().substring(0, file.getName().length()-5);
								if(new File(dir, libNoSuffix+".dll").exists())
									continue;
							}
						}
					}
				}else if(osArchName.startsWith("android-")) {
					String suffix;
					isLibrary = file.getName().endsWith(".so");
					switch(osArchName) {
					case "android-arm64":
						suffix = "_arm64-v8a.so";
						if(isLibrary)
							targetDirName = "lib/arm64-v8a/";
						break;
					case "android-arm":
						suffix = "_armeabi-v7a.so";
						if(isLibrary)
							targetDirName = "lib/armeabi-v7a/";
						break;
					case "android-x64":
						suffix = "_x86_64.so";
						if(isLibrary)
							targetDirName = "lib/x86_64/";
						break;
					default:
						suffix = "_x86.so";
						if(isLibrary)
							targetDirName = "lib/x86/";
						break;
					}
					if(isLibrary && !file.getName().endsWith(suffix))
						continue;
				}else if(osArchName.startsWith("linux-") || osArchName.startsWith("solaris-") || osArchName.contains("bsd-")) {
					isLibrary = file.getName().endsWith(".so");
					if(file.getName().endsWith(".debug")) {
						if(isForceDebugInfo) {
							targetJarFile = libDebugInfoJarFile;
							targetDoc = debugInfoDoc;
						}else if(!isDebug) {
							continue;
						}
					}
				}else if(osArchName.equals("macos")) {
					isLibrary = file.getName().endsWith(".dylib");
					if(file.getName().endsWith(".dSYM")) {
						if(isForceDebugInfo) {
							targetJarFile = libDebugInfoJarFile;
							targetDoc = debugInfoDoc;
						}else if(!isDebug) {
							continue;
						}
					}
				}
				if(file.getName().startsWith("v8_context_snapshot") && file.getName().endsWith(".bin")) {
					if(file.getName().endsWith(".debug.bin") != isDebug)
						continue;
				}
				Element element;
				if(isLibrary) {
					element = libDoc.createElement("library");
					element.setAttribute("name", targetDirName + file.getName());
					jarFile.putNextEntry(new ZipEntry(targetDirName + file.getName()));
					Files.copy(file.toPath(), jarFile);
					jarFile.closeEntry();
					libDoc.getDocumentElement().appendChild(element);									
				}else {
					element = targetDoc.createElement("file");
					element.setAttribute("name", targetDirName + file.getName());
					if(osArchName.equals("macos") && file.getName().equals("QtWebEngineProcess"))
						element.setAttribute("executable", "true");
					targetJarFile.putNextEntry(new ZipEntry(targetDirName + file.getName()));
					Files.copy(file.toPath(), targetJarFile);
					targetJarFile.closeEntry();
					targetDoc.getDocumentElement().appendChild(element);									
				}
			}
		}
	}
}

final class Utility extends io.qt.internal.NativeUtility{
	protected static <K,V> Function<K, ArrayList<V>> arrayListFactory(){
		return io.qt.internal.NativeUtility.arrayListFactory();
	}
}

final class DeployerUtility extends io.qt.internal.DeployerUtility{
	protected static Map<String,QPair<Long,Long>> getRegisteredTypeSizesAndAlignments(){
		return io.qt.internal.DeployerUtility.getRegisteredTypeSizesAndAlignments();
	}
	protected static String getInterfaceIID(Class<?> cls) {
		return io.qt.internal.DeployerUtility.getInterfaceIID(cls);
	}
	protected static File qtLibraryPath() {
		return io.qt.internal.DeployerUtility.qtLibraryPath();
	}
	protected static String osArchName() {
		return io.qt.internal.DeployerUtility.osArchName();
	}
	protected static boolean isMinGWBuilt() {
		return io.qt.internal.DeployerUtility.isMinGWBuilt();
	}
}