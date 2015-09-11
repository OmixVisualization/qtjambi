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
import org.qtjambi.qt.gui.QPaintEvent;

public class TestClassFunctionalityProblem extends QApplicationTest {
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

    // Tests the ownership transfer that we need to have objects like
    // QEvent stay alive after they are posted to the event queue...
    // The verification here is basically that the vm doesn't crash.
    private static class OwnershipTransferReceiver extends QObject {
        public QEvent.Type event_id;

        public QRect rect;

        @Override
        public boolean event(QEvent e) {
            event_id = e.type();

            QPaintEvent pe = null;
            try {
                pe = (QPaintEvent) e;
                rect = pe.rect();
            } catch (Exception ex) {
                ex.printStackTrace();
            }

            return super.event(e);
        }
    }

    private static class CustomPaintEvent extends QPaintEvent {
        public static boolean finalized = false;

        public CustomPaintEvent(QRect rect) {
            super(rect);
        }

        @Override
        protected void disposed() {
            synchronized(CustomPaintEvent.class) {
                finalized = true;
            }
            super.disposed();
        }
    }

    @Test
    public void run_testOwnershipTransfer() {
Utils.println(3, "run_testOwnershipTransfer() BEGIN");
        OwnershipTransferReceiver receiver = new OwnershipTransferReceiver();

        for (int i = 0; i < 3; ++i) {	// was: i < 1  // it appears we leak receiver
            QRect rect = new QRect(1, 2, 3, 4);
            QApplication.postEvent(receiver, new CustomPaintEvent(rect));
        }

        // To attempt deletion of the QPaintEvent, should not happen at
        // this time...
        System.gc();
        System.runFinalization();
        synchronized(CustomPaintEvent.class) {
            assertEquals(CustomPaintEvent.finalized, false);
        }

        // Process the event, thus also deleting it...
        QApplication.processEvents();

        // To provoke collection of the java side of the object, now
        // that C++ has released its hold on it
        System.gc();
        System.runFinalization();
        try {
            for(int i = 0; i < 60; i++) {
                synchronized(CustomPaintEvent.class) {
                    if(CustomPaintEvent.finalized)
                        break;
                }
                Thread.sleep(10);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

Utils.println(3, "run_testOwnershipTransfer() TEST");
        synchronized(CustomPaintEvent.class) {
            assertEquals(CustomPaintEvent.finalized, true);
        }

        // Sanity check the data...
        assertEquals(receiver.event_id, QEvent.Type.Paint);
        assertEquals(1, receiver.rect.x());
        assertEquals(2, receiver.rect.y());
        assertEquals(3, receiver.rect.width());
        assertEquals(4, receiver.rect.height());
Utils.println(3, "run_testOwnershipTransfer() END");
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestClassFunctionalityProblem.class.getName());
    }
}
