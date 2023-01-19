/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import java.io.FileNotFoundException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import io.qt.core.QDir;
import io.qt.core.QFileInfo;
import io.qt.core.QProcess;

public class RunTests {
    public static void main(String[] args) throws Exception {
        QDir dir = new QDir("io/qt/autotests");
        if (!dir.exists())
            throw new FileNotFoundException("io/qt/autotests");

        List<String> filters = new ArrayList<String>();
        filters.add("*.class");
        List<QFileInfo> infos = dir.entryInfoList(filters);

        for (QFileInfo info : infos) {
            String className = "io.qt.autotests." + info.baseName();
            Class<?> cl = Class.forName(className);

            Method methods[] = cl.getMethods();
            boolean hasTestFunctions = false;
            for (Method m : methods) {
                if (m.isAnnotationPresent(org.junit.Test.class)) {
                    hasTestFunctions = true;
                    break;
                }
            }

            if (hasTestFunctions) {
                List<String> cmds = new ArrayList<String>();
                cmds.add("org.junit.runner.JUnitCore");
                cmds.add(cl.getName());
                System.out.println();
                for (int i=0; i<72; ++i) {
                    System.out.print("*");
                }
                System.out.println("\nRunning test: " + cl.getName());
                QProcess.execute("java", cmds);
            }
        }
    }
}
