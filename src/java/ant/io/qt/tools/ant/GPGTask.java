package io.qt.tools.ant;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.types.FileSet;
import org.apache.tools.ant.types.resources.FileResource;

public class GPGTask extends Task {

	@Override
	public void execute() throws BuildException {
		List<Runnable> tasks = new ArrayList<>();
		for (FileSet fileSet : fileSets) {
			fileSet.forEach(resource -> {
				Optional<FileResource> optional = resource.asOptional(FileResource.class);
				if(optional.isPresent()) {
					FileResource fr = optional.get();
					if(fr.getFile().isFile()
							&& !fr.getFile().getName().endsWith(".asc")
							&& !fr.getFile().getName().endsWith(".md5")
							&& !fr.getFile().getName().endsWith(".sha1")
							&& !fr.getFile().getName().endsWith(".sha256")
							&& !fr.getFile().getName().endsWith(".sha512")) {
						File ascFile = new File(fr.getFile().getAbsolutePath()+".asc");
						if(!ascFile.exists())
							tasks.add(Exec.executeAsync(this, Arrays.asList("gpg", "-sb", "--armor"/*, "-u", "info@qtjambi.io"*/, "--yes", fr.getFile().getName()), fr.getBaseDir(), getProject(), null, null));
					}
				}
			});
		}
		
		BuildException exception = null;
		for (Runnable runnable : tasks) {
			try {
				runnable.run();
			} catch (BuildException e) {
				if(exception==null) {
					exception = e;
				}else{
					exception.addSuppressed(e);
				}
			}
		}
		if(exception!=null)
			throw exception;
	}
	
	public void addConfigured(FileSet fileSet) {
		fileSets.add(fileSet);
	}
	
	private final List<FileSet> fileSets = new ArrayList<>();
}
