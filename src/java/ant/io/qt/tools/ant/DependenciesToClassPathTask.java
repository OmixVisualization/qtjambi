package io.qt.tools.ant;

import java.util.ArrayList;
import java.util.List;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class DependenciesToClassPathTask extends Task {
	
	@Override
	public void execute() throws BuildException {
		PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());
		if(dependencies!=null && !dependencies.isEmpty()) {
			List<String> moduleList = new ArrayList<>();
			List<String> jarList = new ArrayList<>();
			for(String dep : dependencies.split(",")) {
				dep = dep.trim();
				if(!dep.isEmpty()) {
					moduleList.add(dep);
					dep = dep.replace('.', '-');
					jarList.add(dep + "-" + jarVersion + ".jar");
				}
			}
			dependencies = String.join(",", jarList);
			ThreadedSubantTask.waitForModules(getProject(), moduleList);
		}
		props.setProperty(property, dependencies, true);
	}
	
	private String property;
	private String dependencies;
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
}
