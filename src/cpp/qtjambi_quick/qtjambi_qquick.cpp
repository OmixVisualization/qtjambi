/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include <QtQuick/QQuickItem>
#include <QtQml/QQmlPropertyValueSource>
#include <QtQuick/QSGGeometry>
#include "qtjambi_quick_repository.h"
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_qml.h>
#include <qtjambi/qtjambi_cast.h>

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
    registerCreateQmlErrorDummyObjectFunction([](const QMetaObject* metaObject, void* placement, int vsCast, int viCast) -> QObject*{
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
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSGGeometry::vertexData() const")
    try{
        QSGGeometry *__qt_this = qtjambi_to_object<QSGGeometry>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(__qt_this->attributeCount() == 1
                && __qt_this->sizeOfVertex() == 2 * sizeof(float)
                && __qt_this->attributes()[0].tupleSize == 2
                && __qt_this->attributes()[0].type == QSGGeometry::FloatType
                && __qt_this->attributes()[0].attributeType == QSGGeometry::PositionAttribute
                && __qt_this->attributes()[0].position == 0)
            return Java::QtQuick::QSGGeometry$Point2DVertexData::newInstance(__jni_env, jlong(__qt_this->vertexData()), jint(__qt_this->vertexCount()));
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
            return Java::QtQuick::QSGGeometry$TexturedPoint2DVertexData::newInstance(__jni_env, jlong(__qt_this->vertexData()), jint(__qt_this->vertexCount()));
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
            return Java::QtQuick::QSGGeometry$ColoredPoint2DVertexData::newInstance(__jni_env, jlong(__qt_this->vertexData()), jint(__qt_this->vertexCount()));
        else
            return Java::QtQuick::QSGGeometry$VertexData::newInstance(__jni_env, jlong(__qt_this->vertexData()), jint(__qt_this->vertexCount()));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getColoredPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    try{
        QSGGeometry::ColoredPoint2D* vertexData = reinterpret_cast<QSGGeometry::ColoredPoint2D*>(pointer);
        return qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getTexturedPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    try{
        QSGGeometry::TexturedPoint2D* vertexData = reinterpret_cast<QSGGeometry::TexturedPoint2D*>(pointer);
        return qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_getPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index
 )
{
    try{
        QSGGeometry::Point2D* vertexData = reinterpret_cast<QSGGeometry::Point2D*>(pointer);
        return qtjambi_cast<jobject>(__jni_env, vertexData[index]);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setColoredPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    try{
        QSGGeometry::ColoredPoint2D* vertexData = reinterpret_cast<QSGGeometry::ColoredPoint2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::ColoredPoint2D>(__jni_env, value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setTexturedPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    try{
        QSGGeometry::TexturedPoint2D* vertexData = reinterpret_cast<QSGGeometry::TexturedPoint2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::TexturedPoint2D>(__jni_env, value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_QSGGeometry_setPoint2D)
(JNIEnv *__jni_env,
 jclass,
 jlong pointer,
 jint index,
 jobject value
 )
{
    try{
        QSGGeometry::Point2D* vertexData = reinterpret_cast<QSGGeometry::Point2D*>(pointer);
        vertexData[index] = qtjambi_cast<QSGGeometry::Point2D>(__jni_env, value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

