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
#include <QtGui/QtGui>

#include <qtjambi_core/qtjambi_core_qhashes.h>

inline uint qHash(const QCursor &cursor)
{
    uint hashCode = qHash(int(cursor.shape()));
    if(cursor.shape()==Qt::BitmapCursor){
        hashCode = hashCode * 31 + qHash(cursor.bitmap());
        hashCode = hashCode * 31 + qHash(cursor.pixmap());
    }
    return hashCode;
}

inline uint qHash(const QColor &color)
{
    return qHash(quint64(color.rgba64()));
}

inline uint qHash(const QBrush &brush)
{
    uint hashCode = qHash(int(brush.style()));
    hashCode = hashCode * 31 + qHash(brush.color());
    return hashCode;
}

#if QT_VERSION >= 0x050000
inline uint qHash(const QGradient &gradient)
{
    uint hashCode = qHash(int(gradient.interpolationMode()));
    hashCode = hashCode * 31 + qHash(int(gradient.type()));
    hashCode = hashCode * 31 + qHash(int(gradient.spread()));
    hashCode = hashCode * 31 + qHash(int(gradient.coordinateMode()));
    for(const QGradientStop& stop : gradient.stops()){
        hashCode = hashCode * 31 + qHash(stop.first);
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}

inline uint qHash(const QLinearGradient &gradient)
{
    uint hashCode = qHash(int(gradient.interpolationMode()));
    hashCode = hashCode * 31 + qHash(int(gradient.type()));
    hashCode = hashCode * 31 + qHash(int(gradient.spread()));
    hashCode = hashCode * 31 + qHash(int(gradient.coordinateMode()));
    hashCode = hashCode * 31 + qHash(gradient.start());
    hashCode = hashCode * 31 + qHash(gradient.finalStop());
    for(const QGradientStop& stop : gradient.stops()){
        hashCode = hashCode * 31 + qHash(stop.first);
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}

inline uint qHash(const QRadialGradient &gradient)
{
    uint hashCode = qHash(int(gradient.interpolationMode()));
    hashCode = hashCode * 31 + qHash(int(gradient.type()));
    hashCode = hashCode * 31 + qHash(int(gradient.spread()));
    hashCode = hashCode * 31 + qHash(int(gradient.coordinateMode()));
    hashCode = hashCode * 31 + qHash(gradient.center());
    hashCode = hashCode * 31 + qHash(gradient.centerRadius());
    hashCode = hashCode * 31 + qHash(gradient.focalPoint());
    hashCode = hashCode * 31 + qHash(gradient.focalRadius());
    hashCode = hashCode * 31 + qHash(gradient.radius());
    for(const QGradientStop& stop : gradient.stops()){
        hashCode = hashCode * 31 + qHash(stop.first);
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}

inline uint qHash(const QConicalGradient &gradient)
{
    uint hashCode = qHash(int(gradient.interpolationMode()));
    hashCode = hashCode * 31 + qHash(int(gradient.type()));
    hashCode = hashCode * 31 + qHash(int(gradient.spread()));
    hashCode = hashCode * 31 + qHash(int(gradient.coordinateMode()));
    hashCode = hashCode * 31 + qHash(gradient.angle());
    hashCode = hashCode * 31 + qHash(gradient.center());
    for(const QGradientStop& stop : gradient.stops()){
        hashCode = hashCode * 31 + qHash(stop.first);
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}
#endif

inline uint qHash(const QRegion &region)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(region.rectCount());
    for(const QRect& rect : region)
        hashCode = hashCode * 31 + qHash(rect);
    return hashCode;
}

inline uint qHash(const QPolygon &polygon)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(polygon.size());
    for (int i=0; i<polygon.size(); ++i)
        hashCode = hashCode * 31 + qHash(polygon.at(i));
    return hashCode;
}

inline uint qHash(const QPalette &palette)
{
    uint hashCode = 1;
    for (int role=0;role<int(QPalette::NColorRoles);++role) {
        for (int group=0;group<int(QPalette::NColorGroups);++group) {
            hashCode = hashCode * 31 + qHash(palette.color(QPalette::ColorGroup(group), QPalette::ColorRole(role)));
        }
    }
    return hashCode;
}

#if QT_VERSION < 0x050300
inline uint qHash(const QFont &font)
{
    uint hashCode = font.pixelSize();
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


inline uint qHash(const QMatrix &matrix)
{
    uint hashCode = qHash(matrix.m11());
    hashCode = hashCode * 31 + qHash(matrix.m12());
    hashCode = hashCode * 31 + qHash(matrix.m21());
    hashCode = hashCode * 31 + qHash(matrix.m22());
    hashCode = hashCode * 31 + qHash(matrix.dx());
    hashCode = hashCode * 31 + qHash(matrix.dy());
    return hashCode;
}

inline uint qHash(const QImage &image)
{
    return qHash(image.cacheKey());
}

inline uint qHash(const QPen &pen)
{
    uint hashCode = qHash(int(pen.style()));
    hashCode = hashCode * 31 + qHash(int(pen.capStyle()));
    hashCode = hashCode * 31 + qHash(int(pen.joinStyle()));
    hashCode = hashCode * 31 + qHash(pen.width());
    hashCode = hashCode * 31 + qHash(pen.brush());
    hashCode = hashCode * 31 + qHash(pen.isCosmetic());
    return hashCode;
}

#if QT_VERSION < 0x050600
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
#endif

inline uint qHash(const QPolygonF &polygon)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(polygon.size());
    for (int i=0; i<polygon.size(); ++i)
        hashCode = hashCode * 31 + qHash(polygon.at(i));
    return hashCode;
}

inline uint qHash(const QVector2D &vec)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(vec.x());
    hashCode = hashCode * 31 + qHash(vec.y());
    return hashCode;
}

inline uint qHash(const QVector3D &vec)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(vec.x());
    hashCode = hashCode * 31 + qHash(vec.y());
    hashCode = hashCode * 31 + qHash(vec.z());
    return hashCode;
}

inline uint qHash(const QVector4D &vec)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(vec.x());
    hashCode = hashCode * 31 + qHash(vec.y());
    hashCode = hashCode * 31 + qHash(vec.z());
    hashCode = hashCode * 31 + qHash(vec.w());
    return hashCode;
}

/*
inline int qHash(const QInputMethodEvent::Attribute &attr)
{
    int hashCode = 1;
    hashCode = hashCode * 31 + qHash((qint32)attr.type);
    hashCode = hashCode * 31 + qHash(attr.start);
    hashCode = hashCode * 31 + qHash(attr.length);
    hashCode = hashCode * 31 + qHash((qint32)attr.value.type());
    hashCode = hashCode * 31 + qHash((qintptr)attr.value.data());
    return hashCode;
}*/

inline uint qHash(const QFontMetrics &value)
{
    struct FontMetrics{
        QExplicitlySharedDataPointer<void*> p;
    };
    const FontMetrics* fontMetrics = reinterpret_cast<const FontMetrics* >(&value);

    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(quintptr(fontMetrics->p.data()));
    return hashCode;
}

inline uint qHash(const QFontMetricsF &value)
{
    struct FontMetricsF{
        QExplicitlySharedDataPointer<void*> p;
    };
    const FontMetricsF* fontMetrics = reinterpret_cast<const FontMetricsF* >(&value);

    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(quintptr(fontMetrics->p.data()));
    return hashCode;
}

inline uint qHash(const QGlyphRun &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.rawFont());
    hashCode = hashCode * 31 + qHash(value.glyphIndexes());
    hashCode = hashCode * 31 + qHash(value.positions());
    hashCode = hashCode * 31 + qHash(value.overline());
    hashCode = hashCode * 31 + qHash(value.underline());
    hashCode = hashCode * 31 + qHash(value.strikeOut());
    hashCode = hashCode * 31 + qHash(value.isRightToLeft());
    hashCode = hashCode * 31 + qHash(int(value.flags()));
    hashCode = hashCode * 31 + qHash(value.boundingRect());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}

inline uint qHash(const QAccessible::State &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix2x2 &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix2x3 &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix2x4 &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix3x2 &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix3x3 &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix3x4 &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix4x2 &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix4x3 &value)
{
    return genericHash(value);
}

inline uint qHash(const QMatrix4x4 &value)
{
    return genericHash(value);
}

inline uint qHash(const QOpenGLDebugMessage &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.source()));
    hashCode = hashCode * 31 + qHash(int(value.type()));
    hashCode = hashCode * 31 + qHash(int(value.severity()));
    hashCode = hashCode * 31 + qHash(value.id());
    hashCode = hashCode * 31 + qHash(value.message());
    return hashCode;
}

inline uint qHash(const QOpenGLFramebufferObjectFormat &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.samples());
    hashCode = hashCode * 31 + qHash(value.mipmap());
    hashCode = hashCode * 31 + qHash(int(value.attachment()));
    hashCode = hashCode * 31 + qHash(value.textureTarget());
    hashCode = hashCode * 31 + qHash(value.internalTextureFormat());
    return hashCode;
}

inline uint qHash(const QPageSize &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.key());
    hashCode = hashCode * 31 + qHash(int(value.id()));
    hashCode = hashCode * 31 + qHash(int(value.definitionUnits()));
    hashCode = hashCode * 31 + qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.windowsId());
    hashCode = hashCode * 31 + qHash(value.definitionSize());
    hashCode = hashCode * 31 + qHash(value.sizePoints());
    hashCode = hashCode * 31 + qHash(value.rectPoints());
    return hashCode;
}

inline uint qHash(const QPageLayout &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.pageSize());
    hashCode = hashCode * 31 + qHash(int(value.mode()));
    hashCode = hashCode * 31 + qHash(int(value.orientation()));
    hashCode = hashCode * 31 + qHash(int(value.units()));
    hashCode = hashCode * 31 + qHash(value.margins());
    hashCode = hashCode * 31 + qHash(value.marginsPoints());
    hashCode = hashCode * 31 + qHash(value.minimumMargins());
    hashCode = hashCode * 31 + qHash(value.fullRect());
    hashCode = hashCode * 31 + qHash(value.fullRectPoints());
    hashCode = hashCode * 31 + qHash(value.paintRect());
    hashCode = hashCode * 31 + qHash(value.paintRectPoints());
    return hashCode;
}

inline uint qHash(const QPainterPath::Element &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.type));
    hashCode = hashCode * 31 + qHash(value.x);
    hashCode = hashCode * 31 + qHash(value.y);
    return hashCode;
}

inline uint qHash(const QPainterPath &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.fillRule()));
    hashCode = hashCode * 31 + qHash(value.elementCount());
    for(int i=0; i<value.elementCount(); ++i){
        hashCode = hashCode * 31 + qHash(value.elementAt(i));
    }
    return hashCode;
}

inline uint qHash(const QTextOption::Tab &value)
{
    return genericHash(value);
}

inline uint qHash(const QTextOption &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.alignment()));
    hashCode = hashCode * 31 + qHash(int(value.textDirection()));
    hashCode = hashCode * 31 + qHash(int(value.wrapMode()));
    hashCode = hashCode * 31 + qHash(int(value.flags()));
    hashCode = hashCode * 31 + qHash(int(value.useDesignMetrics()));
    hashCode = hashCode * 31 + qHash(value.tabStopDistance());
    hashCode = hashCode * 31 + qHash(value.tabArray());
    hashCode = hashCode * 31 + qHash(value.tabs());
    return hashCode;
}

inline uint qHash(const QStaticText &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.textFormat()));
    hashCode = hashCode * 31 + qHash(int(value.performanceHint()));
    hashCode = hashCode * 31 + qHash(value.text());
    hashCode = hashCode * 31 + qHash(value.textWidth());
    hashCode = hashCode * 31 + qHash(value.textOption());
    hashCode = hashCode * 31 + qHash(value.size());
    return hashCode;
}

inline uint qHash(const QSurfaceFormat &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.swapBehavior()));
    hashCode = hashCode * 31 + qHash(int(value.profile()));
    hashCode = hashCode * 31 + qHash(int(value.renderableType()));
    hashCode = hashCode * 31 + qHash(int(value.options()));
    hashCode = hashCode * 31 + qHash(int(value.colorSpace()));
    hashCode = hashCode * 31 + qHash(value.depthBufferSize());
    hashCode = hashCode * 31 + qHash(value.stencilBufferSize());
    hashCode = hashCode * 31 + qHash(value.redBufferSize());
    hashCode = hashCode * 31 + qHash(value.greenBufferSize());
    hashCode = hashCode * 31 + qHash(value.blueBufferSize());
    hashCode = hashCode * 31 + qHash(value.alphaBufferSize());
    hashCode = hashCode * 31 + qHash(value.samples());
    hashCode = hashCode * 31 + qHash(value.hasAlpha());
    hashCode = hashCode * 31 + qHash(value.majorVersion());
    hashCode = hashCode * 31 + qHash(value.minorVersion());
    hashCode = hashCode * 31 + qHash(value.stereo());
    hashCode = hashCode * 31 + qHash(value.swapInterval());
    return hashCode;
}

inline uint qHash(const QQuaternion &value)
{
    return genericHash(value);
}

inline uint qHash(const QPixmapCache::Key &value)
{
    return genericHash(value);
}

inline uint qHash(const QTextFragment &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.length());
    return hashCode;
}

inline uint qHash(const QTextBlock::iterator &value)
{
    return genericHash(value);
}

inline uint qHash(const QTextBlock &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.blockNumber());
    return hashCode;
}

inline uint qHash(const QTextCursor &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.anchor());
    hashCode = hashCode * 31 + qHash(value.selectedText());
    return hashCode;
}

inline uint qHash(const QTextFormat &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.objectIndex());
    hashCode = hashCode * 31 + qHash(value.objectType());
    hashCode = hashCode * 31 + qHash(value.propertyCount());
    return hashCode;
}

inline uint qHash(const QTextFrame &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.parentFrame());
    hashCode = hashCode * 31 + qHash(value.firstPosition());
    hashCode = hashCode * 31 + qHash(value.lastPosition());
    return hashCode;
}

inline uint qHash(const QTextFrame::iterator &value)
{
    return genericHash(value);
}

inline uint qHash(const QTextLength &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.rawValue());
    return hashCode;
}

inline uint qHash(const QTextTableCell &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.firstPosition());
    hashCode = hashCode * 31 + qHash(value.lastPosition());
    hashCode = hashCode * 31 + qHash(value.row());
    hashCode = hashCode * 31 + qHash(value.column());
    hashCode = hashCode * 31 + qHash(value.rowSpan());
    hashCode = hashCode * 31 + qHash(value.columnSpan());
    hashCode = hashCode * 31 + qHash(value.tableCellFormatIndex());
    return hashCode;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
inline uint qHash(const QColorSpace &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(uint(value.primaries()));
    hashCode = hashCode * 31 + qHash(uint(value.transferFunction()));
    hashCode = hashCode * 31 + qHash(value.gamma());
    hashCode = hashCode * 31 + qHash(value.isValid());
    return hashCode;
}
#endif
#endif // QTJAMBI_GUI_QHASHES_H
