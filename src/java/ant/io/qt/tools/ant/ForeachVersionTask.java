package io.qt.tools.ant;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.TreeSet;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.taskdefs.CallTarget;
import org.apache.tools.ant.taskdefs.Property;

import io.qt.tools.ant.FindCompiler.Compiler;
import io.qt.tools.ant.OSInfo.Architecture;

public class ForeachVersionTask extends Task {

	@Override
	public void execute() throws BuildException {
		Properties targetProperties = new Properties();
		PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
		for (String key : Arrays.asList("java.srcdir", 
				"qtjambi.junit.threads", "qtjambi.junit.timeout", "qtjambi.generate.nullness", 
				"parallel", "qtjambi.build.module", "test",
				"test.enable.criticals", "jnicheck")) {
			String value = AntUtil.getPropertyAsString(propertyHelper, key);
			if (value != null && !value.isEmpty())
				targetProperties.setProperty(key, value);
		}
		
		String dirs = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdirs");
		String ndk = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.android.ndk");
		if(ndk==null || ndk.isEmpty())
			ndk = AntUtil.getPropertyAsString(propertyHelper, "ndk");
		if(ndk==null || ndk.isEmpty())
			ndk = System.getenv("ANDROID_NDK");
		String createAndroid = AntUtil.getPropertyAsString(propertyHelper, "android");
		String crossCompile = AntUtil.getPropertyAsString(propertyHelper, "cross");
		boolean takeDefaultQt = false;
		String qmakePath = AntUtil.getPropertyAsString(propertyHelper, "qmake");
		String conf = AntUtil.getPropertyAsString(propertyHelper, "conf");
		OSInfo osInfo = new OSInfo();
		Set<String> skippedCross = new HashSet<>();
		{
			String skippedConfs = AntUtil.getPropertyAsString(propertyHelper, "skipped.confs");
			if(skippedConfs!=null && !skippedConfs.isEmpty()) {
				skippedCross.addAll(Arrays.asList(skippedConfs.split("[\\,\\:\\;\\ \\+]")));
			}
		}
		if(qmakePath!=null && !qmakePath.isEmpty()) {
			for(String target : targets) {
				CallTarget ct = (CallTarget) getProject().createTask("antcall");
				ct.setOwningTarget(getOwningTarget());
				ct.init();
				ct.bindToOwner(this);
				ct.setInheritAll(false);
				ct.setTarget(target);
				Property param = ct.createParam();
				param.setName("qtjambi.osinfo");
				param.setValue(OSInfo.initializeNewOSInfo().id);
				if(ndk!=null && !ndk.isEmpty()) {
					param = ct.createParam();
					param.setName("qtjambi.android.ndk");
					param.setValue(ndk);
				}
				ct.execute();
			}
			return;
		}
		if(conf!=null && !conf.isEmpty()) {
			Properties javaProperties = new Properties();
			File propertieFile = new File("java.properties");
			if(propertieFile.exists()) {
				try {
					try(FileInputStream fis = new FileInputStream(propertieFile)){
						javaProperties.load(fis);
					}
				} catch (FileNotFoundException e) {
				} catch (IOException e) {
				}
			}
			propertieFile = new File("tests.properties");
			if(propertieFile.exists()) {
				try {
					try(FileInputStream fis = new FileInputStream(propertieFile)){
						javaProperties.load(fis);
					}
				} catch (FileNotFoundException e) {
				} catch (IOException e) {
				}
			}
			List<String> confs = new ArrayList<>(Arrays.asList(conf.split("[\\,\\:\\;\\ \\+]")));
			String versions = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtversions");
			if(versions==null)
				versions = AntUtil.getPropertyAsString(propertyHelper, "qt");
			if(versions==null)
				versions = AntUtil.getPropertyAsString(propertyHelper, "QTVERSION");
			if(versions==null)
				versions = AntUtil.getPropertyAsString(propertyHelper, "QTVERSIONS");
			if(versions==null)
				versions = System.getenv("QTVERSION");
			if(versions==null)
				versions = System.getenv("QTVERSIONS");
			if(versions!=null) {
				List<String> _versions = new ArrayList<>(Arrays.asList(versions.split("[\\,\\:\\;\\ \\+]")));
				Set<String> executedTargets = new HashSet<>();
				for (String cf : confs) {
					for (String version : _versions) {
						for (String target : targets) {
							if(target.startsWith("test") && target.contains("run")) {
								executedTargets.add(target);
								CallTarget ct = (CallTarget) getProject().createTask("antcall");
								ct.setOwningTarget(getOwningTarget());
								ct.init();
								ct.bindToOwner(this);
								ct.setInheritAll(false);
								ct.setTarget(target);
								Property param = ct.createParam();
								param.setName("qtjambi.osinfo");
								OSInfo _osInfo = OSInfo.initializeNewOSInfo();
								param.setValue(_osInfo.id);
								boolean found = false;
								FindCompiler.Compiler compiler = null;
								if(cf.equals("macos")) {
									_osInfo.setCrossCompilation(OSInfo.OperationSystem.MacOS, _osInfo.arch());
									found = true;
									compiler = FindCompiler.Compiler.CLANG;
								}else if(cf.startsWith("android-")) {
									_osInfo.setCrossCompilation(OSInfo.OperationSystem.Android, OSInfo.Architecture.arm64);
									found = true;
									compiler = FindCompiler.Compiler.GCC;
								}else {
									for(OSInfo.OperationSystem os : OSInfo.OperationSystem.values()) {
										String osName = os.name().toLowerCase()+"-";
										if(cf.startsWith(osName)) {
											for(OSInfo.Architecture arch : OSInfo.Architecture.values()) {
												String archName = "-"+arch.name().toLowerCase();
												if(arch==OSInfo.Architecture.x86_64)
													archName = "x64";
												if(cf.endsWith(archName)) {
													found = true;
													_osInfo.setCrossCompilation(os, arch);
													switch (os) {
													case Windows:
														if(cf.contains("llvm")) {
															compiler = FindCompiler.Compiler.LLVM_MinGW_W64;
														}else if(cf.contains("mingw")) {
															compiler = FindCompiler.Compiler.MinGW_W64;
														}else {
															if(arch==OSInfo.Architecture.x86_64)
																compiler = FindCompiler.Compiler.MSVC2022_x64;
															else
																compiler = FindCompiler.Compiler.MSVC2022_arm64;
														}
														break;
													case Linux:
													case NetBSD:
													case OpenBSD:
													case Solaris:
													case FreeBSD:
														compiler = FindCompiler.Compiler.GCC;
														break;
													case MacOS:
													case IOS:
														compiler = FindCompiler.Compiler.CLANG;
														break;
													default:
														break;
													}
												}
											}
										}
									}
								}
								if(found) {
									param = ct.createParam();
									param.setName("qtjambi.test.conf");
									param.setValue(cf);
									param = ct.createParam();
									param.setName(Constants.OSNAME);
									param.setValue(cf);
									param = ct.createParam();
									param.setName("qtjambi.test.version");
									param.setValue(version);
									param = ct.createParam();
									param.setName("skip.report");
									param.setValue("true");
									if(compiler!=null) {
										param = ct.createParam();
										param.setName(Constants.COMPILER);
										param.setValue(compiler.toString());
									}
									if(javaProperties.containsKey("java.home."+cf+"-"+version)) {
										param = ct.createParam();
										param.setName("test.jvm");
										param.setValue(javaProperties.get("java.home."+cf+"-"+version));
									}else if(javaProperties.containsKey("java.home."+cf)) {
										param = ct.createParam();
										param.setName("test.jvm");
										param.setValue(javaProperties.get("java.home."+cf));
									}else {
										String javaHome = AntUtil.getPropertyAsString(propertyHelper, "java.home."+cf+"-"+version);
										if(javaHome==null || javaHome.isEmpty()) {
											javaHome = AntUtil.getPropertyAsString(propertyHelper, "java.home."+cf);
										}
										if(javaHome!=null && !javaHome.isEmpty()) {
											param = ct.createParam();
											param.setName("test.jvm");
											param.setValue(javaHome);
										}
									}
									for(Object key : targetProperties.keySet()) {
										if(targetProperties.get(key)!=null) {
											param = ct.createParam();
											param.setName(key.toString());
											param.setValue(targetProperties.get(key));
										}
									}
									ct.execute();
								}
							}
						}
					}
				}
				targets.removeAll(executedTargets);
				{
					CallTarget ct = (CallTarget) getProject().createTask("antcall");
					ct.setOwningTarget(getOwningTarget());
					ct.init();
					ct.bindToOwner(this);
					ct.setInheritAll(false);
					ct.setTarget("tests.report.impl");
					Property param = ct.createParam();
					param.setName("qtjambi.osinfo");
					param.setValue(OSInfo.initializeNewOSInfo().id);
					param = ct.createParam();
					param.setName("skip.report");
					param.setValue("false");
					for(Object key : targetProperties.keySet()) {
						if(targetProperties.get(key)!=null) {
							param = ct.createParam();
							param.setName(key.toString());
							param.setValue(targetProperties.get(key));
						}
					}
					ct.execute();
				}
				if(targets.isEmpty())
					return;
			}
		}
		if(dirs==null)
			dirs = AntUtil.getPropertyAsString(propertyHelper, "qtdir");
		if(dirs==null)
			dirs = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir");
		if(dirs==null)
			dirs = AntUtil.getPropertyAsString(propertyHelper, "QTDIR");
		if(dirs==null)
			dirs = AntUtil.getPropertyAsString(propertyHelper, "QTDIRS");
		if(dirs==null)
			dirs = System.getenv("QTDIR");
		if(dirs==null)
			dirs = System.getenv("QTDIRS");
		if(dirs!=null && !dirs.isEmpty() && !"default".equals(dirs)) {
			for(String dir : dirs.split("\\"+java.io.File.pathSeparator)) {
				Properties specProperties = (Properties)targetProperties.clone();
				specProperties.setProperty("QTDIR", dir);
				specProperties.setProperty("qtjambi.qtdir", dir);
				if(ndk!=null && !ndk.isEmpty())
					specProperties.setProperty("qtjambi.android.ndk", ndk);
				for(String target : targets) {
					CallTarget ct = (CallTarget) getProject().createTask("antcall");
					ct.setOwningTarget(getOwningTarget());
					ct.init();
					ct.bindToOwner(this);
					ct.setInheritAll(false);
					ct.setTarget(target);
					Property param = ct.createParam();
					param.setName("qtjambi.osinfo");
					param.setValue(OSInfo.initializeNewOSInfo().id);
					for(Object key : specProperties.keySet()) {
						if(specProperties.get(key)!=null) {
							param = ct.createParam();
							param.setName(key.toString());
							param.setValue(specProperties.get(key));
						}
					}
					ct.execute();
				}
			}
		}else {
			String base = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtbase");
			if(base==null)
				base = AntUtil.getPropertyAsString(propertyHelper, "qtbase");
			if(base==null)
				base = AntUtil.getPropertyAsString(propertyHelper, "QTBASE");
			if(base==null)
				base = System.getenv("QTBASE");
			if(base==null) {
				takeDefaultQt = "default".equals(dirs);
				if (!takeDefaultQt) {
					if(osInfo.os()==OSInfo.OperationSystem.Windows) {
						base = "C:\\Qt";
						if(base==null || !new java.io.File(base).isDirectory()) {
							base = null;
						}
					}else if(osInfo.os()==OSInfo.OperationSystem.MacOS) {
						base = "/Library/Qt";
						if(base==null || !new java.io.File(base).isDirectory()) {
							base = "/System/Qt";
						}
						if(base==null || !new java.io.File(base).isDirectory()) {
							base = "/opt/Qt";
						}
						if(base==null || !new java.io.File(base).isDirectory()) {
							base = null;
						}
					}else if(osInfo.os().isUnixLike()) {
						base = "/opt/Qt";
						if(base==null || !new java.io.File(base).isDirectory()) {
							base = null;
						}
					}
					if(base==null || !new java.io.File(base).isDirectory()) {
						base = System.getProperty("user.home")+java.io.File.separator+"Qt";
					}
					if(base==null || !new java.io.File(base).isDirectory()) {
						base = System.getProperty("user.dir")+java.io.File.separator+".."+java.io.File.separator+"Qt";
					}
					if(base==null || !new java.io.File(base).isDirectory()) {
						base = null;
					}
					if(base==null) {
						takeDefaultQt = true;
					}
				}
			}
			if(takeDefaultQt) {
				List<java.io.File> qmakes = new ArrayList<>();
				java.io.File qmake;
				switch(osInfo.os()) {
				case Windows:
					qmake = Util.TRY_LOCATE_EXEC("qmake.exe", "", "");
					if(qmake!=null && qmake.exists()) {
						qmakes.add(qmake);
					}else {
						qmake = Util.TRY_LOCATE_EXEC("qmake.bat", "", "");
						if(qmake!=null && qmake.exists())
							qmakes.add(qmake);
					}
					break;
				default:
					if(!osInfo.os().isUnixLike()) {
						break;
					}
				case MacOS:
					List<String> paths = new ArrayList<>(Arrays.asList(System.getProperty("java.library.path", "").split("\\"+File.pathSeparator)));
					paths.add(0, "/usr/pkg");
					paths.add(0, "/usr/local/bin");
					paths.add(0, "/usr/bin");
					String _paths = String.join(File.pathSeparator, paths);
					qmake = Util.TRY_LOCATE_EXEC("qt6/bin/qmake6", _paths, "");
					if(qmake!=null && qmake.exists()) {
						qmakes.add(qmake);
					}else {
						qmake = Util.TRY_LOCATE_EXEC("qt6/bin/qmake-qt6", _paths, "");
						if(qmake!=null && qmake.exists()) {
							qmakes.add(qmake);
						}else {
							qmake = Util.TRY_LOCATE_EXEC("qt6/bin/qmake", _paths, "");
							if(qmake!=null && qmake.exists()) {
								qmakes.add(qmake);
							}else {
								qmake = Util.TRY_LOCATE_EXEC("qmake-qt6", _paths, "");
								if(qmake!=null && qmake.exists()) {
									qmakes.add(qmake);
								}else {
									qmake = Util.TRY_LOCATE_EXEC("qmake6", _paths, "");
									if(qmake!=null && qmake.exists()) {
										qmakes.add(qmake);
									}
								}
							}
						}
					}
					if(qmakes.isEmpty()) {
						qmake = Util.TRY_LOCATE_EXEC("qmake", "/usr/bin", "");
						if(qmake!=null && qmake.exists())
							qmakes.add(qmake);
					}
					break;
				}
				if(qmakes.isEmpty()) {
					// to cause an error...
					throw new BuildException("Unable to find qmake");
				}else {
					boolean containsTestReport = false;
					if(!"true".equals(AntUtil.getPropertyAsString(propertyHelper, "skip.report"))) {
						for(String target : targets) {
							if((target.startsWith("tests.") && target.contains(".run"))) {
								containsTestReport = true;
								break;
							}
						}
					}
					Properties lastProperties = null;
					Properties versionProperties = (Properties)targetProperties.clone();
					Set<Map<String,String>> queries = new HashSet<>();
					
					for (java.io.File file : qmakes) {
						String absolutePath;
						try {
							java.nio.file.Path path = file.toPath();
							if(Files.isSymbolicLink(path)) {
								path = path.toRealPath();
							}
							absolutePath = path.toFile().getAbsolutePath();
						} catch (Throwable e) {
							absolutePath = file.getAbsolutePath();
						}
						Map<String,String> query = QMakeTask.query(osInfo, this, absolutePath);
						if(queries.contains(query))
							continue;
						queries.add(query);
						Properties specProperties = (Properties)versionProperties.clone();
						specProperties.setProperty("qmake", absolutePath);
						if(containsTestReport)
							specProperties.setProperty("skip.report", "true");
						for(String target : targets) {
							if(!target.equals("tests.report")) {
								CallTarget ct = (CallTarget) getProject().createTask("antcall");
								ct.setOwningTarget(getOwningTarget());
								ct.init();
								ct.bindToOwner(this);
								ct.setInheritAll(false);
								ct.setTarget(target);
								Property param = ct.createParam();
								param.setName("qtjambi.osinfo");
								param.setValue(OSInfo.initializeNewOSInfo().id);
								for(Object key : specProperties.keySet()) {
									if(specProperties.get(key)!=null) {
										param = ct.createParam();
										param.setName(key.toString());
										param.setValue(specProperties.get(key));
									}
								}
								try {
									ct.execute();
								}catch(InitializeBuildTask.VersionSkip s) {}
							}
						}
						lastProperties = specProperties;
					}
					if(containsTestReport && lastProperties!=null) {
						CallTarget ct = (CallTarget) getProject().createTask("antcall");
						ct.setOwningTarget(getOwningTarget());
						ct.init();
						ct.bindToOwner(this);
						ct.setInheritAll(false);
						ct.setTarget("tests.report.impl");
						Property param = ct.createParam();
						param.setName("qtjambi.osinfo");
						param.setValue(OSInfo.initializeNewOSInfo().id);
						lastProperties.setProperty("skip.report", "false");
						for(Object key : lastProperties.keySet()) {
							if(lastProperties.get(key)!=null) {
								param = ct.createParam();
								param.setName(key.toString());
								param.setValue(lastProperties.get(key));
							}
						}
						ct.execute();
					}
					return;
				}
			}else if(base!=null) {
				java.io.File qtBaseDir = new java.io.File(base);
				if(qtBaseDir.isDirectory()) {
					String versions = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtversions");
					if(versions==null)
						versions = AntUtil.getPropertyAsString(propertyHelper, "qt");
					if(versions==null)
						versions = AntUtil.getPropertyAsString(propertyHelper, "QTVERSION");
					if(versions==null)
						versions = AntUtil.getPropertyAsString(propertyHelper, "QTVERSIONS");
					if(versions==null)
						versions = System.getenv("QTVERSION");
					if(versions==null)
						versions = System.getenv("QTVERSIONS");
					FindCompiler.Compiler compiler = null;
					FindCompiler finder = new FindCompiler(propertyHelper, osInfo, this);
					try {
						compiler = finder.decideCompiler();
					} catch (BuildException e1) {
					}
					boolean sortVersions = false;
					if(versions==null) {
						List<String> versionStrings = new ArrayList<>();
						for(java.io.File versionDir : qtBaseDir.listFiles()) {
							if(versionDir.isDirectory()) {
								String[] _v = versionDir.getName().split("\\.");
								if(_v.length==3) {
									try {
										int[] iVersion = new int[3];
										for (int i = 0; i < _v.length && i < 3; i++) {
											iVersion[i] = Integer.parseInt(_v[i]);
										}
										String versionString = String.format("%1$s.%2$s", iVersion[0], iVersion[1]);
										if(!versionStrings.contains(versionString) && !versionString.startsWith("5."))
											versionStrings.add(versionString);
									} catch (NumberFormatException e) {}
								}
							}
						}
						if(!versionStrings.isEmpty())
							versions = String.join(",", versionStrings);
						sortVersions = true;
					}
					if(versions!=null) {
						List<String> _versions = new ArrayList<>(Arrays.asList(versions.split("[\\,\\:\\;\\ \\+]")));
						
						Properties lastProperties = null;
						boolean containsTestReport = false;
						if(!"true".equals(AntUtil.getPropertyAsString(propertyHelper, "skip.report"))) {
							for(String target : targets) {
								if((target.startsWith("tests.") && target.contains(".run"))) {
									containsTestReport = true;
									break;
								}
							}
						}
						Collection<int[]> sortedVersions = sortVersions ? new TreeSet<>(Arrays::compare) : new ArrayList<>();
						while(!_versions.isEmpty()) {
							try {
								String v = _versions.remove(0);
								if(!v.isEmpty()) {
									String[] _v = v.split("\\.");
									if(_v.length>=1) {
										int[] iVersion = new int[3];
										for (int i = 0; i < _v.length && i < 3; i++) {
											iVersion[i] = Integer.parseInt(_v[i]);
										}
										if(_v.length<=2) {
											if(_v.length==1) {
												for(java.io.File vdir : qtBaseDir.listFiles()) {
													if(vdir.getName().startsWith(v+".")) {
														String[] _dirV = vdir.getName().split("\\.");
														if(_dirV.length==3) {
															try {
																Integer.parseInt(_dirV[1]);
																_versions.add(0, v += "." + _dirV[1]);
															} catch (NumberFormatException e) {
															}
														}
													}
												}
												continue;
											}
											iVersion[2] = -1;
											for(java.io.File vdir : qtBaseDir.listFiles()) {
												if(vdir.getName().startsWith(v)) {
													int patchVersion = Integer.parseInt(vdir.getName().substring(v.length()+1));
													if(patchVersion>iVersion[2])
														iVersion[2] = patchVersion;
												}
											}
										}
										sortedVersions.add(iVersion);
									}
								}
							}catch(NumberFormatException e) {
							}
						}
						for(int[] iVersion : sortedVersions) {
							Properties versionProperties = (Properties)targetProperties.clone();
							java.io.File versionDir = new java.io.File(qtBaseDir, String.format("%1$s.%2$s.%3$s", iVersion[0], iVersion[1], iVersion[2]));
							if(versionDir.isDirectory()) {
								Map<java.io.File,String> crossCompileDirs = new HashMap<>();
								List<java.io.File> qtDirs = new ArrayList<>();
								String toolsBin = null;
								String toolsLib = null;
								
								if(!"only".equalsIgnoreCase(createAndroid) && !"only".equalsIgnoreCase(crossCompile)) {
									switch(osInfo.crossOS()) {
									case IOS:
										qtDirs.add(new java.io.File(versionDir, "ios"));
										break;
									case MacOS:
										if(!"only".equalsIgnoreCase(AntUtil.getPropertyAsString(propertyHelper, "ios"))) {
											if(iVersion[0]<6) {
												qtDirs.add(new java.io.File(versionDir, "clang_64"));
											}else {
												qtDirs.add(new java.io.File(versionDir, "macos"));
											}
										}
										if("true".equalsIgnoreCase(AntUtil.getPropertyAsString(propertyHelper, "ios"))) {
											qtDirs.add(new java.io.File(versionDir, "ios"));
										}
										break;
									case Windows:
										if(compiler==null) {
											if(!"true".equalsIgnoreCase(crossCompile) && !"only".equalsIgnoreCase(crossCompile)) {
												java.io.File f = new java.io.File(versionDir, "msvc2022_64");
												if(!f.exists())
													f = new java.io.File(versionDir, "msvc2019_64");
												qtDirs.add(f);
											}
										}else switch(compiler) {
										case MinGW_W64:{
											java.io.File f = new java.io.File(versionDir, "mingw_64");
											qtDirs.add(f);
											if(!f.isDirectory()) {
												f = new java.io.File(versionDir, "mingw81_64");
												qtDirs.add(f);
											}
										}
											break;
										case LLVM_MinGW_W64:{
											java.io.File f = new java.io.File(versionDir, "llvm-mingw_64");
											qtDirs.add(f);
										}
											break;
										default:
											if(compiler.name().endsWith("_arm64")) {
												java.io.File f = new java.io.File(versionDir, "msvc2022_arm64");
												if(!f.exists())
													f = new java.io.File(versionDir, "msvc2019_arm64");
												qtDirs.add(f);
											}else {
												java.io.File f = new java.io.File(versionDir, "msvc2022_64");
												if(!f.exists())
													f = new java.io.File(versionDir, "msvc2019_64");
												qtDirs.add(f);
											}
											break;
										}
										break;
									default:
										if(osInfo.crossOS().isUnixLike()){
											if(osInfo.arch()==Architecture.arm64) {
												qtDirs.add(new java.io.File(versionDir, "gcc_arm64"));
											}else {
												qtDirs.add(new java.io.File(versionDir, "gcc_64"));
											}
										}
										break;
									}
								}
								
								if("true".equalsIgnoreCase(createAndroid) || "only".equalsIgnoreCase(createAndroid)
										|| "true".equalsIgnoreCase(crossCompile) || "only".equalsIgnoreCase(crossCompile) ) {
									if(!"false".equalsIgnoreCase(createAndroid)) {
										String androidAbi = AntUtil.getPropertyAsString(propertyHelper, "abi");
										if("all".equals(androidAbi)){
											for(java.io.File adir : versionDir.listFiles()) {
												if(adir.getName().toLowerCase().startsWith("android") && adir.isDirectory()) {
													qtDirs.add(adir);
													crossCompileDirs.put(adir, "android");
												}
											}
										}else if(androidAbi==null || androidAbi.isEmpty()) {
											androidAbi = "arm64_v8a,x86_64";
										}
										for(String abi : androidAbi.split(",")) {
											if("x64".equals(abi))
												abi = "x86_64";
											else if("arm64".equals(abi) || "aarch64".equals(abi))
												abi = "arm64_v8a";
											java.io.File adir = new java.io.File(versionDir, "android_"+abi);
											if(adir.isDirectory()) {
												qtDirs.add(adir);
												crossCompileDirs.put(adir, "android");
											}
										}
									}
									if("true".equalsIgnoreCase(crossCompile) || "only".equalsIgnoreCase(crossCompile)) {
										java.io.File cdir;
										switch(osInfo.os()) {
										case Windows:
											cdir = new java.io.File(versionDir, "gcc_64");
											if(cdir.isDirectory()) {
												qtDirs.add(cdir);
												crossCompileDirs.put(cdir, "linux-x64");
											}
											cdir = new java.io.File(versionDir, "gcc_arm64");
											if(cdir.isDirectory()) {
												qtDirs.add(cdir);
												crossCompileDirs.put(cdir, "linux-arm64");
											}
											if(compiler!=Compiler.LLVM_MinGW_W64) {
												cdir = new java.io.File(versionDir, "llvm-mingw_64");
												if(cdir.isDirectory()) {
													qtDirs.add(cdir);
													crossCompileDirs.put(cdir, "windows-llvm-mingw-x64");
												}
											}
											if(compiler!=Compiler.MinGW_W64) {
												cdir = new java.io.File(versionDir, "mingw_64");
												if(cdir.isDirectory()) {
													qtDirs.add(cdir);
													crossCompileDirs.put(cdir, "windows-mingw-x64");
												}
											}
											if(compiler==null || compiler==Compiler.MinGW_W64 || compiler==Compiler.LLVM_MinGW_W64) {
												cdir = new java.io.File(versionDir, "msvc2022_arm64");
												if(cdir.isDirectory()) {
													qtDirs.add(cdir);
													crossCompileDirs.put(cdir, "windows-arm64");
												}
												cdir = new java.io.File(versionDir, "msvc2022_64");
												if(cdir.isDirectory()) {
													qtDirs.add(cdir);
													crossCompileDirs.put(cdir, "windows-x64");
												}
											}else {
												switch(osInfo.arch()) {
												case arm64:
													if(compiler!=Compiler.MSVC2022_x64 && compiler!=Compiler.MSVC20XX_x64) {
														cdir = new java.io.File(versionDir, "msvc2022_64");
														if(cdir.isDirectory()) {
															qtDirs.add(cdir);
															crossCompileDirs.put(cdir, "windows-x64");
														}
													}
													break;
												case x86_64:
													if(compiler!=Compiler.MSVC2022_arm64 && compiler!=Compiler.MSVC20XX_arm64) {
														cdir = new java.io.File(versionDir, "msvc2022_arm64");
														if(cdir.isDirectory()) {
															qtDirs.add(cdir);
															crossCompileDirs.put(cdir, "windows-arm64");
														}
													}
													break;
												default:
													break;
												}
											}
											break;
										default:
											if(osInfo.os().isUnixLike()){
												if(osInfo.arch()==Architecture.arm64) {
													cdir = new java.io.File(versionDir, "gcc_64");
													qtDirs.add(cdir);
													crossCompileDirs.put(cdir, "linux-x64");
												}else {
													cdir = new java.io.File(versionDir, "gcc_arm64");
													qtDirs.add(cdir);
													crossCompileDirs.put(cdir, "linux-arm64");
												}
											}
											break;
										}
									}
									java.io.File qtDir = null;
									switch(osInfo.os()) {
									case MacOS:
										if(iVersion[0]<6) {
											qtDir = new java.io.File(versionDir, "clang_64");
										}else {
											qtDir = new java.io.File(versionDir, "macos");
										}
										break;
									case Linux:
										if(osInfo.arch()==Architecture.arm64)
											qtDir = new java.io.File(versionDir, "gcc_arm64");
										else
											qtDir = new java.io.File(versionDir, "gcc_64");
										break;
									case Windows:
										if(compiler==null) {
											qtDir = new java.io.File(versionDir, "msvc2022_64");
											if(!qtDir.exists())
												qtDir = new java.io.File(versionDir, "msvc2019_64");
										}else switch(compiler) {
										case MinGW_W64:
											qtDir = new java.io.File(versionDir, "mingw_64");
											if(!qtDir.isDirectory()) {
												File _qtDir = new java.io.File(versionDir, "mingw81_64");
												if(_qtDir.isDirectory()) {
													qtDir = _qtDir;
												}
											}
											break;
										case LLVM_MinGW_W64:
											qtDir = new java.io.File(versionDir, "llvm-mingw_64");
											break;
										default:
											if(compiler.name().endsWith("_arm64")) {
												qtDir = new java.io.File(versionDir, "msvc2022_arm64");
												if(!qtDir.exists())
													qtDir = new java.io.File(versionDir, "msvc2019_arm64");
											}else {
												qtDir = new java.io.File(versionDir, "msvc2022_64");
												if(!qtDir.exists())
													qtDir = new java.io.File(versionDir, "msvc2019_64");
											}
											break;
										}
									default:
										break;
									}
									if(qtDir!=null) {
										toolsBin = new java.io.File(qtDir, "bin").getAbsolutePath();
										toolsLib = new java.io.File(qtDir, "lib").getAbsolutePath();
									}
								}
								for(java.io.File qtDir : qtDirs) {
									if(qtDir!=null && qtDir.isDirectory()) {
										Properties specProperties = (Properties)versionProperties.clone();
										String dir = qtDir.getAbsolutePath();
										String crossSpec = crossCompileDirs.get(qtDir);
										OSInfo.OperationSystem crossOS = null;
										OSInfo.Architecture crossArch = null;
										if(crossSpec!=null) {
											if(skippedCross.contains(crossSpec))
												continue;
											if(toolsBin!=null && !toolsBin.isEmpty()) {
												specProperties.setProperty(Constants.TOOLS_BINDIR, toolsBin);
												switch(osInfo.os()) {
												case Windows:
													specProperties.setProperty(Constants.TOOLS_QMAKE, "qmake.exe");
													specProperties.setProperty(Constants.TOOLS_QMAKE_ABSPATH, toolsBin+"\\qmake.exe");
													break;
												default:
													specProperties.setProperty(Constants.TOOLS_QMAKE, "qmake");
													specProperties.setProperty(Constants.TOOLS_QMAKE_ABSPATH, toolsBin+"/qmake");
													break;
												}
											}
											if(toolsLib!=null && !toolsLib.isEmpty())
												specProperties.setProperty(Constants.TOOLS_LIBDIR, toolsLib);
											switch(crossSpec) {
											case "android":
												if(ndk!=null && !ndk.isEmpty())
													specProperties.setProperty("qtjambi.android.ndk", ndk);
												crossOS = OSInfo.OperationSystem.Android;
												crossArch = null;
												break;
											default:{
													switch(crossSpec) {
													case "windows-x64":
														specProperties.setProperty("qtjambi.compiler", "msvc2022x64");
														crossOS = OSInfo.OperationSystem.Windows;
														crossArch = OSInfo.Architecture.x86_64;
														break;
													case "windows-arm64":
														specProperties.setProperty("qtjambi.compiler", "msvc2022arm64");
														crossOS = OSInfo.OperationSystem.Windows;
														crossArch = OSInfo.Architecture.arm64;
														break;
													case "windows-llvm-mingw-x64":
														specProperties.setProperty("qtjambi.compiler", "llvm-mingw-w64");
														crossOS = OSInfo.OperationSystem.Windows;
														crossArch = OSInfo.Architecture.x86_64;
														break;
													case "windows-mingw-x64":
														specProperties.setProperty("qtjambi.compiler", "mingw-w64");
														crossOS = OSInfo.OperationSystem.Windows;
														crossArch = OSInfo.Architecture.x86_64;
														break;
													case "linux-x64":
														crossOS = OSInfo.OperationSystem.Linux;
														crossArch = OSInfo.Architecture.x86_64;
														break;
													case "linux-arm64":
														crossOS = OSInfo.OperationSystem.Linux;
														crossArch = OSInfo.Architecture.arm64;
														break;
													case "macos":
														crossOS = OSInfo.OperationSystem.MacOS;
														crossArch = null;
														break;
													}
													String versionStrg = String.format("%1$s.%2$s", iVersion[0], iVersion[1]);
													Properties makeProperties = new Properties();
													File propertieFile = new File("make.properties");
													if(propertieFile.exists()) {
														try {
															try(FileInputStream fis = new FileInputStream(propertieFile)){
																makeProperties.load(fis);
															}
														} catch (FileNotFoundException e) {
														} catch (IOException e) {
														}
													}
													propertieFile = new File("java.properties");
													if(propertieFile.exists()) {
														try {
															try(FileInputStream fis = new FileInputStream(propertieFile)){
																makeProperties.load(fis);
															}
														} catch (FileNotFoundException e) {
														} catch (IOException e) {
														}
													}
													String _qmake;
													if(makeProperties.containsKey("qmake."+crossSpec+"-"+versionStrg)) {
														_qmake = makeProperties.getProperty("qmake."+crossSpec+"-"+versionStrg);
													}else if(makeProperties.containsKey("qmake."+crossSpec)) {
														_qmake = makeProperties.getProperty("qmake."+crossSpec);
													}else {
														_qmake = AntUtil.getPropertyAsString(propertyHelper, "qmake."+crossSpec+"-"+versionStrg);
														if(_qmake==null || _qmake.isEmpty()) {
															_qmake = AntUtil.getPropertyAsString(propertyHelper, "qmake."+crossSpec);
														}
													}
													if(_qmake!=null && !_qmake.isEmpty()) {
														specProperties.setProperty(Constants.QMAKE, _qmake);
														specProperties.setProperty(Constants.QMAKE_ABSPATH, _qmake);
													}
													if(makeProperties.containsKey("make."+crossSpec+"-"+versionStrg)) {
														specProperties.setProperty("make.tool", makeProperties.getProperty("make."+crossSpec+"-"+versionStrg));
													}else if(makeProperties.containsKey("make."+crossSpec)) {
														specProperties.setProperty("make.tool", makeProperties.getProperty("make."+crossSpec));
													}else {
														String make = AntUtil.getPropertyAsString(propertyHelper, "make."+crossSpec+"-"+versionStrg);
														if(make==null || make.isEmpty()) {
															make = AntUtil.getPropertyAsString(propertyHelper, "make."+crossSpec);
														}
														if(make!=null && !make.isEmpty()) {
															specProperties.setProperty("make.tool", make);
														}
													}
													if(makeProperties.containsKey("java.home."+crossSpec+"-"+versionStrg)) {
														specProperties.setProperty("test.jvm", makeProperties.getProperty("java.home."+crossSpec+"-"+versionStrg));
													}else if(makeProperties.containsKey("java.home."+crossSpec)) {
														specProperties.setProperty("test.jvm", makeProperties.getProperty("java.home."+crossSpec));
													}else {
														String javaHome = AntUtil.getPropertyAsString(propertyHelper, "java.home."+crossSpec+"-"+versionStrg);
														if(javaHome==null || javaHome.isEmpty()) {
															javaHome = AntUtil.getPropertyAsString(propertyHelper, "java.home."+crossSpec);
														}
														if(javaHome!=null && !javaHome.isEmpty()) {
															specProperties.setProperty("test.jvm", javaHome);
														}
													}
													if(makeProperties.containsKey("fsroot."+crossSpec+"-"+versionStrg)) {
														specProperties.setProperty("fsroot", makeProperties.getProperty("fsroot."+crossSpec+"-"+versionStrg));
													}else if(makeProperties.containsKey("fsroot."+crossSpec)) {
														specProperties.setProperty("fsroot", makeProperties.getProperty("fsroot."+crossSpec));
													}else {
														String path = AntUtil.getPropertyAsString(propertyHelper, "fsroot."+crossSpec+"-"+versionStrg);
														if(path==null || path.isEmpty()) {
															path = AntUtil.getPropertyAsString(propertyHelper, "fsroot."+crossSpec);
														}
														if(path!=null && !path.isEmpty()) {
															specProperties.setProperty("fsroot", path);
														}
													}
												}
												break;
											}
										}
										switch(osInfo.os()) {
										case Windows:
											if(!new java.io.File(new java.io.File(qtDir, "bin"), "qmake.exe").exists()
													&& !new java.io.File(new java.io.File(qtDir, "bin"), "qmake.bat").exists()) {
												continue;
											}
											break;
										default:
											if(!new java.io.File(new java.io.File(qtDir, "bin"), "qmake").exists()) {
												continue;
											}
											break;
										}
										specProperties.setProperty("QTDIR", dir);
										specProperties.setProperty("qtjambi.qtdir", dir);
										if(containsTestReport)
											specProperties.setProperty("skip.report", "true");
										for(String target : targets) {
											if(!target.equals("tests.report")) {
												CallTarget ct = (CallTarget) getProject().createTask("antcall");
												ct.setOwningTarget(getOwningTarget());
												ct.init();
												ct.bindToOwner(this);
												ct.setInheritAll(false);
												ct.setTarget(target);
												Property param = ct.createParam();
												param.setName("qtjambi.osinfo");
												OSInfo info = OSInfo.initializeNewOSInfo();
												param.setValue(info.id);
												if(crossOS!=null) {
													if(crossArch==null)
														crossArch = info.arch();
													info.setCrossCompilation(crossOS, crossArch);
												}
												for(Object key : specProperties.keySet()) {
													if(specProperties.get(key)!=null) {
														param = ct.createParam();
														param.setName(key.toString());
														param.setValue(specProperties.get(key));
													}
												}
												ct.execute();
											}
										}
										versionProperties.setProperty("skip-generate", "true");
										versionProperties.setProperty("skip-java-build", "true");
										lastProperties = specProperties;
									}
								}
							}
						}
						if(containsTestReport && lastProperties!=null) {
							CallTarget ct = (CallTarget) getProject().createTask("antcall");
							ct.setOwningTarget(getOwningTarget());
							ct.init();
							ct.bindToOwner(this);
							ct.setInheritAll(false);
							ct.setTarget("tests.report.impl");
							lastProperties.setProperty("skip.report", "false");
							Property param = ct.createParam();
							param.setName("qtjambi.osinfo");
							param.setValue(OSInfo.initializeNewOSInfo().id);
							for(Object key : lastProperties.keySet()) {
								if(lastProperties.get(key)!=null) {
									param = ct.createParam();
									param.setName(key.toString());
									param.setValue(lastProperties.get(key));
								}
							}
							ct.execute();
						}
						return;
					}
				}else {
					throw new BuildException(base+" is not a directory");
				}
			}
			for(String target : targets) {
				CallTarget ct = (CallTarget) getProject().createTask("antcall");
				ct.setOwningTarget(getOwningTarget());
				ct.init();
				ct.bindToOwner(this);
				ct.setInheritAll(false);
				ct.setTarget(target);
				Property param = ct.createParam();
				param.setName("qtjambi.osinfo");
				param.setValue(OSInfo.initializeNewOSInfo().id);
				ct.execute();
			}
		}
	}
	
	private List<String> targets = new ArrayList<>();
	
	public void setTargets(String targets) {
		this.targets.clear();
		this.targets.addAll(Arrays.asList(targets.split("[[ ]\\,\\;]")));
	}
}
