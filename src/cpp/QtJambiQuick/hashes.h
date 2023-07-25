/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIQUICK_HASHES_H
#define QTJAMBIQUICK_HASHES_H

#include <QtCore/qglobal.h>
#include <QtJambi/QtJambiAPI>
#include <QtJambiGui/hashes.h>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGGeometry>
#include <QtQuick/QSGMaterialShader>

#ifndef QTJAMBI_GENERATOR_RUNNING
struct RenderState{
    QSGMaterialShader::RenderState::DirtyStates m_dirty;
    const void *m_data;
};
#endif

inline hash_type qHash(const QSGMaterialShader::RenderState &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.dirtyStates());
    seed = hash(seed, quintptr(reinterpret_cast<const RenderState&>(value).m_data));
    return seed;
}
inline bool operator==(const QSGMaterialShader::RenderState &value1, const QSGMaterialShader::RenderState &value2)
{
    return value1.dirtyStates()==value2.dirtyStates()
            && reinterpret_cast<const RenderState&>(value1).m_data==reinterpret_cast<const RenderState&>(value2).m_data;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtQuick/QSGMaterialRhiShader>

inline hash_type qHash(const QSGMaterialRhiShader::RenderState &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.dirtyStates());
    seed = hash(seed, quintptr(reinterpret_cast<const RenderState&>(value).m_data));
    return seed;
}
inline bool operator==(const QSGMaterialRhiShader::RenderState &value1, const QSGMaterialRhiShader::RenderState &value2)
{
    return value1.dirtyStates()==value2.dirtyStates()
            && reinterpret_cast<const RenderState&>(value1).m_data==reinterpret_cast<const RenderState&>(value2).m_data;
}

inline hash_type qHash(const QSGMaterialRhiShader::GraphicsPipelineState &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.blendEnable);
    seed = hash(seed, value.srcColor);
    seed = hash(seed, value.dstColor);
    seed = hash(seed, value.colorWrite);
    seed = hash(seed, value.blendConstant);
    seed = hash(seed, value.cullMode);
    return seed;
}
inline bool operator==(const QSGMaterialRhiShader::GraphicsPipelineState &value1, const QSGMaterialRhiShader::GraphicsPipelineState &value2)
{
    return value1.blendEnable==value2.blendEnable
            && value1.srcColor==value2.srcColor
            && value1.dstColor==value2.dstColor
            && value1.colorWrite==value2.colorWrite
            && value1.blendConstant==value2.blendConstant
            && value1.cullMode==value2.cullMode;
}
#else

inline hash_type qHash(const QSGMaterialShader::GraphicsPipelineState &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.blendEnable);
    seed = hash(seed, value.srcColor);
    seed = hash(seed, value.dstColor);
    seed = hash(seed, value.colorWrite);
    seed = hash(seed, value.blendConstant);
    seed = hash(seed, value.cullMode);
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    seed = hash(seed, value.polygonMode);
#endif
    return seed;
}
inline bool operator==(const QSGMaterialShader::GraphicsPipelineState &value1, const QSGMaterialShader::GraphicsPipelineState &value2)
{
    return value1.blendEnable==value2.blendEnable
            && value1.srcColor==value2.srcColor
            && value1.dstColor==value2.dstColor
            && value1.colorWrite==value2.colorWrite
            && value1.blendConstant==value2.blendConstant
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
            && value1.polygonMode==value2.polygonMode
#endif
            && value1.cullMode==value2.cullMode;
}

#endif

inline hash_type qHash(const QSGGeometry::ColoredPoint2D &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.x);
    seed = hash(seed, value.y);
    seed = hash(seed, value.r);
    seed = hash(seed, value.g);
    seed = hash(seed, value.b);
    seed = hash(seed, value.a);
    return seed;
}
inline bool operator==(const QSGGeometry::ColoredPoint2D &value1, const QSGGeometry::ColoredPoint2D &value2)
{
    return value1.x==value2.x
            && value1.y==value2.y
            && value1.r==value2.r
            && value1.g==value2.g
            && value1.b==value2.b
            && value1.a==value2.a;
}

inline hash_type qHash(const QSGGeometry::TexturedPoint2D &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.x);
    seed = hash(seed, value.y);
    seed = hash(seed, value.tx);
    seed = hash(seed, value.ty);
    return seed;
}
inline bool operator==(const QSGGeometry::TexturedPoint2D &value1, const QSGGeometry::TexturedPoint2D &value2)
{
    return value1.x==value2.x
            && value1.y==value2.y
            && value1.tx==value2.tx
            && value1.ty==value2.ty;
}

inline hash_type qHash(const QSGGeometry::Point2D &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.x);
    seed = hash(seed, value.y);
    return seed;
}
inline bool operator==(const QSGGeometry::Point2D &value1, const QSGGeometry::Point2D &value2)
{
    return value1.x==value2.x
            && value1.y==value2.y;
}

inline hash_type qHash(const QSGGeometry::Attribute &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.position);
    seed = hash(seed, value.tupleSize);
    seed = hash(seed, value.type);
    seed = hash(seed, value.isVertexCoordinate);
    seed = hash(seed, int(value.attributeType));
    return seed;
}
inline bool operator==(const QSGGeometry::Attribute &value1, const QSGGeometry::Attribute &value2)
{
    return value1.position==value2.position
            && value1.tupleSize==value2.tupleSize
            && value1.type==value2.type
            && value1.isVertexCoordinate==value2.isVertexCoordinate
            && value1.attributeType==value2.attributeType;
}

inline hash_type qHash(const QSGGeometry::AttributeSet &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.stride);
    seed = hash(seed, value.count);
    for(int i=0; i<value.count; ++i){
        seed = hash(seed, value.attributes[i]);
    }
    return seed;
}
inline bool operator==(const QSGGeometry::AttributeSet &value1, const QSGGeometry::AttributeSet &value2)
{
    if(value1.stride==value2.stride
            && value1.count==value2.count){
        for(int i=0; i<value1.count; ++i){
            if(!(value1.attributes[i]==value2.attributes[i]))
                return false;
        }
        return true;
    }
    return false;
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QtQuick/QQuickWindow>
inline hash_type qHash(const QQuickWindow::GraphicsStateInfo &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.currentFrameSlot);
    seed = hash(seed, value.framesInFlight);
    return seed;
}
inline bool operator==(const QQuickWindow::GraphicsStateInfo &value1, const QQuickWindow::GraphicsStateInfo &value2)
{
    return value1.currentFrameSlot==value2.currentFrameSlot && value1.framesInFlight==value2.framesInFlight;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#include <QtQuick/QQuickGraphicsDevice>
#include <QtQuick/QQuickGraphicsConfiguration>

inline hash_type qHash(const QQuickGraphicsDevice &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, *reinterpret_cast<quintptr const*>(&value));
    return seed;
}
inline bool operator==(const QQuickGraphicsDevice &value1, const QQuickGraphicsDevice &value2)
{
    return *reinterpret_cast<quintptr const*>(&value1)==*reinterpret_cast<quintptr const*>(&value2);
}
inline hash_type qHash(const QQuickGraphicsConfiguration &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, *reinterpret_cast<quintptr const*>(&value));
    return seed;
}
inline bool operator==(const QQuickGraphicsConfiguration &value1, const QQuickGraphicsConfiguration &value2)
{
    return *reinterpret_cast<quintptr const*>(&value1)==*reinterpret_cast<quintptr const*>(&value2);
}

#include <QtQuick/QQuickRenderTarget>

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtQuick/private/qquickrendertarget_p.h>
inline hash_type qHash(const QQuickRenderTarget &value, hash_type seed = 0)
{
    const QQuickRenderTargetPrivate* p = QQuickRenderTargetPrivate::get(&value);
    if(!p){
        return seed;
    }
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(p->type));
    seed = hash(seed, p->pixelSize);
    seed = hash(seed, p->sampleCount);
    switch(p->type){
    case QQuickRenderTargetPrivate::Type::NativeTexture:
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        seed = hash(seed, p->u.nativeTexture.layoutOrState);
        seed = hash(seed, p->u.nativeTexture.rhiFormat);
        seed = hash(seed, p->u.nativeTexture.rhiFlags);
#else
        seed = hash(seed, p->u.nativeTexture.layout);
#endif
        seed = hash(seed, p->u.nativeTexture.object);
        break;
    case QQuickRenderTargetPrivate::Type::RhiRenderTarget:
        seed = hash(seed, p->u.rhiRt);
        break;
    default:
        break;
    }
    return seed;
}
#else
hash_type qHash(const QQuickRenderTarget &value, hash_type seed = 0);
#endif

#endif

#endif // QTJAMBIQUICK_HASHES_H
