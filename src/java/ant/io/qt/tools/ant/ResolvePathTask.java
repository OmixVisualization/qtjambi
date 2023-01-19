/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.tools.ant;

import java.io.File;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class ResolvePathTask extends Task {
    private boolean verbose;
    private boolean overwrite;
    private String path;
    private String var;
    private boolean failonerror = true;

    public boolean isFailonerror() {
		return failonerror;
	}
	public void setFailonerror(boolean failonerror) {
		this.failonerror = failonerror;
	}
	public boolean isVerbose() {
        return verbose;
    }
    public void setVerbose(boolean verbose) {
        this.verbose = verbose;
    }

    public boolean isOverwrite() {
        return overwrite;
    }
    public void setOverwrite(boolean overwrite) {
        this.overwrite = overwrite;
    }

    public String getVar() {
        return var;
    }
    public void setVar(String var) {
        this.var = var;
    }

    public String getPath() {
        return path;
    }
    public void setPath(String path) {
        this.path = path;
    }

    public void execute() throws BuildException {
        if(var == null || var.length() == 0) {
            throw new BuildException("ResolvePathTask.var property is not set or empty string");
        }
        if(path == null || path.length() == 0) {
            throw new BuildException("ResolvePathTask.path property is not set or empty string");
        }

        try {
			PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());
			// Don't like this kind of hack
			for(int i = 0; i < 10; i++) {
			    String replacedPath = props.replaceProperties(null, path, null);
			    if(replacedPath.equals(path))
			        break;
			    path = replacedPath;
			}

			String newValue = resolveWithString(path);

			String oldPath = AntUtil.getPropertyAsString(props, var);
			if(oldPath == null) {
			    AntUtil.setNewProperty(props, var, newValue);
			    getProject().log(this, var + "=\"" + newValue + "\"", Project.MSG_VERBOSE);
			} else {
			    if(isOverwrite()) {
			        AntUtil.setProperty(props, var, newValue, false);
			        getProject().log(this, var + "=\"" + newValue + "\" (old value=\"" + oldPath + "\")", Project.MSG_VERBOSE);
			    }
			}
		} catch (BuildException e) {
			if(failonerror)
				throw e;
			else
				getProject().log(this, e.getMessage(), Project.MSG_WARN);
		}
    }

    public static File resolve(File path) {
        if(!path.exists()) {
        	throw new BuildException("Unable to resolve path \"" + path + "\" to absolute path");
        }
        File result = path.getAbsoluteFile();
        do {
            // Canonicalize with removal of "." in trailing part of path, we could do better here...
            String n = result.getName();
            if(n.equals(".") == false)
                break;
            File nextResult = result.getParentFile();
            if(nextResult == null)
                break;
            result = nextResult;
        } while(true);
        return result;
    }

    public static String resolveWithString(String path) {
        File file = new File(path);
        File result = resolve(file);
        return result.getAbsolutePath();
    }

}
