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

package org.qtjambi.benchmarks.signalslot;

import org.qtjambi.qt.core.*;

public class EmitVsDirect extends QObject {
    public static final int TIME = 10000;

    public Signal2<Integer, QByteArray> signal_int_QByteArray;
    public Signal3<Integer, QByteArray, Double> signal_int_QByteArray_double;

    private int x;

    private void private_slot_int_QByteArray(int i, QByteArray ar) {
        x = i % ar.size();
    }

    protected void protected_slot_int_QByteArray(int i, QByteArray ar) {
        x = i % ar.size();
    }

    public void public_slot_int_QByteArray(int i, QByteArray ar) {
        x = i % ar.size();
    }

    public static void main(String args[]) {
        QByteArray ar = new QByteArray("abcdefghijklmnopqrstuvwxyz");

        {
            EmitVsDirect obj = new EmitVsDirect();
            obj.signal_int_QByteArray.connect(obj, "private_slot_int_QByteArray(int,QByteArray)");

            QTime t = new QTime();
            t.start();
            long callTimes = 0;
            while (t.elapsed() < TIME) {
                for (int i=0; i<1000; ++i) {
                    obj.signal_int_QByteArray.emit(i, ar);
                }
                callTimes += 1000;
            }
            System.out.printf("emittions to private slot took %f ms / op\n", t.elapsed() / (double) callTimes);
        }

        {
            EmitVsDirect obj = new EmitVsDirect();
            obj.signal_int_QByteArray.connect(obj, "protected_slot_int_QByteArray(int,QByteArray)");

            QTime t = new QTime();
            t.start();
            long callTimes = 0;
            while (t.elapsed() < TIME) {
                for (int i=0; i<1000; ++i) {
                    obj.signal_int_QByteArray.emit(i, ar);
                }
                callTimes += 1000;
            }
            System.out.printf("emittions to protected slots took %f ms / op\n",
                              t.elapsed() / (double) callTimes);
        }

        {
            EmitVsDirect obj = new EmitVsDirect();
            obj.signal_int_QByteArray.connect(obj, "public_slot_int_QByteArray(int,QByteArray)");

            QTime t = new QTime();
            t.start();
            long callTimes = 0;
            while (t.elapsed() < TIME) {
                for (int i=0; i<1000; ++i) {
                    obj.signal_int_QByteArray.emit(i, ar);
                }
                callTimes += 1000;
            }
            System.out.printf("emittions to public slots took %f ms / op\n", t.elapsed() / (double) callTimes);
        }

        {
            EmitVsDirect obj = new EmitVsDirect();
            obj.signal_int_QByteArray_double.connect(obj, "public_slot_int_QByteArray(int,QByteArray)");

            QTime t = new QTime();
            t.start();
            long callTimes = 0;
            while (t.elapsed() < TIME) {
                for (int i=0; i<1000; ++i) {
                    obj.signal_int_QByteArray_double.emit(i, ar, 3.2);
                }
                callTimes += 1000;
            }
            System.out.printf("emittions to public slots, (wrong argument count) took %f ms / op\n", t.elapsed() / (double) callTimes);
        }

        {
            EmitVsDirect obj = new EmitVsDirect();
            QTime t = new QTime();
            t.start();
            long callTimes = 0;
            while (t.elapsed() < TIME) {
                for (int i=0; i<1000; ++i) {
                    obj.public_slot_int_QByteArray(i, ar);
                }
                callTimes += 1000;
            }
            System.out.printf("direct call took %f ms / op\n", t.elapsed() / (double) callTimes);
        }
    }
}
