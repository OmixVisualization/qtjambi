package io.qt.tools.ant;

import java.util.Objects;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;

public class RegisterFinishedModuleTask extends Task{
	@Override
	public void execute() throws BuildException {
		ThreadedSubantTask.registerModuleFinished(getProject(), Objects.requireNonNull(module));
	}
	
	private String module;

	public String getModule() {
		return module;
	}

	public void setModule(String module) {
		this.module = module;
	}
}
