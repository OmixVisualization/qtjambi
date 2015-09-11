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

import org.qtjambi.qt.core.*;

public class ObjectOwnership {

    public static class DyingObject extends QObject {

        public DyingObject() {
            System.out.println("created: " + id);
            setObjectName("DyingObject(" + id + ")");
        }

        @Override
        public void disposed() {
            System.out.println(" -> disposed: " + id);
        }

        static int counter = 0;
        public int id = ++counter;
    }

    public static void main(String args[]) throws InterruptedException {

        QCoreApplication.initialize(args);

        space();
        System.out.println("Creating an object in scope and calling system gc...");
        oneObject();
        System.gc();

        space();
        System.out.println("Creating an object with parent in scope and calling system gc..");
        objectWithParent();
        System.gc();
        QCoreApplication.processEvents(QEventLoop.ProcessEventsFlag.DeferredDeletion);

        space();
        System.out.println("Creating an object with parent, then unparent it and calling system gc..");
        objectWithUnParent();
        System.gc();
        QCoreApplication.processEvents(QEventLoop.ProcessEventsFlag.DeferredDeletion);

        space();
        System.out.println("Creating an object in a thread and explicitly dispose it");
        objectInThreadExplicitDelete();
        System.gc();

        space();
        System.out.println("Creating an object in a thread and rely on gc");
        objectInThreadImplicitDelete();
        System.gc();
        QCoreApplication.processEvents(QEventLoop.ProcessEventsFlag.DeferredDeletion);

        QCoreApplication.invokeLater(new Runnable() {
            public void run() {
                QCoreApplication.quit();
            }
        });
        QCoreApplication.execStatic();
        QCoreApplication.shutdown();
    }

    private static void space() {
        System.out.println("\n\n");
    }

    private static void oneObject() {
        new DyingObject();
    }

    private static void objectWithParent() {
        DyingObject p = new DyingObject();
        new DyingObject().setParent(p);
    }

    private static void objectWithUnParent() {
        DyingObject p = new DyingObject();
        DyingObject o = new DyingObject();
        o.setParent(p);
        o.setParent(null);
    }

    private static void objectInThreadExplicitDelete() {
        Thread t = new Thread() {
            @Override
            public void run() {
                DyingObject o = new DyingObject();
                o.dispose();
            }
        };

        t.start();
        try {
            t.join(1000);
        } catch (Exception e) { e.printStackTrace(); }
    }

    private static void objectInThreadImplicitDelete() {
        Thread t = new Thread(new Runnable() {
            public void run() {
                oneObject();
                objectWithParent();
                objectWithUnParent();
            }
        });

        t.start();
        try {
            t.join(1000);
        } catch (Exception e) { e.printStackTrace(); }
    }


}
