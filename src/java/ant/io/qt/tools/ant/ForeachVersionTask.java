package io.qt.tools.ant;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.TreeSet;

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
		boolean takeDefaultQt = false;
		String qmakePath = AntUtil.getPropertyAsString(propertyHelper, "qmake");
		if(qmakePath!=null && !qmakePath.isEmpty()) {
			if(ndk!=null && !ndk.isEmpty())
				AntUtil.setProperty(propertyHelper, "qtjambi.android.ndk", ndk);
			for(String target : targets)
				this.getProject().executeTarget(target);
			return;
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
					AntUtil.setProperty(propertyHelper, "proxy.target", target);
					OSInfo.setQMakeXSpec(null);
					InitializeBuildTask.setTargetProperties(specProperties);
					getProject().executeTarget("targetproxy");
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
					if(base==null) {
						takeDefaultQt = true;
					}
				}
			}
			if(takeDefaultQt) {
				List<java.io.File> qmakes = new ArrayList<>();
				java.io.File qmake;
				switch(OSInfo.os()) {
				case MacOS:
				case Linux:
					qmake = Util.LOCATE_EXEC("qmake", "/usr/bin", "");
					if(qmake.exists())
						qmakes.add(qmake);
					qmake = Util.LOCATE_EXEC("qmake5", "/usr/bin", "");
					if(qmake.exists())
						qmakes.add(qmake);
					qmake = Util.LOCATE_EXEC("qmake-qt5", "/usr/bin", "");
					if(qmake.exists())
						qmakes.add(qmake);
					qmake = Util.LOCATE_EXEC("qmake6", "/usr/bin", "");
					if(qmake.exists())
						qmakes.add(qmake);
					qmake = Util.LOCATE_EXEC("qmake-qt6", "/usr/bin", "");
					if(qmake.exists())
						qmakes.add(qmake);
					break;
				case Windows:
					qmake = Util.LOCATE_EXEC("qmake.exe", "", "");
					if(qmake.exists()) {
						qmakes.add(qmake);
					}else {
						qmake = Util.LOCATE_EXEC("qmake.bat", "", "");
						if(qmake.exists())
							qmakes.add(qmake);
					}
					break;
					default:
						break;
				}
				if(qmakes.isEmpty()) {
					// to cause an error...
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
					FindCompiler.Compiler compiler = null;
					FindCompiler finder = new FindCompiler(propertyHelper, this);
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
										if(!versionStrings.contains(versionString))
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
						}
						for(int[] iVersion : sortedVersions) {
							Properties versionProperties = (Properties)targetProperties.clone();
							java.io.File versionDir = new java.io.File(qtBaseDir, String.format("%1$s.%2$s.%3$s", iVersion[0], iVersion[1], iVersion[2]));
							if(versionDir.isDirectory()) {
								List<java.io.File> qtDirs = new ArrayList<>();
								String toolsBin = null;
								String toolsLib = null;
								String qmake = null;
								
								if(!"only".equalsIgnoreCase(createAndroid)) {
									switch(OSInfo.crossOS()) {
									case IOS:
										qtDirs.add(new java.io.File(versionDir, "ios"));
										break;
									case Linux:
										qtDirs.add(new java.io.File(versionDir, "gcc_64"));
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
								
								if("true".equalsIgnoreCase(createAndroid) || "only".equalsIgnoreCase(createAndroid)) {
									for(java.io.File adir : versionDir.listFiles()) {
										if(adir.getName().toLowerCase().startsWith("android") && adir.isDirectory()) {
											qtDirs.add(adir);
										}
									}
									java.io.File qtDir = null;
									switch(OSInfo.os()) {
									case MacOS:
										if(iVersion[0]<6) {
											qtDir = new java.io.File(versionDir, "clang_64");
										}else {
											qtDir = new java.io.File(versionDir, "macos");
										}
										qmake = "qmake";
										break;
									case Linux:
										qtDir = new java.io.File(versionDir, "gcc_64");
										qmake = "qmake";
										break;
									case Windows:
										if(compiler==null) {
											qtDir = new java.io.File(versionDir, "msvc2019_64");
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
											if(qtDir.getName().toLowerCase().startsWith("android")) {
												if(ndk!=null && !ndk.isEmpty())
													specProperties.setProperty("qtjambi.android.ndk", ndk);
											}
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
