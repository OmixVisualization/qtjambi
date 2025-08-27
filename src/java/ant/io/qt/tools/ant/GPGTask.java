package io.qt.tools.ant;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.taskdefs.Checksum;
import org.apache.tools.ant.types.FileSet;
import org.apache.tools.ant.types.resources.FileResource;

public class GPGTask extends Task {

	@Override
	public void execute() throws BuildException {
		List<Runnable> tasks = new ArrayList<>();
		ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
		boolean[] deleteBundle = {false};
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
						Checksum checksumTest = new Checksum();
						checksumTest.setProject(getProject());
						checksumTest.setAlgorithm("MD5");
						checksumTest.setFile(fr.getFile());
						checksumTest.setFileext(".md5");
						if(!checksumTest.eval()) {
							if(fr.getFile().exists())
								System.out.println("MD5 checksum for file "+fr.getFile()+" does not match");
							deleteBundle[0] = true;
							tasks.add(Exec.executeAsync(this, Arrays.asList("gpg", "-sb", "--armor"/*, "-u", "info@qtjambi.io"*/, "--yes", fr.getFile().getName()), fr.getBaseDir(), getProject(), null, null));
							checksumTest = new Checksum();
							checksumTest.setProject(getProject());
							checksumTest.setAlgorithm("MD5");
							checksumTest.setFile(fr.getFile());
							checksumTest.setFileext(".md5");
							{
								Future<?> future = executor.submit(checksumTest::execute);
								tasks.add(()->{
									try {
										future.get();
									} catch (InterruptedException e) {
									} catch (ExecutionException e) {
										throw new BuildException(e);
									}
								});
							}
							checksumTest = new Checksum();
							checksumTest.setProject(getProject());
							checksumTest.setAlgorithm("SHA-1");
							checksumTest.setFile(fr.getFile());
							checksumTest.setFileext(".sha1");
							{
								Future<?> future = executor.submit(checksumTest::execute);
								tasks.add(()->{
									try {
										future.get();
									} catch (InterruptedException e) {
									} catch (ExecutionException e) {
										throw new BuildException(e);
									}
								});
							}
						}
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
		if(deleteBundle[0] && bundle!=null) {
			new java.io.File(bundle).delete();
		}
		if(exception!=null)
			throw exception;
	}
	
	public void addConfigured(FileSet fileSet) {
		fileSets.add(fileSet);
	}
	
	private final List<FileSet> fileSets = new ArrayList<>();
	
	private String bundle;

	public String getBundle() {
		return bundle;
	}

	public void setBundle(String bundle) {
		this.bundle = bundle;
	}
}
