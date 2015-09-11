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
import org.qtjambi.extensions.awt.*;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.*;

class Renderer {
    public static void render(Graphics2D g) {
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        g.setColor(Color.red);
        g.fillRect(0, 0, 100, 100);

        g.setColor(Color.blue);
        g.drawRect(50, 50, 75, 50);

        g.fillOval(100, 200, 30, 30);

        g.setColor(Color.green);
        g.drawLine(200, 100, 300, 100);
        g.drawString("Hello Dolly", 200, 100);

        g.setPaint(new GradientPaint(100, 100, Color.blue, 200, 200, new Color(255, 255, 255, 63)));

        GeneralPath path = new GeneralPath();
        path.moveTo(100, 100);
        path.curveTo(200, 100, 150, 150, 200, 200);
        path.lineTo(100, 100);
        g.fill(path);
    }
}

public class GraphicsTest extends QWidget {

    public static class Component extends JComponent {
        private static final long serialVersionUID = 1L;

        @Override
        public void paintComponent(Graphics g) {
            Renderer.render((Graphics2D) g);
        }

        @Override
        public Dimension getPreferredSize() {
            return new Dimension(500, 400);
        }
    }


    public GraphicsTest() {
        this(null);
    }

    public GraphicsTest(QWidget parent) {
        super(parent);
    }

    @Override
    protected void paintEvent(QPaintEvent e) {
        QPainter p = new QPainter(this);
        org.qtjambi.extensions.awt.QPainterGraphics g = new QPainterGraphics(p);

        System.out.println();
        Renderer.render(g);
    }

    @Override
    public QSize sizeHint() {
        return new QSize(500, 400);
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        GraphicsTest w = new GraphicsTest();
        w.show();

        JFrame f = new JFrame();
        Component comp = new Component();

        f.getContentPane().add(comp);
        f.pack();
        f.setVisible(true);

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
