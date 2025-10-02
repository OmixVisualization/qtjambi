package io.qt.tools.ant;

import java.util.Arrays;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class AidlTask extends Task {
	
	@Override
	public void execute() throws BuildException {
		OSInfo osInfo = OSInfo.instance(getProject());
		PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
		String buildtools = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.android.buildtools").split("\\,")[0];
		String platforms = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.android.platform.dir");
		String qtdir = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir");
		System.out.println("qtjambi.android.buildtools="+buildtools);
		System.out.println("qtjambi.android.platform.dir="+platforms);
		java.io.File buildtoolsDir, platformsDir;
		if(buildtools!=null 
				&& !buildtools.isEmpty() 
				&& (buildtoolsDir = new java.io.File(buildtools)).isDirectory()
				&& platforms!=null 
				&& !platforms.isEmpty() 
				&& (platformsDir = new java.io.File(platforms)).isDirectory()) {
			java.io.File aidlPath = new java.io.File(buildtoolsDir, osInfo.os()==OSInfo.OperationSystem.Windows ? "aidl.exe" : "aidl");
			java.io.File frameworkPath = new java.io.File(platformsDir, "framework.aidl");
			java.io.File f1 = new java.io.File(qtdir, "src/android/java/src/org/kde/necessitas/ministro/IMinistroCallback.aidl");
			java.io.File f2 = new java.io.File(qtdir, "src/android/java/src/org/kde/necessitas/ministro/IMinistro.aidl");
			java.io.File pp = new java.io.File(path, "preprocessed.aidl");
			Exec.execute(this, Arrays.asList(
					aidlPath.getAbsolutePath(), 
					"--preprocess",
					pp.getAbsolutePath(), 
					frameworkPath.getAbsolutePath(),
					f1.getAbsolutePath(),
					f2.getAbsolutePath()
					), new java.io.File(path), getProject(), null, null);
			Exec.execute(this, Arrays.asList(
					aidlPath.getAbsolutePath(), 
					"-o"+new java.io.File(path).getAbsolutePath(), 
					"-I"+new java.io.File(path).getAbsolutePath(), 
					"-p"+pp.getAbsolutePath(),
					f1.getAbsolutePath()
					), new java.io.File(path), getProject(), null, null);
			Exec.execute(this, Arrays.asList(
					aidlPath.getAbsolutePath(), 
					"-o"+new java.io.File(path).getAbsolutePath(), 
					"-I"+new java.io.File(path).getAbsolutePath(), 
					"-p"+pp.getAbsolutePath(),
					f2.getAbsolutePath()
					), new java.io.File(path), getProject(), null, null);
			pp.delete();
		}
	}
	
	private String path;

	public String getPath() {
		return path;
	}

	public void setPath(String path) {
		this.path = path;
	}
}
