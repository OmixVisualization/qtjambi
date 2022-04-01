package io.qt.tools.ant;

import java.util.ArrayList;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class DependenciesToClassPathTask extends Task {
	
	private final static String[] platforms = {/*"-native-windows-x64-", "-native-linux-x64-", "-native-macos-"*/};
	
	@Override
	public void execute() throws BuildException {
		PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());
		if(dependencies!=null && !dependencies.isEmpty()) {
			ArrayList<String> list = new ArrayList<>();
			for(String dep : dependencies.split(",")) {
				dep = dep.trim();
				if(!dep.isEmpty()) {
					dep = dep.replace('.', '-');
					list.add(dep + (ismodulebased ? "-" : "-jre8-") + jarVersion + ".jar");
					if(forManifest) {
						for(String plf : platforms) {
							list.add(dep + plf + jarVersion + ".jar");
						}
					}
				}
			}
			dependencies = String.join(forManifest ? " " : ",", list);
		}
		props.setProperty(property, dependencies, true);
	}
	
	private String property;
	private String dependencies;
	private boolean forManifest;
	public void setDependencies(String dependencies) {
		this.dependencies = dependencies;
	}
	public void setJarVersion(String jarVersion) {
		this.jarVersion = jarVersion;
	}

	private String jarVersion;
	
	public void setProperty(String property) {
		this.property = property;
	}
	
	private boolean ismodulebased;

	public void setIsmodulebased(boolean ismodulebased) {
		this.ismodulebased = ismodulebased;
	}
	public boolean isForManifest() {
		return forManifest;
	}
	public void setForManifest(boolean forManifest) {
		this.forManifest = forManifest;
	}
}
