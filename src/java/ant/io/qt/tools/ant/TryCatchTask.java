/*
 * Copyright (c) 2001-2004 Ant-Contrib project.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package io.qt.tools.ant;

import java.util.*;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.taskdefs.Sequential;

public class TryCatchTask extends Task {
	
	public static final class CatchBlock extends Sequential {
		private String throwable = BuildException.class.getName();

		public CatchBlock() {
			super();
		}

		public void setThrowable(String throwable) {
			this.throwable = throwable;
		}

		public boolean execute(Throwable t) throws BuildException {
			try {
				Class<?> c = Thread.currentThread().getContextClassLoader().loadClass(throwable);
				if (c.isAssignableFrom(t.getClass())) {
					execute();
					return true;
				}
				return false;
			} catch (ClassNotFoundException e) {
				throw new BuildException(e);
			}
		}
	}


	public TryCatchTask() {
	}

	private Sequential tryBlock = null;
	private final List<CatchBlock> catchBlocks = new ArrayList<>();
	private Sequential finallyBlock = null;
	private String property = null;
	private String reference = null;
	
	public void addTry(Sequential tr) throws BuildException {
		if (tryBlock != null)
			throw new BuildException("<trycatch> does not accept more than one <try>");
		tryBlock = tr;
	}

	public void addCatch(CatchBlock cb) {
		catchBlocks.add(cb);
	}

	public void addFinally(Sequential fin) throws BuildException {
		if (finallyBlock != null)
			throw new BuildException("<trycatch> does not accept more than one <finally>");
		finallyBlock = fin;
	}

	public void execute() throws BuildException {
		Throwable thrown = null;
		if (tryBlock == null)
			throw new BuildException("A nested <try> element is required");
		try {
			tryBlock.perform();
		} catch (Throwable e) {
			if (property != null)
				getProject().setProperty(property, e.getMessage());
			if (reference != null)
				getProject().addReference(reference, e);
			boolean executed = false;
			for(CatchBlock cb : catchBlocks) {
				executed = cb.execute(e);
				if(executed)
					break;
			}
			if (!executed)
				thrown = e;
		} finally {
			if (finallyBlock != null)
				finallyBlock.perform();
		}
		if (thrown instanceof BuildException)
			throw (BuildException)thrown;
		if (thrown != null)
			throw new BuildException(thrown);
	}

}
