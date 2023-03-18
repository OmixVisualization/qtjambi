package io.qt.tools.ant;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

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
		boolean maybeAndroid = ndk!=null 
				&& !ndk.isEmpty() 
				&& new java.io.File(ndk).isDirectory();
		boolean takeDefaultQt = false;
		String qmakePath = AntUtil.getPropertyAsString(propertyHelper, "qmake");
		if(qmakePath!=null && !qmakePath.isEmpty()) {
			if(ndk!=null)
				AntUtil.setProperty(propertyHelper, "qtjambi.android.ndk", ndk);
			for(String target : targets)
				this.getProject().executeTarget(target);
			return;
		}
		if(dirs==null)
			dirs = AntUtil.getPropertyAsString(propertyHelper, "QTDIRS");
		if(dirs==null)
			dirs = System.getenv("QTDIRS");
		if(dirs!=null && !dirs.isEmpty()) {
			for(String dir : dirs.split("\\"+java.io.File.pathSeparator)) {
				Properties specProperties = (Properties)targetProperties.clone();
				specProperties.setProperty("QTDIR", dir);
				specProperties.setProperty("qtjambi.qtdir", dir);
				if(ndk!=null)
					specProperties.setProperty("qtjambi.android.ndk", ndk);
				for(String target : targets) {
					AntUtil.setProperty(propertyHelper, "proxy.target", target);
					OSInfo.setQMakeXSpec(null);
					InitializeBuildTask.setTargetProperties(specProperties);
					getProject().executeTarget("targetproxy");
				}
			}
		}else {
			String base = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtbase");
			if(base==null)
				base = AntUtil.getPropertyAsString(propertyHelper, "QTBASE");
			if(base==null)
				base = System.getenv("QTBASE");
			if(base==null) {
				String QTDIR = AntUtil.getPropertyAsString(propertyHelper, "QTDIR"); // used here
				if (QTDIR == null || QTDIR.isEmpty()) {
					QTDIR = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir");
				}
				if (QTDIR == null || QTDIR.isEmpty()) {
					QTDIR = System.getenv("QTDIR");
				}
				takeDefaultQt = OSInfo.os()==OSInfo.OS.Linux && "default".equals(QTDIR);
				if (!takeDefaultQt && (QTDIR == null || QTDIR.isEmpty() || !new java.io.File(QTDIR).exists())) {
					if(OSInfo.os()==OSInfo.OS.Windows) {
						base = "C:\\Qt";
						if(base==null || !new java.io.File(base).isDirectory()) {
							base = null;
						}
					}else if(OSInfo.os()==OSInfo.OS.MacOS) {
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
					}else if(OSInfo.os()==OSInfo.OS.Linux) {
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
					if(OSInfo.os()==OSInfo.OS.Linux && base==null) {
						takeDefaultQt = true;
					}
				}
			}
			if(takeDefaultQt) {
				List<java.io.File> qmakes = new ArrayList<>();
				java.io.File qmake = new java.io.File("/usr/bin/qmake");
				if(qmake.exists())
					qmakes.add(qmake);
				qmake = new java.io.File("/usr/bin/qmake5");
				if(qmake.exists())
					qmakes.add(qmake);
				qmake = new java.io.File("/usr/bin/qmake-qt5");
				if(qmake.exists())
					qmakes.add(qmake);
				qmake = new java.io.File("/usr/bin/qmake6");
				if(qmake.exists())
					qmakes.add(qmake);
				qmake = new java.io.File("/usr/bin/qmake-qt6");
				if(qmake.exists())
					qmakes.add(qmake);
				if(qmakes.isEmpty()) {
					for(String target : targets)
						this.getProject().executeTarget(target);
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
						Map<String,String> query = QMakeTask.query(this, file.getAbsolutePath());
						if(queries.contains(query))
							continue;
						queries.add(query);
						Properties specProperties = (Properties)versionProperties.clone();
						specProperties.setProperty("qmake", file.getAbsolutePath());
						if(containsTestReport)
							specProperties.setProperty("skip.report", "true");
						InitializeBuildTask.setTargetProperties(specProperties);
						for(String target : targets) {
							if(!target.equals("tests.report")) {
								AntUtil.setProperty(propertyHelper, "proxy.target", target);
								getProject().executeTarget("targetproxy");
							}
						}
						InitializeBuildTask.setTargetProperties(null);
						OSInfo.setQMakeXSpec(null);
						lastProperties = specProperties;
					}
					if(containsTestReport && lastProperties!=null) {
						lastProperties.setProperty("skip.report", "false");
						AntUtil.setProperty(propertyHelper, "proxy.target", "tests.report.impl");
						getProject().executeTarget("targetproxy");
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
					FindCompiler finder = new FindCompiler(propertyHelper, this);
					FindCompiler.Compiler compiler = null;
					try {
						compiler = finder.decideCompiler();
					} catch (BuildException e1) {
					}
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
										if(!versionStrings.contains(versionString))
											versionStrings.add(versionString);
									} catch (NumberFormatException e) {}
								}
							}
						}
						if(!versionStrings.isEmpty())
							versions = String.join(",", versionStrings);
					}
					if(versions!=null) {
						String[] _versions = versions.split("[\\,\\:\\;\\ \\+]");
						
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
						for (String v : _versions) {
							if(!v.isEmpty()) {
								String[] _v = v.split("\\.");
								if(_v.length>=2) {
									Properties versionProperties = (Properties)targetProperties.clone();
									int[] iVersion = new int[3];
									for (int i = 0; i < _v.length && i < 3; i++) {
										iVersion[i] = Integer.parseInt(_v[i]);
									}
									if(_v.length==2) {
										for(java.io.File vdir : qtBaseDir.listFiles()) {
											if(vdir.getName().startsWith(v)) {
												int patchVersion = Integer.parseInt(vdir.getName().substring(v.length()+1));
												if(patchVersion>iVersion[2])
													iVersion[2] = patchVersion;
											}
										}
									}
									java.io.File versionDir = new java.io.File(qtBaseDir, String.format("%1$s.%2$s.%3$s", iVersion[0], iVersion[1], iVersion[2]));
									if(versionDir.isDirectory()) {
										List<java.io.File> qtDirs = new ArrayList<>();
										String toolsBin = null;
										String toolsLib = null;
										String qmake = null;
										
										if(!maybeAndroid || !"true".equalsIgnoreCase(createAndroid)) {
											switch(OSInfo.crossOS()) {
											case IOS:
												qtDirs.add(new java.io.File(versionDir, "ios"));
												break;
											case Linux:
												qtDirs.add(new java.io.File(versionDir, "gcc_64"));
												break;
											case MacOS:
												if(!"true".equalsIgnoreCase(AntUtil.getPropertyAsString(propertyHelper, "ios"))) {
													if(iVersion[0]<6) {
														qtDirs.add(new java.io.File(versionDir, "clang_64"));
													}else {
														qtDirs.add(new java.io.File(versionDir, "macos"));
													}
												}
												if(!"false".equalsIgnoreCase(AntUtil.getPropertyAsString(propertyHelper, "ios"))) {
													qtDirs.add(new java.io.File(versionDir, "ios"));
												}
												break;
											case Windows:
												if(compiler==null) {
													qtDirs.add(new java.io.File(versionDir, "msvc2019_64"));
												}else switch(compiler) {
												case MinGW_W64:
													qtDirs.add(new java.io.File(versionDir, "mingw_64"));
													break;
												default:
													qtDirs.add(new java.io.File(versionDir, "msvc2019_64"));
													break;
												}
												break;
											default:
												break;
											}
										}
										
										if(maybeAndroid && !"false".equalsIgnoreCase(createAndroid)) {
											for(java.io.File adir : versionDir.listFiles()) {
												if(adir.getName().toLowerCase().startsWith("android") && adir.isDirectory()) {
													qtDirs.add(adir);
												}
											}
											java.io.File qtDir = null;
											switch(OSInfo.os()) {
											case MacOS:
												if(!"true".equalsIgnoreCase(AntUtil.getPropertyAsString(propertyHelper, "ios"))) {
													if(iVersion[0]<6) {
														qtDir = new java.io.File(versionDir, "clang_64");
													}else {
														qtDir = new java.io.File(versionDir, "macos");
													}
												}
												if(!"false".equalsIgnoreCase(AntUtil.getPropertyAsString(propertyHelper, "ios"))) {
													qtDirs.add(new java.io.File(versionDir, "ios"));
												}
												qmake = "qmake";
												break;
											case Linux:
												qtDir = new java.io.File(versionDir, "gcc_64");
												qmake = "qmake";
												break;
											case Windows:
												if(compiler==null) {
													qtDirs.add(new java.io.File(versionDir, "msvc2019_64"));
												}else switch(compiler) {
												case MinGW_W64:
													qtDir = new java.io.File(versionDir, "mingw_64");
													break;
												default:
													qtDir = new java.io.File(versionDir, "msvc2019_64");
													break;
												}
												if(iVersion[0]<6) {
													qmake = "qmake.exe";
												}else {
													qmake = "qmake.bat";
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
												if(qtDir.getName().toLowerCase().startsWith("android")
														|| qtDir.getName().equals("ios")) {
													if(qtDir.getName().toLowerCase().startsWith("android"))
														specProperties.setProperty("qtjambi.android.ndk", ndk);
													if(toolsBin!=null && !toolsBin.isEmpty()) {
														specProperties.setProperty(Constants.TOOLS_BINDIR, toolsBin);
														switch(OSInfo.os()) {
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
													if(qmake!=null && !qmake.isEmpty()) {
														specProperties.setProperty(Constants.QMAKE, qmake);
														specProperties.setProperty(Constants.QMAKE_ABSPATH, new java.io.File(new java.io.File(qtDir, "bin"), qmake).getAbsolutePath());
													}
												}
												specProperties.setProperty("QTDIR", dir);
												specProperties.setProperty("qtjambi.qtdir", dir);
												if(containsTestReport)
													specProperties.setProperty("skip.report", "true");
												InitializeBuildTask.setTargetProperties(specProperties);
												for(String target : targets) {
													if(!target.equals("tests.report")) {
														AntUtil.setProperty(propertyHelper, "proxy.target", target);
														getProject().executeTarget("targetproxy");
													}
												}
												InitializeBuildTask.setTargetProperties(null);
												OSInfo.setQMakeXSpec(null);
												versionProperties.setProperty("skip-generate", "true");
												versionProperties.setProperty("skip-java-build", "true");
												lastProperties = specProperties;
											}
										}
									}
								}
							}
						}
						if(containsTestReport && lastProperties!=null) {
							lastProperties.setProperty("skip.report", "false");
							AntUtil.setProperty(propertyHelper, "proxy.target", "tests.report.impl");
							getProject().executeTarget("targetproxy");
						}
						return;
					}
				}else {
					throw new BuildException(base+" is not a directory");
				}
			}
			for(String target : targets)
				this.getProject().executeTarget(target);
		}
	}
	
	private String[] targets;
	
	public void setTargets(String targets) {
		this.targets = targets.split("[[ ]\\,\\;]");
	}
}
