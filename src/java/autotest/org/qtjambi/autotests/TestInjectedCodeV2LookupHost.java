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
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.network.QHostInfo;

public class TestInjectedCodeV2LookupHost extends QApplicationTest {
    public static class LookupHostQObject extends QObject {
        public String fromFirstSlot = "";
        public String fromSecondSlot = "";

        // You gotta love that fruity company... Their latest version
        // of Java for 32-bit, Java 1.5.0_16 introduces a bug where
        // generics innerclasses from other packages tends to lead to
        // internal compiler errors. They have of course moved on to
        // 64-bit and Java 1.6 as their new platform so this bug will
        // live forever.
        //
        // The error manifests itself in the helloObject.mySignal
        // access in the testLookupHostWithSignal function below.
        //
        // Bug ID: 6356636 on Bugparade
//        public Signal1<QHostInfo> mySignal = new Signal1<QHostInfo>();
//        public Signal1/*<QHostInfo>*/ mySignal/* = new Signal1<QHostInfo>()*/;
        public Signal1<QHostInfo> mySignal/* = new Signal1<QHostInfo>()*/;
        {
            //mySignal.connect(this, "secondSlot(QHostInfo)");
        }
        public LookupHostQObject() {
            mySignal = new Signal1<QHostInfo>();
            //Signal1<QHostInfo> xSignal = mySignal;
            mySignal.connect(this, "secondSlot(QHostInfo)");
//            mySignal.connect(this, "secondSlot()");
//            mySignal.connect(this, "secondSlot");
        }

        public void firstSlot(QHostInfo info) {
            fromFirstSlot = info.addresses().isEmpty() ? "null" : info.addresses().get(0).toString();
        }

        public void secondSlot(QHostInfo info) {
            fromSecondSlot = info.addresses().isEmpty() ? "null" : info.addresses().get(0).toString();
        }
    }

    @Test
    public void testLookupHostWithSlot()
    {
        LookupHostQObject helloObject = new LookupHostQObject();

        // This API does not need the extra argument only the method name
        QHostInfo.lookupHost("qt.io", helloObject, "firstSlot" /*+"(QHostInfo)"*/);
        int i = 0;
        while (helloObject.fromFirstSlot.length() == 0 && i < 100) {
            QApplication.processEvents();
            try {
                Thread.sleep(100);
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            i++;
        }

        // Now: 62.116.130.8 (since 22-08-2015)
        assertEquals("62.116.130.8", helloObject.fromFirstSlot);
        Utils.println(2, "RESULT: " + helloObject.fromFirstSlot + " for " + "qt.io");
    }

    @SuppressWarnings("unchecked")
    private void lookupHost(String host, Signal1 signal) {
        // Put in a method on its own for @SuppressWarnings isolation
        QHostInfo.lookupHost(host, signal);
    }

    @Test
    public void testLookupHostWithSignal()
    {
        LookupHostQObject helloObject = new LookupHostQObject();

        // We can't let the Sun JDK5 compiler know the genericified type otherwise it triggers the bug.
        // So this cast (to ensure it is a raw type) allows correct code to compiled and the results it
        // correct code generated (correct enough to pass the testcase anyhow).
        // Ideally this would be (or without cast if the type is already qualified) :
        // QHostInfo.lookupHost("qt.io", (Signal1<QHostInfo>)helloObject.mySignal);
        // QHostInfo.lookupHost("qt.io", (Signal1/*<QHostInfo>*/)helloObject.mySignal);
        lookupHost("qt.io", (Signal1/*<QHostInfo>*/)helloObject.mySignal);
        int i = 0;
        while (helloObject.fromSecondSlot.length() == 0 && i < 100) {
            QApplication.processEvents();
            try {
                Thread.sleep(100);
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            i++;
        }

        // Now: 62.116.130.8 (since 22-08-2015)
        assertEquals("62.116.130.8", helloObject.fromSecondSlot);
        Utils.println(2, "RESULT: " + helloObject.fromSecondSlot + " for " + "qt.io");
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestInjectedCodeV2LookupHost.class.getName());
    }
}
