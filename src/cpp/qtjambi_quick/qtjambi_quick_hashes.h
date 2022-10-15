#ifndef QTJAMBI_QUICK_HASHES_H
#define QTJAMBI_QUICK_HASHES_H

#include <QtCore/qglobal.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGGeometry>
#include <QtQuick/QSGMaterialShader>

#ifndef QT_JAMBI_RUN
struct RenderState{
    QSGMaterialShader::RenderState::DirtyStates m_dirty;
    const void *m_data;
};
#endif

inline hash_type qHash(const QSGMaterialShader::RenderState &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.dirtyStates());
    hashCode = hashCode * 31 + qHash(quintptr(reinterpret_cast<const RenderState&>(value).m_data));
    return hashCode;
}
inline bool operator==(const QSGMaterialShader::RenderState &value1, const QSGMaterialShader::RenderState &value2)
{
    return value1.dirtyStates()==value2.dirtyStates()
            && reinterpret_cast<const RenderState&>(value1).m_data==reinterpret_cast<const RenderState&>(value2).m_data;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtQuick/QSGMaterialRhiShader>

inline hash_type qHash(const QSGMaterialRhiShader::RenderState &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.dirtyStates());
    hashCode = hashCode * 31 + qHash(quintptr(reinterpret_cast<const RenderState&>(value).m_data));
    return hashCode;
}
inline bool operator==(const QSGMaterialRhiShader::RenderState &value1, const QSGMaterialRhiShader::RenderState &value2)
{
    return value1.dirtyStates()==value2.dirtyStates()
            && reinterpret_cast<const RenderState&>(value1).m_data==reinterpret_cast<const RenderState&>(value2).m_data;
}

inline hash_type qHash(const QSGMaterialRhiShader::GraphicsPipelineState &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.blendEnable);
    hashCode = hashCode * 31 + qHash(value.srcColor);
    hashCode = hashCode * 31 + qHash(value.dstColor);
    hashCode = hashCode * 31 + qHash(value.colorWrite);
    hashCode = hashCode * 31 + qHash(value.blendConstant);
    hashCode = hashCode * 31 + qHash(value.cullMode);
    return hashCode;
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

inline hash_type qHash(const QSGMaterialShader::GraphicsPipelineState &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.blendEnable);
    hashCode = hashCode * 31 + qHash(value.srcColor);
    hashCode = hashCode * 31 + qHash(value.dstColor);
    hashCode = hashCode * 31 + qHash(value.colorWrite);
    hashCode = hashCode * 31 + qHash(value.blendConstant);
    hashCode = hashCode * 31 + qHash(value.cullMode);
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    hashCode = hashCode * 31 + qHash(value.polygonMode);
#endif
    return hashCode;
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

inline hash_type qHash(const QSGGeometry::ColoredPoint2D &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.x);
    hashCode = hashCode * 31 + qHash(value.y);
    hashCode = hashCode * 31 + qHash(value.r);
    hashCode = hashCode * 31 + qHash(value.g);
    hashCode = hashCode * 31 + qHash(value.b);
    hashCode = hashCode * 31 + qHash(value.a);
    return hashCode;
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

inline hash_type qHash(const QSGGeometry::TexturedPoint2D &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.x);
    hashCode = hashCode * 31 + qHash(value.y);
    hashCode = hashCode * 31 + qHash(value.tx);
    hashCode = hashCode * 31 + qHash(value.ty);
    return hashCode;
}
inline bool operator==(const QSGGeometry::TexturedPoint2D &value1, const QSGGeometry::TexturedPoint2D &value2)
{
    return value1.x==value2.x
            && value1.y==value2.y
            && value1.tx==value2.tx
            && value1.ty==value2.ty;
}

inline hash_type qHash(const QSGGeometry::Point2D &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.x);
    hashCode = hashCode * 31 + qHash(value.y);
    return hashCode;
}
inline bool operator==(const QSGGeometry::Point2D &value1, const QSGGeometry::Point2D &value2)
{
    return value1.x==value2.x
            && value1.y==value2.y;
}

inline hash_type qHash(const QSGGeometry::Attribute &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.position);
    hashCode = hashCode * 31 + qHash(value.tupleSize);
    hashCode = hashCode * 31 + qHash(value.type);
    hashCode = hashCode * 31 + qHash(value.isVertexCoordinate);
    hashCode = hashCode * 31 + qHash(int(value.attributeType));
    return hashCode;
}
inline bool operator==(const QSGGeometry::Attribute &value1, const QSGGeometry::Attribute &value2)
{
    return value1.position==value2.position
            && value1.tupleSize==value2.tupleSize
            && value1.type==value2.type
            && value1.isVertexCoordinate==value2.isVertexCoordinate
            && value1.attributeType==value2.attributeType;
}

inline hash_type qHash(const QSGGeometry::AttributeSet &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.stride);
    hashCode = hashCode * 31 + qHash(value.count);
    for(int i=0; i<value.count; ++i){
        hashCode = hashCode * 31 + qHash(value.attributes[i]);
    }
    return hashCode;
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
inline hash_type qHash(const QQuickWindow::GraphicsStateInfo &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(value.currentFrameSlot);
    hashCode = hashCode * 31 + qHash(value.framesInFlight);
    return hashCode;
}
inline bool operator==(const QQuickWindow::GraphicsStateInfo &value1, const QQuickWindow::GraphicsStateInfo &value2)
{
    return value1.currentFrameSlot==value2.currentFrameSlot && value1.framesInFlight==value2.framesInFlight;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#include <QtQuick/QQuickGraphicsDevice>
#include <QtQuick/QQuickGraphicsConfiguration>

inline hash_type qHash(const QQuickGraphicsDevice &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(*reinterpret_cast<quintptr const*>(&value));
    return hashCode;
}
inline bool operator==(const QQuickGraphicsDevice &value1, const QQuickGraphicsDevice &value2)
{
    return *reinterpret_cast<quintptr const*>(&value1)==*reinterpret_cast<quintptr const*>(&value2);
}
inline hash_type qHash(const QQuickGraphicsConfiguration &value, hash_type hashCode = 0)
{
    hashCode = hashCode * 31 + qHash(*reinterpret_cast<quintptr const*>(&value));
    return hashCode;
}
inline bool operator==(const QQuickGraphicsConfiguration &value1, const QQuickGraphicsConfiguration &value2)
{
    return *reinterpret_cast<quintptr const*>(&value1)==*reinterpret_cast<quintptr const*>(&value2);
}

#include <QtQuick/QQuickRenderTarget>

#ifndef QT_JAMBI_RUN
#include <QtQuick/private/qquickrendertarget_p.h>
inline hash_type qHash(const QQuickRenderTarget &value, hash_type hashCode = 0)
{
    const QQuickRenderTargetPrivate* p = QQuickRenderTargetPrivate::get(&value);
    if(!p){
        return hashCode;
    }
    hashCode = hashCode * 31 + qHash(int(p->type));
    hashCode = hashCode * 31 + qHash(p->pixelSize);
    hashCode = hashCode * 31 + qHash(p->sampleCount);
    switch(p->type){
    case QQuickRenderTargetPrivate::Type::NativeTexture:
        hashCode = hashCode * 31 + qHash(p->u.nativeTexture.layout);
        hashCode = hashCode * 31 + qHash(p->u.nativeTexture.object);
        break;
    case QQuickRenderTargetPrivate::Type::RhiRenderTarget:
        hashCode = hashCode * 31 + qHash(p->u.rhiRt);
        break;
    default:
        break;
    }
    return hashCode;
}
#else
hash_type qHash(const QQuickRenderTarget &value, hash_type hashCode = 0);
#endif

#endif

#endif // QTJAMBI_QUICK_HASHES_H
