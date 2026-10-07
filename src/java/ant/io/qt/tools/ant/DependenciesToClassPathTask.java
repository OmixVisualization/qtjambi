package io.qt.tools.ant;

import java.util.*;

import org.apache.tools.ant.*;

public class DependenciesToClassPathTask extends Task {
	
	@Override
	public void execute() throws BuildException {
		PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());
		if(dependencies!=null && !dependencies.isEmpty()) {
			List<String> moduleList = new ArrayList<>();
			List<String> jarList = new ArrayList<>();
			List<String> unavailableModules = new ArrayList<>();
			Set<String> skippedModules = new HashSet<>();
			String skipped = AntUtil.getPropertyAsString(props, "skipped.qtjambi.modules");
			if (skipped != null) {
				for (String s : skipped.split(",")) {
					skippedModules.add("qtjambi."+s);
				}
			}
			for(String dep : dependencies.split(",")) {
				dep = dep.trim();
				if(!dep.isEmpty()) {
					moduleList.add(dep);
					if(skippedModules.contains(dep))
						unavailableModules.add(dep);
					dep = dep.replace('.', '-');
					jarList.add(dep + "-" + jarVersion + ".jar");
				}
			}
			if(!unavailableModules.isEmpty()) {
				throw new BuildException("Module "+module+" has unavailable dependencies: "+String.join(", ", unavailableModules));
			}
			dependencies = String.join(",", jarList);
			ThreadedSubantTask.waitForModules(getProject(), moduleList);
		}
		props.setProperty(property, dependencies, true);
	}
	
	private String property;
	private String dependencies;
	private String module;
	public String getModule() {
		return module;
	}
	public void setModule(String module) {
		this.module = module;
	}
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
