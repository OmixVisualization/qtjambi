package io.qt.tools.ant;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;

public class ForeachNativeTask extends Task{
	@Override
	public void execute() throws BuildException {
		java.io.File dir = new java.io.File(this.dir);
		if(dir.isDirectory()) {
			String prefix = module + "-native-";
			String suffix = "-" + version + ".jar";
			for(String file : dir.list()) {
				if(file.startsWith(prefix) && file.endsWith(suffix)) {
					String platform = file.substring(prefix.length(), file.length() - suffix.length());
					getProject().setProperty("native-spec", platform);
					getProject().executeTarget(target);
				}
			}
		}else {
			throw new BuildException(this.dir + " is not a directory");
		}
	}
	
	private String dir;
	private String module;
	private String version;
	private String target;
	
	public void setDir(String dir) {
		this.dir = dir;
	}
	public void setModule(String module) {
		this.module = module;
	}
	public void setVersion(String version) {
		this.version = version;
	}
	public void setTarget(String target) {
		this.target = target;
	}
}
