package io.qt.tools.ant;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;

public class PropertyCopyTask extends Task{
	@Override
	public void execute() throws BuildException {
        String sourceName = getProject().replaceProperties(from);
        String value = getProject().getProperty(sourceName);
        if (value == null) {
            value = defaultValue;
        }
        if (value != null) {
            if (override) {
                getProject().setProperty(name, value);
            } else {
                getProject().setNewProperty(name, value);
            }
        }
	}
	
	private String name;
	private String from;
	private String defaultValue;
    private boolean override = false;
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getFrom() {
		return from;
	}
	public void setFrom(String from) {
		this.from = from;
	}
	public String getDefaultValue() {
		return defaultValue;
	}
	public void setDefaultValue(String defaultValue) {
		this.defaultValue = defaultValue;
	}
	public boolean isOverride() {
		return override;
	}
	public void setOverride(boolean override) {
		this.override = override;
	}
}
