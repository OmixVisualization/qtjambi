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

#ifndef PAINTENGINE_H
#define PAINTENGINE_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

class CustomPaintEngine: public QPaintEngine
{
public:
    void callDrawLines(const QLine &first_line, int size)
    {
        QLine *lines = 0;
        if (size > 0) {
            lines = new QLine[size];
            lines[0] = first_line;
            for (int i=1; i<size; ++i)
                lines[i] = QLine();
        }

        drawLines(lines, size);

        delete[] lines;
    }

    void callDrawLineFs(const QLineF &first_line, int size)
    {
        QLineF *lines = 0;
        if (size > 0) {
            lines = new QLineF[size];
            lines[0] = first_line;
            for (int i=1; i<size; ++i)
                lines[i] = QLineF();
        }

        drawLines(lines, size);

        delete[] lines;
    }

    void callDrawRectFs(const QRectF &first_rect, int size)
    {
        QRectF *rects = 0;
        if (size > 0) {
            rects = new QRectF[size];
            rects[0] = first_rect;
            for (int i=1; i<size; ++i)
                rects[i] = QRectF();
        }

        drawRects(rects, size);

        delete[] rects;
    }

    void callDrawRects(const QRect &first_rect, int size)
    {
        QRect *rects = 0;
        if (size > 0) {
            rects = new QRect[size];
            rects[0] = first_rect;
            for (int i=1; i<size; ++i)
                rects[i] = QRect();
        }

        drawRects(rects, size);

        delete[] rects;
    }

    void callDrawPointFs(const QPointF &first_point, int size)
    {
        QPointF *points = 0;
        if (size > 0) {
            points = new QPointF[size];
            points[0] = first_point;
            for (int i=1; i<size; ++i)
                points[i] = QPointF();
        }

        drawPoints(points, size);

        delete[] points;
    }

    void callDrawPoints(const QPoint &first_point, int size)
    {
        QPoint *points = 0;
        if (size > 0) {
            points = new QPoint[size];
            points[0] = first_point;
            for (int i=1; i<size; ++i)
                points[i] = QPoint();
        }

        drawPoints(points, size);

        delete[] points;
    }


    void callDrawPolygonFs(const QPointF &first_point, int size)
    {
        QPointF *points = 0;
        if (size > 0) {
            points = new QPointF[size];
            points[0] = first_point;
            for (int i=1; i<size; ++i)
                points[i] = QPointF();
        }

        drawPolygon(points, size, QPaintEngine::OddEvenMode);

        delete[] points;
    }

    void callDrawPolygons(const QPoint &first_point, int size)
    {
        QPoint *points = 0;
        if (size > 0) {
            points = new QPoint[size];
            points[0] = first_point;
            for (int i=1; i<size; ++i)
                points[i] = QPoint();
        }

        drawPolygon(points, size, QPaintEngine::OddEvenMode);

        delete[] points;
    }

};

class OtherCustomPaintEngine: public QPaintEngine
{
public:
    void drawLines(const QLine *lines, int size)
    {
        if (lines == 0) {
            line = QLine();
            length = -1;
        } else {
            line = lines[0];
            length = size;
        }
    }

    void drawLines(const QLineF *lines, int size)
    {
        if (lines == 0) {
            lineF = QLineF();
            length = -1;
        } else {
            lineF = lines[0];
            length = size;
        }
    }

    void drawRects(const QRectF *rects, int size)
    {
        if (rects == 0) {
            rectF = QRectF();
            length = -1;
        } else {
            rectF = rects[0];
            length = size;
        }
    }

    void drawRects(const QRect *rects, int size)
    {
        if (rects == 0) {
            rect = QRect();
            length = -1;
        } else {
            rect = rects[0];
            length = size;
        }
    }

    void drawPoints(const QPointF *points, int size)
    {
        if (points == 0) {
            pointF = QPointF();
            length = -1;
        } else {
            pointF = points[0];
            length = size;
        }
    }

    void drawPoints(const QPoint *points, int size)
    {
        if (points == 0) {
            point = QPoint();
            length = -1;
        } else {
            point = points[0];
            length = size;
        }
    }

    void drawPolygon(const QPointF *points, int size, PolygonDrawMode)
    {
        if (points == 0) {
            pointF = QPointF();
            length = -1;
        } else {
            pointF = points[0];
            length = size;
        }
    }

    void drawPolygon(const QPoint *points, int size, PolygonDrawMode)
    {
        if (points == 0) {
            point = QPoint();
            length = -1;
        } else {
            point = points[0];
            length = size;
        }
    }

    bool begin(QPaintDevice *) { return true; }
    bool end() { return true; }
    Type type()const { return QPaintEngine::User; }
    void drawPixmap(const QRectF &, const QPixmap &, const QRectF &) {}
    void updateState(const QPaintEngineState &) {}

public:
    QLine line;
    QLineF lineF;
    QRectF rectF;
    QRect rect;
    QPointF pointF;
    QPoint point;
    int length;

};

#endif
