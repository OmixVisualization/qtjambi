/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtGui/qtguiglobal.h>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_CONFIG(vulkan)
#if !__has_include(<vulkan/vulkan.h>)
#define QVULKANINSTANCE_H
typedef typename std::conditional<sizeof(void*)==sizeof(qint64), struct VkImage_T *, uint64_t>::type VkImage;
typedef std::conditional<sizeof(void*)==sizeof(qint64), struct VkPhysicalDevice_T *, uint64_t>::type VkPhysicalDevice;
typedef std::conditional<sizeof(void*)==sizeof(qint64), struct VkDevice_T *, uint64_t>::type VkDevice;
//typedef struct VkDevice_T *VkDevice;
typedef enum VkImageLayout{}VkImageLayout;
typedef enum VkFormat{}VkFormat;
#endif

#include <QtQuick/QQuickRenderTarget>
#include <QtQuick/QQuickGraphicsDevice>
#include <qsgtexture_platform.h>
#endif

#include <QtQuick/QQuickItem>
#include <QtQml/QQmlPropertyValueSource>
#include <QtQuick/QSGGeometry>
#include "utils_p.h"
#include <QtJambi/QtJambiAPI>
#include <QtJambi/qmlapi.h>
#include <QtJambi/qtjambi_cast.h>

class QmlPropertyValueSource : public QQmlPropertyValueSource{
    void setTarget(const QQmlProperty &) override;
};

void QmlPropertyValueSource::setTarget(const QQmlProperty &){}

class ErrorDummyQuickItem : public QQuickItem{
public:
    ErrorDummyQuickItem(int _vsCast, int _viCast);
    ~ErrorDummyQuickItem() override;
private:
    int vsCast;
    int viCast;
};

ErrorDummyQuickItem::ErrorDummyQuickItem(int _vsCast, int _viCast)
    : QQuickItem(), vsCast(_vsCast), viCast(_viCast)
{
    if(vsCast>0){
        void * vsCastPtr = reinterpret_cast<void*>(qintptr(this)+vsCast);
        new(vsCastPtr) QmlPropertyValueSource();
    }
    if(viCast>0){
        void * viCastPtr = reinterpret_cast<void*>(qintptr(this)+viCast);
        Q_UNUSED(viCastPtr)
    }
}

ErrorDummyQuickItem::~ErrorDummyQuickItem(){
    if(vsCast>0){
        QmlPropertyValueSource * vsCastPtr = reinterpret_cast<QmlPropertyValueSource*>(qintptr(this)+vsCast);
        vsCastPtr->~QmlPropertyValueSource();
    }
    if(viCast>0){
        void * viCastPtr = reinterpret_cast<void*>(qintptr(this)+viCast);
        Q_UNUSED(viCastPtr)
    }
}

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGSimpleMaterialShader_isOpenGL)
(JNIEnv *, jclass)
{
#if QT_CONFIG(opengl)
    return true;
#else
    return false;
#endif
}

void initialize_meta_info_registerParserStatusCaster(){
    QmlAPI::registerCreateQmlErrorDummyObjectFunction([](const QMetaObject* metaObject, void* placement, int vsCast, int viCast) -> QObject*{
        if(metaObject->inherits(&QQuickItem::staticMetaObject)){
            return new(placement) ErrorDummyQuickItem(vsCast, viCast);
        }
        return nullptr;
    });
}

// QSGGeometry::vertexData() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_vertexData__)
(JNIEnv *__jni_env,
 jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QSGGeometry *__qt_this = QtJambiAPI::convertJavaObjectToNative<QSGGeometry>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSGGeometry::vertexData() const", __qt_this)
        if(__qt_this->attributeCount() == 1
                && __qt_this->sizeOfVertex() == 2 * sizeof(float)
                && __qt_this->attributes()[0].tupleSize == 2
                && __qt_this->attributes()[0].type == QSGGeometry::FloatType
                && __qt_this->attributes()[0].attributeType == QSGGeometry::PositionAttribute
                && __qt_this->attributes()[0].position == 0)
            _result = Java::QtQuick::QSGGeometry$Point2DVertexData::newInstance(__jni_env, jlong(__qt_this->vertexData()), jint(__qt_this->vertexCount()));
        else if(__qt_this->attributeCount() == 2
                && __qt_this->sizeOfVertex() == 4 * sizeof(float)
                && __qt_this->attributes()[0].tupleSize == 2
                && __qt_this->attributes()[0].type == QSGGeometry::FloatType
                && __qt_this->attributes()[0].attributeType == QSGGeometry::PositionAttribute
                && __qt_this->attributes()[0].position == 0
                && __qt_this->attributes()[1].tupleSize == 2
                && __qt_this->attributes()[1].type == QSGGeometry::FloatType
                && __qt_this->attributes()[1].attributeType == QSGGeometry::TexCoordAttribute
                && __qt_this->attributes()[1].position == 1)
            _result = Java::QtQuick::QSGGeometry$TexturedPoint2DVertexData::newInstance(__jni_env, jlong(__qt_this->vertexData()), jint(__qt_this->vertexCount()));
        else if(__qt_this->attributeCount() == 2
                && __qt_this->sizeOfVertex() == 2 * sizeof(float) + 4 * sizeof(char)
                && __qt_this->attributes()[0].tupleSize == 2
                && __qt_this->attributes()[0].type == QSGGeometry::FloatType
                && __qt_this->attributes()[0].attributeType == QSGGeometry::PositionAttribute
                && __qt_this->attributes()[0].position == 0
                && __qt_this->attributes()[1].tupleSize == 4
                && __qt_this->attributes()[1].type == QSGGeometry::UnsignedByteType
                && __qt_this->attributes()[1].attributeType == QSGGeometry::ColorAttribute
                && __qt_this->attributes()[1].position == 1)
            _result = Java::QtQuick::QSGGeometry$ColoredPoint2DVertexData::newInstance(__jni_env, jlong(__qt_this->vertexData()), jint(__qt_this->vertexCount()));
        else
            _result = Java::QtQuick::QSGGeometry$VertexData::newInstance(__jni_env, jlong(__qt_this->vertexData()), jint(__qt_this->vertexCount()));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getColoredPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QSGGeometry::ColoredPoint2D* vertexData = reinterpret_cast<QSGGeometry::ColoredPoint2D*>(pointer);
        _result = qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getTexturedPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QSGGeometry::TexturedPoint2D* vertexData = reinterpret_cast<QSGGeometry::TexturedPoint2D*>(pointer);
        _result = qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QSGGeometry::Point2D* vertexData = reinterpret_cast<QSGGeometry::Point2D*>(pointer);
        _result = qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setColoredPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    QTJAMBI_TRY{
        QSGGeometry::ColoredPoint2D* vertexData = reinterpret_cast<QSGGeometry::ColoredPoint2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::ColoredPoint2D>(__jni_env, value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setTexturedPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    QTJAMBI_TRY{
        QSGGeometry::TexturedPoint2D* vertexData = reinterpret_cast<QSGGeometry::TexturedPoint2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::TexturedPoint2D>(__jni_env, value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    QTJAMBI_TRY{
        QSGGeometry::Point2D* vertexData = reinterpret_cast<QSGGeometry::Point2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::Point2D>(__jni_env, value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_CONFIG(vulkan)

template<typename T, bool = std::is_pointer<T>::value>
struct CastHelper{
    static constexpr T cast(jlong image) {return reinterpret_cast<T>(image);}
};

template<typename T>
struct CastHelper<T,false>{
    static constexpr T cast(jlong image) {return static_cast<T>(image);}
};

QSGTexture * qtjambi_QSGVulkanTexture_fromNative(JNIEnv *, jlong image, jint layout, QQuickWindow* window, const QSize& size, QQuickWindow::CreateTextureOptions options){
    return QNativeInterface::QSGVulkanTexture::fromNative(CastHelper<VkImage>::cast(image), VkImageLayout(layout), window, size, options);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
QQuickRenderTarget qtjambi_QQuickRenderTarget_fromVulkanImage(JNIEnv *, jlong image, jint layout, const QSize& pixelSize, int sampleCount){
    return QQuickRenderTarget::fromVulkanImage(CastHelper<VkImage>::cast(image), VkImageLayout(layout), pixelSize, sampleCount);
}

QQuickGraphicsDevice qtjambi_QQuickGraphicsDevice_fromPhysicalDevice(JNIEnv *, jlong physicalDevice){
    return QQuickGraphicsDevice::fromPhysicalDevice(VkPhysicalDevice(physicalDevice));
}

QQuickGraphicsDevice qtjambi_QQuickGraphicsDevice_fromDeviceObjects(JNIEnv *, jlong physicalDevice, jlong device, int queueFamilyIndex, int queueIndex){
    return QQuickGraphicsDevice::fromDeviceObjects(VkPhysicalDevice(physicalDevice), VkDevice(device), queueFamilyIndex, queueIndex);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
QQuickRenderTarget qtjambi_QQuickRenderTarget_fromVulkanImage(JNIEnv *, jlong image, jint layout, jint format, const QSize& pixelSize, int sampleCount){
    return QQuickRenderTarget::fromVulkanImage(CastHelper<VkImage>::cast(image), VkImageLayout(layout), VkFormat(format), pixelSize, sampleCount);
}
#endif
#endif
#endif


namespace Java{
namespace QtQuick{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/quick,QQuickItem,
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/quick,QSGGeometry,
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/quick,QSGGeometry$AttributeSet,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(clone,()Lio/qt/quick/QSGGeometry$AttributeSet;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/quick,QSGGeometry$Point2DVertexData,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JI))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/quick,QSGGeometry$TexturedPoint2DVertexData,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JI))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/quick,QSGGeometry$ColoredPoint2DVertexData,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JI))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/quick,QSGGeometry$VertexData,
                                 QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JI))

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/quick,QQuickWindow,
)
}
}
