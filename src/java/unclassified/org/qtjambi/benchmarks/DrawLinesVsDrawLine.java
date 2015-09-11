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

package org.qtjambi.benchmarks;

import java.util.ArrayList;
import java.util.List;

import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QWidget;


public class DrawLinesVsDrawLine extends QWidget {

    void drawLines(QPainter p, int size) {
        double radius = 100.0;
        p.save();
        p.translate(width() / 2.0, height() / 2.0);
        p.setPen(QColor.black);

        long millis = System.currentTimeMillis();

        List<QLineF> lines = new ArrayList<QLineF>(size);

        for (int i=0; i<size; ++i) {
            double degree = i % 360;
            lines.add(new QLineF(Math.cos(degree) * radius, Math.sin(degree) * radius,
                                   Math.cos(degree) * -radius, Math.sin(degree) * -radius));
        }

        p.drawLinesF(lines);

        long delta = System.currentTimeMillis() - millis;
        System.out.println("drawLines -- size == " + size + " -- time == " + delta + " ms.");
        p.restore();
    }

    void drawCachedLines(QPainter p, int size) {
        double radius = 100.0;
        p.save();
        p.translate(width() / 2.0, height() / 2.0);
        p.setPen(QColor.black);


        List<QLineF> lines = new ArrayList<QLineF>(size);

        for (int i=0; i<size; ++i) {
            double degree = i % 360;
            lines.add(new QLineF(Math.cos(degree) * radius, Math.sin(degree) * radius,
                                   Math.cos(degree) * -radius, Math.sin(degree) * -radius));
        }

        long millis = System.currentTimeMillis();
        p.drawLinesF(lines);

        long delta = System.currentTimeMillis() - millis;
        System.out.println("drawCachedLines -- size == " + size + " -- time == " + delta + " ms.");
        p.restore();
    }

    void drawLine(QPainter p, int size) {
        double radius = 100.0;
        p.save();
        p.translate(width() / 2.0, height() / 2.0);
        p.setPen(QColor.black);

        long millis = System.currentTimeMillis();

        for (int i=0; i<size; ++i) {
            double degree = i % 360;
            p.drawLine(new QLineF(Math.cos(degree) * radius, Math.sin(degree) * radius,
                    Math.cos(degree) * -radius, Math.sin(degree) * -radius));
        }

        long delta = System.currentTimeMillis() - millis;
        System.out.println("drawLine -- size == " + size + " -- time == " + delta + " ms.");
        p.restore();
    }

    void drawCachedLine(QPainter p, int size) {
        double radius = 100.0;
        p.save();
        p.translate(width() / 2.0, height() / 2.0);
        p.setPen(QColor.black);

        QLineF lines[] = new QLineF[size];
        for (int i=0; i<size; ++i) {
            double degree = i % 360;
            lines[i] = new QLineF(Math.cos(degree) * radius, Math.sin(degree) * radius,
                                  Math.cos(degree) * -radius, Math.sin(degree) * -radius);
        }

        long millis = System.currentTimeMillis();
        for (int i=0; i<size; ++i) {
            p.drawLine(lines[i]);
        }

        long delta = System.currentTimeMillis() - millis;
        System.out.println("drawCachedLine -- size == " + size + " -- time == " + delta + " ms.");
        p.restore();
    }

    private int do_this_just_once = 0;
    @Override
    protected void paintEvent(QPaintEvent e) {
        if (do_this_just_once++ == 0) {
            QPainter p = new QPainter(this);
            for (int size=5000; size<=300000; size += size >= 50000 ? 50000 : 5000) {
                p.fillRect(rect(), new QBrush(QColor.white));
                drawLines(p, size);
                System.gc();

                p.fillRect(rect(), new QBrush(QColor.white));
                drawLine(p, size);
                System.gc();

                p.fillRect(rect(), new QBrush(QColor.white));
                drawCachedLines(p, size);
                System.gc();

                p.fillRect(rect(), new QBrush(QColor.white));
                drawCachedLine(p, size);
                System.gc();
            }
            p.end();
        }


    }

    public static void main(String[] args) {
        QApplication.initialize(args);

        QWidget w = new DrawLinesVsDrawLine();
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
