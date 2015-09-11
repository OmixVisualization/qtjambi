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

package org.qtjambi.extensions.jfc;

import java.awt.*;
import java.awt.geom.*;
import java.util.Calendar;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.gui.QPolygon;

public class Util {

    public static Calendar convert(QDateTime from) {
        Calendar to = Calendar.getInstance();
        QDate fromDate = from.date();
        QTime fromTime = from.time();
        to.set(fromDate.year(), fromDate.month() - 1, fromDate.day(), fromTime.hour(), fromTime.minute(), fromTime.second());
        return to;
    }

    public static QDateTime convert(Calendar from) {
        return new QDateTime(new QDate(from.get(Calendar.YEAR), from.get(Calendar.MONTH) + 1, from.get(Calendar.DAY_OF_MONTH)), new QTime(from
                .get(Calendar.HOUR), from.get(Calendar.MINUTE), from.get(Calendar.SECOND)));
    }

    public static Rectangle2D convert(QRect from) {
        return new Rectangle(from.x(), from.y(), from.width(), from.height());
    }

    public static QRect convert(Rectangle from) {
        return new QRect(from.x, from.y, from.width, from.height);
    }

    public static Rectangle2D convert(QRectF from) {
        return new Rectangle2D.Double(from.x(), from.y(), from.width(), from.height());
    }

    public static QRectF convert(Rectangle2D.Double from) {
        return new QRectF(from.getX(), from.getY(), from.getWidth(), from.getHeight());
    }

    public static QPoint convert(Point from) {
        return new QPoint(from.x, from.y);
    }

    public static Point convert(QPoint from) {
        return new Point(from.x(), from.y());
    }

    public static QPointF convert(Point2D.Double from) {
        return new QPointF(from.x, from.y);
    }

    public static Point2D convert(QPointF from) {
        return new Point2D.Double(from.x(), from.y());
    }

    public static QLineF convert(Line2D.Double from) {
        return new QLineF(from.x1, from.y1, from.x2, from.y2);
    }

    public static Line2D convert(QLineF from) {
        return new Line2D.Double(from.x1(), from.y1(), from.x2(), from.y2());
    }

    public static QPolygon convert(Polygon from) {
        QPolygon to = new QPolygon();
        for (int i = 0; i < from.npoints; i++) {
            to.add(from.xpoints[i], from.ypoints[i]);
        }
        return to;
    }

    public static Polygon convert(QPolygon from) {
        Polygon to = new Polygon();
        for (int i = 0; i < from.size(); i++) {
            Point point = convert(from.at(i));
            to.addPoint(point.x, point.y);
        }
        return to;
    }
}
