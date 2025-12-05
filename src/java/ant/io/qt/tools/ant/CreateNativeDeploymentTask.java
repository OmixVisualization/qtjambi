package io.qt.tools.ant;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.AbstractMap.SimpleEntry;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.TimeUnit;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class CreateNativeDeploymentTask extends Task {
	
	private static Boolean no_otool = null;

	public String getLibraries() {
		return libraries;
	}
	public void setLibraries(String libraries) {
		this.libraries = libraries;
	}
	
	public String getOutputDirectory() {
		return outputDirectory;
	}
	public void setOutputDirectory(String outputDirectory) {
		this.outputDirectory = outputDirectory;
	}

	public String getModule() {
		return moduleName;
	}
	public void setModule(String moduleName) {
		this.moduleName = moduleName;
	}

	public boolean isDebug() {
		return debug;
	}
	public void setDebug(boolean debug) {
		this.debug = debug;
	}

	public boolean isPlugin() {
		return plugin;
	}
	public void setPlugin(boolean plugin) {
		this.plugin = plugin;
	}

    private String msg = "";
    boolean skipNativeJar = true;
    
	@Override
    public void execute() throws BuildException {
		OSInfo osInfo = OSInfo.instance(getProject());
        getProject().log(this, msg, Project.MSG_INFO);
        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        boolean useFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_GENERATE_FRAMEWORKS));
        skipNativeJar = true;
        if(jarFile==null || jarFile.isEmpty() || !new File(jarFile).exists() || plugin) {
        	skipNativeJar = false;
        }
    	String libraryIncludes = "";
    	String sourcesIncludes = "";
    	List<String> sourcesPaths = new ArrayList<>();
    	java.io.File targetLibDir = null;
        List<String> _libraries = Collections.emptyList();
        int qtMajorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR));
        int qtMinorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MINOR));
//        int qtPatchlevelVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_PATCHLEVEL));
        int qtjambiPatchlevelVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, "qtjambi.patchversion"));
		try {
			java.io.File directory = new java.io.File(outputDirectory);
			Transformer transformer = TransformerFactory.newInstance().newTransformer();
			transformer.setOutputProperty(OutputKeys.INDENT, "yes");
			transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
			transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			factory.setValidating(false);
			DocumentBuilder builder = factory.newDocumentBuilder();
			Document doc = builder.getDOMImplementation().createDocument(
					null,
					"qtjambi-deploy",
					null
				);
			java.io.File builddir = new java.io.File(AntUtil.getPropertyAsString(propertyHelper, "qtjambi.builddir"));
			java.io.File cppsourcedir = new java.io.File(AntUtil.getPropertyAsString(propertyHelper, "native.srcdir"));
			java.io.File sourcestargetdir = new java.io.File(directory, "sources");
			java.io.File cppoutputdir = new java.io.File(new java.io.File(AntUtil.getPropertyAsString(propertyHelper, "generator.outputdir")), "cpp");
			doc.getDocumentElement().setAttribute("module", moduleName);
			doc.getDocumentElement().setAttribute("system", AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME));
			doc.getDocumentElement().setAttribute("version", String.format("%1$s.%2$s.%3$s", qtMajorVersion, qtMinorVersion, qtjambiPatchlevelVersion));
			doc.getDocumentElement().setAttribute("compiler", AntUtil.getPropertyAsString(propertyHelper, "qtjambi.compiler"));
			doc.getDocumentElement().setAttribute("configuration", debug ? "debug" : (forceDebugInfo ? "debuginfo" : "release"));
//			doc.getDocumentElement().setAttribute("date", AntUtil.getPropertyAsString(propertyHelper, "DSTAMP"));
//			doc.getDocumentElement().setAttribute("time", AntUtil.getPropertyAsString(propertyHelper, "TSTAMP"));
			String libdir;
			java.io.File libc_for_android = null;
			String abi = null;
			switch(osInfo.crossOS()) {
			case Android:
				switch(AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME)) {
	        	case "android-arm":
	        		osInfo.setCrossCompilation(OSInfo.OperationSystem.Android, OSInfo.Architecture.arm);
	        		abi = "armeabi-v7a"; 
	        		break;
	        	case "android-arm64":
	        		osInfo.setCrossCompilation(OSInfo.OperationSystem.Android, OSInfo.Architecture.arm64);
	        		abi = "arm64-v8a"; 
	        		break;
	        	case "android-x86":
	        		osInfo.setCrossCompilation(OSInfo.OperationSystem.Android, OSInfo.Architecture.x86);
	        		abi = "x86";
	        		break;
	        	case "android-x64":
	        		osInfo.setCrossCompilation(OSInfo.OperationSystem.Android, OSInfo.Architecture.x86_64);
	        		abi = "x86_64";
	        		break;
	        	case "android-riscv64":
	        		osInfo.setCrossCompilation(OSInfo.OperationSystem.Android, OSInfo.Architecture.riscv64);
	        		abi = "riscv64";
	        		break;
	        	default:break;
	        	}
				switch(moduleName) {
				case "qtjambi":
					String ndkPath = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.android.ndk");
					if(ndkPath!=null) {
						java.io.File ndkdir = new java.io.File(ndkPath);
						if(ndkdir.isDirectory()) {
							java.io.File libcdir = new java.io.File(ndkdir, "sources");
							libcdir = new java.io.File(libcdir, "cxx-stl");
							libcdir = new java.io.File(libcdir, "llvm-libc++");
							libcdir = new java.io.File(libcdir, "libs");
							libcdir = new java.io.File(libcdir, abi);
							libc_for_android = new java.io.File(libcdir, "libc++_shared.so");
							if(!libc_for_android.exists()) {
								libcdir = new java.io.File(ndkdir, "toolchains");
								libcdir = new java.io.File(libcdir, "llvm");
								libcdir = new java.io.File(libcdir, "prebuilt");
								switch(osInfo.os()) {
								case Windows:
									libcdir = new java.io.File(libcdir, "windows-x86_64");
									break;
								case MacOS:
									libcdir = new java.io.File(libcdir, "darwin-x86_64");
									break;
								case Linux:
									libcdir = new java.io.File(libcdir, "linux-x86_64");
									break;
								default: 
									break;
								}
								libcdir = new java.io.File(libcdir, "sysroot");
								libcdir = new java.io.File(libcdir, "usr");
								libcdir = new java.io.File(libcdir, "lib");
								switch(osInfo.crossArch()) {
					        	case arm:
					        		libcdir = new java.io.File(libcdir, "arm-linux-androideabi");
					        		break;
					        	case arm64:
					        		libcdir = new java.io.File(libcdir, "aarch64-linux-android");
					        		break;
					        	case x86:
					        		libcdir = new java.io.File(libcdir, "i686-linux-android");
					        		break;
					        	case x86_64:
					        		libcdir = new java.io.File(libcdir, "x86_64-linux-android");
					        		break;
					        	case riscv64:
					        		libcdir = new java.io.File(libcdir, "riscv64-linux-android");
					        		break;
					        	default: break;
					        	}
								libc_for_android = new java.io.File(libcdir, "libc++_shared.so");
							}
						}
					}
					break;
				}
				libdir = "lib";
				break;
			case Windows:
				libdir = "bin";
				break;
			default:
				libdir = "lib";
				break;
			}
			targetLibDir = new java.io.File(directory, libdir);
			switch(osInfo.crossOS()) {
			case Android:
				targetLibDir = new java.io.File(targetLibDir, abi);
				targetLibDir.mkdirs();
				if(!forceDebugInfo) {
					if(libc_for_android!=null) {
						copyFiles(libc_for_android, new java.io.File(targetLibDir, libc_for_android.getName()));
						if(!libraryIncludes.isEmpty())
							libraryIncludes += ",";
						libraryIncludes += libdir + "/" + targetLibDir.getName() + "/" + libc_for_android.getName();
					}
				}
				break;
			case MacOS:
				if(no_otool==null) {
					no_otool = false;
					try {
			        	String[] out = Exec.executeCaptureOutput(this, Arrays.asList("otool", "--version"), builddir, getProject(), osInfo, null, null, false);
			        	if(out==null || out.length<2 || out[0]==null || !out[0].contains("Apple")) {
			        		if(out==null || out.length<=2 || out[1]==null || !out[1].contains("Apple")) {
			        			no_otool = true;
			        		}
			        	}
			        } catch ( Exception e ) {
			        	no_otool = true;
			        }
				}
				break;
				default: break;
			}
	        
        	switch(moduleName) {
			case "qtjambi.deployer":
				switch(osInfo.crossOS()) {
				case Android:
	        		_libraries = Arrays.asList("QtJambiPlugin", "jarimport");
	        		break;
        		default:
        			_libraries = Arrays.asList("QtJambiLauncher", "QtJambiPlugin", "jarimport");
	        		break;
				}
        		targetLibDir = new java.io.File(directory, "utilities");
				break;
			case "containeraccess":
			{
				libdir = "plugins/containeraccess";
				targetLibDir = new java.io.File(new java.io.File(directory, "plugins"), "containeraccess");
				builddir = new java.io.File(AntUtil.getPropertyAsString(propertyHelper, "plugins.builddir"));
				java.io.File pluginDir = new java.io.File(new java.io.File(new java.io.File(builddir, debug ? "debug" : "release"), "plugins"), "containeraccess");
				_libraries = new ArrayList<>();
				if(pluginDir.isDirectory()) {
					for(String f : pluginDir.list()) {
						switch(osInfo.crossOS()) {
						case Windows:
							if(!debug && !forceDebugInfo && f.endsWith(".dll") && !f.endsWith("d.dll")) {
								_libraries.add(f);
							}else if(debug && f.endsWith("d.dll")) {
								_libraries.add(f);
							}
							break;
						case MacOS:
							if(f.endsWith(".dylib")) {
								_libraries.add(f);
							}
							break;
						case Android:
							String suffix = ".so";
							switch(osInfo.crossArch()) {
				        	case arm:
				        		targetLibDir = new java.io.File(new java.io.File(directory, "lib"), "arm32");
				        		suffix = "_armeabi-v7a.so"; break;
				        	case arm64:
				        		targetLibDir = new java.io.File(new java.io.File(directory, "lib"), "arm64");
				        		suffix = "_arm64-v8a.so"; break;
				        	case x86:
				        		targetLibDir = new java.io.File(new java.io.File(directory, "lib"), "x86");
				        		suffix = "_x86.so"; break;
				        	case x86_64:
				        		targetLibDir = new java.io.File(new java.io.File(directory, "lib"), "x86_64");
				        		suffix = "_x86_64.so"; break;
			        		default: break;
				        	}
							if(f.endsWith(suffix)) {
								_libraries.add(f);
							}
							break;
						default:
							if(osInfo.crossOS().isUnixLike()) {
								if(f.endsWith(".so")) {
									_libraries.add(f);
								}
							}
							break;
						}
					}
				}else {
					System.out.println("directory missing: "+pluginDir);
				}
				break;
			}
			default:
				if(this.libraries!=null && !this.libraries.isEmpty()) {
					_libraries = new ArrayList<>();
					for(String lib : this.libraries.split(",")) {
						_libraries.add(lib.trim());
					}
		        }
				break;
			}
        	targetLibDir.mkdirs();
        	
	        Set<String> librariesList = new HashSet<>();
			for(String name : _libraries) {
				if(librariesList.contains(name))
					continue;
				librariesList.add(name);
				String libName;
				boolean isMacBundle = false;
				String _libdir = libdir;
				String jarpath = libdir;
				switch(moduleName) {
				case "containeraccess":
					_libdir = debug ? "debug/plugins/containeraccess" : "release/plugins/containeraccess";
					jarpath = "plugins/containeraccess";
					libName = name;
					break;
				case "qtjambi.deployer":
					jarpath = "utilities";
					if("QtJambiLauncher".equals(name)) {
						_libdir = "bin";
						switch(osInfo.crossOS()) {
						case MacOS:
							libName = name + (debug ? "_debug.app" : ".app");
							isMacBundle = true;
							break;
						case Windows:
							libName = name + (debug ? "d.exe" : ".exe");
							break;
						default:
							libName = debug ? name + "_debug" : name;
							break;
						}
					}else {
						libName = formatQtJambiQmlPluginName(osInfo, name, debug, "");
					}
					break;
				default:
					if(plugin) {
						java.io.File pluginDir = new java.io.File(new java.io.File(directory, "plugins"), name);
						for(String f : pluginDir.list()) {
							if(!f.equals(".") && !f.equals("..")) {
								Element libraryElement = doc.createElement("library");
								libraryElement.setAttribute("name", "plugins/"+name+"/"+f);
								doc.getDocumentElement().appendChild(libraryElement);
							}
						}
						continue;
					}else if(osInfo.crossOS()==OSInfo.OperationSystem.MacOS && useFrameworks) {
						libName = name + (debug ? "_debug.framework" : ".framework");
						isMacBundle = true;
					}else {
						libName = formatQtJambiName(osInfo, name, debug, qtMajorVersion, qtMinorVersion, qtjambiPatchlevelVersion);
						if(osInfo.crossOS()==OSInfo.OperationSystem.Android) {
							switch(osInfo.crossArch()) {
				        	case arm:
				        		jarpath += "/armeabi-v7a"; break;
				        	case arm64:
				        		jarpath += "/arm64-v8a"; break;
				        	case x86:
				        		jarpath += "/x86"; break;
				        	case x86_64:
				        		jarpath += "/x86_64"; break;
			        		default: break;
				        	}
						}
					}
					break;
				}
				if(!forceDebugInfo) {
					if(!libraryIncludes.isEmpty())
						libraryIncludes += ",";
					libraryIncludes += jarpath + "/" + libName;
				}
				if(debug || forceDebugInfo) {
					java.io.File libsourcedir = new java.io.File(cppsourcedir, name);
					java.io.File libsourcetargetdir = new java.io.File(sourcestargetdir, name);
					List<Map.Entry<String,Boolean>> additionalFiles = new ArrayList<>();
					if(libsourcedir.isDirectory()) {
						if(!sourcesPaths.contains(cppsourcedir.getAbsolutePath()))
							sourcesPaths.add(cppsourcedir.getAbsolutePath());
						libsourcetargetdir.mkdirs();
						copySubdirs(sourcestargetdir, libsourcedir, libsourcetargetdir, false, additionalFiles, true);
					}
					libsourcedir = new java.io.File(cppoutputdir, name);
					if(libsourcedir.isDirectory()) {
						if(!sourcesPaths.contains(cppoutputdir.getAbsolutePath()))
							sourcesPaths.add(cppoutputdir.getAbsolutePath());
						libsourcetargetdir.mkdirs();
						copySubdirs(sourcestargetdir, libsourcedir, libsourcetargetdir, false, additionalFiles, true);
					}
					if(libsourcetargetdir.isDirectory()) {
						if(forceDebugInfo) {
							if(!libraryIncludes.isEmpty())
								libraryIncludes += ",";
							libraryIncludes += "sources/" + name + "/*";
							for(Map.Entry<String,Boolean> file : additionalFiles) {
								Element libraryElement = doc.createElement("file");
								libraryElement.setAttribute("name", "sources/" + name + "/" + new File(file.getKey()).getName());
								doc.getDocumentElement().appendChild(libraryElement);
					        }
						}else {
							if(!sourcesIncludes.isEmpty())
								sourcesIncludes += ",";
							sourcesIncludes += "sources/" + name+"/*";
						}
					}
				}
				java.io.File libFile = new java.io.File(new java.io.File(builddir, _libdir), libName);
				java.io.File includeDir = new java.io.File(new java.io.File(builddir, "include"), name);
				java.io.File target = new java.io.File(targetLibDir, libName);
				if(isMacBundle) {
					List<Map.Entry<String,Boolean>> additionalFiles = new ArrayList<>();
					List<Map.Entry<String,String>> symlinks = new ArrayList<>();
					if(!debug && forceDebugInfo) {
						libName += ".dSYM";
						libFile = new java.io.File(new java.io.File(builddir, _libdir), libName);
						target = new java.io.File(targetLibDir, libName);
					}
					if(debug && "QtJambiLauncher".equals(name) && !new java.io.File(builddir, _libdir+"/QtJambiLauncher_debug.app/Contents/Info.plist").exists()) {
						Files.copy(new java.io.File(builddir, _libdir+"/QtJambiLauncher.app/Contents/Info.plist").toPath(), 
								new java.io.File(builddir, _libdir+"/QtJambiLauncher_debug.app/Contents/Info.plist").toPath(), StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
					}
					copySubdirs(libFile.getParentFile(), libFile, target, debug, additionalFiles, true);
					symlinkSubdirs(libFile.getParentFile(), libFile, target, debug, symlinks);
					String mainName = "/" + (debug ? name + "_debug" : name);
					for(Map.Entry<String,Boolean> file : additionalFiles) {
						Element libraryElement;
						if(!debug && forceDebugInfo) {
							libraryElement = doc.createElement("file");
						}else if("QtJambiLauncher".equals(name)) {
							libraryElement = doc.createElement("file");
							if(Boolean.TRUE.equals(file.getValue())) {
								libraryElement.setAttribute("executable", "true");
							}
						}else {
							if(file.getKey().endsWith(mainName)) {
								libraryElement = doc.createElement("library");
							}else {
								libraryElement = doc.createElement("file");
							}
							if(Boolean.TRUE.equals(file.getValue())) {
								libraryElement.setAttribute("executable", "true");
							}
						}
						libraryElement.setAttribute("name", jarpath+"/"+file.getKey());
						doc.getDocumentElement().appendChild(libraryElement);
						libraryIncludes += "," + jarpath+"/"+file.getKey();
			        }
			        
			        for(Map.Entry<String,String> unpack : symlinks) {
						Element libraryElement = doc.createElement("symlink");
						libraryElement.setAttribute("name", jarpath+"/"+unpack.getKey());
						libraryElement.setAttribute("target", jarpath+"/"+unpack.getValue());
						doc.getDocumentElement().appendChild(libraryElement);
			        }
				}else if(!libFile.isFile()) {
					libraryIncludes = "";
					continue;
				}else {
//					System.out.println("Copying "+libFile+" to "+target+".");
					copyFiles(libFile, target);
					if(includeDir.isDirectory() && !forceDebugInfo) {
						java.io.File incDir = new java.io.File(new java.io.File(directory, "include"), name);
						incDir.mkdirs();
						for(File header : includeDir.listFiles()) {
							if(!Files.isSymbolicLink(header.toPath())){
								copyFiles(header, new java.io.File(incDir, header.getName()));
								Element libraryElement = doc.createElement("file");
								libraryElement.setAttribute("name", "include/"+name+"/"+header.getName());
								doc.getDocumentElement().appendChild(libraryElement);
								libraryIncludes += "," + "include/"+name+"/"+header.getName();
							}
						}
					}
					switch(osInfo.crossOS()) {
					case Windows:
						if((libName.endsWith(".dll") || libName.endsWith(".exe")) && (debug || forceDebugInfo)){
							String dbgName = libName.substring(0, libName.length()-3)+"pdb";
							java.io.File dbgFile = new java.io.File(new java.io.File(builddir, _libdir), dbgName);
							if(!dbgFile.exists() && _libdir.equals("bin")) {
								dbgFile = new java.io.File(new java.io.File(builddir, "lib"), dbgName);
							}
							if(dbgFile.exists()) {
								copyFiles(dbgFile, new java.io.File(targetLibDir, dbgName));
								if(!libraryIncludes.isEmpty())
									libraryIncludes += ",";
								libraryIncludes += jarpath + "/" + dbgName;
								Element libraryElement = doc.createElement("file");
								libraryElement.setAttribute("name", jarpath + "/" + dbgName);
								doc.getDocumentElement().appendChild(libraryElement);
							}else{
								dbgFile = new java.io.File(new java.io.File(builddir, _libdir), libName+".debug");
								if(dbgFile.exists()) {
									copyFiles(dbgFile, new java.io.File(targetLibDir, libName+".debug"));
									if(!libraryIncludes.isEmpty())
										libraryIncludes += ",";
									libraryIncludes += jarpath + "/" + libName+".debug";
									Element libraryElement = doc.createElement("file");
									libraryElement.setAttribute("name", jarpath + "/" + libName+".debug");
									doc.getDocumentElement().appendChild(libraryElement);
								}
							}
						}
						switch(moduleName) {
						case "qtjambi.generator":
						{
							java.io.File qtjambiLibFile = new java.io.File(new java.io.File(builddir, "lib"), (debug ? "QtJambid" : "QtJambi")+qtMajorVersion+".lib");
							if(qtjambiLibFile.exists()) {
								java.io.File targetLibdir = new java.io.File(directory, "lib");
								targetLibdir.mkdirs();
								copyFiles(qtjambiLibFile, new java.io.File(targetLibdir, qtjambiLibFile.getName()));
								if(!libraryIncludes.isEmpty())
									libraryIncludes += ",";
								libraryIncludes += "lib/" + qtjambiLibFile.getName();
								Element libraryElement = doc.createElement("file");
								libraryElement.setAttribute("name", "lib/"+qtjambiLibFile.getName());
								doc.getDocumentElement().appendChild(libraryElement);
							}
						}
							break;
							default:
								break;
						}
						break;
					case MacOS:
						if(debug || forceDebugInfo){
							String dbgName = libName+".dSYM";
							java.io.File dbgFile = new java.io.File(new java.io.File(builddir, _libdir), dbgName);
							if(dbgFile.exists()) {
								if(dbgFile.isDirectory()) {
									List<Map.Entry<String,Boolean>> additionalFiles = new ArrayList<>();
									List<Map.Entry<String,String>> symlinks = new ArrayList<>();
									java.io.File dbgTarget = new java.io.File(targetLibDir, dbgName);
									copySubdirs(dbgFile.getParentFile(), dbgFile, dbgTarget, debug, additionalFiles, true);
									symlinkSubdirs(dbgFile.getParentFile(), dbgFile, dbgTarget, debug, symlinks);
//									String mainName = "/" + (debug ? name + "_debug" : name);
									for(Map.Entry<String,Boolean> file : additionalFiles) {
										Element libraryElement;
										libraryElement = doc.createElement("file");
										if(Boolean.TRUE.equals(file.getValue())) {
											libraryElement.setAttribute("executable", "true");
										}
										libraryElement.setAttribute("name", jarpath+"/"+file.getKey());
										doc.getDocumentElement().appendChild(libraryElement);
										libraryIncludes += "," + jarpath+"/"+file.getKey();
							        }
							        
							        for(Map.Entry<String,String> unpack : symlinks) {
										Element libraryElement = doc.createElement("symlink");
										libraryElement.setAttribute("name", jarpath+"/"+unpack.getKey());
										libraryElement.setAttribute("target", jarpath+"/"+unpack.getValue());
										doc.getDocumentElement().appendChild(libraryElement);
							        }
								}else {
									java.io.File dbgTarget = new java.io.File(targetLibDir, dbgName);
									copyFiles(dbgFile, dbgTarget);
									Element libraryElement = doc.createElement("file");
									libraryElement.setAttribute("name", "lib/"+dbgTarget.getName());
									doc.getDocumentElement().appendChild(libraryElement);
								}
								if(!libraryIncludes.isEmpty())
									libraryIncludes += ",";
								libraryIncludes += jarpath + "/" + dbgName;
							}
						}
						break;
					default:
						if(debug || forceDebugInfo) {
							java.io.File dbgFile = new java.io.File(new java.io.File(builddir, _libdir), libName+".debug");
							if(dbgFile.exists()) {
								copyFiles(dbgFile, new java.io.File(targetLibDir, libName+".debug"));
								if(!libraryIncludes.isEmpty())
									libraryIncludes += ",";
								libraryIncludes += jarpath + "/" + libName+".debug";
								Element libraryElement = doc.createElement("file");
								libraryElement.setAttribute("name", jarpath + "/" + libName+".debug");
								doc.getDocumentElement().appendChild(libraryElement);
							}
						}
						break;
					}
				}
				if(!isMacBundle) {
					_libdir = libdir;
					boolean noSymlinks = false;
		        	switch(moduleName) {
					case "containeraccess":
						noSymlinks = true;
						break;
					case "qtjambi.deployer":
						_libdir = "utilities";
						noSymlinks = true;
						break;
					default:
						if(osInfo.crossOS()==OSInfo.OperationSystem.Android) {
							switch(osInfo.crossArch()) {
				        	case arm:
								_libdir += "/armeabi-v7a"; break;
				        	case arm64:
				        		_libdir += "/arm64-v8a"; break;
				        	case x86:
				        		_libdir += "/x86"; break;
				        	case x86_64:
				        		_libdir += "/x86_64"; break;
				        		default: break;
				        	}
						}
						break;
		        	}
		        	if(debug || !forceDebugInfo) {
						Element libraryElement = doc.createElement("library");
						libraryElement.setAttribute("name", _libdir+"/"+libName);
						doc.getDocumentElement().appendChild(libraryElement);
		        	}
					
					switch(osInfo.crossOS()) {
					case Windows:
						if(libName.endsWith(".dll")){
							String pdbName = libName.substring(0, libName.length()-3)+"pdb";
							java.io.File pdbFile = new java.io.File(new java.io.File(builddir, _libdir), pdbName);
							if(!pdbFile.exists() && _libdir.equals("bin")) {
								pdbFile = new java.io.File(new java.io.File(builddir, "lib"), pdbName);
							}
							if(pdbFile.exists()) {
								if(debug || forceDebugInfo) {
									Element libraryElement = doc.createElement("file");
									libraryElement.setAttribute("name", _libdir+"/"+pdbName);
									doc.getDocumentElement().appendChild(libraryElement);
								}
							}else if(debug || forceDebugInfo){
								pdbFile = new java.io.File(new java.io.File(builddir, _libdir), libName+".debug");
								if(pdbFile.exists()) {
									Element libraryElement = doc.createElement("file");
									libraryElement.setAttribute("name", _libdir+"/"+libName+".debug");
									doc.getDocumentElement().appendChild(libraryElement);
								}
							}
						}
						break;
					case MacOS:
						break;
					default:
						if(debug || forceDebugInfo){
							java.io.File dbgFile = new java.io.File(new java.io.File(builddir, _libdir), libName+".debug");
							if(dbgFile.exists()) {
								Element libraryElement = doc.createElement("file");
								libraryElement.setAttribute("name", _libdir+"/"+libName+".debug");
								doc.getDocumentElement().appendChild(libraryElement);
							}
						}
						break;
					}
					
					String shortName;
					Element symlinkElement;
					if(!noSymlinks && (debug || !forceDebugInfo)) {
						switch(osInfo.crossOS()) {
						case Windows:
						case Android:
						case IOS:
							break;
						case MacOS:
							if(useFrameworks) {
								break;
							}
							//fallthrough:
						default:
							symlinkElement = doc.createElement("symlink");
							shortName = formatQtJambiName(osInfo, name, debug, -1, -1, -1);
							symlinkElement.setAttribute("name", _libdir+"/"+shortName);
							symlinkElement.setAttribute("target", _libdir+"/"+libName);
							doc.getDocumentElement().appendChild(symlinkElement);
							Path link = new java.io.File(target.getParentFile(), shortName).toPath();
							boolean create = true;
							if(Files.exists(link)) {
								create = false;
								if(!Files.isSymbolicLink(link)){
		    						create = true;
		    					}else {
		    						Path targetPath = link.toRealPath();
		    						if(!Path.of(target.getName()).equals(targetPath)) {
		    							create = true;
		    						}
		    					}
							}
							if(create) {
								try{
									Files.deleteIfExists(link);
									getProject().log(this, "create symbolic link " + link + " -> " + libName, Project.MSG_INFO);
									Files.createSymbolicLink(link, Path.of(target.getName()));
								}catch(java.nio.file.FileAlreadyExistsException ex) {
								} catch (Exception ex) {
									ex.printStackTrace();
								}
	    					}
							symlinkElement = doc.createElement("symlink");
							shortName = formatQtJambiName(osInfo, name, debug, qtMajorVersion, -1, -1);
							symlinkElement.setAttribute("name", _libdir+"/"+shortName);
							symlinkElement.setAttribute("target", _libdir+"/"+libName);
							doc.getDocumentElement().appendChild(symlinkElement);
							link = new java.io.File(target.getParentFile(), shortName).toPath();
							create = true;
							if(Files.exists(link)) {
								create = false;
								if(!Files.isSymbolicLink(link)){
		    						create = true;
		    					}else {
		    						Path targetPath = link.toRealPath();
		    						if(!Path.of(target.getName()).equals(targetPath)) {
		    							create = true;
		    						}
		    					}
							}
							if(create) {
								try{
									Files.deleteIfExists(link);
									getProject().log(this, "create symbolic link " + link + " -> " + libName, Project.MSG_INFO);
									Files.createSymbolicLink(link, Path.of(target.getName()));
								}catch(java.nio.file.FileAlreadyExistsException ex) {
								} catch (Exception ex) {
									ex.printStackTrace();
								}
	    					}
							symlinkElement = doc.createElement("symlink");
							shortName = formatQtJambiName(osInfo, name, debug, qtMajorVersion, qtMinorVersion, -1);
							symlinkElement.setAttribute("name", _libdir+"/"+shortName);
							symlinkElement.setAttribute("target", _libdir+"/"+libName);
							doc.getDocumentElement().appendChild(symlinkElement);
							link = new java.io.File(target.getParentFile(), shortName).toPath();
							create = true;
							if(Files.exists(link)) {
								create = false;
								if(!Files.isSymbolicLink(link)){
		    						create = true;
		    					}else {
		    						Path targetPath = link.toRealPath();
		    						if(!Path.of(target.getName()).equals(targetPath)) {
		    							create = true;
		    						}
		    					}
							}
							if(create) {
								try{
									Files.deleteIfExists(link);
									getProject().log(this, "create symbolic link " + link + " -> " + libName, Project.MSG_INFO);
									Files.createSymbolicLink(link, Path.of(target.getName()));
								}catch(java.nio.file.FileAlreadyExistsException ex) {
								} catch (Exception ex) {
									ex.printStackTrace();
								}
	    					}
							break;
						}
					}
				}
			}
			
			java.io.File metainfFile = new java.io.File(tempDirectory!=null ? new java.io.File(tempDirectory) : directory, "META-INF");
			System.out.println("make directory: "+metainfFile);
			metainfFile.mkdirs();
			java.io.File deploymentFile = new java.io.File(metainfFile, "qtjambi-deployment.xml");
        	deploymentFile.delete();
			boolean isUtilities = false;
			switch(moduleName) {
			case "qtjambi.deployer":
				deploymentFile = new java.io.File(metainfFile, "qtjambi-utilities.xml");
				deploymentFile.delete();
				isUtilities = true;
				switch(osInfo.crossOS()) {
				case Android:
					Element libraryElement = doc.createElement("file");
					libraryElement.setAttribute("name", "utilities/QtAndroidBindings.jar");
					doc.getDocumentElement().appendChild(libraryElement);
					libraryIncludes += ",utilities/QtAndroidBindings.jar";
	        		break;
				default: break;
				}
				default:
					if(doc.getDocumentElement().hasChildNodes()) {
						if(!AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME).startsWith("android") || isUtilities) {
							System.out.println("write file: "+deploymentFile);
							try(FileOutputStream fos = new FileOutputStream(deploymentFile)){
								StreamResult result = new StreamResult(fos);
								transformer.transform(new DOMSource(doc), result);
							}
						}else if(!isUtilities){
							System.out.println("Skipping META-INF/qtjambi-deployment.xml for android bundle.");
						}
					}else {
						System.out.println("Skipping META-INF/qtjambi-deployment.xml: no entries.");
					}
        	}
			if(!"true".equalsIgnoreCase(getProject().getProperty("qtjambi.force.native-jar-creation"))) {
				if(skipNativeJar) {
					libraryIncludes = sourcesIncludes = "";
					sourcesPaths.clear();
				}
			}
		}catch(RuntimeException | Error e){
			throw e;
		}catch(Exception e){
			throw new BuildException(e);
		}finally {
//			System.out.println("library.includes="+libraryIncludes);
			getProject().setProperty("library.includes", libraryIncludes);
			getProject().setProperty("sources.includes", sourcesIncludes);
			getProject().setProperty("sources.paths", String.join(File.pathSeparator, sourcesPaths));
		}
	}
	
	private String libraries;
	private String moduleName;
	private String outputDirectory;
	private String tempDirectory;
	private String jarFile;
	private boolean debug = false;
	private boolean forceDebugInfo = false;
	private boolean plugin = false;
	
	private void copyFiles(java.io.File sourceFile, java.io.File targetFile) throws IOException {
		Path sourceFilePath = sourceFile.toPath();
		Path targetFilePath = targetFile.toPath();
		if(Files.notExists(targetFilePath)
				|| Files.getLastModifiedTime(targetFilePath).to(TimeUnit.SECONDS)!=Files.getLastModifiedTime(sourceFilePath).to(TimeUnit.SECONDS)
				|| Files.size(targetFilePath)!=Files.size(sourceFilePath)) {
			skipNativeJar = false;
			targetFile.getParentFile().mkdirs();
			try {
				Files.copy(sourceFilePath, targetFilePath, StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
			}catch(FileSystemException e){
				if(!e.getFile().contains("._"))
					throw e;
			}
		}
	}
    
    private void copySubdirs(File root, File srcDir, File destDir, boolean debug, List<Map.Entry<String,Boolean>> additionalFiles, boolean allowHeaders) throws IOException {
    	if(srcDir.isDirectory()) {
	    	for(File content : srcDir.listFiles()) {
	    		if((debug || (!content.getName().endsWith(".prl") && !content.getName().endsWith(".dSYM")))
	    				&& (allowHeaders || !content.getName().equals("Headers"))
	    				&& !content.getName().endsWith(".pri") && !content.getName().endsWith(".pro")) {
	    			if(!Files.isSymbolicLink(content.toPath())){
	    				if(content.isDirectory()) {
	        				copySubdirs(root, new File(srcDir, content.getName()), new File(destDir, content.getName()), debug, additionalFiles, allowHeaders);
	        			}else {
		    				File destFile = new File(destDir, content.getName());
		    				destDir.mkdirs();
		    				copyFiles(content, destFile);
		    				additionalFiles.add(new SimpleEntry<>(root.toPath().relativize(content.toPath()).toString(), content.canExecute()));
	        			}
	    			}
	    		}
	    	}
    	}else {
    		System.err.println("cannot copy directory "+srcDir);
    	}
    }
    
    private void symlinkSubdirs(File root, File srcDir, File destDir, boolean debug, List<Map.Entry<String,String>> symlinks) throws IOException {
    	if(srcDir.isDirectory()) {
	    	for(File content : srcDir.listFiles()) {
	    		if((debug || (!content.getName().endsWith(".prl") && !content.getName().endsWith(".dSYM")))
	    				//&& !content.getName().equals("Headers")
    				) {
	    			if(Files.isSymbolicLink(content.toPath())){
	    				File destFile = new File(destDir, content.getName());
	    				Path target = null;
	    				try {
	    					target = content.toPath().toRealPath();
	    					if(target.isAbsolute())
	    						target = srcDir.toPath().relativize(target);
	    					if(!Files.exists(destFile.toPath())) {
	    						try{
	    							getProject().log(this, "create symbolic link " + destFile + " -> " + content, Project.MSG_INFO);
	    							Files.createSymbolicLink(destFile.toPath(), target);
	    						}catch(java.nio.file.FileAlreadyExistsException e) {}
	    					}else if(!Files.isSymbolicLink(destFile.toPath())){
	    						destFile.delete();
	    						try{
	    							getProject().log(this, "create symbolic link " + destFile + " -> " + content, Project.MSG_INFO);
		    						Files.createSymbolicLink(destFile.toPath(), target);
								}catch(java.nio.file.FileAlreadyExistsException e) {}
	    					}
	    					if(!target.isAbsolute())
	    						target = srcDir.toPath().resolve(target);
							symlinks.add(new SimpleEntry<>(root.toPath().relativize(content.toPath()).toString(), root.toPath().relativize(target).toString()));
						} catch (Exception e) {
							System.out.println("root: "+root.toPath());
							System.out.println("link: "+content.toPath());
							System.out.println("target: "+target);
							e.printStackTrace();
						}
	    			}else if(content.isDirectory()) {
						symlinkSubdirs(root, new File(srcDir, content.getName()), new File(destDir, content.getName()), debug, symlinks);
	    			}
	    		}
	    	}
    	}
    }

    public static String formatPluginName(OSInfo osInfo, String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if(debug) {
            switch(osInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14){
                    return "lib" + name + "_debug.dylib";
                }else{
                    return "lib" + name + ".dylib";
                }
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	        		default: break;
	            	}
            	}
            case IOS:
                return "lib" + name + "_debug.a";
			default:
				if(osInfo.crossOS().isUnixLike())
					return "lib" + name + ".so";
				break;
            }
        } else {
            switch(osInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	            	default: break;
	            	}
            	}
            case IOS:
                return "lib" + name + ".a";
			default:
				if(osInfo.crossOS().isUnixLike())
					return "lib" + name + ".so";
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

	public static String formatQmlPluginName(OSInfo osInfo, String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if (debug) {
            switch (osInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS: 
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + "_debug.dylib";
                else return "lib" + name + ".dylib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
            		switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	            	default: break;
	            	}
            	}
            case IOS: return "lib" + name + "_debug.a";
			default:
				if(osInfo.crossOS().isUnixLike())
					return "lib" + name + ".so";
				break;
            }
        } else {
            switch (osInfo.crossOS()) {
            case Windows: return name + ".dll";
            case MacOS: return "lib" + name + ".dylib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
            		switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	            	default: break;
	            	}
            	}
            case IOS: return "lib" + name + ".a";
			default:
				if(osInfo.crossOS().isUnixLike())
					return "lib" + name + ".so";
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }
    
    public static String formatQtNameAsFramework(String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        String result = name + ".framework/Versions/" + qtMajorVersion + "/" + name;
        if(qtMajorVersion==6) {
        	result = name + ".framework/Versions/A/" + name;
        }
        if(qtMajorVersion==5 && qtMinorVersion<14){
            if(debug)
                result += "_debug";
        }
        return result;
    }
    
    public static String formatQtName(OSInfo osInfo, String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
    	return formatQtName(osInfo, name, infix, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, false);
    }

    public static String formatQtName(OSInfo osInfo, String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion, boolean staticLib) {
        String tmpVersionString;
        if(qtMajorVersion>=5){
    		if(name.startsWith("Qt") && !name.startsWith("Qt"+qtMajorVersion)){
    			name = "Qt"+qtMajorVersion+name.substring(2);
    		}
    		tmpVersionString = "";
    	}else if(qtMajorVersion>=0){
            tmpVersionString = ""+qtMajorVersion;
        }else{
            tmpVersionString = "";
        }
        if(infix==null || name.startsWith("lib")){
    		infix = "";
    	}
        String libPrefix = name.startsWith("lib") ? "" : "lib";
        if(staticLib) {
        	switch(osInfo.crossOS()) {
            case Windows:
                return name + infix + (debug ? "d" : "") + tmpVersionString + ".lib";
            default:
            	return "lib" + name + infix + (debug ? "_debug" : "") + ".a";
            }
        }else if(debug) {
            String tmpDotVersionString;
            if(qtMajorVersion>=0){
                if(qtMinorVersion>=0){
                    if(qtPatchlevelVersion>=0){
                        tmpDotVersionString = String.format(".%1$s.%2$s.%3$s", qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
                    }else{
                        tmpDotVersionString = String.format(".%1$s.%2$s", qtMajorVersion, qtMinorVersion);
                    }
                }else{
                    tmpDotVersionString = String.format(".%1$s", qtMajorVersion);
                }
            }else{
                tmpDotVersionString = "";
            }
            switch(osInfo.crossOS()) {
            case Windows:
                return name + infix + "d" + tmpVersionString + ".dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14){
                    return libPrefix + name + infix + "_debug" + tmpDotVersionString + ".dylib";
                }else{
                    return libPrefix + name + infix + tmpDotVersionString + ".dylib";
                }
            case Android:
            	if(infix==null || infix.isEmpty()) {
                	if(osInfo.crossArch()==null) {
                		if(infix.isEmpty())
                			return libPrefix + name + infix + "_x86_64.so";
                		else
                			return libPrefix + name + infix + ".so";
                	}else {
		            	switch(osInfo.crossArch()) {
		            	case arm:
		            		return libPrefix + name + infix + "_armeabi-v7a.so";
		            	case arm64:
		            		return libPrefix + name + infix + "_arm64-v8a.so";
		            	case x86:
		            		return libPrefix + name + infix + "_x86.so";
		            	case x86_64:
		            		return libPrefix + name + infix + "_x86_64.so";
		            	case riscv64:
		            		return libPrefix + name + infix + "_riscv64.so";
		         		default: break;
		            	}
                	}
            	}
                return libPrefix + name + infix + ".so";
            case IOS: return "lib" + name + infix + "_debug.a";
			default:
				if(osInfo.crossOS().isUnixLike())
	                return libPrefix + name + infix + ".so" + tmpDotVersionString;
				break;
            }
        } else {
            String tmpDotVersionString;
            if(qtMajorVersion>=0){
                if(qtMinorVersion>=0){
                    if(qtPatchlevelVersion>=0){
                        tmpDotVersionString = String.format(".%1$s.%2$s.%3$s", qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
                    }else{
                        tmpDotVersionString = String.format(".%1$s.%2$s", qtMajorVersion, qtMinorVersion);
                    }
                }else{
                    tmpDotVersionString = String.format(".%1$s", qtMajorVersion);
                }
            }else{
                tmpDotVersionString = "";
            }
            switch(osInfo.crossOS()) {
            case Windows:
                return name + infix + tmpVersionString + ".dll";
            case MacOS:
                return libPrefix + name + infix + tmpDotVersionString + ".dylib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		if(infix.isEmpty())
            			return libPrefix + name + infix + "_x86_64.so";
            		else
            			return libPrefix + name + infix + ".so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return libPrefix + name + infix + "_armeabi-v7a.so";
	            	case arm64:
	            		return libPrefix + name + infix + "_arm64-v8a.so";
	            	case x86:
	            		return libPrefix + name + infix + "_x86.so";
	            	case x86_64:
	            		return libPrefix + name + infix + "_x86_64.so";
	            	case riscv64:
	            		return libPrefix + name + infix + "_riscv64.so";
	         		default: break;
	            	}
            	}
                return libPrefix + name + infix + ".so";
            case IOS: return "lib" + name + infix + ".a";
			default:
				if(osInfo.crossOS().isUnixLike())
	                return libPrefix + name + infix + ".so" + tmpDotVersionString;
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }
    
    public static String formatQtPrlNameAsFramework(String name, String infix, boolean debug) {
        String result = name + ".framework/" + name;
        if(debug)
            result += "_debug";
        result += ".prl";
        return result;
    }

    public static String formatQtPrlName(OSInfo osInfo, String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        // Windows: QtCore.prl QtCored.prl
        //   Linux: libQtCore.prl ??????
        //  MacOSX: libQtCore.prl libQtCore_debug.prl
    	if(qtMajorVersion>=5){
    		if(name.startsWith("Qt") && !name.startsWith("Qt"+qtMajorVersion)){
    			name = "Qt"+qtMajorVersion+name.substring(2);
    		}
    	}
    	if(infix==null){
    		infix = "";
    	}
        if(debug) {
            String tmpDebugSuffix = "_debug";
            switch(osInfo.crossOS()) {
            case Windows:
                return name + infix + "d" + ".prl";
            case MacOS:
            case IOS:
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + infix + ".prl";
                else return "lib" + name + infix + tmpDebugSuffix + ".prl";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + infix + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + infix + "_armeabi-v7a.prl";
	            	case arm64:
	            		return "lib" + name + infix + "_arm64-v8a.prl";
	            	case x86:
	            		return "lib" + name + infix + "_x86.prl";
	            	case x86_64:
	            		return "lib" + name + infix + "_x86_64.prl";
	            	case riscv64:
	            		return "lib" + name + infix + "_riscv64.prl";
	         		default: break;
	            	}
            	}
			default:
				if(osInfo.crossOS().isUnixLike())
	                return "lib" + name + infix + ".prl";
				break;
            }
        } else {
            switch(osInfo.crossOS()) {
            case Windows:
                return name + ".prl";
            case MacOS:
            case IOS:
                return "lib" + name + infix + ".prl";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + infix + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + infix + "_armeabi-v7a.prl";
	            	case arm64:
	            		return "lib" + name + infix + "_arm64-v8a.prl";
	            	case x86:
	            		return "lib" + name + infix + "_x86.prl";
	            	case x86_64:
	            		return "lib" + name + infix + "_x86_64.prl";
	            	case riscv64:
	            		return "lib" + name + infix + "_riscv64.prl";
	         		default: break;
	            	}
            	}
			default:
				if(osInfo.crossOS().isUnixLike())
	                return "lib" + name + infix + ".prl";
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatUnversionedPluginName(OSInfo osInfo, String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if(debug) {
            String tmpDebugSuffix = "_debug";
            switch(osInfo.crossOS()) {
            case Windows:
                return name + "d.dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + tmpDebugSuffix + ".dylib";
                else return "lib" + name + ".dylib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	         		default: break;
	            	}
            	}
            case IOS:
                return "lib" + name + ".a";
			default:
				if(osInfo.crossOS().isUnixLike())
	                return "lib" + name + ".so";
				break;
            }
        } else {
            switch(osInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	         		default: break;
	            	}
            	}
            case IOS:
                return "lib" + name + ".a";
			default:
				if(osInfo.crossOS().isUnixLike())
	                return "lib" + name + ".so";
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }
    
    public static String formatQtJambiName(OSInfo osInfo, String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        String tmpVersionString = (qtMajorVersion>=0) ? ""+qtMajorVersion : "";
    	String tmpDotVersionString;
        if(qtMajorVersion>=0){
            if(qtMinorVersion>=0){
                if(qtPatchlevelVersion>=0){
                    tmpDotVersionString = String.format(".%1$s.%2$s.%3$s", qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
                }else{
                    tmpDotVersionString = String.format(".%1$s.%2$s", qtMajorVersion, qtMinorVersion);
                }
            }else{
                tmpDotVersionString = String.format(".%1$s", qtMajorVersion);
            }
        }else{
            tmpDotVersionString = "";
        }
        if(debug) {
            String tmpDebugSuffix = "_debug";
            switch(osInfo.crossOS()) {
            case Windows:
        		return name + "d" + tmpVersionString + ".dll";
            case MacOS:
        		return "lib" + name + tmpDebugSuffix + tmpDotVersionString + ".jnilib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + tmpDebugSuffix + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + tmpDebugSuffix + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + tmpDebugSuffix + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + tmpDebugSuffix + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + tmpDebugSuffix + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + tmpDebugSuffix + "_riscv64.so";
	            	default: break;
	            	}
            	}
                return "lib" + name + tmpDebugSuffix + ".so";
            case IOS: return "lib" + name + tmpDebugSuffix + ".a";
			default:
				if(osInfo.crossOS().isUnixLike())
	                return "lib" + name + tmpDebugSuffix + ".so" + tmpDotVersionString;
				break;
            }
        } else {
            switch(osInfo.crossOS()) {
            case Windows:
        		return name + tmpVersionString + ".dll";
            case MacOS:
        		return "lib" + name + tmpDotVersionString + ".jnilib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	            	default: break;
	            	}
            	}
                return "lib" + name + ".so";
            case IOS: return "lib" + name + ".a";
			default:
				if(osInfo.crossOS().isUnixLike())
	                return "lib" + name + ".so" + tmpDotVersionString;
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatQtJambiPluginName(OSInfo osInfo, String name, boolean debug, String versionString) {
    	if(debug) {
            switch(osInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS:
                return "lib" + name + "_debug.dylib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	         		default: break;
	            	}
            	}
            case IOS:
                return "lib" + name + "_debug.a";
			default:
				if(osInfo.crossOS().isUnixLike())
	                return "lib" + name + "_debug.so";
				break;
            }
        } else {
            switch(osInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	            	switch(osInfo.crossArch()) {
	            	case arm:
	            		return "lib" + name + "_armeabi-v7a.so";
	            	case arm64:
	            		return "lib" + name + "_arm64-v8a.so";
	            	case x86:
	            		return "lib" + name + "_x86.so";
	            	case x86_64:
	            		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	         		default: break;
	            	}
            	}
            case IOS:
                return "lib" + name + ".a";
			default:
				if(osInfo.crossOS().isUnixLike())
	                return "lib" + name + ".so";
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatQtJambiQmlPluginName(OSInfo osInfo, String name, boolean debug, String versionString) {
//        String tmpDotVersionString = (versionString != null && versionString.length() > 0) ? "." + versionString : "";
         if(debug) {
            String tmpDebugSuffix = "_debug";
             switch(osInfo.crossOS()) {
             case Windows:
        		 return name + "d.dll";
             case MacOS:
                return "lib" + name + tmpDebugSuffix + ".dylib";
             case Android:
             	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	             	switch(osInfo.crossArch()) {
	             	case arm:
	             		return "lib" + name + "_armeabi-v7a.so";
	             	case arm64:
	             		return "lib" + name + "_arm64-v8a.so";
	             	case x86:
	             		return "lib" + name + "_x86.so";
	             	case x86_64:
	             		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	         		default: break;
	             	}
            	}
             case IOS: return "lib" + name + tmpDebugSuffix + ".a";
 			default:
				if(osInfo.crossOS().isUnixLike())
					return "lib" + name + tmpDebugSuffix + ".so";
				break;
             }
         } else {
             switch(osInfo.crossOS()) {
             case Windows:
        		 return name + ".dll";
             case MacOS:
                return "lib" + name + ".dylib";
             case Android:
             	if(osInfo.crossArch()==null) {
            		return "lib" + name + "_x86_64.so";
            	}else {
	             	switch(osInfo.crossArch()) {
	             	case arm:
	             		return "lib" + name + "_armeabi-v7a.so";
	             	case arm64:
	             		return "lib" + name + "_arm64-v8a.so";
	             	case x86:
	             		return "lib" + name + "_x86.so";
	             	case x86_64:
	             		return "lib" + name + "_x86_64.so";
	            	case riscv64:
	            		return "lib" + name + "_riscv64.so";
	         		default: break;
	             	}
            	}
             case IOS: return "lib" + name + ".a";
 			default:
				if(osInfo.crossOS().isUnixLike())
					return "lib" + name + ".so";
				break;
             }
         }
         throw new BuildException("unhandled case...");
    }
	public boolean isForceDebugInfo() {
		return forceDebugInfo;
	}
	public void setForceDebugInfo(boolean forceDebugInfo) {
		this.forceDebugInfo = forceDebugInfo;
	}
	public String getJarFile() {
		return jarFile;
	}
	public void setJarFile(String jarFile) {
		this.jarFile = jarFile;
	}
	public String getTempDirectory() {
		return tempDirectory;
	}
	public void setTempDirectory(String tempDirectory) {
		this.tempDirectory = tempDirectory;
	}
}
