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
#include <QtQml>

#include <QtQml/qqmlprivate.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cast.h>
#include <qtjambi/qtjambi_qml.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_application.h>
#include <qtjambi/qtjambi_containeraccess.h>
#include "qtjambi_qml_repository.h"
#include "qmlcreatorfunction.h"

class QQmlParserStatus;
class QQmlPropertyValueSource;
class QQmlPropertyValueInterceptor;

typedef QObject*(*CreateParentFunction)(QObject*);
CreateParentFunction createParentFunction(JNIEnv * env, jclass clazz, jmethodID constructor){
    if(!constructor)
        return nullptr;
    hash_type hash = 1;
    hash = 31 * hash + qHash(qint64(constructor));
    hash = 31 * hash + uint(qtjambi_java_object_hashcode(env, clazz));
    clazz = qtjambi_to_global_reference(env, clazz);
    return qtjambi_function_pointer<16,QObject*(QObject*)>([clazz, constructor](QObject* parent) -> QObject* {
        if(JNIEnv * env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            jobject result = env->NewObject(clazz, constructor, qtjambi_from_QObject(env, parent));
            qtjambi_throw_java_exception(env);
            return qtjambi_to_qobject(env, result);
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
            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QString("Attached property method %1 needs to be static.").arg(qtjambi_to_qstring(env, method)) QTJAMBI_STACKTRACEINFO );
        }
        jclass attachedClass;
        if(attachedAnnotation){
            attachedClass = Java::QtQml::Util::QmlAttached::value(env, attachedAnnotation);
        }else{
            attachedClass = Java::Runtime::Method::getReturnType(env, method);
        }
        const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, attachedClass);
        if(!meta_object){
            jclass closestClass = resolveClosestQtSuperclass(env, attachedClass);
            if(closestClass){
                const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                meta_object = qtjambi_metaobject_for_class(env, attachedClass, original_meta_object);
            }
        }
        jmethodID qmlAttachedProperties = env->FromReflectedMethod(method);
        clazz = qtjambi_to_global_reference(env, clazz);
        QQmlAttachedPropertiesFunc function = qtjambi_function_pointer<16,QObject*(QObject*)>([clazz, qmlAttachedProperties](QObject* parent) -> QObject* {
            if(JNIEnv * env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject result = env->CallStaticObjectMethod(clazz, qmlAttachedProperties, qtjambi_from_QObject(env, parent));
                qtjambi_throw_java_exception(env);
                return qtjambi_to_qobject(env, result);
            }
            return nullptr;
        }, qtjambi_java_object_hashcode(env, clazz));
        return AttachedPropertiesInfo{function, meta_object};
    }else if(attachedAnnotation){
        jclass attachedClass = Java::QtQml::Util::QmlAttached::value(env, attachedAnnotation);
        Java::QtQml::QmlTypeRegistrationException::throwNew(env, QString("Class %1 is missing method 'static %2 qmlAttachedProperties(QObject parent)'").arg(qtjambi_class_name(env, clazz), qtjambi_class_name(env, attachedClass)) QTJAMBI_STACKTRACEINFO );
    }
    return AttachedPropertiesInfo{nullptr, nullptr};
}

int parserStatusCast(JNIEnv * env, jclass clazz){
    return qtjambi_interface_offset(env, clazz, typeid(QQmlParserStatus));
}

int valueSourceCast(JNIEnv * env, jclass clazz){
    return qtjambi_interface_offset(env, clazz, typeid(QQmlPropertyValueSource));
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
    return registerQmlMetaType(env, Java::QtQml::QQmlListProperty::getClass(env),
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
    registerJavaClassForCustomMetaType(env, typeId, javaClass);
    return typeId;
#else
    QMetaType definedType = QMetaType::fromName(qtName);
    if(definedType.id()!=QMetaType::UnknownType){
        return definedType.id();
    }
    if(Java::QtCore::QObject::isAssignableFrom(env, javaClass)){
        return registerQmlMetaType(env, javaClass, qtName,
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
        return registerQmlMetaType(env, javaClass, qtName,
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
    jmethodID constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
    jmethodID superClassConstructor = nullptr;
    jclass generatedSuperclass = qtjambi_find_generated_superclass(env, clazz);
    if(generatedSuperclass){
        superClassConstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", generatedSuperclass);
    }else{
        Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class %1 cannot be registered as Qml type since it does not inherit a Qt class.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
    }
    if(!constructor){
        if(superClassConstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QDeclarativeConstructor) to register as Qml type.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }else{
            QString superClassName = qtjambi_class_name(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')), superClassName) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if(!superClassConstructor){
            QString superClassName = qtjambi_class_name(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
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
    if(jmethodID jsvFactory = resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(qtjambi_class_name(env, clazz).replace('.', '/'))), clazz, true)){
        clazz = qtjambi_to_global_reference(env, clazz);
        QMetaType typeId(qtjambiMetaTypeId(env, clazz));
        if(!typeId.isValid() && Java::QtCore::QObject::isAssignableFrom(env, clazz))
            typeId = QMetaType(QMetaType::QObjectStar);
        result = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                    [clazz,jsvFactory,typeId](const QJSValue & arguments) -> QVariant {
                                        if(JNIEnv* env = qtjambi_current_environment()){
                                            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
                                            jobject args = qtjambi_cast<jobject>(env, arguments);
                                            jobject result = env->CallStaticObjectMethod(clazz, jsvFactory, args);
                                            QVariant v = qtjambi_to_qvariant(env, result);
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
    data.javaName = qtjambi_class_name(env, clazz);
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
        QMetaType typeId(qtjambi_register_metatype(env, clazz, data.javaName.replace(".", "/")));
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
        data.listId = qtjambi_register_container_metatype(env, ContainerType::QList, typeId);
        if(!skip.testFlag(RegisterOption::SkipObjectSize)){
            data.objectSize = int(typeId.sizeOf());
        }
        if(!skip.testFlag(RegisterOption::SkipMetaObject)){
            data.meta_object = typeId.metaObject();
        }
        if(!skip.testFlag(RegisterOption::SkipCreator)){
            jmethodID jsvConstructor(nullptr);
            if(!(typeId.flags() & QMetaType::IsPointer)){
                jsvConstructor = resolveMethod(env, "<init>", "(Lio/qt/qml/QJSValue;)V", clazz);
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
                    clazz = qtjambi_to_global_reference(env, clazz);
                    data.createValueType = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                                [clazz,jsvConstructor,typeId](const QJSValue & arguments) -> QVariant {
                                                    if(JNIEnv* env = qtjambi_current_environment()){
                                                        QTJAMBI_JNI_LOCAL_FRAME(env, 300)
                                                        jobject args = qtjambi_cast<jobject>(env, arguments);
                                                        jobject result = env->NewObject(clazz, jsvConstructor, args);
                                                        QVariant v = qtjambi_to_qvariant(env, result);
                                                        if(v.metaType()!=typeId)
                                                            v.convert(typeId);
                                                        return v;
                                                    }
                                                    return QVariant();
                                                }, Java::Runtime::Object::hashCode(env, clazz)
                    );
                    qtjambi_register_metatype_converter(env, QMetaType::fromType<QJSValue>(), Java::QtQml::QJSValue::getClass(env), typeId, clazz, jsvConstructor);
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
            data.objectSize = int(qtjambi_extended_size_for_class(env, clazz));
        }
        if(!skip.testFlag(RegisterOption::SkipMetaObject)){
            data.meta_object = qtjambi_metaobject_for_class(env, clazz);
            if(!data.meta_object){
                jclass closestClass = resolveClosestQtSuperclass(env, clazz);
                if(closestClass){
                    const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                    data.meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
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
    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
        /* int structVersion */ 1,

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
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID extendedConstructor = nullptr;
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        extendedConstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!extendedConstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterExtendedType(JNIEnv *env, jclass clazz, jclass extendedClazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions());
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID econstructor = nullptr;
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterExtendedUncreatableType(JNIEnv *env, jclass clazz, jclass extendedClazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName, QString reason){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions(SkipCreator));
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID econstructor = nullptr;
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

int qtjambi_qmlRegisterExtendedUncreatableType(JNIEnv *env, jclass clazz, jclass extendedClazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName, QString reason){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, RegisterOptions(SkipCreator));
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID econstructor = nullptr;
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
int qtjambi_qmlRegisterInterface(JNIEnv *env, jclass clazz, const char* uri){
    QString typeName = qtjambi_class_name(env, clazz);
    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
        }
    }
    const char* iid = getInterfaceIID(env, clazz);
    if(!iid){
        iid = registerInterfaceID(env, clazz);
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
    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
        }
    }
    QString typeName = qtjambi_class_name(env, clazz);
    const char* iid = getInterfaceIID(env, clazz);
    if(!iid){
        iid = registerInterfaceID(env, clazz);
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
    QString javaName = qtjambi_class_name(env, clazz);

    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
#if QT_VERSION >= QT_VERSION_CHECK(6,3,0)
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
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::TypeRegistration, &type);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)

QQmlPrivate::ConstructionMode constructionMode(JNIEnv *env, jclass type, jclass wrapperType, bool singleton, jmethodID& method){
    if(!Java::QtCore::QObject::isAssignableFrom(env, type))
        return QQmlPrivate::ConstructionMode::None;
    if(!env->IsSameObject(type, wrapperType)
            && (method = resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(qtjambi_class_name(env, type).replace('.', '/'))), wrapperType, true))){
        return QQmlPrivate::ConstructionMode::FactoryWrapper;
    }
    if(singleton){
        if ((method = resolveMethod(env, "<init>", "()V", type, false)))
            return QQmlPrivate::ConstructionMode::Constructor;
    }else{
        if ((method = findConstructor(env, type, qtjambi_class_name(env, type).replace('.', '/'))))
            return QQmlPrivate::ConstructionMode::Constructor;
    }
    if ((method = resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(qtjambi_class_name(env, type).replace('.', '/'))), type, true)))
        return QQmlPrivate::ConstructionMode::Factory;
    return QQmlPrivate::ConstructionMode::None;
}

std::function<QObject*(QQmlEngine *, QJSEngine *)> getCreateSingletonFunction(JNIEnv *env, jclass type, jclass wrapperType){
    jmethodID method{nullptr};
    jclass factoryType = type;
    switch(constructionMode(env, type, wrapperType, true, method)){
    case QQmlPrivate::ConstructionMode::Constructor:
        type = qtjambi_to_global_reference(env, type);
        return [type, method](QQmlEngine *, QJSEngine *)->QObject*{
            QObject* result{nullptr};
            if(JNIEnv* env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 300)
                jobject o = env->NewObject(type, method);
                result = qtjambi_cast<QObject*>(env, o);
                qtjambi_set_cpp_ownership_for_toplevel_object(env, result);
            }
            return result;
        };
    case QQmlPrivate::ConstructionMode::FactoryWrapper:
        factoryType = wrapperType;
        Q_FALLTHROUGH();
    case QQmlPrivate::ConstructionMode::Factory:
        factoryType = qtjambi_to_global_reference(env, factoryType);
        return [factoryType, method](QQmlEngine * qe, QJSEngine * je)->QObject*{
            QObject* result{nullptr};
            if(JNIEnv* env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 300)
                jobject o = env->CallStaticObjectMethod(factoryType, method, qtjambi_cast<jobject>(env, qe), qtjambi_cast<jobject>(env, je));
                result = qtjambi_cast<QObject*>(env, o);
                qtjambi_set_cpp_ownership_for_toplevel_object(env, result);
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
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID econstructor(nullptr);
    if(Java::QtCore::QObject::isAssignableFrom(env, extendedClazz)){
        econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
        if(!econstructor){
            Java::Runtime::IllegalAccessException::throwNew(env, QStringLiteral("Class must offer the econstructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }

    const QMetaObject *extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, nullptr);
    if(!extended_meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, extendedClazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            extended_meta_object = qtjambi_metaobject_for_class(env, extendedClazz, original_meta_object);
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
            Java::QtQml::QmlTypeRegistrationException::throwNew(env, "Qml sequential containers are not supported in Qt5." QTJAMBI_STACKTRACEINFO );
#else
            const QMetaObject *type_meta_object = qtjambi_metaobject_for_class(env, type);
            if(!type_meta_object){
                jclass closestClass = resolveClosestQtSuperclass(env, type);
                if(closestClass){
                    const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                    type_meta_object = qtjambi_metaobject_for_class(env, type, original_meta_object);
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
                                valueMetaType = QMetaType(qtjambi_register_metatype(env, valueClass, qtjambi_class_name(env, valueClass).replace(".", "/")));
                            }else{
                                valueMetaType = QMetaType::fromName(valueMetaTypeName.toUtf8());
                            }
                            if(valueMetaType.isValid()){
                                if(Java::QtCore::QStack::isSameClass(env, resolvedClass)){
                                    resolvedMetaTypeId = qtjambi_register_container_metatype(env, ContainerType::QStack, valueMetaType);
                                } else if(Java::QtCore::QQueue::isSameClass(env, resolvedClass)){
                                    resolvedMetaTypeId = qtjambi_register_container_metatype(env, ContainerType::QQueue, valueMetaType);
                                } else if(Java::QtCore::QSet::isAssignableFrom(env, resolvedClass)){
                                    resolvedMetaTypeId = qtjambi_register_container_metatype(env, ContainerType::QSet, valueMetaType);
                                } else if(Java::QtCore::QList::isAssignableFrom(env, resolvedClass)){
                                    resolvedMetaTypeId = qtjambi_register_container_metatype(env, ContainerType::QList, valueMetaType);
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
                containerMetaType = QMetaType(qtjambi_register_metatype(env, resolvedClass, qtjambi_class_name(env, resolvedClass).replace(".", "/")));
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
            jclass resolvedClass = type;
            jclass extendedClass = nullptr;
            if(jobject unavailableAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlUnavailable::getClass(env))){
                resolvedClass = Java::Runtime::Object::getClass(env);
            }else if(jobject foreignAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlForeign::getClass(env))){
                resolvedClass = Java::QtQml::Util::QmlForeign::value(env, foreignAnnotation);
            }
            if(jobject extendedAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlExtended::getClass(env))){
                extendedClass = Java::QtQml::Util::QmlExtended::value(env, extendedAnnotation);
            }
            if(!Java::QtCore::QObject::isAssignableFrom(env, resolvedClass))
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Singleton type %1 is not subtyping QObject.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );

            const QMetaObject *type_meta_object = qtjambi_metaobject_for_class(env, type);
            if(!type_meta_object){
                jclass closestClass = resolveClosestQtSuperclass(env, type);
                if(closestClass){
                    const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                    type_meta_object = qtjambi_metaobject_for_class(env, type, original_meta_object);
                }
            }
            const QMetaObject *resolved_meta_object = nullptr;
            if(resolvedClass==type){
                resolved_meta_object = type_meta_object;
            }else if(resolvedClass!=Java::Runtime::Object::getClass(env)){
                resolved_meta_object = qtjambi_metaobject_for_class(env, resolvedClass);
                if(!resolved_meta_object){
                    jclass closestClass = resolveClosestQtSuperclass(env, resolvedClass);
                    if(closestClass){
                        const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                        resolved_meta_object = qtjambi_metaobject_for_class(env, resolvedClass, original_meta_object);
                    }
                }
            }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            jmethodID constr = resolveMethod(env, "<init>", "()V", resolvedClass, false);
            resolvedClass = qtjambi_to_global_reference(env, resolvedClass);
            if(extendedClass)
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Singleton %1 is extended by type %2. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.'), qtjambi_class_name(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
#else
            const QMetaObject *extended_meta_object = nullptr;
            jmethodID extendedConstructor = nullptr;
            if(extendedClass){
                if(Java::QtCore::QObject::isAssignableFrom(env, extendedClass))
                    extendedConstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClass);
                else
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is a QObject, but is extended by non-QObject type %2. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.'), qtjambi_class_name(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                qtjambi_metaobject_for_class(env, extendedClass);
                if(!extended_meta_object){
                    jclass closestClass = resolveClosestQtSuperclass(env, extendedClass);
                    if(closestClass){
                        const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                        extended_meta_object = qtjambi_metaobject_for_class(env, extendedClass, original_meta_object);
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
                /* int typeId */ registerQObjectAsQmlMetaType(env, resolvedClass, qtjambi_class_name(env, resolvedClass).replace(".", "/"), resolved_meta_object),
                [resolvedClass, constr](QQmlEngine *, QJSEngine *)->QObject*{
                            QObject* result{nullptr};
                            if(JNIEnv* env = qtjambi_current_environment()){
                                QTJAMBI_JNI_LOCAL_FRAME(env, 300)
                                jobject o = env->NewObject(resolvedClass, constr);
                                result = qtjambi_cast<QObject*>(env, o);
                                qtjambi_set_cpp_ownership_for_toplevel_object(env, result);
                            }
                            return result;
                        },
#else
                QMetaType(resolvedClass!=Java::Runtime::Object::getClass(env) ? qtjambi_register_metatype(env, resolvedClass, qtjambi_class_name(env, resolvedClass).replace(".", "/")) : QMetaType::UnknownType),
                createParentFunction(env, extendedClass, extendedConstructor),
                extended_meta_object,
                qmlTypeIds
#endif
            };
            const int id = QQmlPrivate::qmlregister(QQmlPrivate::SingletonAndRevisionsRegistration, &api);
            if (qmlTypeIds)
                qmlTypeIds->append(id);
        }else if(jobject interfaceAnnotation = Java::Runtime::Class::isInterface(env, type) ? Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlInterface::getClass(env)) : nullptr){
            const int id = qtjambi_qmlRegisterInterface(env, type, uri, versionMajor);
            if (qmlTypeIds)
                qmlTypeIds->append(id);
        }else{
            jclass resolvedClass = type;
            QString resolvedMetaTypeName;
            int resolvedMetaTypeId = 0;
            jclass extendedClass = nullptr;
            if(jobject unavailableAnnotation = Java::Runtime::Class::getAnnotation(env, type, Java::QtQml::Util::QmlUnavailable::getClass(env))){
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

            const QMetaObject *type_meta_object = qtjambi_metaobject_for_class(env, type);
            if(!type_meta_object){
                jclass closestClass = resolveClosestQtSuperclass(env, type);
                if(closestClass){
                    const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                    type_meta_object = qtjambi_metaobject_for_class(env, type, original_meta_object);
                }
            }
            const QMetaObject *resolved_meta_object = nullptr;
            if(resolvedClass==type){
                resolved_meta_object = type_meta_object;
            }else if(resolvedClass!=Java::Runtime::Object::getClass(env)){
                resolved_meta_object = qtjambi_metaobject_for_class(env, resolvedClass);
                if(!resolved_meta_object){
                    jclass closestClass = resolveClosestQtSuperclass(env, resolvedClass);
                    if(closestClass){
                        const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                        resolved_meta_object = qtjambi_metaobject_for_class(env, resolvedClass, original_meta_object);
                    }
                }
            }
            bool isQObject = Java::QtCore::QObject::isAssignableFrom(env, resolvedClass);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            if(!isQObject)
                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject but used as Qml type. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
#endif
            jmethodID extendedConstructor = nullptr;
            const QMetaObject *extended_meta_object = nullptr;
            if(extendedClass){
                extended_meta_object = qtjambi_metaobject_for_class(env, extendedClass);
                if(!extended_meta_object){
                    jclass closestClass = resolveClosestQtSuperclass(env, extendedClass);
                    if(closestClass){
                        const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                        extended_meta_object = qtjambi_metaobject_for_class(env, extendedClass, original_meta_object);
                    }
                }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                if(!isQObject){
                    if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, resolvedClass)){
                        if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, extendedClass)){
                            if(Java::QtCore::QObject::isAssignableFrom(env, extendedClass)){
                                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject, but is extended by QObject subtype %2. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.'), qtjambi_class_name(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                            }else if(!resolveMethod(env, "<init>", "(Lio/qt/QtObject$QPrivateConstructor;)V", extendedClass)){
                                QString className = qtjambi_class_name(env, extendedClass).replace('$', '.');
                                Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is missing %2(QPrivateConstructor) constructor.").arg(className, className.mid(className.lastIndexOf('.')+1)) QTJAMBI_STACKTRACEINFO );
                            }
                        }else{
                            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is native value type, but is extended by pure java type %2. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.'), qtjambi_class_name(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        if(Java::QtCore::QObject::isAssignableFrom(env, extendedClass)){
                            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject, but is extended by QObject subtype %2. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.'), qtjambi_class_name(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }else if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, extendedClass)){
                            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is pure java type, but is extended by native Qt type %2. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.'), qtjambi_class_name(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                        }
                        if(extended_meta_object && !qtjambi_register_qml_extension(env, extended_meta_object, resolvedClass))
                            Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("Cannot use native type %1 as qml extension.").arg(extended_meta_object->className()) QTJAMBI_STACKTRACEINFO );
                    }
                }else
#endif
                if(!Java::QtCore::QObject::isAssignableFrom(env, extendedClass)){
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is a QObject, but is extended by non-QObject type %2. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.'), qtjambi_class_name(env, extendedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }else{
                    extendedConstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClass);
                }
            }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QString javaName = qtjambi_class_name(env, resolvedClass).replace(".", "/");
            int typeId = registerQObjectAsQmlMetaType(env, resolvedClass, javaName, resolved_meta_object);
            int listId = registerQQmlListPropertyAsQmlMetaType(env, javaName);
#else
            QMetaType typeId;
            QMetaType listId;
            if(!isQObject){
                if(resolvedMetaTypeId!=0){
                    typeId = QMetaType(resolvedMetaTypeId);
                }else if(resolvedMetaTypeName.isEmpty()){
                    typeId = QMetaType(qtjambi_register_metatype(env, resolvedClass, qtjambi_class_name(env, resolvedClass).replace(".", "/")));
                }else{
                    typeId = QMetaType::fromName(resolvedMetaTypeName.toUtf8());
                }
                if(!typeId.iface()->defaultCtr || !typeId.iface()->copyCtr)
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is neither a QObject, nor default- and copy-constructible. You should not use it as a QML type.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                listId = QMetaType(qtjambi_register_container_metatype(env, ContainerType::QList, typeId));
                if(Java::Runtime::Class::getAnnotation(env, resolvedClass, Java::QtQml::Util::QmlAttached::getClass(env))
                        || findQmlAttachedProperties(env, resolvedClass).function){
                    Java::QtQml::QmlTypeRegistrationException::throwNew(env, QStringLiteral("%1 is not a QObject, but has attached properties. This won't work.").arg(qtjambi_class_name(env, resolvedClass).replace('$', '.')) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                QString javaName = qtjambi_class_name(env, resolvedClass).replace(".", "/");
                typeId = QMetaType(registerQObjectAsQmlMetaType(env, resolvedClass, javaName, resolved_meta_object));
                listId = QMetaType(registerQQmlListPropertyAsQmlMetaType(env, javaName));
            }
#endif

            int objectSize = int(qtjambi_extended_size_for_class(env, resolvedClass));
            int psCast = parserStatusCast(env, resolvedClass);
            int vsCast = valueSourceCast(env, resolvedClass);
            int viCast = valueInterceptorCast(env, resolvedClass);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            int fhCast = finalizerHookCast(env, resolvedClass);
            jmethodID constructor{nullptr};
            void (*create)(void *, void *) = nullptr;
            void* userdata = nullptr;
            switch(constructionMode(env, resolvedClass, resolvedClass, false, constructor)){
            case QQmlPrivate::ConstructionMode::Constructor:
                create = &createQmlObject;
                userdata = creatorFunctionMetaData(env, resolved_meta_object, resolvedClass, constructor, objectSize, psCast, vsCast, viCast, fhCast);
                break;
            default: break;
            }
            QQmlPrivate::CreateValueTypeFunction createValueType = nullptr;
            if(!(typeId.flags() & QMetaType::IsPointer)){
                if(jmethodID jsvConstructor = resolveMethod(env, "<init>", "(Lio/qt/qml/QJSValue;)V", resolvedClass)){
                    resolvedClass = qtjambi_to_global_reference(env, resolvedClass);
                    createValueType = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                                [resolvedClass,jsvConstructor,typeId](const QJSValue & arguments) -> QVariant {
                                                    if(JNIEnv* env = qtjambi_current_environment()){
                                                        QTJAMBI_JNI_LOCAL_FRAME(env, 300)
                                                        jobject args = qtjambi_cast<jobject>(env, arguments);
                                                        jobject result = env->NewObject(resolvedClass, jsvConstructor, args);
                                                        QVariant v = qtjambi_to_qvariant(env, result);
                                                        if(v.metaType()!=typeId)
                                                            v.convert(typeId);
                                                        return v;
                                                    }
                                                    return QVariant();
                                                }, Java::Runtime::Object::hashCode(env, resolvedClass)
                    );
                    qtjambi_register_metatype_converter(env, QMetaType::fromType<QJSValue>(), Java::QtQml::QJSValue::getClass(env), typeId, resolvedClass, jsvConstructor);
                }else if(extendedClass){
                    if(jmethodID jsvFactory = resolveMethod(env, "create", qPrintable(QStringLiteral("(Lio/qt/qml/QJSValue;)L%1;").arg(qtjambi_class_name(env, resolvedClass).replace('.', '/'))), extendedClass, true)){
                        extendedClass = qtjambi_to_global_reference(env, extendedClass);
                        createValueType = qtjambi_function_pointer<64, QVariant(const QJSValue&)>(
                                    [extendedClass,jsvFactory,typeId](const QJSValue & arguments) -> QVariant {
                                                        if(JNIEnv* env = qtjambi_current_environment()){
                                                            QTJAMBI_JNI_LOCAL_FRAME(env, 300)
                                                            jobject args = qtjambi_cast<jobject>(env, arguments);
                                                            jobject result = env->CallStaticObjectMethod(extendedClass, jsvFactory, args);
                                                            QVariant v = qtjambi_to_qvariant(env, result);
                                                            if(v.metaType()!=typeId)
                                                                v.convert(typeId);
                                                            return v;
                                                        }
                                                        return QVariant();
                                                    }, Java::Runtime::Object::hashCode(env, resolvedClass)
                        );
                        qtjambi_register_metatype_converter(env, QMetaType::fromType<QJSValue>(), Java::QtQml::QJSValue::getClass(env), typeId, resolvedClass, jsvFactory);
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
                QMetaType extendedTypeID(qtjambi_register_metatype(env, extendedClass, qtjambi_class_name(env, extendedClass).replace(".", "/")));
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
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QQmlEngine::singletonInstance(int)")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QQmlEngine *__qt_this = qtjambi_object_from_nativeId<QQmlEngine>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
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
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_fromVariant)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, jobject jVariant, QtJambiNativeID metaTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::fromVariant")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        QVariant value = qtjambi_to_qvariant(env, jVariant);
        QMetaType targetType = qtjambi_object_from_nativeId_deref<QMetaType>(env, metaTypeId);
        if (value.metaType()==targetType){
            result = jVariant;
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
                result = qtjambi_from_qvariant(env, t);
            }else{
                QMetaType::convert(value.metaType(), value.constData(), targetType, t.data());
                result = qtjambi_from_qvariant(env, t);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
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
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_fromScriptValue)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, QtJambiNativeID jmanaged, QtJambiNativeID metaTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::fromScriptValue")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        const QJSValue& value = qtjambi_object_from_nativeId_deref<QJSValue>(env, jmanaged);
        QMetaType targetType = qtjambi_object_from_nativeId_deref<QMetaType>(env, metaTypeId);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
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
            result = qtjambi_from_qvariant(env, t);
        }else if (value.isVariant()){
            t = value.toVariant();
            t.convert(targetType);
            result = qtjambi_from_qvariant(env, t);
        }else{
            result = qtjambi_from_qvariant(env, QVariant(targetType, nullptr));
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

using Create = QJSValue(QJSEngine::*)(QMetaType type, const void *ptr);
template <>
constexpr Create qjsvalue_cast<Create>(const QJSValue &)
{
    return &QJSEngine::create;
}
// QJSEngine::toScriptValue
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_toScriptValue)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, jobject object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::toScriptValue")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        QVariant variant = qtjambi_to_qvariant(env, object);
        Create create = qjsvalue_cast<Create>(QJSValue{});
        QJSValue jsval = (__qt_this->*create)(variant.metaType(), variant.data());
        result = qtjambi_cast<jobject>(env, jsval);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
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
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_fromManagedValue)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, QtJambiNativeID jmanaged, QtJambiNativeID metaTypeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::fromManagedValue")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        const QJSManagedValue& value = qtjambi_object_from_nativeId_deref<QJSManagedValue>(env, jmanaged);
        QMetaType targetType = qtjambi_object_from_nativeId_deref<QMetaType>(env, metaTypeId);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
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
            result = qtjambi_from_qvariant(env, t);
        }else{
            t = value.toVariant();
            t.convert(targetType);
            result = qtjambi_from_qvariant(env, t);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

using CreateManaged = QJSManagedValue(QJSEngine::*)(QMetaType type, const void *ptr);
template <>
constexpr CreateManaged qjsvalue_cast<CreateManaged>(const QJSValue &)
{
    return &QJSEngine::createManaged;
}
// QJSEngine::toManagedValue
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_qml_QJSEngine_toManagedValue)
(JNIEnv *env,
 jclass,
 QtJambiNativeID __this_nativeId, jobject object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QJSEngine::toManagedValue")
    jobject result{nullptr};
    QTJAMBI_TRY {
        QJSEngine *__qt_this = qtjambi_object_from_nativeId<QJSEngine>(__this_nativeId);
        qtjambi_check_resource(env, __qt_this);
        QVariant variant = qtjambi_to_qvariant(env, object);
        CreateManaged createManaged = qjsvalue_cast<CreateManaged>(QJSManagedValue{});
        QJSManagedValue jsval = (__qt_this->*createManaged)(variant.metaType(), variant.data());
        result = qtjambi_cast<jobject>(env, jsval);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}
#endif

