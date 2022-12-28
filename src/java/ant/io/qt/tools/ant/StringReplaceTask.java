package io.qt.tools.ant;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class StringReplaceTask extends Task{
	@Override
	public void execute() throws BuildException {
		PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());
		String value = AntUtil.getPropertyAsString(props, property);
		value = value.replace(target, replacement);
		props.setProperty(property, value, true);
	}
	
	private String property;
	private String target;
	public String getTarget() {
		return target;
	}
	public void setTarget(String target) {
		this.target = target;
	}
	public String getReplacement() {
		return replacement;
	}
	public void setReplacement(String replacement) {
		this.replacement = replacement;
	}

	private String replacement;
	
	public String getProperty() {
		return property;
	}
	public void setProperty(String property) {
		this.property = property;
	}
}
