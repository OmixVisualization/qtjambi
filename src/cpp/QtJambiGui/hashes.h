/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#ifndef QTJAMBIGUI_HASHES_H
#define QTJAMBIGUI_HASHES_H

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtJambi/Global>

#include <QtJambiCore/hashes.h>

#ifndef QTJAMBI_GENERATOR_RUNNING
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

inline hash_type qHash(const QPixmap &value, hash_type seed = 0)
{
    return genericHash(value.handle(), seed);
}

inline hash_type qHash(const QCursor &cursor, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(cursor.shape()));
    if(cursor.shape()==Qt::BitmapCursor){
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        seed = hash(seed, cursor.bitmap(Qt::ReturnByValue));
        seed = hash(seed, cursor.mask(Qt::ReturnByValue));
#else
        seed = hash(seed, cursor.bitmap());
        seed = hash(seed, cursor.mask());
#endif
        seed = hash(seed, cursor.pixmap());
        seed = hash(seed, cursor.hotSpot());
    }
    return seed;
}

inline hash_type qHash(const QPixelFormat &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QColor &color, hash_type seed = 0)
{
    return qHash(quint64(color.rgba64()), seed);
}

inline hash_type qHash(const QBrush &brush, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(brush.style()));
    seed = hash(seed, brush.color());
    return seed;
}

#if QT_VERSION >= 0x050000
inline hash_type qHash(const QGradient &gradient, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, int(gradient.interpolationMode()));
    seed = hash(seed, int(gradient.type()));
    seed = hash(seed, int(gradient.spread()));
    seed = hash(seed, int(gradient.coordinateMode()));
    for(const QGradientStop& stop : gradient.stops()){
        seed = hash(seed, stop.first);
        seed = hash(seed, stop.second);
    }
    return seed;
}

inline hash_type qHash(const QLinearGradient &gradient, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, gradient.start().x());
    seed = hash(seed, gradient.start().y());
    seed = hash(seed, gradient.finalStop().x());
    seed = hash(seed, gradient.finalStop().y());
    seed = hash(seed, static_cast<const QGradient &>(gradient));
    return seed;
}

inline hash_type qHash(const QRadialGradient &gradient, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, gradient.center().x());
    seed = hash(seed, gradient.center().y());
    seed = hash(seed, gradient.centerRadius());
    seed = hash(seed, gradient.focalPoint().x());
    seed = hash(seed, gradient.focalPoint().y());
    seed = hash(seed, gradient.focalRadius());
    seed = hash(seed, gradient.radius());
    seed = hash(seed, static_cast<const QGradient &>(gradient));
    return seed;
}

inline hash_type qHash(const QConicalGradient &gradient, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, gradient.angle());
    seed = hash(seed, gradient.center().x());
    seed = hash(seed, gradient.center().y());
    seed = hash(seed, static_cast<const QGradient &>(gradient));
    return seed;
}
#endif

inline hash_type qHash(const QRegion &region, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, region.rectCount());
    for(const QRect& rect : region)
        seed = hash(seed, rect);
    return seed;
}

inline hash_type qHash(const QPolygon &polygon, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, polygon.size());
    for (int i=0; i<polygon.size(); ++i)
        seed = hash(seed, polygon.at(i));
    return seed;
}

inline hash_type qHash(const QPalette &palette, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    for (int role=0;role<int(QPalette::NColorRoles);++role) {
        for (int group=0;group<int(QPalette::NColorGroups);++group) {
            seed = hash(seed, palette.color(QPalette::ColorGroup(group), QPalette::ColorRole(role)));
        }
    }
    return seed;
}

#if QT_VERSION < 0x050300
inline hash_type qHash(const QFont &font, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    hash_type hashCode = font.pixelSize();
    hashCode = hashCode * 31 + font.weight();
    hashCode = hashCode * 31 + int(font.style());
    hashCode = hashCode * 31 + font.stretch();
    hashCode = hashCode * 31 + int(font.styleHint());
    hashCode = hashCode * 31 + int(font.styleStrategy());
    hashCode = hashCode * 31 + int(font.fixedPitch());
    seed = hash(seed, font.family());
    seed = hash(seed, font.pointSize());
    hashCode = hashCode * 31 + int(font.underline());
    hashCode = hashCode * 31 + int(font.overline());
    hashCode = hashCode * 31 + int(font.strikeOut());
    hashCode = hashCode * 31 + int(font.kerning());
    return seed;
}
#endif


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QMatrix &matrix, hash_type seed) noexcept
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, matrix.m11());
    seed = hash(seed, matrix.m12());
    seed = hash(seed, matrix.m21());
    seed = hash(seed, matrix.m22());
    seed = hash(seed, matrix.dx());
    seed = hash(seed, matrix.dy());
    return seed;
}
#endif

inline hash_type qHash(const QImage &image, hash_type seed = 0)
{
    if(image.isNull())
        return seed;
    return qHash(image.cacheKey(), seed);
}

inline hash_type qHash(const QPen &pen, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(pen.style()));
    seed = hash(seed, int(pen.capStyle()));
    seed = hash(seed, int(pen.joinStyle()));
    seed = hash(seed, pen.width());
    seed = hash(seed, pen.brush());
    seed = hash(seed, pen.isCosmetic());
    return seed;
}

inline hash_type qHash(const QPolygonF &polygon, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, polygon.size());
    for (int i=0; i<polygon.size(); ++i){
        seed = hash(seed, polygon.at(i).x());
        seed = hash(seed, polygon.at(i).y());
    }
    return seed;
}

inline hash_type qHash(const QVector2D &vec, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, vec.x());
    seed = hash(seed, vec.y());
    return seed;
}

inline hash_type qHash(const QVector3D &vec, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, vec.x());
    seed = hash(seed, vec.y());
    seed = hash(seed, vec.z());
    return seed;
}

inline hash_type qHash(const QVector4D &vec, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, vec.x());
    seed = hash(seed, vec.y());
    seed = hash(seed, vec.z());
    seed = hash(seed, vec.w());
    return seed;
}

inline hash_type qHash(const QFontMetrics &value, hash_type seed = 0)
{
    struct FontMetrics{
        QExplicitlySharedDataPointer<void*> p;
    };
    const FontMetrics* fontMetrics = reinterpret_cast<const FontMetrics* >(&value);

    QtPrivate::QHashCombine hash;
    seed = hash(seed, quintptr(fontMetrics->p.data()));
    return seed;
}

inline hash_type qHash(const QFontMetricsF &value, hash_type seed = 0)
{
    struct FontMetricsF{
        QExplicitlySharedDataPointer<void*> p;
    };
    const FontMetricsF* fontMetrics = reinterpret_cast<const FontMetricsF* >(&value);

    QtPrivate::QHashCombine hash;
    seed = hash(seed, quintptr(fontMetrics->p.data()));
    return seed;
}

inline hash_type qHash(const QGlyphRun &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.rawFont());
    seed = hash(seed, value.glyphIndexes());
    auto positions = value.positions();
    seed = hash(seed, positions.size());
    for (int i=0; i<positions.size(); ++i){
        seed = hash(seed, positions.at(i).x());
        seed = hash(seed, positions.at(i).y());
    }
    seed = hash(seed, value.overline());
    seed = hash(seed, value.underline());
    seed = hash(seed, value.strikeOut());
    seed = hash(seed, value.isRightToLeft());
    seed = hash(seed, int(value.flags()));
    seed = hash(seed, value.boundingRect());
    seed = hash(seed, value.isEmpty());
    return seed;
}

inline hash_type qHash(const QAccessible::State &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix2x2 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix2x3 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix2x4 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix3x2 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix3x3 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix3x4 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix4x2 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix4x3 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QMatrix4x4 &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QOpenGLDebugMessage &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.source()));
    seed = hash(seed, int(value.type()));
    seed = hash(seed, int(value.severity()));
    seed = hash(seed, value.id());
    seed = hash(seed, value.message());
    return seed;
}

inline hash_type qHash(const QOpenGLFramebufferObjectFormat &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.samples());
    seed = hash(seed, value.mipmap());
    seed = hash(seed, int(value.attachment()));
    seed = hash(seed, value.textureTarget());
    seed = hash(seed, value.internalTextureFormat());
    return seed;
}
#else
inline hash_type qHash(const QEventPoint &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, quintptr(value.device()));
    seed = hash(seed, value.ellipseDiameters());
    seed = hash(seed, value.globalGrabPosition().x());
    seed = hash(seed, value.globalGrabPosition().y());
    seed = hash(seed, value.globalLastPosition().x());
    seed = hash(seed, value.globalLastPosition().y());
    seed = hash(seed, value.globalPosition().x());
    seed = hash(seed, value.globalPosition().y());
    seed = hash(seed, value.globalPressPosition().x());
    seed = hash(seed, value.globalPressPosition().y());
    seed = hash(seed, value.grabPosition().x());
    seed = hash(seed, value.grabPosition().y());
    seed = hash(seed, value.id());
    seed = hash(seed, value.isAccepted());
    seed = hash(seed, value.lastPosition().x());
    seed = hash(seed, value.lastPosition().y());
    seed = hash(seed, value.lastTimestamp());
    seed = hash(seed, value.normalizedPosition().x());
    seed = hash(seed, value.normalizedPosition().y());
    seed = hash(seed, value.position().x());
    seed = hash(seed, value.position().y());
    seed = hash(seed, value.pressPosition().x());
    seed = hash(seed, value.pressPosition().y());
    seed = hash(seed, value.pressTimestamp());
    seed = hash(seed, value.pressure());
    seed = hash(seed, value.rotation());
    seed = hash(seed, value.sceneGrabPosition().x());
    seed = hash(seed, value.sceneGrabPosition().y());
    seed = hash(seed, value.sceneLastPosition().x());
    seed = hash(seed, value.sceneLastPosition().y());
    seed = hash(seed, value.scenePosition().x());
    seed = hash(seed, value.scenePosition().y());
    seed = hash(seed, value.scenePressPosition().x());
    seed = hash(seed, value.scenePressPosition().y());
    seed = hash(seed, value.state());
    seed = hash(seed, value.timeHeld());
    seed = hash(seed, value.timestamp());
    seed = hash(seed, value.uniqueId());
    seed = hash(seed, value.velocity());
    return seed;
}

inline hash_type qHash(const QPageRanges::Range &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QPageRanges &value, hash_type seed = 0)
{
    return qHash(value.toRangeList(), seed);
}

#endif

hash_type qHash(const QTextFormat &value, hash_type seed = 0);

inline hash_type qHash(const QTextLayout::FormatRange &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.start);
    seed = hash(seed, value.length);
    seed = hash(seed, value.format);
    return seed;
}

inline hash_type qHash(const QPageSize &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.key());
    seed = hash(seed, int(value.id()));
    seed = hash(seed, int(value.definitionUnits()));
    seed = hash(seed, value.name());
    seed = hash(seed, value.windowsId());
    seed = hash(seed, value.definitionSize());
    seed = hash(seed, value.sizePoints());
    seed = hash(seed, value.rectPoints());
    return seed;
}

inline hash_type qHash(const QPageLayout &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.pageSize());
    seed = hash(seed, int(value.mode()));
    seed = hash(seed, int(value.orientation()));
    seed = hash(seed, int(value.units()));
    seed = hash(seed, value.margins());
    seed = hash(seed, value.marginsPoints());
    seed = hash(seed, value.minimumMargins());
    seed = hash(seed, value.fullRect());
    seed = hash(seed, value.fullRectPoints());
    seed = hash(seed, value.paintRect());
    seed = hash(seed, value.paintRectPoints());
    return seed;
}

inline hash_type qHash(const QPainterPath::Element &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.type));
    seed = hash(seed, value.x);
    seed = hash(seed, value.y);
    return seed;
}

inline hash_type qHash(const QPainterPath &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.fillRule()));
    seed = hash(seed, value.elementCount());
    for(int i=0; i<value.elementCount(); ++i){
        seed = hash(seed, value.elementAt(i));
    }
    return seed;
}

inline hash_type qHash(const QTextOption::Tab &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QTextOption &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.alignment()));
    seed = hash(seed, int(value.textDirection()));
    seed = hash(seed, int(value.wrapMode()));
    seed = hash(seed, int(value.flags()));
    seed = hash(seed, int(value.useDesignMetrics()));
    seed = hash(seed, value.tabStopDistance());
    seed = hash(seed, value.tabArray());
    seed = hash(seed, value.tabs());
    return seed;
}

inline hash_type qHash(const QStaticText &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.textFormat()));
    seed = hash(seed, int(value.performanceHint()));
    seed = hash(seed, value.text());
    seed = hash(seed, value.textWidth());
    seed = hash(seed, value.textOption());
    seed = hash(seed, value.size());
    return seed;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
inline hash_type qHash(const QColorSpace &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, hash_type(value.primaries()));
    seed = hash(seed, hash_type(value.transferFunction()));
    seed = hash(seed, value.gamma());
    seed = hash(seed, value.isValid());
    return seed;
}
#endif

inline hash_type qHash(const QSurfaceFormat &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.swapBehavior()));
    seed = hash(seed, int(value.profile()));
    seed = hash(seed, int(value.renderableType()));
    seed = hash(seed, int(value.options()));
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    seed = hash(seed, value.colorSpace());
#else
    seed = hash(seed, int(value.colorSpace()));
#endif
    seed = hash(seed, value.depthBufferSize());
    seed = hash(seed, value.stencilBufferSize());
    seed = hash(seed, value.redBufferSize());
    seed = hash(seed, value.greenBufferSize());
    seed = hash(seed, value.blueBufferSize());
    seed = hash(seed, value.alphaBufferSize());
    seed = hash(seed, value.samples());
    seed = hash(seed, value.hasAlpha());
    seed = hash(seed, value.majorVersion());
    seed = hash(seed, value.minorVersion());
    seed = hash(seed, value.stereo());
    seed = hash(seed, value.swapInterval());
    return seed;
}

inline hash_type qHash(const QQuaternion &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

#if QT_VERSION < QT_VERSION_CHECK(6,6,0)
inline hash_type qHash(const QPixmapCache::Key &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}
#endif

inline hash_type qHash(const QTextFragment &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.position());
    seed = hash(seed, value.length());
    return seed;
}

inline hash_type qHash(const QTextBlock::iterator &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QTextBlock &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.position());
    seed = hash(seed, value.blockNumber());
    return seed;
}

inline hash_type qHash(const QTextCursor &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.position());
    seed = hash(seed, value.anchor());
    seed = hash(seed, value.selectedText());
    return seed;
}

hash_type qHash(const QTextLength &value, hash_type seed = 0);

inline hash_type qHash(const QTextFormat &value, hash_type seed)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.type());
    seed = hash(seed, value.objectIndex());
    seed = hash(seed, value.objectType());
    seed = hash(seed, value.propertyCount());
    QMap<int, QVariant> properties = value.properties();
    for(int key : properties.keys()){
        seed = hash(seed, key);
        const QVariant& variant = properties[key];
        switch(variant.userType()){
        case QMetaType::Bool:
            seed = hash(seed, variant.toBool());
            break;
        case QMetaType::Int:
            seed = hash(seed, variant.toInt());
            break;
        case QMetaType::Float:
            seed = hash(seed, variant.toFloat());
            break;
        case QMetaType::Double:
            seed = hash(seed, variant.toDouble());
            break;
        case QMetaType::QString:
            seed = hash(seed, variant.toString());
            break;
        case QMetaType::QColor:
            seed = hash(seed, variant.value<QColor>());
            break;
        case QMetaType::QPen:
            seed = hash(seed, variant.value<QPen>());
            break;
        case QMetaType::QTextLength:
            seed = hash(seed, variant.value<QTextLength>());
            break;
        default:
            if(variant.userType()==qMetaTypeId<QList<QTextLength>>()){
                seed = hash(seed, variant.value<QList<QTextLength>>());
            }
            break;
        }
    }
    return seed;
}

inline hash_type qHash(const QTextFrame &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.parentFrame());
    seed = hash(seed, value.firstPosition());
    seed = hash(seed, value.lastPosition());
    return seed;
}

inline hash_type qHash(const QTextFrame::iterator &value, hash_type seed = 0)
{
    return genericHash(value, seed);
}

inline hash_type qHash(const QTextLength &value, hash_type seed)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.type());
    seed = hash(seed, value.rawValue());
    return seed;
}

inline hash_type qHash(const QTextTableCell &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.firstPosition());
    seed = hash(seed, value.lastPosition());
    seed = hash(seed, value.row());
    seed = hash(seed, value.column());
    seed = hash(seed, value.rowSpan());
    seed = hash(seed, value.columnSpan());
    seed = hash(seed, value.tableCellFormatIndex());
    return seed;
}

inline hash_type qHash(const QAbstractTextDocumentLayout::Selection &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.cursor);
    seed = hash(seed, value.format);
    return seed;
}

inline bool operator==(const QAbstractTextDocumentLayout::Selection &v1, const QAbstractTextDocumentLayout::Selection &v2){
    return v1.cursor==v2.cursor
            && v1.format==v2.format;
}

inline hash_type qHash(const QRgba64 &value, hash_type seed = 0)
{
    return qHash(value.operator unsigned long long(), seed);
}

inline bool operator==(const QRgba64 &v1, const QRgba64 &v2){
    return v1.operator unsigned long long()==v2.operator unsigned long long();
}

inline hash_type qHash(const QAbstractTextDocumentLayout::PaintContext &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.cursorPosition);
    seed = hash(seed, value.palette);
    seed = hash(seed, value.clip);
    seed = hash(seed, value.selections);
    return seed;
}

inline bool operator==(const QAbstractTextDocumentLayout::PaintContext &v1, const QAbstractTextDocumentLayout::PaintContext &v2){
    return v1.cursorPosition==v2.cursorPosition
            && v1.palette==v2.palette
            && v1.clip==v2.clip
            && v1.selections==v2.selections;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0) && defined(QTJAMBI_GENERATOR_RUNNING)
hash_type qHash(const QColorTransform &value, hash_type seed = 0);
#endif //QT_VERSION >= QT_VERSION_CHECK(5,14,0)

#ifdef QTJAMBI_GENERATOR_RUNNING
#define QTJAMBIMATRIX(M,N) QMatrix##M##x##N
#define QMATRIX(M,N) QMatrix##M##x##N

#define QTJAMBI_MATRIX_CONTENT(M,N)\
    public:\
        QTJAMBIMATRIX(M,N)();\
        QTJAMBIMATRIX(M,N)(const QMATRIX(M,N)& other);\
        explicit QTJAMBIMATRIX(M,N)(const float *values);\
        QMATRIX(M,N)& operator/=(float divisor);

#define QTJAMBI_MATRIX_EXTRACONTENT(M,N)\
    float& operator ()(int row, int column);\
    const float& operator ()(int row, int column) const;\
    bool isIdentity() const;\
    void setToIdentity();\
    void fill(float value);\
    QMATRIX(M,N)& operator+=(const QMATRIX(M,N)& other);\
    QMATRIX(M,N)& operator-=(const QMATRIX(M,N)& other);\
    QMATRIX(M,N)& operator*=(float factor);\
    bool operator==(const QMATRIX(M,N)& other) const;\
    QMATRIX(N,M) transposed() const;\
    void copyDataTo(float *values) const;\
    float *data();\
    const float *data() const;\
    const float *constData() const;\

#define QTJAMBI_MATRIX(M,N)\
    class QTJAMBIMATRIX(M,N){\
    QTJAMBI_MATRIX_CONTENT(M,N)\
    QTJAMBI_MATRIX_EXTRACONTENT(M,N)\
    };\
    hash_type qHash(const QTJAMBIMATRIX(M,N) &);
QTJAMBI_MATRIX(2,2)
QTJAMBI_MATRIX(2,3)
QTJAMBI_MATRIX(2,4)
QTJAMBI_MATRIX(3,2)
QTJAMBI_MATRIX(3,3)
QTJAMBI_MATRIX(3,4)
QTJAMBI_MATRIX(4,2)
QTJAMBI_MATRIX(4,3)
#endif

#endif // HASHES_H
