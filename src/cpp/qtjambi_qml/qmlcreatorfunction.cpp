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

#include <QtCore/QtGlobal>
#include <qtjambi/qtjambi_global.h>
#include <QtCore/QSharedPointer>
#include <QtCore/private/qobject_p.h>
#include <QtQml/QQmlParserStatus>
#include <QtQml/QQmlPropertyValueSource>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_repository.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_qml.h>
#include <qtjambi/qtjambi_application.h>
#include "qmlcreatorfunction.h"

class QmlParserStatus : public QQmlParserStatus{
    inline void classBegin()override{}
    inline void componentComplete()override{}
};

class QmlPropertyValueSource : public QQmlPropertyValueSource{
    inline void setTarget(const QQmlProperty &)override{}
};

class ErrorDummyObject : public QObject{
public:
    inline ErrorDummyObject(int _psCast, int _vsCast, int _viCast)
        : QObject(), psCast(_psCast), vsCast(_vsCast), viCast(_viCast)
    {
        if(psCast>0){
            void * psCastPtr = reinterpret_cast<void*>(qintptr(this)+psCast);
            new(psCastPtr) QmlParserStatus();
        }
        if(vsCast>0){
            void * vsCastPtr = reinterpret_cast<void*>(qintptr(this)+vsCast);
            new(vsCastPtr) QmlPropertyValueSource();
        }
        if(viCast>0){
            void * viCastPtr = reinterpret_cast<void*>(qintptr(this)+viCast);
            Q_UNUSED(viCastPtr)
        }
    }

    inline ~ErrorDummyObject() override {
        if(psCast>0){
            QmlParserStatus* psCastPtr = reinterpret_cast<QmlParserStatus*>(qintptr(this)+psCast);
            psCastPtr->~QmlParserStatus();
        }
        if(vsCast>0){
            QmlPropertyValueSource * vsCastPtr = reinterpret_cast<QmlPropertyValueSource*>(qintptr(this)+vsCast);
            vsCastPtr->~QmlPropertyValueSource();
        }
        if(viCast>0){
            void * viCastPtr = reinterpret_cast<void*>(qintptr(this)+viCast);
            Q_UNUSED(viCastPtr)
        }
    }

private:
    int psCast;
    int vsCast;
    int viCast;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
CreatorFunction creatorFunction(JNIEnv * env, const QMetaObject *meta_object, jclass clazz, jmethodID constructor, size_t objectSize, int psCast, int vsCast, int viCast){
    uint hash = 1;
    hash = 31 * hash + uint(qtjambi_java_object_hashcode(env, clazz));
    hash = 31 * hash + qHash(qint64(constructor));
    hash = 31 * hash + qHash(psCast);
    hash = 31 * hash + qHash(vsCast);
    hash = 31 * hash + qHash(viCast);
    JObjectWrapper clazzWrapper(env, clazz);
    return qtjambi_function_pointer<16,void(void*)>([clazzWrapper, meta_object, constructor, objectSize, psCast, vsCast, viCast](void* placement){
        memset(placement, 0, objectSize);
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QtJambiExceptionInhibitor __exnHandler;
            QTJAMBI_TRY{
                jobject declarativeConstructor = Java::QtCore::QObject$QDeclarativeConstructor::newInstance(env, clazzWrapper.object(), jlong(placement));
                QTJAMBI_TRY{
                    env->NewObject(jclass(clazzWrapper.object()), constructor, declarativeConstructor);
                    qtjambi_throw_java_exception(env);
                }QTJAMBI_CATCH(const JavaException& exn){
                    jlong pl = Java::QtCore::QObject$QDeclarativeConstructor::placement(env, declarativeConstructor);
                    if(pl==0){
                        QObject* obj = reinterpret_cast<QObject*>(placement);
                        obj->deleteLater();
                        __exnHandler.handle(env, exn, nullptr);
                        return;
                    }else{
                        exn.raise();
                    }
                }
                jlong pl = Java::QtCore::QObject$QDeclarativeConstructor::placement(env, declarativeConstructor);
                if(pl!=0){
                    QString className = qtjambi_class_name(env, jclass(clazzWrapper.object()));
                    Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Irreguar implementation of declarative constructor in class %1. Please call super constructor by submitting QDeclarativeConstructor object.").arg(className) QTJAMBI_STACKTRACEINFO );
                }
            }QTJAMBI_CATCH(const JavaException& exn){
                QObject* obj = reinterpret_cast<QObject*>(placement);
                if(QObjectPrivate::get(obj) != nullptr){
                    obj->~QObject();
                }
                obj = createQmlErrorDummyObject(meta_object, placement, vsCast, viCast);
                if(!obj)
                    obj = new (placement) ErrorDummyObject(psCast, vsCast, viCast);
                obj->deleteLater();
                __exnHandler.handle(env, exn, nullptr);
            }QTJAMBI_TRY_END
        }else{
            QObject* obj = createQmlErrorDummyObject(meta_object, placement, vsCast, viCast);
            if(!obj)
                obj = new (placement) ErrorDummyObject(psCast, vsCast, viCast);
            obj->deleteLater();
        }
    }, hash);
}
#else

struct CreatorFunctionMetaData : QSharedData{
    jclass clazz;
    const QMetaObject *meta_object;
    jmethodID constructor;
    size_t objectSize;
    int psCast;
    int vsCast;
    int viCast;
    int fhCast;
};

typedef QHash<hash_type,QSharedDataPointer<CreatorFunctionMetaData>> MetaDataHash;
Q_GLOBAL_STATIC(MetaDataHash, gMetaDataHash)

void* creatorFunctionMetaData(JNIEnv * env, const QMetaObject *meta_object, jclass clazz, jmethodID constructor, size_t objectSize, int psCast, int vsCast, int viCast, int fhCast){
    hash_type hash = qHashMulti(0, qtjambi_java_object_hashcode(env, clazz), qint64(constructor), psCast, vsCast, viCast, fhCast);
    if(gMetaDataHash->contains(hash)){
        return (*gMetaDataHash)[hash];
    }else{
        (*gMetaDataHash)[hash] = QSharedDataPointer<CreatorFunctionMetaData>(new CreatorFunctionMetaData{QSharedData(), qtjambi_to_global_reference(env, clazz), meta_object, constructor, objectSize, psCast, vsCast, viCast, fhCast});
        return (*gMetaDataHash)[hash];
    }
}

void createQmlObject(void* placement,void* _metaData){
    if(CreatorFunctionMetaData* metaData = reinterpret_cast<CreatorFunctionMetaData*>(_metaData)){
        memset(placement, 0, metaData->objectSize);
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            QtJambiExceptionInhibitor __exnHandler;
            QTJAMBI_TRY{
                jobject declarativeConstructor = Java::QtCore::QObject$QDeclarativeConstructor::newInstance(env, metaData->clazz, jlong(placement));
                QTJAMBI_TRY{
                    env->NewObject(metaData->clazz, metaData->constructor, declarativeConstructor);
                    qtjambi_throw_java_exception(env);
                }QTJAMBI_CATCH(const JavaException& exn){
                    jlong pl = Java::QtCore::QObject$QDeclarativeConstructor::placement(env, declarativeConstructor);
                    if(pl==0){
                        QObject* obj = reinterpret_cast<QObject*>(placement);
                        obj->deleteLater();
                        __exnHandler.handle(env, exn, nullptr);
                        return;
                    }else{
                        exn.raise();
                    }
                }QTJAMBI_TRY_END
                jlong pl = Java::QtCore::QObject$QDeclarativeConstructor::placement(env, declarativeConstructor);
                if(pl!=0){
                    QString className = qtjambi_class_name(env, metaData->clazz);
                    Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Irreguar implementation of declarative constructor in class %1. Please call super constructor by submitting QDeclarativeConstructor object.").arg(className) QTJAMBI_STACKTRACEINFO );
                }
            }QTJAMBI_CATCH(const JavaException& exn){
                QObject* obj = reinterpret_cast<QObject*>(placement);
                if(QObjectPrivate::get(obj) != nullptr){
                    obj->~QObject();
                }
                obj = createQmlErrorDummyObject(metaData->meta_object, placement, metaData->vsCast, metaData->viCast);
                if(!obj)
                    obj = new (placement) ErrorDummyObject(metaData->psCast, metaData->vsCast, metaData->viCast);
                obj->deleteLater();
                __exnHandler.handle(env, exn, nullptr);
            }QTJAMBI_TRY_END
        }else{
            QObject* obj = createQmlErrorDummyObject(metaData->meta_object, placement, metaData->vsCast, metaData->viCast);
            if(!obj)
                obj = new (placement) ErrorDummyObject(metaData->psCast, metaData->vsCast, metaData->viCast);
            obj->deleteLater();
        }
    }
}

#endif
