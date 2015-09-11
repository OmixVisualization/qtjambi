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

package org.qtjambi.extensions.jython;

import org.qtjambi.qt.*;
import org.python.core.*;

public class Signals {

    private static String signature(int count) {
        StringBuilder s = new StringBuilder();
        s.append("execute_").append(count).append('(');
        for (int i=0; i<count; ++i) {
            if (i != 0)
                s.append(',');
            s.append("Object");
        }
        s.append(')');
        return s.toString();
    }

    public static void connect(QSignalEmitter.AbstractSignal signal,
                               PyMethod method) {
        PythonSlot receiver = new PythonSlot(method);
        if (signal instanceof QSignalEmitter.Signal0)
            ((QSignalEmitter.Signal0) signal).connect(receiver, signature(0));
        else if (signal instanceof QSignalEmitter.Signal1)
            ((QSignalEmitter.Signal1) signal).connect(receiver, signature(1));
        else if (signal instanceof QSignalEmitter.Signal2)
            ((QSignalEmitter.Signal2) signal).connect(receiver, signature(2));
        else if (signal instanceof QSignalEmitter.Signal3)
            ((QSignalEmitter.Signal3) signal).connect(receiver, signature(3));
        else if (signal instanceof QSignalEmitter.Signal4)
            ((QSignalEmitter.Signal4) signal).connect(receiver, signature(4));
        else if (signal instanceof QSignalEmitter.Signal5)
            ((QSignalEmitter.Signal5) signal).connect(receiver, signature(5));
        else if (signal instanceof QSignalEmitter.Signal6)
            ((QSignalEmitter.Signal6) signal).connect(receiver, signature(6));
        else if (signal instanceof QSignalEmitter.Signal7)
            ((QSignalEmitter.Signal7) signal).connect(receiver, signature(7));
        else if (signal instanceof QSignalEmitter.Signal8)
            ((QSignalEmitter.Signal8) signal).connect(receiver, signature(8));
        else if (signal instanceof QSignalEmitter.Signal9)
            ((QSignalEmitter.Signal9) signal).connect(receiver, signature(9));
    }

    public static void connect(PythonSignal signal, PyMethod method) {
        PythonSlot receiver = new PythonSlot(method);
        if (signal instanceof Signal0) {
            ((Signal0) signal).signal.connect(receiver, signature(0));
        } else if (signal instanceof Signal1) {
            ((Signal1) signal).signal.connect(receiver, signature(1));
        } else if (signal instanceof Signal2) {
            ((Signal2) signal).signal.connect(receiver, signature(2));
        } else if (signal instanceof Signal3) {
            ((Signal3) signal).signal.connect(receiver, signature(3));
        } else if (signal instanceof Signal4) {
            ((Signal4) signal).signal.connect(receiver, signature(4));
        } else if (signal instanceof Signal5) {
            ((Signal5) signal).signal.connect(receiver, signature(5));
        } else if (signal instanceof Signal6) {
            ((Signal6) signal).signal.connect(receiver, signature(6));
        } else if (signal instanceof Signal7) {
            ((Signal7) signal).signal.connect(receiver, signature(7));
        } else if (signal instanceof Signal8) {
            ((Signal8) signal).signal.connect(receiver, signature(8));
        } else if (signal instanceof Signal9) {
            ((Signal9) signal).signal.connect(receiver, signature(9));
        }
    }

}

/*
  testcase:

from org.qtjambi.qt.gui import *
from org.qtjambi.extensions.jython import *

class MyButton (QPushButton):

    def __init__(self):
        self.setText("MyButton")
        self.setCheckable(1)
        self.bar = Signal1()
        Signals.connect(self.bar, self.zoot);
        Signals.connect(self.toggled, self.foo);

        self.bar.emit('foobar');

    def foo(self, on):
        print("toggled, re-emitting: %s" % on)
        self.bar.emit(on);

    def zoot(self, on):
        print("got %s from python signal" % on)


QApplication.initialize([])
button = MyButton();
button.show();
QApplication.execStatic();
QApplication.shutdown();

*/
