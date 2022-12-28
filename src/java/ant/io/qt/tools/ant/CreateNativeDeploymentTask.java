package io.qt.tools.ant;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
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
    
	@Override
    public void execute() throws BuildException {
        getProject().log(this, msg, Project.MSG_INFO);
        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        boolean useFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_GENERATE_FRAMEWORKS));
    	String libraryIncludes = "";
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
			doc.getDocumentElement().setAttribute("module", moduleName);
			doc.getDocumentElement().setAttribute("system", AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME));
			doc.getDocumentElement().setAttribute("version", String.format("%1$s.%2$s.%3$s", qtMajorVersion, qtMinorVersion, qtjambiPatchlevelVersion));
			doc.getDocumentElement().setAttribute("compiler", AntUtil.getPropertyAsString(propertyHelper, "qtjambi.compiler"));
			doc.getDocumentElement().setAttribute("configuration", debug ? "debug" : "release");
			doc.getDocumentElement().setAttribute("date", AntUtil.getPropertyAsString(propertyHelper, "DSTAMP"));
			doc.getDocumentElement().setAttribute("time", AntUtil.getPropertyAsString(propertyHelper, "TSTAMP"));
			String libdir;
			java.io.File libc_for_android = null;
			String abi = null;
			switch(OSInfo.crossOS()) {
			case Android:
				OSInfo.setCrossOSArchName(AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME));
				switch(OSInfo.crossOSArchName()) {
	        	case OSInfo.K_ANDROID_ARM32:
	        		abi = "armeabi-v7a"; 
	        		break;
	        	case OSInfo.K_ANDROID_ARM64:
	        		abi = "arm64-v8a"; 
	        		break;
	        	case OSInfo.K_ANDROID_X86:
	        		abi = "x86";
	        		break;
	        	default: //case OSInfo.K_ANDROID_X64:
	        		abi = "x86_64";
	        		break;
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
						}
					}
					/*
					Document resources = builder.getDOMImplementation().createDocument(
							null,
							"resources",
							null
						);
					Element arrayElement = resources.createElement("array");
					arrayElement.setAttribute("name", "bundled_libs");
					resources.getDocumentElement().appendChild(arrayElement);
					arrayElement = resources.createElement("array");
					arrayElement.setAttribute("name", "qt_libs");
					Element itemElement = resources.createElement("item");
					itemElement.setTextContent(abi+";QtJambi_"+abi);
					arrayElement.appendChild(itemElement);
					resources.getDocumentElement().appendChild(arrayElement);
					arrayElement = resources.createElement("array");
					arrayElement.setAttribute("name", "load_local_libs");
					resources.getDocumentElement().appendChild(arrayElement);
					arrayElement = resources.createElement("string");
					arrayElement.setAttribute("name", "static_init_classes");
					resources.getDocumentElement().appendChild(arrayElement);
					arrayElement = resources.createElement("string");
					arrayElement.setAttribute("name", "use_local_qt_libs");
					arrayElement.setTextContent("1");
					resources.getDocumentElement().appendChild(arrayElement);
					arrayElement = resources.createElement("string");
					arrayElement.setAttribute("name", "bundle_local_qt_libs");
					arrayElement.setTextContent("1");
					resources.getDocumentElement().appendChild(arrayElement);
					java.io.File resourcesFile = directory;
					resourcesFile = new java.io.File(resourcesFile, "res");
					resourcesFile = new java.io.File(resourcesFile, "values");
					resourcesFile.mkdirs();
					resourcesFile = new java.io.File(resourcesFile, abi+".xml");
					try(FileOutputStream fos = new FileOutputStream(resourcesFile)){
						StreamResult result = new StreamResult(fos);
						transformer.transform(new DOMSource(resources), result);
					}
					if(!libraryIncludes.isEmpty())
						libraryIncludes += ",";
					libraryIncludes += resourcesFile.getParentFile().getParentFile().getName()+"/"+resourcesFile.getParentFile().getName()+"/"+resourcesFile.getName();
					*/
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
			switch(OSInfo.crossOS()) {
			case Android:
				targetLibDir = new java.io.File(targetLibDir, abi);
				if(libc_for_android!=null && libc_for_android.exists()) {
					targetLibDir.mkdirs();
					Files.copy(libc_for_android.toPath(), new java.io.File(targetLibDir, libc_for_android.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING);
					if(!libraryIncludes.isEmpty())
						libraryIncludes += ",";
					libraryIncludes += libdir + "/" + targetLibDir.getName() + "/" + libc_for_android.getName();
				}
				break;
			case MacOS:
				if(no_otool==null) {
					no_otool = false;
					try {
			        	String[] out = Exec.executeCaptureOutput(this, Arrays.asList("otool", "--version"), builddir, getProject(), null, null, false);
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
//			String debugLib1 = debug ? "_debug" : "";
//			String debugQtLib = debugLib1;
//	        if(qtMajorVersion==5 && qtMinorVersion>=14){
//	        	debugQtLib = "";
//	        }
//	        String qtlibdir = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qt.libdir");
	        
        	switch(moduleName) {
			case "qtjambi.deployer":
				switch(OSInfo.crossOS()) {
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
						switch(OSInfo.crossOS()) {
						case Windows:
							if(!debug && f.endsWith(".dll") && !f.endsWith("d.dll")) {
								_libraries.add(f);
							}else if(debug && (f.endsWith("d.dll") || f.endsWith("d.pdb") || f.endsWith(".dll.debug"))) {
								_libraries.add(f);
							}
							break;
						case Linux:
							if(f.endsWith(".so")) {
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
							switch(OSInfo.crossOSArchName()) {
				        	case OSInfo.K_ANDROID_ARM32:
				        		targetLibDir = new java.io.File(new java.io.File(directory, "lib"), "arm32");
				        		suffix = "_armeabi-v7a.so"; break;
				        	case OSInfo.K_ANDROID_ARM64:
				        		targetLibDir = new java.io.File(new java.io.File(directory, "lib"), "arm64");
				        		suffix = "_arm64-v8a.so"; break;
				        	case OSInfo.K_ANDROID_X86:
				        		targetLibDir = new java.io.File(new java.io.File(directory, "lib"), "x86");
				        		suffix = "_x86.so"; break;
				        	case OSInfo.K_ANDROID_X64:
				        		targetLibDir = new java.io.File(new java.io.File(directory, "lib"), "x86_64");
				        		suffix = "_x86_64.so"; break;
				        	}
							if(f.endsWith(suffix)) {
								_libraries.add(f);
							}
							break;
						default:
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
						switch(OSInfo.crossOS()) {
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
						libName = formatQtJambiQmlPluginName(name, debug, "");
					}
					break;
				default:
					if(plugin) {
						if(!"containeraccess".equals(moduleName)) {
							java.io.File pluginDir = new java.io.File(new java.io.File(directory, "plugins"), name);
							for(String f : pluginDir.list()) {
								if(!f.equals(".") && !f.equals("..")) {
									Element libraryElement = doc.createElement("library");
									libraryElement.setAttribute("name", "plugins/"+name+"/"+f);
									doc.getDocumentElement().appendChild(libraryElement);
								}
							}
						}
						continue;
					}else if(OSInfo.crossOS()==OSInfo.OS.MacOS && useFrameworks) {
						libName = name + (debug ? "_debug.framework" : ".framework");
						isMacBundle = true;
					}else {
						libName = formatQtJambiName(name, debug, qtMajorVersion, qtMinorVersion, qtjambiPatchlevelVersion);
						switch(OSInfo.crossOSArchName()) {
			        	case OSInfo.K_ANDROID_ARM32:
			        		jarpath += "/armeabi-v7a"; break;
			        	case OSInfo.K_ANDROID_ARM64:
			        		jarpath += "/arm64-v8a"; break;
			        	case OSInfo.K_ANDROID_X86:
			        		jarpath += "/x86"; break;
			        	case OSInfo.K_ANDROID_X64:
			        		jarpath += "/x86_64"; break;
			        	}
					}
					break;
				}
				if(!libraryIncludes.isEmpty())
					libraryIncludes += ",";
				libraryIncludes += jarpath + "/" + libName;
//				if(isMacBundle && "QtJambiLauncher".equals(name)) {
//					libraryIncludes += "/Contents/MacOS/QtJambiLauncher";
//					if(debug)
//						libraryIncludes += "_debug";
//					libraryIncludes += "," + jarpath + "/" + libName + "/Contents/Resources/empty.lproj";
//					libraryIncludes += "," + jarpath + "/" + libName + "/Contents/Info.plist";
//					libraryIncludes += "," + jarpath + "/" + libName + "/Contents/PkgInfo";
//				}
				java.io.File libFile = new java.io.File(new java.io.File(builddir, _libdir), libName);
				java.io.File includeDir = new java.io.File(new java.io.File(builddir, "include"), name);
				java.io.File target = new java.io.File(targetLibDir, libName);
				if(isMacBundle) {
					List<Map.Entry<String,Boolean>> additionalFiles = new ArrayList<>();
					List<Map.Entry<String,String>> symlinks = new ArrayList<>();
					if(debug && "QtJambiLauncher".equals(name) && !new java.io.File(builddir, _libdir+"/QtJambiLauncher_debug.app/Contents/Info.plist").exists()) {
						Files.copy(new java.io.File(builddir, _libdir+"/QtJambiLauncher.app/Contents/Info.plist").toPath(), 
								new java.io.File(builddir, _libdir+"/QtJambiLauncher_debug.app/Contents/Info.plist").toPath(), StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
					}
					copySubdirs(libFile.getParentFile(), libFile, target, debug, additionalFiles, true);
					symlinkSubdirs(libFile.getParentFile(), libFile, target, debug, symlinks);
					String mainName = "/" + (debug ? name + "_debug" : name);
					for(Map.Entry<String,Boolean> file : additionalFiles) {
						Element libraryElement;
						if("QtJambiLauncher".equals(name)) {
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
					Files.copy(libFile.toPath(), target.toPath(), StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
					if(includeDir.isDirectory()) {
						java.io.File incDir = new java.io.File(new java.io.File(directory, "include"), name);
						incDir.mkdirs();
						for(File header : includeDir.listFiles()) {
							if(!Files.isSymbolicLink(header.toPath())){
								Files.copy(header.toPath(), new java.io.File(incDir, header.getName()).toPath(), StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
								Element libraryElement = doc.createElement("file");
								libraryElement.setAttribute("name", "include/"+name+"/"+header.getName());
								doc.getDocumentElement().appendChild(libraryElement);
								libraryIncludes += "," + "include/"+name+"/"+header.getName();
							}
						}
					}
					if(debug) {
						switch(OSInfo.crossOS()) {
						case Windows:
							if(libName.endsWith(".dll")){
								String pdbName = libName.substring(0, libName.length()-3)+"pdb";
								java.io.File pdbFile = new java.io.File(new java.io.File(builddir, _libdir), pdbName);
								if(!pdbFile.exists() && _libdir.equals("bin")) {
									pdbFile = new java.io.File(new java.io.File(builddir, "lib"), pdbName);
								}
								if(pdbFile.exists()) {
									/*
									 * don't include pdb files in jar
									java.io.File pdbTarget = new java.io.File(targetLibDir, pdbName);
									Files.copy(pdbFile.toPath(), pdbTarget.toPath(), StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
									if(!libraryIncludes.isEmpty())
										libraryIncludes += ",";
									libraryIncludes += jarpath + "/" + pdbName;
									*/
								}else {
									pdbFile = new java.io.File(new java.io.File(builddir, _libdir), libName+".debug");
									if(pdbFile.exists()) {
										java.io.File pdbTarget = new java.io.File(targetLibDir, libName+".debug");
										Files.copy(pdbFile.toPath(), pdbTarget.toPath(), StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
										if(!libraryIncludes.isEmpty())
											libraryIncludes += ",";
										libraryIncludes += jarpath + "/" + libName+".debug";
									}
								}
							}
							break;
							default:
						}
					}
				}
				switch(OSInfo.crossOS()) {
				case Linux:/*
					switch_moduleName:{
						switch(moduleName) {
						case "qtjambi.deployer":
							switch(name) {
							case "QtJambiLauncher":
								try{
									Exec.exec (this, new String[]{"chrpath", "--replace", "$ORIGIN/lib:$ORIGIN/../lib", target.getAbsolutePath()}, targetLibDir, getProject(), true );
								}catch(Exception e){
								}
								break switch_moduleName;
							case "QtJambiPlugin":
								try{
									Exec.exec (this, new String[]{"chrpath", "--replace", "$ORIGIN/../../lib", target.getAbsolutePath()}, targetLibDir, getProject(), true );
								}catch(Exception e){
								}
								break switch_moduleName;
							case "jarimport":
								try{
									Exec.exec (this, new String[]{"chrpath", "--replace", "$ORIGIN/../lib"
																						+ ":$ORIGIN/../../lib"
																						+ ":$ORIGIN/../../../lib"
																						+ ":$ORIGIN/../../../../lib"
																						+ ":$ORIGIN/../../../../../lib"
																						+ ":$ORIGIN/../../../../../../lib", target.getAbsolutePath()}, targetLibDir, getProject(), true );
								}catch(Exception e){
								}
								break switch_moduleName;
								default:
							}
						default:
							try{
								Exec.exec (this, new String[]{"chrpath", "--replace", "$ORIGIN/.", target.getAbsolutePath()}, targetLibDir, getProject(), true );
							}catch(Exception e){
							}
						}
					}*/
					break;
				case MacOS:/*
					switch(moduleName) {
					case "containeraccess": break;
					default:
						{
							String libInfix = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qt.libinfix");
							if(isMacExecutable) {
								libName += "/Contents/MacOS/"+name;
								if(debug)
									libName += "_debug";
							}
							OToolOut otoolOut = no_otool ? null : getOtoolOut(this, libName, targetLibDir);
					        if(otoolOut==null && !no_otool) {
					        	getProject().log(this, "otool does not provide info for " + libName, Project.MSG_INFO);
					        }
					        final List<String> command =  new ArrayList<String>();
					        command.add("install_name_tool");
					        command.add("-change");
					        command.add("");
					        command.add("@rpath/");
					        command.add(libName);
					        if(!name.equals("QtJambi")) {
						        for (String library : new String[]{
						            "libQtJambi"+debugLib1+"."+qtMajorVersion+".jnilib"
						        }) {
						            command.set(2, library);
						            command.set(3, "@rpath/"+library);
						            Exec.execute(this, command, targetLibDir, getProject());
						        }
					        }
					        
					        boolean useFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_USE_FRAMEWORK));
					        boolean convertQtFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_CONVERT_QT_FRAMEWORK));
					        String[] libraries = {};
					        if(convertQtFrameworks){
					        	if(qtMajorVersion<6) {
						            for (String library : libraries) {
						                command.set(2, "@rpath/Qt"+library+".framework/Versions/"+qtMajorVersion+"/Qt"+library);
						                command.set(3, "@rpath/libQt"+qtMajorVersion+library+libInfix+debugQtLib+"."+qtMajorVersion+".dylib");
						                Exec.execute(this, command, targetLibDir, getProject());
						            }
					        	}else {
					        		for (String library : libraries) {
						                command.set(2, "@rpath/Qt"+library+".framework/Versions/A/Qt"+library);
						                command.set(3, "@rpath/libQt"+qtMajorVersion+library+libInfix+debugQtLib+"."+qtMajorVersion+".dylib");
						                Exec.execute(this, command, targetLibDir, getProject());
						            }
					        	}
					        }else {
					        	if(!useFrameworks) {
						        	for (String library : libraries) {
						                command.set(2, "libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
						                command.set(3, "@rpath/libQt"+qtMajorVersion+library+libInfix+debugQtLib+"."+qtMajorVersion+".dylib");
						                Exec.execute(this, command, targetLibDir, getProject());
						            }
					        	}
					            if(qtlibdir!=null){
					                for (String library : new String[]{
					                    "libQtJambi"+debugLib1+"."+qtMajorVersion+".jnilib"
					                }) {
					                    command.set(2, qtlibdir+"/"+library);
					                    command.set(3, "@rpath/"+library);
					                    Exec.execute(this, command, targetLibDir, getProject());
					                }
					                for (String library : libraries) {
					                    command.set(2, qtlibdir+"/libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
					                    command.set(3, "@rpath/libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
					                    Exec.execute(this, command, targetLibDir, getProject());
					                }
					            }
					        }
					        Set<String> deleteCommands = new TreeSet<>();
					        String path = "@executable_path/../Frameworks";
					        if(otoolOut==null || otoolOut.rpaths.contains(path)) {
					        	deleteCommands.add(path);
					        }
					        path = "@loader_path/Frameworks";
					        if(otoolOut==null || otoolOut.rpaths.contains(path)) {
					        	deleteCommands.add(path);
					        }
					        path = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
							if(otoolOut==null || otoolOut.rpaths.contains(path)) {
								deleteCommands.add(path);
							}
							List<String> deletes = new ArrayList<>(deleteCommands);
							while(!deletes.isEmpty()) {
					    		List<String> commands = new ArrayList<>();
								commands.add("install_name_tool");
					    		for (int i = 0; i < 10 && i < deletes.size(); i++) 
					    		{
					                commands.add("-delete_rpath");
									commands.add(deletes.remove(0));
								}
								commands.add(libName);
								try{
									Exec.exec(this, commands.toArray(new String[commands.size()]), targetLibDir, getProject(), false);
								} catch ( BuildException e )
						        {
						        	getProject().log(this, " - " + (e.getMessage()!=null && !e.getMessage().isEmpty() ? e.getMessage() : e), e, Project.MSG_INFO);
						        }
					    	}
						}
					}*/
					break;
				default:
					break;
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
						switch(OSInfo.crossOSArchName()) {
			        	case OSInfo.K_ANDROID_ARM32:
							_libdir += "/armeabi-v7a"; break;
			        	case OSInfo.K_ANDROID_ARM64:
			        		_libdir += "/arm64-v8a"; break;
			        	case OSInfo.K_ANDROID_X86:
			        		_libdir += "/x86"; break;
			        	case OSInfo.K_ANDROID_X64:
			        		_libdir += "/x86_64"; break;
			        	}
						break;
		        	}
					Element libraryElement = doc.createElement("library");
					libraryElement.setAttribute("name", _libdir+"/"+libName);
					doc.getDocumentElement().appendChild(libraryElement);
					
					if(debug) {
						switch(OSInfo.crossOS()) {
						case Windows:
							if(libName.endsWith(".dll")){
								String pdbName = libName.substring(0, libName.length()-3)+"pdb";
								java.io.File pdbFile = new java.io.File(new java.io.File(builddir, _libdir), pdbName);
								if(!pdbFile.exists() && _libdir.equals("bin")) {
									pdbFile = new java.io.File(new java.io.File(builddir, "lib"), pdbName);
								}
								if(pdbFile.exists()) {
									libraryElement = doc.createElement("file");
									libraryElement.setAttribute("name", _libdir+"/"+pdbName);
									doc.getDocumentElement().appendChild(libraryElement);
								}else {
									pdbFile = new java.io.File(new java.io.File(builddir, _libdir), libName+".debug");
									if(pdbFile.exists()) {
										libraryElement = doc.createElement("file");
										libraryElement.setAttribute("name", _libdir+"/"+libName+".debug");
										doc.getDocumentElement().appendChild(libraryElement);
									}
								}
							}
							break;
							default:
						}
					}
					
					String shortName;
					Element symlinkElement;
					if(!noSymlinks) {
						switch(OSInfo.crossOS()) {
						case Windows:
						case Android:
						case IOS:
							break;
						case MacOS:
							if(useFrameworks) {
//								if(debug)
//									libraryIncludes += "_debug";
//								libraryIncludes += "," + jarpath + "/" + libName + "/Contents/Resources/empty.lproj";
//								libraryIncludes += "," + jarpath + "/" + libName + "/Contents/Info.plist";
//								libraryIncludes += "," + jarpath + "/" + libName + "/Contents/PkgInfo";
								break;
							}else {
								symlinkElement = doc.createElement("symlink");
								shortName = formatQtJambiName(name, debug, -1, -1, -1);
								symlinkElement.setAttribute("name", _libdir+"/"+shortName);
								symlinkElement.setAttribute("target", _libdir+"/"+libName);
								doc.getDocumentElement().appendChild(symlinkElement);
								try{
									getProject().log(this, "create symbolic link " + new java.io.File(target.getParentFile(), shortName) + " -> " + libName, Project.MSG_INFO);
									Files.createSymbolicLink(new java.io.File(target.getParentFile(), shortName).toPath(), Path.of(target.getName()));
								}catch(java.nio.file.FileAlreadyExistsException ex) {
								} catch (Exception ex) {
									ex.printStackTrace();
								}
							}
							//fallthrough:
						default:
							symlinkElement = doc.createElement("symlink");
							shortName = formatQtJambiName(name, debug, qtMajorVersion, -1, -1);
							symlinkElement.setAttribute("name", _libdir+"/"+shortName);
							symlinkElement.setAttribute("target", _libdir+"/"+libName);
							doc.getDocumentElement().appendChild(symlinkElement);
							try{
								getProject().log(this, "create symbolic link " + new java.io.File(target.getParentFile(), shortName) + " -> " + libName, Project.MSG_INFO);
								Files.createSymbolicLink(new java.io.File(target.getParentFile(), shortName).toPath(), Path.of(target.getName()));
							}catch(java.nio.file.FileAlreadyExistsException ex) {
							} catch (Exception ex) {
								ex.printStackTrace();
							}
							symlinkElement = doc.createElement("symlink");
							shortName = formatQtJambiName(name, debug, qtMajorVersion, qtMinorVersion, -1);
							symlinkElement.setAttribute("name", _libdir+"/"+shortName);
							symlinkElement.setAttribute("target", _libdir+"/"+libName);
							doc.getDocumentElement().appendChild(symlinkElement);
							try{
								getProject().log(this, "create symbolic link " + new java.io.File(target.getParentFile(), shortName) + " -> " + libName, Project.MSG_INFO);
								Files.createSymbolicLink(new java.io.File(target.getParentFile(), shortName).toPath(), Path.of(target.getName()));
							}catch(java.nio.file.FileAlreadyExistsException ex) {
							} catch (Exception ex) {
								ex.printStackTrace();
							}
							break;
						}
					}
				}
			}
			
			java.io.File deploymentFile = new java.io.File(directory, "qtjambi-deployment.xml");
        	deploymentFile.delete();
			boolean isUtilities = false;
			switch(moduleName) {
			case "qtjambi.deployer":
				deploymentFile = new java.io.File(directory, "qtjambi-utilities.xml");
				deploymentFile.delete();
				isUtilities = true;
				switch(OSInfo.crossOS()) {
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
							libraryIncludes += "," + deploymentFile.getName();
							try(FileOutputStream fos = new FileOutputStream(deploymentFile)){
								StreamResult result = new StreamResult(fos);
								transformer.transform(new DOMSource(doc), result);
							}
						}else if(!isUtilities){
							System.out.println("Skipping qtjambi-deployment.xml for android bundle.");
						}
					}else {
						System.out.println("Skipping qtjambi-deployment.xml: no entries.");
					}
        	}
		}catch(RuntimeException | Error e){
			throw e;
		}catch(Exception e){
			throw new BuildException(e);
		}finally {
//			System.out.println("library.includes="+libraryIncludes);
			getProject().setProperty("library.includes", libraryIncludes);
		}
	}
	
	private String libraries;
	private String moduleName;
	private String outputDirectory;
	private boolean debug = false;
	private boolean plugin = false;
    
    static void copySubdirs(File root, File srcDir, File destDir, boolean debug, List<Map.Entry<String,Boolean>> additionalFiles, boolean allowHeaders) throws IOException {
    	if(srcDir.isDirectory()) {
	    	for(File content : srcDir.listFiles()) {
	    		if((debug || (!content.getName().endsWith(".prl") && !content.getName().endsWith(".dSYM")))
	    				&& (allowHeaders || !content.getName().equals("Headers"))) {
	    			if(!Files.isSymbolicLink(content.toPath())){
	    				if(content.isDirectory()) {
	        				copySubdirs(root, new File(srcDir, content.getName()), new File(destDir, content.getName()), debug, additionalFiles, allowHeaders);
	        			}else {
		    				File destFile = new File(destDir, content.getName());
		    				destDir.mkdirs();
		    				Util.copy(content, destFile);
		    				if(content.canExecute())
			                	destFile.setExecutable(true);
		    				additionalFiles.add(new SimpleEntry<>(root.toPath().relativize(content.toPath()).toString(), content.canExecute()));
	        			}
	    			}
	    		}
	    	}
    	}else {
    		System.err.println("cannot copy directory "+srcDir);
    	}
    }
    
    static void symlinkSubdirs(File root, File srcDir, File destDir, boolean debug, List<Map.Entry<String,String>> symlinks) throws IOException {
    	if(srcDir.isDirectory()) {
	    	for(File content : srcDir.listFiles()) {
	    		if((debug || (!content.getName().endsWith(".prl") && !content.getName().endsWith(".dSYM")))
	    				//&& !content.getName().equals("Headers")
    				) {
	    			if(Files.isSymbolicLink(content.toPath())){
	    				File destFile = new File(destDir, content.getName());
	    				Path target = null;
	    				try {
	    					target = Files.readSymbolicLink(content.toPath());
	    					if(target.isAbsolute())
	    						target = srcDir.toPath().relativize(target);
	    					if(!Files.exists(destFile.toPath())) {
	    						try{
	    							Files.createSymbolicLink(destFile.toPath(), target);
	    						}catch(java.nio.file.FileAlreadyExistsException e) {}
	    					}else if(!Files.isSymbolicLink(destFile.toPath())){
	    						destFile.delete();
	    						try{
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

    public static String formatPluginName(String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if(debug) {
            switch(OSInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14){
                    return "lib" + name + "_debug.dylib";
                }else{
                    return "lib" + name + ".dylib";
                }
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
            case Linux:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + "_debug.a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
            case Linux:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

	public static String formatQmlPluginName(String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if (debug) {
            switch (OSInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS: 
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + "_debug.dylib";
                else return "lib" + name + ".dylib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
            case Linux: return "lib" + name + ".so";
            case IOS: return "lib" + name + "_debug.a";
			default:
				break;
            }
        } else {
            switch (OSInfo.crossOS()) {
            case Windows: return name + ".dll";
            case MacOS: return "lib" + name + ".dylib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
            case Linux: return "lib" + name + ".so";
            case IOS: return "lib" + name + ".a";
			default:
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
    
    public static String formatQtName(String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
    	return formatQtName(name, infix, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion, false);
    }

    public static String formatQtName(String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion, boolean staticLib) {
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
        	switch(OSInfo.crossOS()) {
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
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + infix + "d" + tmpVersionString + ".dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14){
                    return libPrefix + name + infix + "_debug" + tmpDotVersionString + ".dylib";
                }else{
                    return libPrefix + name + infix + tmpDotVersionString + ".dylib";
                }
            case Linux:
                return libPrefix + name + infix + ".so" + tmpDotVersionString;
            case Android:
            	if(infix==null || infix.isEmpty()) {
	            	switch(OSInfo.crossOSArchName()) {
	            	case OSInfo.K_ANDROID_ARM32:
	            		return libPrefix + name + infix + "_armeabi-v7a.so";
	            	case OSInfo.K_ANDROID_ARM64:
	            		return libPrefix + name + infix + "_arm64-v8a.so";
	            	case OSInfo.K_ANDROID_X86:
	            		return libPrefix + name + infix + "_x86.so";
	            	case OSInfo.K_ANDROID_X64:
	            		return libPrefix + name + infix + "_x86_64.so";
	            	}
            	}
                return libPrefix + name + infix + ".so";
            case IOS: return "lib" + name + infix + "_debug.a";
			default:
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
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + infix + tmpVersionString + ".dll";
            case MacOS:
                return libPrefix + name + infix + tmpDotVersionString + ".dylib";
            case Linux:
                return libPrefix + name + infix + ".so" + tmpDotVersionString;
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return libPrefix + name + infix + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return libPrefix + name + infix + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return libPrefix + name + infix + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return libPrefix + name + infix + "_x86_64.so";
            	}
                return libPrefix + name + infix + ".so";
            case IOS: return "lib" + name + infix + ".a";
			default:
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

    public static String formatQtPrlName(String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
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
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + infix + "d" + ".prl";
            case MacOS:
            case IOS:
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + infix + ".prl";
                else return "lib" + name + infix + tmpDebugSuffix + ".prl";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + infix + "_armeabi-v7a.prl";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + infix + "_arm64-v8a.prl";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + infix + "_x86.prl";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + infix + "_x86_64.prl";
            	}
            case Linux:
                return "lib" + name + infix + ".prl";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + ".prl";
            case MacOS:
            case IOS:
                return "lib" + name + infix + ".prl";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + infix + "_armeabi-v7a.prl";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + infix + "_arm64-v8a.prl";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + infix + "_x86.prl";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + infix + "_x86_64.prl";
            	}
            case Linux:
                return "lib" + name + infix + ".prl";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatUnversionedPluginName(String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if(debug) {
            String tmpDebugSuffix = "_debug";
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + "d.dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + tmpDebugSuffix + ".dylib";
                else return "lib" + name + ".dylib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
            case Linux:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + ".a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
            case Linux:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }
    
    public static String formatQtJambiName(String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
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
            switch(OSInfo.crossOS()) {
            case Windows:
        		return name + "d" + tmpVersionString + ".dll";
            case MacOS:
        		return "lib" + name + tmpDebugSuffix + tmpDotVersionString + ".jnilib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + tmpDebugSuffix + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + tmpDebugSuffix + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + tmpDebugSuffix + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + tmpDebugSuffix + "_x86_64.so";
            	}
                return "lib" + name + tmpDebugSuffix + ".so";
            case Linux:
                return "lib" + name + tmpDebugSuffix + ".so" + tmpDotVersionString;
            case IOS: return "lib" + name + tmpDebugSuffix + ".a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
        		return name + tmpVersionString + ".dll";
            case MacOS:
        		return "lib" + name + tmpDotVersionString + ".jnilib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
                return "lib" + name + ".so";
            case Linux:
                return "lib" + name + ".so" + tmpDotVersionString;
            case IOS: return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatQtJambiPluginName(String name, boolean debug, String versionString) {
    	if(debug) {
            switch(OSInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS:
                return "lib" + name + "_debug.dylib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
            case Linux:
                return "lib" + name + "_debug.so";
            case IOS:
                return "lib" + name + "_debug.a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            	switch(OSInfo.crossOSArchName()) {
            	case OSInfo.K_ANDROID_ARM32:
            		return "lib" + name + "_armeabi-v7a.so";
            	case OSInfo.K_ANDROID_ARM64:
            		return "lib" + name + "_arm64-v8a.so";
            	case OSInfo.K_ANDROID_X86:
            		return "lib" + name + "_x86.so";
            	case OSInfo.K_ANDROID_X64:
            		return "lib" + name + "_x86_64.so";
            	}
            case Linux:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatQtJambiQmlPluginName(String name, boolean debug, String versionString) {
//        String tmpDotVersionString = (versionString != null && versionString.length() > 0) ? "." + versionString : "";
         if(debug) {
            String tmpDebugSuffix = "_debug";
             switch(OSInfo.crossOS()) {
             case Windows:
        		 return name + "d.dll";
             case MacOS:
                return "lib" + name + tmpDebugSuffix + ".dylib";
             case Android:
             	switch(OSInfo.crossOSArchName()) {
             	case OSInfo.K_ANDROID_ARM32:
             		return "lib" + name + "_armeabi-v7a.so";
             	case OSInfo.K_ANDROID_ARM64:
             		return "lib" + name + "_arm64-v8a.so";
             	case OSInfo.K_ANDROID_X86:
             		return "lib" + name + "_x86.so";
             	case OSInfo.K_ANDROID_X64:
             		return "lib" + name + "_x86_64.so";
             	}
             case Linux:
                return "lib" + name + tmpDebugSuffix + ".so";
             case IOS: return "lib" + name + tmpDebugSuffix + ".a";
 			default:
				break;
             }
         } else {
             switch(OSInfo.crossOS()) {
             case Windows:
        		 return name + ".dll";
             case MacOS:
                return "lib" + name + ".dylib";
             case Android:
             	switch(OSInfo.crossOSArchName()) {
             	case OSInfo.K_ANDROID_ARM32:
             		return "lib" + name + "_armeabi-v7a.so";
             	case OSInfo.K_ANDROID_ARM64:
             		return "lib" + name + "_arm64-v8a.so";
             	case OSInfo.K_ANDROID_X86:
             		return "lib" + name + "_x86.so";
             	case OSInfo.K_ANDROID_X64:
             		return "lib" + name + "_x86_64.so";
             	}
             case Linux:
                return "lib" + name + ".so";
             case IOS: return "lib" + name + ".a";
 			default:
				break;
             }
         }
         throw new BuildException("unhandled case...");
    }
}
