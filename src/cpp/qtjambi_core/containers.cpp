/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QProperty>
#endif

#include <QtCore/QWriteLocker>
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/private/qcoreapplication_p.h>
#include "qtjambiqfuture.h"

#include <qtjambi/qtjambi_application.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_repository.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_cast.h>

void __qt_construct_QPropertyChangeHandler(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*);
void __qt_destruct_QPropertyObserver(void* ptr);
void deleter_QPropertyObserver(void *ptr);

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_preinit)
(JNIEnv *__jni_env, jclass)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QCoreApplication::preinit()")
    try{
#ifdef Q_OS_DARWIN
        if (!pthread_main_np()) {
            JavaException::raiseError(__jni_env, "QtJambi does not appear to be running on the main thread and will "
                     "most likely be unstable and crash. "
                     "Please make sure to launch your 'java' command with the "
                     "'-XstartOnFirstThread' command line option. For instance: "
                     "java -XstartOnFirstThread any.vendor.MainClass" QTJAMBI_STACKTRACEINFO );
        }
#endif
        QCoreApplicationPrivate::theMainThread = QThread::currentThread();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

typedef QList<JObjectWrapper> RoutineList;
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gRoutineLock, (QReadWriteLock::Recursive))
Q_GLOBAL_STATIC(RoutineList, gPreRoutines)
Q_GLOBAL_STATIC(RoutineList, gPostRoutines)

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_addPreRoutine)
(JNIEnv *__jni_env, jclass, jobject cleanUpFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qAddPostRoutine(QtCleanUpFunction)")

    if (QCoreApplication::instance()){
        Java::Runtime::Runnable::run(__jni_env, cleanUpFunction);
    }else if(!gPreRoutines.isDestroyed()){
        QWriteLocker locker(gRoutineLock());
        gPreRoutines->append(JObjectWrapper(__jni_env, cleanUpFunction));
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_addPostRoutine)
(JNIEnv *__jni_env, jclass, jobject cleanUpFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qAddPostRoutine(QtCleanUpFunction)")
    if(!gPostRoutines.isDestroyed()){
        QWriteLocker locker(gRoutineLock());
        gPostRoutines->prepend(JObjectWrapper(__jni_env, cleanUpFunction));
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QCoreApplication_removePostRoutine)
(JNIEnv *__jni_env, jclass, jobject cleanUpFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qAddPostRoutine(QtCleanUpFunction)")
    if(!gPostRoutines.isDestroyed()){
        QWriteLocker locker(gRoutineLock());
        gPostRoutines->removeAll(JObjectWrapper(__jni_env, cleanUpFunction));
    }
}

void qtjambi_register_map_access2();
void qtjambi_register_map_access3();
void qtjambi_register_map_access4();
void qtjambi_register_map_access5();
void qtjambi_register_map_access6();
void qtjambi_register_hash_access2();
void qtjambi_register_hash_access3();
void qtjambi_register_hash_access4();
void qtjambi_register_hash_access5();
void qtjambi_register_hash_access6();
void qtjambi_register_multimap_access2();
void qtjambi_register_multimap_access3();
void qtjambi_register_multimap_access4();
void qtjambi_register_multimap_access5();
void qtjambi_register_multimap_access6();
void qtjambi_register_multihash_access2();
void qtjambi_register_multihash_access3();
void qtjambi_register_multihash_access4();
void qtjambi_register_multihash_access5();
void qtjambi_register_multihash_access6();

void initialize_meta_info_QtCore()
{
    qtjambi_register_map_access2();
    qtjambi_register_map_access3();
    qtjambi_register_map_access4();
    qtjambi_register_map_access5();
    qtjambi_register_map_access6();
    qtjambi_register_hash_access2();
    qtjambi_register_hash_access3();
    qtjambi_register_hash_access4();
    qtjambi_register_hash_access5();
    qtjambi_register_hash_access6();
    qtjambi_register_multimap_access2();
    qtjambi_register_multimap_access3();
    qtjambi_register_multimap_access4();
    qtjambi_register_multimap_access5();
    qtjambi_register_multimap_access6();
    qtjambi_register_multihash_access2();
    qtjambi_register_multihash_access3();
    qtjambi_register_multihash_access4();
    qtjambi_register_multihash_access5();
    qtjambi_register_multihash_access6();

    qAddPreRoutine([](){
        if(JNIEnv *__jni_env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 400)
            for(const JObjectWrapper& cleanUpFunction : *gPreRoutines){
                try{
                    Java::Runtime::Runnable::run(__jni_env, cleanUpFunction.object());
                }catch(const JavaException& exn){
                    exn.report(__jni_env);
                }
            }
        }
        gPreRoutines->clear();
    });
    qAddPostRoutine([](){
        if(JNIEnv *__jni_env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 400)
            for(const JObjectWrapper& cleanUpFunction : *gPreRoutines){
                try{
                    Java::Runtime::Runnable::run(__jni_env, cleanUpFunction.object());
                }catch(const JavaException& exn){
                    exn.report(__jni_env);
                }
            }
        }
        gPostRoutines->clear();
    });

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    {
        struct AccessBindable : QUntypedBindable{
            const QtPrivate::QBindableInterface *iface() const{
                return QUntypedBindable::iface;
            }
        };
        const std::type_info& typeId = typeid(QUntypedBindable);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.id()==QMetaType::Bool;
                }
                return false;
            }, "io/qt/core/QBooleanBindable", registerValueTypeInfo<QBindable<bool>>("QBindable<bool>", "io/qt/core/QBooleanBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.sizeOf()==1
                            && (metaType.id()==QMetaType::SChar
                                || metaType.id()==QMetaType::Char
                                || metaType.id()==QMetaType::UChar);
                }
                return false;
            }, "io/qt/core/QByteBindable", registerValueTypeInfo<QBindable<qint8>>("QBindable<qint8>", "io/qt/core/QByteBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.sizeOf()==2
                            && (metaType.id()==QMetaType::QChar);
                }
                return false;
            }, "io/qt/core/QCharBindable", registerValueTypeInfo<QBindable<QChar>>("QBindable<QChar>", "io/qt/core/QCharBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.sizeOf()==2
                            && (metaType.id()==QMetaType::Char16);
                }
                return false;
            }, "io/qt/core/QCharBindable", registerValueTypeInfo<QBindable<char16_t>>("QBindable<char16_t>", "io/qt/core/QCharBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.sizeOf()==2
                            && (metaType.id()==QMetaType::Short
                                || metaType.id()==QMetaType::UShort);
                }
                return false;
            }, "io/qt/core/QShortBindable", registerValueTypeInfo<QBindable<qint16>>("QBindable<qint16>", "io/qt/core/QShortBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.sizeOf()==4
                            && (metaType.id()==QMetaType::Int
                                || metaType.id()==QMetaType::UInt
                                || metaType.id()==QMetaType::Long
                                || metaType.id()==QMetaType::ULong
                                || metaType.id()==QMetaType::Char32);
                }
                return false;
            }, "io/qt/core/QIntBindable", registerValueTypeInfo<QBindable<qint32>>("QBindable<qint32>", "io/qt/core/QIntBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.sizeOf()==8
                            && (metaType.id()==QMetaType::Long
                                || metaType.id()==QMetaType::LongLong
                                || metaType.id()==QMetaType::ULong
                                || metaType.id()==QMetaType::ULongLong);
                }
                return false;
            }, "io/qt/core/QLongBindable", registerValueTypeInfo<QBindable<qint64>>("QBindable<qint64>", "io/qt/core/QLongBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType==QMetaType::fromType<char32_t>();
                }
                return false;
            }, "io/qt/core/QIntBindable", registerValueTypeInfo<QBindable<char32_t>>("QBindable<char32_t>", "io/qt/core/QIntBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.sizeOf()==4
                            && (metaType.id()==QMetaType::Float
                                || metaType.id()==QMetaType::Double);
                }
                return false;
            }, "io/qt/core/QFloatBindable", registerValueTypeInfo<QBindable<float>>("QBindable<float>", "io/qt/core/QFloatBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return metaType.sizeOf()==8
                            && (metaType.id()==QMetaType::Float
                                || metaType.id()==QMetaType::Double);
                }
                return false;
            }, "io/qt/core/QDoubleBindable", registerValueTypeInfo<QBindable<double>>("QBindable<double>", "io/qt/core/QDoubleBindable"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                AccessBindable *bindable = reinterpret_cast<AccessBindable *>(ptr);
                Q_ASSERT(bindable);
                if(bindable->iface()
                        && bindable->iface()->metaType){
                    QMetaType metaType = bindable->iface()->metaType();
                    return !(metaType.id()==QMetaType::SChar
                             || metaType.id()==QMetaType::Char
                             || metaType.id()==QMetaType::UChar
                             || metaType.id()==QMetaType::QChar
                             || metaType.id()==QMetaType::Char16
                             || metaType.id()==QMetaType::Short
                             || metaType.id()==QMetaType::UShort
                             || metaType.id()==QMetaType::Int
                             || metaType.id()==QMetaType::UInt
                             || metaType.id()==QMetaType::Char32
                             || metaType.id()==QMetaType::Long
                             || metaType.id()==QMetaType::LongLong
                             || metaType.id()==QMetaType::ULong
                             || metaType.id()==QMetaType::ULongLong
                             || metaType.id()==QMetaType::Float
                             || metaType.id()==QMetaType::Double
                             || metaType.id()==QMetaType::Bool
                             || metaType.id()==QMetaType::Void
                             || metaType.id()==QMetaType::UnknownType);
                }
                return false;
            }, "io/qt/core/QBindable", registerValueTypeInfo<QBindable<QVariant>>("QBindable<QVariant>", "io/qt/core/QBindable"), false);
        registerMetaType<QBindable<bool>>("QBindable<bool>");
        registerMetaType<QBindable<qint8>>("QBindable<qint8>");
        registerMetaType<QBindable<qint16>>("QBindable<qint16>");
        registerMetaType<QBindable<qint32>>("QBindable<qint32>");
        registerMetaType<QBindable<char32_t>>("QBindable<char32_t>");
        registerMetaType<QBindable<qint64>>("QBindable<qint64>");
        registerMetaType<QBindable<float>>("QBindable<float>");
        registerMetaType<QBindable<double>>("QBindable<double>");
        registerMetaType<QBindable<QChar>>("QBindable<QChar>");
        registerMetaType<QBindable<char16_t>>("QBindable<char16_t>");
        registerMetaType<QBindable<QVariant>>("QBindable<QVariant>");
    }

    {
        const std::type_info& typeId = typeid(QUntypedPropertyBinding);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return metaType.id()==QMetaType::Bool;
            }, "io/qt/core/QBooleanPropertyBinding", registerValueTypeInfo<QPropertyBinding<bool>>("QPropertyBinding<bool>", "io/qt/core/QBooleanPropertyBinding"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return metaType.sizeOf()==1
                        && (metaType.id()==QMetaType::SChar
                            || metaType.id()==QMetaType::Char
                            || metaType.id()==QMetaType::UChar);
            }, "io/qt/core/QBytePropertyBinding", registerValueTypeInfo<QPropertyBinding<qint8>>("QPropertyBinding<qint8>", "io/qt/core/QBytePropertyBinding"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return metaType.sizeOf()==2
                        && (metaType.id()==QMetaType::QChar
                            || metaType.id()==QMetaType::Char16);
            }, "io/qt/core/QCharPropertyBinding", registerValueTypeInfo<QPropertyBinding<QChar>>("QPropertyBinding<QChar>", "io/qt/core/QCharPropertyBinding"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return metaType.sizeOf()==2
                        && (metaType.id()==QMetaType::Short
                            || metaType.id()==QMetaType::UShort);
            }, "io/qt/core/QShortPropertyBinding", registerValueTypeInfo<QPropertyBinding<qint16>>("QPropertyBinding<qint16>", "io/qt/core/QShortPropertyBinding"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return metaType.sizeOf()==4
                        && (metaType.id()==QMetaType::Int
                            || metaType.id()==QMetaType::UInt
                            || metaType.id()==QMetaType::Long
                            || metaType.id()==QMetaType::ULong
                            || metaType.id()==QMetaType::Char32);
            }, "io/qt/core/QIntPropertyBinding", registerValueTypeInfo<QPropertyBinding<qint32>>("QPropertyBinding<qint32>", "io/qt/core/QIntPropertyBinding"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return metaType.sizeOf()==8
                        && (metaType.id()==QMetaType::Long
                            || metaType.id()==QMetaType::LongLong
                            || metaType.id()==QMetaType::ULong
                            || metaType.id()==QMetaType::ULongLong);
            }, "io/qt/core/QLongPropertyBinding", registerValueTypeInfo<QPropertyBinding<qint64>>("QPropertyBinding<qint64>", "io/qt/core/QLongPropertyBinding"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return metaType.sizeOf()==4
                        && (metaType.id()==QMetaType::Float
                            || metaType.id()==QMetaType::Double);
            }, "io/qt/core/QFloatPropertyBinding", registerValueTypeInfo<QPropertyBinding<float>>("QPropertyBinding<float>", "io/qt/core/QFloatPropertyBinding"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return metaType.sizeOf()==8
                        && (metaType.id()==QMetaType::Float
                            || metaType.id()==QMetaType::Double);
            }, "io/qt/core/QDoublePropertyBinding", registerValueTypeInfo<QPropertyBinding<double>>("QPropertyBinding<double>", "io/qt/core/QDoublePropertyBinding"), false);
        registerPolymorphyHandler(typeId, [](void *ptr) -> bool {
                QUntypedPropertyBinding *binding = reinterpret_cast<QUntypedPropertyBinding *>(ptr);
                Q_ASSERT(binding);
                QMetaType metaType = binding->valueMetaType();
                return !(metaType.id()==QMetaType::SChar
                         || metaType.id()==QMetaType::Char
                         || metaType.id()==QMetaType::UChar
                         || metaType.id()==QMetaType::QChar
                         || metaType.id()==QMetaType::Char16
                         || metaType.id()==QMetaType::Short
                         || metaType.id()==QMetaType::UShort
                         || metaType.id()==QMetaType::Int
                         || metaType.id()==QMetaType::UInt
                         || metaType.id()==QMetaType::Char32
                         || metaType.id()==QMetaType::Long
                         || metaType.id()==QMetaType::LongLong
                         || metaType.id()==QMetaType::ULong
                         || metaType.id()==QMetaType::ULongLong
                         || metaType.id()==QMetaType::Float
                         || metaType.id()==QMetaType::Double
                         || metaType.id()==QMetaType::Void
                         || metaType.id()==QMetaType::UnknownType);
            }, "io/qt/core/QPropertyBinding", registerValueTypeInfo<QPropertyBinding<QVariant>>("QPropertyBinding<QVariant>", "io/qt/core/QPropertyBinding"), false);
        registerMetaType<QPropertyBinding<bool>>("QPropertyBinding<bool>");
        registerMetaType<QPropertyBinding<qint8>>("QPropertyBinding<qint8>");
        registerMetaType<QPropertyBinding<QChar>>("QPropertyBinding<QChar>");
        registerMetaType<QPropertyBinding<qint16>>("QPropertyBinding<qint16>");
        registerMetaType<QPropertyBinding<qint32>>("QPropertyBinding<qint32>");
        registerMetaType<QPropertyBinding<qint64>>("QPropertyBinding<qint64>");
        registerMetaType<QPropertyBinding<float>>("QPropertyBinding<float>");
        registerMetaType<QPropertyBinding<double>>("QPropertyBinding<double>");
        registerMetaType<QPropertyBinding<QVariant>>("QPropertyBinding<QVariant>");
    }

    {
        registerValueTypeInfo<QPropertyAlias<bool>>("QPropertyAlias<bool>", "io/qt/core/QBooleanPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<qint8>>("QPropertyAlias<qint8>", "io/qt/core/QBytePropertyAlias");
        registerValueTypeInfo<QPropertyAlias<qint16>>("QPropertyAlias<qint16>", "io/qt/core/QShortPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<qint32>>("QPropertyAlias<qint32>", "io/qt/core/QIntPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<char32_t>>("QPropertyAlias<char32_t>", "io/qt/core/QIntPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<qint64>>("QPropertyAlias<qint64>", "io/qt/core/QLongPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<float>>("QPropertyAlias<float>", "io/qt/core/QFloatPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<double>>("QPropertyAlias<double>", "io/qt/core/QDoublePropertyAlias");
        registerValueTypeInfo<QPropertyAlias<QChar>>("QPropertyAlias<QChar>", "io/qt/core/QCharPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<char16_t>>("QPropertyAlias<char16_t>", "io/qt/core/QCharPropertyAlias");
        registerValueTypeInfo<QPropertyAlias<QVariant>>("QPropertyAlias<QVariant>", "io/qt/core/QPropertyAlias");
        registerMetaType<QPropertyAlias<bool>>("QPropertyAlias<bool>");
        registerMetaType<QPropertyAlias<qint8>>("QPropertyAlias<qint8>");
        registerMetaType<QPropertyAlias<qint16>>("QPropertyAlias<qint16>");
        registerMetaType<QPropertyAlias<qint32>>("QPropertyAlias<qint32>");
        registerMetaType<QPropertyAlias<char32_t>>("QPropertyAlias<char32_t>");
        registerMetaType<QPropertyAlias<qint64>>("QPropertyAlias<qint64>");
        registerMetaType<QPropertyAlias<float>>("QPropertyAlias<float>");
        registerMetaType<QPropertyAlias<double>>("QPropertyAlias<double>");
        registerMetaType<QPropertyAlias<QChar>>("QPropertyAlias<QChar>");
        registerMetaType<QPropertyAlias<char16_t>>("QPropertyAlias<char16_t>");
        registerMetaType<QPropertyAlias<QVariant>>("QPropertyAlias<QVariant>");
    }
    {
        const std::type_info& typeId = registerObjectTypeInfo<QPropertyChangeHandler<void(*)()>>("QPropertyChangeHandler", "io/qt/core/QPropertyChangeHandler");
        registerConstructorInfos(typeId, &__qt_destruct_QPropertyObserver, {
            ConstructorInfo(&__qt_construct_QPropertyChangeHandler, nullptr)
        });
        registerDeleter(typeId, &deleter_QPropertyObserver);
        registerSizeOfShell(typeId, sizeof(QPropertyObserver));
    }
#endif
}
