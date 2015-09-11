/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
**
** This file is part of Qt Jambi.
**
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
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
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

package org.qtjambi.tools.ant;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;

import java.util.ArrayList;
import java.util.List;

public class Directory extends Task implements Dirent {
    private String name;
    private String rootPath;
    private String subdir;
    private String destSubdir;
    private Boolean ifValue;
    private Boolean recursive;
    private List<Dirent> childList;

    public Directory(String rootPath, String name) {
        childList = new ArrayList<Dirent>();
        setRootPath(rootPath);
        setName(name);
    }

    public Directory() {
        childList = new ArrayList<Dirent>();
    }

    public void execute() throws BuildException {
        if(name == null)
            throw new BuildException("Attribute 'name' is missing or invalid; name=" + name);
        if(name.length() == 0 && rootPath == null)  // rootPath might be the full path when name.length()==0
            throw new BuildException("Attribute 'name' is missing or invalid; name=" + name);
        if(rootPath == null)
            throw new BuildException("Attribute 'rootPath' is missing or invalid; rootPath=" + rootPath);
    }

    public void addConfigured(File file) {
        try {
            file.perform();
            childList.add(file);
        } catch(Exception e) {
            e.printStackTrace();
            throw new BuildException("Failed to add file sub-element.......");
        }
    }

    public void addConfigured(Directory directory) {
        try {
            directory.perform();
            childList.add(directory);
        } catch(Exception e) {
            e.printStackTrace();
            throw new BuildException("Failed to add directory sub-element.......");
        }
    }

    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }

    public String getRootPath() {
        return rootPath;
    }
    public void setRootPath(String rootPath) {
        this.rootPath = rootPath;
    }

    public String getSubdir() {
        return subdir;
    }
    public void setSubdir(String subdir) {
        this.subdir = subdir;
    }

    public String getDestSubdir() {
        return destSubdir;
    }
    public void setDestSubdir(String destSubdir) {
        this.destSubdir = destSubdir;
    }

    public Boolean getIf() {
        return ifValue;
    }
    public void setIf(Boolean ifValue) {
        this.ifValue = ifValue;
    }

    public Boolean getRecursive() {
        return recursive;
    }
    public void setRecursive(Boolean recursive) {
        this.recursive = recursive;
    }

    public Dirent getChild(String name) {
        for(Dirent d : childList) {
            if(name.equals(d.getName()))
                return d;
        }
        return null;
    }

    public List<Dirent> getChildList() {
        return childList;
    }
    public void setChildList(List<Dirent> childList) {
        this.childList = childList;
    }

    public boolean isDirectory() {
        return true;
    }
}
