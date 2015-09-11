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

import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

public class Threads implements Runnable {
    static Reciever global_reciever = null;
    public QEventLoop eventLoop;

    public static class Reciever extends QWidget {
        @Override
        protected void customEvent(QEvent e) {
            System.out.println("got event in thread="
                    + Thread.currentThread().getId() + ", type=" + e.type());
        }
    }

    public class ThreadReceiver extends QObject {
        @Override
        public boolean event(QEvent e) {
            if (e.type() == QEvent.Type.Quit) {
                System.out.println("ThreadReceiver should call quit()");
                eventLoop.quit();
            } else if (e.type().value() > QEvent.Type.User.value()) {
                System.out.println("ThreadReceiver: got user event: " + Thread.currentThread().getId() + ", id=" + e.type());
            }

            return super.event(e);
        }
    }

    public void run() {
        System.out.println("Thread: " + this + ", starting");

        QTime time = new QTime();
        time.start();

        int counter = 0;
        ThreadReceiver tr = new ThreadReceiver();

        while (time.elapsed() < 1000) {
            try {
                Thread.sleep(10);
            } catch (Exception e) {
            }

            QEvent e = new QEvent(QEvent.Type.resolve(QEvent.Type.User.value() + ++counter));
            QApplication.postEvent(tr, e);
        }

        QEvent e = new QEvent(QEvent.Type.Quit);
        QApplication.postEvent(tr, e);

        eventLoop = new QEventLoop();
        eventLoop.exec();

        System.out.println("Thread: " + this + ", completed");
        System.out.println("object.thread=" + tr.thread());
    }

    public static void runThreads(int count) {
        Thread threads[] = new Thread[count];
        for (int i = 0; i < count; ++i) {
            threads[i] = new Thread(new Threads());
            threads[i].start();
        }

        for (int i=0; i<count; ++i)
            try { threads[i].join(); } catch (Exception e) { }
    }

    public static void main(String args[]) {
        QApplication.initialize(args);
        runThreads(10);

        System.gc();

        System.out.println("All done...");

        //QApplication.execStatic();
        QApplication.shutdown();
    }
};
