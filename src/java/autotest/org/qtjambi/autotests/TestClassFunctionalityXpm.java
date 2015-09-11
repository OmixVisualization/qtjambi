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

import static org.junit.Assert.assertEquals;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QPixmap;

public class TestClassFunctionalityXpm extends QApplicationTest {
    @Before
    public void setUp() {
        QApplication.processEvents();
    }

    @After
    public void tearDown() {
        QApplication.processEvents();
        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());

        System.gc();
        System.runFinalization();
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }
    }

    // Check that const char *[] is handled properly by the generated code
    @Test
    public void run_XPMConstructors() {
Utils.println(3, "run_XPMConstructors() BEGIN");
        String qt_plastique_radio[] = { "13 13 2 1", "X c #000000", ". c #ffffff", "....XXXXX....", "..XX.....XX..", ".X.........X.", ".X.........X.", "X...........X", "X...........X",
                "X...........X", "X...........X", "X...........X", ".X.........X.", ".X.........X.", "..XX.....XX..", "....XXXXX...." };

        QImage img = new QImage(qt_plastique_radio);
        assertEquals(img.width(), 13);
        assertEquals(img.height(), 13);
Utils.println(3, "run_XPMConstructors() QIMAGE TESTING");

        assertEquals(img.pixel(2, 1), 0xff000000);
        assertEquals(img.pixel(0, 0), 0xffffffff);

Utils.println(3, "run_XPMConstructors() QPIXMAP TESTING");
        QPixmap pm = new QPixmap(qt_plastique_radio);
        QImage img2 = pm.toImage();
        assertEquals(img2.pixel(2, 1), 0xff000000);
        assertEquals(img2.pixel(12, 12), 0xffffffff);
Utils.println(3, "run_XPMConstructors() END");
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestClassFunctionalityXpm.class.getName());
    }
}
