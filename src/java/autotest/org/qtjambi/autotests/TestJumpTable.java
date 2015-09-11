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

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;

import org.junit.Assert;
import org.junit.Test;

import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.widgets.QApplication;

public class TestJumpTable extends QApplicationTest {
    @Test
    public void testMemberFunctions() {
        QRect r = new QRect(1, 2, 3, 4);

        Assert.assertEquals(r.x(), 1);
        Assert.assertEquals(r.y(), 2);
        Assert.assertEquals(r.width(), 3);
        Assert.assertEquals(r.height(), 4);
    }

    @Test
    public void testStaticFunctions() {
        QCoreApplication.setApplicationName("TestApp");
        Assert.assertEquals(QCoreApplication.applicationName(), "TestApp");
    }

    @Test
    public void testReplaceValueFunctions() throws Exception {
        File f = File.createTempFile("jambi", "autotest");
        f.deleteOnExit();

        String path = f.getAbsolutePath();

        QFile file = new QFile(path);
        file.open(QIODevice.OpenModeFlag.WriteOnly);
        QTextStream stream = new QTextStream(file);
        stream.writeLong(1234);
        stream.flush();
        stream.dispose();
        file.dispose();

        BufferedReader reader = null;
        try {
        	reader = new BufferedReader(new InputStreamReader(new FileInputStream(path)));
			String content = reader.readLine();
			Assert.assertEquals(content, "1234");
		} finally {
			if(reader!=null){
				reader.close();
			}
		}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestJumpTable.class.getName());
    }
}
