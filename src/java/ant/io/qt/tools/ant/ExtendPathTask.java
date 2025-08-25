package io.qt.tools.ant;

import java.io.File;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.types.Path;

public class ExtendPathTask extends Task {

	@Override
	public void execute() throws BuildException {
		Path path = getProject().getReference(refid);
		path.add(_path);
	}
	
	public void addConfigured(Path.PathElement e) {
		this._path.add(e);
	}
	
	public void addConfigured(Path e) {
		this._path.add(e);
	}
	
	public Path.PathElement createPathElement() {
		return _path.createPathElement();
	}
	
	private final Path _path = new Path(this.getProject());
	private String refid;
	
	public void setLocation(File location) {
		Path.PathElement el = _path.createPathElement();
		el.setLocation(location);
		_path.add(el);
	}

	public void setPath(String path) {
		Path.PathElement el = _path.createPathElement();
		el.setPath(path);
		_path.add(el);
	}

	public void setRefid(String refid) {
		this.refid = refid;
	}
}
