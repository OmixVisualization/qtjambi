/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.assertEquals;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QEvent;
import io.qt.gui.QImage;
import io.qt.gui.QPixmap;
import io.qt.widgets.QApplication;

public class TestClassFunctionalityXpm extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Before
    public void setUp() {
        QApplication.processEvents();
    }

    @After
    public void tearDown() {
        QApplication.processEvents();
        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());

        ApplicationInitializer.runGC();
    }

    // Check that const char *[] is handled properly by the generated code
    @Test
    public void run_XPMConstructors() {
java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_XPMConstructors() BEGIN");
        String qt_plastique_radio[] = { "13 13 2 1", "X c #000000", ". c #ffffff", "....XXXXX....", "..XX.....XX..", ".X.........X.", ".X.........X.", "X...........X", "X...........X",
                "X...........X", "X...........X", "X...........X", ".X.........X.", ".X.........X.", "..XX.....XX..", "....XXXXX...." };

        QImage img = new QImage(qt_plastique_radio);
        assertEquals(img.width(), 13);
        assertEquals(img.height(), 13);
java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_XPMConstructors() QIMAGE TESTING");

        assertEquals(img.pixel(2, 1), 0xff000000);
        assertEquals(img.pixel(0, 0), 0xffffffff);

java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_XPMConstructors() QPIXMAP TESTING");
        QPixmap pm = new QPixmap(qt_plastique_radio);
        QImage img2 = pm.toImage();
        assertEquals(img2.pixel(2, 1), 0xff000000);
        assertEquals(img2.pixel(12, 12), 0xffffffff);
java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_XPMConstructors() END");
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestClassFunctionalityXpm.class.getName());
    }
}
