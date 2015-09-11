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

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.opengl.*;

import javax.swing.*;
import java.awt.*;



public class DrawLines {

    private static int SIZE = 513;
    private static int HOR_MULT = 0;
    private static int VER_MULT = 1;

    private static class AwtWidget extends Component {

    private static final long serialVersionUID = 1L;

    @Override
    public void paint(Graphics g) {
        g.setColor(new Color(255, 0, 0));

        int runningTime = 1000;

        for (int size=8; size<=512; size*=2) {

        long drawCount = 0;
        long startTime = System.currentTimeMillis();
        long endTime = startTime + runningTime;
        while (System.currentTimeMillis() < endTime) {
            for (int j=0; j<100; ++j) {
            g.drawLine(0, 0, HOR_MULT * size, VER_MULT * size);
            ++drawCount;
            }
        }

        long opsPrSec = (drawCount * 1000 / (endTime - startTime));
        System.out.printf("Awt:   diagonal lines: size=%3d: ops/sec=%d\n", size, opsPrSec);
        }

        setVisible(false);
    }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(SIZE, SIZE);
    }
    }


    private static class QtWidget extends QWidget {
    @Override
    protected void paintEvent(QPaintEvent e) {

        QPainter p = new QPainter();
        p.begin(this);

        p.setPen(new QPen(QColor.red));

        int runningTime = 1000;

        for (int size=8; size<=512; size*=2) {
        long drawCount = 0;
        long startTime = System.currentTimeMillis();
        long endTime = startTime + runningTime;
        while (System.currentTimeMillis() < endTime) {
            for (int j=0; j<100; ++j) {
            p.drawLine(0, 0, HOR_MULT * size, VER_MULT * size);
            ++drawCount;
            }
        }

        long opsPrSec = (drawCount / (endTime - startTime) * 1000);
        System.out.printf("Qt:    diagonal lines: size=%3d: ops/sec=%d\n", size, opsPrSec);
        }

        p.end();

        hide();
    }

    @Override
    public QSize sizeHint() {
        return new QSize(SIZE, SIZE);
    }
    }

    private static class QtGLWidget extends QGLWidget {
    @Override
    protected void paintEvent(QPaintEvent e) {

        QPainter p = new QPainter();
        p.begin(this);

        p.setPen(new QPen(QColor.red));

        int runningTime = 1000;

        for (int size=8; size<=512; size*=2) {
        long drawCount = 0;
        long startTime = System.currentTimeMillis();
        long endTime = startTime + runningTime;
        while (System.currentTimeMillis() < endTime) {
            for (int j=0; j<100; ++j) {
            p.drawLine(0, 0, HOR_MULT * size, VER_MULT * size);
            ++drawCount;
            }
        }

        long opsPrSec = (drawCount / (endTime - startTime) * 1000);
        System.out.printf("Qt/GL: diagonal lines: size=%3d: ops/sec=%d\n", size, opsPrSec);
        }

        p.end();

        hide();
    }

    @Override
    public QSize sizeHint() {
        return new QSize(SIZE, SIZE);
    }
    }

    public static void main(String args[]) {
    {
        JFrame f = new JFrame();
        AwtWidget awt = new AwtWidget();
        f.add(awt);

        f.pack();

        f.setVisible(true);

    }


    try { Thread.sleep(10 * 1000); } catch (Exception e) { }

    {
        QApplication.initialize(args);

        QtWidget w = new QtWidget();
        w.show();

        QtGLWidget wgl = new QtGLWidget();
        wgl.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }



    }
}
