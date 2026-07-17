/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBICANVASPAINTER_HASHES_H
#define QTJAMBICANVASPAINTER_HASHES_H

#include <QtCanvasPainter/QtCanvasPainter>
#include <QtJambiGui/hashes.h>
#include <QtJambi/Global>

inline size_t qHash(const QCanvasBoxShadow &value, size_t seed = 0)
{
    return qHashMulti(seed, value.rect(), value.radius(), value.blur(), value.color(), value.topLeftRadius(), value.topRightRadius(), value.bottomLeftRadius(), value.bottomRightRadius());
}

inline size_t qHash(const QCanvasGridPattern &value, size_t seed = 0)
{
    return qHashMulti(seed, value.cellSize(), value.backgroundColor(), value.lineColor(), value.feather(), value.lineWidth(), value.rotation());
}

size_t qHash(const QCanvasImage &value, size_t seed = 0);

size_t qHash(const QCanvasPath &value, size_t seed = 0);

size_t qHash(const QCanvasCustomBrush &value, size_t seed = 0);

inline size_t qHash(const QCanvasOffscreenCanvas &value, size_t seed = 0)
{
    return qHashMulti(seed, value.fillColor(), qintptr(value.texture()), value.flags());
}

inline size_t qHash(const QCanvasImagePattern &value, size_t seed = 0)
{
    return qHashMulti(seed, value.startPosition().x(), value.startPosition().y(), value.imageSize().width(), value.imageSize().height(), value.rotation(), value.tintColor());
}

inline size_t qHash(const QCanvasGradientStop &value, size_t seed = 0)
{
    return qHashMulti(seed, value.color, value.position);
}

inline size_t qHash(const QCanvasLinearGradient &value, size_t seed = 0){
    return qHashMulti(seed, value.stops(), value.startPosition().x(), value.startPosition().y(), value.endPosition().x(), value.endPosition().y());
}

inline size_t qHash(const QCanvasRadialGradient &value, size_t seed = 0){
    return qHashMulti(seed, value.stops(), value.centerPosition().x(), value.centerPosition().y(), value.outerRadius(), value.innerRadius());
}

inline size_t qHash(const QCanvasConicalGradient &value, size_t seed = 0){
    return qHashMulti(seed, value.stops(), value.centerPosition().x(), value.centerPosition().y(), value.angle());
}

inline size_t qHash(const QCanvasBoxGradient &value, size_t seed = 0){
    return qHashMulti(seed, value.stops(), value.rect(), value.feather(), value.radius());
}

inline size_t qHash(const QCanvasGradient &value, size_t seed = 0)
{
    switch(value.type()) {
    case QCanvasBrush::BrushType::LinearGradient:
        return qHash(reinterpret_cast<const QCanvasLinearGradient &>(value), seed);
    case QCanvasBrush::BrushType::RadialGradient:
        return qHash(reinterpret_cast<const QCanvasRadialGradient &>(value), seed);
    case QCanvasBrush::BrushType::ConicalGradient:
        return qHash(reinterpret_cast<const QCanvasConicalGradient &>(value), seed);
    case QCanvasBrush::BrushType::BoxGradient:
        return qHash(reinterpret_cast<const QCanvasBoxGradient &>(value), seed);
    default:
        return qHashMulti(seed, value.stops());
    }
}

inline size_t qHash(const QCanvasBrush &value, size_t seed = 0)
{
    switch(value.type()) {
    case QCanvasBrush::BrushType::LinearGradient:
#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
        return qHash(reinterpret_cast<const QCanvasLinearGradient &>(value), seed);
#else
        return qHash(value.as<QCanvasLinearGradient>(), seed);
#endif
    case QCanvasBrush::BrushType::RadialGradient:
#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
        return qHash(reinterpret_cast<const QCanvasRadialGradient &>(value), seed);
#else
        return qHash(value.as<QCanvasRadialGradient>(), seed);
#endif
    case QCanvasBrush::BrushType::ConicalGradient:
#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
        return qHash(reinterpret_cast<const QCanvasConicalGradient &>(value), seed);
#else
        return qHash(value.as<QCanvasConicalGradient>(), seed);
#endif
    case QCanvasBrush::BrushType::BoxGradient:
#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
        return qHash(reinterpret_cast<const QCanvasBoxGradient &>(value), seed);
#else
        return qHash(value.as<QCanvasBoxGradient>(), seed);
#endif
    case QCanvasBrush::BrushType::BoxShadow:
#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
        return qHash(reinterpret_cast<const QCanvasBoxShadow &>(value), seed);
#else
        return qHash(value.as<QCanvasBoxShadow>(), seed);
#endif
    case QCanvasBrush::BrushType::ImagePattern:
#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
        return qHash(reinterpret_cast<const QCanvasImagePattern &>(value), seed);
#else
        return qHash(value.as<QCanvasImagePattern>(), seed);
#endif
    case QCanvasBrush::BrushType::GridPattern:
#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
        return qHash(reinterpret_cast<const QCanvasGridPattern &>(value), seed);
#else
        return qHash(value.as<QCanvasGridPattern>(), seed);
#endif
    default:
#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
        if(value.type()>=QCanvasBrush::BrushType::Custom){
            return qHash(reinterpret_cast<const QCanvasCustomBrush &>(value), seed);
        }else{
            return seed;
        }
#else
        if(value.type()>=QCanvasBrush::BrushType::Custom){
            return qHash(value.as<QCanvasCustomBrush>(), seed);
        }else{
            return seed;
        }
#endif
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 12, 0)
inline bool operator==(const QCanvasBrush& v1, const QCanvasBrush& v2){
    if(v1.type()==v2.type()){
        switch(v1.type()){
        case QCanvasBrush::BrushType::LinearGradient:
            return reinterpret_cast<const QCanvasLinearGradient &>(v1)==reinterpret_cast<const QCanvasLinearGradient &>(v2);
        case QCanvasBrush::BrushType::RadialGradient:
            return reinterpret_cast<const QCanvasRadialGradient &>(v1)==reinterpret_cast<const QCanvasRadialGradient &>(v2);
        case QCanvasBrush::BrushType::ConicalGradient:
            return reinterpret_cast<const QCanvasConicalGradient &>(v1)==reinterpret_cast<const QCanvasConicalGradient &>(v2);
        case QCanvasBrush::BrushType::BoxGradient:
            return reinterpret_cast<const QCanvasBoxGradient &>(v1)==reinterpret_cast<const QCanvasBoxGradient &>(v2);
        case QCanvasBrush::BrushType::BoxShadow:
            return reinterpret_cast<const QCanvasBoxShadow &>(v1)==reinterpret_cast<const QCanvasBoxShadow &>(v2);
        case QCanvasBrush::BrushType::ImagePattern:
            return reinterpret_cast<const QCanvasImagePattern &>(v1)==reinterpret_cast<const QCanvasImagePattern &>(v2);
        case QCanvasBrush::BrushType::GridPattern:
            return reinterpret_cast<const QCanvasGridPattern &>(v1)==reinterpret_cast<const QCanvasGridPattern &>(v2);
        default:
            if(v1.type()>=QCanvasBrush::BrushType::Custom){
                return reinterpret_cast<const QCanvasCustomBrush &>(v1)==reinterpret_cast<const QCanvasCustomBrush &>(v2);
            }else{
                return true;
            }
        }
    }
    return false;
}
#endif

#endif // QTJAMBICANVASPAINTER_HASHES_H
