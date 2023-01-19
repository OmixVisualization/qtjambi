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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtCore/QWriteLocker>
#include <QtCore/QCoreApplication>
#include <QtCore/QQueue>
#include <QtCore/QStack>
#include <QtCore/QDir>
#include <QtCore/QCborValue>
#include <QtCore/QLibrary>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#include <QtCore/QXmlStreamStringRef>
#endif
#include "registryutil_p.h"
#include "java_p.h"
#include "qtjambilink_p.h"

#include "qtjambi_cast.h"

#ifdef Q_OS_IOS
#ifdef QT_NO_DEBUG
#define JNI_OnLoad JNI_OnLoad_QtJambi
#define JNI_OnUnload JNI_OnUnload_QtJambi
#else
#define JNI_OnLoad JNI_OnLoad_QtJambi_debug
#define JNI_OnUnload JNI_OnUnload_QtJambi_debug
#endif
#endif

#ifndef JNI_VERSION_1_8
#define JNI_VERSION_1_8 JNI_VERSION_1_6
#endif

void shutdown(JNIEnv * env);
void clearGlobalClassPointersAtShutdown(JNIEnv *env);
void clearRegistryAtShutdown(JNIEnv * env);
void clearTypeHandlersAtShutdown(JNIEnv *env);
void clearMessageHandlerAtShutdown(JNIEnv *env);
void clearSuperTypesAtShutdown(JNIEnv *env);
void clearMetaObjectsAtShutdown(JNIEnv * env);
void clearJarImportersAtShutdown(JNIEnv * env);
void clearFunctionPointersAtShutdown();
void registerPointerContainerAccess();
JNIEnv *currentJNIEnvironment(bool initializeJavaThread = true);
QObject* connectionSender(const QMetaObject::Connection* connection);
void registerPluginImporter();

std::atomic<bool>* getJVMLoaded();
std::atomic<JavaVM *>* getJVM();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

namespace QtJambiVariant {
    void registerHandler();
    void unregisterHandler();
}

void jobjectwrapper_save(QDataStream &stream, const void *_jObjectWrapper)
{
    if(JniEnvironment env{200}){
        const JObjectWrapper *jObjectWrapper = static_cast<const JObjectWrapper *>(_jObjectWrapper);
        stream << *jObjectWrapper;
    }
}

void jcollectionwrapper_save(QDataStream &stream, const void *_jCollectionWrapper)
{
    const JCollectionWrapper *jCollectionWrapper = static_cast<const JCollectionWrapper *>(_jCollectionWrapper);
    bool ok = false;
    QStringList stringList = jCollectionWrapper->toStringList(&ok);
    if(ok){
        stream << stringList;
    }else{
        stream << jCollectionWrapper->toList();
    }
}

void jmapwrapper_save(QDataStream &stream, const void *_jMapWrapper)
{
    const JMapWrapper *jMapWrapper = static_cast<const JMapWrapper *>(_jMapWrapper);
    bool ok = false;
    QVariantMap variantMap = jMapWrapper->toStringMap(&ok);
    if(ok){
        stream << variantMap;
    }else{
        stream << jMapWrapper->toMap();
    }
}

void jcollectionwrapper_load(QDataStream &stream, void *_jCollectionWrapper)
{
    if(JniEnvironment env{200}){
        JCollectionWrapper *jCollectionWrapper = static_cast<JCollectionWrapper *>(_jCollectionWrapper);
        QList<QVariant> list;
        stream >> list;
        jobject res = QtJambiAPI::convertQVariantToJavaObject(env, QVariant::fromValue<QList<QVariant>>(list));
        *jCollectionWrapper = JCollectionWrapper(env, res);
    }
}

void jmapwrapper_load(QDataStream &stream, void *_jMapWrapper)
{
    if(JniEnvironment env{200}){
        JMapWrapper *jMapWrapper = static_cast<JMapWrapper *>(_jMapWrapper);
        QMap<QVariant,QVariant> map;
        stream >> map;
        jobject res = QtJambiAPI::convertQVariantToJavaObject(env, QVariant::fromValue<QMap<QVariant,QVariant>>(map));
        *jMapWrapper = JMapWrapper(env, res);
    }
}

void jobjectwrapper_load(QDataStream &stream, void *_jObjectWrapper)
{
    if(JniEnvironment env{200}){
        QtJambiExceptionHandler __exnHandler;
        try{
            JObjectWrapper *jObjectWrapper = static_cast<JObjectWrapper *>(_jObjectWrapper);
            stream >> *jObjectWrapper;
        }catch(const JavaException& exn){
            __exnHandler.handle(env, exn, nullptr);
        }
    }
}
#endif

template<typename T>
const std::type_info& registerSpecialTypeInfo(const char *qt_name, const char *java_name)
{
    using namespace RegistryAPI;
    const std::type_info& id = typeid(T);
    registerTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, EntryTypes::SpecialTypeInfo);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerStringTypeInfo(const char *qt_name, const char *java_name)
{
    using namespace RegistryAPI;
    const std::type_info& id = typeid(T);
    registerTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, EntryTypes::StringTypeInfo);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerPrimitiveMetaTypeInfo(const char *qt_name, const char *java_name)
{
    using namespace RegistryAPI;
    const std::type_info& id = registerPrimitiveTypeInfo<T>(qt_name, java_name);
    int mid = qRegisterMetaType<T>(qt_name);
    registerMetaTypeID(id, mid);
/*#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QtJambiPrivate::MetaTypeStreamOperatorsHelper<T>::registerStreamOperators(mid);
    QtJambiPrivate::MetaTypeComparatorHelper<T>::registerComparators(mid);
    QtJambiPrivate::MetaTypeDebugStreamOperatorHelper<T>::registerDebugStreamOperator(mid);
#endif*/
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerContainerTypeInfo(const char *qt_name, const char *java_name, const char *java_interface)
{
    using namespace RegistryAPI;
    const std::type_info& id = typeid(T);
    registerContainerTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, java_interface);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    //registerOperators<T>();
    return id;
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnLoad)(JavaVM *vm, void *)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi::JNI_OnLoad(JavaVM *, void *)")
    using namespace RegistryAPI;
    if(std::atomic<bool>* atm = getJVMLoaded()){
        if(atm->load())
            return JNI_VERSION_1_8;
        atm->store(true);
    }else{
        return JNI_VERSION_1_8;
    }
    if(std::atomic<JavaVM *>* atm = getJVM()){
        if(atm->load()!=nullptr)
            return JNI_VERSION_1_8;
        atm->store(vm);
    }else{
        return JNI_VERSION_1_8;
    }
    ::atexit([]{shutdown(nullptr);});

    JNIEnv * env = currentJNIEnvironment(vm);
#if defined(Q_OS_ANDROID) && !defined(QT_NO_DEBUG)
    if(env){
        JniLocalFrame __jniLocalFrame(env, 16);
        Java::Runtime::System::setProperty(env, env->NewStringUTF("io.qt.debug"), env->NewStringUTF("debug"));
    }
#endif
    {
        try{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QtJambiLinkUserData::id();
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if(env){
                JniLocalFrame __jniLocalFrame(env, 200);
                if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("qt.disable.thread.affinity.check"))
                        || Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.disable-thread-affinity-check"))){
                    disableThreadAffinity();
                    QInternal::registerCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
                }else if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("qt.disable.event.thread.affinity.check"))
                         || Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.disable-event-thread-affinity-check"))){
                    QInternal::registerCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
                }else{
                    QInternal::registerCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
                }
            }else{
                QInternal::registerCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
            }

            registerPointerContainerAccess();
            //qtjambi_register_containeraccess_all();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QtJambiVariant::registerHandler();
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

            registerMetaTypeID(typeid(JObjectWrapper), qMetaTypeId<JObjectWrapper>());
            registerSpecialTypeInfo<JObjectWrapper>("JObjectWrapper", "java/lang/Object");
            registerSpecialTypeInfo<JObjectArrayWrapper>("JObjectArrayWrapper", "[Ljava/lang/Object;");
            {
                const std::type_info& typeId = registerSpecialTypeInfo<JIntArrayWrapper>("JIntArrayWrapper", "[I");
                registerMetaTypeID(typeId, qMetaTypeId<JIntArrayWrapper>());
                registerTypeAlias(typeId, QMetaType::fromType<JIntArrayWrapper>().name(), nullptr);
            }
            {
                const std::type_info& typeId = registerSpecialTypeInfo<JShortArrayWrapper>("JShortArrayWrapper", "[S");
                registerMetaTypeID(typeId, qMetaTypeId<JShortArrayWrapper>());
                registerTypeAlias(typeId, QMetaType::fromType<JShortArrayWrapper>().name(), nullptr);
            }
            {
                const std::type_info& typeId = registerSpecialTypeInfo<JByteArrayWrapper>("JByteArrayWrapper", "[B");
                registerMetaTypeID(typeId, qMetaTypeId<JByteArrayWrapper>());
                registerTypeAlias(typeId, QMetaType::fromType<JByteArrayWrapper>().name(), nullptr);
            }
            {
                const std::type_info& typeId = registerSpecialTypeInfo<JLongArrayWrapper>("JLongArrayWrapper", "[J");
                registerMetaTypeID(typeId, qMetaTypeId<JLongArrayWrapper>());
                registerTypeAlias(typeId, QMetaType::fromType<JLongArrayWrapper>().name(), nullptr);
            }
            {
                const std::type_info& typeId = registerSpecialTypeInfo<JBooleanArrayWrapper>("JBooleanArrayWrapper", "[Z");
                registerMetaTypeID(typeId, qMetaTypeId<JBooleanArrayWrapper>());
                registerTypeAlias(typeId, QMetaType::fromType<JBooleanArrayWrapper>().name(), nullptr);
            }
            {
                const std::type_info& typeId = registerSpecialTypeInfo<JCharArrayWrapper>("JCharArrayWrapper", "[C");
                registerMetaTypeID(typeId, qMetaTypeId<JCharArrayWrapper>());
                registerTypeAlias(typeId, QMetaType::fromType<JCharArrayWrapper>().name(), nullptr);
            }
            {
                const std::type_info& typeId = registerSpecialTypeInfo<JDoubleArrayWrapper>("JDoubleArrayWrapper", "[D");
                registerMetaTypeID(typeId, qMetaTypeId<JDoubleArrayWrapper>());
                registerTypeAlias(typeId, QMetaType::fromType<JDoubleArrayWrapper>().name(), nullptr);
            }
            {
                const std::type_info& typeId = registerSpecialTypeInfo<JFloatArrayWrapper>("JFloatArrayWrapper", "[F");
                registerMetaTypeID(typeId, qMetaTypeId<JFloatArrayWrapper>());
                registerTypeAlias(typeId, QMetaType::fromType<JFloatArrayWrapper>().name(), nullptr);
            }
            {
                const std::type_info& typeId = registerSpecialTypeInfo<std::nullptr_t>("std::nullptr_t", "java/lang/Object");
                registerMetaTypeID(typeId, QMetaType::Nullptr);
            }
            registerMetaTypeID(typeid(QObject*), typeid(QObject), QMetaType::QObjectStar);
            registerMetaType<JNIEnv*>("JNIEnv*");
            registerSpecialTypeInfo<void*>("void*", "io/qt/QNativePointer");
            registerMetaTypeID(typeid(void*), QMetaType::VoidStar);
            {
                const std::type_info& typeId = registerSpecialTypeInfo<QVariant>("QVariant", "java/lang/Object");
                registerTypeAlias(typeId, nullptr, "io/qt/core/QVariant");
                registerMetaTypeID(typeId, QMetaType::QVariant);
            }

            {
                registerEnumTypeInfo<QVariant::Type>("QVariant::Type", "io/qt/core/QVariant$Type");
            }
            registerMetaTypeID(typeid(JObjectArrayWrapper), qMetaTypeId<JObjectArrayWrapper>());
            registerMetaTypeID(typeid(JMapWrapper), qMetaTypeId<JMapWrapper>());
            registerMetaTypeID(typeid(JCollectionWrapper), qMetaTypeId<JCollectionWrapper>());
            registerMetaTypeID(typeid(JIteratorWrapper), qMetaTypeId<JIteratorWrapper>());
            registerMetaTypeID(typeid(JEnumWrapper), qMetaTypeId<JEnumWrapper>());
            registerMetaType<QMap<QVariant,QVariant> >("QMap<QVariant,QVariant>");
            registerSpecialTypeInfo<JEnumWrapper>("JEnumWrapper", "java/lang/Enum");
            registerSpecialTypeInfo<JIteratorWrapper>("JIteratorWrapper", "java/util/Iterator");
            registerSpecialTypeInfo<JCollectionWrapper>("JCollectionWrapper", "java/util/Collection");
            registerSpecialTypeInfo<JMapWrapper>("JMapWrapper", "java/util/Map");
            //qRegisterMetaTypeStreamOperators<JCollectionWrapper>();
            QMetaType::registerConverter<JEnumWrapper,qint32>(&JEnumWrapper::ordinal);
            QMetaType::registerConverter<JCollectionWrapper,QList<QVariant>>(&JCollectionWrapper::toList);
            QMetaType::registerConverter<JCollectionWrapper,QStringList>(&JCollectionWrapper::toStringList);
            QMetaType::registerConverter<JCollectionWrapper,JObjectWrapper>([](const JObjectWrapper& w) -> JObjectWrapper {return JObjectWrapper(w);});
            QMetaType::registerConverter<JIteratorWrapper,JObjectWrapper>([](const JIteratorWrapper& w) -> JObjectWrapper {return JObjectWrapper(w);});
            QMetaType::registerConverter<JMapWrapper,JObjectWrapper>([](const JMapWrapper& w) -> JObjectWrapper {return JObjectWrapper(w);});
            QMetaType::registerConverter<JObjectWrapper,QString>(&JObjectWrapper::toString);
            QMetaType::registerConverter<JCollectionWrapper,QString>([](const JObjectWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JIteratorWrapper,QString>([](const JIteratorWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JMapWrapper,QString>([](const JMapWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JObjectArrayWrapper,QString>(&JObjectArrayWrapper::toString);
            QMetaType::registerConverter<JIntArrayWrapper,QString>([](const JIntArrayWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JShortArrayWrapper,QString>([](const JShortArrayWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JByteArrayWrapper,QString>([](const JByteArrayWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JLongArrayWrapper,QString>([](const JLongArrayWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JBooleanArrayWrapper,QString>([](const JBooleanArrayWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JCharArrayWrapper,QString>([](const JCharArrayWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JDoubleArrayWrapper,QString>([](const JDoubleArrayWrapper& w) -> QString {return w.toString();});
            QMetaType::registerConverter<JFloatArrayWrapper,QString>([](const JFloatArrayWrapper& w) -> QString {return w.toString();});
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QMetaType::registerConverter<JObjectWrapper,jobject>([](const JObjectWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JCollectionWrapper,jobject>([](const JObjectWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JMapWrapper,jobject>([](const JMapWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JIteratorWrapper,jobject>([](const JIteratorWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JObjectArrayWrapper,jobject>([](JObjectArrayWrapper w) -> jobject {return w;});
            QMetaType::registerConverter<JObjectArrayWrapper,jobjectArray>([](JObjectArrayWrapper w) -> jobjectArray {return w;});
            QMetaType::registerConverter<JIntArrayWrapper,jobject>([](const JIntArrayWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JIntArrayWrapper,jintArray>([](const JIntArrayWrapper& w) -> jintArray {return w;});
            QMetaType::registerConverter<JShortArrayWrapper,jobject>([](const JShortArrayWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JShortArrayWrapper,jshortArray>([](const JShortArrayWrapper& w) -> jshortArray {return w;});
            QMetaType::registerConverter<JByteArrayWrapper,jobject>([](const JByteArrayWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JByteArrayWrapper,jbyteArray>([](const JByteArrayWrapper& w) -> jbyteArray {return w;});
            QMetaType::registerConverter<JLongArrayWrapper,jobject>([](const JLongArrayWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JLongArrayWrapper,jlongArray>([](const JLongArrayWrapper& w) -> jlongArray {return w;});
            QMetaType::registerConverter<JBooleanArrayWrapper,jobject>([](const JBooleanArrayWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JBooleanArrayWrapper,jbooleanArray>([](const JBooleanArrayWrapper& w) -> jbooleanArray {return w;});
            QMetaType::registerConverter<JCharArrayWrapper,jobject>([](const JCharArrayWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JCharArrayWrapper,jcharArray>([](const JCharArrayWrapper& w) -> jcharArray {return w;});
            QMetaType::registerConverter<JDoubleArrayWrapper,jobject>([](const JDoubleArrayWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JDoubleArrayWrapper,jdoubleArray>([](const JDoubleArrayWrapper& w) -> jdoubleArray {return w;});
            QMetaType::registerConverter<JFloatArrayWrapper,jobject>([](const JFloatArrayWrapper& w) -> jobject {return w;});
            QMetaType::registerConverter<JFloatArrayWrapper,jfloatArray>([](const JFloatArrayWrapper& w) -> jfloatArray {return w;});
            QMetaType::registerConverter<JObjectWrapper,JEnumWrapper>([](const JObjectWrapper& w) -> JEnumWrapper {
                if(JniEnvironment env{200}){
                    if(Java::Runtime::Enum::isInstanceOf(env, w.object())){
                        return JEnumWrapper(env, w.object());
                    }
                }
                return JEnumWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JCollectionWrapper>([](const JObjectWrapper& w) -> JCollectionWrapper {
                if(JniEnvironment env{200}){
                    if(Java::Runtime::Collection::isInstanceOf(env, w.object())){
                        return JCollectionWrapper(env, w.object());
                    }
                }
                return JCollectionWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JMapWrapper>([](const JObjectWrapper& w) -> JMapWrapper {
                if(JniEnvironment env{200}){
                    if(Java::Runtime::Map::isInstanceOf(env, w.object())){
                        return JMapWrapper(env, w.object());
                    }
                }
                return JMapWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JObjectArrayWrapper>([](const JObjectWrapper& w) -> JObjectArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && !Java::Runtime::Class::isPrimitive(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JObjectArrayWrapper(env, jobjectArray(w.object()));
                        }
                    }
                }
                return JObjectArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JIntArrayWrapper>([](const JObjectWrapper& w) -> JIntArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && Java::Runtime::Integer::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JIntArrayWrapper(env, jintArray(w.object()));
                        }
                    }
                }
                return JIntArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JLongArrayWrapper>([](const JObjectWrapper& w) -> JLongArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && Java::Runtime::Long::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JLongArrayWrapper(env, jlongArray(w.object()));
                        }
                    }
                }
                return JLongArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JShortArrayWrapper>([](const JObjectWrapper& w) -> JShortArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && Java::Runtime::Short::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JShortArrayWrapper(env, jshortArray(w.object()));
                        }
                    }
                }
                return JShortArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JByteArrayWrapper>([](const JObjectWrapper& w) -> JByteArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && Java::Runtime::Byte::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JByteArrayWrapper(env, jbyteArray(w.object()));
                        }
                    }
                }
                return JByteArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JBooleanArrayWrapper>([](const JObjectWrapper& w) -> JBooleanArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && Java::Runtime::Boolean::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JBooleanArrayWrapper(env, jbooleanArray(w.object()));
                        }
                    }
                }
                return JBooleanArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JCharArrayWrapper>([](const JObjectWrapper& w) -> JCharArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && Java::Runtime::Character::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JCharArrayWrapper(env, jcharArray(w.object()));
                        }
                    }
                }
                return JCharArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JFloatArrayWrapper>([](const JObjectWrapper& w) -> JFloatArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && Java::Runtime::Float::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JFloatArrayWrapper(env, jfloatArray(w.object()));
                        }
                    }
                }
                return JFloatArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JDoubleArrayWrapper>([](const JObjectWrapper& w) -> JDoubleArrayWrapper {
                if(JniEnvironment env{200}){
                    if(w.object()){
                        jclass objectClass = env->GetObjectClass(w.object());
                        if(Java::Runtime::Class::isArray(env, objectClass)
                                && Java::Runtime::Double::isPrimitiveType(env, Java::Runtime::Class::getComponentType(env, objectClass))){
                            return JDoubleArrayWrapper(env, jdoubleArray(w.object()));
                        }
                    }
                }
                return JDoubleArrayWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,QVariant>([](const JObjectWrapper& w) -> QVariant {
                return QVariant::fromValue(w);
            });
            QMetaType::registerConverter<JObjectWrapper,std::nullptr_t>([](const JObjectWrapper&) -> std::nullptr_t {
                return nullptr;
            });
#endif
            QMetaType::registerConverter<JMapWrapper,QMap<QVariant,QVariant>>(&JMapWrapper::toMap);
            QMetaType::registerConverter<JMapWrapper,QMap<QString,QVariant>>(&JMapWrapper::toStringMap);
            QMetaType::registerConverter<JMapWrapper,QHash<QString,QVariant>>(&JMapWrapper::toStringHash);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QMetaType::registerStreamOperators(qMetaTypeId<JEnumWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JObjectWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JCollectionWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jcollectionwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jcollectionwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JMapWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jmapwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jmapwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JIteratorWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JObjectArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JIntArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JShortArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JByteArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JLongArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JCharArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JBooleanArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JDoubleArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
            QMetaType::registerStreamOperators(qMetaTypeId<JFloatArrayWrapper>(),
                               reinterpret_cast<QMetaType::SaveOperator>(jobjectwrapper_save),
                               reinterpret_cast<QMetaType::LoadOperator>(jobjectwrapper_load));
#endif
            registerTypeInfo(typeid(void), QtJambiTypeInfo::of<void>(), "void", "void", EntryTypes::PrimitiveTypeInfo);
            registerMetaTypeID(typeid(void), QMetaType::Void);
            registerPrimitiveMetaTypeInfo<qint64>("qint64", "long");
            registerPrimitiveMetaTypeInfo<quint64>("quint64", "long");
            registerPrimitiveMetaTypeInfo<qulonglong>("qulonglong", "long");
            registerPrimitiveMetaTypeInfo<qlonglong>("qlonglong", "long");
            registerPrimitiveMetaTypeInfo<qint32>("qint32", "int");
            registerPrimitiveMetaTypeInfo<quint32>("quint32", "int");
            registerPrimitiveMetaTypeInfo<qint16>("qint16", "short");
            registerPrimitiveMetaTypeInfo<quint16>("quint16", "short");
            registerPrimitiveMetaTypeInfo<qint8>("qint8", "byte");
            registerPrimitiveMetaTypeInfo<quint8>("quint8", "byte");
            registerPrimitiveMetaTypeInfo<uchar>("uchar", "byte");
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            registerPrimitiveMetaTypeInfo<std::byte>("char", "byte");
#endif
            registerPrimitiveMetaTypeInfo<float>("float", "float");
            registerPrimitiveMetaTypeInfo<double>("double", "double");
            registerPrimitiveMetaTypeInfo<bool>("bool", "boolean");
            registerPrimitiveMetaTypeInfo<int>("int", "int");
            registerPrimitiveMetaTypeInfo<char>("char", "byte");
            registerPrimitiveMetaTypeInfo<long long>("long long", "long");
            registerPrimitiveMetaTypeInfo<short>("short", "short");
            registerPrimitiveMetaTypeInfo<QChar>("QChar", "char");
            registerPrimitiveMetaTypeInfo<wchar_t>("wchar_t", "char");
            registerPrimitiveMetaTypeInfo<char16_t>("char16_t", "char");
            registerPrimitiveMetaTypeInfo<char32_t>("char32_t", "int");
            registerPrimitiveTypeInfo<QLatin1Char>("QLatin1Char", "char");

            registerStringTypeInfo<QString>("QString", "java/lang/String");
            registerMetaType<QString>("QString");
            registerTypeAlias(typeid(QString), nullptr, "io/qt/core/QString");
            registerTypeAlias(typeid(unsigned int), "QRgb", nullptr);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            registerStringTypeInfo<QXmlStreamStringRef>("QXmlStreamStringRef", "java/lang/String");
            registerMetaType<QXmlStreamStringRef>("QXmlStreamStringRef");

            registerStringTypeInfo<QStringRef>("QStringRef", "java/lang/String");
            registerMetaType<QStringRef>("QStringRef");
#else
            registerStringTypeInfo<QAnyStringView>("QAnyStringView", "java/lang/String");
            registerMetaType<QAnyStringView>("QAnyStringView");
            registerStringTypeInfo<QUtf8StringView>("QUtf8StringView", "java/lang/String");
            registerMetaType<QUtf8StringView>("QUtf8StringView");
#endif // QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

            registerStringTypeInfo<QLatin1String>("QLatin1String", "java/lang/String");

            registerStringTypeInfo<QStringView>("QStringView", "java/lang/String");
            registerMetaType<QStringView>("QStringView");

            registerSpecialTypeInfo<QMetaObject>("QMetaObject", "io/qt/core/QMetaObject");
            registerEnumTypeInfo<QMetaObject::Call>("QMetaObject::Call", "io/qt/core/QMetaObject$Call");
            registerSpecialTypeInfo<QMetaEnum>("QMetaEnum", "io/qt/core/QMetaEnum");
            registerMetaType<QMetaEnum>("QMetaEnum");

            registerSpecialTypeInfo<QMetaMethod>("QMetaMethod", "io/qt/core/QMetaMethod");
            registerMetaType<QMetaMethod>("QMetaMethod");

            registerSpecialTypeInfo<QMetaProperty>("QMetaProperty", "io/qt/core/QMetaProperty");
            registerMetaType<QMetaProperty>("QMetaProperty");

            {
                const std::type_info& typeId = registerSpecialTypeInfo<QMetaObject::Connection>("QMetaObject::Connection", "io/qt/core/QMetaObject$Connection");
                registerMetaType<QMetaObject::Connection>("QMetaObject::Connection");
                registerOwnerFunction(typeId, [](const void* p) -> const QObject* {
                    return connectionSender(reinterpret_cast<const QMetaObject::Connection*>(p));
                });
            }

#if QT_VERSION >= 0x050C00
            registerSpecialTypeInfo<QCborValueRef>("QCborValueRef", "io/qt/core/QCborValue");
#endif
            {
                registerSpecialTypeInfo<QUrl::FormattingOptions>("QUrlTwoFlags<QUrl::UrlFormattingOption,QUrl::ComponentFormattingOption>", "io/qt/core/QUrl$FormattingOptions");
                registerTypeAlias(typeid(QUrl::FormattingOptions), "QUrl::FormattingOptions", nullptr);
                int id = registerMetaType<QUrl::FormattingOptions>("QUrlTwoFlags<QUrl::UrlFormattingOption,QUrl::ComponentFormattingOption>");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                QtJambiPrivate::MetaTypeStreamOperatorsHelper<QUrl::FormattingOptions>::registerStreamOperators(id);
                QtJambiPrivate::MetaTypeComparatorHelper<QUrl::FormattingOptions>::registerComparators(id);
                QtJambiPrivate::MetaTypeDebugStreamOperatorHelper<QUrl::FormattingOptions>::registerDebugStreamOperator(id);
#else
                Q_UNUSED(id)
#endif

                registerValueTypeInfo<QStringList>("QStringList", "io/qt/core/QStringList");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                QMetaType::registerNormalizedTypedef("QList<QString>", QMetaType::QStringList);
                QMetaType::registerNormalizedTypedef("QList<QByteArray>", QMetaType::QByteArrayList);
                QMetaType::registerNormalizedTypedef("QList<QVariant>", QMetaType::QVariantList);
                QMetaType::registerNormalizedTypedef("QMap<QString,QVariant>", QMetaType::QVariantMap);
                QMetaType::registerNormalizedTypedef("QHash<QString,QVariant>", QMetaType::QVariantHash);
#else
                QMetaType::registerNormalizedTypedef("QList<QString>", QMetaType(QMetaType::QStringList));
                QMetaType::registerNormalizedTypedef("QList<QByteArray>", QMetaType(QMetaType::QByteArrayList));
                QMetaType::registerNormalizedTypedef("QList<QVariant>", QMetaType(QMetaType::QVariantList));
                QMetaType::registerNormalizedTypedef("QMap<QString,QVariant>", QMetaType(QMetaType::QVariantMap));
                QMetaType::registerNormalizedTypedef("QHash<QString,QVariant>", QMetaType(QMetaType::QVariantHash));
#endif
                registerSpecialTypeInfo<QPair<QVariant,QVariant>>("QPair", "io/qt/core/QPair");
                registerSpecialTypeInfo<std::pair<QVariant,QVariant>>("QPair", "io/qt/core/QPair");
                registerContainerTypeInfo<QHash<QVariant,QVariant>>("QHash", "io/qt/core/QHash", "java/util/Map");
                registerContainerTypeInfo<QMultiHash<QVariant,QVariant>>("QMultiHash", "io/qt/core/QMultiHash", "java/util/Map");
                registerContainerTypeInfo<QMultiMap<QVariant,QVariant>>("QMultiMap", "io/qt/core/QMultiMap", "java/util/NavigableMap");
                registerContainerTypeInfo<QMap<QVariant,QVariant>>("QMap", "io/qt/core/QMap", "java/util/NavigableMap");
                registerContainerTypeInfo<QList<QVariant>::iterator>("QSequentialIterator", "io/qt/core/QSequentialIterator", "java/lang/Iterable");
                registerContainerTypeInfo<QMap<QVariant,QVariant>::iterator>("QAssociativeIterator", "io/qt/core/QAssociativeIterator", "java/lang/Iterable");
                registerContainerTypeInfo<QList<QVariant>::const_iterator>("QSequentialConstIterator", "io/qt/core/QSequentialConstIterator", "java/lang/Iterable");
                registerContainerTypeInfo<QMap<QVariant,QVariant>::const_iterator>("QAssociativeConstIterator", "io/qt/core/QAssociativeConstIterator", "java/lang/Iterable");
                registerContainerTypeInfo<QList<QVariant>>("QList", "io/qt/core/QList", "java/util/List");
                registerContainerTypeInfo<std::vector<QVariant>>("std::vector", "java/util/ArrayList", "java/util/List");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                registerContainerTypeInfo<QLinkedList<QVariant>>("QLinkedList", "io/qt/core/QLinkedList", "java/util/Collection");
                registerContainerTypeInfo<QVector<QVariant>>("QVector", "io/qt/core/QVector", "java/util/List");
#endif
                registerContainerTypeInfo<QQueue<QVariant>>("QQueue", "io/qt/core/QQueue", "java/util/Queue");
                registerContainerTypeInfo<QSet<QVariant>>("QSet", "io/qt/core/QSet", "java/util/Set");
                registerContainerTypeInfo<QStack<QVariant>>("QStack", "io/qt/core/QStack", "java/util/Deque");
                registerEnumTypeInfo<QString::SectionFlag>("QString::SectionFlag", "io/qt/core/QString$SectionFlag", "QFlags<QtString::SectionFlag>", "QtString::SectionFlags", "io/qt/core/QString$SectionFlags");
            }
            registerPluginImporter();

            if(env){
                JniLocalFrame __jniLocalFrame(env, 200);
                bool hasChanged = false;
                Java::QtJambi::ResourceUtility::initialize(env);
                QStringList libraryPaths = QCoreApplication::libraryPaths();
                QFileInfo sunBootLibraryPath(qtjambi_cast<QString>(env, Java::Runtime::System::getProperty(env, env->NewStringUTF("sun.boot.library.path"), nullptr)));
                if(sunBootLibraryPath.exists())
                    hasChanged |= libraryPaths.removeAll(sunBootLibraryPath.absoluteFilePath()) > 0;
                QFileInfo thisLibraryPath(getFunctionLibraryPath(reinterpret_cast<QFunctionPointer>(&JNI_OnLoad)));
#ifdef Q_OS_ANDROID
                //qputenv("QT_DEBUG_PLUGINS", "1");
                libraryPaths << thisLibraryPath.absolutePath();
                hasChanged = true;
                QLibrary library(thisLibraryPath.absolutePath() + "/" + thisLibraryPath.fileName().replace("QtJambi"
#if !defined(QT_NO_DEBUG)
                                                                                                           "_debug"
#endif
                                                                                                           , "plugins_platforms_qtforandroid"));
                if(!library.load()){
                    qWarning("Unable to load qtforandroid: %s", qPrintable(library.errorString()));
                }
                qputenv("QML_IMPORT_PATH", qPrintable(thisLibraryPath.absolutePath()));
                qputenv("QML2_IMPORT_PATH", qPrintable(thisLibraryPath.absolutePath()));

                if(jobject context = Java::Android::QtNative::getContext(env)){
                    if(jobject appInfo = Java::Android::Context::getApplicationInfo(env, context)){
                        jstring sourceDir = Java::Android::ApplicationInfo::sourceDir(env, appInfo);
                        QString _sourceDir = "file:"+qtjambi_cast<QString>(env, sourceDir);
                        Java::QtJambi::ResourceUtility::addSearchPath(env, Java::Runtime::URL::newInstance(env, qtjambi_cast<jstring>(env, _sourceDir)));
                    }
                }
#else
                if(thisLibraryPath.isFile()){
                    if(thisLibraryPath.absolutePath().endsWith("bin")
                            || thisLibraryPath.absolutePath().endsWith("lib")){
                        QDir dir = thisLibraryPath.absoluteDir();
                        dir.cdUp();
                        if(dir.cd("plugins")){
                            libraryPaths << dir.absolutePath();
                            hasChanged = true;
                        }
                    }
                }else if(thisLibraryPath.isDir()){
                    if(thisLibraryPath.absoluteFilePath().endsWith("bin")
                            || thisLibraryPath.absoluteFilePath().endsWith("lib")){
                        QDir dir = thisLibraryPath.absoluteDir();
                        if(dir.cd("plugins")){
                            libraryPaths << dir.absolutePath();
                            hasChanged = true;
                        }
                    }
                }
#endif
#ifdef Q_OS_WINDOWS
#define PATHSEP ';'
#else
#define PATHSEP ':'
#endif
                {
                    QStringList paths = qtjambi_cast<QString>(env, Java::Runtime::System::getProperty(env, env->NewStringUTF("io.qt.pluginpath"), nullptr)).split(PATHSEP);
                    paths << qtjambi_cast<QString>(env, Java::Runtime::System::getProperty(env, env->NewStringUTF("qtjambi.pluginpath"), nullptr)).split(PATHSEP);
                    for (const QString& p : qAsConst(paths)) {
                        libraryPaths << QFileInfo(p).absoluteFilePath();
                        hasChanged = true;
                    }
                    if(Java::QtJambi::LibraryUtility::isNativeDeployment(env)){
                        jobject pluginPaths = Java::QtJambi::LibraryUtility::pluginPaths(env);
                        // don't use qtjambi_cast here!
                        jobject iter = QtJambiAPI::iteratorOfJavaCollection(env, pluginPaths);
                        while(QtJambiAPI::hasJavaIteratorNext(env, iter)) {
                            jstring p = jstring(QtJambiAPI::nextOfJavaIterator(env, iter));
                            libraryPaths << QFileInfo(qtjambi_cast<QString>(env, p)).absoluteFilePath();
                            hasChanged = true;
                        }
                    }
                }
                hasChanged |= libraryPaths.removeDuplicates() > 0 || libraryPaths.removeAll({}) > 0;
                if(hasChanged)
                    QCoreApplication::setLibraryPaths(libraryPaths);
            }

            if(!QCoreApplication::instance()){
                qAddPreRoutine([](){
                    QtJambiExceptionInhibitor __exnHandler;
                    if(JniEnvironment env{400}){
                        try{
                            Java::QtCore::Internal::QCoreApplication::execPreRoutines(env);
                        }catch(const JavaException& exn){
                            __exnHandler.handle(env, exn, "preRoutine");
                        }
                    }
                });
            }
            qAddPostRoutine([](){
                QtJambiExceptionInhibitor __exnHandler;
                if(JniEnvironment env{400}){
                    try{
                        Java::QtCore::Internal::QCoreApplication::execPostRoutines(env);
                    }catch(const JavaException& exn){
                        __exnHandler.handle(env, exn, "postRoutine");
                    }
                }
            });
            Java::Runtime::URL::getClass(env);
            Java::Runtime::URLConnection::getClass(env);
        }catch(const JavaException& e){
            if(env){
                JniLocalFrame __jniLocalFrame(env, 16);
                e.raiseInJava(env);
            }else{
                qWarning("%s", e.what());
            }
        }catch (const std::exception& exn) {
            qWarning("An error occurred: %s", exn.what());
        }catch (...) {
            qWarning("An error occurred.");
        }
    }
    return JNI_VERSION_1_8;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnUnload)(JavaVM *, void *)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi::JNI_OnUnload(JavaVM *, void *)")
    DefaultJniEnvironment env{32};
    shutdown(env);
    clearGlobalClassPointersAtShutdown(env);
    clearRegistryAtShutdown(env);
    if(std::atomic<JavaVM *>* atm = getJVM())
        atm->store(nullptr);
}

void shutdown(JNIEnv * env)
{
    if(std::atomic<bool>* atm = getJVMLoaded()){
        if(!atm->load())
            return;
        JavaException exception;
        if(env){
            clearMessageHandlerAtShutdown(env);
            try{
                Java::Runtime::System::gc(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
            try{
                Java::QtJambi::NativeUtility::terminateCleanupThread(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
        }

        QInternal::unregisterCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
        QInternal::unregisterCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QtJambiVariant::unregisterHandler();
    #endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        clearJarImportersAtShutdown(env);
        clearSuperTypesAtShutdown(env);
        clearMetaObjectsAtShutdown(env);
        clearTypeHandlersAtShutdown(env);
        clearFunctionPointersAtShutdown();
        if(env){
            try{
                Java::QtJambi::LibraryUtility::clear(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
            try{
                Java::QtJambi::ResourceUtility::clear(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
        }
        atm->store(false);
        if(exception)
            throw exception;
    }
}

#ifdef Q_OS_ANDROID

int main(int argc, char *argv[])
{
    if(JniEnvironment env{1024}){
        bool tryKt = false;
        QByteArray className;
        try{
            jobject activity = Java::Android::QtNative::activity(env);
            jobject service = Java::Android::QtNative::service(env);
            if(activity || service){
                if(jstring packageName = Java::Android::ContextWrapper::getPackageName(env, activity ? activity : service)){
                    if(jobject packageManager = Java::Android::ContextWrapper::getPackageManager(env, activity ? activity : service)){
                        jstring mainClassProp = env->NewStringUTF("qtjambi.main");
                        jobject info = nullptr;
                        try{
                            info = Java::Android::PackageManager::getApplicationInfo(env, packageManager, packageName, Java::Android::PackageManager::GET_META_DATA(env));
                        }catch (...) {}
                        if(info){
                            if(jobject metaData = Java::Android::PackageItemInfo::metaData(env, info)){
                                jstring mainClass = Java::Android::BaseBundle::getString(env, metaData, mainClassProp);
                                if(mainClass && env->GetStringLength(mainClass)>0){
                                    const char* c = env->GetStringUTFChars(mainClass, nullptr);
                                    className = c;
                                    env->ReleaseStringUTFChars(mainClass, c);
                                }
                            }
                        }
                        if(className.isEmpty()){
                            if(activity){
                                try{
                                    info = Java::Android::PackageManager::getActivityInfo(env, packageManager, Java::Android::Activity::getComponentName(env, activity), Java::Android::PackageManager::GET_META_DATA(env));
                                }catch (...) {}
                                if(info){
                                    if(jobject metaData = Java::Android::PackageItemInfo::metaData(env, info)){
                                        jstring mainClass = Java::Android::BaseBundle::getString(env, metaData, mainClassProp);
                                        if(mainClass && env->GetStringLength(mainClass)>0){
                                            const char* c = env->GetStringUTFChars(mainClass, nullptr);
                                            className = c;
                                            env->ReleaseStringUTFChars(mainClass, c);
                                        }
                                    }
                                }
                            }else{
                                try{
                                    info = Java::Android::PackageManager::getServiceInfo(env, packageManager, Java::Android::Service::getComponentName(env, activity), Java::Android::PackageManager::GET_META_DATA(env));
                                }catch (...) {}
                                if(info){
                                    if(jobject metaData = Java::Android::PackageItemInfo::metaData(env, info)){
                                        jstring mainClass = Java::Android::BaseBundle::getString(env, metaData, mainClassProp);
                                        if(mainClass && env->GetStringLength(mainClass)>0){
                                            const char* c = env->GetStringUTFChars(mainClass, nullptr);
                                            className = c;
                                            env->ReleaseStringUTFChars(mainClass, c);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(className.isEmpty()){
                        const char* c = env->GetStringUTFChars(packageName, nullptr);
                        className = c;
                        env->ReleaseStringUTFChars(packageName, c);
                        className += ".Main";
                        tryKt = true;
                    }
                }
            }
        }catch (const JavaException& exn) {
            Java::Runtime::Throwable::printStackTrace(env, exn.object());
            return -1;
        }catch (const std::exception& exn) {
            qWarning("An error occurred: %s", exn.what());
            return -1;
        }catch (...) {
            qWarning("An error occurred.");
            return -1;
        }
        int reduceArgs = 1;
        if(className.isEmpty() && argc>=2){
            className = argv[1];
            reduceArgs = 2;
        }
        if(!className.isEmpty()){
            try{
                className.replace('.', '/');
                jclass cls = env->FindClass(className.data());
                if(tryKt && !cls){
                    if(env->ExceptionCheck())
                        env->ExceptionClear();
                    className += "Kt";
                    cls = env->FindClass(className.data());
                }
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                    return -1;
                }
                jmethodID mainMethod = env->GetStaticMethodID(cls, "main", "([Ljava/lang/String;)V");
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                    return -1;
                }
                jsize arrayLength = argc > 0 ? argc-reduceArgs : 0;
                jobjectArray arguments = env->NewObjectArray(arrayLength, Java::Runtime::String::getClass(env), nullptr);
                for(jsize i=0; i<arrayLength; ++i){
                    env->SetObjectArrayElement(arguments, i, env->NewStringUTF(argv[reduceArgs+i]));
                }
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                    return -1;
                }
                env->CallStaticVoidMethod(cls, mainMethod, arguments);
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                    return -1;
                }
            }catch (const JavaException& exn) {
                Java::Runtime::Throwable::printStackTrace(env, exn.object());
                return -1;
            }catch (const std::exception& exn) {
                qWarning("An error occurred: %s", exn.what());
                return -1;
            }catch (...) {
                qWarning("An error occurred.");
                return -1;
            }
            qputenv("QT_ANDROID_NO_EXIT_CALL", "1");
            return 0;
        }else{
            qWarning("no main class specified");
        }
    }else{
        qWarning("Unable to load JVM.");
    }
    return -1;
}
#endif
