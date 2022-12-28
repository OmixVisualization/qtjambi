package io.qt.tools.ant;

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
		for (FileSet fileSet : fileSets) {
			fileSet.forEach(resource -> {
				Optional<FileResource> optional = resource.asOptional(FileResource.class);
				if(optional.isPresent()) {
					FileResource fr = optional.get();
					if(fr.getFile().isFile() && !fr.getFile().getName().endsWith(".asc")) {
						Exec.execute(this, Arrays.asList("gpg", "-sb", "--armor"/*, "-u", "info@qtjambi.io"*/, "--yes", fr.getFile().getName()), fr.getBaseDir(), getProject(), null, null);
					}
				}
			});
		}
	}
	
	public void addConfigured(FileSet fileSet) {
		fileSets.add(fileSet);
	}
	
	private final List<FileSet> fileSets = new ArrayList<>();
}
