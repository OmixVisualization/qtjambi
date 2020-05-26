/****************************************************************************
**
** Copyright (C) 2019 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.tools.ant;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class InstallNameToolTask extends Task {

    private String msg = "";
    private String dir = ".";
    private String appname = "";
    private boolean debug = false;

    @Override
    public void execute() throws NullPointerException {
        getProject().log(this, msg, Project.MSG_INFO);
        
        int qtMajorVersion = Integer.parseInt(""+PropertyHelper.getProperty(getProject(), "qtjambi.soname.version.major"));
        int qtMinorVersion = Integer.parseInt(""+PropertyHelper.getProperty(getProject(), "qtjambi.soname.version.minor"));
        String libInfix = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qt.libinfix");
        String qtlibdir = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qt.libdir");

        String debugLib1 = debug ? "_debug" : "";
        String debugQtLib = debugLib1;
        if(qtMajorVersion==5 && qtMinorVersion>=14){
        	debugQtLib = "";
        }
        String[] libraries = {"Network", "Core", "Gui", "Widgets", "Quick", "Sql", "Xml", "Qml"};
        File dirExecute = null;
        String libraryName = "lib"+appname+debugLib1+".jnilib";
        if(dir != null){
            dirExecute = new File(dir);
            if(!new File(dirExecute, libraryName).exists()){
                return;
            }
        }
        final List<String> command =  new ArrayList<String>();
        command.add("install_name_tool");
        command.add("-change");
        command.add("");
        command.add("@loader_path/");
        command.add(libraryName);
        for (String library : new String[]{
            "libQtJambi"+debugLib1+"."+qtMajorVersion+".jnilib"
        }) {
            command.set(2, library);
            command.set(3, "@loader_path/../../platform-output" + ( debug ? "-debug" : "" ) + "/lib/"+library);
            Exec.execute(command, dirExecute, getProject());
        }
        
        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        boolean useQtFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_USE_QT_FRAMEWORK));
        if(useQtFrameworks){
            for (String library : libraries) {
                command.set(2, "@rpath/Qt"+library+".framework/Versions/"+qtMajorVersion+"/Qt"+library);
                command.set(3, "@loader_path/../../platform-output" + ( debug ? "-debug" : "" ) + "/lib/libQt"+qtMajorVersion+library+libInfix+debugQtLib+"."+qtMajorVersion+".dylib");
                Exec.execute(command, dirExecute, getProject());
            }
        }else{
            for (String library : libraries) {
                command.set(2, "libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
                command.set(3, "@loader_path/../../platform-output" + ( debug ? "-debug" : "" ) + "/lib/libQt"+qtMajorVersion+library+libInfix+debugQtLib+"."+qtMajorVersion+".dylib");
                Exec.execute(command, dirExecute, getProject());
            }
            if(qtlibdir!=null){
                for (String library : new String[]{
                    "libQtJambi"+debugLib1+"."+qtMajorVersion+".jnilib"
                }) {
                    command.set(2, qtlibdir+"/"+library);
                    command.set(3, "@loader_path/../../platform-output" + ( debug ? "-debug" : "" ) + "/lib/"+library);
                    Exec.execute(command, dirExecute, getProject());
                }
                for (String library : libraries) {
                    command.set(2, qtlibdir+"/libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
                    command.set(3, "@loader_path/../../platform-output" + ( debug ? "-debug" : "" ) + "/lib/libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
                    Exec.execute(command, dirExecute, getProject());
                }
            }
        }
    }

    public void setMessage(String msg) {
        this.msg = msg;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }

	public String getLibname() {
		return appname;
	}

	public void setLibname(String executable) {
		this.appname = executable;
	}

	public boolean isDebug() {
		return debug;
	}

	public void setDebug(boolean debug) {
		this.debug = debug;
	}
}
