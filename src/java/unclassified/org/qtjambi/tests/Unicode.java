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

public class Unicode extends QWidget {

    @Override
    protected void paintEvent(QPaintEvent e) {
        QPainter p = new QPainter(this);

        p.translate(0, 20);

        renderUnicodeTable(p, new QFont("Times New Roman"));

        p.translate(240, 0);
        renderUnicodeTable(p, new QFont("Courier New"));

        p.translate(240, 0);
        renderUnicodeTable(p, new QFont("Tahoma"));

        p.translate(240, 0);
        renderUnicodeTable(p, new QFont("Comic Sans MS"));

    }

    private void renderUnicodeTable(QPainter p, QFont font) {
        p.setFont(font);
        p.drawText(0, 0, font.family());
        for (int c=0; c<8; ++c) {
            for (int r=0; r<16; ++r) {
                char character = (char) (0x2500 | (c << 4) | r);
                p.drawText(10 + c * font.pointSize() * 2, 10 + r * font.pointSize() * 2, "" + character);
            }
        }
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        Unicode code = new Unicode();
        code.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}

