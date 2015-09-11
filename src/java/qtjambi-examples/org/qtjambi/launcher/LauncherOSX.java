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

package org.qtjambi.launcher;

import org.qtjambi.qt.*;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;

import java.io.*;
import java.net.*;

public class LauncherOSX {

    // private static final String JAVA_PATH_MACOSX = "/System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Commands/java";
    //private static final String PLUGINS_JAR_MACOSX = "qtjambi-mac-gpl-" + Utilities.VERSION_STRING + ".jar";

    private  static void copy(URL sourceUrl, String destination) throws IOException {
        System.out.println("using source: " + sourceUrl);

        URLConnection connection = sourceUrl.openConnection();
        if (connection instanceof JarURLConnection)
            sourceUrl = ((JarURLConnection) connection).getJarFileURL();
        else
            throw new IllegalArgumentException("bad input url...: " + sourceUrl);

        copy(sourceUrl.openStream(), new FileOutputStream(destination));
    }

    private static void copy(InputStream in, OutputStream out) throws IOException {
        byte buffer[] = new byte[1024 * 64];
        while (in.available() > 0) {
            int read = in.read(buffer);
            out.write(buffer, 0, read);
        }
        in.close();
        out.close();
    }

    public static void main(String args[]) throws Exception {

        if (!System.getProperty("os.name").toLowerCase().contains("mac os x")) {
            Launcher.main(args);
            return;
        }

        String tmp = Utilities.jambiTempDir().getAbsolutePath();
        new File(tmp).mkdirs();

        copy(Thread.currentThread().getContextClassLoader().getResource("org/qtjambi/qt/internal/QtJambiObject.class"),
             tmp + "/classes.jar");
        copy(Thread.currentThread().getContextClassLoader().getResource("org/qtjambi/launcher/Launcher.class"),
             tmp + "/examples.jar");
        copy(Thread.currentThread().getContextClassLoader().getResource("lib/libQtCore.4.dylib"),
             tmp + "/native.jar");

        StringBuffer cmd = new StringBuffer();

        String javaLocation = System.getProperty("java.home") + "/bin/";
        cmd.append(javaLocation + "java");

        // classpath...
        cmd.append(" -cp " + tmp + "/classes.jar:" + tmp + "/examples.jar:" + tmp + "/native.jar");

        cmd.append(" -XstartOnFirstThread");
        cmd.append(" -Dorg.qtjambi.launcher.webstart=true");

        // the app itself...
        cmd.append(" org.qtjambi.launcher.Launcher");

        System.out.println(cmd.toString());

        String[] processArgs = RetroTranslatorHelper.split(cmd.toString(), " ");
        ProcessBuilder procBuilder = new ProcessBuilder(processArgs);
        Process proc = procBuilder.start();

        proc.waitFor();
    }
}
