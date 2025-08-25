package io.qt.tools.ant;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.taskdefs.CallTarget;
import org.apache.tools.ant.taskdefs.Property;

public class ForeachNativeTask extends Task{
	@Override
	public void execute() throws BuildException {
		java.io.File dir = new java.io.File(this.dir);
		if(dir.isDirectory()) {
			String prefix = module + "-native-";
			String suffix = "-" + version + ".jar";
			List<Callable<Void>> tasks = new ArrayList<>();
			for(String file : dir.list()) {
				if(file.startsWith(prefix) && file.endsWith(suffix)) {
					String platform = file.substring(prefix.length(), file.length() - suffix.length());
					if(!platform.endsWith("-debug")) {
						CallTarget ct = (CallTarget) getProject().createTask("antcall");
						ct.setOwningTarget(getOwningTarget());
						ct.init();
						ct.setTarget(target);
						Property param = ct.createParam();
						param.setName("native-spec");
						param.setValue(platform);
						tasks.add(()->{ct.execute(); return null;});
					}
				}
			}
			ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
			List<Future<Void>> results;
			try {
				results = executor.invokeAll(tasks);
		        for (Future<Void> f : results) {
		        	try {
						f.get();
					} catch (ExecutionException e) {
						e.printStackTrace();
						if(e.getCause() instanceof BuildException) {
							throw (BuildException)e.getCause();
						}else if(e.getCause() instanceof RuntimeException) {
							throw (RuntimeException)e.getCause();
						}else if(e.getCause() instanceof Error) {
							throw (Error)e.getCause();
						}
					}
		        }
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

	        executor.shutdown();
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
