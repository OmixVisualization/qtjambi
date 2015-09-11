/****************************************************************************
**
** Copyright (C) 2012 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2012 D L Miles Consulting Ltd.  All rights reserved.
**
** This file is part of Qt Jambi.
**
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import org.qtjambi.autotests.generated.MetaObjectQtMetaCast;

import org.qtjambi.qt.widgets.QWidget;

public class TestMetaObjectQtMetaCast extends QApplicationTest {
    // Not used
    private static class MyLauncher extends QWidget {
        public final Signal0 mySignal = new Signal0();
        private int slotCalledCount;
        public void mySlot() {
            synchronized (this) {
                slotCalledCount++;
            }
        }
        public int getSlotCalledCount() {
            synchronized (this) {
                return slotCalledCount;
            }
        }
    }
    private static class MetaObjectQtMetaCastSubclass extends MetaObjectQtMetaCast {
    }

    @Test
    public void testBasicUsage() {
        MetaObjectQtMetaCastSubclass moqmc = new MetaObjectQtMetaCastSubclass();

        String thisClass = TestMetaObjectQtMetaCast.class.getName().replace(".", "::");
        String s;

        MyLauncher myLauncher = new MyLauncher();  // the target of our inspection

        s = moqmc.className(myLauncher);
        assertEquals(thisClass + "$MyLauncher", s);

        s = moqmc.superClassName(myLauncher, 0);
        assertEquals("QWidget", s);

        s = moqmc.superClassName(myLauncher, 1);
        assertEquals("QObject", s);

        s = moqmc.superClassName(myLauncher, 2);
        assertEquals("", s); // FIXME: expected null here 

        assertTrue(moqmc.inherits(myLauncher, "QObject"));
        assertFalse(moqmc.inherits(myLauncher, "QtJambiShell_QObject"));
        assertTrue(moqmc.inherits(myLauncher, "QWidget"));
        assertTrue(moqmc.inherits(myLauncher, "QtJambiShell_QWidget"));

        //assertTrue(moqmc.inherits(myLauncher, thisClass + "$MyLauncher"));

        long l;
        l = moqmc.do_qt_metacast(myLauncher, "QObject");
        assertTrue(isMetacastSuccessful(l));

        l = moqmc.do_qt_metacast(myLauncher, "QWidget");
        assertTrue(isMetacastSuccessful(l));

        l = moqmc.do_qt_metacast(myLauncher, "QtJambiShell_QWidget");
        assertTrue(isMetacastSuccessful(l));

        l = moqmc.do_qt_metacast(myLauncher, "QtJambiShell_QObject");
        assertFalse(isMetacastSuccessful(l));  // NOT

        l = moqmc.do_qt_metacast(myLauncher, thisClass + "$MyLauncher");
        assertTrue(isMetacastSuccessful(l));
    }

    /**
     * Driven by bug#213
     */
    @Test
    public void testBug213() {
        MetaObjectQtMetaCastSubclass moqmc = new MetaObjectQtMetaCastSubclass();

        MyLauncher myLauncher = new MyLauncher();  // the target of our inspection

        assertTrue(moqmc.inherits(myLauncher, "QObject"));
        assertFalse(moqmc.inherits(myLauncher, "QtJambiShell_QObject"));
        assertTrue(moqmc.inherits(myLauncher, "QWidget"));
        assertTrue(moqmc.inherits(myLauncher, "QtJambiShell_QWidget"));

        // This is the bug being reported
        String thisClass = TestMetaObjectQtMetaCast.class.getName().replace(".", "::");
        assertTrue(moqmc.inherits(myLauncher, thisClass + "$MyLauncher"));

        long l = moqmc.do_qt_metacast(myLauncher, thisClass + "$MyLauncher");
        assertTrue(isMetacastSuccessful(l));
    }

    private boolean isMetacastSuccessful(long l) {
        if(l != 0 && l != -1)
            return true;
        return false;
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaObjectQtMetaCast.class.getName());
    }
}
