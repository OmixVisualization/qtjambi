/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.tools.ant;

import org.apache.tools.ant.Project;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.qtjambi.qt.osinfo.OSInfo;

public class MakeTask extends Task {

    private String msg = "";
    private String target;
    private String dir = ".";
    private boolean silent = false;
    private boolean failOnError = true;

    private String compilerName() {
        String make = System.getenv("MAKE");
        if(make !=  null)
            return make;

        switch(OSInfo.os()) {
        case Windows:
            PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
            String compiler = AntUtil.getPropertyAsString(propertyHelper, Constants.COMPILER);

            if(FindCompiler.Compiler.MinGW.toString().equals(compiler))
                return "mingw32-make";
            // our instructions for mingw-w64 are to copy mingw32-make.exe (and its required DLLs) into
            //  a directory on their own such as $MINGW_W64/mybin to make use of the GNU make from mingw.
            if(FindCompiler.Compiler.MinGW_W64.toString().equals(compiler))
                return "mingw32-make";
            return "nmake";
        }
        return "make";
    }

    @Override
    public void execute() throws BuildException {
        getProject().log(this, msg, Project.MSG_INFO);

        List<String> commandArray = new ArrayList<String>();
        commandArray.add(compilerName());

        if(silent && OSInfo.os() != OSInfo.OS.Windows) {
            commandArray.add("-s");
        }

        try {
            final String makeOptions = System.getenv("MAKEOPTS");
            List<String> list = Util.safeSplitStringTokenizer(makeOptions);
            if(list != null)
                commandArray.addAll(list);
        } catch(SecurityException e) {
        }

        if(target != null && target.length() > 0)  // isEmpty() is Java1.6 :(
            commandArray.add(target);

        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        String ldpath = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
        try {
            File dirExecute = null;
            if(dir != null)
                dirExecute = new File(dir);
            Exec.execute(commandArray, dirExecute, getProject(), null, ldpath);
        } catch(BuildException e) {
            if(failOnError)
                throw e;
            else
                getProject().log(this, e.getMessage() + "; failOnError=" + failOnError + "; continuing", e, Project.MSG_ERR);
        }
    }

    public void setMessage(String msg) {
        this.msg = msg;
    }

    public void setTarget(String target) {
        this.target = target;
    }

    public void setSilent(boolean silent) {
        this.silent = silent;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }

    public void setFailOnError(boolean failOnError) {
        this.failOnError = failOnError;
    }
}
