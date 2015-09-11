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

package org.qtjambi.examples;

import org.qtjambi.qt.core.QBasicTimer;
import org.qtjambi.qt.core.QTimerEvent;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QFontMetrics;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

//! [0]
class WigglyWidget extends QWidget {
    static final int sineTable[] = { 0, 38, 71, 92, 100, 92, 71, 38, 0, -38, -71, -92, -100, -92, -71, -38 };

    private QBasicTimer timer;
    private String text;
    private int step;
//! [0]

//! [1]
    WigglyWidget(QWidget parent) {
        super(parent);
        setBackgroundRole(QPalette.ColorRole.Midlight);

        QFont newFont = font();
        newFont.setPointSize(newFont.pointSize() + 20);
        setFont(newFont);
//! [1]

//! [2]
        step = 0;

        timer = new QBasicTimer();
        timer.start(60, this);
    }
//! [2]

//! [3]
    public void setText(String s) {
        text = s;
    }
//! [3]

    @Override
//! [4]
    protected void paintEvent(QPaintEvent e) {
        QFontMetrics metrics = new QFontMetrics(font());
        int x = (width() - metrics.width(text)) / 2;
        int y = (height() + metrics.ascent() - metrics.descent()) / 2;
        QColor color = new QColor();
//! [4]

//! [5]
        QPainter painter = new QPainter();
        painter.begin(this);
        for (int i = 0; i < text.length(); ++i) {
            int index = (step + i) % 16;
            color.setHsv((15 - index) * 16, 255, 191);
            painter.setPen(color);
            painter.drawText(x, y - ((sineTable[index] * metrics.height()) / 400), text.substring(i, i + 1));
            x += metrics.width(text.substring(i, i + 1));
        }
        painter.end();
//! [5] //! [6]
    }
//! [6]

    @Override
//! [7]
    protected void timerEvent(QTimerEvent event) {
        if (event.timerId() == timer.timerId()) {
            ++step;
            update();
        } else {
            super.timerEvent(event);
//! [7] //! [8]
        }
//! [8] //! [9]
    }
//! [9] //! [10]
}
//! [10]

@QtJambiExample(name = "Wiggly")
//! [11]
public class Wiggly extends QDialog {
//! [11] //! [12]
    public Wiggly(QWidget parent) {
        super(parent);

        WigglyWidget wigglyWidget = new WigglyWidget(null);
        QLineEdit lineEdit = new QLineEdit();

        QVBoxLayout layout = new QVBoxLayout();
        layout.addWidget(wigglyWidget);
        layout.addWidget(lineEdit);
        setLayout(layout);

        lineEdit.textChanged.connect(wigglyWidget, "setText(String)");

        lineEdit.setText("Hello world!");

        setWindowTitle("Wiggly");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        resize(360, 145);
    }
//! [12]

//! [13]
    public static void main(String args[]) {
        QApplication.initialize(args);

        Wiggly d = new Wiggly(null);
        d.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [13] //! [14]
}
//! [14]
