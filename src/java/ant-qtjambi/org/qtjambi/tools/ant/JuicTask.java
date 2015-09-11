/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
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
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.tools.ant;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.DirectoryScanner;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.taskdefs.MatchingTask;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

import org.qtjambi.qt.osinfo.OSInfo;

public class JuicTask extends MatchingTask {
    private String msg;
    private String classpath;
    private String outputDir;
    private String trFunction;
    private String classNamePrefix;
    private boolean alwaysUpdate;
    private String qtLibDirectory;
    private String jambiDirectory;
    private String juicDirectory;
    private String dir;

    private List<String> searchPath() {
        List<String> pathList = new ArrayList<String>();

        if(juicDirectory != null) {
            File dirJuicDirectory = new File(juicDirectory);
            if(dirJuicDirectory.isDirectory()) {
                pathList.add(dirJuicDirectory.getAbsolutePath());

                File dirRelease = new File(juicDirectory, "release");
                if(dirRelease.isDirectory())
                    pathList.add(dirRelease.getAbsolutePath());

                File dirDebug = new File(juicDirectory, "debug");
                if(dirDebug.isDirectory())
                    pathList.add(dirDebug.getAbsolutePath());
            }
        }

        if(jambiDirectory != null) {
            File dirJambiDirectory = new File(jambiDirectory);
            if(dirJambiDirectory.isDirectory()) {
                File dirJuicDirectory = new File(jambiDirectory, "juic");
                if(dirJuicDirectory.isDirectory()) {
                    pathList.add(dirJuicDirectory.getAbsolutePath());

                    File dirRelease = new File(jambiDirectory, "release");
                    if(dirRelease.isDirectory())
                        pathList.add(dirRelease.getAbsolutePath());

                    File dirDebug = new File(jambiDirectory, "debug");
                    if(dirDebug.isDirectory())
                        pathList.add(dirDebug.getAbsolutePath());
                }
            }
        }

        return pathList;
    }

    public static String executableName() {
        String exe;
        switch(OSInfo.os()) {
        case Windows:
            exe = "juic.exe";
            break;
        default:
            exe = "juic";
            break;
        }
        return exe;
    }

    public String resolveExecutableAbsolutePath() {
        String executableName = executableName();
        return Util.LOCATE_EXEC(executableName, searchPath(), null).getAbsolutePath();
    }

    @Override
    public void execute() throws BuildException {
        if(msg != null)
            getProject().log(this, msg, Project.MSG_INFO);

        List<String> commandList = new ArrayList<String>();
        commandList.add(resolveExecutableAbsolutePath());
        if(outputDir != null) {
            commandList.add("-d");
            commandList.add(outputDir);
        }
        if(trFunction != null) {
            commandList.add("-tr");
            commandList.add(trFunction);
        }
        if(classNamePrefix != null) {
            commandList.add("-pf");
            commandList.add(classNamePrefix);
        }
        if(alwaysUpdate) {
            commandList.add("-a");
        }

        StringTokenizer tokenizer = new StringTokenizer(classpath, File.pathSeparator);
        while(tokenizer.hasMoreTokens()) {
            File dirToScan = Util.makeCanonical(tokenizer.nextToken());

            DirectoryScanner ds = getDirectoryScanner(dirToScan);
            String[] files = ds.getIncludedFiles();
            for(String file : files) {

                file = file.replaceAll("\\\\", "/");

                List<String> thisCommandList = new ArrayList<String>();
                thisCommandList.addAll(commandList);
                String packageString = file.substring(0, file.lastIndexOf('/')).replaceAll("/", ".");
                String uicFileString = dirToScan.getAbsolutePath() + '/' + file;
                thisCommandList.add("-p");
                thisCommandList.add(packageString);
                thisCommandList.add(uicFileString);

                File dirExecute = null;
                if(dir != null)
                    dirExecute = new File(dir);
                String binpath = AntUtil.getPropertyAsString(PropertyHelper.getPropertyHelper(getProject()), Constants.BINDIR);
                Exec.execute(thisCommandList, dirExecute, getProject(), binpath, qtLibDirectory);
            }
        }
    }

    public void setMessage(String msg) {
        this.msg = msg;
    }

    public void setClasspath(String classpath) {
        this.classpath = classpath;
    }

    public void setOutputDir(String outputDir) {
        this.outputDir = outputDir;
    }

    public void setTrFunction(String trFunction) {
        this.trFunction = trFunction;
    }

    public void setClassNamePrefix(String classNamePrefix) {
        this.classNamePrefix = classNamePrefix;
    }

    public void setAlwaysUpdate(boolean alwaysUpdate) {
        this.alwaysUpdate = alwaysUpdate;
    }

    public void setQtLibDirectory(String qtLibDirectory) {
        this.qtLibDirectory = qtLibDirectory;
    }

    public void setJambiDirectory(String jambiDirectory) {
        this.jambiDirectory = jambiDirectory;
    }

    public void setJuicDirectory(String juicDirectory) {
        this.juicDirectory = juicDirectory;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }
}
