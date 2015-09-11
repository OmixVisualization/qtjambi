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
import org.qtjambi.qt.gui.QPainter.RenderHint;

public class GlassPane extends QWidget {

    public static class Overlay extends QWidget {
        public enum Transition {
            Fade,
            ScrollLeft,
            ScrollRight,
            ScrollUp,
            ScrollDown,
            SplitVertical,
            SplitHorizontal,
            Rotate,
            Rect,
            Flip,
            Stuff
        };

        public Overlay(QPixmap start, QPixmap end) {
            this.start = start;
            this.end = end;
            this.timeline = new QTimeLine(700, this);
            transition = Transition.Fade;

            setAttribute(Qt.WidgetAttribute.WA_OpaquePaintEvent);

            timeline.finished.connect(this, "disposeLater()");
            timeline.finished.connect(this, "hide()");
            timeline.valueChanged.connect(this, "update(double)");
            timeline.start();
        }

        public void update(double d) {
            level = d;
            update();
        }

        public void setTransition(Transition type) {
            transition = type == null ? Transition.Fade : type;
        }

        @Override
        protected void paintEvent(QPaintEvent e) {
            QPainter p = new QPainter(this);

            switch (transition) {
            case Fade:
                p.drawPixmap(0, 0, start);
                p.setOpacity(level);
                p.drawPixmap(0, 0, end);
                break;
            case ScrollLeft:
                p.drawPixmap(0, 0, end);
                p.drawPixmap((int) (level * width()), 0, start);
                break;
            case ScrollRight:
                p.drawPixmap(0, 0, end);
                p.drawPixmap((int) (-level * width()), 0, start);
                break;
            case ScrollUp:
                p.drawPixmap(0, 0, end);
                p.drawPixmap(0, (int) (-level * height()), start);
                break;
            case ScrollDown:
                p.drawPixmap(0, 0, end);
                p.drawPixmap(0, (int) (level * height()), start);
                break;
            case SplitHorizontal:
                if (level < 0.2) {
                    level *= 5;
                    p.drawLine(0, height() / 2, (int)(level * width()), height() / 2);
                } else {
                    level = (level - 0.2) / 0.8;
                    int h2 = start.height() / 2;
                    p.drawPixmap(0, 0, end);
                    p.drawPixmap(0, (int) (-level * height()), start.width(), h2,
                                 start,
                                 0, 0, start.width(), h2);
                    p.drawPixmap(0, (int) (level * height()) + h2, start.width(), h2,
                                 start,
                                 0, h2, start.width(), h2);
                }
                break;
            case SplitVertical:
                if (level < 0.2) {
                    level *= 5;
                    p.drawLine(width() / 2, 0, width() / 2, (int) (level * height()));
                } else {
                    level = (level - 0.2) / 0.8;
                    int w2 = start.width() / 2;
                    p.drawPixmap(0, 0, end);
                    p.drawPixmap((int) (level * width()) + w2, 0, w2, start.height(),
                                 start,
                                 w2, 0, w2, start.height());
                    p.drawPixmap((int) (-level * height()), 0, w2, start.height(),
                                 start,
                                 0, 0, w2, start.height());
                }
                break;
            case Rotate:
                p.eraseRect(rect());

                p.setRenderHint(RenderHint.SmoothPixmapTransform);
                p.rotate(level * 90);
                p.drawPixmap(0, 0, start);

                p.rotate(-90);
                p.drawPixmap(0, 0, end);
                break;

            case Rect:
                int w = start.width();
                int h = start.height();

                p.drawPixmap((int)(w/2 - w/2 * level), (int)(h/2 - h/2 * level), end, (int)(w/2 - w/2 * level), (int)(h/2 - h/2 * level), (int)(w * level) , (int)(h * level));
                p.drawRoundRect((int)(w/2 - w/2 * level), (int)(h/2 - h/2 * level), (int)(w * level) , (int)(h * level), 10, 10);
                break;

            case Flip:
                double l = Math.sin(level * Math.PI/2);

                p.eraseRect(rect());
                p.setRenderHint(RenderHint.SmoothPixmapTransform);
                if( l < 0.5 ) {
                    p.translate(0, height() * l);
                    p.scale(1, 1 - l * 2);
                    p.drawPixmap(0, 0, start);
                }
                else {
                    p.translate(0, height() * (1 - l));
                    p.scale(1, (0.5 - l) * -2);
                    p.drawPixmap(0, 0, end);
                }
                break;
            case Stuff:
                p.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform);
                p.fillRect(rect(), palette().brush(backgroundRole()));

                int x = (int) ((1 - level)  * width());
                int h2 = height() / 2;

                QTransform t = new QTransform();
                t.translate(x, h2);
                t.rotate(level * 90, Qt.Axis.YAxis);
                p.setTransform(t);
                p.drawPixmap(-start.width(), -h2, start);

                t = new QTransform();
                t.translate(x, h2);
                t.rotate((1 - level) * -90, Qt.Axis.YAxis);
                p.setTransform(t);
                p.drawPixmap(0, -h2, end);
            }

        }

        private Transition transition;
        private double level;
        private QTimeLine timeline;
        private QPixmap start;
        private QPixmap end;
    }

    public static QWidget createPanel(String baseTitle) {
        QWidget widget = new QGroupBox(baseTitle);

        QGridLayout layout = new QGridLayout(widget);
        for (int i=0; i<6; ++i) {
            QLabel label = new QLabel(baseTitle + ": " + (i+1));
            QLineEdit edit = new QLineEdit();
            QToolButton button = new QToolButton();

            layout.addWidget(label, i, 0);
            layout.addWidget(edit, i, 1);
            layout.addWidget(button, i, 2);
        }
        return widget;
    }


    public GlassPane() {
        // Initialization
        panels = new QStackedWidget();
        toggle = new QPushButton("&Toggle");
        frontPanel = createPanel("Front Panel");
        backPanel = createPanel("Back Panel");
        selector = new QComboBox();

        // Extended setup
        for (Overlay.Transition t : Overlay.Transition.values()) {
            selector.addItem(t.name());
        }
        // Layout
        QVBoxLayout layout = new QVBoxLayout(this);
        layout.addWidget(panels);
        layout.addWidget(toggle);
        layout.addWidget(selector);
        panels.addWidget(frontPanel);
        panels.addWidget(backPanel);

        // connections
        toggle.clicked.connect(this, "toggle()");
        selector.activated.connect(this, "changeTransition(String)");
    }

    @SuppressWarnings("unused")
    private void toggle() {
        QWidget current = panels.currentWidget();
        QWidget next = panels.widget((panels.currentIndex() + 1) % 2);

        // make sure the layout is up to date...
        next.setGeometry(current.geometry());

        QPixmap startpm = QPixmap.grabWidget(current);
        QPixmap endpm = QPixmap.grabWidget(next);

        Overlay overlay = new Overlay(startpm, endpm);
        overlay.setTransition(transition);
        overlay.setParent(this);
        overlay.setGeometry(panels.geometry());
        overlay.show();

        panels.setCurrentIndex((panels.currentIndex() + 1) % 2);
    }

    @SuppressWarnings("unused")
    private void changeTransition(String type) {
        for (Overlay.Transition t : Overlay.Transition.values()) {
            if (type.equals(t.name())) {
                this.transition = t;
                break;
            }
        }
    }

    private Overlay.Transition transition;
    private QWidget frontPanel;
    private QWidget backPanel;
    private QStackedWidget panels;
    private QPushButton toggle;
    private QComboBox selector;

    public static void main(String args[]) {
        QApplication.initialize(args);

        GlassPane pane = new GlassPane();
        pane.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
