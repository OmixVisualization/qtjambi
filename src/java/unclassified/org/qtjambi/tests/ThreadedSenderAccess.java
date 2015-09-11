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

package org.qtjambi.tests;

import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

import java.util.*;

public class ThreadedSenderAccess extends QObject implements Runnable {

    QSignalEmitter.Signal0 signal = new QSignalEmitter.Signal0();
    boolean doBlock = false;
    private int i;
    private ThreadedSenderAccess other;
    HashMap<QSignalEmitter, Integer> stuff = new HashMap<QSignalEmitter, Integer>();

    public ThreadedSenderAccess(int i) {
        this.i = i;
    }

    public void run() {
        signal.connect(this, "accessSender()");
        System.out.println("executing thread: " + Thread.currentThread().getId());

        while (1 + 1 == 2) {
            signal.emit();
        }
    }

    public void accessSender() {
        if (signalSender() != other) {
            throw new RuntimeException("Bad signalSender()");
        }

        if (doBlock)
            return;

        ThreadedSenderAccess that = new ThreadedSenderAccess(-1);
        that.doBlock = true;
        that.other = that;
        that.signal.connect(that, "accessSender()");
        that.signal.emit();
        that.dispose();

        if (signalSender() != other) {
            throw new RuntimeException("Bad signalSender()");
        }
    }

    public void stuff() {
        QSignalEmitter emitter = QSignalEmitter.signalSender();

        Integer callTimes = stuff.get(emitter);
        int ct = callTimes != null ? callTimes.intValue() : 0;
        stuff.put(emitter, ct + 1);

        if (ct % 100 == 0) {
            System.out.println("called 100 times from: " + emitter);
        }
    }

    @Override
    public String toString() { return "Accessor(" + i + ")"; }

    public static void main(String args[]) {
        QApplication.initialize(args);

        ThreadedSenderAccess me = new ThreadedSenderAccess(0);

        for (int i=0; i<10; ++i) {
            ThreadedSenderAccess access = new ThreadedSenderAccess(i+1);
            access.other = access;
            access.signal.connect(me, "stuff()");

            Thread t = new Thread(access);
            access.moveToThread(t);
            t.start();

        }

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
