package io.qt.tools.ant;

import java.util.Arrays;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class AidlTask extends Task {
	
	@Override
	public void execute() throws BuildException {
		PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
		String ndk = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.android.ndk");
		String qtdir = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.qtdir");
		java.io.File nkdDir;
		if(ndk!=null 
				&& !ndk.isEmpty() 
				&& (nkdDir = new java.io.File(ndk)).isDirectory()) {
			java.io.File aidlPath = new java.io.File(nkdDir.getParentFile().getParentFile(), "build-tools");
			if(aidlPath.isDirectory()) {
				for(java.io.File dir : aidlPath.listFiles()) {
					if(dir.isDirectory()) {
						aidlPath = new java.io.File(dir, OSInfo.os()==OSInfo.OS.Windows ? "aidl.exe" : "aidl");
						if(aidlPath.exists()) {
							java.io.File frameworkPath = new java.io.File(nkdDir.getParentFile().getParentFile(), "platforms");
							if(frameworkPath.isDirectory()) {
								for(java.io.File fdir : frameworkPath.listFiles()) {
									frameworkPath = new java.io.File(fdir, "framework.aidl");
									if(frameworkPath.exists()) {
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
										break;
									}
								}
							}
							break;
						}
					}
				}
			}
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
