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

package org.qtjambi.autotests;

import java.io.FileNotFoundException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QProcess;

public class RunTests {
    public static void main(String[] args) throws Exception {
        QDir dir = new QDir("org/qtjambi/autotests");
        if (!dir.exists())
            throw new FileNotFoundException("org/qtjambi/autotests");

        List<String> filters = new ArrayList<String>();
        filters.add("*.class");
        List<QFileInfo> infos = dir.entryInfoList(filters);

        for (QFileInfo info : infos) {
            String className = "org.qtjambi.autotests." + info.baseName();
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
