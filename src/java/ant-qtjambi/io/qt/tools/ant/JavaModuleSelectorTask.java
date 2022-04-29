package io.qt.tools.ant;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.TreeMap;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class JavaModuleSelectorTask extends Task {
	
	@Override
	public void execute() throws BuildException {
		PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());
		java.io.File sourcedir = new java.io.File(this.sourcedir);
		java.io.File generatordir = new java.io.File(this.generatordir);
		List<String> modules = new ArrayList<>();
		Map<String,List<String>> requirements = new TreeMap<>();
		for(java.io.File subdir : sourcedir.listFiles()) {
			if(subdir.isDirectory()) {
				List<String> requiredModules = new ArrayList<>();
				requirements.put(subdir.getName(), requiredModules);
				java.io.File otherDir = new java.io.File(generatordir, subdir.getName());
				if(otherDir.isDirectory()) {
					java.io.File buildprop = new java.io.File(otherDir, "build.properties");
					if(buildprop.exists()) {
						Properties properties = new Properties();
						try(FileInputStream stream = new FileInputStream(buildprop)){
							properties.load(stream);
						} catch (IOException e) {
						}
						for(String m : properties.getOrDefault("qtjambi.required.modules", "").toString().split(",")) {
							if(!m.isEmpty())
								requiredModules.add(m.trim());
						}
					}
				}
			}
		}
		
		List<String> keys = new ArrayList<>(requirements.keySet());
		while(!keys.isEmpty()) {
			String mod = keys.remove(0);
			List<String> reqs = requirements.get(mod);
			reqs.removeAll(modules);
			if(reqs.isEmpty()) {
				modules.add(mod);
			}else {
				boolean skip = false;
				for(String r : reqs) {
					if(!requirements.containsKey(r)) {
						skip = true;
					}
				}
				if(skip) {
					modules.add(mod);
				}else {
					keys.add(mod);
				}
			}
		}
		
		props.setProperty(property, String.join(",", modules), true);
	}
	
	private String property = "";
	private String sourcedir = "";
	private String generatordir = "";

	public String getProperty() {
		return property;
	}
	public void setProperty(String property) {
		this.property = property;
	}
	public String getSourcedir() {
		return sourcedir;
	}
	public void setSourcedir(String sourcedir) {
		this.sourcedir = sourcedir;
	}
	public String getGeneratordir() {
		return generatordir;
	}
	public void setGeneratordir(String generatordir) {
		this.generatordir = generatordir;
	}
}
