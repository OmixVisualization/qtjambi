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

#include <qtjambi/qtjambi_global.h>
#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include <qtjambi_core/qtjambi_core_qhashes.h>

#ifndef QT_JAMBI_RUN
inline bool operator <(const QPointF& p1, const QPointF& p2){
    if(p1.x()>=p2.x())
        return p1.y()<p2.y();
    return p1.x()<p2.x();
}

inline bool operator <(const QPoint& p1, const QPoint& p2){
    if(p1.x()>=p2.x())
        return p1.y()<p2.y();
    return p1.x()<p2.x();
}

inline bool operator <(const QColor& c1, const QColor& c2){
    return quint64(c1.rgba64())<quint64(c2.rgba64());
}
#endif

inline hash_type qHash(const QPixmap &value)
{
    return genericHash(value.handle());
}

inline hash_type qHash(const QCursor &cursor)
{
    hash_type hashCode = qHash(int(cursor.shape()));
    if(cursor.shape()==Qt::BitmapCursor){
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
        hashCode = hashCode * 31 + qHash(cursor.bitmap(Qt::ReturnByValue));
        hashCode = hashCode * 31 + qHash(cursor.mask(Qt::ReturnByValue));
#else
        hashCode = hashCode * 31 + qHash(cursor.bitmap());
        hashCode = hashCode * 31 + qHash(cursor.mask());
#endif
        hashCode = hashCode * 31 + qHash(cursor.pixmap());
        hashCode = hashCode * 31 + qHash(cursor.hotSpot());
    }
    return hashCode;
}

inline hash_type qHash(const QPixelFormat &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QColor &color)
{
    return qHash(quint64(color.rgba64()));
}

inline hash_type qHash(const QBrush &brush)
{
    hash_type hashCode = qHash(int(brush.style()));
    hashCode = hashCode * 31 + qHash(brush.color());
    return hashCode;
}

#if QT_VERSION >= 0x050000
inline hash_type qHash(const QGradient &gradient)
{
    hash_type hashCode = qHash(int(gradient.interpolationMode()));
    hashCode = hashCode * 31 + qHash(int(gradient.type()));
    hashCode = hashCode * 31 + qHash(int(gradient.spread()));
    hashCode = hashCode * 31 + qHash(int(gradient.coordinateMode()));
    for(const QGradientStop& stop : gradient.stops()){
        hashCode = hashCode * 31 + qHash(stop.first);
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}

inline hash_type qHash(const QLinearGradient &gradient)
{
    hash_type hashCode = qHash(int(gradient.interpolationMode()));
    hashCode = hashCode * 31 + qHash(int(gradient.type()));
    hashCode = hashCode * 31 + qHash(int(gradient.spread()));
    hashCode = hashCode * 31 + qHash(int(gradient.coordinateMode()));
    hashCode = hashCode * 31 + qHash(gradient.start().x());
    hashCode = hashCode * 31 + qHash(gradient.start().y());
    hashCode = hashCode * 31 + qHash(gradient.finalStop().x());
    hashCode = hashCode * 31 + qHash(gradient.finalStop().y());
    for(const QGradientStop& stop : gradient.stops()){
        hashCode = hashCode * 31 + qHash(stop.first);
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}

inline hash_type qHash(const QRadialGradient &gradient)
{
    hash_type hashCode = qHash(int(gradient.interpolationMode()));
    hashCode = hashCode * 31 + qHash(int(gradient.type()));
    hashCode = hashCode * 31 + qHash(int(gradient.spread()));
    hashCode = hashCode * 31 + qHash(int(gradient.coordinateMode()));
    hashCode = hashCode * 31 + qHash(gradient.center().x());
    hashCode = hashCode * 31 + qHash(gradient.center().y());
    hashCode = hashCode * 31 + qHash(gradient.centerRadius());
    hashCode = hashCode * 31 + qHash(gradient.focalPoint().x());
    hashCode = hashCode * 31 + qHash(gradient.focalPoint().y());
    hashCode = hashCode * 31 + qHash(gradient.focalRadius());
    hashCode = hashCode * 31 + qHash(gradient.radius());
    for(const QGradientStop& stop : gradient.stops()){
        hashCode = hashCode * 31 + qHash(stop.first);
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}

inline hash_type qHash(const QConicalGradient &gradient)
{
    hash_type hashCode = qHash(int(gradient.interpolationMode()));
    hashCode = hashCode * 31 + qHash(int(gradient.type()));
    hashCode = hashCode * 31 + qHash(int(gradient.spread()));
    hashCode = hashCode * 31 + qHash(int(gradient.coordinateMode()));
    hashCode = hashCode * 31 + qHash(gradient.angle());
    hashCode = hashCode * 31 + qHash(gradient.center().x());
    hashCode = hashCode * 31 + qHash(gradient.center().y());
    for(const QGradientStop& stop : gradient.stops()){
        hashCode = hashCode * 31 + qHash(stop.first);
        hashCode = hashCode * 31 + qHash(stop.second);
    }
    return hashCode;
}
#endif

inline hash_type qHash(const QRegion &region)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(region.rectCount());
    for(const QRect& rect : region)
        hashCode = hashCode * 31 + qHash(rect);
    return hashCode;
}

inline hash_type qHash(const QPolygon &polygon)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(polygon.size());
    for (int i=0; i<polygon.size(); ++i)
        hashCode = hashCode * 31 + qHash(polygon.at(i));
    return hashCode;
}

inline hash_type qHash(const QPalette &palette)
{
    hash_type hashCode = 1;
    for (int role=0;role<int(QPalette::NColorRoles);++role) {
        for (int group=0;group<int(QPalette::NColorGroups);++group) {
            hashCode = hashCode * 31 + qHash(palette.color(QPalette::ColorGroup(group), QPalette::ColorRole(role)));
        }
    }
    return hashCode;
}

#if QT_VERSION < 0x050300
inline hash_type qHash(const QFont &font)
{
    hash_type hashCode = font.pixelSize();
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


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QMatrix &matrix)
{
    hash_type hashCode = qHash(matrix.m11());
    hashCode = hashCode * 31 + qHash(matrix.m12());
    hashCode = hashCode * 31 + qHash(matrix.m21());
    hashCode = hashCode * 31 + qHash(matrix.m22());
    hashCode = hashCode * 31 + qHash(matrix.dx());
    hashCode = hashCode * 31 + qHash(matrix.dy());
    return hashCode;
}
#endif

inline hash_type qHash(const QImage &image)
{
    if(image.isNull())
        return 0;
    return qHash(image.cacheKey());
}

inline hash_type qHash(const QPen &pen)
{
    hash_type hashCode = qHash(int(pen.style()));
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

inline hash_type qHash(const QPolygonF &polygon)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(polygon.size());
    for (int i=0; i<polygon.size(); ++i){
        hashCode = hashCode * 31 + qHash(polygon.at(i).x());
        hashCode = hashCode * 31 + qHash(polygon.at(i).y());
    }
    return hashCode;
}

inline hash_type qHash(const QVector2D &vec)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(vec.x());
    hashCode = hashCode * 31 + qHash(vec.y());
    return hashCode;
}

inline hash_type qHash(const QVector3D &vec)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(vec.x());
    hashCode = hashCode * 31 + qHash(vec.y());
    hashCode = hashCode * 31 + qHash(vec.z());
    return hashCode;
}

inline hash_type qHash(const QVector4D &vec)
{
    hash_type hashCode = 1;
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

inline hash_type qHash(const QFontMetrics &value)
{
    struct FontMetrics{
        QExplicitlySharedDataPointer<void*> p;
    };
    const FontMetrics* fontMetrics = reinterpret_cast<const FontMetrics* >(&value);

    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(quintptr(fontMetrics->p.data()));
    return hashCode;
}

inline hash_type qHash(const QFontMetricsF &value)
{
    struct FontMetricsF{
        QExplicitlySharedDataPointer<void*> p;
    };
    const FontMetricsF* fontMetrics = reinterpret_cast<const FontMetricsF* >(&value);

    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(quintptr(fontMetrics->p.data()));
    return hashCode;
}

inline hash_type qHash(const QGlyphRun &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.rawFont());
    hashCode = hashCode * 31 + qHash(value.glyphIndexes());
    auto positions = value.positions();
    hashCode = hashCode * 31 + qHash(positions.size());
    for (int i=0; i<positions.size(); ++i){
        hashCode = hashCode * 31 + qHash(positions.at(i).x());
        hashCode = hashCode * 31 + qHash(positions.at(i).y());
    }
    hashCode = hashCode * 31 + qHash(value.overline());
    hashCode = hashCode * 31 + qHash(value.underline());
    hashCode = hashCode * 31 + qHash(value.strikeOut());
    hashCode = hashCode * 31 + qHash(value.isRightToLeft());
    hashCode = hashCode * 31 + qHash(int(value.flags()));
    hashCode = hashCode * 31 + qHash(value.boundingRect());
    hashCode = hashCode * 31 + qHash(value.isEmpty());
    return hashCode;
}

inline hash_type qHash(const QAccessible::State &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix2x2 &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix2x3 &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix2x4 &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix3x2 &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix3x3 &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix3x4 &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix4x2 &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix4x3 &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QMatrix4x4 &value)
{
    return genericHash(value);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QOpenGLDebugMessage &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.source()));
    hashCode = hashCode * 31 + qHash(int(value.type()));
    hashCode = hashCode * 31 + qHash(int(value.severity()));
    hashCode = hashCode * 31 + qHash(value.id());
    hashCode = hashCode * 31 + qHash(value.message());
    return hashCode;
}

inline hash_type qHash(const QOpenGLFramebufferObjectFormat &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.samples());
    hashCode = hashCode * 31 + qHash(value.mipmap());
    hashCode = hashCode * 31 + qHash(int(value.attachment()));
    hashCode = hashCode * 31 + qHash(value.textureTarget());
    hashCode = hashCode * 31 + qHash(value.internalTextureFormat());
    return hashCode;
}
#else
inline hash_type qHash(const QEventPoint &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(quintptr(value.device()));
    hashCode = hashCode * 31 + qHash(value.ellipseDiameters());
    hashCode = hashCode * 31 + qHash(value.globalGrabPosition().x());
    hashCode = hashCode * 31 + qHash(value.globalGrabPosition().y());
    hashCode = hashCode * 31 + qHash(value.globalLastPosition().x());
    hashCode = hashCode * 31 + qHash(value.globalLastPosition().y());
    hashCode = hashCode * 31 + qHash(value.globalPosition().x());
    hashCode = hashCode * 31 + qHash(value.globalPosition().y());
    hashCode = hashCode * 31 + qHash(value.globalPressPosition().x());
    hashCode = hashCode * 31 + qHash(value.globalPressPosition().y());
    hashCode = hashCode * 31 + qHash(value.grabPosition().x());
    hashCode = hashCode * 31 + qHash(value.grabPosition().y());
    hashCode = hashCode * 31 + qHash(value.id());
    hashCode = hashCode * 31 + qHash(value.isAccepted());
    hashCode = hashCode * 31 + qHash(value.lastPosition().x());
    hashCode = hashCode * 31 + qHash(value.lastPosition().y());
    hashCode = hashCode * 31 + qHash(value.lastTimestamp());
    hashCode = hashCode * 31 + qHash(value.normalizedPosition().x());
    hashCode = hashCode * 31 + qHash(value.normalizedPosition().y());
    hashCode = hashCode * 31 + qHash(value.position().x());
    hashCode = hashCode * 31 + qHash(value.position().y());
    hashCode = hashCode * 31 + qHash(value.pressPosition().x());
    hashCode = hashCode * 31 + qHash(value.pressPosition().y());
    hashCode = hashCode * 31 + qHash(value.pressTimestamp());
    hashCode = hashCode * 31 + qHash(value.pressure());
    hashCode = hashCode * 31 + qHash(value.rotation());
    hashCode = hashCode * 31 + qHash(value.sceneGrabPosition().x());
    hashCode = hashCode * 31 + qHash(value.sceneGrabPosition().y());
    hashCode = hashCode * 31 + qHash(value.sceneLastPosition().x());
    hashCode = hashCode * 31 + qHash(value.sceneLastPosition().y());
    hashCode = hashCode * 31 + qHash(value.scenePosition().x());
    hashCode = hashCode * 31 + qHash(value.scenePosition().y());
    hashCode = hashCode * 31 + qHash(value.scenePressPosition().x());
    hashCode = hashCode * 31 + qHash(value.scenePressPosition().y());
    hashCode = hashCode * 31 + qHash(value.state());
    hashCode = hashCode * 31 + qHash(value.timeHeld());
    hashCode = hashCode * 31 + qHash(value.timestamp());
    hashCode = hashCode * 31 + qHash(value.uniqueId());
    hashCode = hashCode * 31 + qHash(value.velocity());
    return hashCode;
}

inline hash_type qHash(const QPageRanges::Range &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QPageRanges &value)
{
    return qHash(value.toRangeList());
}

#endif

hash_type qHash(const QTextFormat &value);

inline hash_type qHash(const QTextLayout::FormatRange &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.start);
    hashCode = hashCode * 31 + qHash(value.length);
    hashCode = hashCode * 31 + qHash(value.format);
    return hashCode;
}

inline hash_type qHash(const QPageSize &value)
{
    hash_type hashCode = 1;
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

inline hash_type qHash(const QPageLayout &value)
{
    hash_type hashCode = 1;
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

inline hash_type qHash(const QPainterPath::Element &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.type));
    hashCode = hashCode * 31 + qHash(value.x);
    hashCode = hashCode * 31 + qHash(value.y);
    return hashCode;
}

inline hash_type qHash(const QPainterPath &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.fillRule()));
    hashCode = hashCode * 31 + qHash(value.elementCount());
    for(int i=0; i<value.elementCount(); ++i){
        hashCode = hashCode * 31 + qHash(value.elementAt(i));
    }
    return hashCode;
}

inline hash_type qHash(const QTextOption::Tab &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QTextOption &value)
{
    hash_type hashCode = 1;
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

inline hash_type qHash(const QStaticText &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.textFormat()));
    hashCode = hashCode * 31 + qHash(int(value.performanceHint()));
    hashCode = hashCode * 31 + qHash(value.text());
    hashCode = hashCode * 31 + qHash(value.textWidth());
    hashCode = hashCode * 31 + qHash(value.textOption());
    hashCode = hashCode * 31 + qHash(value.size());
    return hashCode;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
inline hash_type qHash(const QColorSpace &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(hash_type(value.primaries()));
    hashCode = hashCode * 31 + qHash(hash_type(value.transferFunction()));
    hashCode = hashCode * 31 + qHash(value.gamma());
    hashCode = hashCode * 31 + qHash(value.isValid());
    return hashCode;
}
#endif

inline hash_type qHash(const QSurfaceFormat &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.swapBehavior()));
    hashCode = hashCode * 31 + qHash(int(value.profile()));
    hashCode = hashCode * 31 + qHash(int(value.renderableType()));
    hashCode = hashCode * 31 + qHash(int(value.options()));
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    hashCode = hashCode * 31 + qHash(value.colorSpace());
#else
    hashCode = hashCode * 31 + qHash(int(value.colorSpace()));
#endif
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

inline hash_type qHash(const QQuaternion &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QPixmapCache::Key &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QTextFragment &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.length());
    return hashCode;
}

inline hash_type qHash(const QTextBlock::iterator &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QTextBlock &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.blockNumber());
    return hashCode;
}

inline hash_type qHash(const QTextCursor &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.position());
    hashCode = hashCode * 31 + qHash(value.anchor());
    hashCode = hashCode * 31 + qHash(value.selectedText());
    return hashCode;
}

hash_type qHash(const QTextLength &value);

inline hash_type qHash(const QTextFormat &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.objectIndex());
    hashCode = hashCode * 31 + qHash(value.objectType());
    hashCode = hashCode * 31 + qHash(value.propertyCount());
    QMap<int, QVariant> properties = value.properties();
    for(int key : properties.keys()){
        hashCode = hashCode * 31 + qHash(key);
        const QVariant& variant = properties[key];
        switch(variant.userType()){
        case QMetaType::Bool:
            hashCode = hashCode * 31 + qHash(variant.toBool());
            break;
        case QMetaType::Int:
            hashCode = hashCode * 31 + qHash(variant.toInt());
            break;
        case QMetaType::Float:
            hashCode = hashCode * 31 + qHash(variant.toFloat());
            break;
        case QMetaType::Double:
            hashCode = hashCode * 31 + qHash(variant.toDouble());
            break;
        case QMetaType::QString:
            hashCode = hashCode * 31 + qHash(variant.toString());
            break;
        case QMetaType::QColor:
            hashCode = hashCode * 31 + qHash(variant.value<QColor>());
            break;
        case QMetaType::QPen:
            hashCode = hashCode * 31 + qHash(variant.value<QPen>());
            break;
        case QMetaType::QTextLength:
            hashCode = hashCode * 31 + qHash(variant.value<QTextLength>());
            break;
        default:
            if(variant.userType()==qMetaTypeId<QList<QTextLength>>()){
                hashCode = hashCode * 31 + qHash(variant.value<QList<QTextLength>>());
            }
            break;
        }
    }
    return hashCode;
}

inline hash_type qHash(const QTextFrame &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.parentFrame());
    hashCode = hashCode * 31 + qHash(value.firstPosition());
    hashCode = hashCode * 31 + qHash(value.lastPosition());
    return hashCode;
}

inline hash_type qHash(const QTextFrame::iterator &value)
{
    return genericHash(value);
}

inline hash_type qHash(const QTextLength &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.rawValue());
    return hashCode;
}

inline hash_type qHash(const QTextTableCell &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.firstPosition());
    hashCode = hashCode * 31 + qHash(value.lastPosition());
    hashCode = hashCode * 31 + qHash(value.row());
    hashCode = hashCode * 31 + qHash(value.column());
    hashCode = hashCode * 31 + qHash(value.rowSpan());
    hashCode = hashCode * 31 + qHash(value.columnSpan());
    hashCode = hashCode * 31 + qHash(value.tableCellFormatIndex());
    return hashCode;
}

inline hash_type qHash(const QAbstractTextDocumentLayout::Selection &value)
{
    hash_type hashCode = qHash(value.cursor);
    hashCode = hashCode * 31 + qHash(value.format);
    return hashCode;
}

inline bool operator==(const QAbstractTextDocumentLayout::Selection &v1, const QAbstractTextDocumentLayout::Selection &v2){
    return v1.cursor==v2.cursor
            && v1.format==v2.format;
}

inline hash_type qHash(const QRgba64 &value)
{
    return qHash(value.operator unsigned long long());
}

inline bool operator==(const QRgba64 &v1, const QRgba64 &v2){
    return v1.operator unsigned long long()==v2.operator unsigned long long();
}

inline hash_type qHash(const QAbstractTextDocumentLayout::PaintContext &value)
{
    hash_type hashCode = qHash(value.cursorPosition);
    hashCode = hashCode * 31 + qHash(value.palette);
    hashCode = hashCode * 31 + qHash(value.clip);
    hashCode = hashCode * 31 + qHash(value.selections);
    return hashCode;
}

inline bool operator==(const QAbstractTextDocumentLayout::PaintContext &v1, const QAbstractTextDocumentLayout::PaintContext &v2){
    return v1.cursorPosition==v2.cursorPosition
            && v1.palette==v2.palette
            && v1.clip==v2.clip
            && v1.selections==v2.selections;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0) && defined(QT_JAMBI_RUN)
hash_type qHash(const QColorTransform &value, hash_type seed = 0);
#endif //QT_VERSION >= QT_VERSION_CHECK(5,14,0)

#endif // QTJAMBI_GUI_QHASHES_H
