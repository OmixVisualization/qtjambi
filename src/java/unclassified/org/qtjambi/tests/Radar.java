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

public class Radar extends QWidget {

    private static QColor dark = new QColor(25, 50, 25);
    private static QColor light = new QColor(0, 127, 0);
    private static QColor mark = new QColor(0, 50, 0);

    public Radar() {
        startTimer(50);

        QConicalGradient gradient = new QConicalGradient(0, 0, 0);
        gradient.setColorAt(0, dark);
        gradient.setColorAt(0.01, light);
        gradient.setColorAt(0.25, dark);
        brush = new QBrush(gradient);

        pen = new QPen(mark);
    }

    protected void timerEvent(QTimerEvent e) {
        rotation += 2;
        update();
    }

    protected void paintEvent(QPaintEvent e) {
        QPainter p = new QPainter(this);

        p.setRenderHint(QPainter.RenderHint.Antialiasing);

        p.scale(width() * 0.5, height() * 0.5);
        p.translate(1, 1);

        // Draw radar gradient background
        p.save();
          p.rotate(rotation);
          p.setBrush(brush);
          p.setPen(QPen.NoPen);
          p.drawEllipse(-1, -1, 2, 2);
        p.restore();

        // Draw the lines...
        p.setPen(pen);
        int circles = 6;
        for (int i=circles; i>0; --i) {
            double r = i / (double) circles;
            p.drawEllipse(new QRectF(-r, -r, r*2, r*2));
        }

    }

    public QSize sizeHint() {
        return new QSize(200, 200);
    }

    private QPen pen;
    private QBrush brush;
    private double rotation;

    public static void main(String args[]) {
        QApplication.initialize(args);

        Radar r = new Radar();
        r.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
