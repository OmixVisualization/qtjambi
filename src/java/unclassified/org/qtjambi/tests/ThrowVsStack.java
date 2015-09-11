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
import org.qtjambi.qt.gui.*;


public class ThrowVsStack {

    static void recurseTimeException(int rec) {
        if (rec == 0)
            timeException();
        else recurseTimeException(rec - 1);
    }

    static void timeException()
    {
        try {
            throw new Exception();
        } catch (Exception e) {
            StackTraceElement elements[] = e.getStackTrace();
            if (elements[1].getMethodName() == "just one of those strings...")
                System.out.println("...");
        }
    }

    static void recurseThread(int i) {
        if (i == 0)
            timeThreadLookup();
        else
            recurseThread(i - 1);
    }

    static void timeThreadLookup()
    {
        StackTraceElement elements[] = Thread.currentThread().getStackTrace();
        if (elements[1].getMethodName() == "just one of those strings...")
            System.out.println("...");
    }

    static void timeSillyFunctionCall(String s)
    {
        if (s == "just one of those strings...")
            System.out.println("...");
    }

    static void timeSillyFunctionCall(int s)
    {
        if (s == 7891235)
            System.out.println("...");
    }

    public static void main(String[] args)
    {
        int COUNT = args.length > 0 ? Integer.parseInt(args[0]) : 100000;

//        if (false) {
//            long t1 = System.currentTimeMillis();
//            for (int i=0; i<COUNT; ++i) {
//                recurseTimeException();
//            }
//            System.out.println("timeException " + COUNT + " times " + (System.currentTimeMillis() - t1));
//        }
//
//        if (false) {
//            long t1 = System.currentTimeMillis();
//            for (int i=0; i<COUNT; ++i) {
//                recurseThread();
//            }
//            System.out.println("timeThreadLookup " + COUNT + " times " + (System.currentTimeMillis() - t1));
//        }
//
       {
           long t1 = System.currentTimeMillis();
           for (int i=0; i<COUNT; ++i) {
               timeSillyFunctionCall(args[1]);
           }
           System.out.println("sullidull " + COUNT + " times " + (System.currentTimeMillis() - t1));
       }

       {
           long t1 = System.currentTimeMillis();
           for (int i=0; i<COUNT; ++i) {
               timeSillyFunctionCall(args[0]);
           }
           System.out.println("sullidull int " + COUNT + " times " + (System.currentTimeMillis() - t1));
       }

//       {
//           QObject o = new QObject();
//           long t1 = System.currentTimeMillis();
//           for (int i=0; i<COUNT; ++i) {
//               o.dumpObjectInfo();
//           }
//           System.out.println("dumpObjectInfo " + COUNT + " times " + (System.currentTimeMillis() - t1));
//       }
       {
           QObject o = new QObject();
           long t1 = System.currentTimeMillis();
           for (int i=0; i<COUNT; ++i) {
               o.killTimer(1000);
           }
           System.out.println("killTimer " + COUNT + " times " + (System.currentTimeMillis() - t1));
       }
       {
           QObject o = new QObject();
           long t1 = System.currentTimeMillis();
           String s = args[1];
           for (int i=0; i<COUNT; ++i) {
               o.setObjectName(s);
           }
           System.out.println("setObjectName " + COUNT + " times " + (System.currentTimeMillis() - t1));
       }

       if (true) {
            QApplication.initialize(args);
            QPixmap pm = new QPixmap(100, 100);
            QPainter p = new QPainter();
            p.begin(pm);
            p.setPen(Qt.PenStyle.NoPen);
            p.setBrush(new QBrush(new QColor(Qt.GlobalColor.blue)));
        long ops = 100000;
            long t1 = System.currentTimeMillis();
            for (int i=0; i<ops; ++i) {
                p.drawRect(0, 0, 100, 100);
            }
            long t2 = (System.currentTimeMillis() - t1);
            System.out.println("1 million rects took=" + t2 + ", " + ops * 1000.0 / t2 + "ops/sec");
        }
    }

}
