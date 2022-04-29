package io.qt.tools.ant;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class ForeachVersionTask extends Task {

	@Override
	public void execute() throws BuildException {
		PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
		String dirs = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdirs");
		propertyHelper.setNewProperty("qtjambi.build.module", "");
		propertyHelper.setNewProperty("qtjambi.autotests", "");
		propertyHelper.setNewProperty("qtjambi.autotests.jnicheck", "");
		if(dirs==null)
			dirs = AntUtil.getPropertyAsString(propertyHelper, "QTDIRS");
		if(dirs==null)
			dirs = System.getenv("QTDIRS");
		String generatorInclPaths = AntUtil.getPropertyAsString(propertyHelper, Constants.GENERATOR_INCLUDEPATHS);
		if(dirs!=null && !dirs.isEmpty()) {
			for(String dir : dirs.split("\\"+java.io.File.pathSeparator)) {
				AntUtil.setProperty(propertyHelper, "QTDIR", dir);
				AntUtil.setProperty(propertyHelper, "qtjambi.qtdir", dir);
				for(String target : targets)
					this.getProject().executeTarget(target);
			}
		}else {
			String base = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtbase");
			if(base==null)
				base = AntUtil.getPropertyAsString(propertyHelper, "QTBASE");
			if(base==null)
				base = System.getenv("QTBASE");
			if(base!=null) {
				java.io.File qtBaseDir = new java.io.File(base);
				if(qtBaseDir.isDirectory()) {
					String versions = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtversions");
					if(versions==null)
						versions = AntUtil.getPropertyAsString(propertyHelper, "QTVERSIONS");
					if(versions==null)
						versions = System.getenv("QTVERSIONS");
					FindCompiler finder = new FindCompiler(propertyHelper, this);
					FindCompiler.Compiler compiler = finder.decideCompiler();
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
						Set<String> propertyNames = new TreeSet<>(getProject().getProperties().keySet());
//						propertyNames.forEach(a->{
//							if(a.startsWith("ant."))
//								System.out.println(a+"="+getProject().getProperty(a));
//						});
//						propertyNames.forEach(System.out::println);
						for (String v : _versions) {
							if(!v.isEmpty()) {
								String[] _v = v.split("\\.");
								if(_v.length>=2) {
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
										java.io.File qtDir = null;
										switch(OSInfo.crossOS()) {
										case Android:
											qtDir = new java.io.File(versionDir, "android");
											break;
										case IOS:
											qtDir = new java.io.File(versionDir, "ios");
											break;
										case Linux:
											qtDir = new java.io.File(versionDir, "gcc_64");
											break;
										case MacOS:
											if(iVersion[0]<6) {
												qtDir = new java.io.File(versionDir, "clang_64");
											}else {
												qtDir = new java.io.File(versionDir, "macos");
											}
											break;
										case Windows:
											switch(compiler) {
											case MinGW_W64:
												qtDir = new java.io.File(versionDir, "mingw_64");
												break;
											default:
												qtDir = new java.io.File(versionDir, "msvc2019_64");
												break;
											}
											break;
										default:
											break;
										}
										if(qtDir!=null && qtDir.isDirectory()) {
											/*
											Hashtable<String, Object> prp = getProject().getProperties();
											for(String prpKey : prp.keySet()) {
												if(!propertyNames.contains(prpKey)
//												      && ( prp.get(prpKey).toString().startsWith(qtBaseDir.getAbsolutePath())
//														|| prpKey.startsWith("tools.qt.")
//														|| prpKey.contains("qtjambi.")
//														|| prpKey.contains("version"))
												      ) {
													propertyHelper.setProperty(prpKey, "", true);
												}
											}
											String dir = qtDir.getAbsolutePath();
											AntUtil.setProperty(propertyHelper, Constants.GENERATOR_INCLUDEPATHS, generatorInclPaths);
											AntUtil.setProperty(propertyHelper, "QTDIR", dir);
											AntUtil.setProperty(propertyHelper, "qtjambi.qtdir", dir);
											for(String target : targets) {
												getProject().executeTarget(target);
											}
											*/
											String dir = qtDir.getAbsolutePath();
											AntUtil.setProperty(propertyHelper, "QTDIR", dir);
											AntUtil.setProperty(propertyHelper, "qtjambi.qtdir", dir);
											for(String target : targets) {
												AntUtil.setProperty(propertyHelper, "proxy.target", target);
												getProject().executeTarget("targetproxy");
											}
										}
									}
								}
							}
						}
//						throw new Error();
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
