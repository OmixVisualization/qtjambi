package io.qt.tools.ant;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.taskdefs.Ant.Reference;
import org.apache.tools.ant.taskdefs.Property;
import org.apache.tools.ant.taskdefs.SubAnt;
import org.apache.tools.ant.types.DirSet;
import org.apache.tools.ant.types.Path;
import org.apache.tools.ant.types.ResourceCollection;
import org.apache.tools.ant.types.resources.FileResource;

public class ThreadedSubantTask extends Task {
	private String target;
	private boolean inheritall;
	private Path buildpath;
	
	public String getTarget() {
		return target;
	}
	public void setTarget(String target) {
		this.target = target;
	}
	public boolean isInheritall() {
		return inheritall;
	}
	public void setInheritall(boolean inheritall) {
		this.inheritall = inheritall;
	}
	public void addDirset(DirSet set) {
        add(set);
    }
	public void add(ResourceCollection rc) {
        getBuildpath().add(rc);
    }
	public void setBuildpath(Path s) {
        getBuildpath().append(s);
    }
	public Path createBuildpath() {
        return getBuildpath().createPath();
    }
	public Path.PathElement createBuildpathElement() {
        return getBuildpath().createPathElement();
    }
	private Path getBuildpath() {
        if (buildpath == null) {
            buildpath = new Path(getProject());
        }
        return buildpath;
    }
	public void setBuildpathRef(Reference r) {
        createBuildpath().setRefid(r);
    }
	
    private static final ThreadLocal<ThreadedSubantTask> threadedSubantTasks = new ThreadLocal<>();
    
    private static class InterruptedBuildException extends BuildException{}
	
	@Override
	public void execute() throws BuildException {
		if (buildpath == null) {
            throw new BuildException("No buildpath specified");
        }

        final String[] filenames = buildpath.list();
        final int count = filenames.length;
        if (count < 1) {
            log("No sub-builds to iterate on", Project.MSG_WARN);
            return;
        }
		ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
		List<Future<?>> futures = new ArrayList<>();
		for (String filename : filenames) {
			File file = new File(filename);
            if (file.isDirectory()) {
            	SubAnt subant = new SubAnt();
            	subant.setOwningTarget(getOwningTarget());
            	subant.setProject(getProject());
            	subant.add(new FileResource(file));
            	subant.setInheritall(inheritall);
            	subant.setTarget(target);
            	Property param = new Property();
            	if(!inheritall) {
            		param = new Property();
            		param.setName("qtjambi.osinfo");
    				param.setValue(getProject().getProperty("qtjambi.osinfo"));
            	}
            	futures.add(executor.submit(()->{
            			threadedSubantTasks.set(ThreadedSubantTask.this);
            			subant.execute();
            		}));
            }
		}
		BuildException exn = null;
		for (Future<?> future : futures) {
			try {
				future.get();
			} catch (InterruptedException e) {
			} catch (InterruptedBuildException e) {
			} catch (BuildException e) {
				if(exn==null)
					exn = e;
				else
					exn.addSuppressed(e);
				break;
			} catch (ExecutionException e) {
				if(e.getCause() instanceof InterruptedBuildException) {
				}else if(e.getCause() instanceof BuildException) {
					if(exn==null)
						exn = (BuildException)e.getCause();
					else
						exn.addSuppressed((BuildException)e.getCause());
				}else {
					if(exn==null)
						exn = new BuildException(e);
					else
						exn.addSuppressed(e);
				}
				break;
			}
		}
		if(exn!=null) {
			executor.shutdownNow();
			throw exn;
		}else {
			executor.shutdown();
		}
	}
	
	private final Map<String,BuildException> finishedModules = new TreeMap<>();
	
	static void registerModuleFinished(Project project, String module) {
		ThreadedSubantTask task = threadedSubantTasks.get();
		if(task!=null) {
			BuildException exception = project.getReference("compile-exception-"+module);
			synchronized(task.finishedModules) {
				task.finishedModules.put(module, exception);
				task.finishedModules.notifyAll();
			}
			if(exception!=null) {
				throw exception;
			}
		}
	}
	
	static void waitForModules(Project project, List<String> moduleList) {
		try {
			ThreadedSubantTask task = threadedSubantTasks.get();
			if(task!=null) {
				while(!moduleList.isEmpty()) {
					synchronized(task.finishedModules) {
						for(String mod : moduleList) {
							BuildException exception = task.finishedModules.get(mod);
							if(exception!=null)
								throw new InterruptedException();
						}
						moduleList.removeAll(task.finishedModules.keySet());
						if(!moduleList.isEmpty())
							task.finishedModules.wait();
					}
				}
			}
		} catch (InterruptedException e) {
			throw new InterruptedBuildException();
		}
	}
}
