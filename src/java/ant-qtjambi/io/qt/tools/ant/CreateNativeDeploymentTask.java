package io.qt.tools.ant;

import static io.qt.tools.ant.PlatformJarTask.getOtoolOut;

import java.io.FileOutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

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

import io.qt.tools.ant.OSInfo.OS;
import io.qt.tools.ant.PlatformJarTask.OToolOut;

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
    	String libraryIncludes = "";
    	java.io.File targetLibDir = null;
        List<String> _libraries = Collections.emptyList();
        int qtMajorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR));
        int qtMinorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MINOR));
//        int qtPatchlevelVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_PATCHLEVEL));
        int qtjambiPatchlevelVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, "qtjambi.patchversion"));
		try {
			java.io.File directory = new java.io.File(outputDirectory);
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
			switch(OSInfo.crossOS()) {
			case Windows:
				libdir = "bin";
				break;
			default:
				libdir = "lib";
				break;
			}
			targetLibDir = new java.io.File(directory, libdir);
			if(OSInfo.crossOS()==OS.MacOS) {
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
			}
			String debugLib1 = debug ? "_debug" : "";
			String debugQtLib = debugLib1;
	        if(qtMajorVersion==5 && qtMinorVersion>=14){
	        	debugQtLib = "";
	        }
	        String qtlibdir = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qt.libdir");
	        String[] libraries = {/*"Network", "Core", "Gui", "Widgets", "Quick", "Sql", "Xml", "Qml", "QmlModels"*/};
	        
        	switch(moduleName) {
			case "qtjambi.deployer":
        		_libraries = Arrays.asList("QtJambiLauncher", "qtjambiplugin", "jarimport");
        		targetLibDir = new java.io.File(directory, "utilities");
				break;
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
				boolean isMacExecutable = false;
				String _libdir = libdir;
				String jarpath = libdir;
				switch(moduleName) {
				case "qtjambi.deployer":
					jarpath = "utilities";
					if("QtJambiLauncher".equals(name)) {
						_libdir = "bin";
						switch(OSInfo.crossOS()) {
						case MacOS:
							libName = name + (debug ? "_debug.app" : ".app");
							isMacExecutable = true;
							break;
						case Windows:
							libName = name + (debug ? "d.exe" : ".exe");
							break;
						default:
							libName = debug ? name + "_debug" : name;
							break;
						}
					}else {
						libName = LibraryEntry.formatQtJambiQmlPluginName(name, debug, "");
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
					}else {
						libName = LibraryEntry.formatQtJambiJniName(name, debug, qtMajorVersion, qtMinorVersion, qtjambiPatchlevelVersion);
					}
					break;
				}
				if(!libraryIncludes.isEmpty())
					libraryIncludes += ",";
				libraryIncludes += jarpath + "/" + libName;
				if(isMacExecutable) {
					libraryIncludes += "/Contents/MacOS/QtJambiLauncher";
					if(debug)
						libraryIncludes += "_debug";
					libraryIncludes += "," + jarpath + "/" + libName + "/Contents/Resources/empty.lproj";
					libraryIncludes += "," + jarpath + "/" + libName + "/Contents/Info.plist";
					libraryIncludes += "," + jarpath + "/" + libName + "/Contents/PkgInfo";
				}
				java.io.File libFile = new java.io.File(new java.io.File(builddir, _libdir), libName);
				java.io.File target = new java.io.File(targetLibDir, libName);
				if(isMacExecutable) {
					List<String> additionalFiles = new ArrayList<>();
					List<Map.Entry<String,String>> symlinks = new ArrayList<>();
					PlatformJarTask.copySubdirs(libFile.getParentFile(), libFile, target, debug, additionalFiles);
					PlatformJarTask.symlinkSubdirs(libFile.getParentFile(), libFile, target, debug, symlinks);
					if(debug) {
						if(!additionalFiles.contains("utilities/QtJambiLauncher_debug.app/Contents/Info.plist")) {
							additionalFiles.add("utilities/QtJambiLauncher_debug.app/Contents/Info.plist");
							Files.copy(new java.io.File(builddir, _libdir+"/QtJambiLauncher.app/Contents/Info.plist").toPath(), 
									new java.io.File(target, "Contents/Info.plist").toPath(), StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
						}
					}
					for(String file : additionalFiles) {
						Element libraryElement = doc.createElement("library");
						libraryElement.setAttribute("name", "utilities/"+file);
						doc.getDocumentElement().appendChild(libraryElement);
			        }
			        
			        for(Map.Entry<String,String> unpack : symlinks) {
						Element libraryElement = doc.createElement("library");
						libraryElement.setAttribute("name", "utilities/"+unpack.getKey());
						libraryElement.setAttribute("target", "utilities/"+unpack.getValue());
						doc.getDocumentElement().appendChild(libraryElement);
			        }
				}else {
					Files.copy(libFile.toPath(), target.toPath(), StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);
				}
				switch(OSInfo.crossOS()) {
				case Linux:
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
							case "qtjambiplugin":
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
					}
					break;
				case MacOS:
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
					break;
				default:
					break;
				}
				if(!isMacExecutable) {
					_libdir = libdir;
					boolean noSymlinks = false;
		        	switch(moduleName) {
					case "qtjambi.deployer":
						_libdir = "utilities";
						noSymlinks = true;
						break;
					default:
						break;
		        	}
					Element libraryElement = doc.createElement("library");
					libraryElement.setAttribute("name", _libdir+"/"+libName);
					doc.getDocumentElement().appendChild(libraryElement);
					String shortName;
					Element symlinkElement;
					if(!noSymlinks) {
						switch(OSInfo.crossOS()) {
						case Windows:
							break;
						case MacOS:
							symlinkElement = doc.createElement("symlink");
							shortName = LibraryEntry.formatQtJambiJniName(name, debug, -1, -1, -1);
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
							//fallthrough:
						default:
							symlinkElement = doc.createElement("symlink");
							shortName = LibraryEntry.formatQtJambiJniName(name, debug, qtMajorVersion, -1, -1);
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
							shortName = LibraryEntry.formatQtJambiJniName(name, debug, qtMajorVersion, qtMinorVersion, -1);
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
        	switch(moduleName) {
			case "qtjambi.deployer":
				deploymentFile = new java.io.File(directory, "qtjambi-utilities.xml");
				default:
				libraryIncludes += "," + deploymentFile.getName();
				Transformer transformer = TransformerFactory.newInstance().newTransformer();
				transformer.setOutputProperty(OutputKeys.INDENT, "yes");
				transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
				transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
				try(FileOutputStream fos = new FileOutputStream(deploymentFile)){
					StreamResult result = new StreamResult(fos);
					transformer.transform(new DOMSource(doc), result);
				}
        	}
		}catch(RuntimeException | Error e){
			throw e;
		}catch(Exception e){
			throw new BuildException(e);
		}finally {
//			System.out.println("library.includes = "+libraryIncludes + " -> " + targetLibDir + " (" + outputDirectory + ")");
			getProject().setProperty("library.includes", libraryIncludes);
		}
	}
	
	private String libraries;
	private String moduleName;
	private String outputDirectory;
	private boolean debug = false;
	private boolean plugin = false;
}
