package io.qt.qtjambi.appdeployer;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.AbstractMap.SimpleEntry;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.function.IntFunction;

import io.qt.QtLongEnumerator;
import io.qt.core.QByteArray;
import io.qt.core.QCborArray;
import io.qt.core.QCborMap;
import io.qt.core.QCborValue;
import io.qt.core.QCoreApplication;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;

public class Main {
	
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
	    JVMArguments
	};
	
	public static void main(String[] args) throws InterruptedException, IOException {
		QCoreApplication.initialize("QtJambiAppDeployer", args);
		{
			String appName = null;
			String mainClass = null;
			String jvmPath = null;
			QDir dir = null;
			QFile ico = null;
			List<String> classPaths = new ArrayList<>();
			List<String> modulePaths = new ArrayList<>();
			List<String> libraryPaths = new ArrayList<>();
			List<String> arguments = new ArrayList<>();
			List<Map.Entry<String,String>> executables = new ArrayList<>();
            boolean autoDetect = false;
            int minimumJVMVersion = 11;
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
						if(!new File(exeinfo[1]).isFile()) {
							System.err.println("Specified launcher executable does not exist: "+exeinfo[1]);
							System.exit(-1);
						}
						executables.add(new SimpleEntry<>(exeinfo[0], exeinfo[1]));
					}else {
						String os = null;
						if(exeinfo[0].endsWith(".exe")) {
							os = "windows";
							if(!new File(exeinfo[0]).isFile()) {
								System.err.println("Specified launcher executable does not exist: "+exeinfo[0]);
								System.exit(-1);							
							}
						}else if(exeinfo[0].endsWith(".app")) {
							if(!new File(exeinfo[0]).isDirectory()) {
								System.err.println("Specified launcher executable does not exist: "+exeinfo[0]);
								System.exit(-1);							
							}
							os = "macos";
						}else {
							if(!new File(exeinfo[0]).isFile()) {
								System.err.println("Specified launcher executable does not exist: "+exeinfo[0]);
								System.exit(-1);							
							}
							os = "linux";
						}
						if(os!=null) {
							executables.add(new SimpleEntry<>(os, exeinfo[0]));
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
                            executables.add(new SimpleEntry<>(os, location.absoluteFilePath(entry)));
                        }
					}
				}else {
					arguments.add(args[i]);
				}
			}
			if(appName==null || appName.isEmpty()) {
				System.err.println("Missing application name. Please use --application=...");
				System.exit(-1);
			}
			if(!autoDetect && (jvmPath==null || jvmPath.isEmpty())) {
				System.err.println("Missing JVM path. Please use --jvm-path=...");
				System.exit(-1);
			}
			if(dir==null) {
				System.err.println("Missing target directory. Please use --dir=...");
				System.exit(-1);
			}
			if(mainClass==null || mainClass.isEmpty()) {
				System.err.println("Missing main class. Please use --main-class=...");
				System.exit(-1);
			}
			if(executables.isEmpty()) {
				System.err.println("Missing paths to QtJambiLauncher executables. Please use --executable=<path to executable>, --executable=<platform>"+File.pathSeparatorChar+"<path to executable> or --executable-location=<path>");
				System.exit(-1);
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
			QByteArray cborData = cborValue.toCborValue().toCbor();
			if(cborData.size()>16384) {
				System.err.println("Launcher metadata exceeds maximum size of 16384 byte.");
				System.exit(-1);
			}
			System.gc();
			final QByteArray QTJAMBI_LAUNCHER = new QByteArray("QTJAMBI_LAUNCHER!");
			
			for(Map.Entry<String,String> entry : executables) {
				String os = entry.getKey();
				if(os!=null) {
                    QFile file = new QFile(QDir.fromNativeSeparators(entry.getValue()));
					QFile newFile;
                    switch(os.toLowerCase()) {
					case "win32":
					case "win64":
					case "windows":
						newFile = new QFile(dir.absoluteFilePath(appName + ".exe"));
						System.gc();
						break;
					case "macos":
					case "osx":
						String baseName = new QFileInfo(entry.getValue()).baseName();
						file = new QFile(QDir.fromNativeSeparators(entry.getValue() + "/Contents/MacOS/" + baseName));
						dir.mkpath(appName + ".app/Contents/MacOS");
						dir.mkpath(appName + ".app/Contents/Resources");
						{
							QFile infoPList = new QFile(dir.absoluteFilePath(appName + ".app/Contents/Info.plist"));
							if(infoPList.open(QIODevice.OpenModeFlag.ReadOnly)) {
								QByteArray data = infoPList.readAll();
								long idx = data.indexOf("<key>CFBundleIdentifier</key>");
								if(idx>0) {
									idx = data.indexOf("<string>", idx);
									if(idx>0) {
										idx += 8;
										long idxEnd = data.indexOf("</string>", idx);
										if(idxEnd>idx) {
											data = data.remove(idx, idxEnd-idx);
											data.insert(idx, mainClass);
										}
									}
								}
								data = data.replace(baseName, appName);
								infoPList.close();
								infoPList = new QFile(QDir.fromNativeSeparators(entry.getValue() + "/Contents/Info.plist"));
								infoPList.remove();
								if(infoPList.open(QIODevice.OpenModeFlag.WriteOnly)) {
									infoPList.write(data);
									infoPList.close();
								}else {
									QFile.copy(QDir.fromNativeSeparators(entry.getValue() + "/Contents/Info.plist"), dir.absoluteFilePath(appName + ".app/Contents/Info.plist"));
								}
							}else {
								QFile.copy(QDir.fromNativeSeparators(entry.getValue() + "/Contents/Info.plist"), dir.absoluteFilePath(appName + ".app/Contents/Info.plist"));
							}
							QFile.copy(QDir.fromNativeSeparators(entry.getValue() + "/Contents/PkgInfo"), dir.absoluteFilePath(appName + ".app/Contents/PkgInfo"));
							QFile.copy(QDir.fromNativeSeparators(entry.getValue() + "/Resources/empty.lproj"), dir.absoluteFilePath(appName + ".app/Resources/empty.lproj"));
						}
						newFile = new QFile(dir.absoluteFilePath(appName + ".app/Contents/MacOS/" + appName));
                        break;
					case "linux":
					case "linux32":
					case "linux64":
						newFile = new QFile(dir.absoluteFilePath(appName));
						break;
						default: continue;
					}
                    if(file.open(QIODevice.OpenModeFlag.ReadOnly)) {
                        QByteArray exeData = file.readAll();
                        file.close();
                        long idx = exeData.indexOf(QTJAMBI_LAUNCHER);
                        if(idx>0) {
                            if(newFile.open(QIODevice.OpenModeFlag.WriteOnly)) {
                                newFile.write(exeData);
                                newFile.seek((int)idx);
                                newFile.write(cborData);
                                switch(os.toLowerCase()) {
        						case "win32":
        						case "win64":
        						case "windows":
                                	idx = exeData.indexOf(new QByteArray("QTJAMBI_ICO!"));
                                	if(idx>254) {
                                        if(ico!=null) {
        									if(ico.open(QIODevice.OpenModeFlag.ReadOnly)) {
        										QByteArray icoData = ico.readAll();
        										if(icoData.size()>=92482) {
        			                                System.err.println("Icon size too big.");
        			                                System.exit(-1);        											
        										}
        	                                    newFile.seek((int)idx-254);
        	                                    newFile.write(icoData);
        									}
                                        }else {
    	        							try(InputStream stream = Main.class.getResourceAsStream("icon.ico")){
    	        								byte[] icoData = stream.readAllBytes();
        	                                    newFile.seek((int)idx-254);
        	                                    newFile.write(icoData);
    	        							}
                                        }
                                	}
        							break;
        						default: 
        							break;
                                }
                                newFile.close();
                            }else{
                                System.err.println("Unable to write file "+newFile.fileName());
                                System.exit(-1);
                            }
                        }
                    }else{
                        System.err.println("Unable to read file "+file.fileName());
                        System.exit(-1);
                    }
				}
			}
		}
		QCoreApplication.shutdown();
	}

	private static void showInfo() {
	}

}
