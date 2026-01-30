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

#include <QtCore/QtGlobal>
#include <QtCore/private/qobject_p.h>
#include <QtCore/QMutex>
#include <QtQml/private/qqmllist_p.h>
#include <QtQml/private/qqmldata_p.h>
#include <QtQml/private/qjsvalue_p.h>

#include <QtQml/qqmlprivate.h>
#include "pch_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

namespace Java{
namespace QtCore{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QObject$QDeclarativeConstructor,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/QtConstructInPlace;)
                                )
}
namespace QtJambi {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt,QtConstructInPlace,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(J)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(native_id,J)
                                )
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

void createQmlObject(void* placement,void* _metaData){
    if(QmlAPI::CreatorFunctionMetaData* metaData = reinterpret_cast<QmlAPI::CreatorFunctionMetaData*>(_metaData)){
        memset(placement, 0, metaData->objectSize);
        if(JniEnvironmentExceptionInhibitor env{200}){
            void* pointer = nullptr;
            QTJAMBI_TRY{
                switch(metaData->constructorKind){
                case QmlAPI::ConstructorKind::DeclarativeConstructor:
                {
                    pointer = QmlAPI::beginInPlaceConstruction(placement, metaData->meta_object, metaData->constructorFunction);
                    jobject ipc = Java::QtJambi::QtConstructInPlace::newInstance(env, jlong(pointer));
                    jobject dc = Java::QtCore::QObject$QDeclarativeConstructor::newInstance(env, ipc);
                    jobject object = env->NewObject(metaData->clazz, metaData->constructor, dc);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    QmlAPI::endInPlaceConstruction(env, object, pointer);
                    pointer = nullptr;
                    Java::QtJambi::QtConstructInPlace::set_native_id(env, ipc, 0);
                }
                break;
                case QmlAPI::ConstructorKind::InPlaceConstructor:
                {
                    pointer = QmlAPI::beginInPlaceConstruction(placement, metaData->meta_object, metaData->constructorFunction);
                    jobject ipc = Java::QtJambi::QtConstructInPlace::newInstance(env, jlong(pointer));
                    jobject object = env->NewObject(metaData->clazz, metaData->constructor, ipc);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    QmlAPI::endInPlaceConstruction(env, object, pointer);
                    pointer = nullptr;
                    Java::QtJambi::QtConstructInPlace::set_native_id(env, ipc, 0);
                }
                break;
                case QmlAPI::ConstructorKind::PrivateConstructor:
                {
                    pointer = QmlAPI::beginInPlaceConstruction(placement, metaData->meta_object, metaData->constructorFunction);
                    jobject object = env->NewObject(metaData->clazz, metaData->constructor, nullptr);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    QmlAPI::endInPlaceConstruction(env, object, pointer);
                    pointer = nullptr;
                }
                break;
                default:
                    break;
                }
            }QTJAMBI_CATCH(const JavaException& exn){
                if(pointer){
                    QmlAPI::endInPlaceConstruction(env, nullptr, pointer);
                }
                QObject* obj = reinterpret_cast<QObject*>(placement);
                if(QObjectPrivate::get(obj) != nullptr){
                    obj->~QObject();
                }
                obj = QmlAPI::createQmlErrorDummyObject(metaData->meta_object, placement, metaData->vsCast, metaData->viCast);
                if(!obj)
                    obj = new (placement) ErrorDummyObject(metaData->psCast, metaData->vsCast, metaData->viCast);
                obj->deleteLater();
                env.handleException(exn, nullptr);
            }QTJAMBI_TRY_END
        }else{
            QObject* obj = QmlAPI::createQmlErrorDummyObject(metaData->meta_object, placement, metaData->vsCast, metaData->viCast);
            if(!obj)
                obj = new (placement) ErrorDummyObject(metaData->psCast, metaData->vsCast, metaData->viCast);
            obj->deleteLater();
        }
    }
}

typedef QObject*(*CreateParentFunction)(QObject*);
CreateParentFunction createParentFunction(JNIEnv * env, jclass clazz, jmethodID constructor){
    if(!constructor)
        return nullptr;
    size_t hash = 1;
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
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                jobject result = env->CallStaticObjectMethod(clazz, qmlAttachedProperties, QtJambiAPI::convertQObjectToJavaObject(env, parent));
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
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

QMetaType registerQQmlListPropertyAsQmlMetaType(JNIEnv *env, const QString& javaName){
    QByteArray listName = "QQmlListProperty<" + javaName.toLatin1().replace(".", "::").replace("$", "::").replace("/", "::") + '>';
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QQmlListProperty<QObject>>::Flags),
#else
                                /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QQmlListProperty<QObject>>::flags()),
#endif
                              nullptr,
                              nullptr);
}

/**
 * this method is used by the qml wrapper
 */
QMetaType registerQObjectAsQmlMetaType(JNIEnv *env, jclass javaClass, const QString& javaName, const QMetaObject *meta_object)
{
    QByteArray qtName = javaName.toLatin1().replace(".", "::").replace("$", "::").replace("/", "::")+"*";

    QMetaType definedType = QMetaType::fromName(qtName);
    if(definedType.isValid()){
        return definedType;
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                            /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<QObject*>::Flags),
#else
                                            /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<QObject*>::flags()),
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
                                            /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeTypeFlags<void*>::Flags),
#else
                                            /*.flags=*/ QMetaType::TypeFlags(QtPrivate::QMetaTypeForType<void*>::flags()),
#endif
                                            meta_object,
                                            nullptr);
    }
}

jmethodID findConstructor(JNIEnv * env, jclass clazz){
    jmethodID constructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
#ifdef Q_OS_ANDROID
    if(constructor){
        jobject mtd = env->ToReflectedMethod(clazz, constructor, false);
        if(!env->IsSameObject(clazz, Java::Runtime::Constructor::getDeclaringClass(env, mtd))){
            constructor = nullptr;
        }
    }
#endif
    if(constructor){
        jmethodID superClassConstructor = nullptr;
        jclass generatedSuperclass = Java::QtJambi::ClassAnalyzerUtility::findGeneratedSuperclass(env, clazz);
        if(generatedSuperclass){
            superClassConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", generatedSuperclass);
        }else{
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class %1 cannot be registered as Qml type since it does not inherit a Qt class.").arg(QtJambiAPI::getClassName(env, clazz).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
        if(!superClassConstructor){
            QString superClassName = QtJambiAPI::getClassName(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QtJambiAPI::getClassName(env, clazz).replace(QLatin1Char('$'), QLatin1Char('.')), superClassName) QTJAMBI_STACKTRACEINFO );
        }
    }
    return constructor;
}

void findConstructor(JNIEnv * env, jclass clazz, const QMetaObject *meta_object, jmethodID& constructor, QmlAPI::ConstructorKind& constructorKind, QtJambiAPI::ConstructorFn& constructorFunction){
    constructor = nullptr;
    QmlAPI::InPlaceConstructorInfo info = QmlAPI::findInPlaceConstructor(env, clazz, meta_object);
    if(!info.constructor || info.isPrivate){
        constructor = findConstructor(env, clazz);
        if(constructor)
            constructorKind = QmlAPI::ConstructorKind::DeclarativeConstructor;
    }
    if(!constructor){
        if(info.constructor){
            constructor = info.constructor;
            constructorKind = info.isPrivate ? QmlAPI::ConstructorKind::PrivateConstructor : QmlAPI::ConstructorKind::InPlaceConstructor;
            constructorFunction = info.constructorFunction;
        }else{
            constructorKind = QmlAPI::ConstructorKind::NoConstructor;
        }
    }
}

struct QmlTypeRegistractionData{
    QString javaName;
    QMetaType typeId;
    QMetaType listId;
    int objectSize = 0;
    int psCast = 0;
    int vsCast = 0;
    int viCast = 0;
    int fhCast = 0;
    const QMetaObject *meta_object = nullptr;
    jmethodID constructor = 0;
    QmlAPI::ConstructorKind constructorKind = QmlAPI::ConstructorKind::NoConstructor;
    QtJambiAPI::ConstructorFn constructorFunction = nullptr;
    void (*create)(void *,void *) = nullptr;
    QExplicitlySharedDataPointer<QmlAPI::CreatorFunctionMetaData> userdata{};
    QVariant (*createValueType)(const QJSValue &) = nullptr;
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

CreateValueTypeFn getCreateValueType(JNIEnv *env, jclass clazz, CreateValueTypeFn defaultFn = nullptr){
    CreateValueTypeFn result = nullptr;
    if(jmethodID jsvFactory = JavaAPI::resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(QtJambiAPI::getClassName(env, clazz).replace('.', '/'))), clazz, true)){
        clazz = JavaAPI::toGlobalReference(env, clazz);
        QMetaType metaType(CoreAPI::registeredMetaType(env, clazz));
        if(!metaType.isValid() && Java::QtCore::QObject::isAssignableFrom(env, clazz))
            metaType = QMetaType(QMetaType::QObjectStar);
        result = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                    [clazz,jsvFactory,metaType](const QJSValue & arguments) -> QVariant {
                                        if(JniEnvironment env{300}){
                                            jobject args = qtjambi_cast<jobject>(env, arguments);
                                            if(env->ExceptionCheck()){
                                                env->ExceptionDescribe();
                                                env->ExceptionClear();
                                            }
                                            jobject result = env->CallStaticObjectMethod(clazz, jsvFactory, args);
                                            if(env->ExceptionCheck()){
                                                env->ExceptionDescribe();
                                                env->ExceptionClear();
                                            }
                                            QVariant v = QtJambiAPI::convertJavaObjectToQVariant(env, result);
                                            if(metaType.isValid() && v.metaType()!=metaType)
                                                v.convert(metaType);
                                            return v;
                                        }
                                        return QVariant();
                                    }, Java::Runtime::Object::hashCode(env, clazz)
        );
    }
    return result==nullptr ? defaultFn : result;
}

QmlTypeRegistractionData registerQmlType(JNIEnv *env, jclass clazz, const char* qmlName, RegisterOptions skip = {}){
    const QString javaName = QtJambiAPI::getClassName(env, clazz);
    QmlTypeRegistractionData data;
    data.javaName = javaName;
    data.psCast = parserStatusCast(env, clazz);
    data.vsCast = valueSourceCast(env, clazz);
    data.viCast = valueInterceptorCast(env, clazz);
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
    data.fhCast = finalizerHookCast(env, clazz);
#endif
    bool isQObject = Java::QtCore::QObject::isAssignableFrom(env, clazz);
    if(!isQObject){
        if (findQmlAttachedProperties(env, clazz).function) {
            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject, but has attached properties. This won't work.").arg(data.javaName) QTJAMBI_STACKTRACEINFO );
        }
        QMetaType metaType(QmlAPI::registerMetaType(env, clazz, QString(javaName).replace(".", "/")));
        data.typeId = metaType;
        if(!data.typeId.isValid()){
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
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Invalid QML %1 name \"%2\"").arg(QString(javaName).replace("$", "."), typeName) QTJAMBI_STACKTRACEINFO );
                }
            }
        }
        data.listId = QmlAPI::registerMetaType(env, SequentialContainerType::QList, metaType);
        if(!skip.testFlag(RegisterOption::SkipObjectSize)){
            data.objectSize = int(metaType.sizeOf());
        }
        if(!skip.testFlag(RegisterOption::SkipMetaObject)){
            data.meta_object = metaType.metaObject();
        }
        if(!skip.testFlag(RegisterOption::SkipCreator)){
            jmethodID jsvFactory{nullptr};
            jmethodID jsvConstructor{nullptr};
            if(!(metaType.flags() & QMetaType::IsPointer)){
                jsvFactory = JavaAPI::resolveMethod(env, "create", "(Lio/qt/qml/QJSValue;)Ljava/lang/Object;", clazz, true);
                if(!jsvFactory)
                    jsvFactory = JavaAPI::resolveMethod(env, "create", "(Lio/qt/qml/QJSValue;)Lio/qt/core/QVariant;", clazz, true);
                if(!jsvFactory)
                    jsvFactory = JavaAPI::resolveMethod(env, "create", QByteArray("(Lio/qt/qml/QJSValue;)L") + javaName.toUtf8().replace(".", "/") + ";", clazz, true);
                jsvConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/qml/QJSValue;)V", clazz);
            }
            if((!metaType.iface()->copyCtr || !metaType.iface()->defaultCtr) && !jsvConstructor){
                if(!skip.testFlag(RegisterOption::OptionalCreator)){
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is neither a QObject, nor default- and copy-constructible. You should not use it as a QML type.").arg(data.javaName) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                if(jsvFactory){
                    clazz = JavaAPI::toGlobalReference(env, clazz);
                    data.createValueType = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                            [clazz,jsvFactory,metaType](const QJSValue & arguments) -> QVariant {
                                if(JniEnvironment env{300}){
                                    jobject args = qtjambi_cast<jobject>(env, arguments);
                                    if(env->ExceptionCheck()){
                                        env->ExceptionDescribe();
                                        env->ExceptionClear();
                                    }
                                    jobject result = env->CallStaticObjectMethod(clazz, jsvFactory, args);
                                    if(env->ExceptionCheck()){
                                        env->ExceptionDescribe();
                                        env->ExceptionClear();
                                    }
                                    if(!result)
                                        return QVariant(metaType, nullptr);
                                    QVariant v = QtJambiAPI::convertJavaObjectToQVariant(env, result);
                                    if(v.metaType()!=metaType)
                                        v.convert(metaType);
                                    return v;
                                }
                                return QVariant(metaType, nullptr);
                            }, Java::Runtime::Object::hashCode(env, clazz)
                        );
                    QmlAPI::registerMetaTypeConverter(env, QMetaType::fromType<QJSValue>(), Java::QtQml::QJSValue::getClass(env), metaType, clazz, jsvConstructor);
                }else if(jsvConstructor){
                    clazz = JavaAPI::toGlobalReference(env, clazz);
                    data.createValueType = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                            [clazz,jsvConstructor,metaType](const QJSValue & arguments) -> QVariant {
                                if(JniEnvironment env{300}){
                                    jobject args = qtjambi_cast<jobject>(env, arguments);
                                    jobject result = env->NewObject(clazz, jsvConstructor, args);
                                    if(env->ExceptionCheck()){
                                        env->ExceptionDescribe();
                                        env->ExceptionClear();
                                    }
                                    if(!result)
                                        return QVariant(metaType, nullptr);
                                    QVariant v = QtJambiAPI::convertJavaObjectToQVariant(env, result);
                                    if(v.metaType()!=metaType)
                                        v.convert(metaType);
                                    return v;
                                }
                                return QVariant(metaType, nullptr);
                            }, Java::Runtime::Object::hashCode(env, clazz)
                        );
                    QmlAPI::registerMetaTypeConverter(env, QMetaType::fromType<QJSValue>(), Java::QtQml::QJSValue::getClass(env), metaType, clazz, jsvConstructor);
                }
            }
        }
    }else{
        QString typeName = QLatin1String(qmlName);
        if (!typeName.isEmpty()) {
            if(typeName.at(0).isLower()){
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Invalid QML %1 name \"%2\"; type names must begin with an uppercase letter").arg(QString(javaName).replace("$", "."), typeName) QTJAMBI_STACKTRACEINFO );
            }
            int typeNameLen = typeName.length();
            for (int ii = 0; ii < typeNameLen; ++ii) {
                if (!(typeName.at(ii).isLetterOrNumber() || typeName.at(ii) == u'_')) {
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Invalid QML %1 name \"%2\"").arg(QString(javaName).replace("$", "."), typeName) QTJAMBI_STACKTRACEINFO );
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
            findConstructor(env, clazz, data.meta_object, data.constructor, data.constructorKind, data.constructorFunction);
            if(data.constructor){
                data.create = &createQmlObject;
                data.userdata = QmlAPI::creatorFunctionMetaData(env, data.meta_object, clazz, data.constructor, data.constructorKind, data.constructorFunction, data.objectSize, data.psCast, data.vsCast, data.viCast, data.fhCast);
            }else{
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not constructible.").arg(QtJambiAPI::getClassName(env, clazz).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
            }
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
    const auto func = qmlAttachedPropertiesFunction(nullptr, meta_object);
    return qmlAttachedPropertiesObject(const_cast<QObject *>(obj), func, create);
}

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
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
    typedef QMetaSequence::Iterable SequentialIterable;
#else
    typedef QSequentialIterable SequentialIterable;
#endif
    if(!QMetaType::canConvert(metaType, QMetaType::fromType<SequentialIterable>())){
        Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a valid container type.").arg(metaType.name()) QTJAMBI_STACKTRACEINFO );
    }
    QQmlPrivate::RegisterSequentialContainer type = {
        0,
        uri,
        QTypeRevision::fromMajorVersion(versionMajor),
        nullptr,
        metaType,
        QVariant(metaType).value<SequentialIterable>().metaContainer(),
        QTypeRevision::zero()
    };

    return QQmlPrivate::qmlregister(QQmlPrivate::SequentialContainerRegistration, &type);
}

int qtjambi_qmlRegisterAnonymousType(JNIEnv *env, jclass clazz, const char* uri, int versionMajor){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions(SkipCreator | SkipObjectSize));
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
        return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
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
        3,
        /* QMetaType typeId */ QMetaType(data.typeId),
        /* QMetaType listId */ QMetaType(data.listId),
        data.objectSize,
        /*void (*create)(void *,void *)*/ data.create,
        data.userdata.get(),
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
        /*void *userdata*/ data.userdata.get(),
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
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

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
    QQmlPrivate::RegisterInterface qmlInterface = {
        /* int structVersion */ 0,

        /* QMetaType typeId */ QMetaType(registerQObjectAsQmlMetaType(env, clazz, typeName, meta_object)),
        /* QMetaType listId */ QMetaType(registerQQmlListPropertyAsQmlMetaType(env, typeName)),

        /* const char *iid */ iid,

        /* const char *uri */ uri,
        /* int versionMajor */ QTypeRevision::fromVersion(versionMajor, 0)
    };
    return QQmlPrivate::qmlregister(QQmlPrivate::InterfaceRegistration, &qmlInterface);
}

int qtjambi_qmlRegisterSingletonType(JNIEnv *, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QtQml::ValueCallback& callback){
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
    return QQmlPrivate::qmlregister(QQmlPrivate::SingletonRegistration, &api);
}

int qtjambi_qmlRegisterRevision(JNIEnv *env, jclass clazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, RegisterOptions());

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
        /*void *userdata*/ data.userdata.get(),
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
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterType(JNIEnv *env, jclass clazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions());

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
        /*void *userdata*/ data.userdata.get(),
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
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterType(JNIEnv *env, jclass clazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions());

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
        /*void *userdata*/ data.userdata.get(),
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
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterUncreatableType(JNIEnv *env, jclass clazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QString& reason){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions(SkipCreator));
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
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterUncreatableType(JNIEnv *env, jclass clazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QString& reason){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions(SkipCreator));
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
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
namespace QQmlPrivate{
using SingletonConstructionMode = ConstructionMode;
}
#endif

QQmlPrivate::SingletonConstructionMode singletonConstructionMode(JNIEnv *env, jclass type, jclass wrapperType, bool singleton, const QMetaObject* metaObject, jmethodID& method, QmlAPI::ConstructorKind& constructorKind, QtJambiAPI::ConstructorFn& constructorFunction){
    if(!Java::QtCore::QObject::isAssignableFrom(env, type))
        return QQmlPrivate::SingletonConstructionMode::None;
#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)
    if(!env->IsSameObject(type, wrapperType)
            && (method = JavaAPI::resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(QtJambiAPI::getClassName(env, type).replace('.', '/'))), wrapperType, true))){
        constructorKind = QmlAPI::ConstructorKind::NoConstructor;
        return QQmlPrivate::SingletonConstructionMode::FactoryWrapper;
    }
#else
    Q_UNUSED(wrapperType)
#endif //QT_VERSION >= QT_VERSION_CHECK(6,1,0)
    if(singleton){
        if ((method = JavaAPI::resolveMethod(env, "<init>", "()V", type, false))){
            constructorKind = QmlAPI::ConstructorKind::DefaultConstructor;
            return QQmlPrivate::SingletonConstructionMode::Constructor;
        }
    }else{
        findConstructor(env, type, metaObject, method, constructorKind, constructorFunction);
        if (method)
            return QQmlPrivate::SingletonConstructionMode::Constructor;
    }
    if ((method = JavaAPI::resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(QtJambiAPI::getClassName(env, type).replace('.', '/'))), type, true)))
        return QQmlPrivate::SingletonConstructionMode::Factory;
    return QQmlPrivate::SingletonConstructionMode::None;
}

std::function<QObject*(QQmlEngine *, QJSEngine *)> getCreateSingletonFunction(JNIEnv *env, jclass type, jclass wrapperType){
    QmlAPI::ConstructorKind constructorKind = QmlAPI::ConstructorKind::NoConstructor;
    jmethodID method{nullptr};
    QtJambiAPI::ConstructorFn constructorFunction{nullptr};
    jclass factoryType = type;
    switch(singletonConstructionMode(env, type, wrapperType, true, nullptr, method, constructorKind, constructorFunction)){
    case QQmlPrivate::SingletonConstructionMode::Constructor:
        type = JavaAPI::toGlobalReference(env, type);
        return [type, method, constructorKind](QQmlEngine *, QJSEngine *)->QObject*{
            QObject* result{nullptr};
            if(JniEnvironment env{300}){
                switch(constructorKind){
                case QmlAPI::ConstructorKind::DeclarativeConstructor: break;
                case QmlAPI::ConstructorKind::PrivateConstructor: break;
                case QmlAPI::ConstructorKind::InPlaceConstructor: break;
                default:
                    jobject o = env->NewObject(type, method);
                    result = qtjambi_cast<QObject*>(env, o);
                    QtJambiAPI::setCppOwnershipForTopLevelObject(env, result);
                    break;
                }
            }
            return result;
        };
#if QT_VERSION >= QT_VERSION_CHECK(6,1,0)
    case QQmlPrivate::SingletonConstructionMode::FactoryWrapper:
        factoryType = wrapperType;
        Q_FALLTHROUGH();
#endif //QT_VERSION >= QT_VERSION_CHECK(6,1,0)
    case QQmlPrivate::SingletonConstructionMode::Factory:
        factoryType = JavaAPI::toGlobalReference(env, factoryType);
        return [factoryType, method](QQmlEngine * qe, QJSEngine * je)->QObject*{
            QObject* result{nullptr};
            if(JniEnvironment env{300}){
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                jobject o = env->CallStaticObjectMethod(factoryType, method, qtjambi_cast<jobject>(env, qe), qtjambi_cast<jobject>(env, je));
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                result = qtjambi_cast<QObject*>(env, o);
                QtJambiAPI::setCppOwnershipForTopLevelObject(env, result);
            }
            return result;
        };
    default: break;
    }
    return nullptr;
}

void qtjambi_qmlRegisterTypesAndRevisions(JNIEnv *env, jobjectArray types, const char* uri, int versionMajor, QList<int>* qmlTypeIds){
    for(jsize i = 0, length = env->GetArrayLength(types); i < length; ++i){
        jclass type = jclass(env->GetObjectArrayElement(types, i));
        if(jobject sequencialContainerAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlSequencialContainer::getClass(env))){
            const QMetaObject *type_meta_object = CoreAPI::metaObjectForClass(env, type);
            if(!type_meta_object){
                jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, type);
                if(closestClass){
                    const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
                    type_meta_object = CoreAPI::metaObjectForClass(env, type, original_meta_object);
                }
            }
            jclass resolvedClass = type;
            QString containerMetaTypeName;
            QMetaType containerMetaType;
            if(jobject foreignAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlForeign::getClass(env))){
                resolvedClass = Java::QtQml::Util::QmlForeign::value(env, foreignAnnotation);
                jobject metaTypeAnnotation = Java::QtQml::Util::QmlForeign::metaType(env, foreignAnnotation);
                containerMetaTypeName = qtjambi_cast<QString>(env, Java::QtJambi::QtMetaType::name(env, metaTypeAnnotation));
                if(containerMetaTypeName.isEmpty()){
                    containerMetaType = QMetaType(Java::QtJambi::QtMetaType::id(env, metaTypeAnnotation));
                    if(!containerMetaType.isValid()){
                        containerMetaType = QMetaType(qtjambi_cast<QMetaType::Type>(env, Java::QtJambi::QtMetaType::type(env, metaTypeAnnotation)));
                        if(!containerMetaType.isValid() && env->GetArrayLength(Java::Runtime::Class::getTypeParameters(env, resolvedClass))==1){
                            jclass valueClass = Java::QtQml::Util::QmlSequencialContainer::value(env, sequencialContainerAnnotation);
                            QString valueMetaTypeName;
                            QMetaType valueMetaType;
                            jobject valueTypeAnnotation = Java::QtQml::Util::QmlSequencialContainer::valueType(env, sequencialContainerAnnotation);
                            valueMetaTypeName = qtjambi_cast<QString>(env, Java::QtJambi::QtMetaType::name(env, valueTypeAnnotation));
                            if(valueMetaTypeName.isEmpty()){
                                valueMetaType = QMetaType(Java::QtJambi::QtMetaType::id(env, valueTypeAnnotation));
                                if(!valueMetaType.isValid()){
                                    valueMetaType = QMetaType(qtjambi_cast<QMetaType::Type>(env, Java::QtJambi::QtMetaType::type(env, valueTypeAnnotation)));
                                }
                            }
                            if(!valueMetaType.isValid()){
                                if(valueMetaTypeName.isEmpty()){
                                    valueMetaType = QmlAPI::registerMetaType(env, valueClass, QtJambiAPI::getClassName(env, valueClass).replace(".", "/"));
                                }else{
                                    valueMetaType = QMetaType::fromName(valueMetaTypeName.toUtf8());
                                }
                            }
                            if(valueMetaType.isValid()){
                                if(Java::QtCore::QStack::isSameClass(env, resolvedClass)){
                                    containerMetaType = QmlAPI::registerMetaType(env, SequentialContainerType::QStack, valueMetaType);
                                } else if(Java::QtCore::QQueue::isSameClass(env, resolvedClass)){
                                    containerMetaType = QmlAPI::registerMetaType(env, SequentialContainerType::QQueue, valueMetaType);
                                } else if(Java::QtCore::QSet::isAssignableFrom(env, resolvedClass)){
                                    containerMetaType = QmlAPI::registerMetaType(env, SequentialContainerType::QSet, valueMetaType);
                                } else if(Java::QtCore::QList::isAssignableFrom(env, resolvedClass)){
                                    containerMetaType = QmlAPI::registerMetaType(env, SequentialContainerType::QList, valueMetaType);
                                }
                            }
                        }
                    }
                }
            }
            if(!containerMetaType.isValid()){
                if(containerMetaTypeName.isEmpty()){
                    containerMetaType = QmlAPI::registerMetaType(env, resolvedClass, QtJambiAPI::getClassName(env, resolvedClass).replace(".", "/"));
                }else{
                    containerMetaType = QMetaType::fromName(containerMetaTypeName.toUtf8());
                }
            }
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
            typedef QMetaSequence::Iterable SequentialIterable;
#else
            typedef QSequentialIterable SequentialIterable;
#endif
            if(!QMetaType::canConvert(containerMetaType, QMetaType::fromType<SequentialIterable>())){
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a valid container type.").arg(containerMetaType.name()) QTJAMBI_STACKTRACEINFO );
            }
            QQmlPrivate::RegisterSequentialContainerAndRevisions type = {
                0,
                uri,
                QTypeRevision::fromMajorVersion(versionMajor),
                type_meta_object,
                containerMetaType,
                QVariant(containerMetaType).value<SequentialIterable>().metaContainer(),
                qmlTypeIds
            };
            QQmlPrivate::qmlregister(QQmlPrivate::SequentialContainerAndRevisionsRegistration, &type);
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

            QQmlPrivate::RegisterSingletonTypeAndRevisions api = {
                0,
                uri,
                QTypeRevision::fromMajorVersion(versionMajor),
                getCreateSingletonFunction(env, resolvedClass, type),
                resolved_meta_object,
                type_meta_object,
                resolvedClass!=Java::Runtime::Object::getClass(env) ? QmlAPI::registerMetaType(env, resolvedClass, QtJambiAPI::getClassName(env, resolvedClass).replace(".", "/")) : QMetaType(QMetaType::UnknownType),
                createParentFunction(env, extendedClass, extendedConstructor),
                extended_meta_object,
                qmlTypeIds
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
                if(!Java::QtCore::QObject::isAssignableFrom(env, extendedClass)){
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is a QObject, but is extended by non-QObject type %2. This won't work.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.'), QtJambiAPI::getClassName(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }else{
                    extendedConstructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClass);
                }
            }
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

            int objectSize = int(QmlAPI::extendedSizeForClass(env, resolvedClass));
            int psCast = parserStatusCast(env, resolvedClass);
            int vsCast = valueSourceCast(env, resolvedClass);
            int viCast = valueInterceptorCast(env, resolvedClass);
            int fhCast = finalizerHookCast(env, resolvedClass);
            QmlAPI::ConstructorKind constructorKind = QmlAPI::ConstructorKind::NoConstructor;
            jmethodID constructor{nullptr};
            QtJambiAPI::ConstructorFn constructorFunction{nullptr};
            void (*create)(void *, void *) = nullptr;
            QExplicitlySharedDataPointer<QmlAPI::CreatorFunctionMetaData> userdata{};
            switch(singletonConstructionMode(env, resolvedClass, resolvedClass, false, resolved_meta_object, constructor, constructorKind, constructorFunction)){
            case QQmlPrivate::SingletonConstructionMode::Constructor:
                if(constructor){
                    create = &createQmlObject;
                    userdata = QmlAPI::creatorFunctionMetaData(env, resolved_meta_object, resolvedClass, constructor, constructorKind, constructorFunction, objectSize, psCast, vsCast, viCast, fhCast);
                }else{
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not constructible.").arg(QtJambiAPI::getClassName(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }
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
                                                            if(env->ExceptionCheck()){
                                                                env->ExceptionDescribe();
                                                                env->ExceptionClear();
                                                            }
                                                            jobject result = env->CallStaticObjectMethod(extendedClass, jsvFactory, args);
                                                            if(env->ExceptionCheck()){
                                                                env->ExceptionDescribe();
                                                                env->ExceptionClear();
                                                            }
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
                create,
                userdata.get(),
                createValueType,
                uri,
                QTypeRevision::fromMajorVersion(versionMajor),
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
                qmlTypeIds,
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
                fhCast,

                false,
#if QT_VERSION >= QT_VERSION_CHECK(6,4,0)
                QMetaSequence(),
#endif
#endif
            };

            // Initialize the extension so that we can find it by name or ID.
            if(extendedClass){
                QMetaType extendedTypeID(QmlAPI::registerMetaType(env, extendedClass, QtJambiAPI::getClassName(env, extendedClass).replace(".", "/")));
                if(QByteArrayView(extendedTypeID.name())!=extended_meta_object->className()){
                    QMetaType::registerNormalizedTypedef(extended_meta_object->className(), extendedTypeID);
                }
            }

            const int id = qmlregister(QQmlPrivate::TypeAndRevisionsRegistration, &type);
            if (qmlTypeIds)
                qmlTypeIds->append(id);
        }
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_util_QmlTypes_analyzeType
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

extern "C" JNIEXPORT jlong JNICALL Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_create(JNIEnv *, jclass, jboolean flag){
    return jlong(new std::atomic<bool>(flag));
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_destroy(JNIEnv *, jclass, jlong ptr){
    if(ptr) delete reinterpret_cast<std::atomic<bool>*>(ptr);
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlIncubationController_00024WhileFlag_set(JNIEnv *, jclass, jlong ptr, jboolean flag){
    if(ptr) *reinterpret_cast<std::atomic<bool>*>(ptr) = flag;
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
    QtJambiAPI::checkThread(env, __qt_this);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
using ConvertV2 = bool (*)(const QJSValue &value, QMetaType metaType, void *ptr);
template <>
constexpr ConvertV2 qjsvalue_cast<ConvertV2>(const QJSValue &)
{
    return &QJSEngine::convertV2;
}

// QJSEngine::fromScriptValue
jobject qtjambi_fromScriptValue(JNIEnv *env, QJSEngine *__qt_this, jobject type, const QJSValue& value)
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
    QtJambiAPI::checkThread(env, __qt_this);
    QVariant t(targetType, nullptr);
    ConvertV2 convertV2 = qjsvalue_cast<ConvertV2>(QJSValue{});
    if (convertV2(value, targetType, t.data())){
        return QtJambiAPI::convertQVariantToJavaObject(env, t);
    }else{
        t = value.toVariant();
        t.convert(targetType);
        return QtJambiAPI::convertQVariantToJavaObject(env, t);
    }
}

using Create = QJSValue(QJSEngine::*)(QMetaType type, const void *ptr);
template <>
constexpr Create qjsvalue_cast<Create>(const QJSValue &)
{
    return &QJSEngine::create;
}
// QJSEngine::toScriptValue
QJSValue qtjambi_toScriptValue(JNIEnv *env, QJSEngine *__qt_this, const QVariant& variant)
{
    QtJambiAPI::checkThread(env, __qt_this);
    Create create = qjsvalue_cast<Create>(QJSValue{});
    return (__qt_this->*create)(variant.metaType(), variant.data());
}
#else
// QJSEngine::fromScriptValue
jobject qtjambi_fromScriptValue(JNIEnv *env, QJSEngine *__qt_this, jobject type, const QJSValue& value)
{
    QtJambiAPI::checkThread(env, __qt_this);
    const QMetaType& targetType = qtjambi_cast<const QMetaType&>(env, type);
    QVariant t(targetType, nullptr);
    t = value.toVariant();
    t.convert(targetType);
    return QtJambiAPI::convertQVariantToJavaObject(env, t);
}

using Create = QJSValue(QJSEngine::*)(int type, const void *ptr);
template <>
constexpr Create qjsvalue_cast<Create>(const QJSValue &)
{
    return &QJSEngine::create;
}
// QJSEngine::toScriptValue
QJSValue qtjambi_toScriptValue(JNIEnv *env, QJSEngine *__qt_this, const QVariant& variant)
{
    QtJambiAPI::checkThread(env, __qt_this);
    Create create = qjsvalue_cast<Create>(QJSValue{});
    return (__qt_this->*create)(variant.metaType().id(), variant.data());
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
using ConvertManaged = bool(*)(const QJSManagedValue &value, QMetaType type, void *ptr);
#else
using ConvertManaged = bool(*)(const QJSManagedValue &value, int type, void *ptr);
#endif
template <>
constexpr ConvertManaged qjsvalue_cast<ConvertManaged>(const QJSValue &)
{
    return &QJSEngine::convertManaged;
}
// QJSEngine::fromManagedValue
jobject qtjambi_fromManagedValue(JNIEnv *env, QJSEngine *__qt_this, jobject type, const QJSManagedValue& value)
{
    QtJambiAPI::checkThread(env, __qt_this);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    if (convertManaged(value, targetType, t.data())){
#else
    if (convertManaged(value, targetType.id(), t.data())){
#endif
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
QJSManagedValue qtjambi_toManagedValue(JNIEnv *env, QJSEngine *__qt_this, const QVariant& variant)
{
    QtJambiAPI::checkThread(env, __qt_this);
    CreateManaged createManaged = qjsvalue_cast<CreateManaged>(QJSValue{});
    return (__qt_this->*createManaged)(variant.metaType(), variant.data());
}
#endif


extern "C" JNIEXPORT jobject JNICALL Java_io_qt_qml_util_QtJambi_1LibraryUtilities_internalAccess(JNIEnv *env, jclass cls){
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_qml_QQmlListProperty_clone_1native(JNIEnv *__jni_env, jobject _this){
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
void __qt_create_new_QQmlListProperty(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QQmlListProperty<void>()")
    QQmlListProperty<void> *__qt_this = new(__qtjambi_ptr) QQmlListProperty<void>();
    Q_UNUSED(__qt_this)
}

typedef qsizetype list_int;

// new QQmlListProperty(QObject * o, QList<QObject > & list)
void __qt_create_new_QQmlListProperty_QObject_QList_bool(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
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
        QMetaType metaType;
    };
    FunctionUserData() : QtJambiObjectData() {
    }

    QMap<int,Functions> functions;

    static void append(QQmlListProperty<void> * list, void* element){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            auto locker = QtJambiObjectData::readLock();
            functionData = QtJambiObjectData::userData<FunctionUserData>(list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject appendFunction = functionData->functions[hash].appendFunction.object(env)){
                    jobject _element;
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
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
            auto locker = QtJambiObjectData::readLock();
            functionData = QtJambiObjectData::userData<FunctionUserData>(list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject atFunction = functionData->functions[hash].atFunction.object(env)){
                    jobject result = Java::QtQml::QQmlListProperty$AtFunction::apply(env, atFunction, _object, jlong(index));
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
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
            auto locker = QtJambiObjectData::readLock();
            functionData = QtJambiObjectData::userData<FunctionUserData>(list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject countFunction = functionData->functions[hash].countFunction.object(env)){
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
            auto locker = QtJambiObjectData::readLock();
            functionData = QtJambiObjectData::userData<FunctionUserData>(list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject clearFunction = functionData->functions[hash].clearFunction.object(env)){
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

    static void replace(QQmlListProperty<void> * list, list_int idx, void *element){
        jint hash = jint(qint64(list->data));
        FunctionUserData *functionData{nullptr};
        {
            auto locker = QtJambiObjectData::readLock();
            functionData = QtJambiObjectData::userData<FunctionUserData>(list->object);
        }
        if(JniEnvironment env{300}){
            jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
            if(jobject replaceFunction = functionData->functions[hash].replaceFunction.object(env)){
                jobject _element;
                    if(functionData->functions[hash].metaType.flags() & QMetaType::PointerToQObject){
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
            auto locker = QtJambiObjectData::readLock();
            functionData = QtJambiObjectData::userData<FunctionUserData>(list->object);
        }
        if(functionData && functionData->functions.contains(hash)){
            if(JniEnvironment env{300}){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(env, list->object);
                if(jobject removeLastFunction = functionData->functions[hash].removeLastFunction.object(env)){
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
};

void __qt_create_new_QQmlListProperty_functions(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QQmlListProperty<void>(QObject * o, QList<QObject > & list)()")
    QObject* object = QtJambiAPI::convertJavaObjectToQObject(__jni_env, __java_arguments[1].l);
    const QMetaType& metaType = qtjambi_cast<const QMetaType&>(__jni_env, __java_arguments[0].l);
    jint hash = __java_arguments[8].i;
    FunctionUserData *functionData;
    {
        auto locker = QtJambiObjectData::readLock();
        functionData = QtJambiObjectData::userData<FunctionUserData>(object);
    }
    if(!functionData){
        functionData = new FunctionUserData();
        FunctionUserData * other{nullptr};
        {
            auto locker = QtJambiObjectData::writeLock();
            other = QtJambiObjectData::setUserData<FunctionUserData>(object, functionData);
        }
        if(other)
            delete other;
    }
    if(!functionData->functions.contains(hash)){
        functionData->functions[hash] = FunctionUserData::Functions{
                JObjectWrapper(__jni_env, __java_arguments[2].l),
                JObjectWrapper(__jni_env, __java_arguments[3].l),
                JObjectWrapper(__jni_env, __java_arguments[4].l),
                JObjectWrapper(__jni_env, __java_arguments[5].l),
                JObjectWrapper(__jni_env, __java_arguments[6].l),
                JObjectWrapper(__jni_env, __java_arguments[7].l),
                metaType};
    }
    FunctionUserData::Functions* functions = &functionData->functions[hash];

    QQmlListProperty<void>* listProperty = new(__qtjambi_ptr) QQmlListProperty<void>();
    listProperty->object = object;
    listProperty->data = reinterpret_cast<void*>(qsizetype(hash));
    if(functions->appendFunction.object(__jni_env))
        listProperty->append = FunctionUserData::append;
    if(functions->atFunction.object(__jni_env))
        listProperty->at = FunctionUserData::at;
    if(functions->countFunction.object(__jni_env))
        listProperty->count = FunctionUserData::count;
    if(functions->clearFunction.object(__jni_env)){
        listProperty->clear = FunctionUserData::clear;
    }else if(functions->countFunction.object(__jni_env) && functions->removeLastFunction.object(__jni_env)){
        listProperty->clear = FunctionUserData::slow_clear;
    }
    if(functions->replaceFunction.object(__jni_env)){
        listProperty->replace = FunctionUserData::replace;
    }else if(functions->appendFunction.object(__jni_env)
                && functions->countFunction.object(__jni_env)
                && functions->atFunction.object(__jni_env)
                && (functions->clearFunction.object(__jni_env) || functions->removeLastFunction.object(__jni_env))){
        listProperty->replace = FunctionUserData::slow_replace;
    }
    if(functions->removeLastFunction.object(__jni_env)){
        listProperty->removeLast = FunctionUserData::removeLast;
    }else if(functions->appendFunction.object(__jni_env)
                && functions->countFunction.object(__jni_env)
                && functions->atFunction.object(__jni_env)
                && functions->clearFunction.object(__jni_env)){
        listProperty->removeLast = FunctionUserData::slow_removeLast;
    }
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
extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlListProperty_initialize_1native_1plain(JNIEnv *__jni_env, jclass __jni_class, jobject __jni_object){
    QTJAMBI_NATIVE_METHOD_CALL("QQmlListProperty::QQmlListProperty()")
    QTJAMBI_TRY{
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty, sizeof(QQmlListProperty<void>), alignof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), 0, false, &deleter_QQmlListProperty, nullptr, nullptr);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::QQmlListProperty(QObject * o, QList<QObject > & list)
extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlListProperty_initialize_1native_1list
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
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty_QObject_QList_bool, sizeof(QQmlListProperty<void>), alignof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), 0, false, &deleter_QQmlListProperty, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlListProperty_initialize_1native_1functions
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
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_create_new_QQmlListProperty_functions, sizeof(QQmlListProperty<void>), alignof(QQmlListProperty<void>), typeid(QQmlListProperty<void>), 0, false, &deleter_QQmlListProperty, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_qml_QQmlListProperty_getListElementType(JNIEnv *env, jclass, QtJambiNativeID listNativeId){
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
extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlListProperty_append
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
                auto locker = QtJambiObjectData::readLock();
                functionData = QtJambiObjectData::userData<FunctionUserData>(__qt_this->object);
            }
            if(functionData && functionData->functions.contains(hash)){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(__jni_env, __qt_this->object);
                if(jobject appendFunction = functionData->functions[hash].appendFunction.object(__jni_env)){
                    Java::Runtime::BiConsumer::accept(__jni_env, appendFunction, _object, object0);
                }
            }
        }else if(__qt_this->append){
            void* obj;
            if(!elementType){
                obj = qtjambi_cast<QObject*>(__jni_env, object0);
            }else{
                const QMetaType& metaType = QtJambiAPI::objectReferenceFromNativeId<const QMetaType>(__jni_env, elementType);
                if(jclass cls = CoreAPI::getClassForMetaType(__jni_env, metaType)){
                    if(__jni_env->IsInstanceOf(object0, cls)){
                        obj = qtjambi_cast<QObject*>(__jni_env, object0);
                    }else{
                        JavaException::raiseIllegalArgumentException(__jni_env, QStringLiteral("Cannot cast object of type %1 to %2").arg(
                                                                                    object0 ? QtJambiAPI::getObjectClassName(__jni_env, object0) : QStringLiteral("null"),
                                                                                    QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
                        return;
                    }
                }else{
                    QVariant variant = QVariant::fromValue(JObjectWrapper(__jni_env, object0));
                    if(variant.convert(metaType)){
                        obj = *reinterpret_cast<void**>(variant.data());
                    }else{
                        JavaException::raiseIllegalArgumentException(__jni_env, QStringLiteral("Cannot cast object of type %1 to %2").arg(
                                                                                    object0 ? QtJambiAPI::getObjectClassName(__jni_env, object0) : QStringLiteral("null"),
                                                                                    QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
                        return;
                    }
                }
            }
            __qt_this->append(__qt_this, obj);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::replace(long index, QObject object)
extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlListProperty_replace
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
                auto locker = QtJambiObjectData::readLock();
                functionData = QtJambiObjectData::userData<FunctionUserData>(__qt_this->object);
            }
            {
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(__jni_env, __qt_this->object);
                if(jobject replaceFunction = functionData->functions[hash].replaceFunction.object(__jni_env)){
                    Java::QtQml::QQmlListProperty$ReplaceFunction::accept(__jni_env, replaceFunction, _object, index, object0);
                }
            }
        }else if(__qt_this->replace){
            void* obj;
            if(!elementType){
                obj = qtjambi_cast<QObject*>(__jni_env, object0);
            }else{
                const QMetaType& metaType = QtJambiAPI::objectReferenceFromNativeId<const QMetaType>(__jni_env, elementType);
                if(jclass cls = CoreAPI::getClassForMetaType(__jni_env, metaType)){
                    if(__jni_env->IsInstanceOf(object0, cls)){
                        obj = qtjambi_cast<QObject*>(__jni_env, object0);
                    }else{
                        JavaException::raiseIllegalArgumentException(__jni_env, QStringLiteral("Cannot cast object of type %1 to %2").arg(
                                                                                    object0 ? QtJambiAPI::getObjectClassName(__jni_env, object0) : QStringLiteral("null"),
                                                                                    QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
                        return;
                    }
                }else{
                    QVariant variant = QVariant::fromValue(JObjectWrapper(__jni_env, object0));
                    if(variant.convert(metaType)){
                        obj = *reinterpret_cast<void**>(variant.data());
                    }else{
                        JavaException::raiseIllegalArgumentException(__jni_env, QStringLiteral("Cannot cast object of type %1 to %2").arg(
                                                                                    object0 ? QtJambiAPI::getObjectClassName(__jni_env, object0) : QStringLiteral("null"),
                                                                                    QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
                        return;
                    }
                }
            }
            __qt_this->replace(__qt_this, index, obj);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QQmlListProperty::at(int index)
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_qml_QQmlListProperty_at(JNIEnv *__jni_env, jobject _this, jlong index0, QtJambiNativeID elementType){
    jobject _result{nullptr};
    QTJAMBI_TRY{
        QQmlListProperty<void> *__qt_this = QtJambiAPI::convertJavaObjectToNative<QQmlListProperty<void> >(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QQmlListProperty::at(long index)", __qt_this)
        if(__qt_this->at==FunctionUserData::at){
            jint hash = jint(qint64(__qt_this->data));
            FunctionUserData *functionData{nullptr};
            {
                auto locker = QtJambiObjectData::readLock();
                functionData = QtJambiObjectData::userData<FunctionUserData>(__qt_this->object);
            }
            if(functionData && functionData->functions.contains(hash)){
                jobject _object = QtJambiAPI::convertQObjectToJavaObject(__jni_env, __qt_this->object);
                if(jobject atFunction = functionData->functions[hash].atFunction.object(__jni_env)){
                    _result = Java::QtQml::QQmlListProperty$AtFunction::apply(__jni_env, atFunction, _object, index0);
                }
            }
        }else if(__qt_this->at){
            void*  qt_return_value = __qt_this->at(__qt_this, index0);
            if(!elementType){
                _result = qtjambi_cast<jobject>(__jni_env, reinterpret_cast<QObject*>(qt_return_value));
            }else{
                const QMetaType& metaType = QtJambiAPI::objectReferenceFromNativeId<const QMetaType>(__jni_env, elementType);
                _result = QtJambiAPI::convertQVariantToJavaObject(__jni_env, QVariant(metaType, &qt_return_value));
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return _result;
}

// QQmlListProperty::canAppend()
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_qml_QQmlListProperty_canAppend(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_qml_QQmlListProperty_canAt(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_qml_QQmlListProperty_canClear(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_qml_QQmlListProperty_canCount(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_qml_QQmlListProperty_canRemoveLast(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_qml_QQmlListProperty_canReplace(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlListProperty_clear(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlListProperty_removeLast(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT jlong JNICALL Java_io_qt_qml_QQmlListProperty_count(JNIEnv * __jni_env, jobject _this){
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
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_qml_QQmlListProperty_object(JNIEnv *__jni_env, jobject _this){
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

extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QtJambi_1LibraryUtilities_initializeQmlDebugging(JNIEnv *env, jclass, jboolean enabledQmlDebuggingNowarn, jboolean enabledQmlDebugging){
    QTJAMBI_TRY{
        if(enabledQmlDebuggingNowarn){
#if QT_VERSION < QT_VERSION_CHECK(6,4,0)
            QQmlDebuggingEnabler(false);
#else
            QQmlDebuggingEnabler::enableDebugging(false);
#endif
        }else if(enabledQmlDebugging){
#if QT_VERSION < QT_VERSION_CHECK(6,4,0)
            QQmlDebuggingEnabler(true);
#else
            QQmlDebuggingEnabler::enableDebugging(true);
#endif
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

void initialize_meta_info_QQmlListProperty(){
    using namespace RegistryAPI;
    QmlAPI::setQmlReportDestruction(&QQmlPrivate::qdeclarativeelement_destructor);
    QmlAPI::setGetQmlOwnership([](QObject * obj)->QmlAPI::ObjectOwnerships{
        QmlAPI::ObjectOwnerships result;
        if(QQmlData *ddata = QQmlData::get(obj, false)){
            result.setFlag(QmlAPI::ExplicitSet, ddata->explicitIndestructibleSet);
            result.setFlag(QmlAPI::JavaScriptOwnership, !ddata->indestructible);
        }
        return result;
    });
    const std::type_info& typeId = registerUnspecificTypeInfo<QQmlListProperty<void>>("QQmlListProperty", "io/qt/qml/QQmlListProperty");
    registerOperators<QQmlListProperty<void>>();
    registerConstructorInfos(typeId, 0, &__qt_destruct_QQmlListProperty, {});
    registerDeleter(typeId, &deleter_QQmlListProperty);
    QMetaType metaType = registerMetaType<QQmlListProperty<QObject>>("QQmlListProperty<QObject>",
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
    QMetaType::registerNormalizedTypedef("QQmlListProperty", metaType);
    registerMetaType(typeId, metaType);
}

size_t qHash(const QQmlListReference &value, size_t seed)
{
    QQmlListReferencePrivate* p = QQmlListReferencePrivate::get(const_cast<QQmlListReference*>(&value));
    if(!p){
        return seed;
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
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

QObject* getPointerOwner(const QJSValue* jsValue){
    if(jsValue){
        if(QV4::ExecutionEngine * engine = QJSValuePrivate::engine(jsValue)){
            return engine->jsEngine();
        }
    }
    return nullptr;
}


extern "C" JNIEXPORT void JNICALL Java_io_qt_qml_QQmlInfo_debugStream(JNIEnv *env, jobject, QtJambiNativeID debugId, QtJambiNativeID metaTypeId, jobject value){
    QQmlInfo& debug = QtJambiAPI::objectReferenceFromNativeId<QQmlInfo>(env, debugId);
    const QMetaType& metaType = QtJambiAPI::valueReferenceFromNativeId<QMetaType>(metaTypeId);
    QTJAMBI_TRY{
        bool success = false;
        if(metaType==QMetaType::fromType<JObjectWrapper>()
            || metaType==QMetaType::fromType<JCollectionWrapper>()
            || metaType==QMetaType::fromType<JMapWrapper>()
            || metaType==QMetaType::fromType<JIntArrayWrapper>()
            || metaType==QMetaType::fromType<JByteArrayWrapper>()
            || metaType==QMetaType::fromType<JShortArrayWrapper>()
            || metaType==QMetaType::fromType<JLongArrayWrapper>()
            || metaType==QMetaType::fromType<JDoubleArrayWrapper>()
            || metaType==QMetaType::fromType<JFloatArrayWrapper>()
            || metaType==QMetaType::fromType<JCharArrayWrapper>()
            || metaType==QMetaType::fromType<JBooleanArrayWrapper>()
            || metaType==QMetaType::fromType<JObjectArrayWrapper>()){
            operator<<(debug, JObjectWrapper(env, value));
            success = true;
        }
        if(!success && metaType.hasRegisteredDebugStreamOperator()){
            QVariant variant = qtjambi_cast<QVariant>(env, value);
            if(variant.metaType()==metaType || variant.convert(metaType)){
                success = QMetaType(metaType).debugStream(debug, variant.constData());
            }
        }
        if(!success){
            if(value){
                QtJambiScope scope;
                debug << qtjambi_cast<const char*>(env, scope, Java::Runtime::Object::toString(env, value));
            }else{
                debug << "null";
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

