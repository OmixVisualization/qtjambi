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

import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

import org.qtjambi.qt.osinfo.OSInfo;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class QMakeTask extends Task {

    private String msg = "";
    private String config = "";
    private String dir = ".";
    private String pro = "";

    //parameters
    private String qtconfig;
    private String qtjambiConfig;
    private String includepath;
    private String qmakebinary;

    private boolean recursive;
    private boolean debugTools;

    public static String executableName() {
        String exe;
        switch(OSInfo.os()) {
        case Windows:
            exe = "qmake.exe";
            break;
        default:
            exe = "qmake";
            break;
        }
        return exe;
    }

    public static String resolveExecutableAbsolutePath(Project project, String executableName, String propertyName) {
        if(executableName == null)
            executableName = executableName();

        // If qmakebinary is already absolute use it "/somedir/bin/qmake"
        if(executableName != null) {
            File file = new File(executableName);
            if(file.isAbsolute())
                return file.getAbsolutePath();

            if(propertyName != null) {
                // try in ${qtjambi.qt.bindir} if it is configured (caller supplies propertyName=Constants.BINDIR)
                // try in ${tools.qt.bindir} if it is configured (caller supplies propertyName=Constants.TOOLS_BINDIR)
                String binDir = project.getProperty(propertyName);
                if(binDir != null) {
                    File exeFile = new File(binDir, executableName);
                    if(exeFile.isFile()) {
                        // FIXME: Prepend 'binDir' to $PATH (so qmake can find moc)
                        return exeFile.getAbsolutePath();
                    }
                }
            }

            // try in $QTDIR/bin if it is configured
            try {
                String qtDir = System.getenv("QTDIR");
                if(qtDir != null) {
                    File exeFile = new File(qtDir, "bin" + File.separator + executableName);
                    if(exeFile.isFile()) {
                        // FIXME: Prepend '$QTDIR/bin' to $PATH (so qmake can find moc)
                        return exeFile.getAbsolutePath();
                    }
                }
            } catch(SecurityException eat) {
            }
        }

        // otherwise search $PATH
        return Util.LOCATE_EXEC(executableName, (String) null, null).getAbsolutePath();
    }

    public static String resolveExecutableAbsolutePath(Project project, String executableName) {
        return resolveExecutableAbsolutePath(project, executableName, Constants.BINDIR);
    }

    public String resolveExecutableAbsolutePath() {
        return resolveExecutableAbsolutePath(getProject(), qmakebinary);
    }

    private List<String> parseArguments() {
        List<String> arguments = new ArrayList<String>();

        StringTokenizer tokenizer = new StringTokenizer(config, " ");

        while(tokenizer.hasMoreTokens()) {
            arguments.add("-config");
            arguments.add(tokenizer.nextToken());
        }

        if(recursive)
            arguments.add("-r");

        if(debugTools)
            arguments.add("DEFINES+=QTJAMBI_DEBUG_TOOLS");

        return arguments;
    }

    private List<String> parseParameters() {
        List<String> parameters = new ArrayList<String>();

        if(qtconfig != null) {
            if(qtconfig.indexOf('{') >= 0) // windows name does not like (unescaped) "{" character this creates
                getProject().log(this, "QT_CONFIG not exported as value is: " + qtconfig, Project.MSG_INFO);
            else
                parameters.add("QT_CONFIG+=" + qtconfig);
        }

        String macSdk = getProject().getProperty(Constants.QTJAMBI_MACOSX_MAC_SDK);
        if(macSdk != null && macSdk.length() > 0)
            parameters.add("QMAKE_MAC_SDK=" + macSdk);

        if(qtjambiConfig != null && qtjambiConfig.length() > 0)
            parameters.add("QTJAMBI_CONFIG=" + qtjambiConfig);

        if(includepath != null && includepath.length() > 0)
            parameters.add("INCLUDEPATH+=" + includepath);

        return parameters;
    }

    @Override
    public void execute() throws NullPointerException {
        getProject().log(this, msg, Project.MSG_INFO);

        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        if(qtjambiConfig == null) {
            String thisQtjambiConfig = AntUtil.getPropertyAsString(propertyHelper, Constants.CONFIG);
            if(thisQtjambiConfig != null) {
                if(Constants.CONFIG_RELEASE.equals(thisQtjambiConfig))
                    qtjambiConfig = thisQtjambiConfig;
                else if(Constants.CONFIG_DEBUG.equals(thisQtjambiConfig))
                    qtjambiConfig = thisQtjambiConfig;
                else if(Constants.CONFIG_TEST.equals(thisQtjambiConfig))
                    qtjambiConfig = thisQtjambiConfig;
                else
                    getProject().log(this, "WARNING: QTJAMBI_CONFIG will not be exported as value " + thisQtjambiConfig + " is not recognised (from " + Constants.CONFIG + ")", Project.MSG_INFO);
                if(thisQtjambiConfig != null)
                    getProject().log(this, "QTJAMBI_CONFIG will be exported as " + qtjambiConfig + " (from " + Constants.CONFIG + ")", Project.MSG_INFO);
            }
        }

        String proFile = "";
        if(!pro.equals(""))
            proFile = Util.makeCanonical(pro).getAbsolutePath();

        final List<String> command =  new ArrayList<String>();

        command.add(resolveExecutableAbsolutePath());
        command.add(proFile);

        List<String> arguments = parseArguments();
        if(!arguments.isEmpty())
            command.addAll(arguments);

        List<String> parameters = parseParameters();
        if(!parameters.isEmpty())
            command.addAll(parameters);

        File dirExecute = null;
        if(dir != null)
            dirExecute = new File(dir);
        
        String binpath = AntUtil.getPropertyAsString(propertyHelper, Constants.BINDIR);
        Exec.execute(command, dirExecute, getProject(), binpath, null);
    }

    public void setMessage(String msg) {
        this.msg = msg;
    }

    public void setConfig(String config) {
        this.config = config;
    }

    public void setQtconfig(String qtconfig) {
        this.qtconfig = qtconfig;
    }

    public void setQtjambiConfig(String qtjambiConfig) {
        this.qtjambiConfig = qtjambiConfig;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }

    public void setQmakebinary(String binary) {
        this.qmakebinary = binary;
    }

    public void setIncludepath(String path) {
        this.includepath = path;
    }

    public void setRecursive(boolean recursive) {
        this.recursive = recursive;
    }

    public void setPro(String pro) {
        this.pro = pro;
    }

    public void setDebugTools(boolean debugTools) {
        this.debugTools = debugTools;
    }
}
