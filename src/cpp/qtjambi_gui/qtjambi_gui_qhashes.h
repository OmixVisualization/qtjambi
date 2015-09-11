/****************************************************************************
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef QTJAMBI_GUI_QHASHES_H
#define QTJAMBI_GUI_QHASHES_H

#include <QtCore/QtCore>
#include <QtCore/QHash>
#include <QtCore/QVector>

#include <QtGui/QColor>
#include <QtGui/QBrush>
#include <QtGui/QRegion>
#include <QtGui/QPolygon>
#include <QtGui/QPalette>
#include <QtGui/QFont>
#include <QtGui/QMatrix>
#include <QtGui/QImage>
#include <QtGui/QPen>
#include <QtGui/QTransform>
#include <QtGui/QPolygonF>

#include <qtjambi_core/qtjambi_core_qhashes.h>

inline int qHash(const QColor &color)
{
    return int(color.rgba());
}

inline int qHash(const QBrush &brush)
{
    int hashCode = int(brush.style());
    hashCode = hashCode * 31 + qHash(brush.color());
    return hashCode;
}

#if QT_VERSION >= 0x050000
inline int qHash(const QLinearGradient &gradient)
{
    int hashCode = int(gradient.interpolationMode());
    hashCode = hashCode * 31 + int(gradient.type());
    hashCode = hashCode * 31 + int(gradient.spread());
    hashCode = hashCode * 31 + int(gradient.coordinateMode());
    hashCode = hashCode * 31 + qHash(gradient.start());
    hashCode = hashCode * 31 + qHash(gradient.finalStop());
    foreach(QGradientStop stop , gradient.stops()){
        hashCode = hashCode * 31 + qHash(quint64(stop.first));
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}

inline int qHash(const QRadialGradient &gradient)
{
    int hashCode = int(gradient.interpolationMode());
    hashCode = hashCode * 31 + int(gradient.type());
    hashCode = hashCode * 31 + int(gradient.spread());
    hashCode = hashCode * 31 + int(gradient.coordinateMode());
    hashCode = hashCode * 31 + qHash(gradient.center());
    hashCode = hashCode * 31 + qHash(quint64(gradient.centerRadius()));
    hashCode = hashCode * 31 + qHash(gradient.focalPoint());
    hashCode = hashCode * 31 + qHash(quint64(gradient.focalRadius()));
    hashCode = hashCode * 31 + qHash(quint64(gradient.radius()));
    foreach(QGradientStop stop , gradient.stops()){
        hashCode = hashCode * 31 + qHash(quint64(stop.first));
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}

inline int qHash(const QConicalGradient &gradient)
{
    int hashCode = int(gradient.interpolationMode());
    hashCode = hashCode * 31 + int(gradient.type());
    hashCode = hashCode * 31 + int(gradient.spread());
    hashCode = hashCode * 31 + int(gradient.coordinateMode());
    hashCode = hashCode * 31 + qHash(quint64(gradient.angle()));
    hashCode = hashCode * 31 + qHash(gradient.center());
    foreach(QGradientStop stop , gradient.stops()){
        hashCode = hashCode * 31 + qHash(quint64(stop.first));
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}
#endif

inline int qHash(const QRegion &region)
{
    int hashCode = 1;
    QVector<QRect> rects = region.rects();
    for (int i=0; i<rects.size(); ++i)
        hashCode = hashCode * 31 + qHash(rects.at(i));
    return hashCode;
}

inline int qHash(const QPolygon &polygon)
{
    int hashCode = 1;
    for (int i=0; i<polygon.size(); ++i)
        hashCode = hashCode * 31 + qHash(polygon.at(i));
    return hashCode;
}

inline int qHash(const QPalette &palette)
{
    int hashCode = 1;
    for (int role=0;role<int(QPalette::NColorRoles);++role) {
        for (int group=0;group<int(QPalette::NColorGroups);++group) {
            hashCode = hashCode * 31 + qHash(palette.color(QPalette::ColorGroup(group), QPalette::ColorRole(role)));
        }
    }
    return hashCode;
}

#if QT_VERSION < 0x050300
inline int qHash(const QFont &font)
{
    int hashCode = font.pixelSize();
    hashCode = hashCode * 31 + font.weight();
    hashCode = hashCode * 31 + int(font.style());
    hashCode = hashCode * 31 + font.stretch();
    hashCode = hashCode * 31 + int(font.styleHint());
    hashCode = hashCode * 31 + int(font.styleStrategy());
    hashCode = hashCode * 31 + int(font.fixedPitch());
    hashCode = hashCode * 31 + qHash(font.family());
    hashCode = hashCode * 31 + qHash(font.pointSize());
    hashCode = hashCode * 31 + int(font.underline());
    hashCode = hashCode * 31 + int(font.overline());
    hashCode = hashCode * 31 + int(font.strikeOut());
    hashCode = hashCode * 31 + int(font.kerning());
    return hashCode;
}
#endif


inline int qHash(const QMatrix &matrix)
{
    int hashCode = int(matrix.m11());
    hashCode = hashCode * 31 + int(matrix.m12());
    hashCode = hashCode * 31 + int(matrix.m21());
    hashCode = hashCode * 31 + int(matrix.m22());
    hashCode = hashCode * 31 + int(matrix.dx());
    hashCode = hashCode * 31 + int(matrix.dy());
    return hashCode;
}

inline int qHash(const QImage &image)
{
    return qHash(image.cacheKey());
}

inline int qHash(const QPen &pen)
{
    int hashCode = int(pen.style()); 
    hashCode = hashCode * 31 + int(pen.capStyle());
    hashCode = hashCode * 31 + int(pen.joinStyle());
    hashCode = hashCode * 31 + pen.width();
    hashCode = hashCode * 31 + qHash(pen.brush());
    hashCode = hashCode * 31 + int(pen.isCosmetic());
    return hashCode;
}

inline int qHash(const QTransform &transform)
{
    int hashCode = int(transform.m11());
    hashCode = hashCode * 31 + int(transform.m12());
    hashCode = hashCode * 31 + int(transform.m13());

    hashCode = hashCode * 31 + int(transform.m21());
    hashCode = hashCode * 31 + int(transform.m22());
    hashCode = hashCode * 31 + int(transform.m23());

    hashCode = hashCode * 31 + int(transform.m31());
    hashCode = hashCode * 31 + int(transform.m32());
    hashCode = hashCode * 31 + int(transform.m33());

    return hashCode;
}

inline int qHash(const QPolygonF &polygon)
{
    int hashCode = 1;
    for (int i=0; i<polygon.size(); ++i)
        hashCode = hashCode * 31 + qHash(polygon.at(i));
    return hashCode;
}

#endif // QTJAMBI_GUI_QHASHES_H
