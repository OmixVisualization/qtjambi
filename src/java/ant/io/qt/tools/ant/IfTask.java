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
import org.apache.tools.ant.taskdefs.Sequential;
import org.apache.tools.ant.taskdefs.condition.ConditionBase;

public class IfTask extends ConditionBase {
	public IfTask() {
		super();
	}

	public static class Elseif extends ConditionBase{
		private Sequential thenTasks = null;
		public void addThen(Sequential thenTasks) {
			if (this.thenTasks != null)
				throw new BuildException("Multiple <then> not allowed in <elseif>");
			this.thenTasks = thenTasks;
		}
		
		public boolean eval() throws BuildException {
			if (countConditions() > 1)
				throw new BuildException("Multiple conditions not allowed for <elseif>");
			if (countConditions() < 1)
				throw new BuildException("<elseif> requires condition");
			return super.getConditions().nextElement().eval();
		}

		public void execute() throws BuildException {
			if (thenTasks != null)
				thenTasks.execute();
		}
	}
	
	private Sequential thenTasks = null;
	private List<Elseif> elseIfTasks = new ArrayList<>();
	private Sequential elseTasks = null;
	public void addThen(Sequential thenTasks) {
		if (this.thenTasks != null)
			throw new BuildException("Multiple <then> not allowed in <if>");
		this.thenTasks = thenTasks;
	}
	public void addElseIf(Elseif elseIf) {
		this.elseIfTasks.add(elseIf);
	}
	public void addElse(Sequential elseTasks) {
		if (this.elseTasks != null)
			throw new BuildException("Multiple <else> not allowed in <if>");
		this.elseTasks = elseTasks;
	}
	
	private boolean test() throws BuildException {
		if (countConditions() > 1)
			throw new BuildException("Multiple conditions not allowed for <elseif>");
		if (countConditions() < 1)
			throw new BuildException("<elseif> requires condition");
		return super.getConditions().nextElement().eval();
	}
	
	public void execute() throws BuildException {
		if (test()) {
			if (thenTasks != null)
				thenTasks.execute();
		} else {
			boolean done = false;
			for (Elseif elseIf : elseIfTasks) {
				if (elseIf.eval()) {
					done = true;
					elseIf.execute();
					break;
				}
			}

			if (!done && elseTasks != null)
				elseTasks.execute();
		}
	}
}
