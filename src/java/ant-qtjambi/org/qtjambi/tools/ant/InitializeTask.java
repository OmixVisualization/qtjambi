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

import java.io.File;
import java.util.Enumeration;
import java.util.Properties;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;

import org.qtjambi.qt.osinfo.OSInfo;

public class InitializeTask extends AbstractInitializeTask {

    public void executeInitialize() throws BuildException {
        String sourceValue;

        String sep = AntUtil.getPropertyAsString(propertyHelper, "sep");
        if(sep == null) {
            sep = File.separator;
            sourceValue = " (auto-detect)";
        } else {
            sourceValue = null;
        }
        mySetProperty(-1, "sep", sourceValue, sep, false);

        String psep = AntUtil.getPropertyAsString(propertyHelper, "psep");
        if(psep == null) {
            psep = File.pathSeparator;
            sourceValue = " (auto-detect)";
        } else {
            sourceValue = null;
        }
        mySetProperty(-1, "psep", sourceValue, psep, false);

        final String[] emitA = {
            Constants.DIRECTORY
        };
        for(String emit : emitA) {
            String value = AntUtil.getPropertyAsString(propertyHelper, emit);
            File dir = new File(value);
            if(dir.isDirectory() == false)
                sourceValue = " (WARNING: path does not exist or is not a directory)";
            else
                sourceValue = null;
            mySetProperty(-1, emit, sourceValue, value, false);
        }

        String detectedOsname = OSInfo.osArchName();
        String osname = AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME);
        if(osname == null) {
            sourceValue = " (auto-detected)";
            osname = detectedOsname;
        } else {
            sourceValue = " (detected: " + detectedOsname + ")";
        }
        mySetProperty(-1, Constants.OSNAME, sourceValue, osname, false);  // report value


        String s;

        s = null;
        if(OSInfo.isLinux())
            s = OSInfo.K_LINUX;
        else if(OSInfo.isWindows())
            s = OSInfo.K_WINDOWS;
        else if(OSInfo.isMacOS())
            s = OSInfo.K_MACOSX;
        else if(OSInfo.isFreeBSD())
            s = OSInfo.K_FREEBSD;
        else if(OSInfo.isSolaris())
            s = OSInfo.K_SUNOS;
        if(s != null)
            AntUtil.setNewProperty(propertyHelper, Constants.OSPLATFORM, s);

        s = null;

        if(true) {    // FIXME lookup if verbose is set
            Properties props = System.getProperties();
            Enumeration e = props.propertyNames();
            while(e.hasMoreElements()) {
                String k = (String) e.nextElement();
                Object v = System.getProperty(k);
                getProject().log("systemProperty[" + k + "] = " + v, Project.MSG_VERBOSE);
            }
        }

        String javaOsArch = System.getProperty("os.arch");	// arm|
        if("arm".equals(javaOsArch)) {
            // FIXME get LE or BE
            s = "arm";
        } else {
            if(osname.endsWith("64"))
                s = "x86_64";
            else
                s = "i386";
        }
        if(s != null)
            AntUtil.setNewProperty(propertyHelper, Constants.OSCPU, s);
        String osCpu = s;


        String JAVA_HOME = System.getenv("JAVA_HOME");   // used here
        if(JAVA_HOME != null)
            getProject().log(this, "JAVA_HOME is set: " + prettyValue(JAVA_HOME), Project.MSG_INFO);

        String javaHomeTarget = decideJavaHomeTarget();
        if(javaHomeTarget == null)
            throw new BuildException("Unable to determine JAVA_HOME_TARGET, setup environment variable JAVA_HOME (or JAVA_HOME_TARGET) or edit build.properties");

        String javaOsarchTarget = decideJavaOsarchTarget();
        if(javaOsarchTarget == null) {
            if(OSInfo.isMacOS() == false)  // On MacOSX there is no sub-dir inside the JDK include directory that contains jni.h
                throw new BuildException("Unable to determine JAVA_OSARCH_TARGET, setup environment variable JAVA_OSARCH_TARGET or edit build.properties");
        }

        String javaOscpu = decideJavaOscpu();
        if(javaOscpu == null)
            throw new BuildException("Unable to determine JAVA_OSCPU, setup environment variable JAVA_OSCPU or edit build.properties");

        String javaOscpuTarget = decideJavaOscpuTarget();
        if(javaOscpuTarget  == null)
                throw new BuildException("Unable to determine JAVA_OSCPU_TARGET, setup environment variable JAVA_OSCPU_TARGET or edit build.properties");

        if(OSInfo.isMacOS())
            mySetProperty(0, Constants.QTJAMBI_CONFIG_ISMACOSX, " (set by init)", "true", false);
    }
}
