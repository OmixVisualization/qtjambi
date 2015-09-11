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

package org.qtjambi.qt;

import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPaintDeviceInterface;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.widgets.QAbstractScrollArea;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QWidget;

import java.util.HashMap;

/**
 * @exclude
 */
public class QtJambiGuiInternal {
    /**
     * Shows an about box for Qt Jambi
     */
    public static void aboutQtJambi() {
        QMessageBox mb = new QMessageBox(QApplication.activeWindow());
        mb.setWindowTitle("About Qt Jambi");
        mb.setText("<h3>About Qt Jambi</h3>"
                   + "<p>Qt Jambi is a Java toolkit based on Qt, a C++ toolkit for"
                   + " cross-platform application development.</p>"
                   + "<p>This program uses Qt version "
                   + QtInfo.versionString()
                   + ".</p>"
                   + "<p>Qt Jambi provides single-source "
                   + "portability across MS&nbsp;Windows, Mac&nbsp;OS&nbsp;X, "
                   + "Linux, and all major commercial Unix variants"
                   + "<p>See "
                   + "<a href=\"http://www.qt-jambi.org\">http://www.qt-jambi.org</a> for more information.</p>");
        mb.setIconPixmap(new QPixmap(
                "classpath:org/qtjambi/qt/images/qt-logo.png"));
        mb.exec();
    }



    private static HashMap<QWidget, QPainter> painters = new HashMap<QWidget, QPainter>();

    public static boolean beginPaint(QWidget widget, QPainter painter) {
        if (painters.containsKey(widget))
            throw new RuntimeException("Painter opened twice on the same widget");
        if (painter.isActive())
            throw new RuntimeException("Painter already active");
        painters.put(widget, painter);
        return painter.begin((QPaintDeviceInterface) widget);
    }

    private static void endPaint(QWidget widget) {
        QPainter p = painters.get(widget);
        if (p != null) {
            if (p.isActive())
                p.end();
            p.dispose();
            painters.remove(widget);
        }

        if (widget instanceof QAbstractScrollArea) {
            endPaint(((QAbstractScrollArea) widget).viewport());
        }
    }

}
