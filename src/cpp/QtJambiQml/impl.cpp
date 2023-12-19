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

#include <QtCore/QtGlobal>
#include <QtQml>
#include <QtCore/private/qobject_p.h>
#include <QtCore/QMutex>
#include <QtQml/private/qqmllist_p.h>

#include <QtQml/qqmlprivate.h>
#include <QtJambi/Global>
#include <QtJambi/MetaInfo>
#include <QtJambi/qmlapi.h>
#include <QtJambi/RegistryAPI>
#include <QtJambi/FunctionPointer>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/CoreAPI>
#include <QtJambi/ContainerAPI>
#include "hashes.h"
#include "utils_p.h"
#include <QtJambi/qtjambi_cast.h>

namespace Java{
namespace QtJambi {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtMetaType,
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(type,()Lio/qt/core/QMetaType$Type;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(name,()Ljava/lang/String;)
                                 QTJAMBI_REPOSITORY_DEFINE_METHOD(id,()I))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,ClassAnalyzerUtility,
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findGeneratedSuperclass,(Ljava/lang/Class;)Ljava/lang/Class;))
}

namespace QtQml{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml,QQmlExpression$ValueIsUndefined,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml,QQmlIncubationController$WhileFlag,
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(flag,J)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml,QQmlListProperty,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml,QQmlListProperty$ReplaceFunction,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Ljava/lang/Object;JLjava/lang/Object;)V))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml,QQmlListProperty$AtFunction,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(apply,(Ljava/lang/Object;J)Ljava/lang/Object;))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml,QJSValue,)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlTypeRegistrationException,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/String;)
)
namespace Util{
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlSingleton,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlElement,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlNamedElement,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlValueType,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlUncreatable,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlUnavailable,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlExtended,
                                    QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Class;))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlAnonymous,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlInterface,)
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlAttached,
                                    QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Class;))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlForeign,
                                    QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Class;)
                                    QTJAMBI_REPOSITORY_DEFINE_METHOD(metaType,()Lio/qt/QtMetaType;))
    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/qml/util,QmlSequencialContainer,
                                    QTJAMBI_REPOSITORY_DEFINE_METHOD(value,()Ljava/lang/Class;)
                                    QTJAMBI_REPOSITORY_DEFINE_METHOD(valueType,()Lio/qt/QtMetaType;))
}
}
}

class QQmlParserStatus;
class QQmlPropertyValueSource;
class QQmlPropertyValueInterceptor;

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
typedef void(*CreatorFunction)(void*);
CreatorFunction creatorFunction(JNIEnv * env, const QMetaObject *meta_object, jclass clazz, jmethodID constructor, size_t objectSize, int psCast, int vsCast, int viCast){
    uint hash = 1;
    hash = 31 * hash + uint(QtJambiAPI::getJavaObjectHashCode(env, clazz));
    hash = 31 * hash + qHash(qint64(constructor));
    hash = 31 * hash + qHash(psCast);
    hash = 31 * hash + qHash(vsCast);
    hash = 31 * hash + qHash(viCast);
    JObjectWrapper clazzWrapper(env, clazz);
    return qtjambi_function_pointer<16,void(void*)>([clazzWrapper, meta_object, constructor, objectSize, psCast, vsCast, viCast](void* placement){
        memset(placement, 0, objectSize);
        if(JniEnvironment env{200}){
            QtJambiExceptionInhibitor __exnHandler;
            QTJAMBI_TRY{
                jobject declarativeConstructor = Java::QtCore::QObject$QDeclarativeConstructor::newInstance(env, clazzWrapper.object(), jlong(placement));
                QTJAMBI_TRY{
                    env->NewObject(jclass(clazzWrapper.object()), constructor, declarativeConstructor);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
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
                    QString className = QtJambiAPI::getClassName(env, jclass(clazzWrapper.object()));
                    Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Irreguar implementation of declarative constructor in class %1. Please call super constructor by submitting QDeclarativeConstructor object.").arg(className) QTJAMBI_STACKTRACEINFO );
                }
            }QTJAMBI_CATCH(const JavaException& exn){
                QObject* obj = reinterpret_cast<QObject*>(placement);
                if(QObjectPrivate::get(obj) != nullptr){
                    obj->~QObject();
                }
                obj = QmlAPI::createQmlErrorDummyObject(meta_object, placement, vsCast, viCast);
                if(!obj)
                    obj = new (placement) ErrorDummyObject(psCast, vsCast, viCast);
                obj->deleteLater();
                __exnHandler.handle(env, exn, nullptr);
            }QTJAMBI_TRY_END
        }else{
            QObject* obj = QmlAPI::createQmlErrorDummyObject(meta_object, placement, vsCast, viCast);
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
    hash_type hash = qHashMulti(0, QtJambiAPI::getJavaObjectHashCode(env, clazz), qint64(constructor), psCast, vsCast, viCast, fhCast);
    if(gMetaDataHash->contains(hash)){
        return (*gMetaDataHash)[hash];
    }else{
        (*gMetaDataHash)[hash] = QSharedDataPointer<CreatorFunctionMetaData>(new CreatorFunctionMetaData{QSharedData(), JavaAPI::toGlobalReference(env, clazz), meta_object, constructor, objectSize, psCast, vsCast, viCast, fhCast});
        return (*gMetaDataHash)[hash];
    }
}

void createQmlObject(void* placement,void* _metaData){
    if(CreatorFunctionMetaData* metaData = reinterpret_cast<CreatorFunctionMetaData*>(_metaData)){
        memset(placement, 0, metaData->objectSize);
        if(JniEnvironment env{200}){
            QtJambiExceptionInhibitor __exnHandler;
            QTJAMBI_TRY{
                jobject declarativeConstructor = Java::QtCore::QObject$QDeclarativeConstructor::newInstance(env, metaData->clazz, jlong(placement));
                QTJAMBI_TRY{
                    env->NewObject(metaData->clazz, metaData->constructor, declarativeConstructor);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
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
                    QString className = QtJambiAPI::getClassName(env, metaData->clazz);
                    Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Irreguar implementation of declarative constructor in class %1. Please call super constructor by submitting QDeclarativeConstructor object.").arg(className) QTJAMBI_STACKTRACEINFO );
                }
            }QTJAMBI_CATCH(const JavaException& exn){
                QObject* obj = reinterpret_cast<QObject*>(placement);
                if(QObjectPrivate::get(obj) != nullptr){
                    obj->~QObject();
                }
                obj = QmlAPI::createQmlErrorDummyObject(metaData->meta_object, placement, metaData->vsCast, metaData->viCast);
                if(!obj)
                    obj = new (placement) ErrorDummyObject(metaData->psCast, metaData->vsCast, metaData->viCast);
                obj->deleteLater();
                __exnHandler.handle(env, exn, nullptr);
            }QTJAMBI_TRY_END
        }else{
            QObject* obj = QmlAPI::createQmlErrorDummyObject(metaData->meta_object, placement, metaData->vsCast, metaData->viCast);
            if(!obj)
                obj = new (placement) ErrorDummyObject(metaData->psCast, metaData->vsCast, metaData->viCast);
            obj->deleteLater();
        }
    }
}

#endif

typedef QObject*(*CreateParentFunction)(QObject*);
CreateParentFunction createParentFunction(JNIEnv * env, jclass clazz, jmethodID constructor){
    if(!constructor)
        return nullptr;
    hash_type hash = 1;
    hash = 31 * hash + qHash(qint64(constructor));
    hash = 31 * hash + uint(QtJambiAPI::getJavaObjectHashCode(env, clazz));
    clazz = JavaAPI::toGlobalReference(env, clazz);
    return qtjambi_function_pointer<16,QObject*(QObject*)>([clazz, constructor](QObject* parent) -> QObject* {
        if(JniEnvironment env{200}){
            jobject result = env->NewObject(clazz, constructor, QtJambiAPI::convertQObjectToJavaObject(env, parent));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return QtJambiAPI::convertJavaObjectToQObject(env, result);
        }
        return nullptr;
    }, hash);
}

struct AttachedPropertiesInfo{
    QQmlAttachedPropertiesFunc function;
    const QMetaObject *meta_object;
};

AttachedPropertiesInfo findQmlAttachedProperties(JNIEnv * env, jclass clazz, jobject attachedAnnotation = nullptr){
    jobjectArray args = env->NewObjectArray(1, Java::Runtime::Class::getClass(env), Java::QtCore::QObject::getClass(env));
    jobject method = Java::Runtime::Class::tryGetDeclaredMethod(env, clazz, env->NewStringUTF("qmlAttachedProperties"), args);
    if(env->ExceptionCheck())
        env->ExceptionClear();
    if(method){
        int modifier = Java::Runtime::Executable::getModifiers(env, method);
        if(!Java::Runtime::Modifier::isStatic(env, modifier)){
            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QString("Attached property method %1 needs to be static.").arg(qtjambi_cast<QString>(env, method)) QTJAMBI_STACKTRACEINFO );
        }
        jclass attachedClass;
        if(attachedAnnotation){
            attachedClass = Java::QtQml::Util::QmlAttached::value(env, attachedAnnotation);
        }else{
            attachedClass = Java::Runtime::Method::getReturnType(env, method);
        }
        const QMetaObject *meta_object = CoreAPI::metaObjectForClass(env, attachedClass);
        if(!meta_object){
            jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, attachedClass);
            if(closestClass){
                const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                meta_object = CoreAPI::metaObjectForClass(env, attachedClass, original_meta_object);
            }
        }
        jmethodID qmlAttachedProperties = env->FromReflectedMethod(method);
        clazz = JavaAPI::toGlobalReference(env, clazz);
        QQmlAttachedPropertiesFunc function = qtjambi_function_pointer<16,QObject*(QObject*)>([clazz, qmlAttachedProperties](QObject* parent) -> QObject* {
            if(JniEnvironment env{200}){
                jobject result = env->CallStaticObjectMethod(clazz, qmlAttachedProperties, QtJambiAPI::convertQObjectToJavaObject(env, parent));
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                return QtJambiAPI::convertJavaObjectToQObject(env, result);
            }
            return nullptr;
        }, QtJambiAPI::getJavaObjectHashCode(env, clazz));
        return AttachedPropertiesInfo{function, meta_object};
    }else if(attachedAnnotation){
        jclass attachedClass = Java::QtQml::Util::QmlAttached::value(env, attachedAnnotation);
        Java::QtQml::QmlTypeRegistrationException::throwNew(env, QString("Class %1 is missing method 'static %2 qmlAttachedProperties(QObject parent)'").arg(QtJambiAPI::getClassName(env, clazz), QtJambiAPI::getClassName(env, attachedClass)) QTJAMBI_STACKTRACEINFO );
    }
    return AttachedPropertiesInfo{nullptr, nullptr};
}

int parserStatusCast(JNIEnv * env, jclass clazz){
    return QmlAPI::getInterfaceOffset(env, clazz, typeid(QQmlParserStatus));
}

int valueSourceCast(JNIEnv * env, jclass clazz){
    return QmlAPI::getInterfaceOffset(env, clazz, typeid(QQmlPropertyValueSource));
}

int valueInterceptorCast(JNIEnv *, jclass){
    return -1;
}

int finalizerHookCast(JNIEnv *, jclass){
    return -1;
}

int registerQQmlListPropertyAsQmlMetaType(JNIEnv *env, const QString& javaName){
    QByteArray listName = "QQmlListProperty<" + javaName.toLatin1().replace(".", "::").replace("$", "::").replace("/", "::") + '>';
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    Q_UNUSED(env)
    return QMetaType::registerNormalizedType(listName,
                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<QQmlListProperty<QObject> >::Destruct,
                                      [](void * ptr, const void * copy)->void*{
                                            const QQmlListProperty<QObject>* _copy = reinterpret_cast<const QQmlListProperty<QObject>*>(copy);
                                            QQmlListProperty<QObject>* list = new(ptr) QQmlListProperty<QObject>();
                                            if(_copy){
                                                list->append = _copy->append;
                                                list->at = _copy->at;
                                                list->clear = _copy->clear;
                                                list->count = _copy->count;
                                                list->data = _copy->data;
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                                                list->dummy1 = _copy->dummy1;
                                                list->dummy2 = _copy->dummy2;
#else
                                                list->replace = _copy->replace;
                                                list->removeLast = _copy->removeLast;
#endif
                                                list->object = _copy->object;
                                            }
                                            return list;
                                      },
                                      sizeof(QQmlListProperty<QObject>),
                                      QMetaType::MovableType | QMetaType::NeedsConstruction | QMetaType::NeedsDestruction,
                                      nullptr
                                      );
#else
    return QmlAPI::registerQmlMetaType(env, Java::QtQml::QQmlListProperty::getClass(env),
                               listName,
                                /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QQmlListProperty<QObject>>::defaultCtr,
                                /*.copyCtr=*/ [](const QtPrivate::QMetaTypeInterface *, void * ptr, const void * copy){
                                                  const QQmlListProperty<QObject>* _copy = reinterpret_cast<const QQmlListProperty<QObject>*>(copy);
                                                  QQmlListProperty<QObject>* list = new(ptr) QQmlListProperty<QObject>();
                                                  if(_copy){
                                                      list->append = _copy->append;
                                                      list->at = _copy->at;
                                                      list->clear = _copy->clear;
                                                      list->count = _copy->count;
                                                      list->data = _copy->data;
                                                      list->replace = _copy->replace;
                                                      list->removeLast = _copy->removeLast;
                                                      list->object = _copy->object;
                                                  }
                                            },
                                /*.moveCtr=*/ [](const QtPrivate::QMetaTypeInterface *, void * ptr, void * copy){
                                                  const QQmlListProperty<QObject>* _copy = reinterpret_cast<const QQmlListProperty<QObject>*>(copy);
                                                  QQmlListProperty<QObject>* list = new(ptr) QQmlListProperty<QObject>();
                                                  if(_copy){
                                                      list->append = std::move(_copy->append);
                                                      list->at = std::move(_copy->at);
                                                      list->clear = std::move(_copy->clear);
                                                      list->count = std::move(_copy->count);
                                                      list->data = std::move(_copy->data);
                                                      list->replace = std::move(_copy->replace);
                                                      list->removeLast = std::move(_copy->removeLast);
                                                      list->object = std::move(_copy->object);
                                                  }
                                            },
                              /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QQmlListProperty<QObject>>::dtor,
                              /*.equals=*/ QtPrivate::QEqualityOperatorForType<QQmlListProperty<QObject>>::equals,
                              /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<QQmlListProperty<QObject>>::lessThan,
                              /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<QQmlListProperty<QObject>>::debugStream,
                              /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<QQmlListProperty<QObject>>::dataStreamOut,
                              /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<QQmlListProperty<QObject>>::dataStreamIn,
                              /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QQmlListProperty<QObject>>::legacyRegisterOp,
                              /*.size=*/ sizeof(QQmlListProperty<QObject>),
                              /*.alignment=*/ alignof(QQmlListProperty<QObject>),
                              /*.typeId=*/ QMetaType::UnknownType,
                              /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QQmlListProperty<QObject>>::Flags),
                              nullptr,
                              nullptr);
#endif
}

/**
 * this method is used by the qml wrapper
 */
int registerQObjectAsQmlMetaType(JNIEnv *env, jclass javaClass, const QString& javaName, const QMetaObject *meta_object)
{
    QByteArray qtName = javaName.toLatin1().replace(".", "::").replace("$", "::").replace("/", "::")+"*";

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    int definedType = QMetaType::type(qtName);
    if(definedType!=QMetaType::UnknownType){
        return definedType;
    }
    int typeId = QMetaType::registerNormalizedType(qtName,
                                     QtMetaTypePrivate::QMetaTypeFunctionHelper<QObject*>::Destruct,
                                     QtMetaTypePrivate::QMetaTypeFunctionHelper<QObject*>::Construct,
                                     sizeof(QObject*),
                                     QMetaType::TypeFlags( QMetaType::MovableType | QMetaType::PointerToQObject ),
                                     meta_object);
    QmlAPI::registerJavaClassForCustomMetaType(env, typeId, javaClass);
    return typeId;
#else
    QMetaType definedType = QMetaType::fromName(qtName);
    if(definedType.id()!=QMetaType::UnknownType){
        return definedType.id();
    }
    if(Java::QtCore::QObject::isAssignableFrom(env, javaClass)){
        return QmlAPI::registerQmlMetaType(env, javaClass, qtName,
                                            /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::defaultCtr,
                                            /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::copyCtr,
                                            /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::moveCtr,
                                            /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::dtor,
                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<QObject*>::equals,
                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<QObject*>::lessThan,
                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<QObject*>::debugStream,
                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<QObject*>::dataStreamOut,
                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<QObject*>::dataStreamIn,
                                            /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<QObject*>::legacyRegisterOp,
                                            /*.size=*/ sizeof(QObject*),
                                            /*.alignment=*/ alignof(QObject*),
                                            /*.typeId=*/ QMetaType::UnknownType,
                                            /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags),
                                            meta_object,
                                            nullptr);
    }else{
        return QmlAPI::registerQmlMetaType(env, javaClass, qtName,
                                            /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::defaultCtr,
                                            /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::copyCtr,
                                            /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::moveCtr,
                                            /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::dtor,
                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                            /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::legacyRegisterOp,
                                            /*.size=*/ sizeof(void*),
                                            /*.alignment=*/ alignof(void*),
                                            /*.typeId=*/ QMetaType::UnknownType,
                                            /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags),
                                            meta_object,
                                            nullptr);
    }
#endif
}

jmethodID findConstructor(JNIEnv * env, jclass clazz, const QString& javaName){
    jmethodID constructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
    jmethodID superClassConstructor = nullptr;
    jclass generatedSuperclass = Java::QtJambi::ClassAnalyzerUtility::findGeneratedSuperclass(env, clazz);
    if(generatedSuperclass){
        superClassConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", generatedSuperclass);
    }else{
        Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class %1 cannot be registered as Qml type since it does not inherit a Qt class.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
    }
    if(!constructor){
        if(superClassConstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QDeclarativeConstructor) to register as Qml type.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }else{
            QString superClassName = QtJambiAPI::getClassName(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')), superClassName) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if(!superClassConstructor){
            QString superClassName = QtJambiAPI::getClassName(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')), superClassName) QTJAMBI_STACKTRACEINFO );
        }
    }
    return constructor;
}

struct QmlTypeRegistractionData{
    QString javaName;
    int typeId = 0;
    int listId = 0;
    int objectSize = 0;
    int psCast = 0;
    int vsCast = 0;
    int viCast = 0;
    int fhCast = 0;
    const QMetaObject *meta_object = nullptr;
    jmethodID constructor = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void (*create)(void *,void *) = nullptr;
    void *userdata = nullptr;
#else
    void (*create)(void *) = nullptr;
#endif
    QVariant (*createValueType)(const QJSValue &);
    QQmlAttachedPropertiesFunc attachedPropertiesFunction = nullptr;
    const QMetaObject *attachedPropertiesMetaObject = nullptr;
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
    QQmlPrivate::ValueTypeCreationMethod creationMethod = QQmlPrivate::ValueTypeCreationMethod::None;
#endif
};

enum RegisterOption{
    SkipCreator = 0x01,
    SkipAttachedProperties = 0x02,
    SkipMetaObject = 0x04,
    SkipObjectSize = 0x08,
    OptionalCreator = 0x010,
    SkipName = 0x20,
};
typedef QFlags<RegisterOption> RegisterOptions;

typedef QVariant (*CreateValueTypeFn)(const QJSValue &);

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
CreateValueTypeFn getCreateValueType(JNIEnv *env, jclass clazz, CreateValueTypeFn defaultFn = nullptr){
    CreateValueTypeFn result = nullptr;
    if(jmethodID jsvFactory = JavaAPI::resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(QtJambiAPI::getClassName(env, clazz).replace('.', '/'))), clazz, true)){
        clazz = JavaAPI::toGlobalReference(env, clazz);
        QMetaType typeId(CoreAPI::metaTypeId(env, clazz));
        if(!typeId.isValid() && Java::QtCore::QObject::isAssignableFrom(env, clazz))
            typeId = QMetaType(QMetaType::QObjectStar);
        result = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                    [clazz,jsvFactory,typeId](const QJSValue & arguments) -> QVariant {
                                        if(JniEnvironment env{300}){
                                            jobject args = qtjambi_cast<jobject>(env, arguments);
                                            jobject result = env->CallStaticObjectMethod(clazz, jsvFactory, args);
                                            QVariant v = QtJambiAPI::convertJavaObjectToQVariant(env, result);
                                            if(typeId.isValid() && v.metaType()!=typeId)
                                                v.convert(typeId);
                                            return v;
                                        }
                                        return QVariant();
                                    }, Java::Runtime::Object::hashCode(env, clazz)
        );
    }
    return result==nullptr ? defaultFn : result;
}
#endif

QmlTypeRegistractionData registerQmlType(JNIEnv *env, jclass clazz, const char* qmlName, RegisterOptions skip = {}){
    QmlTypeRegistractionData data;
    data.javaName = QtJambiAPI::getClassName(env, clazz);
    data.psCast = parserStatusCast(env, clazz);
    data.vsCast = valueSourceCast(env, clazz);
    data.viCast = valueInterceptorCast(env, clazz);
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
    data.fhCast = finalizerHookCast(env, clazz);
#endif
    bool isQObject = Java::QtCore::QObject::isAssignableFrom(env, clazz);
    if(!isQObject){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if (findQmlAttachedProperties(env, clazz).function) {
            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject, but has attached properties. This won't work.").arg(data.javaName) QTJAMBI_STACKTRACEINFO );
        }
        QMetaType typeId(QmlAPI::registerMetaType(env, clazz, data.javaName.replace(".", "/")));
        data.typeId = typeId.id();
        if(data.typeId==0){
            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a valid Qt value type. Valid value tyoes define a default constructor as well as a clone method and implement java.lang.Cloneable.").arg(data.javaName) QTJAMBI_STACKTRACEINFO );
        }
        QString typeName = QLatin1String(qmlName);
        if (!typeName.isEmpty()) {
            if(typeName.at(0).isUpper()){
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Invalid QML %1 name \"%2\"; value type names should begin with a lowercase letter").arg(data.javaName, typeName) QTJAMBI_STACKTRACEINFO );
            }
            int typeNameLen = typeName.length();
            for (int ii = 0; ii < typeNameLen; ++ii) {
                if (!(typeName.at(ii).isLetterOrNumber() || typeName.at(ii) == u'_')) {
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Invalid QML %1 name \"%2\"").arg(data.javaName, typeName) QTJAMBI_STACKTRACEINFO );
                }
            }
        }
        data.listId = QmlAPI::registerMetaType(env, SequentialContainerType::QList, typeId);
        if(!skip.testFlag(RegisterOption::SkipObjectSize)){
            data.objectSize = int(typeId.sizeOf());
        }
        if(!skip.testFlag(RegisterOption::SkipMetaObject)){
            data.meta_object = typeId.metaObject();
        }
        if(!skip.testFlag(RegisterOption::SkipCreator)){
            jmethodID jsvConstructor(nullptr);
            if(!(typeId.flags() & QMetaType::IsPointer)){
                jsvConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/qml/QJSValue;)V", clazz);
            }
            if((!typeId.iface()->copyCtr || !typeId.iface()->defaultCtr) && !jsvConstructor){
                if(!skip.testFlag(RegisterOption::OptionalCreator)){
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is neither a QObject, nor default- and copy-constructible. You should not use it as a QML type.").arg(data.javaName) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                /*if(typeId.iface()->copyCtr && typeId.iface()->defaultCtr){
                    data.create = [](void* placement,void* metaData){
                        const QtPrivate::QMetaTypeInterface* iface = reinterpret_cast<const QtPrivate::QMetaTypeInterface*>(metaData);
                        Q_ASSERT(iface->defaultCtr);
                        iface->defaultCtr(iface, placement);
                    };
                    data.userdata = const_cast<QtPrivate::QMetaTypeInterface*>(typeId.iface());
                }*/
                if(jsvConstructor){
                    clazz = JavaAPI::toGlobalReference(env, clazz);
                    data.createValueType = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                                [clazz,jsvConstructor,typeId](const QJSValue & arguments) -> QVariant {
                                                    if(JniEnvironment env{300}){
                                                        jobject args = qtjambi_cast<jobject>(env, arguments);
                                                        jobject result = env->NewObject(clazz, jsvConstructor, args);
                                                        QVariant v = QtJambiAPI::convertJavaObjectToQVariant(env, result);
                                                        if(v.metaType()!=typeId)
                                                            v.convert(typeId);
                                                        return v;
                                                    }
                                                    return QVariant();
                                                }, Java::Runtime::Object::hashCode(env, clazz)
                    );
                    QmlAPI::registerMetaTypeConverter(env, QMetaType::fromType<QJSValue>(), Java::QtQml::QJSValue::getClass(env), typeId, clazz, jsvConstructor);
                }
            }
        }
#else
        Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject.").arg(data.javaName) QTJAMBI_STACKTRACEINFO );
#endif
    }else{
        QString typeName = QLatin1String(qmlName);
        if (!typeName.isEmpty()) {
            if(typeName.at(0).isLower()){
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Invalid QML %1 name \"%2\"; type names must begin with an uppercase letter").arg(data.javaName, typeName) QTJAMBI_STACKTRACEINFO );
            }
            int typeNameLen = typeName.length();
            for (int ii = 0; ii < typeNameLen; ++ii) {
                if (!(typeName.at(ii).isLetterOrNumber() || typeName.at(ii) == u'_')) {
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Invalid QML %1 name \"%2\"").arg(data.javaName, typeName) QTJAMBI_STACKTRACEINFO );
                }
            }
        }
        if(!skip.testFlag(RegisterOption::SkipObjectSize)){
            data.objectSize = int(QmlAPI::extendedSizeForClass(env, clazz));
        }
        if(!skip.testFlag(RegisterOption::SkipMetaObject)){
            data.meta_object = CoreAPI::metaObjectForClass(env, clazz);
            if(!data.meta_object){
                jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, clazz);
                if(closestClass){
                    const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                    data.meta_object = CoreAPI::metaObjectForClass(env, clazz, original_meta_object);
                }
            }
        }
        data.typeId = registerQObjectAsQmlMetaType(env, clazz, data.javaName, data.meta_object);
        data.listId = registerQQmlListPropertyAsQmlMetaType(env, data.javaName);
        if(!skip.testFlag(RegisterOption::SkipCreator)){
            data.constructor = findConstructor(env, clazz, data.javaName);
            Q_ASSERT(data.constructor);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            data.create = &createQmlObject;
            data.userdata = creatorFunctionMetaData(env, data.meta_object, clazz, data.constructor, data.objectSize, data.psCast, data.vsCast, data.viCast, data.fhCast);
#else
            data.create = creatorFunction(env, data.meta_object, clazz, data.constructor, data.objectSize, data.psCast, data.vsCast, data.viCast);
#endif
        }
        if(!skip.testFlag(RegisterOption::SkipAttachedProperties)){
            AttachedPropertiesInfo info = findQmlAttachedProperties(env, clazz, Java::Runtime::Class::getAnnotation(env, clazz, Java::QtQml::Util::QmlAttached::getClass(env)));
            data.attachedPropertiesFunction = info.function;
            data.attachedPropertiesMetaObject = info.meta_object;
        }
    }
    return data;
}

QObject* qtjambi_qmlAttachedPropertiesObject(JNIEnv *env, jclass clazz, const QObject* obj, bool create){
    const QMetaObject *meta_object = CoreAPI::metaObjectForClass(env, clazz);
    if(!meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            meta_object = CoreAPI::metaObjectForClass(env, clazz, original_meta_object);
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
    int idx = -1;
    return QtQml::qmlAttachedPropertiesObject(&idx, const_cast<QObject *>(obj), meta_object, create);
#else
    const auto func = qmlAttachedPropertiesFunction(nullptr, meta_object);
    return qmlAttachedPropertiesObject(const_cast<QObject *>(obj), func, create);
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)

struct MetaContainerInterfaceExtractor : QMetaContainer{
    static inline const QtMetaContainerPrivate::QMetaSequenceInterface * getInterface(const QMetaSequence& m){
        const QtMetaContainerPrivate::QMetaContainerInterface * iface = reinterpret_cast<const MetaContainerInterfaceExtractor*>(&m)->d_ptr;
        return reinterpret_cast<const QtMetaContainerPrivate::QMetaSequenceInterface*>(iface);
    }
    static inline const QtMetaContainerPrivate::QMetaAssociationInterface * getInterface(const QMetaAssociation& m){
        const QtMetaContainerPrivate::QMetaContainerInterface * iface = reinterpret_cast<const MetaContainerInterfaceExtractor*>(&m)->d_ptr;
        return reinterpret_cast<const QtMetaContainerPrivate::QMetaAssociationInterface*>(iface);
    }
};

int qtjambi_qmlRegisterAnonymousSequentialContainer(JNIEnv *env, jobject containerType, const char* uri, int versionMajor){
    QMetaType metaType = qtjambi_cast<QMetaType>(env, containerType);
    if(!QMetaType::canConvert(metaType, QMetaType::fromType<QSequentialIterable>())){
        Java::QtQml::QmlTypeRegistrationException::throwNew(env, "Not a valid container type." QTJAMBI_STACKTRACEINFO );
    }
    QQmlPrivate::RegisterSequentialContainer type = {
        0,
        uri,
        QTypeRevision::fromMajorVersion(versionMajor),
        nullptr,
        metaType,
        QVariant(metaType).value<QSequentialIterable>().metaContainer(),
        QTypeRevision::zero()
    };

    return QQmlPrivate::qmlregister(QQmlPrivate::SequentialContainerRegistration, &type);
}
#endif

int qtjambi_qmlRegisterAnonymousType(JNIEnv *env, jclass clazz, const char* uri, int versionMajor){
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        Q_UNUSED(clazz)
        Q_UNUSED(uri)
        Q_UNUSED(versionMajor)
        JavaException::raiseQNoImplementationException(env, "qmlRegisterAnonymousType() is not available in Qt versions older than 5.14" QTJAMBI_STACKTRACEINFO);
#else
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions(SkipCreator | SkipObjectSize));

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterType type = {
            /* int version */ 0,

            /*int typeId*/ data.typeId,
            /*int listId*/ data.listId,
            /*int objectSize*/ 0,
            /*void (*create)(void *)*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*const char *uri*/ uri,
            /*int versionMajor*/ int(versionMajor),
            /*int versionMinor*/ 0,
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ data.meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
            /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

            /*int parserStatusCast*/ data.psCast,
            /*int valueSourceCast*/ data.vsCast,
            /*int valueInterceptorCast*/ data.viCast,

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ 0
        };
#else
        QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
            QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
            /* int structVersion */ 1,
#else
            /* int structVersion */ 0,
#endif

            /* QMetaType typeId */ QMetaType(data.typeId),
            /* QMetaType listId */ QMetaType(data.listId),

            /*int objectSize*/ 0,
            /*void (*create)(void *,void *)*/ nullptr,
            /*void *userdata*/ nullptr,
            /*QString noCreationReason*/ QString(),

            /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

            /*const char *uri*/ uri,
            /*int versionMajor*/ QTypeRevision::fromMajorVersion(versionMajor),
            /*const char *elementName*/ nullptr,
            /*const QMetaObject *metaObject*/ data.meta_object,

            /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
            /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

            /*int parserStatusCast*/ data.psCast,
            /*int valueSourceCast*/ data.vsCast,
            /*int valueInterceptorCast*/ data.viCast,

            /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
            /*const QMetaObject *extensionMetaObject*/ nullptr,

            /*QQmlCustomParser *customParser*/ nullptr,
            /*int revision*/ QTypeRevision::zero()
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
            ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
            ,data.creationMethod
#endif
        };
#endif
        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
int qtjambi_qmlRegisterAnonymousType(JNIEnv *env, jclass clazz, int metaObjectRevisionMinor, const char* uri, int versionMajor){
    if(!QTypeRevision::isValidSegment(metaObjectRevisionMinor)){
        Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Not a valid metaObjectRevision %1.").arg(metaObjectRevisionMinor) QTJAMBI_STACKTRACEINFO );
    }
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions(SkipCreator | SkipObjectSize));

    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#else
        /* int structVersion */ 1,
#endif

        /* QMetaType typeId */ QMetaType(data.typeId),
        /* QMetaType listId */ QMetaType(data.listId),

        /*int objectSize*/ 0,
        /*void (*create)(void *,void *)*/ nullptr,
        /*void *userdata*/ nullptr,
        /*QString noCreationReason*/ QString(),

        /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

        /*const char *uri*/ uri,
        /*int versionMajor*/ QTypeRevision::fromMajorVersion(versionMajor),
        /*const char *elementName*/ nullptr,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::fromMinorVersion(metaObjectRevisionMinor)
        ,/*int finalizerCast*/data.fhCast
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,data.creationMethod
#endif
    };
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}
#endif

void qtjambi_qmlRegisterAnonymousTypesAndRevisions(JNIEnv *env, jclass clazz, const char* uri, int versionMajor){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions(OptionalCreator));

    QQmlPrivate::RegisterTypeAndRevisions type = {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        1,
        /* int typeId */ data.typeId,
        /* int listId */ data.listId,
        data.objectSize,
        /*void (*create)(void *)*/ data.create,
        uri,
        versionMajor,
        data.meta_object,
        data.meta_object,
        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,
        data.psCast,
        data.vsCast,
        data.viCast,
        nullptr,
        nullptr,
        nullptr
#else
        3,
        /* QMetaType typeId */ QMetaType(data.typeId),
        /* QMetaType listId */ QMetaType(data.listId),
        data.objectSize,
        /*void (*create)(void *,void *)*/ data.create,
        data.userdata,
        data.createValueType,
        uri,
        QTypeRevision::fromMajorVersion(versionMajor),
        data.meta_object,
        data.meta_object,
        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        nullptr,
        nullptr,

        nullptr,
        nullptr
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,data.fhCast,

        true
#if QT_VERSION >= QT_VERSION_CHECK(6,4,0)
        ,QMetaSequence()
#endif
#endif
#endif
    };

    // Initialize the extension so that we can find it by name or ID.
    QQmlPrivate::qmlregister(QQmlPrivate::TypeAndRevisionsRegistration, &type);
}

int qtjambi_qmlRegisterExtendedType(JNIEnv *env, jclass clazz, jclass extendedClazz, const char* uri, int versionMajor){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions(SkipCreator | SkipObjectSize));
    QString extendedJavaName = QtJambiAPI::getClassName(env, extendedClazz);
    jmethodID extendedConstructor = nullptr;
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        extendedConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!extendedConstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, original_meta_object);
        }
    }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 0,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ 0,
        /*void (*create)(void *)*/ nullptr,
        /*QString noCreationReason*/ QString(),

        /*const char *uri*/ uri,
        /*int versionMajor*/ versionMajor,
        /*int versionMinor*/ 0,
        /*const char *elementName*/ nullptr,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, extendedConstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ 0
    };
#else
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /*int typeId*/ QMetaType(data.typeId),
        /*int listId*/ QMetaType(data.listId),
        /*int objectSize*/ 0,
        /*void (*create)(void *,void *)*/ nullptr,
        /*void *userdata*/ nullptr,
        /*QString noCreationReason*/ QString(),

        /*QVariant (*createValueType)(const QJSValue &);*/ getCreateValueType(env, extendedClazz, data.createValueType),

        /*const char *uri*/ uri,
        /*int version*/ QTypeRevision::fromMajorVersion(versionMajor),
        /*const char *elementName*/ nullptr,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, extendedConstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::zero()
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,data.creationMethod
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterExtendedType(JNIEnv *env, jclass clazz, jclass extendedClazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions());
    QString extendedJavaName = QtJambiAPI::getClassName(env, extendedClazz);
    jmethodID econstructor = nullptr;
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        econstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, original_meta_object);
        }
    }
    if(!data.attachedPropertiesFunction){
        AttachedPropertiesInfo info = findQmlAttachedProperties(env, extendedClazz);
        data.attachedPropertiesFunction = info.function;
        data.attachedPropertiesMetaObject = info.meta_object;
    }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 0,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *)*/ data.create,
        /*QString noCreationReason*/ QString(),

        /*const char *uri*/ uri,
        /*int versionMajor*/ int(versionMajor),
        /*int versionMinor*/ int(versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ 0
    };
#else
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /*int typeId*/ QMetaType(data.typeId),
        /*int listId*/ QMetaType(data.listId),
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *,void *)*/ data.create,
        /*void *userdata*/ data.userdata,
        /*QString noCreationReason*/ QString(),

        /*QVariant (*createValueType)(const QJSValue &);*/ getCreateValueType(env, extendedClazz, data.createValueType),

        /*const char *uri*/ uri,
        /*int version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::zero()
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,data.creationMethod
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterExtendedUncreatableType(JNIEnv *env, jclass clazz, jclass extendedClazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName, QString reason){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions(SkipCreator));
    QString extendedJavaName = QtJambiAPI::getClassName(env, extendedClazz);
    jmethodID econstructor = nullptr;
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        econstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, original_meta_object);
        }
    }
    if(!data.attachedPropertiesFunction){
        AttachedPropertiesInfo info = findQmlAttachedProperties(env, extendedClazz);
        data.attachedPropertiesFunction = info.function;
        data.attachedPropertiesMetaObject = info.meta_object;
    }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 0,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ 0,
        /*void (*create)(void *)*/ nullptr,
        /*QString noCreationReason*/ reason,

        /*const char *uri*/ uri,
        /*int versionMajor*/ int(versionMajor),
        /*int versionMinor*/ int(versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ 0
    };
#else
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /* QMetaType typeId */ QMetaType(data.typeId),
        /* QMetaType listId */ QMetaType(data.listId),
        /*int objectSize*/ 0,
        /*void (*create)(void *,void *)*/ nullptr,
        /*void *userdata*/ nullptr,
        /*QString noCreationReason*/ reason,

        /*QVariant (*createValueType)(const QJSValue &);*/ getCreateValueType(env, extendedClazz),

        /*const char *uri*/ uri,
        /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::zero()
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,data.creationMethod
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterExtendedUncreatableType(JNIEnv *env, jclass clazz, jclass extendedClazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName, QString reason){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions(SkipCreator));
    QString extendedJavaName = QtJambiAPI::getClassName(env, extendedClazz);
    jmethodID econstructor = nullptr;
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        econstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, original_meta_object);
        }
    }
    if(!data.attachedPropertiesFunction){
        AttachedPropertiesInfo info = findQmlAttachedProperties(env, extendedClazz);
        data.attachedPropertiesFunction = info.function;
        data.attachedPropertiesMetaObject = info.meta_object;
    }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 0,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ 0,
        /*void (*create)(void *)*/ nullptr,
        /*QString noCreationReason*/ reason,

        /*const char *uri*/ uri,
        /*int versionMajor*/ int(versionMajor),
        /*int versionMinor*/ int(versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ int(metaObjectRevision)
    };
#else
    if(!QTypeRevision::isValidSegment(metaObjectRevision)){
        Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Not a valid metaObjectRevision %1.").arg(metaObjectRevision) QTJAMBI_STACKTRACEINFO );
    }
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /* QMetaType typeId */ QMetaType(data.typeId),
        /* QMetaType listId */ QMetaType(data.listId),
        /*int objectSize*/ 0,
        /*void (*create)(void *,void *)*/ nullptr,
        /*void *userdata*/ nullptr,
        /*QString noCreationReason*/ reason,

        /*QVariant (*createValueType)(const QJSValue &);*/ getCreateValueType(env, extendedClazz),

        /*const char *uri*/ uri,
        /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::fromMajorVersion(metaObjectRevision)
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,data.creationMethod
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
int qtjambi_qmlRegisterInterface(JNIEnv *env, jclass clazz, const char* uri){
    QString typeName = QtJambiAPI::getClassName(env, clazz);
    const QMetaObject *meta_object = CoreAPI::metaObjectForClass(env, clazz);
    if(!meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            meta_object = CoreAPI::metaObjectForClass(env, clazz, original_meta_object);
        }
    }
    const char* iid = CoreAPI::getInterfaceIID(env, clazz);
    if(!iid){
        iid = RegistryAPI::registerInterfaceID(env, clazz);
    }

    QQmlPrivate::RegisterInterface qmlInterface = {
        /* int version */ 1,

        /* int typeId */ registerQObjectAsQmlMetaType(env, clazz, typeName, meta_object),
        /* int listId */ registerQQmlListPropertyAsQmlMetaType(env, typeName),

        /* const char *iid */ iid,

        /* const char *uri */ uri,
        /* int versionMajor */ 0
    };
    return QQmlPrivate::qmlregister(QQmlPrivate::InterfaceRegistration, &qmlInterface);
}
#endif

int qtjambi_qmlRegisterInterface(JNIEnv *env, jclass clazz, const char* uri, int versionMajor){
    const QMetaObject *meta_object = CoreAPI::metaObjectForClass(env, clazz);
    if(!meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            meta_object = CoreAPI::metaObjectForClass(env, clazz, original_meta_object);
        }
    }
    QString typeName = QtJambiAPI::getClassName(env, clazz);
    const char* iid = CoreAPI::getInterfaceIID(env, clazz);
    if(!iid){
        iid = RegistryAPI::registerInterfaceID(env, clazz);
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterInterface qmlInterface = {
        /* int version */ 1,

        /* int typeId */ registerQObjectAsQmlMetaType(env, clazz, typeName, meta_object),
        /* int listId */ registerQQmlListPropertyAsQmlMetaType(env, typeName),

        /* const char *iid */ iid,

        /* const char *uri */ uri,
        /* int versionMajor */ versionMajor
    };

#else
    QQmlPrivate::RegisterInterface qmlInterface = {
        /* int structVersion */ 0,

        /* QMetaType typeId */ QMetaType(registerQObjectAsQmlMetaType(env, clazz, typeName, meta_object)),
        /* QMetaType listId */ QMetaType(registerQQmlListPropertyAsQmlMetaType(env, typeName)),

        /* const char *iid */ iid,

        /* const char *uri */ uri,
        /* int versionMajor */ QTypeRevision::fromVersion(versionMajor, 0)
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::InterfaceRegistration, &qmlInterface);
}

int qtjambi_qmlRegisterSingletonType(JNIEnv *, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QtQml::ValueCallback& callback){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QQmlPrivate::RegisterSingletonType api = {
            /* int version */     0,
            /* const char *uri */ uri,
            /* int versionMajor */ int(versionMajor),
            /* int versionMinor */ int(versionMinor),
            /* const char *typeName */ qmlName,
            /* scriptApi */ callback,
            /* qobjectApi */ nullptr,
            /* const QMetaObject *instanceMetaObject */ nullptr,
            /* int typeId */ 0,
            /* int revision */ 0
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
            ,/* generalizedQobjectApi */ {}
#endif
        };
#else
        QQmlPrivate::RegisterSingletonType api = {
            /* int structVersion */     0,
            /* const char *uri */ uri,
            /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
            /* const char *typeName */ qmlName,
            /* scriptApi */ callback,
            /* qobjectApi */ nullptr,
            /* const QMetaObject *instanceMetaObject */ nullptr,
            /* QMetaType typeId */ QMetaType(QMetaType::UnknownType),
            /* extensionObjectCreate */ nullptr,
            /* extensionMetaObject */ nullptr,
            /* int revision */ QTypeRevision::zero()
        };
#endif
        return QQmlPrivate::qmlregister(QQmlPrivate::SingletonRegistration, &api);
}

int qtjambi_qmlRegisterSingletonType(JNIEnv *env, jclass clazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QtQml::ObjectCallback& callback){
    QString javaName = QtJambiAPI::getClassName(env, clazz);

    const QMetaObject *meta_object = CoreAPI::metaObjectForClass(env, clazz);
    if(!meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            meta_object = CoreAPI::metaObjectForClass(env, clazz, original_meta_object);
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterSingletonType api = {
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
        /* int version */     3,
#else
        /* int version */     2,
#endif
        /* const char *uri */ uri,
        /* int versionMajor */ int(versionMajor),
        /* int versionMinor */ int(versionMinor),
        /* const char *typeName */ qmlName,
        /* scriptApi */ nullptr,
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
        /* qobjectApi */ nullptr,
#else
        /* qobjectApi */ callback,
#endif
        /* const QMetaObject *instanceMetaObject */ meta_object,
        /* int typeId */ registerQObjectAsQmlMetaType(env, clazz, javaName, meta_object),
        /* int revision */ 0
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
            ,/* generalizedQobjectApi */ callback
#endif
    };
#else
    QQmlPrivate::RegisterSingletonType api = {
        /* int structVersion */     0,
        /* const char *uri */ uri,
        /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /* const char *typeName */ qmlName,
        /* scriptApi */ nullptr,
        /* qobjectApi */ callback,
        /* const QMetaObject *instanceMetaObject */ meta_object,
        /* QMetaType typeId */ QMetaType(registerQObjectAsQmlMetaType(env, clazz, javaName, meta_object)),
        /* extensionObjectCreate */ nullptr,
        /* extensionMetaObject */ nullptr,
        /* int revision */ QTypeRevision::zero()
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::SingletonRegistration, &api);
}

int qtjambi_qmlRegisterRevision(JNIEnv *env, jclass clazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions());

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 1,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *)*/ data.create,
        /*QString noCreationReason*/ QString(),

        /*const char *uri*/ uri,
        /*int versionMajor*/ int(versionMajor),
        /*int versionMinor*/ int(versionMinor),
        /*const char *elementName*/ nullptr,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ int(metaObjectRevision)
    };
#else
    if(!QTypeRevision::isValidSegment(metaObjectRevision)){
        Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Not a valid metaObjectRevision %1.").arg(metaObjectRevision) QTJAMBI_STACKTRACEINFO );
    }
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /*int typeId*/ QMetaType(data.typeId),
        /*int listId*/ QMetaType(data.listId),
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *,void *)*/ data.create,
        /*void *userdata*/ data.userdata,
        /*QString noCreationReason*/ QString(),

        /*QVariant (*createValueType)(const QJSValue &);*/ data.createValueType,

        /*const char *uri*/ uri,
        /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /*const char *elementName*/ nullptr,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::fromMajorVersion(metaObjectRevision)
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,data.creationMethod
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterType(JNIEnv *env, jclass clazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions());

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 1,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *)*/ data.create,
        /*QString noCreationReason*/ QString(),

        /*const char *uri*/ uri,
        /*int versionMajor*/ int(versionMajor),
        /*int versionMinor*/ int(versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ 0
    };
#else
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /*int typeId*/ QMetaType(data.typeId),
        /*int listId*/ QMetaType(data.listId),
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *,void *)*/ data.create,
        /*void *userdata*/ data.userdata,
        /*QString noCreationReason*/ QString(),

        /*QVariant (*createValueType)(const QJSValue &);*/ data.createValueType,

        /*const char *uri*/ uri,
        /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::zero()
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,data.creationMethod
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterType(JNIEnv *env, jclass clazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions());

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 1,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *)*/ data.create,
        /*QString noCreationReason*/ QString(),

        /*const char *uri*/ uri,
        /*int versionMajor*/ int(versionMajor),
        /*int versionMinor*/ int(versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ int(metaObjectRevision)
    };
#else
    if(!QTypeRevision::isValidSegment(metaObjectRevision)){
        Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Not a valid metaObjectRevision %1.").arg(metaObjectRevision) QTJAMBI_STACKTRACEINFO );
    }
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /*int typeId*/ QMetaType(data.typeId),
        /*int listId*/ QMetaType(data.listId),
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *,void *)*/ data.create,
        /*void *userdata*/ data.userdata,
        /*QString noCreationReason*/ QString(),

        /*QVariant (*createValueType)(const QJSValue &);*/ data.createValueType,

        /*const char *uri*/ uri,
        /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        data.psCast,
        data.vsCast,
        data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::fromMajorVersion(metaObjectRevision)
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,data.creationMethod
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterUncreatableType(JNIEnv *env, jclass clazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QString& reason){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions(SkipCreator));
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 1,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ 0,
        /*void (*create)(void *)*/ nullptr,
        /*QString noCreationReason*/ reason,

        /*const char *uri*/ uri,
        /*int versionMajor*/ int(versionMajor),
        /*int versionMinor*/ int(versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ 0
    };
#else
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /*int typeId*/ QMetaType(data.typeId),
        /*int listId*/ QMetaType(data.listId),
        /*int objectSize*/ 0,
        /*void (*create)(void *,void *)*/ nullptr,
        /*void *userdata*/ nullptr,
        /*QString noCreationReason*/ reason,

        /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

        /*const char *uri*/ uri,
        /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::zero()
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,QQmlPrivate::ValueTypeCreationMethod::None
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterUncreatableType(JNIEnv *env, jclass clazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QString& reason){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions(SkipCreator));
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterType type = {
        /* int version */ 1,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ 0,
        /*void (*create)(void *)*/ nullptr,
        /*QString noCreationReason*/ reason,

        /*const char *uri*/ uri,
        /*int versionMajor*/ int(versionMajor),
        /*int versionMinor*/ int(versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ int(metaObjectRevision)
    };
#else
    if(!QTypeRevision::isValidSegment(metaObjectRevision)){
        Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Not a valid metaObjectRevision %1.").arg(metaObjectRevision) QTJAMBI_STACKTRACEINFO );
    }
    QQmlPrivate::RegisterType type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        QQmlPrivate::RegisterType::CurrentVersion,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        /* int structVersion */ 1,
#else
        /* int structVersion */ 0,
#endif

        /*int typeId*/ QMetaType(data.typeId),
        /*int listId*/ QMetaType(data.listId),
        /*int objectSize*/ 0,
        /*void (*create)(void *,void *)*/ nullptr,
        /*void *userdata*/ nullptr,
        /*QString noCreationReason*/ reason,

        /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

        /*const char *uri*/ uri,
        /*QTypeRevision version*/ QTypeRevision::fromVersion(versionMajor, versionMinor),
        /*const char *elementName*/ qmlName,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ QTypeRevision::fromMajorVersion(metaObjectRevision)
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
        ,/*int finalizerCast*/data.fhCast
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
        ,QQmlPrivate::ValueTypeCreationMethod::None
#endif
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)

#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
namespace QQmlPrivate{
using SingletonConstructionMode = ConstructionMode;
}
#endif

QQmlPrivate::SingletonConstructionMode singletonConstructionMode(JNIEnv *env, jclass type, jclass wrapperType, bool singleton, jmethodID& method){
    if(!Java::QtCore::QObject::isAssignableFrom(env, type))
        return QQmlPrivate::SingletonConstructionMode::None;
    if(!env->IsSameObject(type, wrapperType)
            && (method = JavaAPI::resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(QtJambiAPI::getClassName(env, type).replace('.', '/'))), wrapperType, true))){
        return QQmlPrivate::SingletonConstructionMode::FactoryWrapper;
    }
    if(singleton){
        if ((method = JavaAPI::resolveMethod(env, "<init>", "()V", type, false)))
            return QQmlPrivate::SingletonConstructionMode::Constructor;
    }else{
        if ((method = findConstructor(env, type, QtJambiAPI::getClassName(env, type).replace('.', '/'))))
            return QQmlPrivate::SingletonConstructionMode::Constructor;
    }
    if ((method = JavaAPI::resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(QtJambiAPI::getClassName(env, type).replace('.', '/'))), type, true)))
        return QQmlPrivate::SingletonConstructionMode::Factory;
    return QQmlPrivate::SingletonConstructionMode::None;
}

std::function<QObject*(QQmlEngine *, QJSEngine *)> getCreateSingletonFunction(JNIEnv *env, jclass type, jclass wrapperType){
    jmethodID method{nullptr};
    jclass factoryType = type;
    switch(singletonConstructionMode(env, type, wrapperType, true, method)){
    case QQmlPrivate::SingletonConstructionMode::Constructor:
        type = JavaAPI::toGlobalReference(env, type);
        return [type, method](QQmlEngine *, QJSEngine *)->QObject*{
            QObject* result{nullptr};
            if(JniEnvironment env{300}){
                jobject o = env->NewObject(type, method);
                result = qtjambi_cast<QObject*>(env, o);
                QtJambiAPI::setCppOwnershipForTopLevelObject(env, result);
            }
            return result;
        };
    case QQmlPrivate::SingletonConstructionMode::FactoryWrapper:
        factoryType = wrapperType;
        Q_FALLTHROUGH();
    case QQmlPrivate::SingletonConstructionMode::Factory:
        factoryType = JavaAPI::toGlobalReference(env, factoryType);
        return [factoryType, method](QQmlEngine * qe, QJSEngine * je)->QObject*{
            QObject* result{nullptr};
            if(JniEnvironment env{300}){
                jobject o = env->CallStaticObjectMethod(factoryType, method, qtjambi_cast<jobject>(env, qe), qtjambi_cast<jobject>(env, je));
                result = qtjambi_cast<QObject*>(env, o);
                QtJambiAPI::setCppOwnershipForTopLevelObject(env, result);
            }
            return result;
        };
    default: break;
    }
    return nullptr;
}

#else

int qtjambi_qmlRegisterType(JNIEnv *env, jclass clazz){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions(SkipCreator));
    QQmlPrivate::RegisterType type = {
        /* int version */ 0,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ data.objectSize,
        /*void (*create)(void *)*/ nullptr,
        /*QString noCreationReason*/ QString(),

        /*const char *uri*/ nullptr,
        /*int versionMajor*/ 0,
        /*int versionMinor*/ 0,
        /*const char *elementName*/ nullptr,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ nullptr,
        /*const QMetaObject *extensionMetaObject*/ nullptr,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ 0
    };
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterExtendedType(JNIEnv *env, jclass clazz, jclass extendedClazz){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions(SkipCreator | SkipObjectSize));
    QString extendedJavaName = QtJambiAPI::getClassName(env, extendedClazz);
    jmethodID econstructor(nullptr);
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        econstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the econstructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClazz, original_meta_object);
        }
    }

    QQmlPrivate::RegisterType type = {
        /* int version */ 0,

        /*int typeId*/ data.typeId,
        /*int listId*/ data.listId,
        /*int objectSize*/ 0,
        /*void (*create)(void *)*/ nullptr,
        /*QString noCreationReason*/ QString(),

        /*const char *uri*/ nullptr,
        /*int versionMajor*/ 0,
        /*int versionMinor*/ 0,
        /*const char *elementName*/ nullptr,
        /*const QMetaObject *metaObject*/ data.meta_object,

        /*QQmlAttachedPropertiesFunc attachedPropertiesFunction*/ data.attachedPropertiesFunction,
        /*const QMetaObject *attachedPropertiesMetaObject*/ data.attachedPropertiesMetaObject,

        /*int parserStatusCast*/ data.psCast,
        /*int valueSourceCast*/ data.vsCast,
        /*int valueInterceptorCast*/ data.viCast,

        /*QObject *(*extensionObjectCreate)(QObject *)*/ createParentFunction(env, extendedClazz, econstructor),
        /*const QMetaObject *extensionMetaObject*/ extended_meta_object,

        /*QQmlCustomParser *customParser*/ nullptr,
        /*int revision*/ 0
    };
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}
#endif

void qtjambi_qmlRegisterTypesAndRevisions(JNIEnv *env, jobjectArray types, const char* uri, int versionMajor, QList<int>* qmlTypeIds){
    for(jsize i = 0, length = env->GetArrayLength(types); i < length; ++i){
        jclass type = jclass(env->GetObjectArrayElement(types, i));
        if(jobject sequencialContainerAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlSequencialContainer::getClass(env))){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            Q_UNUSED(sequencialContainerAnnotation)
            Java::QtQml::QmlTypeRegistrationException::throwNew(env, "Qml sequential containers are not supported in Qt5." QTJAMBI_STACKTRACEINFO );
#else
            const QMetaObject *type_meta_object = CoreAPI::metaObjectForClass(env, type);
            if(!type_meta_object){
                jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, type);
                if(closestClass){
                    const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                    type_meta_object = CoreAPI::metaObjectForClass(env, type, original_meta_object);
                }
            }
            jclass resolvedClass = type;
            QString resolvedMetaTypeName;
            int resolvedMetaTypeId = 0;
            if(jobject foreignAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlForeign::getClass(env))){
                resolvedClass = Java::QtQml::Util::QmlForeign::value(env, foreignAnnotation);
                jobject metaTypeAnnotation = Java::QtQml::Util::QmlForeign::metaType(env, foreignAnnotation);
                resolvedMetaTypeName = qtjambi_cast<QString>(env, Java::QtJambi::QtMetaType::name(env, metaTypeAnnotation));
                if(resolvedMetaTypeName.isEmpty()){
                    resolvedMetaTypeId = Java::QtJambi::QtMetaType::id(env, metaTypeAnnotation);
                    if(resolvedMetaTypeId==0){
                        resolvedMetaTypeId = qtjambi_cast<QMetaType::Type>(env, Java::QtJambi::QtMetaType::type(env, metaTypeAnnotation));
                        if(resolvedMetaTypeId==0 && env->GetArrayLength(Java::Runtime::Class::getTypeParameters(env, resolvedClass))==1){
                            jclass valueClass = Java::QtQml::Util::QmlSequencialContainer::value(env, sequencialContainerAnnotation);
                            QString valueMetaTypeName;
                            int valueMetaTypeId = 0;
                            jobject valueTypeAnnotation = Java::QtQml::Util::QmlSequencialContainer::valueType(env, sequencialContainerAnnotation);
                            valueMetaTypeName = qtjambi_cast<QString>(env, Java::QtJambi::QtMetaType::name(env, valueTypeAnnotation));
                            if(valueMetaTypeName.isEmpty()){
                                valueMetaTypeId = Java::QtJambi::QtMetaType::id(env, valueTypeAnnotation);
                                if(valueMetaTypeId==0){
                                    valueMetaTypeId = qtjambi_cast<QMetaType::Type>(env, Java::QtJambi::QtMetaType::type(env, valueTypeAnnotation));
                                }
                            }
                            QMetaType valueMetaType;
                            if(valueMetaTypeId!=0){
                                valueMetaType = QMetaType(valueMetaTypeId);
                            }else if(valueMetaTypeName.isEmpty()){
                                valueMetaType = QMetaType(QmlAPI::registerMetaType(env, valueClass, QtJambiAPI::getClassName(env, valueClass).replace(".", "/")));
                            }else{
                                valueMetaType = QMetaType::fromName(valueMetaTypeName.toUtf8());
                            }
                            if(valueMetaType.isValid()){
                                if(Java::QtCore::QStack::isSameClass(env, resolvedClass)){
                                    resolvedMetaTypeId = QmlAPI::registerMetaType(env, SequentialContainerType::QStack, valueMetaType);
                                } else if(Java::QtCore::QQueue::isSameClass(env, resolvedClass)){
                                    resolvedMetaTypeId = QmlAPI::registerMetaType(env, SequentialContainerType::QQueue, valueMetaType);
                                } else if(Java::QtCore::QSet::isAssignableFrom(env, resolvedClass)){
                                    resolvedMetaTypeId = QmlAPI::registerMetaType(env, SequentialContainerType::QSet, valueMetaType);
                                } else if(Java::QtCore::QList::isAssignableFrom(env, resolvedClass)){
                                    resolvedMetaTypeId = QmlAPI::registerMetaType(env, SequentialContainerType::QList, valueMetaType);
                                }
                            }
                        }
                    }
                }
            }
            QMetaType containerMetaType;
            if(resolvedMetaTypeId!=0){
                containerMetaType = QMetaType(resolvedMetaTypeId);
            }else if(resolvedMetaTypeName.isEmpty()){
                containerMetaType = QMetaType(QmlAPI::registerMetaType(env, resolvedClass, QtJambiAPI::getClassName(env, resolvedClass).replace(".", "/")));
            }else{
                containerMetaType = QMetaType::fromName(resolvedMetaTypeName.toUtf8());
            }
            if(!QMetaType::canConvert(containerMetaType, QMetaType::fromType<QSequentialIterable>())){
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a valid container type.").arg(containerMetaType.name()) QTJAMBI_STACKTRACEINFO );
            }
            QQmlPrivate::RegisterSequentialContainerAndRevisions type = {
                0,
                uri,
                QTypeRevision::fromMajorVersion(versionMajor),
                type_meta_object,
                containerMetaType,
                QVariant(containerMetaType).value<QSequentialIterable>().metaContainer(),
                qmlTypeIds
            };
            QQmlPrivate::qmlregister(QQmlPrivate::SequentialContainerAndRevisionsRegistration, &type);
#endif
        }else if(jobject singletonAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlSingleton::getClass(env))){
            Q_UNUSED(singletonAnnotation)
            jclass resolvedClass = type;
            jclass extendedClass = nullptr;
            if(jobject unavailableAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlUnavailable::getClass(env))){
                Q_UNUSED(unavailableAnnotation)
                resolvedClass = Java::Runtime::Object::getClass(env);
            }else if(jobject foreignAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlForeign::getClass(env))){
                resolvedClass = Java::QtQml::Util::QmlForeign::value(env, foreignAnnotation);
            }
            if(jobject extendedAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlExtended::getClass(env))){
                extendedClass = Java::QtQml::Util::QmlExtended::value(env, extendedAnnotation);
            }
            if(!Java::QtCore::QObject::isAssignableFrom(env, resolvedClass))
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Singleton type %1 is not subtyping QObject.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );

            const QMetaObject *type_meta_object = CoreAPI::metaObjectForClass(env, type);
            if(!type_meta_object){
                jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, type);
                if(closestClass){
                    const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                    type_meta_object = CoreAPI::metaObjectForClass(env, type, original_meta_object);
                }
            }
            const QMetaObject *resolved_meta_object = nullptr;
            if(resolvedClass==type){
                resolved_meta_object = type_meta_object;
            }else if(resolvedClass!=Java::Runtime::Object::getClass(env)){
                resolved_meta_object = CoreAPI::metaObjectForClass(env, resolvedClass);
                if(!resolved_meta_object){
                    jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, resolvedClass);
                    if(closestClass){
                        const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                        resolved_meta_object = CoreAPI::metaObjectForClass(env, resolvedClass, original_meta_object);
                    }
                }
            }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            jmethodID constr = JavaAPI::resolveMethod(env, "<init>", "()V", resolvedClass, false);
            resolvedClass = JavaAPI::toGlobalReference(env, resolvedClass);
            if(extendedClass)
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Singleton %1 is extended by type %2. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.'), QtJambiAPI::getClassName(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
#else
            const QMetaObject *extended_meta_object = nullptr;
            jmethodID extendedConstructor = nullptr;
            if(extendedClass){
                if(Java::QtCore::QObject::isAssignableFrom(env, extendedClass))
                    extendedConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClass);
                else
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is a QObject, but is extended by non-QObject type %2. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.'), QtJambiAPI::getClassName(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                CoreAPI::metaObjectForClass(env, extendedClass);
                if(!extended_meta_object){
                    jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, extendedClass);
                    if(closestClass){
                        const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                        extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClass, original_meta_object);
                    }
                }
            }
#endif

            QQmlPrivate::RegisterSingletonTypeAndRevisions api = {
                0,
                uri,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                versionMajor,
                /*QJSValue (*scriptApi)(QQmlEngine *, QJSEngine *)*/ nullptr,
#else
                QTypeRevision::fromMajorVersion(versionMajor),
                getCreateSingletonFunction(env, resolvedClass, type),
#endif
                resolved_meta_object,
                type_meta_object,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                /* int typeId */ registerQObjectAsQmlMetaType(env, resolvedClass, QtJambiAPI::getClassName(env, resolvedClass).replace(".", "/"), resolved_meta_object),
                [resolvedClass, constr](QQmlEngine *, QJSEngine *)->QObject*{
                            QObject* result{nullptr};
                            if(JniEnvironment env{300}){
                                jobject o = env->NewObject(resolvedClass, constr);
                                result = qtjambi_cast<QObject*>(env, o);
                                QtJambiAPI::setCppOwnershipForTopLevelObject(env, result);
                            }
                            return result;
                        },
#else
                QMetaType(resolvedClass!=Java::Runtime::Object::getClass(env) ? QmlAPI::registerMetaType(env, resolvedClass, QtJambiAPI::getClassName(env, resolvedClass).replace(".", "/")) : QMetaType::UnknownType),
                createParentFunction(env, extendedClass, extendedConstructor),
                extended_meta_object,
                qmlTypeIds
#endif
            };
            const int id = QQmlPrivate::qmlregister(QQmlPrivate::SingletonAndRevisionsRegistration, &api);
            if (qmlTypeIds)
                qmlTypeIds->append(id);
        }else if(jobject interfaceAnnotation = Java::Runtime::Class::isInterface(env, type) ? Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlInterface::getClass(env)) : nullptr){
            Q_UNUSED(interfaceAnnotation)
            const int id = qtjambi_qmlRegisterInterface(env, type, uri, versionMajor);
            if (qmlTypeIds)
                qmlTypeIds->append(id);
        }else{
            jclass resolvedClass = type;
            QString resolvedMetaTypeName;
            int resolvedMetaTypeId = 0;
            jclass extendedClass = nullptr;
            if(jobject unavailableAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlUnavailable::getClass(env))){
                Q_UNUSED(unavailableAnnotation)
                resolvedClass = Java::Runtime::Object::getClass(env);
            }else if(jobject foreignAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlForeign::getClass(env))){
                resolvedClass = Java::QtQml::Util::QmlForeign::value(env, foreignAnnotation);
                jobject metaTypeAnnotation = Java::QtQml::Util::QmlForeign::metaType(env, foreignAnnotation);
                resolvedMetaTypeName = qtjambi_cast<QString>(env, Java::QtJambi::QtMetaType::name(env, metaTypeAnnotation));
                if(resolvedMetaTypeName.isEmpty()){
                    resolvedMetaTypeId = Java::QtJambi::QtMetaType::id(env, metaTypeAnnotation);
                    if(resolvedMetaTypeId==0){
                        resolvedMetaTypeId = qtjambi_cast<QMetaType::Type>(env, Java::QtJambi::QtMetaType::type(env, metaTypeAnnotation));
                    }
                }
            }
            if(jobject extendedAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlExtended::getClass(env))){
                extendedClass = Java::QtQml::Util::QmlExtended::value(env, extendedAnnotation);
            }

            const QMetaObject *type_meta_object = CoreAPI::metaObjectForClass(env, type);
            if(!type_meta_object){
                jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, type);
                if(closestClass){
                    const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                    type_meta_object = CoreAPI::metaObjectForClass(env, type, original_meta_object);
                }
            }
            const QMetaObject *resolved_meta_object = nullptr;
            if(resolvedClass==type){
                resolved_meta_object = type_meta_object;
            }else if(resolvedClass!=Java::Runtime::Object::getClass(env)){
                resolved_meta_object = CoreAPI::metaObjectForClass(env, resolvedClass);
                if(!resolved_meta_object){
                    jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, resolvedClass);
                    if(closestClass){
                        const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                        resolved_meta_object = CoreAPI::metaObjectForClass(env, resolvedClass, original_meta_object);
                    }
                }
            }
            bool isQObject = Java::QtCore::QObject::isAssignableFrom(env, resolvedClass);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            if(!isQObject)
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject but used as Qml type. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
#endif
            jmethodID extendedConstructor = nullptr;
            const QMetaObject *extended_meta_object = nullptr;
            if(extendedClass){
                extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClass);
                if(!extended_meta_object){
                    jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, extendedClass);
                    if(closestClass){
                        const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                        extended_meta_object = CoreAPI::metaObjectForClass(env, extendedClass, original_meta_object);
                    }
                }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                if(!isQObject){
                    if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, resolvedClass)){
                        if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, extendedClass)){
                            if(Java::QtCore::QObject::isAssignableFrom(env, extendedClass)){
                                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject, but is extended by QObject subtype %2. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.'), QtJambiAPI::getClassName(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                            }else if(!JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/QtObject$QPrivateConstructor;)V", extendedClass)){
                                QString className = QtJambiAPI::getClassName(env, extendedClass).replace('$', '.');
                                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is missing %2(QPrivateConstructor) constructor.").arg(className, className.mid(className.lastIndexOf('.')+1)) QTJAMBI_STACKTRACEINFO );
                            }
                        }else{
                            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is native value type, but is extended by pure java type %2. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.'), QtJambiAPI::getClassName(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        if(Java::QtCore::QObject::isAssignableFrom(env, extendedClass)){
                            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject, but is extended by QObject subtype %2. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.'), QtJambiAPI::getClassName(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }else if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, extendedClass)){
                            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is pure java type, but is extended by native Qt type %2. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.'), QtJambiAPI::getClassName(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }
                        if(extended_meta_object && !QmlAPI::registerQmlExtension(env, extended_meta_object, resolvedClass))
                            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Cannot use native type %1 as qml extension.").arg(extended_meta_object->className()) QTJAMBI_STACKTRACEINFO );
                    }
                }else
#endif
                if(!Java::QtCore::QObject::isAssignableFrom(env, extendedClass)){
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is a QObject, but is extended by non-QObject type %2. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.'), QtJambiAPI::getClassName(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }else{
                    extendedConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClass);
                }
            }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QString javaName = QtJambiAPI::getClassName(env, resolvedClass).replace(".", "/");
            int typeId = registerQObjectAsQmlMetaType(env, resolvedClass, javaName, resolved_meta_object);
            int listId = registerQQmlListPropertyAsQmlMetaType(env, javaName);
#else
            QMetaType typeId;
            QMetaType listId;
            if(!isQObject){
                if(resolvedMetaTypeId!=0){
                    typeId = QMetaType(resolvedMetaTypeId);
                }else if(resolvedMetaTypeName.isEmpty()){
                    typeId = QMetaType(QmlAPI::registerMetaType(env, resolvedClass, QtJambiAPI::getClassName(env, resolvedClass).replace(".", "/")));
                }else{
                    typeId = QMetaType::fromName(resolvedMetaTypeName.toUtf8());
                }
                if(!typeId.iface()->defaultCtr || !typeId.iface()->copyCtr)
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is neither a QObject, nor default- and copy-constructible. You should not use it as a QML type.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                listId = QMetaType(QmlAPI::registerMetaType(env, SequentialContainerType::QList, typeId));
                if(Java::Runtime::Class::getAnnotation(env, resolvedClass, Java::QtQml::Util::QmlAttached::getClass(env))
                        || findQmlAttachedProperties(env, resolvedClass).function){
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject, but has attached properties. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                QString javaName = QtJambiAPI::getClassName(env, resolvedClass).replace(".", "/");
                typeId = QMetaType(registerQObjectAsQmlMetaType(env, resolvedClass, javaName, resolved_meta_object));
                listId = QMetaType(registerQQmlListPropertyAsQmlMetaType(env, javaName));
            }
#endif

            int objectSize = int(QmlAPI::extendedSizeForClass(env, resolvedClass));
            int psCast = parserStatusCast(env, resolvedClass);
            int vsCast = valueSourceCast(env, resolvedClass);
            int viCast = valueInterceptorCast(env, resolvedClass);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            int fhCast = finalizerHookCast(env, resolvedClass);
            jmethodID constructor{nullptr};
            void (*create)(void *, void *) = nullptr;
            void* userdata = nullptr;
            switch(singletonConstructionMode(env, resolvedClass, resolvedClass, false, constructor)){
            case QQmlPrivate::SingletonConstructionMode::Constructor:
                create = &createQmlObject;
                userdata = creatorFunctionMetaData(env, resolved_meta_object, resolvedClass, constructor, objectSize, psCast, vsCast, viCast, fhCast);
                break;
            default: break;
            }
            QQmlPrivate::CreateValueTypeFunction createValueType = nullptr;
            if(!(typeId.flags() & QMetaType::IsPointer)){
                if(jmethodID jsvConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/qml/QJSValue;)V", resolvedClass)){
                    resolvedClass = JavaAPI::toGlobalReference(env, resolvedClass);
                    createValueType = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                                [resolvedClass,jsvConstructor,typeId](const QJSValue & arguments) -> QVariant {
                                                    if(JniEnvironment env{300}){
                                                        jobject args = qtjambi_cast<jobject>(env, arguments);
                                                        jobject result = env->NewObject(resolvedClass, jsvConstructor, args);
                                                        QVariant v = QtJambiAPI::convertJavaObjectToQVariant(env, result);
                                                        if(v.metaType()!=typeId)
                                                            v.convert(typeId);
                                                        return v;
                                                    }
                                                    return QVariant();
                                                }, Java::Runtime::Object::hashCode(env, resolvedClass)
                    );
                    QmlAPI::registerMetaTypeConverter(env, QMetaType::fromType<QJSValue>(), Java::QtQml::QJSValue::getClass(env), typeId, resolvedClass, jsvConstructor);
                }else if(extendedClass){
                    if(jmethodID jsvFactory = JavaAPI::resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('.', '/'))), extendedClass, true)){
                        extendedClass = JavaAPI::toGlobalReference(env, extendedClass);
                        createValueType = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                                    [extendedClass,jsvFactory,typeId](const QJSValue & arguments) -> QVariant {
                                                        if(JniEnvironment env{300}){
                                                            jobject args = qtjambi_cast<jobject>(env, arguments);
                                                            jobject result = env->CallStaticObjectMethod(extendedClass, jsvFactory, args);
                                                            QVariant v = QtJambiAPI::convertJavaObjectToQVariant(env, result);
                                                            if(v.metaType()!=typeId)
                                                                v.convert(typeId);
                                                            return v;
                                                        }
                                                        return QVariant();
                                                    }, Java::Runtime::Object::hashCode(env, resolvedClass)
                        );
                        QmlAPI::registerMetaTypeConverter(env, QMetaType::fromType<QJSValue>(), Java::QtQml::QJSValue::getClass(env), typeId, resolvedClass, jsvFactory);
                    }
                }
            }
#endif
            AttachedPropertiesInfo attachedPropertiesInfo = findQmlAttachedProperties(env, resolvedClass, Java::Runtime::Class::getAnnotation(env, resolvedClass, Java::QtQml::Util::QmlAttached::getClass(env)));
            QQmlPrivate::RegisterTypeAndRevisions type = {
#if QT_VERSION >= QT_VERSION_CHECK(6,4,0)
                3,
#elif QT_VERSION >= QT_VERSION_CHECK(6,3,0)
                2,
#else
                0,
#endif
                typeId,
                listId,
                objectSize,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                create,
                userdata,
                createValueType,
#else
                creatorFunction(env, resolved_meta_object, resolvedClass, findConstructor(env, resolvedClass, javaName), objectSize, psCast, vsCast, viCast),
#endif
                uri,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                QTypeRevision::fromMajorVersion(versionMajor),
#else
                versionMajor,
#endif
                resolved_meta_object,
                type_meta_object,

                attachedPropertiesInfo.function,
                attachedPropertiesInfo.meta_object,

                psCast,
                vsCast,
                viCast,

                createParentFunction(env, extendedClass, extendedConstructor),
                extended_meta_object,

                &qmlCreateCustomParser<void>,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                qmlTypeIds,
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
                fhCast,

                false,
#if QT_VERSION >= QT_VERSION_CHECK(6,4,0)
                QMetaSequence(),
#endif
#endif
#endif
            };

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            // Initialize the extension so that we can find it by name or ID.
            if(extendedClass){
                QMetaType extendedTypeID(QmlAPI::registerMetaType(env, extendedClass, QtJambiAPI::getClassName(env, extendedClass).replace(".", "/")));
                if(QByteArrayView(extendedTypeID.name())!=extended_meta_object->className()){
                    QMetaType::registerNormalizedTypedef(extended_meta_object->className(), extendedTypeID);
                }
            }
#endif

            const int id = qmlregister(QQmlPrivate::TypeAndRevisionsRegistration, &type);
            if (qmlTypeIds)
                qmlTypeIds->append(id);
        }
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_util_QmlTypes_analyzeType)
(JNIEnv *__jni_env,
 jclass,
 jstring uri0,
 jint versionMajor1,
 jobject qmlTypeIds2,
 jobjectArray types)
{
    QtJambiScope __qtjambi_scope;
    QTJAMBI_TRY {
        J2CStringBuffer __qt_uri0_buffer(__jni_env, jstring(uri0));
        const char* __qt_uri0 = __qt_uri0_buffer;
        QList<int>*  __qt_qmlTypeIds2 = qtjambi_cast<QList<int>* >(__jni_env, __qtjambi_scope, qmlTypeIds2);
        qtjambi_qmlRegisterTypesAndRevisions(__jni_env, types, __qt_uri0, static_cast<int>(versionMajor1), __qt_qmlTypeIds2);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_create)
(JNIEnv *, jclass, jboolean flag)
{
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    return jlong(new volatile bool(flag));
#else
    return jlong(new std::atomic<bool>(flag));
#endif
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_destroy)
(JNIEnv *, jclass, jlong ptr)
{
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    if(ptr) delete reinterpret_cast<volatile bool*>(ptr);
#else
    if(ptr) delete reinterpret_cast<std::atomic<bool>*>(ptr);
#endif
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_set)
(JNIEnv *, jclass, jlong ptr, jboolean flag)
{
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    if(ptr) *reinterpret_cast<volatile bool*>(ptr) = flag;
#else
    if(ptr) *reinterpret_cast<std::atomic<bool>*>(ptr) = flag;
#endif
}

// QQmlEngine::singletonInstance(int)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlEngine_singletonInstance__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId, jint qmlTypeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY {
        QQmlEngine *__qt_this = QtJambiAPI::objectFromNativeId<QQmlEngine>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlEngine::singletonInstance(int)", __qt_this)
        QJSValue value = __qt_this->singletonInstance<QJSValue>(qmlTypeId);
        result = qtjambi_cast<jobject>(__jni_env, value);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
using ConvertVariant = bool(QJSEngine::*)(const QVariant &value, QMetaType metaType, void *ptr);
template <>
constexpr ConvertVariant qjsvalue_cast<ConvertVariant>(const QJSValue &)
{
    return &QJSEngine::convertVariant;
}
// QJSEngine::fromVariant
jobject qtjambi_fromVariant(JNIEnv *env, QJSEngine *__qt_this, jobject type, const QVariant& value){
    QMetaType targetType = qtjambi_cast<QMetaType>(env, type);
    if (value.metaType()==targetType){
        return QtJambiAPI::convertQVariantToJavaObject(env, value);
#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
    }else if (targetType==QMetaType::fromType<QJSValue>()){
        return qtjambi_cast<jobject>(env, qtjambi_toScriptValue(env, __qt_this, value));
    }else if (targetType==QMetaType::fromType<QJSPrimitiveValue>()){
        if(value.userType()==QMetaType::Int){
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value.value<int>()));
        }else if(value.userType()==QMetaType::LongLong){
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value.value<qint64>()));
        }else if(value.userType()==QMetaType::Short){
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value.value<qint16>()));
        }else if(value.userType()==QMetaType::Char){
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value.value<char>()));
        }else if(value.userType()==QMetaType::Double){
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value.value<double>()));
        }else if(value.userType()==QMetaType::Float){
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value.value<float>()));
        }else if(value.userType()==QMetaType::Bool){
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value.value<bool>()));
        }else if(value.userType()==QMetaType::QString){
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value.value<QString>()));
        }else{
            return qtjambi_cast<jobject>(env, __qt_this->toPrimitiveValue(value));
        }
    }else if (targetType==QMetaType::fromType<QJSManagedValue>()){
        jobject result = qtjambi_cast<jobject>(env, new QJSManagedValue(qtjambi_toManagedValue(env, __qt_this, value)));
        QtJambiAPI::setJavaOwnership(env, result);
        return result;
    }else if(value.metaType() == QMetaType::fromType<QJSValue>()){
        return qtjambi_fromScriptValue(env, __qt_this, type, value.value<QJSValue>());
    }else if(value.metaType() == QMetaType::fromType<QJSManagedValue*>() || QMetaType::canConvert(value.metaType(), QMetaType::fromType<QJSManagedValue*>())){
        QJSManagedValue* managed = value.value<QJSManagedValue*>();
        if(managed)
            return qtjambi_fromManagedValue(env, __qt_this, type, *managed);
    }else if(value.metaType() == QMetaType::fromType<QJSPrimitiveValue>()){
        if(targetType.id()==QMetaType::Int){
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<int>(value.value<QJSPrimitiveValue>()));
        }else if(targetType.id()==QMetaType::LongLong){
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<qint64>(value.value<QJSPrimitiveValue>()));
        }else if(targetType.id()==QMetaType::Short){
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<qint16>(value.value<QJSPrimitiveValue>()));
        }else if(targetType.id()==QMetaType::Char){
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<char>(value.value<QJSPrimitiveValue>()));
        }else if(targetType.id()==QMetaType::Double){
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<double>(value.value<QJSPrimitiveValue>()));
        }else if(targetType.id()==QMetaType::Float){
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<float>(value.value<QJSPrimitiveValue>()));
        }else if(targetType.id()==QMetaType::Bool){
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<bool>(value.value<QJSPrimitiveValue>()));
        }else if(targetType.id()==QMetaType::QString){
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<QString>(value.value<QJSPrimitiveValue>()));
        }else{
            return qtjambi_cast<jobject>(env, __qt_this->fromPrimitiveValue<QVariant>(value.value<QJSPrimitiveValue>()));
        }
#endif
    }else{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
        if(!targetType.iface()->defaultCtr){
            Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name())) QTJAMBI_STACKTRACEINFO );
        }
        if(!targetType.iface()->copyCtr){
            Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name())) QTJAMBI_STACKTRACEINFO );
        }
#endif
        QVariant t(targetType, nullptr);
        ConvertVariant convertVariant = qjsvalue_cast<ConvertVariant>(QJSValue{});
        if ((__qt_this->*convertVariant)(value, targetType, t.data())){
            return QtJambiAPI::convertQVariantToJavaObject(env, t);
        }else{
            QMetaType::convert(value.metaType(), value.constData(), targetType, t.data());
            return QtJambiAPI::convertQVariantToJavaObject(env, t);
        }
    }
    return nullptr;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
using ConvertV2 = bool (*)(const QJSValue &value, QMetaType metaType, void *ptr);
template <>
constexpr ConvertV2 qjsvalue_cast<ConvertV2>(const QJSValue &)
{
    return &QJSEngine::convertV2;
}
// QJSEngine::fromScriptValue
jobject qtjambi_fromScriptValue(JNIEnv *env, QJSEngine *, jobject type, const QJSValue& value)
{
    QMetaType targetType = qtjambi_cast<QMetaType>(env, type);
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
    if(!targetType.iface()->defaultCtr){
        Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name())) QTJAMBI_STACKTRACEINFO );
    }
    if(!targetType.iface()->copyCtr){
        Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name())) QTJAMBI_STACKTRACEINFO );
    }
#endif
    ConvertV2 convertV2 = qjsvalue_cast<ConvertV2>(QJSValue{});
    QVariant t(targetType, nullptr);
    if (convertV2(value, targetType, t.data())){
        return QtJambiAPI::convertQVariantToJavaObject(env, t);
    }else if (value.isVariant()){
        t = value.toVariant();
        t.convert(targetType);
        return QtJambiAPI::convertQVariantToJavaObject(env, t);
    }else{
        return QtJambiAPI::convertQVariantToJavaObject(env, QVariant(targetType, nullptr));
    }
}

using Create = QJSValue(QJSEngine::*)(QMetaType type, const void *ptr);
template <>
constexpr Create qjsvalue_cast<Create>(const QJSValue &)
{
    return &QJSEngine::create;
}
// QJSEngine::toScriptValue
QJSValue qtjambi_toScriptValue(JNIEnv *, QJSEngine *__qt_this, const QVariant& variant)
{
    Create create = qjsvalue_cast<Create>(QJSValue{});
    return (__qt_this->*create)(variant.metaType(), variant.data());
}
#else
// QJSEngine::fromScriptValue
jobject qtjambi_fromScriptValue(JNIEnv *env, QJSEngine *, jobject type, const QJSValue& value)
{
    const QMetaType& targetType = qtjambi_cast<const QMetaType&>(env, type);
    QVariant t(targetType.id(), nullptr);
    if (qjsvalue_cast_helper(value, targetType.id(), t.data())){
        return QtJambiAPI::convertQVariantToJavaObject(env, t);
    }else if (value.isVariant()){
        t = value.toVariant();
        t.convert(targetType.id());
        return QtJambiAPI::convertQVariantToJavaObject(env, t);
    }else{
        return QtJambiAPI::convertQVariantToJavaObject(env, QVariant(targetType.id(), nullptr));
    }
}

Q_GLOBAL_STATIC(QThreadStorage<int>, gMetaType)

struct ValueContainer{};
template<>
struct QMetaTypeId2<ValueContainer>{
    enum { Defined = true, IsBuiltIn=false };
    static inline int qt_metatype_id() { return gMetaType->localData(); }
};

// QJSEngine::toScriptValue
QJSValue qtjambi_toScriptValue(JNIEnv *, QJSEngine *__qt_this, const QVariant& variant)
{
    if(variant.constData()){
        void const*const* data = reinterpret_cast<void const*const*>(variant.constData());
        gMetaType->setLocalData(variant.userType());
        return __qt_this->toScriptValue(*reinterpret_cast<const ValueContainer*>(data));
    }else{
        return QJSValue();
    }
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
using ConvertManaged = bool(*)(const QJSManagedValue &value, QMetaType type, void *ptr);
template <>
constexpr ConvertManaged qjsvalue_cast<ConvertManaged>(const QJSValue &)
{
    return &QJSEngine::convertManaged;
}
// QJSEngine::fromManagedValue
jobject qtjambi_fromManagedValue(JNIEnv *env, QJSEngine *, jobject type, const QJSManagedValue& value)
{
    QMetaType targetType = qtjambi_cast<QMetaType>(env, type);
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
    if(!targetType.iface()->defaultCtr){
        Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name())) QTJAMBI_STACKTRACEINFO );
    }
    if(!targetType.iface()->copyCtr){
        Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name())) QTJAMBI_STACKTRACEINFO );
    }
#endif
    ConvertManaged convertManaged = qjsvalue_cast<ConvertManaged>(QJSValue{});
    QVariant t(targetType, nullptr);
    if (convertManaged(value, targetType, t.data())){
        return QtJambiAPI::convertQVariantToJavaObject(env, t);
    }else{
        t = value.toVariant();
        t.convert(targetType);
        return QtJambiAPI::convertQVariantToJavaObject(env, t);
    }
}

using CreateManaged = QJSManagedValue(QJSEngine::*)(QMetaType type, const void *ptr);
template <>
constexpr CreateManaged qjsvalue_cast<CreateManaged>(const QJSValue &)
{
    return &QJSEngine::createManaged;
}
// QJSEngine::toManagedValue
QJSManagedValue qtjambi_toManagedValue(JNIEnv *, QJSEngine *__qt_this, const QVariant& variant)
{
    CreateManaged createManaged = qjsvalue_cast<CreateManaged>(QJSManagedValue{});
    return (__qt_this->*createManaged)(variant.metaType(), variant.data());
}
#endif


extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_util_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){
    jobject result{0};
    QTJAMBI_TRY{
        result = MetaInfoAPI::internalAccess(env, cls);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

void deleter_QQmlListProperty(void* ptr, bool){
    delete reinterpret_cast<QQmlListProperty<void> *>(ptr);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_clone_1native)
(JNIEnv *__jni_env, jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::clone(QQmlListProperty)", __qt_this)
        _result = QtJambiAPI::convertNativeToJavaObjectAsCopy(__jni_env, __qt_this, typeid(QQmlListProperty<void>));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// new QQmlListProperty()
void __qt_create_new_QQmlListProperty_0(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QQmlListProperty<void>()")
    QQmlListProperty<void> *__qt_this = new(__qtjambi_ptr) QQmlListProperty<void>();
    Q_UNUSED(__qt_this)
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
typedef int list_int;
#else
typedef qsizetype list_int;
#endif

// new QQmlListProperty(QObject * o, QList<QObject > & list)
void __qt_create_new_QQmlListProperty_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QQmlListProperty<void>(QObject * o, QList<QObject > & list)")
    jobject o0 = __java_arguments[0].l;
    QList<void*>* list = QtJambiAPI::convertJavaObjectToNative<QList<void*>>(__jni_env, __java_arguments[1].l);
    jboolean readonly = __java_arguments[2].z;
    QQmlListProperty<void>* result = new(__qtjambi_ptr) QQmlListProperty<void>(QtJambiAPI::convertJavaObjectToQObject(__jni_env, o0), list);
    if(readonly){
        result->append = nullptr;
        result->clear = nullptr;
        result->replace = nullptr;
        result->removeLast = nullptr;
    }
}

struct FunctionUserData : QtJambiObjectData{
    struct Functions{
        JObjectWrapper appendFunction;
        JObjectWrapper countFunction;
        JObjectWrapper atFunction;
        JObjectWrapper clearFunction;
        JObjectWrapper replaceFunction;
        JObjectWrapper removeLastFunction;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        int metaType;
#else
        QMetaType metaType;
#endif
    };
    FunctionUserData() : QtJambiObjectData() {
    }

    QMap<int,Functions> functions;
    QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)

    static void append(QQmlListProperty<void> * list, void* element){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(CoreAPI::objectDataLock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject appendFunction = functionData->functions[hash].appendFunction.object()){
                    jobject _element;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(QMetaType(functionData->functions[hash].metaType).flags() & QMetaType::PointerToQObject){
#else
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
#endif
                        _element = QtJambiAPI::convertQObjectToJavaObject(env, reinterpret_cast<QObject*>(element));
                    }else{
                        _element = QtJambiAPI::convertQVariantToJavaObject(env, QVariant(functionData->functions[hash].metaType, &element));
                    }
                    Java::Runtime::BiConsumer::accept(env, appendFunction, _object, _element);
                }
            }
        }
    }

    static void* at(QQmlListProperty<void> * list, list_int index) {
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(CoreAPI::objectDataLock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject atFunction = functionData->functions[hash].atFunction.object()){
                    jobject result = Java::QtQml::QQmlListProperty$AtFunction::apply(env, atFunction, _object, jlong(index));
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(QMetaType(functionData->functions[hash].metaType).flags() & QMetaType::PointerToQObject){
#else
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
#endif
                        return QtJambiAPI::convertJavaObjectToQObject(env, result);
                    }else{
                        QVariant variant = QVariant::fromValue(JObjectWrapper(env, result));
                        if(variant.convert(functionData->functions[hash].metaType)){
                            return *reinterpret_cast<void**>(variant.data());
                        }

                    }
                }
            }
        }
        return nullptr;
    }

    static list_int count(QQmlListProperty<void> * list) {
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(CoreAPI::objectDataLock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject countFunction = functionData->functions[hash].countFunction.object()){
                    return Java::Runtime::ToLongFunction::applyAsLong(env, countFunction, _object);
                }
            }
        }
        return 0;
    }

    static void clear(QQmlListProperty<void> * list){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(CoreAPI::objectDataLock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject clearFunction = functionData->functions[hash].clearFunction.object()){
                    Java::Runtime::Consumer::accept(env, clearFunction, _object);
                }
            }
        }
    }

    static void slow_clear(QQmlListProperty<void> *list)
    {
        for (list_int i = 0, end = list->count(list); i < end; ++i)
            list->removeLast(list);
    }

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    static void replace(QQmlListProperty<void> * list, list_int idx, void *element){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(CoreAPI::objectDataLock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(JniEnvironment env{300}){
            jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
            if(jobject replaceFunction = functionData->functions[hash].replaceFunction.object()){
                jobject _element;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(QMetaType(functionData->functions[hash].metaType).flags() & QMetaType::PointerToQObject){
#else
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
#endif
                    _element = QtJambiAPI::convertQObjectToJavaObject(env, reinterpret_cast<QObject*>(element));
                }else{
                    _element = QtJambiAPI::convertQVariantToJavaObject(env, QVariant(functionData->functions[hash].metaType, &element));
                }
                Java::QtQml::QQmlListProperty$ReplaceFunction::accept(env, replaceFunction, _object, jlong(idx), _element);
            }
        }
    }

    static void removeLast(QQmlListProperty<void> * list){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            QReadLocker locker(CoreAPI::objectDataLock());
            functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject removeLastFunction = functionData->functions[hash].removeLastFunction.object()){
                    Java::Runtime::Consumer::accept(env, removeLastFunction, _object);
                }
            }
        }
    }

    static void slow_replace(QQmlListProperty<void> *list, list_int idx, void *v)
    {
        const list_int length = list->count(list);
        if (idx < 0 || idx >= length)
            return;

        QVector<void *> stash;
        if (list->clear != slow_clear) {
            stash.reserve(length);
            for (list_int i = 0; i < length; ++i)
                stash.append(i == idx ? v : list->at(list, i));
            list->clear(list);
            for (void *item : qAsConst(stash))
                list->append(list, item);
        } else {
            stash.reserve(length - idx - 1);
            for (list_int i = length - 1; i > idx; --i) {
                stash.append(list->at(list, i));
                list->removeLast(list);
            }
            list->removeLast(list);
            list->append(list, v);
            while (!stash.isEmpty())
                list->append(list, stash.takeLast());
        }
    }

    static void slow_removeLast(QQmlListProperty<void> *list)
    {
        const list_int length = list->count(list) - 1;
        if (length < 0)
            return;
        QVector<void *> stash;
        stash.reserve(length);
        for (list_int i = 0; i < length; ++i)
            stash.append(list->at(list, i));
        list->clear(list);
        for (void *item : qAsConst(stash))
            list->append(list, item);
    }
#endif
};

void __qt_create_new_QQmlListProperty_2(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QQmlListProperty<void>(QObject * o, QList<QObject > & list)()")
    QObject* object = QtJambiAPI::convertJavaObjectToQObject(__jni_env, __java_arguments[1].l);
    const QMetaType& metaType = qtjambi_cast<const QMetaType&>(__jni_env, __java_arguments[0].l);
    jint hash = __java_arguments[8].i;
    FunctionUserData *functionData;
    {
        QReadLocker locker(CoreAPI::objectDataLock());
        functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, object);
    }
    if(!functionData){
        QWriteLocker locker(CoreAPI::objectDataLock());
        functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, object);
        if(!functionData)
            QTJAMBI_SET_OBJECTUSERDATA(FunctionUserData, object, functionData = new FunctionUserData());
    }
    if(!functionData->functions.contains(hash)){
        functionData->functions[hash] = FunctionUserData::Functions{
                JObjectWrapper(__jni_env, __java_arguments[2].l),
                JObjectWrapper(__jni_env, __java_arguments[3].l),
                JObjectWrapper(__jni_env, __java_arguments[4].l),
                JObjectWrapper(__jni_env, __java_arguments[5].l),
                JObjectWrapper(__jni_env, __java_arguments[6].l),
                JObjectWrapper(__jni_env, __java_arguments[7].l),
                metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                .id()
#endif
                };
    }
    FunctionUserData::Functions* functions = &functionData->functions[hash];

    QQmlListProperty<void>* listProperty = new(__qtjambi_ptr) QQmlListProperty<void>();
    listProperty->object = object;
    listProperty->data = reinterpret_cast<void*>(qsizetype(hash));
    if(functions->appendFunction.object())
        listProperty->append = FunctionUserData::append;
    if(functions->atFunction.object())
        listProperty->at = FunctionUserData::at;
    if(functions->countFunction.object())
        listProperty->count = FunctionUserData::count;
    if(functions->clearFunction.object()){
        listProperty->clear = FunctionUserData::clear;
    }else if(functions->countFunction.object() && functions->removeLastFunction.object()){
        listProperty->clear = FunctionUserData::slow_clear;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    if(functions->replaceFunction.object()){
        listProperty->replace = FunctionUserData::replace;
    }else if(functions->appendFunction.object()
                && functions->countFunction.object()
                && functions->atFunction.object()
                && (functions->clearFunction.object() || functions->removeLastFunction.object())){
        listProperty->replace = FunctionUserData::slow_replace;
    }
    if(functions->removeLastFunction.object()){
        listProperty->removeLast = FunctionUserData::removeLast;
    }else if(functions->appendFunction.object()
                && functions->countFunction.object()
                && functions->atFunction.object()
                && functions->clearFunction.object()){
        listProperty->removeLast = FunctionUserData::slow_removeLast;
    }
#endif
}

// destruct QQmlListProperty
void __qt_destruct_QQmlListProperty(void* ptr)
{
    QTJAMBI_NATIVE_METHOD_CALL("destruct QQmlListProperty")
    reinterpret_cast<QQmlListProperty<void>*>(ptr)->~QQmlListProperty();
}

// emitting (writeSignalFunction)
// emitting  (functionsInTargetLang writeFinalFunction)
// QQmlListProperty::QQmlListProperty()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_initialize_1native_1plain)
(JNIEnv *__jni_env,
 jclass __jni_class, jobject __jni_object)
{
    QTJAMBI_NATIVE_METHOD_CALL("QQmlListProperty::QQmlListProperty()")
    QTJAMBI_TRY{
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty_0, sizeof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), false, &deleter_QQmlListProperty, nullptr);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::QQmlListProperty(QObject * o, QList<QObject > & list)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_initialize_1native_1list)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject o0,
 jobject list1,
 jboolean readonly)
{
    QTJAMBI_NATIVE_METHOD_CALL("QQmlListProperty::QQmlListProperty(QObject * o, QList<QObject > & list)")
    QTJAMBI_TRY{
        jvalue arguments[3];
        arguments[0].l = o0;
        arguments[1].l = list1;
        arguments[2].z = readonly;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty_1, sizeof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), false, &deleter_QQmlListProperty, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_initialize_1native_1functions)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject metaType,
 jobject o0,
 jobject appendFunction,
 jobject countFunction,
 jobject atFunction,
 jobject clearFunction,
 jobject replaceFunction,
 jobject removeLastFunction,
 jint hash)
{
    QTJAMBI_NATIVE_METHOD_CALL("QQmlListProperty::QQmlListProperty(QObject * o, QList<QObject > & list)")
    QTJAMBI_TRY{
        jvalue arguments[9];
        arguments[0].l = metaType;
        arguments[1].l = o0;
        arguments[2].l = appendFunction;
        arguments[3].l = countFunction;
        arguments[4].l = atFunction;
        arguments[5].l = clearFunction;
        arguments[6].l = replaceFunction;
        arguments[7].l = removeLastFunction;
        arguments[8].i = hash;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty_2, sizeof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), false, &deleter_QQmlListProperty, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_getListElementType)
(JNIEnv *env,
 jclass,
 QtJambiNativeID listNativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(listNativeId);
        QtJambiAPI::checkNullPointer(env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        if(!(containerAccess->elementMetaType().flags() & QMetaType::PointerToQObject)){
            result = qtjambi_cast<jobject>(env, containerAccess->elementMetaType());
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// QQmlListProperty::append(QObject object)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_append)
(JNIEnv *__jni_env,
 jobject _this,
 jobject object0,
 QtJambiNativeID elementType)
{
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::append(QObject object)", __qt_this)
        if(__qt_this->append==FunctionUserData::append){
            jint hash = jint(qint64(__qt_this->data));
            FunctionUserData *functionData{nullptr};
            {
                QReadLocker locker(CoreAPI::objectDataLock());
                functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, __qt_this->object);
            }
            if(functionData && functionData->functions.contains(hash)){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(__jni_env, __qt_this->object);
                if(jobject appendFunction = functionData->functions[hash].appendFunction.object()){
                    Java::Runtime::BiConsumer::accept(__jni_env, appendFunction, _object, object0);
                }
            }
        }else if(__qt_this->append){
            void* obj;
            if(!elementType){
                obj = qtjambi_cast<QObject*>(__jni_env, object0);
            }else{
                const QMetaType& metaType = QtJambiAPI::objectReferenceFromNativeId<const QMetaType>(__jni_env, elementType);
                QVariant variant = QVariant::fromValue(JObjectWrapper(__jni_env, object0));
                if(variant.convert(metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                   .id()
#endif
                                            )){
                    obj = *reinterpret_cast<void**>(variant.data());
                }else{
                    return;
                }
            }
            __qt_this->append(__qt_this, obj);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::replace(long index, QObject object)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_replace)
(JNIEnv *__jni_env,
 jobject _this,
 jlong index,
 jobject object0,
 QtJambiNativeID elementType)
{
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::replace(long index, QObject object)", __qt_this)
        if(__qt_this->replace==FunctionUserData::replace){
            jint hash = jint(qint64(__qt_this->data));
            FunctionUserData *functionData{nullptr};
            {
                QReadLocker locker(CoreAPI::objectDataLock());
                functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, __qt_this->object);
            }
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, __qt_this->object);
                if(jobject replaceFunction = functionData->functions[hash].replaceFunction.object()){
                    Java::QtQml::QQmlListProperty$ReplaceFunction::accept(env, replaceFunction, _object, index, object0);
                }
            }
        }else if(__qt_this->replace){
            void* obj;
            if(!elementType){
                obj = qtjambi_cast<QObject*>(__jni_env, object0);
            }else{
                const QMetaType& metaType = QtJambiAPI::objectReferenceFromNativeId<const QMetaType>(__jni_env, elementType);
                QVariant variant = QVariant::fromValue(JObjectWrapper(__jni_env, object0));
                if(variant.convert(metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                            .id()
#endif
                                            )){
                    obj = *reinterpret_cast<void**>(variant.data());
                }else{
                    return;
                }
            }
            __qt_this->replace(__qt_this, index, obj);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::at(int index)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_at)
(JNIEnv *__jni_env,
 jobject _this,
 jlong index0,
 QtJambiNativeID elementType)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::at(long index)", __qt_this)
        if(__qt_this->at==FunctionUserData::at){
            jint hash = jint(qint64(__qt_this->data));
            FunctionUserData *functionData{nullptr};
            {
                QReadLocker locker(CoreAPI::objectDataLock());
                functionData = QTJAMBI_GET_OBJECTUSERDATA(FunctionUserData, __qt_this->object);
            }
            if(functionData && functionData->functions.contains(hash)){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(__jni_env, __qt_this->object);
                if(jobject atFunction = functionData->functions[hash].atFunction.object()){
                    _result = Java::QtQml::QQmlListProperty$AtFunction::apply(__jni_env, atFunction, _object, index0);
                }
            }
        }else if(__qt_this->at){
            void*  qt_return_value = __qt_this->at(__qt_this, index0);
            if(!elementType){
                _result = qtjambi_cast<jobject>(__jni_env, reinterpret_cast<QObject*>(qt_return_value));
            }else{
                const QMetaType& metaType = QtJambiAPI::objectReferenceFromNativeId<const QMetaType>(__jni_env, elementType);
                _result = QtJambiAPI::convertQVariantToJavaObject(__jni_env, QVariant(metaType
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            .id()
#endif
                                                                            , &qt_return_value));
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canAppend()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canAppend)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::canAppend()", __qt_this)
        _result = __qt_this->append!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canAt()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canAt)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::canAt()", __qt_this)
        _result = __qt_this->at!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canClear()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canClear)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::canClear()", __qt_this)
        _result = __qt_this->clear!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canCount()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canCount)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::canCount()", __qt_this)
        _result = __qt_this->count!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canRemoveLast()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canRemoveLast)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::canRemoveLast()", __qt_this)
        _result = __qt_this->removeLast!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canReplace()
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_canReplace)
(JNIEnv * __jni_env,
 jobject _this)
{
    jboolean _result{false};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::canReplace()", __qt_this)
        _result = __qt_this->replace!=nullptr;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_clear)
(JNIEnv * __jni_env,
 jobject _this)
{
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::clear()", __qt_this)
        if(__qt_this->clear)
            __qt_this->clear(__qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::removeLast()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_removeLast)
(JNIEnv * __jni_env,
 jobject _this)
{
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::removeLast()", __qt_this)
        if(__qt_this->removeLast)
            __qt_this->removeLast(__qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::count()
extern "C" Q_DECL_EXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_count)
(JNIEnv * __jni_env,
 jobject _this)
{
    jlong _result{0};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::count()", __qt_this)
        if(__qt_this->count)
            _result = __qt_this->count(__qt_this);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::object()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QQmlListProperty_object)
(JNIEnv *__jni_env,
 jobject _this)
{
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::object()", __qt_this)
        _result = qtjambi_cast<jobject>(__jni_env, __qt_this->object);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)

void initialize_meta_info_QQmlListProperty(){
    using namespace RegistryAPI;
    if(JniEnvironment env{300}){
        if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enabled-qml-debugging-nowarn"))
            || Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enable-qml-debugging-nowarn"))){
#if QT_VERSION < QT_VERSION_CHECK(6,4,0)
            QQmlDebuggingEnabler(false);
#else
            QQmlDebuggingEnabler::enableDebugging(false);
#endif
        }else if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enabled-qml-debugging"))
                   || Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enable-qml-debugging"))){
#if QT_VERSION < QT_VERSION_CHECK(6,4,0)
            QQmlDebuggingEnabler(true);
#else
            QQmlDebuggingEnabler::enableDebugging(true);
#endif
        }
    }
    QmlAPI::setQmlReportDestruction(&QQmlPrivate::qdeclarativeelement_destructor);
    const std::type_info& typeId = registerUnspecificTypeInfo<QQmlListProperty<void>>("QQmlListProperty", "io/qt/qml/QQmlListProperty");
    registerOperators<QQmlListProperty<void>>();
    registerConstructorInfos(typeId, &__qt_destruct_QQmlListProperty, {ConstructorInfo(&__qt_create_new_QQmlListProperty_0, nullptr),
                                                                       ConstructorInfo(&__qt_create_new_QQmlListProperty_1, "Lio/qt/core/QObject;Lio/qt/core/QList;"),
                                                                       ConstructorInfo(&__qt_create_new_QQmlListProperty_1, "Lio/qt/core/QObject;Lio/qt/core/QList;Z")} );
    registerDeleter(typeId, &deleter_QQmlListProperty);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    int metaTypeID = registerMetaType<QQmlListProperty<QObject>>("QQmlListProperty<QObject>",
                            QtMetaTypePrivate::QMetaTypeFunctionHelper<QQmlListProperty<void>>::Destruct,
                            [](void *where, const void *ptr) -> void *{
                                QQmlListProperty<void>* listProperty = new(where) QQmlListProperty<void>();
                                if (ptr){
                                    const QQmlListProperty<void>* _ptr = reinterpret_cast<const QQmlListProperty<void>*>(ptr);
                                    listProperty->object = _ptr->object;
                                    listProperty->data = _ptr->data;
                                    listProperty->append = _ptr->append;
                                    listProperty->at = _ptr->at;
                                    listProperty->clear = _ptr->clear;
                                    listProperty->count = _ptr->count;
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
                                    listProperty->dummy1 = _ptr->dummy1;
                                    listProperty->dummy2 = _ptr->dummy2;
#else
                                    listProperty->replace = _ptr->replace;
                                    listProperty->removeLast = _ptr->removeLast;
#endif
                                }
                                return listProperty;
                            }
                        );
    QMetaType::registerNormalizedTypedef("QQmlListProperty", metaTypeID);
#else
    int metaTypeID = registerMetaType<QQmlListProperty<QObject>>("QQmlListProperty<QObject>",
                            QtJambiPrivate::QMetaTypeInterfaceFunctions<QQmlListProperty<void>>::defaultCtr,
                            [](const QtPrivate::QMetaTypeInterface *, void *where, const void *ptr){
                                const QQmlListProperty<void>* _ptr = reinterpret_cast<const QQmlListProperty<void>*>(ptr);
                                new(where) QQmlListProperty<void>(*_ptr);
                            },
                            [](const QtPrivate::QMetaTypeInterface *, void *where, void *ptr){
                                QQmlListProperty<void>* _ptr = reinterpret_cast<QQmlListProperty<void>*>(ptr);
                                new(where) QQmlListProperty<void>(std::move(*_ptr));
                            }
                        );
    QMetaType::registerNormalizedTypedef("QQmlListProperty", QMetaType(metaTypeID));
#endif
    registerMetaTypeID(typeId, metaTypeID);
}

hash_type qHash(const QQmlListReference &value, hash_type seed)
{
    QQmlListReferencePrivate* p = QQmlListReferencePrivate::get(const_cast<QQmlListReference*>(&value));
    if(!p){
        return seed;
    }
    QtPrivate::QHashCombine hash;
    seed = hash(seed, quintptr(value.object()));
    seed = hash(seed, quintptr(p->property.data));
    seed = hash(seed, value.canAppend());
    seed = hash(seed, value.canAt());
    seed = hash(seed, value.canClear());
    seed = hash(seed, value.canCount());
    seed = hash(seed, value.canRemoveLast());
    seed = hash(seed, value.canReplace());
    return seed;
}

