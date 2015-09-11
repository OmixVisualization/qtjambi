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

package org.qtjambi.examples.tutorial;

import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QLCDNumber;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QSlider;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

//! [0] //! [1]
public class BlocksGalore extends QWidget
{
    public BlocksGalore()
    {
        QPushButton quit = new QPushButton(tr("Quit"));
        quit.setFont(new QFont("Times", 18, QFont.Weight.Bold.value()));

        quit.clicked.connect(QApplication.instance(), "quit()");
//! [0]

//! [2]
        QGridLayout grid = new QGridLayout();
        QVBoxLayout layout = new QVBoxLayout();
        layout.addWidget(quit);
        layout.addLayout(grid);
        setLayout(layout);
        setWindowTitle(tr("Building Blocks Galore"));

//! [2] //! [3]
        for (int row = 0; row < 3; ++row) {
//! [3] //! [4]
            for (int column = 0; column < 3; ++column) {
                LCDRange lcdRange = new LCDRange();
                grid.addWidget(lcdRange, row, column);
            }
        }
    }
//! [4]

//! [5]
    class LCDRange extends QWidget
    {
//! [5] //! [6]
        public LCDRange()
        {
            QLCDNumber lcd = new QLCDNumber(2);
            lcd.setSegmentStyle(QLCDNumber.SegmentStyle.Filled);

            QSlider slider = new QSlider(Qt.Orientation.Horizontal);
            slider.setRange(0, 99);
            slider.setValue(0);

            slider.valueChanged.connect(lcd, "display(int)");

            QVBoxLayout layout = new QVBoxLayout();
            layout.addWidget(lcd);
            layout.addWidget(slider);
            setLayout(layout);
        }
//! [6]
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        BlocksGalore widget = new BlocksGalore();
        widget.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
//! [1]
