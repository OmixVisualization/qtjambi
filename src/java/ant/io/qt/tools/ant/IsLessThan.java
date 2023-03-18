/*
 * Copyright (c) 2001-2004, 2007 Ant-Contrib project.  All rights reserved.
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

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.taskdefs.condition.Condition;

public final class IsLessThan implements Condition {

	private String arg1, arg2;
	private boolean trim = false;
	private boolean caseSensitive = true;

	public void setArg1(String a1) {
		arg1 = a1;
	}

	public void setArg2(String a2) {
		arg2 = a2;
	}

	/**
	 * Should we want to trim the arguments before comparing them?
	 *
	 * @since Revision: 1.3, Ant 1.5
	 */
	public void setTrim(boolean b) {
		trim = b;
	}

	/**
	 * Should the comparison be case sensitive?
	 *
	 * @since Revision: 1.3, Ant 1.5
	 */
	public void setCasesensitive(boolean b) {
		caseSensitive = b;
	}

	public boolean eval() throws BuildException {
		if (arg1 == null || arg2 == null) {
			throw new BuildException("both arg1 and arg2 are required in " + "less than");
		}

		if (trim) {
			arg1 = arg1.trim();
			arg2 = arg2.trim();
		}

		// check if args are numbers
		try {
			int num1 = Integer.parseInt(arg1);
			int num2 = Integer.parseInt(arg2);
			return num1 < num2;
		} catch (NumberFormatException nfe) {
			// ignored, fall thru to string comparision
		}

		return caseSensitive ? arg1.compareTo(arg2) < 0 : arg1.compareToIgnoreCase(arg2) < 0;
	}

}