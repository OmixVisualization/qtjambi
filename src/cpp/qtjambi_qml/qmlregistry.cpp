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
#include "qtjambi_qml_repository.h"
#include "qmlcreateparentfunction.h"
#include "qmlattachedpropertiesfunction.h"
#include "qmlcreatorfunction.h"

class QQmlParserStatus;
class QQmlPropertyValueSource;
class QQmlPropertyValueInterceptor;

const QMetaObject * attachedPropertiesMetaObject(JNIEnv * env, jobject method){
    if(!method)
        return nullptr;
    jclass returnType = jclass(Java::Runtime::Method::getReturnType(env, method));
    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, returnType);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, returnType);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            meta_object = qtjambi_metaobject_for_class(env, returnType, original_meta_object);
        }
    }
    return meta_object;
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

int registerQQmlListPropertyAsQmlMetaType(const QString& javaName){
    QByteArray listName = "QQmlListProperty<" + javaName.toLatin1().replace(".", "::").replace("$", "::").replace("/", "::") + '>';
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
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
    return registerQmlMetaType({},
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
int registerQObjectAsQmlMetaType(const QString& javaName, const QMetaObject *meta_object)
{
    QByteArray _javaName = javaName.toLatin1().replace(".", "::").replace("$", "::")+"*";

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    int definedType = QMetaType::type(_javaName);
    if(definedType!=QMetaType::UnknownType){
        return definedType;
    }
    int typeId = QMetaType::registerNormalizedType(_javaName,
                                     QtMetaTypePrivate::QMetaTypeFunctionHelper<QObject*>::Destruct,
                                     QtMetaTypePrivate::QMetaTypeFunctionHelper<QObject*>::Construct,
                                     sizeof(QObject*),
                                     QMetaType::TypeFlags( QMetaType::MovableType | QMetaType::PointerToQObject ),
                                     meta_object);
    registerJavaClassForCustomMetaType(typeId, QString(javaName).replace(".", "/").toLatin1());
    return typeId;
#else
    QMetaType definedType = QMetaType::fromName(_javaName);
    if(definedType.id()!=QMetaType::UnknownType){
        return definedType.id();
    }
    return registerQmlMetaType(javaName.toLatin1().replace(".", "/"), _javaName,
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
#endif
}

jmethodID findConstructor(JNIEnv * env, jclass clazz, const QString& javaName){
    jmethodID constructor(nullptr);
    QTJAMBI_TRY_ANY{
        constructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    jmethodID superClassConstructor = nullptr;
    jclass generatedSuperclass = qtjambi_find_generated_superclass(env, clazz);
    if(generatedSuperclass){
        QTJAMBI_TRY_ANY{
            superClassConstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject$QDeclarativeConstructor;)V", clazz);
        }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    }else{
        JavaException::raiseIllegalAccessException(env, qPrintable(QStringLiteral("Class %1 cannot be registered as Qml type since it does not inherit a Qt class.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
    }
    if(!constructor){
        if(superClassConstructor){
            JavaException::raiseIllegalAccessException(env, qPrintable(QStringLiteral("Class must offer the constructor %1(QDeclarativeConstructor) to register as Qml type.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
        }else{
            QString superClassName = qtjambi_class_name(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            JavaException::raiseIllegalAccessException(env, qPrintable(QStringLiteral("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')), superClassName)) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        if(!superClassConstructor){
            QString superClassName = qtjambi_class_name(env, generatedSuperclass).replace(QLatin1Char('$'), QLatin1Char('.'));
            JavaException::raiseIllegalAccessException(env, qPrintable(QStringLiteral("Class %1 cannot be registered as Qml type because its super class %2 is excluded.").arg(QString(javaName).replace(QLatin1Char('$'), QLatin1Char('.')), superClassName)) QTJAMBI_STACKTRACEINFO );
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
    QQmlAttachedPropertiesFunc attachedPropertiesFunction = nullptr;
    const QMetaObject *attachedPropertiesMetaObject = nullptr;
};

enum Skip{
    Creator = 0x01,
    AttachedProperties = 0x02,
    MetaObject = 0x04,
    ObjectSize = 0x08,
};
typedef QFlags<Skip> Skipping;

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void createQmlValue(void* placement,void* metaData){
    const QtPrivate::QMetaTypeInterface* iface = reinterpret_cast<const QtPrivate::QMetaTypeInterface*>(metaData);
    Q_ASSERT(iface->defaultCtr);
    iface->defaultCtr(iface, placement);
}
#endif

QmlTypeRegistractionData registerQmlType(JNIEnv *env, jclass clazz, const char* qmlName, Skipping skip = {}){
    Q_UNUSED(qmlName)
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
        if (qtjambi_find_QmlAttachedProperties(env, clazz)) {
            JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("%1 is not a QObject, but has attached properties. This won't work.").arg(data.javaName)) QTJAMBI_STACKTRACEINFO );
        }
        QMetaType typeId(qtjambi_register_metatype(env, clazz, data.javaName.replace(".", "/")));
        data.typeId = typeId.id();
        if(data.typeId==0){
            JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("%1 is not a valid Qt value type. Valid value tyoes define a default constructor as well as a clone method and implement java.lang.Cloneable.").arg(data.javaName)) QTJAMBI_STACKTRACEINFO );
        }
        QString typeName = QLatin1String(qmlName);
        if (!typeName.isEmpty()) {
            if(typeName.at(0).isUpper()){
                JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("Invalid QML %1 name \"%2\"; value type names should begin with a lowercase letter").arg(data.javaName, typeName)) QTJAMBI_STACKTRACEINFO );
            }
            int typeNameLen = typeName.length();
            for (int ii = 0; ii < typeNameLen; ++ii) {
                if (!(typeName.at(ii).isLetterOrNumber() || typeName.at(ii) == u'_')) {
                    JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("Invalid QML %1 name \"%2\"").arg(data.javaName, typeName)) QTJAMBI_STACKTRACEINFO );
                }
            }
        }
        data.listId = qtjambi_register_list_metatype(env, typeId);
        if(!skip.testFlag(Skip::ObjectSize)){
            data.objectSize = int(typeId.sizeOf());
        }
        if(!skip.testFlag(Skip::MetaObject)){
            data.meta_object = typeId.metaObject();
        }
        if(!skip.testFlag(Skip::Creator)){
            data.create = &createQmlValue;
            data.userdata = const_cast<QtPrivate::QMetaTypeInterface*>(typeId.iface());
        }
#else
        JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("%1 is not a QObject.").arg(data.javaName)) QTJAMBI_STACKTRACEINFO );
#endif
    }else{
        QString typeName = QLatin1String(qmlName);
        if (!typeName.isEmpty()) {
            if(typeName.at(0).isLower()){
                JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("Invalid QML %1 name \"%2\"; type names must begin with an uppercase letter").arg(data.javaName, typeName)) QTJAMBI_STACKTRACEINFO );
            }
            int typeNameLen = typeName.length();
            for (int ii = 0; ii < typeNameLen; ++ii) {
                if (!(typeName.at(ii).isLetterOrNumber() || typeName.at(ii) == u'_')) {
                    JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("Invalid QML %1 name \"%2\"").arg(data.javaName, typeName)) QTJAMBI_STACKTRACEINFO );
                }
            }
        }
        if(!skip.testFlag(Skip::ObjectSize)){
            data.objectSize = int(qtjambi_extended_size_for_class(env, clazz));
        }
        if(!skip.testFlag(Skip::MetaObject)){
            data.meta_object = qtjambi_metaobject_for_class(env, clazz);
            if(!data.meta_object){
                jclass closestClass = resolveClosestQtSuperclass(env, clazz);
                if(closestClass){
                    const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
                    data.meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
                }
            }
        }
        data.typeId = registerQObjectAsQmlMetaType(data.javaName, data.meta_object);
        data.listId = registerQQmlListPropertyAsQmlMetaType(data.javaName);
        if(!skip.testFlag(Skip::Creator)){
            data.constructor = findConstructor(env, clazz, data.javaName);
            if (!Java::QtCore::QObject::isAssignableFrom(env, clazz) || !data.constructor) {
                JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("%1 is neither a QObject, nor default- and copy-constructible. You should not use it as a QML type.").arg(data.javaName)) QTJAMBI_STACKTRACEINFO );
            }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            data.create = &createQmlObject;
            data.userdata = creatorFunctionMetaData(env, data.meta_object, clazz, data.constructor, data.objectSize, data.psCast, data.vsCast, data.viCast, data.fhCast);
#else
            data.create = creatorFunction(env, data.meta_object, clazz, data.constructor, data.objectSize, data.psCast, data.vsCast, data.viCast);
#endif
        }
        if(!skip.testFlag(Skip::AttachedProperties)){
            jobject method = qtjambi_find_QmlAttachedProperties(env, clazz);
            if (!Java::QtCore::QObject::isAssignableFrom(env, clazz) && method) {
                JavaException::raiseIllegalArgumentException(env, qPrintable(QStringLiteral("%1 is not a QObject, but has attached properties. This won't work.").arg(data.javaName)) QTJAMBI_STACKTRACEINFO );
            }
            data.attachedPropertiesFunction = attachedPropertiesFunc(env, clazz, method);
            data.attachedPropertiesMetaObject = attachedPropertiesMetaObject(env, method);
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
    if(!QMetaType::canConvert(metaType, QMetaType::fromType<QMetaSequence>())){
        JavaException::raiseIllegalArgumentException(env, "Not a valid container type." QTJAMBI_STACKTRACEINFO );
    }
    QQmlPrivate::RegisterSequentialContainer type = {
        0,
        uri,
        QTypeRevision::fromMajorVersion(versionMajor),
        nullptr,
        metaType,
        QVariant(metaType).value<QMetaSequence>(),
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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, Skipping(Creator | ObjectSize));

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
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Not a valid metaObjectRevision %1.").arg(metaObjectRevisionMinor)) QTJAMBI_STACKTRACEINFO );
    }
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, Skipping(Creator | ObjectSize));

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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr);

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
        nullptr,
        nullptr,
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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, Skipping(Creator | ObjectSize));
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID extendedConstructor(nullptr);
    QTJAMBI_TRY_ANY{
        extendedConstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    if(!extendedConstructor){
        JavaException::raiseIllegalAccessException(env, qPrintable(QStringLiteral("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
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

        /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, Skipping());
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID econstructor(nullptr);
    QTJAMBI_TRY_ANY{
        econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    if(!econstructor){
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
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
        jobject method = qtjambi_find_QmlAttachedProperties(env, extendedClazz);
        data.attachedPropertiesFunction = attachedPropertiesFunc(env, extendedClazz, method);
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

        /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, Skipping(Creator));
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID econstructor(nullptr);
    QTJAMBI_TRY_ANY{
        econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    if(!econstructor){
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
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
        jobject method = qtjambi_find_QmlAttachedProperties(env, extendedClazz);
        data.attachedPropertiesFunction = attachedPropertiesFunc(env, extendedClazz, method);
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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, Skipping(Creator));
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID econstructor(nullptr);
    QTJAMBI_TRY_ANY{
        econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    if(!econstructor){
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Class must offer the constructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
    }

    const QMetaObject *meta_object = qtjambi_metaobject_for_class(env, clazz);
    if(!meta_object){
        jclass closestClass = resolveClosestQtSuperclass(env, clazz);
        if(closestClass){
            const QMetaObject *original_meta_object = qtjambi_metaobject_for_class(env, closestClass);
            meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
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
    jclass declClass = clazz;
    jobject method = qtjambi_find_QmlAttachedProperties(env, declClass);
    if(!method){
        declClass = extendedClazz;
        method = qtjambi_find_QmlAttachedProperties(env, declClass);
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
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Not a valid metaObjectRevision %1.").arg(metaObjectRevision)) QTJAMBI_STACKTRACEINFO );
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

        /* int typeId */ registerQObjectAsQmlMetaType(typeName, meta_object),
        /* int listId */ registerQQmlListPropertyAsQmlMetaType(typeName),

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
    QString typeName;
    if(meta_object){
        typeName = meta_object->className();
    }

    const char* iid = getInterfaceIID(env, clazz);
    if(!iid){
        iid = registerInterfaceID(env, clazz);
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QQmlPrivate::RegisterInterface qmlInterface = {
        /* int version */ 1,

        /* int typeId */ registerQObjectAsQmlMetaType(typeName, meta_object),
        /* int listId */ registerQQmlListPropertyAsQmlMetaType(typeName),

        /* const char *iid */ iid,

        /* const char *uri */ uri,
        /* int versionMajor */ versionMajor
    };

#else
    QQmlPrivate::RegisterInterface qmlInterface = {
        /* int structVersion */ 0,

        /* QMetaType typeId */ QMetaType(registerQObjectAsQmlMetaType(typeName, meta_object)),
        /* QMetaType listId */ QMetaType(registerQQmlListPropertyAsQmlMetaType(typeName)),

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
        /* int typeId */ registerQObjectAsQmlMetaType(javaName, meta_object),
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
        /* QMetaType typeId */ QMetaType(registerQObjectAsQmlMetaType(javaName, meta_object)),
        /* extensionObjectCreate */ nullptr,
        /* extensionMetaObject */ nullptr,
        /* int revision */ QTypeRevision::zero()
    };
#endif
    return QQmlPrivate::qmlregister(QQmlPrivate::SingletonRegistration, &api);
}

int qtjambi_qmlRegisterRevision(JNIEnv *env, jclass clazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, Skipping());

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
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Not a valid metaObjectRevision %1.").arg(metaObjectRevision)) QTJAMBI_STACKTRACEINFO );
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

        /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, Skipping());

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

        /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, Skipping());

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
        JavaException::raiseIllegalAccessException(env, qPrintable(QStringLiteral("Not a valid metaObjectRevision %1.").arg(metaObjectRevision)) QTJAMBI_STACKTRACEINFO );
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

        /*QVariant (*createValueType)(const QJSValue &);*/ nullptr,

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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, Skipping(Creator));
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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, qmlName, Skipping(Creator));
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
        JavaException::raiseIllegalAccessException(env, qPrintable(QString("Not a valid metaObjectRevision %1.").arg(metaObjectRevision)) QTJAMBI_STACKTRACEINFO );
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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
int qtjambi_qmlRegisterType(JNIEnv *env, jclass clazz){
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, Skipping(Creator));
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
    QmlTypeRegistractionData data = registerQmlType(env, clazz, nullptr, Skipping(Creator | ObjectSize));
    QString extendedJavaName = qtjambi_class_name(env, extendedClazz);
    jmethodID econstructor(nullptr);
    QTJAMBI_TRY_ANY{
        econstructor = resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", extendedClazz);
    }QTJAMBI_CATCH_ANY{}QTJAMBI_TRY_END
    if(!econstructor){
        JavaException::raiseIllegalAccessException(env, qPrintable(QStringLiteral("Class must offer the econstructor %1(QObject) to register as Qml extended type.").arg(QString(extendedJavaName).replace(QLatin1Char('$'), QLatin1Char('.')))) QTJAMBI_STACKTRACEINFO );
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
                JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
            }
            if(!targetType.iface()->copyCtr){
                JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
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
            JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
        }
        if(!targetType.iface()->copyCtr){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
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
            JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing default constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
        }
        if(!targetType.iface()->copyCtr){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Unable to convert to meta type %1 due to missing copy constructor.").arg(QLatin1String(targetType.name()))) QTJAMBI_STACKTRACEINFO );
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

