package io.qt.qtjambi.plugindeployer;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.AbstractMap.SimpleEntry;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import io.qt.core.QByteArray;
import io.qt.core.QCborMap;
import io.qt.core.QCborValue;
import io.qt.core.QCoreApplication;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QJsonDocument;
import io.qt.core.QJsonObject;
import io.qt.core.QJsonParseError.ParseError;
import io.qt.internal.QtJambiInternal;

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
	
	private static final PluginClassLoader pluginClassLoader = new PluginClassLoader();

	public static void main(String[] args) throws MalformedURLException {
		QCoreApplication.initialize("QtJambiPluginDeployer", args);
		String iid = null;
		String className = null;
		String pluginName = null;
		QDir dir = null;
		QJsonObject metaData = null;
		List<String> classPath = new ArrayList<>();
		List<Map.Entry<String,String>> libraries = new ArrayList<>();
		if(args.length==0) {
			System.err.println("No arguments given.");
			showInfo();
			System.exit(-1);
		}
		for (int i = 0; i < args.length; i++) {
			if(args[i].startsWith("--help") 
					|| args[i].startsWith("-h")
					|| args[i].startsWith("-?")) {
				showInfo();
				return;
			}else if(args[i].startsWith("--iid=")) {
				iid = args[i].substring(6);
			}else if(args[i].startsWith("--dir=")) {
				dir = new QDir(QDir.fromNativeSeparators(args[i].substring(6)));
			}else if(args[i].startsWith("--class-name=")) {
				className = args[i].substring(13);
			}else if(args[i].startsWith("--plugin-name=")) {
				pluginName = args[i].substring(14);
			}else if(args[i].startsWith("--class-path=")) {
				Collections.addAll(classPath, args[i].substring(13).split(File.pathSeparator));
			}else if(args[i].startsWith("--plugin-library=")) {
				String[] libinfo = args[i].substring(19).split(File.pathSeparator);
				if(libinfo.length==2){
					if(!new File(libinfo[1]).isFile()) {
						System.err.println("Specified qtjambiplugin library does not exist: "+libinfo[1]);
						System.exit(-1);
					}
					libraries.add(new SimpleEntry<>(libinfo[0], libinfo[1]));
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
						if(!new File(libinfo[0]).isFile()) {
							System.err.println("Specified qtjambiplugin library does not exist: "+libinfo[0]);
							System.exit(-1);							
						}
						libraries.add(new SimpleEntry<>(os, libinfo[0]));
					}else {
						System.err.println("Unable to determine platform for library "+libinfo[0]+". Please use --plugin-library=<platform>"+File.pathSeparatorChar+"<path to library>");
						System.exit(-1);
					}
				}
			}else if(args[i].startsWith("--plugin-library-location=")) {
				QDir location = new QDir(args[i].substring(26));
				for(String entry : location.entryList(QDir.Filter.Files)) {
					String os = null;
					if(entry.endsWith(".dll")) {
						os = "windows";
					}else if(entry.endsWith(".dylib")) {
						os = "macos";
					}else if(entry.endsWith(".so")) {
						os = "linux";
					}
					if(os!=null) {
						libraries.add(new SimpleEntry<>(os, location.absoluteFilePath(entry)));
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
					QFile file = new QFile(_metaData);
					if(file.exists() && file.open(QIODevice.OpenModeFlag.ReadOnly)) {
						data = file.readAll();
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
				System.err.println("Unknown arguments "+args[i]);
				showInfo();
				System.exit(-1);
			}
		}
		if(className==null || className.isEmpty()) {
			System.err.println("Missing class name. Please use --class-name=...");
			System.exit(-1);
		}
		if(dir==null) {
			System.err.println("Missing target directory. Please use --dir=...");
			System.exit(-1);
		}
		if(classPath.isEmpty()) {
			System.err.println("Missing classpath. Please use --class-path=...");
			System.exit(-1);
		}
		if(libraries.isEmpty()) {
			System.err.println("Missing paths to qtjambiplugin library. Please use --plugin-library=<path to library> or --plugin-library=<platform>"+File.pathSeparatorChar+"<path to library>");
			System.exit(-1);
		}
		if(pluginName==null) {
			if(classPath.size()==1) {
				QFileInfo file = new QFileInfo(QDir.fromNativeSeparators(classPath.get(0)));
				String path = file.fileName();
				if(path.endsWith(".jar")) {
					pluginName = path.substring(0, path.length()-4);
				}
			}
		}
		if(pluginName==null) {
			System.err.println("Missing plugin name. Please use --plugin-name=...");
			System.exit(-1);
		}
		if(iid==null) {
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
					System.err.println("Classpath not a file or directory: "+path);
					System.exit(-1);
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
			pluginClassLoader.addURLs(urls);
			Class<?> cls;
			try {
				cls = pluginClassLoader.loadClass(className);
			} catch (ClassNotFoundException | NoClassDefFoundError e) {
				System.err.println("Unable to find class: "+className);
				e.printStackTrace();
				System.exit(-1);
				return;
			}
			iid = QtJambiInternal.getInterfaceIID(cls);
		}
		if(iid!=null && className!=null) {
			QCborMap cborValue = new QCborMap();
			cborValue.setValue(/*QtPluginMetaDataKeys::IID*/ 2, new QCborValue(iid));
			cborValue.setValue(/*QtPluginMetaDataKeys::ClassName*/ 3, new QCborValue(className.replace(".", "::")));
			cborValue.setValue(/*QtPluginMetaDataKeys::MetaData*/ 4, new QCborValue(QCborMap.fromJsonObject(metaData)));
			cborValue.setValue(0x0_CAFEBABE_0L, new QCborValue(pluginName));
			QByteArray cborData = cborValue.toCborValue().toCbor();
			if(cborData.size()>1024) {
				System.err.println("Plugin metadata exceeds mximum size of 1024 byte.");
				System.exit(-1);
			}
			if(classPath.size()==1) {
				QFileInfo file = new QFileInfo(QDir.fromNativeSeparators(classPath.get(0)));
				String newFile = dir.absoluteFilePath(file.fileName());
				QFile.copy(file.absoluteFilePath(), newFile);
			}else {
				dir.mkdir(pluginName);
				QDir subdir = dir.clone();
				subdir.cd(pluginName);
				for (String path : classPath) {
					QFileInfo file = new QFileInfo(QDir.fromNativeSeparators(path));
					String newFile = dir.absoluteFilePath(file.fileName());
					QFile.copy(file.absoluteFilePath(), newFile);
				}
			}
			for(Map.Entry<String,String> entry : libraries) {
				String os = entry.getKey();
				if(os!=null) {
					switch(os.toLowerCase()) {
					case "win32":
					case "win64":
					case "windows":
						{
							QFile file = new QFile(QDir.fromNativeSeparators(entry.getValue()));
							QFile newFile = new QFile(dir.absoluteFilePath(pluginName + (file.fileName().endsWith("d.dll") ? "d.dll" : ".dll")));
							if(file.open(QIODevice.OpenModeFlag.ReadOnly)) {
								QByteArray libData = file.readAll();
								file.close();
								int idx = libData.indexOf("QTMETADATA !");
								if(idx>0) {
									if(newFile.open(QIODevice.OpenModeFlag.WriteOnly)) {
										newFile.write(libData);
										newFile.seek(idx+16);
										newFile.write(cborData);
										newFile.close();
									}
								}
							}
						}
						break;
					case "macos":
					case "osx":
						{
							QFile file = new QFile(QDir.fromNativeSeparators(entry.getValue()));
							QFile newFile = new QFile(dir.absoluteFilePath("lib"+pluginName + ".dylib"));
							if(file.open(QIODevice.OpenModeFlag.ReadOnly)) {
								QByteArray libData = file.readAll();
								file.close();
								int idx = libData.indexOf("QTMETADATA !");
								if(idx>0) {
									if(newFile.open(QIODevice.OpenModeFlag.WriteOnly)) {
										newFile.write(libData);
										newFile.seek(idx+16);
										newFile.write(cborData);
										newFile.close();
									}
								}
							}
						}
						break;
					case "linux":
					case "linux32":
					case "linux64":
						{
							QFile file = new QFile(QDir.fromNativeSeparators(entry.getValue()));
							QFile newFile = new QFile(dir.absoluteFilePath("lib"+pluginName + (file.fileName().endsWith("_debug.so") ? "_debug.so" : ".so")));
							if(file.open(QIODevice.OpenModeFlag.ReadOnly)) {
								QByteArray libData = file.readAll();
								file.close();
								int idx = libData.indexOf("QTMETADATA !");
								if(idx>0) {
									if(newFile.open(QIODevice.OpenModeFlag.WriteOnly)) {
										newFile.write(libData);
										newFile.seek(idx+16);
										newFile.write(cborData);
										newFile.close();
									}
								}
							}
						}
						break;
						default:
					}
				}
			}
		}
		QCoreApplication.shutdown();
	}

	private static void showInfo() {
	}

}
