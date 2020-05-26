/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qtjambi_core.h"
#include "qtjambi_cast.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambi_registry_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambilink_p.h"
#include "qtjambitypemanager_p.h"
#include "qtdynamicmetaobject_p.h"
#include "qtjambivariant_p.h"
#include "qtjambi_thread.h"
#include "qtjambi_thread_p.h"
#include "qtjambi_interfaces.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_application.h"
#include "qtjambishell_p.h"

#include <qglobal.h>
#include <cstring>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/QVariant>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>
#include <QtCore/QAbstractEventDispatcher>
#include <QtWidgets/QWidget>
#include "qtjambi_templates.h"
#if QT_VERSION < 0x050000
#include <QtGui/QStyleOption>
#endif
#include <stdio.h>
#include <string.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#if defined(QTJAMBI_DEBUG_TOOLS)
 #include "qtjambidebugevent_p.h"
#endif

QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_CLANG("-Wdouble-promotion")

bool operator !(QtJambiNativeID nativeId) { return nativeId == InvalidNativeID; }

// #define JOBJECT_REFCOUNT

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, qtjambi_vm_lock, (QReadWriteLock::Recursive))
static int qtjambi_vm_count = 0;
static JavaVM *qtjambi_vm = nullptr;
static jint qtjambi_jni_version = JNI_VERSION_CURRENT;

void qtjambi_startup(JNIEnv * __jni_env);

/*!
 * This function is called by the Virtual Machine when it loads the
 * library. We need this to get a hold of the global VM pointer
 */
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnLoad)(JavaVM *vm, void *)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi::JNI_OnLoad(JavaVM *, void *)")
    jint result = JNI_VERSION_10;
    if(!qtjambi_vm_lock.isDestroyed()){
        {
            QWriteLocker locker(qtjambi_vm_lock());
            Q_UNUSED(locker)
            if(qtjambi_vm_count == 0)
                qtjambi_vm = vm;
            qtjambi_vm_count++;
            result = qtjambi_jni_version;
        }
        qtjambi_startup(qtjambi_current_environment());
    }
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnUnload)(JavaVM *, void *)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi::JNI_OnUnload(JavaVM *, void *)")
    qtjambi_shutdown();
    if(!qtjambi_vm_lock.isDestroyed()){
        QWriteLocker locker(qtjambi_vm_lock());
        Q_UNUSED(locker)
        if(qtjambi_vm_count <= 1)
            qtjambi_vm = nullptr;
        qtjambi_vm_count--;
    }
}

void jobjectwrapper_save(QDataStream &stream, const void *_jObjectWrapper)
{
    if(JNIEnv *env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        const JObjectWrapper *jObjectWrapper = static_cast<const JObjectWrapper *>(_jObjectWrapper);
        jobject jstream = qtjambi_from_object(env, &stream, typeid(QDataStream), false);
        Java::Private::QtJambi::QtJambiInternal.writeSerializableJavaObject(env, jstream, jObjectWrapper->object());
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
    if(JNIEnv *env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        JCollectionWrapper *jCollectionWrapper = static_cast<JCollectionWrapper *>(_jCollectionWrapper);
        QList<QVariant> list;
        stream >> list;
        jobject res = qtjambi_from_qvariant(env, QVariant::fromValue<QList<QVariant>>(list));
        *jCollectionWrapper = JCollectionWrapper(env, res);
    }
}

void jmapwrapper_load(QDataStream &stream, void *_jMapWrapper)
{
    if(JNIEnv *env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        JMapWrapper *jMapWrapper = static_cast<JMapWrapper *>(_jMapWrapper);
        QMap<QVariant,QVariant> map;
        stream >> map;
        jobject res = qtjambi_from_qvariant(env, QVariant::fromValue<QMap<QVariant,QVariant>>(map));
        *jMapWrapper = JMapWrapper(env, res);
    }
}

void jobjectwrapper_load(QDataStream &stream, void *_jObjectWrapper)
{
    if(JNIEnv *env = qtjambi_current_environment()){
        try{
            JObjectWrapper *jObjectWrapper = static_cast<JObjectWrapper *>(_jObjectWrapper);
            jobject jstream = qtjambi_from_object(env, &stream, typeid(QDataStream), false);
            jobject res = Java::Private::QtJambi::QtJambiInternal.readSerializableJavaObject(env, jstream);
            if(!res){
                *jObjectWrapper = JCollectionWrapper();
            }else if(Java::Private::Runtime::Collection.isInstanceOf(env, res)){
                *jObjectWrapper = JCollectionWrapper(env, res);
            }else if(Java::Private::Runtime::Iterator.isInstanceOf(env, res)){
                *jObjectWrapper = JIteratorWrapper(env, res);
            }else if(Java::Private::Runtime::Map.isInstanceOf(env, res)){
                *jObjectWrapper = JMapWrapper(env, res);
            }else if(Java::Private::Runtime::Enum.isInstanceOf(env, res)){
                *jObjectWrapper = JEnumWrapper(env, res);
            }else{
                *jObjectWrapper = JObjectWrapper(env, res);
            }
        }catch(const JavaException& exn){
            if(qtjambi_is_exceptions_blocked()){
                qtjambi_push_blocked_exception(env, exn);
            }else{
                exn.raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
            }
        }
    }
}

#ifdef QTJAMBI_STACKTRACE
#undef qtjambi_exception_check
#endif

/*!  Checks for an exception in the virtual machine and returns true
  if there was one; otherwise returns false. The function will print
  the stack trace of any exception before clearing the exception state
  in the virtual machine.
*/
bool qtjambi_exception_check(JNIEnv *env QTJAMBI_STACKTRACEINFO_DECL )
{
    if (env->ExceptionCheck()) {
        if(jthrowable throbj = env->ExceptionOccurred()) {
            env->ExceptionClear();
            QString message = QLatin1String("QtJambi: Exception pending in native code");
#ifdef QTJAMBI_STACKTRACE
            if (fileName){
                message = QLatin1String("QtJambi: Exception pending in native code %1 in file %2:%3");
                message = message.arg(QLatin1String(methodName)).arg(QLatin1String(fileName)).arg(QString::number(lineNumber));
            }
#endif
            jstring jmessage = env->NewString(reinterpret_cast<const jchar *>(message.constData()), message.length());
            try{
                Java::Private::QtJambi::QtJambiInternal.reportException(env, jmessage, throbj);
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
        return true;
    }
    return false;
}

#ifdef QTJAMBI_STACKTRACE
#define qtjambi_exception_check(ENV) qtjambi_exception_check(ENV QTJAMBI_STACKTRACEINFO )
#endif

JNIEnv *qtjambi_current_environment(bool initializeJavaThread, JavaVM *vm)
{
    JNIEnv *env = nullptr;
    if (!vm) {
        return nullptr;
    }
    int result = vm->GetEnv( reinterpret_cast<void **>(&env), qtjambi_jni_version);
    if (result == JNI_EDETACHED) {
#ifndef QT_QTJAMBI_PORT
        jobject uncaughtExceptionHandler = nullptr;
        jobject contextClassLoader = nullptr;
        QThread* currentThread = QThread::currentThread();
        QThreadUserData* threadData = static_cast<QThreadUserData*>(currentThread->userData(QThreadUserData::id()));
        if(threadData){
            uncaughtExceptionHandler = threadData->getUncaughtExceptionHandler();
            contextClassLoader = threadData->getContextClassLoader();
            QByteArray name(threadData->getName());
            JavaVMAttachArgs args;
            args.version = qtjambi_jni_version;
            args.name = name.isEmpty() ? nullptr : name.data();
            args.group = threadData->getThreadGroup();
            JavaVMAttachArgs* _args = args.name && args.group ? &args : nullptr;
            if(threadData->isDaemon())
                result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), _args);
            else
                result = vm->AttachCurrentThread(reinterpret_cast<void **>(&env), _args);
            threadData->clearThreadGroup();
        }else{
#endif
            result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), nullptr);
#ifndef QT_QTJAMBI_PORT
        }
#endif
        if (result != JNI_OK) {
            qWarning("Failed attaching current thread");
            return nullptr;
        }
#ifndef QT_QTJAMBI_PORT
        if(env && env->functions && initializeJavaThread){
            jobject jthread = Java::Private::Runtime::Thread.currentThread(env);
            if(uncaughtExceptionHandler){
                Java::Private::Runtime::Thread.setUncaughtExceptionHandler(env, jthread, uncaughtExceptionHandler);
                threadData->clearUncaughtExceptionHandler();
                uncaughtExceptionHandler = nullptr;
            }
            if(contextClassLoader){
                Java::Private::Runtime::Thread.setContextClassLoader(env, jthread, contextClassLoader);
                threadData->clearContextClassLoader();
                contextClassLoader = nullptr;
            }
            QSharedPointer<QtJambiLink> link;
            if(jobject qthreadObject = qtjambi_cast<jobject>(env, currentThread)){
                link = QtJambiLink::findLinkForJavaObject(env, qthreadObject);
                jobject jthrd = Java::QtCore::QThread.javaThread(env, qthreadObject);
                if(!jthrd){
                    Java::QtCore::QThread.set_javaThread(env, qthreadObject, jthread);
                    jobject interruptible = Java::QtCore::QThread.interruptible(env);
                    Java::Private::QtJambi::QtJambiInternal.createAssociation(env, jthread, qthreadObject);
                    if(interruptible){
                        JObjectSynchronizer sync(env, qthreadObject);
                        Q_UNUSED(sync)
                        Java::Private::QtJambi::QtJambiInternal.setThreadInterruptible(env, jthread, interruptible);
                        env->DeleteLocalRef(interruptible);
                        interruptible = nullptr;
                    }
                    if(!currentThread->parent()){
                        qtjambi_set_java_ownership(env, qthreadObject);
                    }

                    // detach the QThread from Java
                    // After emitting QThread::finished the QThreadStorage data is cleared.
                    // Thereafter, the event dispatcher is deleted if available.
                    // Let QThreadStorage-deletion check if an event dispatcher is available and create an empty event dispatcher otherwise.
                    // During deletion of the event dispatcher (which is the latest action in a thread) the java thread is detached.
                    if(!EventDispatcherCheck::storage.hasLocalData() || !EventDispatcherCheck::storage.localData()){
                        JObjectWrapper jthreadObjectWrapper(env, jthread);
                        EventDispatcherCheck::storage.setLocalData(QSharedPointer<EventDispatcherCheck>(
                             new EventDispatcherCheck(
                                 currentThread,
                                 [jthreadObjectWrapper](QThread* thread, const QList<std::function<void()>>& finalActions) -> QObjectUserData* {
                                      struct ThreadDetacher : public QObjectUserData {
                                          JObjectWrapper m_jthreadObjectWrapper;
                                          QThread* m_thread;
                                          QList<std::function<void()>> m_finalActions;
                                          ThreadDetacher(const JObjectWrapper& jthreadObjectWrapper,
                                                         QThread* thread,
                                                         const QList<std::function<void()>>& finalActions)
                                              : QObjectUserData(),
                                                m_jthreadObjectWrapper(jthreadObjectWrapper),
                                                m_thread(thread),
                                                m_finalActions(finalActions)
                                          {
                                              QTJAMBI_DEBUG_METHOD_PRINT("native", "ThreadDetacher::ThreadDetacher()")
                                          }
                                          ~ThreadDetacher(){
                                              QTJAMBI_DEBUG_METHOD_PRINT("native", "ThreadDetacher::~ThreadDetacher()")
                                              JavaVM *vm = nullptr;
                                              if(!qtjambi_vm_lock.isDestroyed()){
                                                  QReadLocker locker(qtjambi_vm_lock());
                                                  Q_UNUSED(locker)
                                                  vm = qtjambi_vm;
                                              }
                                              if (vm) {
                                                   QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(m_thread);
                                                   if(link || m_jthreadObjectWrapper.object()){
                                                         JNIEnv* env = nullptr;
                                                         int result = vm->GetEnv( reinterpret_cast<void **>(&env), qtjambi_jni_version);
                                                         if(result==JNI_OK &&  env){
                                                             QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                             Q_ASSERT(link->isQObject());
                                                             if(!link->qobject() || !link->qobject()->parent())
                                                                link->setJavaOwnership(env);
                                                             jobject javaThread = env->NewLocalRef(m_jthreadObjectWrapper.object());
                                                             m_jthreadObjectWrapper = JObjectWrapper();
                                                             if(javaThread){
                                                                 try{
                                                                     Java::Private::QtJambi::QtJambiInternal.setThreadInterruptible(env, javaThread, nullptr);
                                                                 } catch (const JavaException& exn) {
                                                                     exn.report(env);
                                                                 }
                                                                 try {
                                                                     Java::Private::QtJambi::QtJambiInternal.deleteAssociation(env, javaThread);
                                                                 } catch (const JavaException& exn) {
                                                                     exn.report(env);
                                                                 }
                                                             }
                                                         }
                                                   }
                                                   while(!m_finalActions.isEmpty()){
                                                       m_finalActions.takeFirst()();
                                                   }
                                                   if(QThreadUserData* data = static_cast<QThreadUserData*>(m_thread->userData(QThreadUserData::id()))){
                                                       m_thread->setUserData(QThreadUserData::id(), nullptr);
                                                       delete data;
                                                   }
                                                   vm->DetachCurrentThread();
                                              }else{
                                                  while(!m_finalActions.isEmpty()){
                                                      m_finalActions.takeFirst()();
                                                  }
                                                  if(QThreadUserData* data = static_cast<QThreadUserData*>(m_thread->userData(QThreadUserData::id()))){
                                                      m_thread->setUserData(QThreadUserData::id(), nullptr);
                                                      delete data;
                                                  }
                                              }
                                          }
                                      };
                                      return new ThreadDetacher(jthreadObjectWrapper, thread, finalActions);
                                  })
                            ));
                    }
                }else{
                    env->DeleteLocalRef(jthrd);
                }
                if(env->GetObjectRefType(qthreadObject)==jobjectRefType::JNILocalRefType){
                    env->DeleteLocalRef(qthreadObject);
                    qthreadObject = nullptr;
                }
            }
            env->DeleteLocalRef(jthread);
            jthread = nullptr;
        }
        threadData = static_cast<QThreadUserData*>(currentThread->userData(QThreadUserData::id()));
        if(!threadData){
            currentThread->setUserData(QThreadUserData::id(), threadData = new QThreadUserData());
        }
#endif
    } else {
        Q_ASSERT(result == JNI_OK);
    }
    if(env && !env->functions)
        env = nullptr;
    return env;
}

/*!
 * Fetches the current environment based on the global Virtual
 * Machine. This function will fail if the current thread is not
 * attached, but that should never happen.
 */
JNIEnv *qtjambi_current_environment(bool initializeJavaThread)
{
    JavaVM *vm = nullptr;
    if(!qtjambi_vm_lock.isDestroyed()){
        QReadLocker locker(qtjambi_vm_lock());
        Q_UNUSED(locker)
        vm = qtjambi_vm;
    }
    return qtjambi_current_environment(initializeJavaThread, vm);
}

JNIEnv *qtjambi_current_environment()
{
    return qtjambi_current_environment(true);
}


void * qtjambi_get_placement(JNIEnv *env, jclass callingClass, jobject placement_object)
{
    Q_UNUSED(callingClass)
    Q_ASSERT(env);
    if(!placement_object)
        JavaException::raiseNullPointerException(env, "QDeclarativeConstructor is null." QTJAMBI_STACKTRACEINFO );
    jclass cls = Java::QtCore::QObject$QDeclarativeConstructor.cls(env, placement_object);
    void* placement = reinterpret_cast<void*>(Java::QtCore::QObject$QDeclarativeConstructor.placement(env, placement_object));
    if(!env->IsAssignableFrom(cls, callingClass) || !placement){
        JavaException::raiseIllegalAccessException(env, "Illegal use of QDeclarativeConstructor parameter." QTJAMBI_STACKTRACEINFO );
    }
    Java::QtCore::QObject$QDeclarativeConstructor.set_placement(env, placement_object, jlong(0));
    return placement;
}

QString qtjambi_class_name(JNIEnv *env, jclass java_class)
{
    Q_ASSERT(java_class);
    Q_ASSERT(Java::Private::Runtime::Class.isInstanceOf(env, java_class));  // check the java object is right type
    jstring name = Java::Private::Runtime::Class.getName(env,java_class);
    return qtjambi_to_qstring(env, name);
}

QString qtjambi_object_class_name(JNIEnv *env, jobject java_object)
{
    Q_ASSERT(java_object);
    return qtjambi_class_name(env, env->GetObjectClass(java_object));
}

jobject qtjambi_from_qvariant(JNIEnv *env, const QVariant &qt_variant)
{
    switch (qt_variant.userType()) {
    case QVariant::Invalid: return nullptr;
    case QVariant::Int:
    case QVariant::UInt:
    {
        return Java::Runtime::Integer.valueOf(env, qt_variant.toInt());
    }
    case QVariant::Char:
    {
        return Java::Runtime::Character.valueOf(env, jchar(qt_variant.toChar().unicode()));
    }
    case QVariant::Double:
    {
        return Java::Runtime::Double.valueOf(env, qt_variant.toDouble());
    }
    case QVariant::String:
        return qtjambi_from_qstring(env, qt_variant.toString());
    case QVariant::LongLong:
    case QVariant::ULongLong:
    {
        return Java::Runtime::Long.valueOf(env, qt_variant.toLongLong());
    }
    case QVariant::Bool:
    {
        return Java::Runtime::Boolean.valueOf(env, qt_variant.toBool());
    }
    case QVariant::List:
    {
        jobject obj = Java::Private::Runtime::ArrayList.newInstance(env, jint(qt_variant.toList().size()));
        if(obj){
            for(const QVariant& entry_variant : qt_variant.toList()){
                Java::Private::Runtime::Collection.add(env, obj, qtjambi_from_qvariant(env, entry_variant));
            }
        }
        return obj;
    }
    case QVariant::StringList:
    {
        jobject obj = Java::Private::Runtime::ArrayList.newInstance(env, jint(qt_variant.toStringList().size()));
        if(obj){
            for(const QString& entry_string : qt_variant.toStringList()){
                Java::Private::Runtime::Collection.add(env, obj, qtjambi_from_qstring(env, entry_string));
            }
        }
        return obj;
    }
    case QVariant::Map:
    {
        jobject obj = Java::Private::Runtime::TreeMap.newInstance(env);
        if(obj){
            QMap<QString, QVariant> map = qt_variant.toMap();
            for(const QString& key : map.keys()){
                qtjambi_map_put(env, obj, qtjambi_from_qstring(env, key), qtjambi_from_qvariant(env, map.value(key)));
            }
        }
        return obj;
    }
    case QVariant::Hash:
    {
        jobject obj = Java::Private::Runtime::HashMap.newInstance(env, jint(qt_variant.toHash().size()));
        if(obj){
            QHash<QString, QVariant> map = qt_variant.toHash();
            for(const QString& key : map.keys()){
                qtjambi_map_put(env, obj, qtjambi_from_qstring(env, key), qtjambi_from_qvariant(env, map.value(key)));
            }
        }
        return obj;
    }
    default:
        break;
    }

    // generic java object
    if (qt_variant.userType() == qMetaTypeId<qint8>()
            || qt_variant.userType() == qMetaTypeId<quint8>()) {
        return Java::Runtime::Byte.valueOf(env, jbyte(qt_variant.value<qint8>()));
    }else if (qt_variant.userType() == qMetaTypeId<qint16>()
            || qt_variant.userType() == qMetaTypeId<quint16>()) {
        return Java::Runtime::Short.valueOf(env, jshort(qt_variant.value<qint16>()));
    }else if (qt_variant.userType() == qMetaTypeId<float>()) {
        return Java::Runtime::Float.valueOf(env, qt_variant.value<float>());
    }else if (qt_variant.userType() == qMetaTypeId<JObjectWrapper>()) {
#if QT_VERSION < 0x050000
        JObjectWrapper wrapper = qVariantValue<JObjectWrapper>(qt_variant);
#else
		JObjectWrapper wrapper = qt_variant.value<JObjectWrapper>();
#endif
        return env->NewLocalRef(wrapper.object());
    } else if (qt_variant.userType() == qMetaTypeId<JEnumWrapper>()) {
#if QT_VERSION < 0x050000
        JEnumWrapper wrapper = qVariantValue<JEnumWrapper>(qt_variant);
#else
        JEnumWrapper wrapper = qt_variant.value<JEnumWrapper>();
#endif
        return env->NewLocalRef(wrapper.object());
    } else if (qt_variant.userType() == qMetaTypeId<JCollectionWrapper>()
               || qt_variant.userType() == qMetaTypeId<JMapWrapper>()
               || qt_variant.userType() == qMetaTypeId<JIteratorWrapper>()) {
#if QT_VERSION < 0x050000
        JCollectionWrapper wrapper = qVariantValue<JCollectionWrapper>(qt_variant);
#else
        JCollectionWrapper wrapper = qt_variant.value<JCollectionWrapper>();
#endif
        return env->NewLocalRef(wrapper.object());
    } else if (qt_variant.userType() == qMetaTypeId<JMapWrapper>()) {
#if QT_VERSION < 0x050000
         JMapWrapper wrapper = qVariantValue<JMapWrapper>(qt_variant);
#else
         JMapWrapper wrapper = qt_variant.value<JMapWrapper>();
#endif
         return env->NewLocalRef(wrapper.object());
    } else if (qt_variant.userType() == qMetaTypeId<JIteratorWrapper>()) {
 #if QT_VERSION < 0x050000
         JIteratorWrapper wrapper = qVariantValue<JIteratorWrapper>(qt_variant);
 #else
         JIteratorWrapper wrapper = qt_variant.value<JIteratorWrapper>();
 #endif
         return env->NewLocalRef(wrapper.object());
     } else {
        QString qtType = QLatin1String(qt_variant.typeName());
        int metaType = qt_variant.type() == QVariant::UserType ? qt_variant.userType() : int(qt_variant.type());
        QString javaType = QtJambiTypeManager::getExternalTypeName(env, qtType, QtJambiTypeManager::InstantiationType, metaType);
        if(javaType.isEmpty()){
            QString _qtType = QLatin1String(QMetaType::typeName(QMetaType::type(qPrintable(qtType))));
            if(!_qtType.isEmpty() && _qtType!=qtType){
                qtType = _qtType;
                javaType = QtJambiTypeManager::getExternalTypeName(env, qtType, QtJambiTypeManager::InstantiationType, metaType);
            }
        }
        if(javaType.isEmpty() && metaType!=QMetaType::UnknownType && metaType!=QMetaType::Void && metaType!=QMetaType::Nullptr){
            if(qtType.endsWith("*")){
                javaType = QLatin1String("io/qt/QNativePointer");
                if(QMetaType(metaType).flags() & QMetaType::PointerToQObject
                        || QMetaType(metaType).flags() & QMetaType::SharedPointerToQObject
                        || QMetaType(metaType).flags() & QMetaType::WeakPointerToQObject){
                    javaType = QLatin1String("io/qt/core/QObject");
                }else if(QMetaType(metaType).flags() & QMetaType::PointerToGadget){
                     javaType = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                }
            }else{
                javaType = QLatin1String("io/qt/core/QMetaType$GenericValue");
            }
        }
        jobject classLoader = nullptr;

        // Find usage pattern
        QtJambiTypeManager::TypePattern internalType = QtJambiTypeManager::typeIdOfInternal(env, javaType, qtType, metaType, classLoader);

        InternalToExternalConverter converter = QtJambiTypeManager::getInternalToExternalConverter(
                                                env,
                                                qtType,
                                                internalType,
                                                metaType,
                                                resolveClass(env, qPrintable(javaType)),
                                                internalType==QtJambiTypeManager::NativePointer);
        jvalue val;
        val.l = nullptr;
        if(!converter || !converter(env, nullptr, qt_variant.constData(), &val, true)){
            qWarning("Unable to convert qVariant to jobject");
        }
        return val.l;
    }
}

void registerLambdaClass(JNIEnv *env, jclass lambdaClass, const char *className);

jclass resolveLambdaInterface(JNIEnv* env, jclass lambdaClass);

QVariant qtjambi_to_qvariant(JNIEnv *env, jobject java_object, bool convert)
{
    if (!java_object)
        return QVariant(QMetaType::Nullptr, nullptr);

    jclass object_class = env->GetObjectClass(java_object);
    if (!object_class)
        return QVariant();

    // Test some quick ones first...
    if (Java::Private::QtCore::QMetaType$GenericEnumerator.isSameClass(env, object_class)) {
        int internalMetaType = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, java_object);
        if(!convert)
            return QVariant(internalMetaType, nullptr);
        int value = qtjambi_to_enumerator(env, java_object).value<int>();
        return QVariant(internalMetaType, &value);
    } else if (Java::Private::QtCore::QMetaType$GenericByteEnumerator.isSameClass(env, object_class)) {
        int internalMetaType = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, java_object);
        if(!convert)
            return QVariant(internalMetaType, nullptr);
        qint8 value = qtjambi_to_enumerator(env, java_object).value<qint8>();
        return QVariant(internalMetaType, &value);
    } else if (Java::Private::QtCore::QMetaType$GenericShortEnumerator.isSameClass(env, object_class)) {
        int internalMetaType = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, java_object);
        if(!convert)
            return QVariant(internalMetaType, nullptr);
        qint16 value = qtjambi_to_enumerator(env, java_object).value<qint16>();
        return QVariant(internalMetaType, &value);
    } else if (Java::Private::QtCore::QMetaType$GenericLongEnumerator.isSameClass(env, object_class)) {
        int internalMetaType = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, java_object);
        if(!convert)
            return QVariant(internalMetaType, nullptr);
        qint64 value = qtjambi_to_enumerator(env, java_object).value<qint64>();
        return QVariant(internalMetaType, &value);
    } else if (Java::Private::QtCore::QMetaType$GenericFlags.isSameClass(env, object_class)) {
        int internalMetaType = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, java_object);
        if(!convert)
            return QVariant(internalMetaType, nullptr);
        int value = qtjambi_to_enum(env, java_object);
        return QVariant(internalMetaType, &value);
    } else if (Java::Private::QtCore::QMetaType$GenericValue.isSameClass(env, object_class)) {
        int internalMetaType = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, java_object);
        if(!convert)
            return QVariant(internalMetaType, nullptr);
        return QVariant(internalMetaType, qtjambi_to_object(env, java_object));
    } else if (Java::Private::QtCore::QMetaType$GenericGadget.isSameClass(env, object_class)) {
        int internalMetaType = Java::Private::QtCore::QMetaType$GenericTypeInterface.metaType(env, java_object);
        if(!convert)
            return QVariant(internalMetaType, nullptr);
        return QVariant(internalMetaType, qtjambi_to_object(env, java_object));
    } else if (Java::Runtime::String.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::QString, nullptr);
        return qtjambi_to_qstring(env, static_cast<jstring>(java_object));
    } else if (Java::Runtime::Integer.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::Int, nullptr);
        qint32 i = Java::Runtime::Number.intValue(env,java_object);
        return QVariant::fromValue<qint32>(i);
    } else if (Java::Runtime::Character.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::QChar, nullptr);
        ushort s = ushort(Java::Runtime::Character.charValue(env, java_object));
        return QVariant::fromValue<QChar>(QChar(s));
    } else if (Java::Runtime::Double.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::Double, nullptr);
        double d = Java::Runtime::Number.doubleValue(env, java_object);
        return QVariant::fromValue<double>(d);
    } else if (Java::Runtime::Float.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::Float, nullptr);
        float f = Java::Runtime::Number.floatValue(env, java_object);
        return QVariant::fromValue<float>(f);
    } else if (Java::Runtime::Short.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::Short, nullptr);
        qint16 i = Java::Runtime::Number.shortValue(env, java_object);
        return QVariant::fromValue<qint16>(i);
    } else if (Java::Runtime::Byte.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::SChar, nullptr);
        qint8 i = Java::Runtime::Number.byteValue(env, java_object);
        return QVariant::fromValue<qint8>(i);
    } else if (Java::Runtime::Long.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::LongLong, nullptr);
        qint64 i = Java::Runtime::Number.longValue(env, java_object);
        return QVariant::fromValue<qint64>(i);
    } else if (Java::Runtime::Boolean.isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(QMetaType::Bool, nullptr);
        bool b = Java::Runtime::Boolean.booleanValue(env, java_object);
        return QVariant::fromValue<bool>(b);
    } else if (Java::Private::Runtime::Collection.isInstanceOf(env, java_object)) {
        if (Java::Private::QtJambi::QtJambiCollectionObject.isInstanceOf(env, java_object)) {
            jclass elementType = Java::Private::QtJambi::QtJambiCollectionObject.elementType(env, java_object);
            jobject object_classLoader = Java::Private::Runtime::Class.getClassLoader(env, elementType);
            QString elementJavaName = qtjambi_class_name(env, elementType).replace(".", "/");
            QString qtName = QtJambiTypeManager::getInternalTypeName(env, elementJavaName, object_classLoader);
            if(!qtName.isEmpty()){
                if (Java::Private::QtJambi::QtJambiListObject.isInstanceOf(env, java_object)) {
                    qtName = QString("QList<%1>").arg(qtName);
                }else if (Java::Private::QtJambi::QtJambiLinkedListObject.isInstanceOf(env, java_object)) {
                    qtName = QString("QLinkedList<%1>").arg(qtName);
                }else if (Java::Private::QtJambi::QtJambiSetObject.isInstanceOf(env, java_object)) {
                    qtName = QString("QSet<%1>").arg(qtName);
                }else if (Java::Private::QtJambi::QtJambiStackObject.isInstanceOf(env, java_object)) {
                    qtName = QString("QStack<%1>").arg(qtName);
                }else if (Java::Private::QtJambi::QtJambiVectorObject.isInstanceOf(env, java_object)) {
                    qtName = QString("QVector<%1>").arg(qtName);
                }else if (Java::Private::QtJambi::QtJambiQueueObject.isInstanceOf(env, java_object)) {
                    qtName = QString("QQueue<%1>").arg(qtName);
                }else{
                    qtName = "";
                }
                if(!qtName.isEmpty()){
                    int type = QMetaType::type(qPrintable(qtName));
                    if ((type == QVariant::Invalid || type == qMetaTypeId<JCollectionWrapper>())
                            && Java::Private::QtJambi::QtJambiListObject.isInstanceOf(env, java_object)) {
                        if(Java::Runtime::String.isSameClass(env, elementType)){
                            qtName = "QStringList";
                            type = QMetaType::QStringList;
                        }else if(Java::Private::Runtime::Object.isSameClass(env, elementType)){
                            qtName = "QVariantList";
                            type = QMetaType::QVariantList;
                        }
                    }
                    if(type == qMetaTypeId<JCollectionWrapper>()){
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else if(type == qMetaTypeId<JObjectWrapper>()){
                        return QVariant::fromValue(JObjectWrapper(env, java_object));
                    }else if (type != QVariant::Invalid) {
                        if(convert){
                            if(void* ptr = qtjambi_to_object(env, java_object))
                                return QVariant(type, ptr);
                        }else{
                            return QVariant(type, nullptr);
                        }
                    }
                }
            }
        }
        if(!convert)
            return QVariant(qMetaTypeId<JCollectionWrapper>(), nullptr);
        return QVariant::fromValue<JCollectionWrapper>(JCollectionWrapper(env, java_object));
    } else if (Java::Private::Runtime::Map.isInstanceOf(env, java_object)) {
        if (Java::Private::QtJambi::QtJambiAbstractMapObject.isInstanceOf(env, java_object)) {
            jclass keyType = Java::Private::QtJambi::QtJambiAbstractMapObject.keyType(env, java_object);
            jclass valueType = Java::Private::QtJambi::QtJambiAbstractMapObject.valueType(env, java_object);
            jobject object_classLoader = Java::Private::Runtime::Class.getClassLoader(env, keyType);
            QString keyJavaName = qtjambi_class_name(env, keyType).replace(".", "/");
            QString keyQtName = QtJambiTypeManager::getInternalTypeName(env, keyJavaName, object_classLoader);
            if(!keyQtName.isEmpty()){
                object_classLoader = Java::Private::Runtime::Class.getClassLoader(env, valueType);
                QString valueJavaName = qtjambi_class_name(env, valueType).replace(".", "/");
                QString valueQtName = QtJambiTypeManager::getInternalTypeName(env, valueJavaName, object_classLoader);
                if(!valueQtName.isEmpty()){
                    QString qtName;
                    if (Java::Private::QtJambi::QtJambiMapObject.isInstanceOf(env, java_object)) {
                        qtName = QString("QMap<%1,%2>").arg(keyQtName).arg(valueQtName);
                    }else if (Java::Private::QtJambi::QtJambiHashObject.isInstanceOf(env, java_object)) {
                        qtName = QString("QHash<%1,%2>").arg(keyQtName).arg(valueQtName);
                    }
                    if(!qtName.isEmpty()){
                        int type = QMetaType::type(qPrintable(qtName));
                        if (type == QVariant::Invalid || type == qMetaTypeId<JMapWrapper>()) {
                            if(Java::Runtime::String.isSameClass(env, valueType)
                                     && Java::Private::Runtime::Object.isSameClass(env, valueType)){
                                if(Java::Private::QtJambi::QtJambiMapObject.isInstanceOf(env, java_object)){
                                    qtName = "QVariantMap";
                                    type = QMetaType::QVariantMap;
                                }else{
                                    qtName = "QVariantHash";
                                    type = QMetaType::QVariantHash;
                                }
                            }
                        }
                        if(type == qMetaTypeId<JMapWrapper>()){
                            return QVariant::fromValue(JMapWrapper(env, java_object));
                        }else if(type == qMetaTypeId<JObjectWrapper>()){
                            return QVariant::fromValue(JObjectWrapper(env, java_object));
                        }else if (type != QVariant::Invalid) {
                            if(convert){
                                if(void* ptr = qtjambi_to_object(env, java_object))
                                    return QVariant(type, ptr);
                            }else{
                                return QVariant(type, nullptr);
                            }
                        }
                    }
                }
            }
        }else if (Java::Private::QtJambi::QtJambiAbstractMultiMapObject.isInstanceOf(env, java_object)) {
            jclass keyType = Java::Private::QtJambi::QtJambiAbstractMultiMapObject.keyType(env, java_object);
            jclass valueType = Java::Private::QtJambi::QtJambiAbstractMultiMapObject.valueType(env, java_object);
            jobject object_classLoader = Java::Private::Runtime::Class.getClassLoader(env, keyType);
            QString keyJavaName = qtjambi_class_name(env, keyType).replace(".", "/");
            QString keyQtName = QtJambiTypeManager::getInternalTypeName(env, keyJavaName, object_classLoader);
            if(!keyQtName.isEmpty()){
                object_classLoader = Java::Private::Runtime::Class.getClassLoader(env, valueType);
                QString valueJavaName = qtjambi_class_name(env, valueType).replace(".", "/");
                QString valueQtName = QtJambiTypeManager::getInternalTypeName(env, valueJavaName, object_classLoader);
                if(!valueQtName.isEmpty()){
                    QString qtName;
                    if (Java::Private::QtJambi::QtJambiMultiMapObject.isInstanceOf(env, java_object)) {
                        qtName = QString("QMultiMap<%1,%2>").arg(keyQtName).arg(valueQtName);
                    }else if (Java::Private::QtJambi::QtJambiMultiHashObject.isInstanceOf(env, java_object)) {
                        qtName = QString("QMultiHash<%1,%2>").arg(keyQtName).arg(valueQtName);
                    }
                    if(!qtName.isEmpty()){
                        int type = QMetaType::type(qPrintable(qtName));
                        if(type == qMetaTypeId<JMapWrapper>()){
                            return QVariant::fromValue(JMapWrapper(env, java_object));
                        }else if(type == qMetaTypeId<JObjectWrapper>()){
                            return QVariant::fromValue(JObjectWrapper(env, java_object));
                        }else if (type != QVariant::Invalid) {
                            if(convert){
                                if(void* ptr = qtjambi_to_object(env, java_object))
                                    return QVariant(type, ptr);
                            }else{
                                return QVariant(type, nullptr);
                            }
                        }
                    }
                }
            }
        }
        if(!convert)
            return QVariant(qMetaTypeId<JMapWrapper>(), nullptr);
        return QVariant::fromValue<JMapWrapper>(JMapWrapper(env, java_object));
    } else if (Java::Private::Runtime::Iterator.isInstanceOf(env, java_object)) {
        if(!convert)
            return QVariant(qMetaTypeId<JIteratorWrapper>(), nullptr);
        return QVariant::fromValue<JIteratorWrapper>(JIteratorWrapper(env, java_object));
    }
    jobject object_classLoader = Java::Private::Runtime::Class.getClassLoader(env, object_class);
    // Do the slightly slower fallback...
    QString fullJavaName = qtjambi_class_name(env, object_class);
    {
        QString strClassName;
        QString strPackage;
        QtJambiTypeManager::splitClassName(strClassName, strPackage, fullJavaName, QLatin1Char('.'));
        if(strClassName.contains("$Lambda$") && strClassName.contains("/")){
            registerLambdaClass(env, object_class, qPrintable(fullJavaName.replace(".", "/")));
            object_class = resolveLambdaInterface(env, object_class);
            fullJavaName = qtjambi_class_name(env, object_class);
            QtJambiTypeManager::splitClassName(strClassName, strPackage, fullJavaName, QLatin1Char('.'));
        }
    }
    fullJavaName = fullJavaName.replace(".", "/");

    // may it allow enum and flags?
    QString qtName = QtJambiTypeManager::getInternalTypeName(env, fullJavaName, object_classLoader);
    if(qtName=="JEnumWrapper"){
        if(!convert)
            return QVariant(qMetaTypeId<JEnumWrapper>(), nullptr);
        return QVariant::fromValue<JEnumWrapper>(JEnumWrapper(env, java_object));
    }
    int type = !qtName.isEmpty()
        ? QMetaType::type(qPrintable(qtName))
        : int(QVariant::Invalid);

    if (type != QVariant::Invalid) {
        if(convert){
            if(qtName.endsWith("*")){
                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                    if(link->ownership()==QtJambiLink::Ownership::Java){
                        return QVariant::fromValue<JObjectWrapper>(JObjectWrapper(env, java_object));
                    }
                }
            }
            jvalue val;
            val.l = java_object;
            QtJambiTypeManager::TypePattern internalType = QtJambiTypeManager::typeIdOfInternal(env, fullJavaName, qtName, type, object_classLoader);
            const ExternalToInternalConverter& converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, internalType, qtName, type);
            void* ptr = nullptr;
            QtJambiScope scope;
            ConvertResponse result = converter(env, &scope, val, ptr, jValueType::l);
            switch(result){
            case ConvertResponse::NotSuccessful:
                qWarning("Cannot convert external type '%s' to '%s'",
                         qPrintable(fullJavaName.replace("/", ".").replace("$", ".")),
                         qPrintable(qtName));
                type = QVariant::Invalid;
                break;
            case ConvertResponse::JavaObjectNotLinked:
                qWarning("Java object of type '%s' not linked to C++ object",
                         qPrintable(fullJavaName.replace("/", ".").replace("$", ".")));
                type = QVariant::Invalid;
                break;
            default:
                return QVariant(type, ptr);
            }
        }
        return QVariant(type, nullptr);
    }else{
        if(!convert)
            return QVariant(qMetaTypeId<JObjectWrapper>(), nullptr);
        else
            return QVariant::fromValue<JObjectWrapper>(JObjectWrapper(env, java_object));
    }
}

QVariant qtjambi_to_qvariant(JNIEnv *env, jobject java_object)
{
    return qtjambi_to_qvariant(env, java_object, true);
}

bool qtjambi_convert_to_native(JNIEnv *env, const std::type_info& typeId, jobject java_object, void * output)
{
    if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue java_value;
        java_value.l = java_object;
        return typeEntry->convertToNative(env, java_value, jValueType::l, output);
    }else return false;
}

bool qtjambi_convert_to_native(JNIEnv *env, const std::type_info& typeId, const char* qtName, const char* javaName, jobject java_object, void * output)
{
    const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, qtName);
    if(!qtName && javaName && typeEntry->isFunctional()){
        if(const std::type_info* typeId = getTypeByJavaName(javaName))
            qtName = getQtName(*typeId);
        if(qtName)
            typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, qtName);
    }
    if(typeEntry){
        jvalue java_value;
        java_value.l = java_object;
        return typeEntry->convertToNative(env, java_value, jValueType::l, output);
    }else return false;
}

bool qtjambi_convert_from_native(JNIEnv *env, const std::type_info& typeId, const char* qtName, const char* javaName, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jobject& output)
{
    const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, qtName);
    if(!qtName && javaName && typeEntry->isFunctional()){
        if(const std::type_info* typeId = getTypeByJavaName(javaName))
            qtName = getQtName(*typeId);
        if(qtName)
            typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, qtName);
    }
    if(typeEntry){
        jvalue jv;
        jv.l = nullptr;
        bool result = typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, &jv, jValueType::l);
        if(result)
            output = jv.l;
        return result;
    }else return false;
}

bool qtjambi_convert_to_native(JNIEnv *env, const std::type_info& typeId, const char* typeName, jobject java_object, void * output)
{
    const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, typeName);
    if(!typeEntry){
        if(typeName){
            if(const std::type_info* typeId = getTypeByJavaName(typeName)){
                typeName = getQtName(*typeId);
            }
            if(typeName)
                typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, typeName);
        }
    }
    if(typeEntry){
        jvalue java_value;
        java_value.l = java_object;
        return typeEntry->convertToNative(env, java_value, jValueType::l, output);
    }else return false;
}

bool qtjambi_convert_from_native(JNIEnv *env, const std::type_info& typeId, const char* typeName, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jobject& output)
{
    const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, typeName);
    if(!typeEntry){
        if(typeName){
            if(const std::type_info* typeId = getTypeByJavaName(typeName)){
                typeName = getQtName(*typeId);
            }
            if(typeName)
                typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId, typeName);
        }
    }
    if(typeEntry){
        jvalue jv;
        jv.l = nullptr;
        bool result = typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, &jv, jValueType::l);
        if(result)
            output = jv.l;
        return result;
    }else return false;
}

void *qtjambi_to_object(JNIEnv *env, jobject java_object)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, java_object)){
        return link->pointer();
    }else return nullptr;
}

QObject *qtjambi_to_qobject(JNIEnv *env, jobject java_object)
{
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, java_object);
    if(link){
        return link->isQObject() ? link->qobject() : nullptr;
    }else return nullptr;
}

struct QtJambiShellAccess : QtJambiShellImpl{
    void overrideLink(const QSharedPointer<QtJambiLink>& link){
        QtJambiShellImpl::overrideLink(link);
    }
};

const void *qtjambi_to_qobject_from_shared_pointer(JNIEnv *env, jobject java_object, PointerCreator pointerCreator, PointerDeleter pointerDeleter, PointerQObjectGetter pointerGetter)
{
    if(java_object){
        QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object);
        if(link){
            Q_ASSERT(link->isQObject());
            if(link->isSharedPointer()){
                QSharedPointerLink* pointerLink = static_cast<QSharedPointerLink*>(link.data());
                return pointerLink->getSharedPointer();
            }else{
                void* ptr = link->pointer();
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(java_object));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(ptr) + infos.at(0).size );
                }

                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::Private::QtJambi::QtJambiInternal$NativeLink.reset(env, nativeLink);
                }

                link->invalidate(env);
                link = QSharedPointer<QtJambiLink>();
                link = QtJambiLink::createLinkForSharedPointerToQObject(env, java_object, createdByJava, is_shell, pointerCreator(ptr), pointerDeleter, pointerGetter);
                Q_ASSERT(link && link->isSharedPointer());
                if(shell){
                    shell->overrideLink(link);
                }
                return static_cast<QSharedPointerLink*>(link.data())->getSharedPointer();
            }
        }
    }
    // this is a memory leak because this is not linked to java object!!!
    return nullptr;
}

const void *qtjambi_to_object_from_shared_pointer(JNIEnv *env, jobject java_object, PointerCreator pointerCreator, PointerDeleter pointerDeleter, PointerGetter pointerGetter)
{
    if(java_object){
        QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object);
        if(link){
            Q_ASSERT(!link->isQObject());
            if(link->isSharedPointer()){
                return static_cast<QSharedPointerLink*>(link.data())->getSharedPointer();
            }else if(!link->isQObject()){
                void* ptr = link->pointer();
                bool createdByJava = link->createdByJava();
                bool is_shell = link->isShell();
                QtJambiShellAccess* shell = nullptr;
                if(is_shell){
                    const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(java_object));
                    Q_ASSERT(infos.size());
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(ptr) + infos.at(0).size );
                }
                int metaType = link->metaType();
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                const char* className = link->qtTypeName();
#endif
                PtrDeleterFunction deleter = nullptr;
                PtrOwnerFunction registeredOwnerFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    deleter = polink->destructorFunction();
                    registeredOwnerFunction = polink->ownerFunction();
                }

                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::Private::QtJambi::QtJambiInternal$NativeLink.reset(env, nativeLink);
                }

                link->invalidate(env);
                link = QSharedPointer<QtJambiLink>();
                link = QtJambiLink::createLinkForSharedPointerToObject(env,
                                                                           java_object,
                                                                           metaType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                           className,
#endif
                                                                           createdByJava,
                                                                           is_shell,
                                                                           deleter,
                                                                           registeredOwnerFunction,
                                                                           pointerCreator(ptr),
                                                                           pointerDeleter,
                                                                           pointerGetter);
                if(shell){
                    shell->overrideLink(link);
                }
                Q_ASSERT(link && link->isSharedPointer());
                return static_cast<QSharedPointerLink*>(link.data())->getSharedPointer();
            }
        }
    }
    // this is a memory leak because this is not linked to java object!!!
    return nullptr;
}

int qtjambi_to_enum(JNIEnv *env, jobject java_object)
{
    int returned = 0;
    Q_ASSERT(java_object);
    if(Java::Runtime::Number.isInstanceOf(env, java_object)){
        returned = qtjambi_to_int(env, java_object);
    }else if(Java::Private::QtJambi::QFlags.isInstanceOf(env, java_object)){
        returned = Java::Private::QtJambi::QFlags.value(env, java_object);
    }else if(Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, java_object)){
        returned = Java::Private::QtJambi::QtEnumerator.value(env,java_object);
    }else if(Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, java_object)){
        returned = Java::Private::QtJambi::QtByteEnumerator.value(env, java_object);
    }else if(Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, java_object)){
        returned = Java::Private::QtJambi::QtShortEnumerator.value(env, java_object);
    }else if(Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, java_object)){
        returned = int(Java::Private::QtJambi::QtLongEnumerator.value(env, java_object));
    }else if(Java::Private::Runtime::Enum.isInstanceOf(env, java_object)){
        returned = Java::Private::Runtime::Enum.ordinal(env, java_object);
    }
    return returned;
}

/* forward declare */
void *qtjambi_to_interface_impl(JNIEnv *env, jobject object, const std::type_info& typeId);

void *qtjambi_to_interface(JNIEnv *env, jobject object,
                                  const char *, const std::type_info& typeId)
{
    return qtjambi_to_interface_impl(env, object, typeId);
}

void *qtjambi_to_interface(JNIEnv *env, jobject object, const std::type_info& typeId){
    return qtjambi_to_interface_impl(env, object, typeId);
}

void *qtjambi_to_interface_impl(JNIEnv *env, jobject object, const std::type_info& typeId){
    if(env->IsSameObject(object, nullptr))
        return nullptr;
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object);
    if(link){
        return link->typedPointer(typeId);
    }else return nullptr;
}

jobject qtjambi_from_object_impl(JNIEnv *env, const void *qt_object, const std::type_info& typeId, bool makeCopyOfValueTypes, bool cppOwnership, bool *ok = nullptr)
{
    if(ok) *ok = true;
    if (!qt_object)
        return nullptr;
    if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue result;
        result.l = nullptr;
        if(typeEntry->convertToJava(env, qt_object, makeCopyOfValueTypes, cppOwnership, &result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

jobject qtjambi_from_object_no_typeId(JNIEnv *env, const void *qt_object, QByteArray className, jclass clazz, bool makeCopyOfValueTypes, bool cppOwnership)
{
    if (!qt_object || !clazz)
        return nullptr;
    int metaType = QMetaType::UnknownType;
    if(const std::type_info* typeId = getTypeByJavaName(className.data())){
        metaType = registeredMetaTypeID(*typeId);
        if(metaType==QMetaType::UnknownType){
            metaType = QMetaType::type(getQtName(*typeId));
        }
    }

    jobject returned = nullptr;
    if (clazz) {
        if(Java::Private::QtJambi::QtJambiObject.isAssignableFrom(env, clazz)){
            jmethodID constructorId = findInternalPrivateConstructor(env, clazz);
            if(constructorId){
                returned = env->NewObject(clazz, constructorId, 0);
                qtjambi_throw_java_exception(env)
            }else{
                return nullptr;
            }
        }
    }

    if (!returned)
        return nullptr;

    // If it's not a value type, we just link to the pointer directly.
    void *copy = nullptr;
    if (!makeCopyOfValueTypes || metaType == QMetaType::UnknownType) {
        // If the object is constructed in Java, then we can look it up
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(qt_object).toStrongRef()){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                link->invalidate(env);
            }else{
                return obj;
            }
        }

        // Otherwise we have to create it
        copy = const_cast<void *>(qt_object);
    } else {
        copy = QMetaType::create(metaType, qt_object);
        if (!copy)
            return nullptr;
    }

    if (const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                returned,
                copy,
                metaType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                QMetaType::typeName(metaType),
#endif
                false,
                false,
                nullptr,
                nullptr))
    {
        // If the type is copied in, we own the pointer
        if(cppOwnership){
            link->setCppOwnership(env);
        }
        else if (metaType != QMetaType::UnknownType && makeCopyOfValueTypes){
            link->setJavaOwnership(env);
        }
    } else {
        if (metaType != QMetaType::UnknownType && copy)
            QMetaType::destroy(metaType, copy);
        returned = nullptr;
    }

    return returned;
}

jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, const char *className, bool makeCopyOfValueTypes, bool cppOwnership){
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId){
        bool ok;
        jobject result = qtjambi_from_object_impl(env, qt_object, *typeId, makeCopyOfValueTypes, cppOwnership, &ok);
        if(ok)
            return result;
    }
    return qtjambi_from_object_no_typeId(env, qt_object, className, resolveClass(env, className), makeCopyOfValueTypes, cppOwnership);
}

jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, const char *className,
                            const std::type_info& typeId, bool makeCopyOfValueTypes, bool cppOwnership){
    bool ok;
    jobject result = qtjambi_from_object_impl(env, qt_object, typeId, makeCopyOfValueTypes, cppOwnership, &ok);
    if(ok)
        return result;
    return qtjambi_from_object_no_typeId(env, qt_object, className, resolveClass(env, className), makeCopyOfValueTypes, cppOwnership);
}

jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, jclass clazz, bool makeCopyOfValueTypes, bool cppOwnership){
    QString java_name = qtjambi_class_name(env, clazz).replace(QLatin1Char('.'), QLatin1Char('/'));
    const std::type_info* typeId = getTypeByJavaName(java_name);
    if(typeId){
        bool ok;
        jobject result = qtjambi_from_object_impl(env, qt_object, *typeId, makeCopyOfValueTypes, cppOwnership, &ok);
        if(ok)
            return result;
    }
    return qtjambi_from_object_no_typeId(env, qt_object, qPrintable(java_name), clazz, makeCopyOfValueTypes, cppOwnership);
}

jobject qtjambi_from_polymorphic_object_notype(JNIEnv *env, const void *qt_object, const char *className,
                            const std::type_info& polymorphicBaseType, bool makeCopyOfValueTypes, bool cppOwnership)
{
    const char *class_name = nullptr;
    bool isQObject = false;
    const std::type_info* _typeId = nullptr;
    qtjambi_resolve_polymorphic_id(polymorphicBaseType, qt_object, class_name, _typeId, isQObject);
    if(class_name){
        if(_typeId){
            bool ok;
            jobject result = qtjambi_from_object_impl(env, qt_object, *_typeId, makeCopyOfValueTypes, cppOwnership, &ok);
            if(ok)
                return result;
        }
        return qtjambi_from_object_no_typeId(env, qt_object, class_name, resolveClass(env, class_name), makeCopyOfValueTypes, cppOwnership);
    }else{
        return qtjambi_from_object_no_typeId(env, qt_object, class_name, resolveClass(env, className), makeCopyOfValueTypes, cppOwnership);
    }
}

jobject qtjambi_from_polymorphic_object(JNIEnv *env, const void *qt_object, const char *className,
                            const std::type_info& typeId, const std::type_info& polymorphicBaseType, bool makeCopyOfValueTypes, bool cppOwnership)
{
    bool ok;
    jobject result = qtjambi_from_object_impl(env, qt_object, typeId, makeCopyOfValueTypes, cppOwnership, &ok);
    if(ok)
        return result;
    return qtjambi_from_polymorphic_object_notype(env, qt_object, className, polymorphicBaseType, makeCopyOfValueTypes, cppOwnership);
}

jobject qtjambi_from_polymorphic_object(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const std::type_info& polymorphicBaseType,
                                        bool makeCopyOfValueTypes, bool cppOwnership)
{
    bool ok;
    jobject result = qtjambi_from_object_impl(env, qt_object, typeId, makeCopyOfValueTypes, cppOwnership, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    if(!className.isEmpty())
        return qtjambi_from_polymorphic_object_notype(env, qt_object, className, polymorphicBaseType, makeCopyOfValueTypes, cppOwnership);
    return nullptr;
}

jobject qtjambi_from_qobject_notype(JNIEnv *env, const QObject *const_qt_object, QByteArray className, jclass clazz);

jobject qtjambi_from_QMetaObject(JNIEnv *env, const QMetaObject *metaObject);

jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, const std::type_info& typeId, bool makeCopyOfValueTypes, bool cppOwnership){
    bool ok;
    jobject result = qtjambi_from_object_impl(env, qt_object, typeId, makeCopyOfValueTypes, cppOwnership, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    if(!className.isEmpty())
        return qtjambi_from_object_no_typeId(env, qt_object, className, resolveClass(env, className), makeCopyOfValueTypes, cppOwnership);
    return nullptr;
}

jobject qtjambi_from_interface(JNIEnv *env, const void *qt_object, const std::type_info& typeId, bool makeCopyOfValueTypes, bool cppOwnership)
{
    bool ok;
    jobject result = qtjambi_from_object_impl(env, qt_object, typeId, makeCopyOfValueTypes, cppOwnership, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    if(!className.isEmpty())
        return qtjambi_from_object_no_typeId(env, qt_object, className, resolveClass(env, className), makeCopyOfValueTypes, cppOwnership);
    return nullptr;
}

jobject qtjambi_from_interface(JNIEnv *env, const void *qt_object, const char *className, const std::type_info& typeId, bool makeCopyOfValueTypes, bool cppOwnership)
{
    bool ok;
    jobject result = qtjambi_from_object_impl(env, qt_object, typeId, makeCopyOfValueTypes, cppOwnership, &ok);
    if(ok)
        return result;
    return qtjambi_from_object_no_typeId(env, qt_object, className, resolveClass(env, className), makeCopyOfValueTypes, cppOwnership);
}

jobject qtjambi_from_functional_no_typeId(JNIEnv *env, const void *qt_object, const char *className);

jobject qtjambi_from_functional(JNIEnv *env, const void *qt_object, const std::type_info& typeId)
{
    bool ok;
    jobject result = qtjambi_from_object_impl(env, qt_object, typeId, true, false, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    if(!className.isEmpty())
        return qtjambi_from_functional_no_typeId(env, qt_object, className);
    return nullptr;
}

jobject qtjambi_from_functional(JNIEnv *env, const void *qt_object, const char *className)
{
    if (!qt_object)
        return nullptr;
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId)
        return qtjambi_from_functional(env, qt_object, className, *typeId);
    else
        return qtjambi_from_functional_no_typeId(env, qt_object, className);
}

jobject qtjambi_from_functional(JNIEnv *env, const void *qt_object, const char *className, const std::type_info& typeId)
{
    bool ok;
    jobject result = qtjambi_from_object_impl(env, qt_object, typeId, true, false, &ok);
    if(ok)
        return result;
    return qtjambi_from_functional_no_typeId(env, qt_object, className);
}

jobject qtjambi_from_functional_no_typeId(JNIEnv *env, const void *qt_object, const char *className)
{
    if (!qt_object)
        return nullptr;
    jobject returned = nullptr;
    jclass functionalInterfaceClazz = resolveClass(env, className);
    Q_ASSERT(functionalInterfaceClazz);
    QByteArray implClassName = QString("%1$Impl").arg(className).toUtf8();
    jclass implClazz = nullptr;
    try{
        implClazz = resolveClass(env, implClassName);
    }catch(const JavaException&){}
    QByteArray concreteWrapperClassName = implClassName + "$ConcreteWrapper";
    jclass concreteWrapperClazz = nullptr;
    try{
        concreteWrapperClazz = resolveClass(env, concreteWrapperClassName);
    }catch(const JavaException&){}
    if (implClazz && concreteWrapperClazz) {
        int metaType = QMetaType::UnknownType;
        if(const std::type_info* typeId = getTypeByJavaName(className)){
            metaType = registeredMetaTypeID(*typeId);
            if(metaType==QMetaType::UnknownType){
                metaType = QMetaType::type(getQtName(*typeId));
            }
        }
        if(jmethodID constructorId = findInternalPrivateConstructor(env, concreteWrapperClazz)){
            returned = env->NewObject(concreteWrapperClazz, constructorId, 0);
            qtjambi_throw_java_exception(env)
        }else{
            return nullptr;
        }

        // If it's not a value type, we just link to the pointer directly.
        void *copy = nullptr;
        if (metaType == QMetaType::UnknownType) {
            // If the object is constructed in Java, then we can look it up
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(qt_object).toStrongRef()){
                jobject obj = link->getJavaObjectLocalRef(env);
                if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                    link->invalidate(env);
                }else{
                    return obj;
                }
            }

            // Otherwise we have to create it
            copy = const_cast<void *>(qt_object);
        } else {
            copy = QMetaType::create(metaType, qt_object);
            if (!copy)
                return nullptr;
        }

        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(env,
                                                                                       returned,
                                                                                       copy,
                                                                                       metaType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                       QMetaType::typeName(metaType),
#endif
                                                                                       false,
                                                                                       false,
                                                                                       nullptr,
                                                                                       nullptr
                                                                                    );
        if (link) {
            if (metaType != QMetaType::UnknownType){
                link->setJavaOwnership(env);
            }
        } else {
            if (metaType != QMetaType::UnknownType && copy)
                QMetaType::destroy(metaType, copy);

            returned = nullptr;
        }
    }

    return returned;
}

jobject qtjambi_from_object_shared_pointer_impl(JNIEnv *env, const std::type_info& typeId,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter, bool* ok = nullptr);

jobject qtjambi_from_object_shared_pointer_notype(JNIEnv *env, const char *className,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter);

jobject qtjambi_from_qobject_shared_pointer_impl(JNIEnv *env, const std::type_info& typeId,
                             void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter, bool* ok = nullptr);

jobject qtjambi_from_qobject_shared_pointer_notype(JNIEnv *env, const char *className,
                             void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter);

jobject qtjambi_from_interface_shared_pointer(JNIEnv *env, const char *className, const std::type_info& interfaceType,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object)
        return nullptr;
    bool ok;
    jobject result = qtjambi_from_object_shared_pointer_impl(env, interfaceType, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
    if(ok)
        return result;
    return qtjambi_from_object_shared_pointer_notype(env, className, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
}

jobject qtjambi_from_interface_shared_pointer(JNIEnv *env, const char *className,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter)
{
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId){
        bool ok;
        jobject result = qtjambi_from_object_shared_pointer_impl(env, *typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
        if(ok)
            return result;
    }
    return qtjambi_from_object_shared_pointer_notype(env, className, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
}

jobject qtjambi_from_interface_shared_pointer(JNIEnv *env, const std::type_info& interfaceType,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object)
        return nullptr;
    bool ok;
    jobject result = qtjambi_from_object_shared_pointer_impl(env, interfaceType, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(interfaceType);
    if(!className.isEmpty()){
        return qtjambi_from_object_shared_pointer_notype(env, className, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
    }else{
        return nullptr;
    }
}

jobject qtjambi_from_polymorphic_object_shared_pointer(JNIEnv *env, const char *className,
                            const std::type_info& typeId, const std::type_info& polymorphicBaseType,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object)
        return nullptr;
    bool ok;
    jobject result = qtjambi_from_object_shared_pointer_impl(env, typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
    if(ok)
        return result;
    const char *class_name = nullptr;
    bool isQObject = false;
    const std::type_info* _typeId = nullptr;
    qtjambi_resolve_polymorphic_id(polymorphicBaseType, qt_object, class_name, _typeId, isQObject);
    if(class_name){
        if(_typeId){
            jobject result = qtjambi_from_object_shared_pointer_impl(env, *_typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
            if(ok)
                return result;
        }
        return qtjambi_from_object_shared_pointer_notype(env, class_name, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
    }else{
        return qtjambi_from_object_shared_pointer_notype(env, className, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
    }
}

jobject qtjambi_from_polymorphic_object_shared_pointer(JNIEnv *env, const std::type_info& typeId, const std::type_info& polymorphicBaseType,
                                           void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter)
{
    bool ok;
    jobject result = qtjambi_from_object_shared_pointer_impl(env, typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
    if(ok)
        return result;
    const char* java_type_name = getJavaName(typeId);
    if(java_type_name){
        if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
            return nullptr;
        void* qt_object = sharedPointerGetter(ptr_shared_pointer);
        if (!qt_object)
            return nullptr;
        const char *class_name = nullptr;
        bool isQObject = false;
        const std::type_info* _typeId = nullptr;
        qtjambi_resolve_polymorphic_id(polymorphicBaseType, qt_object, class_name, _typeId, isQObject);
        if(class_name){
            if(_typeId){
                jobject result = qtjambi_from_object_shared_pointer_impl(env, *_typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
                if(ok)
                    return result;
            }
            return qtjambi_from_object_shared_pointer_notype(env, class_name, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        }else{
            return qtjambi_from_object_shared_pointer_notype(env, java_type_name, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        }
    }
    return nullptr;
}

jobject qtjambi_from_object_shared_pointer(JNIEnv *env, const std::type_info& typeId,
                                           void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter)
{
    bool ok;
    jobject result = qtjambi_from_object_shared_pointer_impl(env, typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
    if(ok)
        return result;
    const char* java_type_name = getJavaName(typeId);
    if(java_type_name){
        return qtjambi_from_object_shared_pointer_notype(env, java_type_name, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
    }
    return nullptr;
}

jobject qtjambi_from_object_shared_pointer(JNIEnv *env, const char *className,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter)
{
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId){
        bool ok;
        jobject result = qtjambi_from_object_shared_pointer_impl(env, *typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
        if(ok)
            return result;
    }
    return qtjambi_from_object_shared_pointer_notype(env, className, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
}

jobject qtjambi_from_object_shared_pointer(JNIEnv *env, const char *className, const std::type_info& typeId,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter)
{
    bool ok;
    jobject result = qtjambi_from_object_shared_pointer_impl(env, typeId, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &ok);
    if(ok)
        return result;
    return qtjambi_from_object_shared_pointer_notype(env, className, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
}

jobject qtjambi_from_object_shared_pointer_impl(JNIEnv *env,
                            const std::type_info& typeId, void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter, bool* ok)
{
    if(ok) *ok = true;
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter || !sharedPointerGetter(ptr_shared_pointer))
        return nullptr;
    if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue result;
        if(typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter, &result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

jobject qtjambi_from_object_shared_pointer_notype(JNIEnv *env, const char *className,
                            void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetterFunction sharedPointerGetter)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* qt_object = sharedPointerGetter(ptr_shared_pointer);
    if (!qt_object)
        return nullptr;

    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(qt_object).toStrongRef()){
        jobject obj = link->getJavaObjectLocalRef(env);
        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
            link->invalidate(env);
        }else{
            return obj;
        }
    }

    jobject returned = nullptr;
    jclass clazz = resolveClass(env, className);
    if (clazz && Java::Private::QtJambi::QtJambiObject.isAssignableFrom(env, clazz)) {
        jmethodID constructorId = findInternalPrivateConstructor(env, clazz);
        if(constructorId){
            returned = env->NewObject(clazz, constructorId, 0);
        }else{
            qtjambi_throw_java_exception(env)
            return nullptr;
        }
    }
    if (!returned)
        return nullptr;
    int metaTypeID = QMetaType::UnknownType;
    if(const std::type_info* typeId = getTypeByJavaName(className)){
        metaTypeID = registeredMetaTypeID(*typeId);
        if(metaTypeID==QMetaType::UnknownType){
            metaTypeID = QMetaType::type(getQtName(*typeId));
        }
    }

    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(
                env,
                returned,
                metaTypeID,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                QMetaType::typeName(metaTypeID),
#endif
                false,
                false,
                nullptr,
                nullptr,
                ptr_shared_pointer,
                sharedPointerDeleter,
                sharedPointerGetter
            );
    if (!link) {
        returned = nullptr;
    }

    return returned;
}

jobject qtjambi_from_QMetaMethod(JNIEnv *env, jobject jmetaObject, const QMetaObject *metaObject, const QMetaMethod& method, jobject jreturnType, jobject parameterList);

jobject qtjambi_from_QMetaMethod(JNIEnv *env, const QMetaMethod& method){
    if(!method.isValid())
        return nullptr;
    jobject metaObject = qtjambi_from_QMetaObject(env, method.enclosingMetaObject());
    jobject jreturnType = nullptr;
    jobject jparameters = nullptr;
    const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
    Q_ASSERT(parameterTypeInfos);
    jreturnType = parameterTypeInfos->at(0).javaClass();
    jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
    for(int i=1; i<parameterTypeInfos->size(); ++i){
        qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
    }
    return qtjambi_from_QMetaMethod(env, metaObject, method.enclosingMetaObject(), method, jreturnType, jparameters);
}

jobject qtjambi_from_QMetaProperty(JNIEnv *env, jobject jmetaObject, const QMetaProperty& property);

jobject qtjambi_from_QMetaProperty(JNIEnv *env, const QMetaProperty& property){
    jobject metaObject = qtjambi_from_QMetaObject(env, property.enclosingMetaObject());
    return qtjambi_from_QMetaProperty(env, metaObject, property);
}

jobject qtjambi_from_QMetaEnum(JNIEnv *env, const QMetaEnum& enumerator, int enumeratorIdx);

jobject qtjambi_from_QMetaEnum(JNIEnv *env, const QMetaEnum& enumerator){
    if(!enumerator.isValid())
        return nullptr;
    return qtjambi_from_QMetaEnum(env, enumerator, enumerator.enclosingMetaObject()->indexOfEnumerator(enumerator.name()));
}

const QMetaObject *qtjambi_to_QMetaObject(JNIEnv *env, jobject object){
    if(!object)
        return nullptr;
    jlong ptr = Java::Private::QtCore::QMetaObject.metaObjectPointer(env, object);
    return reinterpret_cast<const QMetaObject *>(ptr);
}

QMetaEnum qtjambi_to_QMetaEnum(JNIEnv *env, jobject object){
    if(!object)
        return QMetaEnum();
    jobject mo = Java::Private::QtCore::QMetaEnum.enclosingMetaObject(env, object);
    const QMetaObject * metaObject = qtjambi_to_QMetaObject(env, mo);
    if(metaObject){
        jint idx = Java::Private::QtCore::QMetaEnum.enumeratorIndex(env, object);
        return metaObject->enumerator(idx);
    }else{
        return QMetaEnum();
    }
}

QMetaMethod qtjambi_to_QMetaMethod(JNIEnv *env, jobject object){
    if(!object)
        return QMetaMethod();
    jobject mo = Java::Private::QtCore::QMetaMethod.enclosingMetaObject(env, object);
    const QMetaObject * metaObject = qtjambi_to_QMetaObject(env, mo);
    if(metaObject){
        jint idx = Java::Private::QtCore::QMetaMethod.methodIndex(env, object);
        return metaObject->method(idx);
    }else{
        return QMetaMethod();
    }
}

QMetaProperty qtjambi_to_QMetaProperty(JNIEnv *env, jobject object){
    if(!object)
        return QMetaProperty();
    jobject mo = Java::Private::QtCore::QMetaProperty.metaObject(env, object);
    const QMetaObject * metaObject = qtjambi_to_QMetaObject(env, mo);
    if(metaObject){
        jint idx = Java::Private::QtCore::QMetaProperty.propertyIndex(env, object);
        return metaObject->property(idx);
    }else{
        return QMetaProperty();
    }
}

jobject qtjambi_from_QMetaObject$Connection(JNIEnv *env, const QMetaObject::Connection& connection){
    static int metaTypeId = registeredMetaTypeID(typeid(QMetaObject::Connection));
    jobject result = Java::Private::QtJambi::QtJambiSignals$NativeConnection.newInstance(env, nullptr);
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
            env,
            result,
            new QMetaObject::Connection(connection),
            metaTypeId,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
            "QMetaObject::Connection",
#endif
            false,
            false,
            nullptr,
            nullptr
        );
    link->setJavaOwnership(env);
    return result;
}

QMetaObject::Connection qtjambi_to_QMetaObject$Connection(JNIEnv *env, jobject object){
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
        Q_ASSERT(!link->isQObject());
        return *reinterpret_cast<QMetaObject::Connection*>(link->pointer());
    }
    return QMetaObject::Connection();
}

bool _qtjambi_invalidate_object(JNIEnv *env, jobject java_object, bool checkJavaOwnership)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, java_object)){
        if (!checkJavaOwnership || (   link->ownership() != QtJambiLink::Ownership::Java
                                    && !link->createdByJava()
                                    && !link->isShell()
                                    && !link->isQObject()
                                    )
            ) {
            link->invalidate(env);
        }
        return true;
    }
    return false;
}

void qtjambi_invalidate_object(JNIEnv *env, jobject java_object, bool checkJavaOwnership)
{
    _qtjambi_invalidate_object(env, java_object, checkJavaOwnership);
}

void qtjambi_invalidate_collection(JNIEnv *env, jobject java_collection, bool checkJavaOwnership)
{
    Q_ASSERT(java_collection);
    if(!_qtjambi_invalidate_object(env, java_collection, checkJavaOwnership)){
        jobjectArray java_array = qtjambi_collection_toArray(env, java_collection);
        qtjambi_invalidate_array(env, java_array, checkJavaOwnership);
    }
}

void qtjambi_invalidate_array(JNIEnv *env, jobjectArray java_array, bool checkJavaOwnership)
{
    Q_ASSERT(java_array);
    jsize array_size = env->GetArrayLength(java_array);
    for (int i=0; i<array_size; ++i) {
        jobject java_element = env->GetObjectArrayElement(java_array, i);
        if (java_element) {
            qtjambi_invalidate_object(env, java_element, checkJavaOwnership);
        }
    }
}

// Find the first in meta_object's line of ancestors (including meta_object itself) which
// is a static meta object (hence, not one of Qt Jambi's fake meta objects)
const QMetaObject *qtjambi_find_first_static_metaobject(const QMetaObject *meta_object)
{
    while (meta_object && qtjambi_metaobject_is_dynamic(meta_object))
        meta_object = meta_object->superClass();
    return meta_object;
}

jobject qtjambi_from_QObject(JNIEnv *env, const QObject *qt_object) {
    return qtjambi_from_qobject(env, qt_object, typeid(QObject));
}

jobject qtjambi_from_qobject_type(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId, bool* ok = nullptr);
jobject qtjambi_from_qobject_notype(JNIEnv *env, const QObject *const_qt_object, QByteArray className, jclass clazz);

jobject qtjambi_from_qobject(JNIEnv *env, const QObject *qt_object, const char *className)
{
    const std::type_info* typeId = className ? getTypeByJavaName(className) : &typeid(QObject);
    if(typeId){
        bool ok;
        jobject result = qtjambi_from_qobject_type(env, qt_object, *typeId, &ok);
        if(ok)
            return result;
    }
    return qtjambi_from_qobject_notype(env, qt_object, className, nullptr);
}

jobject qtjambi_from_qobject(JNIEnv *env, const QObject *qt_object, jclass clazz)
{
    QString className;
    const std::type_info* typeId;
    if(clazz){
        className = qtjambi_class_name(env, clazz).replace(".", "/");
        typeId = getTypeByJavaName(className);
    }else{
        typeId = &typeid(QObject);
    }
    if(typeId){
        bool ok;
        jobject result = qtjambi_from_qobject_type(env, qt_object, *typeId, &ok);
        if(ok)
            return result;
    }
    return qtjambi_from_qobject_notype(env, qt_object, className.toUtf8(), clazz);
}

jobject qtjambi_from_qobject(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId)
{
    bool ok;
    jobject result = qtjambi_from_qobject_type(env, qt_object, typeId, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    if(!className.isEmpty())
        return qtjambi_from_qobject_notype(env, qt_object, className, resolveClass(env, className));
    return nullptr;
}

jobject qtjambi_from_qobject_type(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId, bool* ok)
{
    if(ok) *ok = true;
    if (!qt_object)
        return nullptr;
    if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue result;
        result.l = nullptr;
        if(typeEntry->convertToJava(env, qt_object, false, true, &result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

jobject qtjambi_from_qobject_notype(JNIEnv *env, const QObject *const_qt_object, QByteArray className, jclass clazz)
{
    if (!const_qt_object)
        return nullptr;
    QObject * qt_object = const_cast<QObject *>(const_qt_object);
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object).toStrongRef();
    {
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(link && !link->createdByJava()){
            QMutexLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            QtJambiLinkUserData *p = static_cast<QtJambiLinkUserData *>(qt_object->userData(QtJambiLinkUserData::id()));
            if (p && p->metaObject() != qt_object->metaObject()) {
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                jobject nativeLink = link->nativeLink(env);
                Java::Private::QtJambi::QtJambiInternal$NativeLink.reset(env, nativeLink);
                link->setSplitOwnership(env);
                qt_object->setUserData(QtJambiLinkUserData::id(), nullptr);
                delete p;
                link = QSharedPointer<QtJambiLink>();
            }
        }
    }
    jobject obj = nullptr;
    if(link){
        obj = link->getJavaObjectLocalRef(env);
        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
            QMutexLocker locker(QtJambiLinkUserData::lock());
            Q_UNUSED(locker)
            if(QtJambiLinkUserData *p = static_cast<QtJambiLinkUserData *>(qt_object->userData(QtJambiLinkUserData::id()))){
                qt_object->setUserData(QtJambiLinkUserData::id(), nullptr);
                delete p;
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::Private::QtJambi::QtJambiInternal$NativeLink.reset(env, nativeLink);
                }
                link->invalidate(env);
                link = QSharedPointer<QtJambiLink>();
            }
        }
    }
    if (!link || !obj) {
        const QMetaObject *mo = qtjambi_find_first_static_metaobject(qt_object->metaObject());

        QString java_name;
        if(className.isNull() && clazz){
            java_name = qtjambi_class_name(env, clazz).replace(QLatin1Char('.'), QLatin1Char('/'));
        }else{
            java_name = className;
        }

        // Search the hierarchy of classes for a class that has been mapped to Java.
        // Prefer the requested class if no other can be found.
        // Only return objects of subclasses of the requested class
        // If the requested class is not in the object's hierarchy, then we prefer
        // the requested class (this means it's basically a proper subclass of the
        // requested class, and thus probably the concrete wrapper, but atleast a
        // more specific version than anything we can find)
        while (mo) {
            QString javaName;
            if(const std::type_info* typeId = getTypeByMetaObject(mo)){
                javaName = QLatin1String(getJavaName(*typeId));
            }
            if (javaName==java_name){
                // Never go further down the hierarchy than the requested class
                break;
            }else if(!javaName.isEmpty()){
                // use the found java class instead
                jclass cls = nullptr;
                try{
                    cls = resolveClass(env, qPrintable(javaName));
                    qtjambi_throw_java_exception(env)
                }catch(...){}
                if(cls){
                    java_name = javaName;
                    clazz = cls;
                    break;
                }
            }
            mo = mo->superClass();
        }

        if (!clazz) {
            clazz = resolveClass(env, qPrintable(java_name));
        }
        if (clazz) {
            if(Java::Private::Runtime::Class.isInterface(env, clazz)){
                java_name += "$Impl";
                clazz = resolveClass(env, qPrintable(java_name));
            }
        }
        if (clazz) {
            int modifiers = Java::Private::Runtime::Class.getModifiers(env, clazz);
            if(Java::Private::Runtime::Modifier.isAbstract(env, modifiers)){
                java_name += "$ConcreteWrapper";
                clazz = resolveClass(env, qPrintable(java_name));
            }
        }
        if (clazz) {
            jmethodID constructorId = findInternalPrivateConstructor(env, clazz);
            Q_ASSERT(constructorId);
            if(constructorId){
                obj = env->NewObject(clazz, constructorId, 0);
                if (!obj){
                    qWarning("Qt Jambi: Couldn't created object of class %s", qPrintable(java_name));
                    return nullptr;
                }
            }
        }else{
            qWarning("Qt Jambi: Couldn't find java class %s", qPrintable(java_name));
            return nullptr;
        }

        link = QtJambiLink::createLinkForQObject(env, obj, qt_object, false, false);
        if (!link) {
            qWarning("Qt Jambi: Couldn't created wrapper for class %s", qPrintable(java_name));
            return nullptr;
        }
    }
    return obj;
}

jobject qtjambi_from_qobject_shared_pointer_impl(JNIEnv *env, const std::type_info& typeId,
                             void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerGetter pointerGetter, bool* ok)
{
    if(ok) *ok = true;
    if (!ptr_shared_pointer || !shared_pointer_deleter || !pointerGetter || !pointerGetter(ptr_shared_pointer))
        return nullptr;

    if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue result;
        if(typeEntry->convertSharedPointerToJava(env, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, &result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    if(ok) *ok = false;
    return nullptr;
}

jobject qtjambi_from_qobject_shared_pointer(JNIEnv *env, const std::type_info& typeId,
                             void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerGetter pointerGetter)
{
    bool ok;
    jobject result = qtjambi_from_qobject_shared_pointer_impl(env, typeId, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, &ok);
    if(ok)
        return result;
    QByteArray className = getJavaName(typeId);
    return qtjambi_from_qobject_shared_pointer_notype(env, className, ptr_shared_pointer, shared_pointer_deleter, [pointerGetter](const void* ptr)->QObject*{return reinterpret_cast<QObject*>(pointerGetter(ptr));} );
}

jobject qtjambi_from_qobject_shared_pointer(JNIEnv *env, const char *className,
                             void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerGetter pointerGetter)
{
    const std::type_info* typeId = getTypeByJavaName(className);
    if(typeId){
        bool ok;
        jobject result = qtjambi_from_qobject_shared_pointer_impl(env, *typeId, ptr_shared_pointer, shared_pointer_deleter, pointerGetter, &ok);
        if(ok)
            return result;
    }
    return qtjambi_from_qobject_shared_pointer_notype(env, className, ptr_shared_pointer, shared_pointer_deleter, [pointerGetter](const void* ptr)->QObject*{return reinterpret_cast<QObject*>(pointerGetter(ptr));} );
}

jobject qtjambi_from_qobject_shared_pointer_notype(JNIEnv *env, const char *className,
                             void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerQObjectGetterFunction pointerGetter)
{
    Q_ASSERT(pointerGetter);
    Q_ASSERT(shared_pointer_deleter);
    if (!ptr_shared_pointer || !pointerGetter || !shared_pointer_deleter)
        return nullptr;
    QObject* qt_object = pointerGetter(ptr_shared_pointer);
    if(!qt_object)
        return nullptr;

    jobject object = nullptr;
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object).toStrongRef();
    {
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(link){
            object = link->getJavaObjectLocalRef(env);
            if(!object && !link->createdByJava()){
                QMutexLocker locker(QtJambiLinkUserData::lock());
                Q_UNUSED(locker)
                QtJambiLinkUserData *p = static_cast<QtJambiLinkUserData *>(qt_object->userData(QtJambiLinkUserData::id()));
                if (p && p->metaObject() != qt_object->metaObject()) {
                    // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                    // object isn't deleted.
                    jobject nativeLink = link->nativeLink(env);
                    Java::Private::QtJambi::QtJambiInternal$NativeLink.reset(env, nativeLink);
                    link->setSplitOwnership(env);
                    qt_object->setUserData(QtJambiLinkUserData::id(), nullptr);
                    delete p;
                    link = QSharedPointer<QtJambiLink>();
                }
            }
            if(link && !link->isSharedPointer()){
                if(object){
                    bool createdByJava = link->createdByJava();
                    bool is_shell = link->isShell();
                    QtJambiShellAccess* shell = nullptr;
                    if(is_shell){
                        const SuperTypeInfos& infos = getSuperTypeInfos(env, env->GetObjectClass(object));
                        Q_ASSERT(infos.size());
                        shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size );
                    }
                    jobject nativeLink = link->nativeLink(env);
                    if(nativeLink){
                        Java::Private::QtJambi::QtJambiInternal$NativeLink.reset(env, nativeLink);
                    }
                    link->invalidate(env);
                    link = QSharedPointer<QtJambiLink>();
                    link = QtJambiLink::createLinkForSharedPointerToQObject(env, object, createdByJava, is_shell, ptr_shared_pointer, shared_pointer_deleter, pointerGetter);
                    if(shell){
                        shell->overrideLink(link);
                    }
                    env->DeleteLocalRef(nativeLink);
                    if (!link) {
                        qWarning("Qt Jambi: Couldn't created wrapper for class %s", className);
                        return nullptr;
                    }
                }else{
                    QMutexLocker locker(QtJambiLinkUserData::lock());
                    Q_UNUSED(locker)
                    if(QtJambiLinkUserData *p = static_cast<QtJambiLinkUserData *>(qt_object->userData(QtJambiLinkUserData::id()))){
                        qt_object->setUserData(QtJambiLinkUserData::id(), nullptr);
                        delete p;
                        jobject nativeLink = link->nativeLink(env);
                        if(nativeLink){
                            Java::Private::QtJambi::QtJambiInternal$NativeLink.reset(env, nativeLink);
                        }
                        link->invalidate(env);
                        link = QSharedPointer<QtJambiLink>();
                    }
                }
            }
        }
    }
    if (!link || !object) {
        const QMetaObject *mo = qtjambi_find_first_static_metaobject(qt_object->metaObject());

        QByteArray javaClassName(className);

        jclass clazz = nullptr;
        try{
            clazz = resolveClass(env, javaClassName);
        }catch(...){}

        // Search the hierarchy of classes for a class that has been mapped to Java.
        // Prefer the requested class if no other can be found.
        // Only return objects of subclasses of the requested class
        // If the requested class is not in the object's hierarchy, then we prefer
        // the requested class (this means it's basically a proper subclass of the
        // requested class, and thus probably the concrete wrapper, but atleast a
        // more specific version than anything we can find)
        while (mo) {
            QString javaName;
            if(const std::type_info* typeId = getTypeByMetaObject(mo)){
                javaName = QLatin1String(getJavaName(*typeId));
            }
            if (javaName==javaClassName){
                // Never go further down the hierarchy than the requested class
                break;
            }else if(!javaName.isEmpty()){
                // use the found java class instead
                jclass cls = nullptr;
                try{
                    cls = resolveClass(env, qPrintable(javaName));
                }catch(...){}
                if(cls){
                    javaClassName = javaName.toLatin1();
                    clazz = cls;
                    break;
                }
                javaClassName = javaName.toLatin1();
                break;
            }
            mo = mo->superClass();
        }

        if(!clazz){
            clazz = resolveClass(env, javaClassName);
        }
        if (clazz) {
            if(Java::Private::Runtime::Class.isInterface(env, clazz)){
                javaClassName += "$Impl";
                clazz = resolveClass(env, javaClassName);
            }
        }
        if (clazz) {
            int modifiers = Java::Private::Runtime::Class.getModifiers(env, clazz);
            if(Java::Private::Runtime::Modifier.isAbstract(env, modifiers)){
                javaClassName += "$ConcreteWrapper";
                clazz = resolveClass(env, javaClassName);
            }
        }
        if (clazz) {
            jmethodID constructorId = findInternalPrivateConstructor(env, clazz);
            qtjambi_throw_java_exception(env)
            Q_ASSERT(constructorId);
            if(constructorId){
                object = env->NewObject(clazz, constructorId, 0);
                qtjambi_throw_java_exception(env)
            }
        }

        if (!object)
            return nullptr;

        link = QtJambiLink::createLinkForSharedPointerToQObject(env, object, false, false, const_cast<void*>(ptr_shared_pointer), shared_pointer_deleter, pointerGetter);
        if (!link) {
            qWarning("Qt Jambi: Couldn't created wrapper for class %s", className);
            return nullptr;
        }
    }
    return object;
}

jobject resolveLongEnum(JNIEnv *env, jint hashCode, jclass enumClass, jlong value, jstring entryName);

jobject resolveByteEnum(JNIEnv *env, jint hashCode, jclass enumClass, jbyte value, jstring entryName);

jobject resolveShortEnum(JNIEnv *env, jint hashCode, jclass enumClass, jshort value, jstring entryName);

jobject resolveIntEnum(JNIEnv *env, jint hashCode, jclass enumClass, jint value, jstring entryName);

template<typename INT>
jobject qtjambi_from_enum(JNIEnv *env, INT qt_enum, jclass cl, const QString& className, jobject (*resolveEnum)(JNIEnv *, jint, jclass, INT, jstring))
{
    Q_ASSERT(cl);
#if defined(QTJAMBI_DEBUG_TOOLS)
    if(!cl) {
        fprintf(stderr, "qtjambi_from_enum(): %s NOTFOUND\n", qPrintable(className));
        fflush(stderr);
        qWarning("qtjambi_from_enum(): %s NOTFOUND", qPrintable(className));
    }
#endif
    Q_ASSERT(cl);

    jobject obj;
    QString sig;
    switch ( sizeof(INT) ) {
    case 1:  sig = QLatin1String("(B)L%1;"); break;
    case 2:  sig = QLatin1String("(S)L%1;"); break;
    case 8:  sig = QLatin1String("(J)L%1;"); break;
    default: sig = QLatin1String("(I)L%1;"); break;
    }
    if(jmethodID method = resolveMethod(env, "resolve", qPrintable(sig.arg(className)), cl, true)){
        obj = env->CallStaticObjectMethod(cl, method, qt_enum);
    }else{
        if(env->ExceptionCheck())
            env->ExceptionClear();
        jint hashCode = Java::Private::Runtime::Object.hashCode(env,cl);
        obj = resolveEnum(env, hashCode, cl, qt_enum, nullptr);
    }
    qtjambi_throw_java_exception(env)
    return obj;
}

jobject qtjambi_from_flags_or_enum(JNIEnv *env, const void* qt_enum_value, const std::type_info& typeId)
{
    if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId)){
        jvalue result;
        result.l = nullptr;
        if(typeEntry->convertToJava(env, qt_enum_value, false, false, &result, jValueType::l)){
            return result.l;
        }else{
            return nullptr;
        }
    }
    return nullptr;
}

jobject qtjambi_from_enum(JNIEnv *env, qint32 qt_enum, const char *className)
{
    return qtjambi_from_enum<jint>(env, jint(qt_enum), resolveClass(env, className), QLatin1String(className), &resolveIntEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, qint16 qt_enum, const char *className)
{
    return qtjambi_from_enum<jshort>(env, jshort(qt_enum), resolveClass(env, className), QLatin1String(className), &resolveShortEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, qint8 qt_enum, const char *className)
{
    return qtjambi_from_enum<jbyte>(env, jbyte(qt_enum), resolveClass(env, className), QLatin1String(className), &resolveByteEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, qint64 qt_enum, const char *className)
{
    return qtjambi_from_enum<jlong>(env, jlong(qt_enum), resolveClass(env, className), QLatin1String(className), &resolveLongEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, quint32 qt_enum, const char *className)
{
    return qtjambi_from_enum<jint>(env, jint(qt_enum), resolveClass(env, className), QLatin1String(className), &resolveIntEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, quint16 qt_enum, const char *className)
{
    return qtjambi_from_enum<jshort>(env, jshort(qt_enum), resolveClass(env, className), QLatin1String(className), &resolveShortEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, quint8 qt_enum, const char *className)
{
    return qtjambi_from_enum<jbyte>(env, jbyte(qt_enum), resolveClass(env, className), QLatin1String(className), &resolveByteEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, quint64 qt_enum, const char *className)
{
    return qtjambi_from_enum<jlong>(env, jlong(qt_enum), resolveClass(env, className), QLatin1String(className), &resolveLongEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, qint32 qt_enum, jclass cls)
{
    return qtjambi_from_enum<jint>(env, jint(qt_enum), cls, qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveIntEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, qint16 qt_enum, jclass cls)
{
    return qtjambi_from_enum<jshort>(env, jshort(qt_enum), cls, qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveShortEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, qint8 qt_enum, jclass cls)
{
    return qtjambi_from_enum<jbyte>(env, jbyte(qt_enum), cls, qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveByteEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, qint64 qt_enum, jclass cls)
{
    return qtjambi_from_enum<jlong>(env, jlong(qt_enum), cls, qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveLongEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, quint32 qt_enum, jclass cls)
{
    return qtjambi_from_enum<jint>(env, jint(qt_enum), cls, qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveIntEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, quint16 qt_enum, jclass cls)
{
    return qtjambi_from_enum<jshort>(env, jshort(qt_enum), cls, qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveShortEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, quint8 qt_enum, jclass cls)
{
    return qtjambi_from_enum<jbyte>(env, jbyte(qt_enum), cls, qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveByteEnum);
}

jobject qtjambi_from_enum(JNIEnv *env, quint64 qt_enum, jclass cls)
{
    return qtjambi_from_enum<jlong>(env, jlong(qt_enum), cls, qtjambi_class_name(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveLongEnum);
}

jobject qtjambi_from_flags(JNIEnv *env, int qt_flags, const char *className)
{
    Q_ASSERT(className);
    jclass cl = resolveClass(env, className);
    return qtjambi_from_flags(env, qt_flags, cl);
}

jobject qtjambi_from_flags(JNIEnv *env, int qt_flags, jclass cl)
{
    jobject obj = nullptr;
    Q_ASSERT(cl);
    jmethodID method = nullptr;
    try{
        method = resolveMethod(env, "<init>", "(I)V", cl);
        qtjambi_throw_java_exception(env)
    }catch(const JavaException&){}
    if(method){
        obj = env->NewObject(cl, method, qt_flags);
    }else{
        method = resolveMethod(env, "<init>", "()V", cl);
        qtjambi_throw_java_exception(env)
        Q_ASSERT(method);
        obj = env->NewObject(cl, method);
        qtjambi_throw_java_exception(env)
        try{
            Java::Private::QtJambi::QFlags.setValue(env, obj, qt_flags);
        }catch(const JavaException& exn){
            exn.raise(QTJAMBI_STACKTRACEINFO_ENV(env));
        }
    }
    return obj;
}

QVariant qtjambi_to_enumerator(JNIEnv *env, jobject value)
{
    Q_ASSERT(value);
    QVariant result;
    if(Java::Private::QtJambi::QFlags.isInstanceOf(env, value)){
        result = QVariant::fromValue<qint32>(Java::Private::QtJambi::QFlags.value(env, value));
    }else if(Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, value)){
        result = QVariant::fromValue<qint32>(Java::Private::QtJambi::QtEnumerator.value(env, value));
    }else if(Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, value)){
        result = QVariant::fromValue<qint8>(Java::Private::QtJambi::QtByteEnumerator.value(env, value));
    }else if(Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, value)){
        result = QVariant::fromValue<qint16>(Java::Private::QtJambi::QtShortEnumerator.value(env, value));
    }else if(Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, value)){
        result = QVariant::fromValue<qint64>(Java::Private::QtJambi::QtLongEnumerator.value(env, value));
    }else{
        Q_ASSERT(false);
        return QVariant();
    }
    return result;
}

void *qtjambi_to_cpointer(JNIEnv *env, jobject java_object, int indirections)
{
    if (!java_object)
        return nullptr;
    Q_ASSERT(Java::Private::QtJambi::QNativePointer.isInstanceOf(env, java_object));  // check the java object is right type
    int object_indirections = Java::Private::QtJambi::QNativePointer.indirections(env, java_object);
    // What is this != test doing ?
    if (object_indirections != indirections) {
        JavaException::raiseIllegalArgumentException(env, "Illegal number of indirections" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
    return reinterpret_cast<void *>(Java::Private::QtJambi::QNativePointer.pointer(env,java_object));
}

jobject qtjambi_from_cpointer(JNIEnv *env, const void *qt_object, QNativePointer::Type type, int indirections)
{
    return Java::Private::QtJambi::QNativePointer.fromNative(env, reinterpret_cast<jlong>(qt_object), int(type), jlong(-1), indirections, false);
}

jstring qtjambi_from_qstringref(JNIEnv *env, const QStringRef &s)
{
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.constData()), s.length());
    Q_ASSERT(str);
    qtjambi_throw_java_exception(env)
    return str;
}

jstring qtjambi_from_qstringview(JNIEnv *env, QStringView s)
{
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.data()), s.length());
    Q_ASSERT(str);
    qtjambi_throw_java_exception(env)
    return str;
}

jstring qtjambi_from_qstring(JNIEnv *env, const QString &s)
{
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.constData()), s.length());
    Q_ASSERT(str);
    qtjambi_throw_java_exception(env)
    return str;
}

jstring qtjambi_to_jstring(JNIEnv *env, jobject object)
{
    jstring java_string = nullptr;
    if (object){
        if(Java::Runtime::String.isInstanceOf(env, object)){
            java_string = jstring(object);
        }else{
            java_string = Java::Private::Runtime::Object.toString(env, object);
            qtjambi_throw_java_exception(env)
        }
    }
    return java_string;
}


void qtjambi_to_qstring(QString& result, JNIEnv *env, jobject object)
{
    qtjambi_to_qstring(result, env, qtjambi_to_jstring(env, object));
}

QString qtjambi_to_qstring(JNIEnv *env, jobject object)
{
    QString result;
    qtjambi_to_qstring(result, env, object);
    return result;
}

QString qtjambi_to_qstring(JNIEnv *env, jstring java_string)
{
    QString result;
    qtjambi_to_qstring(result, env, java_string);
    return result;
}

void qtjambi_to_qstring(QString& result, JNIEnv *env, jstring java_string)
{
    if (java_string){
        int length = env->GetStringLength(java_string);
        result.resize(length);
        env->GetStringRegion(java_string, 0, length, reinterpret_cast<ushort*>(result.data()));
        qtjambi_throw_java_exception(env)
    }else{
        result.clear();
    }
}

void qtjambi_to_qbytearray(QByteArray& result, JNIEnv *env, jstring java_string)
{
    if (java_string){
        int length = env->GetStringLength(java_string);
        result.resize(length);
        env->GetStringUTFRegion(java_string, 0, length, result.data());
        qtjambi_throw_java_exception(env)
    }else{
        result.clear();
    }
}

QByteArray qtjambi_to_qbytearray(JNIEnv *env, jstring java_string)
{
    QByteArray result;
    qtjambi_to_qbytearray(result, env, java_string);
    return result;
}

// #### forward declare from qtjambilink.cpp ####
jobject getNativeLink(JNIEnv *env, jobject java);
// ##############################################

void qtjambi_dispose_interface(JNIEnv *env, jobject i)
{
    if(Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env,i))
        Java::Private::QtJambi::QtObjectInterface.dispose(env, i);
}

bool qtjambi_is_interface_disposed(JNIEnv *env, jobject i){
    if(Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env,i))
        return Java::Private::QtJambi::QtObjectInterface.isDisposed(env, i);
    return true;
}

JBufferConstData::JBufferConstData(JNIEnv *env, jobject buffer_object, bool purgeOnDelete) :
    m_buffer_object(buffer_object ? env->NewGlobalRef(buffer_object) : nullptr),
    m_size(0),
    m_data(nullptr),
    m_purgeOnDelete(purgeOnDelete),
    m_isdirect(true)
{
    if(m_buffer_object){
        m_isdirect = Java::Runtime::Buffer.isDirect(env, m_buffer_object);
        if(m_isdirect){
            m_size = env->GetDirectBufferCapacity(m_buffer_object);
            m_data = env->GetDirectBufferAddress(m_buffer_object);
        }else{
            m_size = Java::Runtime::Buffer.capacity(env, m_buffer_object);
            if(Java::Runtime::ByteBuffer.isInstanceOf(env, m_buffer_object)){
                qint8* array = new qint8[size_t(m_size)];
                for(jsize i=0; i<m_size; ++i){
                    array[i] = Java::Runtime::ByteBuffer.get(env, m_buffer_object, i);
                }
                m_data = array;
            }else if(Java::Runtime::IntBuffer.isInstanceOf(env, m_buffer_object)){
                qint32* array = new qint32[size_t(m_size)];
                for(jsize i=0; i<m_size; ++i){
                    array[i] = Java::Runtime::IntBuffer.get(env, m_buffer_object, i);
                }
                m_data = array;
            }else if(Java::Runtime::ShortBuffer.isInstanceOf(env, m_buffer_object)){
                qint16* array = new qint16[size_t(m_size)];
                for(jsize i=0; i<m_size; ++i){
                    array[i] = Java::Runtime::ShortBuffer.get(env, m_buffer_object, i);
                }
                m_data = array;
            }else if(Java::Runtime::CharBuffer.isInstanceOf(env, m_buffer_object)){
                char* array = new char[size_t(m_size)];
                for(jsize i=0; i<m_size; ++i){
                    array[i] = char(Java::Runtime::CharBuffer.get(env, m_buffer_object, i));
                }
                m_data = array;
            }else if(Java::Runtime::LongBuffer.isInstanceOf(env, m_buffer_object)){
                qint64* array = new qint64[size_t(m_size)];
                for(jsize i=0; i<m_size; ++i){
                    array[i] = Java::Runtime::LongBuffer.get(env, m_buffer_object, i);
                }
                m_data = array;
            }else if(Java::Runtime::FloatBuffer.isInstanceOf(env, m_buffer_object)){
                float* array = new float[size_t(m_size)];
                for(jsize i=0; i<m_size; ++i){
                    array[i] = Java::Runtime::FloatBuffer.get(env, m_buffer_object, i);
                }
                m_data = array;
            }else if(Java::Runtime::DoubleBuffer.isInstanceOf(env, m_buffer_object)){
                double* array = new double[size_t(m_size)];
                for(jsize i=0; i<m_size; ++i){
                    array[i] = Java::Runtime::DoubleBuffer.get(env, m_buffer_object, i);
                }
                m_data = array;
            }
        }
    }
}

JBufferConstData::~JBufferConstData(){
    try{
        if(m_buffer_object){
            if(JNIEnv* env = qtjambi_current_environment(false)){
                if(m_purgeOnDelete && !m_isdirect){
                    if(Java::Runtime::ByteBuffer.isInstanceOf(env, m_buffer_object)){
                        qint8* array = reinterpret_cast<qint8*>(m_data);
                        delete[] array;
                    }else if(Java::Runtime::IntBuffer.isInstanceOf(env, m_buffer_object)){
                        qint32* array = reinterpret_cast<qint32*>(m_data);
                        delete[] array;
                    }else if(Java::Runtime::ShortBuffer.isInstanceOf(env, m_buffer_object)){
                        qint16* array = reinterpret_cast<qint16*>(m_data);
                        delete[] array;
                    }else if(Java::Runtime::CharBuffer.isInstanceOf(env, m_buffer_object)){
                        char* array = reinterpret_cast<char*>(m_data);
                        delete[] array;
                    }else if(Java::Runtime::LongBuffer.isInstanceOf(env, m_buffer_object)){
                        qint64* array = reinterpret_cast<qint64*>(m_data);
                        delete[] array;
                    }else if(Java::Runtime::FloatBuffer.isInstanceOf(env, m_buffer_object)){
                        float* array = reinterpret_cast<float*>(m_data);
                        delete[] array;
                    }else if(Java::Runtime::DoubleBuffer.isInstanceOf(env, m_buffer_object)){
                        double* array = reinterpret_cast<double*>(m_data);
                        delete[] array;
                    }
                }
                env->DeleteGlobalRef(m_buffer_object);
            }
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

const void* JBufferConstData::data() const {
    return m_data;
}

const void* JBufferConstData::constData() const {
    return m_data;
}

qint64 JBufferConstData::size() const{
    return m_size;
}

JBufferData::JBufferData(JNIEnv *env, jobject buffer_object) :
    JBufferConstData(env, buffer_object, false)
{
}

JBufferData::~JBufferData(){
    try{
        if(m_buffer_object){
            if(JNIEnv* env = qtjambi_current_environment(false)){
                jboolean isReadOnly = Java::Runtime::Buffer.isReadOnly(env, m_buffer_object);
                if(!m_isdirect && !isReadOnly){
                    if(Java::Runtime::ByteBuffer.isInstanceOf(env, m_buffer_object)){
                        qint8* array = reinterpret_cast<qint8*>(m_data);
                        for(jsize i=0; i<m_size; ++i){
                            Java::Runtime::ByteBuffer.put(env, m_buffer_object, i, array[i]);
                        }
                    }else if(Java::Runtime::IntBuffer.isInstanceOf(env, m_buffer_object)){
                        qint32* array = reinterpret_cast<qint32*>(m_data);
                        for(jsize i=0; i<m_size; ++i){
                            Java::Runtime::IntBuffer.put(env, m_buffer_object, i, array[i]);
                        }
                    }else if(Java::Runtime::ShortBuffer.isInstanceOf(env, m_buffer_object)){
                        qint16* array = reinterpret_cast<qint16*>(m_data);
                        for(jsize i=0; i<m_size; ++i){
                            Java::Runtime::ShortBuffer.put(env, m_buffer_object, i, array[i]);
                        }
                    }else if(Java::Runtime::CharBuffer.isInstanceOf(env, m_buffer_object)){
                        char* array = reinterpret_cast<char*>(m_data);
                        for(jsize i=0; i<m_size; ++i){
                            Java::Runtime::CharBuffer.put(env, m_buffer_object, i, array[i]);
                        }
                    }else if(Java::Runtime::LongBuffer.isInstanceOf(env, m_buffer_object)){
                        qint64* array = reinterpret_cast<qint64*>(m_data);
                        for(jsize i=0; i<m_size; ++i){
                            Java::Runtime::LongBuffer.put(env, m_buffer_object, i, array[i]);
                        }
                    }else if(Java::Runtime::FloatBuffer.isInstanceOf(env, m_buffer_object)){
                        float* array = reinterpret_cast<float*>(m_data);
                        for(jsize i=0; i<m_size; ++i){
                            Java::Runtime::FloatBuffer.put(env, m_buffer_object, i, double(array[i]));
                        }
                    }else if(Java::Runtime::DoubleBuffer.isInstanceOf(env, m_buffer_object)){
                        double* array = reinterpret_cast<double*>(m_data);
                        for(jsize i=0; i<m_size; ++i){
                            Java::Runtime::DoubleBuffer.put(env, m_buffer_object, i, array[i]);
                        }
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

void* JBufferData::data(){
    return m_data;
}

jobjectArray qtjambi_create_array(JNIEnv *env, const char* componentClass, jsize size){
    jclass arrayElementType = resolveClass(env, componentClass);
    qtjambi_throw_java_exception(env)
    Q_ASSERT(arrayElementType);
    jobjectArray array = env->NewObjectArray(jsize(size), arrayElementType, nullptr);
    qtjambi_throw_java_exception(env)
    return array;
}

const char * getJavaInterfaceName(const std::type_info& typeId);

jobjectArray qtjambi_create_array(JNIEnv *env, const std::type_info& componentType, jsize size){
    jclass arrayElementType = resolveClass(env, getJavaInterfaceName(componentType));
    qtjambi_throw_java_exception(env)
    Q_ASSERT(arrayElementType);
    jobjectArray array = env->NewObjectArray(jsize(size), arrayElementType, nullptr);
    qtjambi_throw_java_exception(env)
    return array;
}

#define PointerArrayINIT(Type,type)\
     : PointerArray(env, pointer, env->New##Type##Array(pointer ? size : 0), pointer ? size : 0) {\
    if(pointer){\
        qtjambi_throw_java_exception(env)\
        env->Set##Type##ArrayRegion(array(), 0, size, reinterpret_cast<const j##type *>(pointer));\
        qtjambi_throw_java_exception(env)\
    }\
}

#define PointerArrayDEL(Type,type)\
{\
    if(m_array){\
        if(JNIEnv* env = qtjambi_current_environment()){\
            env->Get##Type##ArrayRegion(m_array, 0, m_size, reinterpret_cast<j##type *>(m_pointer));\
            qtjambi_throw_java_exception(env)\
        }\
    }\
}

CharPointerArray::CharPointerArray(JNIEnv *env, char* pointer, jsize size) PointerArrayINIT(Byte,byte)

CharPointerArray::~CharPointerArray() PointerArrayDEL(Byte,byte)

WCharPointerArray::WCharPointerArray(JNIEnv *env, ushort* pointer, jsize size) PointerArrayINIT(Char,char)

WCharPointerArray::~WCharPointerArray() PointerArrayDEL(Char,char)

QCharPointerArray::QCharPointerArray(JNIEnv *env, QChar* pointer, jsize size) PointerArrayINIT(Char,char)

QCharPointerArray::~QCharPointerArray() PointerArrayDEL(Char,char)

DoublePointerArray::DoublePointerArray(JNIEnv *env, double* pointer, jsize size) PointerArrayINIT(Double,double)

DoublePointerArray::~DoublePointerArray() PointerArrayDEL(Double,double)

FloatPointerArray::FloatPointerArray(JNIEnv *env, float* pointer, jsize size) PointerArrayINIT(Float,float)

FloatPointerArray::~FloatPointerArray() PointerArrayDEL(Float,float)

Int8PointerArray::Int8PointerArray(JNIEnv *env, qint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

Int8PointerArray::~Int8PointerArray() PointerArrayDEL(Byte,byte)

Int16PointerArray::Int16PointerArray(JNIEnv *env, qint16* pointer, jsize size) PointerArrayINIT(Short,short)

Int16PointerArray::~Int16PointerArray() PointerArrayDEL(Short,short)

Int32PointerArray::Int32PointerArray(JNIEnv *env, qint32* pointer, jsize size) PointerArrayINIT(Int,int)

Int32PointerArray::~Int32PointerArray() PointerArrayDEL(Int,int)

Int64PointerArray::Int64PointerArray(JNIEnv *env, qint64* pointer, jsize size) PointerArrayINIT(Long,long)

Int64PointerArray::~Int64PointerArray() PointerArrayDEL(Long,long)

BoolPointerArray::BoolPointerArray(JNIEnv *env, bool* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

BoolPointerArray::~BoolPointerArray() PointerArrayDEL(Boolean,boolean)

Bool2PointerArray::Bool2PointerArray(JNIEnv *env, uchar* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

Bool2PointerArray::~Bool2PointerArray() PointerArrayDEL(Boolean,boolean)

ConstCharPointerArray::ConstCharPointerArray(JNIEnv *env, const char* pointer, jsize size) PointerArrayINIT(Byte,byte)

ConstInt8PointerArray::ConstInt8PointerArray(JNIEnv *env, const qint8* pointer, jsize size) PointerArrayINIT(Byte,byte)

ConstInt16PointerArray::ConstInt16PointerArray(JNIEnv *env, const qint16* pointer, jsize size) PointerArrayINIT(Short,short)

ConstInt32PointerArray::ConstInt32PointerArray(JNIEnv *env, const qint32* pointer, jsize size) PointerArrayINIT(Int,int)

ConstInt64PointerArray::ConstInt64PointerArray(JNIEnv *env, const qint64* pointer, jsize size) PointerArrayINIT(Long,long)

ConstBoolPointerArray::ConstBoolPointerArray(JNIEnv *env, const bool* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

ConstBool2PointerArray::ConstBool2PointerArray(JNIEnv *env, const uchar* pointer, jsize size) PointerArrayINIT(Boolean,boolean)

ConstWCharPointerArray::ConstWCharPointerArray(JNIEnv *env, const ushort* pointer, jsize size) PointerArrayINIT(Char,char)

ConstDoublePointerArray::ConstDoublePointerArray(JNIEnv *env, const double* pointer, jsize size) PointerArrayINIT(Double,double)

ConstFloatPointerArray::ConstFloatPointerArray(JNIEnv *env, const float* pointer, jsize size) PointerArrayINIT(Float,float)

ConstQCharPointerArray::ConstQCharPointerArray(JNIEnv *env, const QChar* pointer, jsize size) PointerArrayINIT(Char,char)

#undef PointerArrayINIT
#undef PointerArrayDEL

#define ArrayPointerINIT(Type)\
    if(m_array && m_size>0){\
        m_array_elements = env->Get##Type##ArrayElements(m_array, &m_is_copy);\
        qtjambi_throw_java_exception(env)\
    }
#define ArrayPointerDEL(Type)\
    if(m_array && m_size>0){\
        if(JNIEnv* env = qtjambi_current_environment()){\
            env->Release##Type##ArrayElements(m_array, m_array_elements, m_writable ? 0 : JNI_ABORT);\
            qtjambi_throw_java_exception(env)\
        }\
    }

#define ArrayPointerStructors(Type,type)\
J##Type##ArrayPointer::J##Type##ArrayPointer(JNIEnv *env, j##type##Array array, bool writable)\
    : JArrayPointer(env, array, writable) {\
    ArrayPointerINIT(Type)\
}\
J##Type##ArrayPointer::~J##Type##ArrayPointer()\
{\
    try{\
        ArrayPointerDEL(Type)\
    } catch (const std::exception& e) {\
        qWarning("%s", e.what());\
    } catch (...) {\
    }\
}

ArrayPointerStructors(Byte,byte)
ArrayPointerStructors(Int,int)
ArrayPointerStructors(Short,short)
ArrayPointerStructors(Long,long)
ArrayPointerStructors(Char,char)
ArrayPointerStructors(Double,double)
ArrayPointerStructors(Float,float)

#undef ArrayPointerStructors
#undef ArrayPointerINIT
#undef ArrayPointerDEL

JBooleanArrayPointer::JBooleanArrayPointer(JNIEnv *env, jbooleanArray array, bool writable)
    : JArrayPointer(env, array, writable) {
    if(m_array && m_size>0){
        m_boolean_array = env->GetBooleanArrayElements(m_array, &m_is_copy);
        qtjambi_throw_java_exception(env)
        if(sizeof(jboolean)==sizeof(bool)){
            m_array_elements = reinterpret_cast<bool*>(m_boolean_array);
        }else{
            m_array_elements = new bool[size_t(m_size)];
            for (int i= 0; i < m_size; ++i) {
                m_array_elements[i] = m_boolean_array[i]==JNI_TRUE;
            }
        }
    }
}

JBooleanArrayPointer::~JBooleanArrayPointer()
{
    try{
        if(m_array && m_size>0){
            if(JNIEnv* env = qtjambi_current_environment(false)){
                if(sizeof(jboolean)!=sizeof(bool) && m_writable){
                    for (int i= 0; i < m_size; ++i) {
                        m_boolean_array[i] = m_array_elements[i] ? JNI_TRUE : JNI_FALSE;
                    }
                }
                env->ReleaseBooleanArrayElements(m_array, m_boolean_array, m_writable ? 0 : JNI_ABORT);
                qtjambi_throw_java_exception(env)
            }
            if(sizeof(jboolean)!=sizeof(bool)){
                delete[] m_array_elements;
            }
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

jboolean* JBooleanArrayPointer::booleanArray(){
    return m_boolean_array;
}

const jboolean* JBooleanArrayPointer::booleanArray() const{
    return m_boolean_array;
}

bool qtjambi_is_valid_array(JNIEnv *env, jobject object, jclass contentType){
    jclass arrayClass = env->GetObjectClass(object);
    if(Java::Private::Runtime::Class.isArray(env, arrayClass)){
        jclass componentType = Java::Private::Runtime::Class.getComponentType(env, arrayClass);
        return env->IsAssignableFrom(componentType, contentType);
    }
    return false;
}

bool qtjambi_is_valid_array(JNIEnv *env, jobject object, const std::type_info& typeId){
    if(jclass contentType = resolveClass(env, getJavaInterfaceName(typeId))){
        return qtjambi_is_valid_array(env, object, contentType);
    }
    return false;
}

bool JBooleanArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Boolean.primitiveType(env));
}

bool JIntArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Integer.primitiveType(env));
}

bool JShortArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Short.primitiveType(env));
}

bool JByteArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Byte.primitiveType(env));
}

bool JCharArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Character.primitiveType(env));
}

bool JLongArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Long.primitiveType(env));
}

bool JDoubleArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Double.primitiveType(env));
}

bool JFloatArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Float.primitiveType(env));
}

J2CStringBuffer::J2CStringBuffer(JNIEnv* env, jstring strg) : m_length(-1), m_buffer(nullptr, 0) {
    if(strg){
        QString qstrg = qtjambi_to_qstring(env, strg);
        m_buffer = qstrg.toUtf8();
        m_length = qstrg.length();
    }
}

J2CStringBuffer::~J2CStringBuffer(){}

J2CStringBuffer::operator const char*() const {return m_buffer.isNull() ? nullptr : m_buffer.data();}

J2CStringBuffer::operator char*() {return m_buffer.isNull() ? nullptr : m_buffer.data();}

int J2CStringBuffer::length() const {return m_length;}

Destructor registeredDestructor(const std::type_info& typeId);

jobject qtjambi_array_to_nativepointer(JNIEnv *env, jobjectArray array, size_t elementSize)
{
    Q_ASSERT(array);
    jsize len = env->GetArrayLength(array);
    if (len == 0)
        return nullptr;
    jobject nativePointer = Java::Private::QtJambi::QNativePointer.newInstance(env, jint(QNativePointer::Type::Byte), elementSize * size_t(len), 1, false);
    char *buf = reinterpret_cast<char *>(qtjambi_to_cpointer(env, nativePointer, 1));
    for (jsize i=0; i<len; ++i) {
        jobject java_object = env->GetObjectArrayElement(array, i);
        void *ptr = nullptr;
        if (java_object) {
            const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, java_object);
            if (link)
                ptr = link->pointer();
        }
        if (ptr)
            memcpy(buf + size_t(i) * elementSize, ptr, elementSize);
    }
    return nativePointer;
}

QThread *qtjambi_find_qthread_for_jthread(JNIEnv *env, jobject thread)
{
    jobject associatedObject = Java::Private::QtJambi::QtJambiInternal.findAssociation(env, thread);
#ifdef QT_QTJAMBI_PORT
    if(blockerLock && Java::Private::QtCore::QThread$Wrapper.isInstanceOf(env, blockerLock)){
#else
    if(associatedObject && Java::QtCore::QThread.isInstanceOf(env, associatedObject)){
#endif
        return static_cast<QThread *>(qtjambi_to_qobject(env, associatedObject));
    }
    return nullptr;
}

QThread *qtjambi_to_thread(JNIEnv *env, jobject thread)
{
    if(thread==nullptr)
        return nullptr;

    QThread *qt_thread = qtjambi_find_qthread_for_jthread(env, thread);
    if (qt_thread)
        return qt_thread;
#ifdef QT_QTJAMBI_PORT
    qt_thread = new AdoptedThread(env, thread, false);
#endif
    return qt_thread;
}

#ifdef QT_QTJAMBI_PORT
const QSharedPointer<QtJambiLink>&
#else
void
#endif
qtjambi_adopt_thread(JNIEnv *env, jobject java_thread, jobject java_qthread, QThread *qt_thread, bool created_by_java);

jobject qtjambi_from_thread_impl(JNIEnv * env, jobject java_qthread, QThread *thread)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_qthread)){
        link->setNoThreadInitializationOnPurge(true);
    }
    if(thread == QThread::currentThread()){
        jobject currentThread = Java::Private::Runtime::Thread.currentThread(env);
        qtjambi_adopt_thread(env, currentThread, java_qthread, thread, false);
        return currentThread;
    }else{
#ifdef QT_QTJAMBI_PORT
        Q_ASSERT(false);
#else
        if(thread->eventDispatcher()){
            class Adopter : public QObject{
            public:
                bool event(QEvent* event){
                    if(JNIEnv *env = qtjambi_current_environment()){
                        jobject currentThread = Java::Private::Runtime::Thread.currentThread(env);
                        qtjambi_adopt_thread(env, currentThread, m_java_qthread.object(), thread(), false);
                    }
                    return QObject::event(event);
                }
                JObjectWrapper m_java_qthread;
            };
            Adopter* adopter = new Adopter();
            adopter->m_java_qthread = JObjectWrapper(env, java_qthread);
            adopter->moveToThread(thread);
            adopter->deleteLater();
        }
#endif
    }
    return nullptr;
}

jobject qtjambi_from_thread(JNIEnv * env, QThread *thread)
{
    if(!thread)
        return nullptr;
    {
        QMutexLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        if(QtJambiLinkUserData* threadUserData = static_cast<QtJambiLinkUserData*>(thread->userData(QtJambiLinkUserData::id()))){
            if(QSharedPointer<QtJambiLink> link = threadUserData->link().toStrongRef()){
                if(jobject o = link->getJavaObjectLocalRef(env)){
#ifdef QT_QTJAMBI_PORT
                    jobject result = Java::Private::QtCore::QThread$Wrapper.thread(env, o);
#else
                    jobject result = Java::QtCore::QThread.javaThread(env, o);
#endif
                    if(result)
                        return result;
                }
            }
        }
    }
    return qtjambi_from_thread_impl(env, nullptr, thread);
}

#ifdef QT_QTJAMBI_PORT

class ThreadFactory final : public QThread{
private:
    class Event{
    public:
        Event(jobject java_thread, AdoptedThread* &result, QSemaphore *semaphore):
            m_java_thread(java_thread),
            m_result(result),
            m_semaphore(semaphore)
        {
        }
        jobject m_java_thread;
        AdoptedThread* &m_result;
        QSemaphore *m_semaphore;
    };
public:
    ThreadFactory() : QThread(),
        m_nextEvent(nullptr),
        m_semaphore(),
        m_lock(),
        m_interrupted(false),
        m_interrupted_lock()
    {
        setObjectName("QtjambiThreadFactory");
        start();
    }

    void run() override {
        while(true){
            m_semaphore.acquire();
            if(m_nextEvent){
                if(JNIEnv *env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    jobject thread = env->NewGlobalRef(m_nextEvent->m_java_thread);
                    m_nextEvent->m_result = new AdoptedThread(env, thread, true);
                    env->DeleteGlobalRef(thread);
                    m_nextEvent->m_semaphore->release();
                    m_nextEvent = nullptr;
                }else{
                    break;
                }
            }
            {
                QReadLocker locker(&m_interrupted_lock);
                Q_UNUSED(locker)
                if(m_interrupted)
                    break;
            }
        }
        delete this;
    }

    void interrupt(){
        bool was_interrupted = true;
        {
            QWriteLocker locker(&m_interrupted_lock);
            Q_UNUSED(locker)
            was_interrupted = m_interrupted;
            if(was_interrupted){
                m_interrupted = true;
            }
        }
        if(!was_interrupted)
            m_semaphore.release();
    }

    AdoptedThread* createThread(jobject java_thread)
    {
        Q_ASSERT(java_thread);
        QWriteLocker locker(&m_lock);
        Q_UNUSED(locker)
        AdoptedThread* result = nullptr;
        QSemaphore semaphore;
        ThreadFactory::Event event(java_thread, result, &semaphore);
        m_nextEvent = &event;
        m_semaphore.release();
        semaphore.acquire();
        return result;
    }

    static inline void cleanup(ThreadFactory *pointer) { if (pointer) pointer->interrupt(); }
private:
    Event* m_nextEvent;
    QSemaphore m_semaphore;
    QReadWriteLock m_lock;
    bool m_interrupted;
    QReadWriteLock m_interrupted_lock;
};

typedef QScopedPointer<ThreadFactory,ThreadFactory> ThreadFactoryPointer;
Q_GLOBAL_STATIC_WITH_ARGS(ThreadFactoryPointer, qtjambi_thread_factory, (new ThreadFactory()));

bool qtjambi_adopt_current_thread(void **args)
{
    JNIEnv *env = qtjambi_current_environment();
    if (env == nullptr)
      return false;

    QTJAMBI_JNI_LOCAL_FRAME(env, 200)

    jobject java_thread = Java::Private::Runtime::Thread.currentThread(env);

    QThread *qt_thread = qtjambi_find_qthread_for_jthread(env, java_thread);
    if (!qt_thread && qtjambi_thread_factory && qtjambi_thread_factory->data()){
        jobject global_java_thread = env->NewGlobalRef(java_thread);
        qt_thread = qtjambi_thread_factory->data()->createThread(global_java_thread);
        env->DeleteGlobalRef(global_java_thread);
    }

    if (!qt_thread)
        return false;

    QThreadData* threadData = QThreadData::get2(qt_thread);
    // QThreadData::ref() is not external.
    // Solution: the first field of QThreadData class is QAtomicInt, so we can cast to QAtomicInt*
    reinterpret_cast<QAtomicInt*>(threadData)->ref();

    *args = qt_thread;
    return true;
}
#endif

/*
FIXME:
We need some way of comparing the pointer to this type offsets and sizes
of the members against QModelIndex so any changes show up.
We have to do this in the first place because Qt provides no way to create
QModelIndex from public API. This is because application code should never
create them, but Jambi is closer Qt library code than an application
and hence needs to play with QModelIndex.
*/
typedef struct _QModelIndexAccessor {
    int r;
    int c;
    quintptr p;
    const QAbstractItemModel *m;
} /*__attribute__((__may_alias__))*/ QModelIndexAccessor;

QModelIndex qtjambi_to_QModelIndex(JNIEnv *env, jobject index)
{
    if (!index)
        return QModelIndex();
    QModelIndexAccessor mia = {
        Java::Private::QtCore::QModelIndex.row(env,index),
        Java::Private::QtCore::QModelIndex.column(env,index),
        quintptr(Java::Private::QtCore::QModelIndex.internalId(env, index)),
        qtjambi_cast<const QAbstractItemModel *>(env, Java::Private::QtCore::QModelIndex.model(env, index))
    };
    // Newer GCC show this as type-prunned pointer will break strict-aliasing rules
#if 1
    void *vp = &mia;
    return *reinterpret_cast<QModelIndex *>(vp);
#else
    return * (QModelIndex *) (void *) (&mia) ;
#endif

}

jobject qtjambi_from_QModelIndex(JNIEnv *env, const QModelIndex &index)
{
    if (!index.isValid())
        return nullptr;
    return Java::Private::QtCore::QModelIndex.newInstance(env,
                                    jint(index.row()),
                                    jint(index.column()),
                                    jlong(index.internalId()),
                                    qtjambi_cast<jobject>(env, index.model()));
}

bool qtjambi_is_created_by_java(QObject *qobject)
{
    QMutexLocker locker(QtJambiLinkUserData::lock());
    Q_UNUSED(locker)
    QtJambiLinkUserData *userData = static_cast<QtJambiLinkUserData *>(qobject->userData(QtJambiLinkUserData::id()));

    Q_ASSERT(!userData || userData->link());

    QSharedPointer<QtJambiLink> link;
    if(userData)
        link = userData->link().toStrongRef();
    return link && link->createdByJava();
}

bool qtjambi_destroy_vm()
{
    JavaVM *vm = nullptr;

    if(!qtjambi_vm_lock.isDestroyed()){
        QWriteLocker locker(qtjambi_vm_lock());
        Q_UNUSED(locker)
        vm = qtjambi_vm;
    }
    if (!vm)
        return false;

    qtjambi_vm = nullptr;
    return vm->DestroyJavaVM() == 0;
}

#ifdef QT_QTJAMBI_PORT

struct ResolvedConnectionData
{
    jobject java_sender;
    jobject java_receiver;
    jobject java_signal;
    jobject java_method;
};

struct BasicConnectionData
{
    QObject *sender;
    char *signal;
    QObject *receiver;
    char *method;
};

Q_GLOBAL_STATIC(QThreadStorage<bool>, gIsInDisconnect)
Q_GLOBAL_STATIC(QThreadStorage<bool>, gIsInConnectFromJava)
Q_GLOBAL_STATIC(QThreadStorage<bool>, gIsInDisconnectFromJava)

/* If we return 'true' we always populate 'resolved_data' */
static bool qtjambi_resolve_connection_data(JNIEnv *jni_env, QSharedPointer<QtJambiLink> sender_link, const BasicConnectionData *data,
                                            ResolvedConnectionData *resolved_data,
                                            QMetaMethod* metaSignal,
                                            QMetaMethod* metaMethod,
                                            bool create_java_objects)
{
    Q_ASSERT(jni_env);
    Q_ASSERT(data);
    Q_ASSERT(resolved_data);
    Q_ASSERT(data->sender);
    Q_ASSERT(data->signal);

    BasicConnectionData data_normalized =
        { data->sender, data->signal, data->receiver, data->method };
    QByteArray signal_normalized = QMetaObject::normalizedSignature(data->signal);
    data_normalized.signal = signal_normalized.data();

    QByteArray method_normalized;
    method_normalized = QMetaObject::normalizedSignature(data->method);
    data_normalized.method = data->method ? method_normalized.data() : nullptr;
    data = &data_normalized;

    QSharedPointer<QtJambiLink> receiver_link = QtJambiLink::findLinkForQObject(data->receiver).toStrongRef();

    if (!receiver_link && data->receiver) {
        if (!create_java_objects) // don't create new java objects for disconnects
            return false;         // because unless there's a java object there isn't
                                  // a connection in java
        receiver_link = QtJambiLink::findLinkForJavaObject(jni_env, qtjambi_from_QObject(jni_env, data->receiver));
    }

    if (!sender_link && data->sender) {
        if (!create_java_objects)
            return false;
        sender_link = QtJambiLink::findLinkForJavaObject(jni_env, qtjambi_from_QObject(jni_env, data->sender));
    }

    if (!sender_link) {
        qWarning("qtjambi_resolve_connection_data(): can't resolve sender '%s'",
                    qPrintable(data->sender->objectName()));
        return false;
    }

    resolved_data->java_sender = sender_link->getJavaObjectRef(jni_env);
    if (!resolved_data->java_sender)
        return false;

    if(receiver_link){
        resolved_data->java_receiver = receiver_link->getJavaObjectRef(jni_env);
    }else{
        resolved_data->java_receiver = nullptr;
    }

    Java::Runtime::resolveClass(jni_env);
    Java::Runtime::resolveQtJambiInternal(jni_env);
    QMetaMethod signalMethod = metaSignal ? *metaSignal : data->sender->metaObject()->method(data->sender->metaObject()->indexOfSignal(data->signal + 1));
    if(signalMethod.isValid()){
        const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(jni_env, signalMethod);
        Q_ASSERT(parameterTypeInfos);
        jobjectArray jparameters = jni_env->NewObjectArray(parameterTypeInfos->size()-1, Java::Private::Runtime::Class.class_ref, nullptr);
        for(int i=1; i<parameterTypeInfos->size(); ++i){
            jni_env->SetObjectArrayElement(jparameters, i-1, parameterTypeInfos->at(i).javaClass());
        }
        resolved_data->java_signal =
            Java::Private::QtJambi::QtJambiInternal.lookupSignal(jni_env,
                                            resolved_data->java_sender,
                                            qtjambi_from_qstring(jni_env, QString::fromUtf8(signalMethod.name())), jparameters);
        jni_env->DeleteLocalRef(jparameters);
    }else if(data->signal){
        resolved_data->java_signal =
            Java::Private::QtJambi::QtJambiInternal.lookupSignal(jni_env,
                                            resolved_data->java_sender,
                                            jni_env->NewStringUTF(data->signal + 1), nullptr);
    }


    if (!resolved_data->java_signal)  // unmapped signal, cannot be resolved by us
        return false;

    if (resolved_data->java_receiver && data->method) {
        QMetaMethod slotMethod = metaMethod ? *metaMethod : data->receiver->metaObject()->method(data->receiver->metaObject()->indexOfSlot(data->method + 1));
        if(slotMethod.isValid()){
            const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(jni_env, slotMethod);
            Q_ASSERT(parameterTypeInfos);
            jobjectArray jparameters = jni_env->NewObjectArray(parameterTypeInfos->size()-1, Java::Private::Runtime::Class.class_ref, nullptr);
            for(int i=1; i<parameterTypeInfos->size(); ++i){
                jni_env->SetObjectArrayElement(jparameters, i-1, parameterTypeInfos->at(i).javaClass());
            }
            if(slotMethod.methodType()==QMetaMethod::Signal){
                resolved_data->java_receiver =
                    Java::Private::QtJambi::QtJambiInternal.lookupSignal(jni_env,
                                                    resolved_data->java_receiver,
                                                    qtjambi_from_qstring(jni_env, QString::fromUtf8(slotMethod.name())), jparameters);
                if (resolved_data->java_receiver){
                    jclass receiver_class = jni_env->GetObjectClass(resolved_data->java_receiver);
                    jmethodID emitMethod = findEmitMethod(jni_env, receiver_class);
                    if(emitMethod)
                        resolved_data->java_method = env->ToReflectedMethod(receiver_class, emitMethod);
                }else{
                    resolved_data->java_method = nullptr;
                }
            }else{
                resolved_data->java_method =
                    Java::Private::QtJambi::QtJambiInternal.lookupSlot(jni_env,
                                                    resolved_data->java_receiver,
                                                    qtjambi_from_qstring(jni_env, QString::fromUtf8(slotMethod.name())),
                                                    jparameters);
            }
            jni_env->DeleteLocalRef(jparameters);
        }else{
            resolved_data->java_method =
                Java::Private::QtJambi::QtJambiInternal.lookupSlot(jni_env,
                                                resolved_data->java_receiver,
                                                qtjambi_from_qstring(jni_env, QString::fromLatin1(data->method + 1)),
                                                nullptr);
        }

        if (resolved_data->java_method == nullptr)
            return false;

    } else {
        resolved_data->java_method = nullptr;
    }

    return true;
}


static bool qtjambi_disconnect_all(JNIEnv *jni_env, const QSharedPointer<QtJambiLink>& java_link, QObject *receiver)
{
    bool result = false;
    if (java_link) {
        jobject java_sender = java_link->getJavaObjectRef(jni_env);
        // This (java_sender==nullptr) may for instance happens when a object has
        // disconnect() as part of its destructor
        if (java_sender) {
            jobject java_receiver = receiver ? qtjambi_from_QObject(jni_env, receiver) : nullptr;

            // It is allowed for the argument java_receiver==0
            result = Java::Private::QtJambi::QtJambiInternal.disconnectAll(jni_env, java_sender, java_receiver);
            java_link->releaseJavaObjectRef(jni_env, java_sender);
        }
    }
    return result;
}

static bool qtjambi_disconnect_callback(void **raw_data)
{
    Q_ASSERT(raw_data != nullptr);

    // ### try to avoid...
    if (QCoreApplication::closingDown())
        return false;

    JNIEnv *jni_env = qtjambi_current_environment();
    if(!jni_env)
        return false;

    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
    BasicConnectionData *data = reinterpret_cast<BasicConnectionData *>(raw_data);
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(data->sender).toStrongRef();
    Q_ASSERT(data->sender);
    // here, make sure on disconnect all to reconnect all native signals to java.
    if(link && (!data->receiver || data->receiver==data->sender) && !gIsInDisconnect->localData()){
        if(!link->reconnectBlocked()){
            gIsInDisconnect->setLocalData(true);
            QObject::disconnect(data->sender, data->signal, data->receiver, data->method);
            if(link->isSharedPointer()){
                static_cast<QSharedPointerToQObjectLink*>(link.data())->reconnect();
            }else{
                static_cast<PointerToQObjectLink*>(link.data())->reconnect();
            }
            gIsInDisconnect->setLocalData(false);
            return true;
        }
    }else if(gIsInDisconnectFromJava->localData()){
        return false;
    }
    if (data->method == nullptr && data->signal == nullptr) {
        qtjambi_disconnect_all(jni_env, link, data->receiver);
    } else {
        bool result = false;
        ResolvedConnectionData resolved_data;
        resolved_data.java_sender = nullptr;
        resolved_data.java_signal = nullptr;
        resolved_data.java_receiver = nullptr;
        resolved_data.java_method = nullptr;
        if (qtjambi_resolve_connection_data(jni_env, link, data, &resolved_data, nullptr, nullptr, false)
                && resolved_data.java_signal){
            Q_ASSERT(resolved_data.java_signal);
            bool _result = Java::Runtime::QtJambiSignals$AbstractSignal.removeConnectionFromCpp(jni_env, resolved_data.java_signal,
                                    resolved_data.java_receiver,
                                    resolved_data.java_method);
            if(data->signal && data->sender && data->receiver && data->method){
                result = _result;
            }
        }
        if(resolved_data.java_sender)
            jni_env->DeleteLocalRef(resolved_data.java_sender);
        if(resolved_data.java_signal)
            jni_env->DeleteLocalRef(resolved_data.java_signal);
        if(resolved_data.java_receiver)
            jni_env->DeleteLocalRef(resolved_data.java_receiver);
        if(resolved_data.java_method)
            jni_env->DeleteLocalRef(resolved_data.java_method);

        return result;
    }

    return false; // always disconnect through default impl. as well
}

struct ConnectData: public BasicConnectionData
{
    Qt::ConnectionType *type;
};

static bool qtjambi_connect_callback(void **raw_data)
{
    if(gIsInConnectFromJava()->localData())
        return false;
    Q_ASSERT(raw_data != nullptr);

    JNIEnv *jni_env = qtjambi_current_environment();
    if(!jni_env)
        return false;
    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
    static int NativeConnectionPolicy = Java::Private::QtJambi::QtJambiSignals.NativeConnectionPolicy(env);

    ConnectData *data = reinterpret_cast<ConnectData *>(raw_data);
    if (data->sender == nullptr || data->signal == nullptr || data->receiver == nullptr
        || data->method == nullptr  || data->type == nullptr) {
        qWarning("qtjambi_connect_callback(): received unexpected null parameters");
        return false;
    }

    // if methods exist for both, signal and slot, and if defining metaObjects are not dynamic --> use native connection
    QMetaMethod metaSignal = data->sender->metaObject()->method(data->sender->metaObject()->indexOfSignal(QMetaObject::normalizedSignature(data->signal + 1).data()));
    QMetaMethod metaMethod = data->receiver->metaObject()->method(data->receiver->metaObject()->indexOfSlot(QMetaObject::normalizedSignature(data->method + 1).data()));
    if(    metaSignal.isValid()
        && metaMethod.isValid() ) {
        switch(NativeConnectionPolicy){
        case 0: // connections made in java are solved in java. Connections made in cpp are solved in cpp if sender and receiver are not generated classes.
            if(    !qtjambi_metaobject_is_dynamic(metaSignal.enclosingMetaObject())
                && !qtjambi_metaobject_is_dynamic(metaMethod.enclosingMetaObject()) ){
                return false;
            }
            break;
        case 1: // all connections are created as in-java connections
            break;
        case 2: //only those connections are in-java connections whose sender and receiver is a generated Qt class
            if(    !qtjambi_metaobject_is_dynamic(metaSignal.enclosingMetaObject())
                && !qtjambi_metaobject_is_dynamic(metaMethod.enclosingMetaObject()) ){
                return false;
            }
            break;
        case 3: //only those connections are in-java connections whose either sender or receiver is a generated Qt class
            if(    !qtjambi_metaobject_is_dynamic(metaSignal.enclosingMetaObject())
                || !qtjambi_metaobject_is_dynamic(metaMethod.enclosingMetaObject()) ){
                return false;
            }
            break;
        case 4: // all connections are created as in-cpp connections
            return false;
        default:
            break;
        }
    }
    //     printf("   --- (connect callback) %s::%s -> %s::%s, %d\n",
    //            data->sender->metaObject()->className(), data->signal,
    //            data->receiver->metaObject()->className(), data->method,
    //            *data->type);
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(data->sender).toStrongRef();
    ResolvedConnectionData resolved_data;
    resolved_data.java_sender = nullptr;
    resolved_data.java_signal = nullptr;
    resolved_data.java_receiver = nullptr;
    resolved_data.java_method = nullptr;
    bool result = false;
    if (qtjambi_resolve_connection_data(jni_env, link, data, &resolved_data, &metaSignal, &metaMethod, true)
            && resolved_data.java_signal){
        Q_ASSERT(resolved_data.java_signal);
        jobject qconnection = jni_env->CallObjectMethod(resolved_data.java_signal,
                             Java::Runtime::QtJambiSignals$AbstractSignal.addConnection,
                             resolved_data.java_receiver,
                             resolved_data.java_method,
                             *data->type);
        result = qconnection!=nullptr;
        if(qconnection)
            jni_env->DeleteLocalRef(qconnection);
        qtjambi_throw_java_exception(jni_env);
    }
    if(resolved_data.java_sender)
        jni_env->DeleteLocalRef(resolved_data.java_sender);
    if(resolved_data.java_signal)
        jni_env->DeleteLocalRef(resolved_data.java_signal);
    if(resolved_data.java_receiver)
        jni_env->DeleteLocalRef(resolved_data.java_receiver);
    if(resolved_data.java_method)
        jni_env->DeleteLocalRef(resolved_data.java_method);

    // If we succeeded in connecting in Java, we skip the C++ connection
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_nativeConnecting)
    (JNIEnv *, jclass, jboolean c)
{
    gIsInConnectFromJava->setLocalData(c);
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_nativeDisconnecting)
    (JNIEnv *, jclass, jboolean c)
{
    gIsInDisconnectFromJava->setLocalData(c);
}

#else
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_nativeConnecting)(JNIEnv *, jclass, jboolean){}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_nativeDisconnecting)(JNIEnv *, jclass, jboolean){}
#endif

static bool qtjambi_event_notify(void **data)
{
    if(QObject *receiver = reinterpret_cast<QObject *>(data[0])){
        QEvent *event = reinterpret_cast<QEvent *>(data[1]);
        QThread *currentThread = QThread::currentThread();
        QThread *thr = receiver->thread();
        if(thr && currentThread != thr){
            switch (event->type()) {
            case QEvent::ApplicationFontChange:
            case QEvent::ApplicationPaletteChange:
            case QEvent::ApplicationLayoutDirectionChange:
            case QEvent::ApplicationStateChange:
            case QEvent::ApplicationWindowIconChange:
                QCoreApplication::postEvent(receiver, new QEvent(event->type()));
                return true;
            default:
                break;
            }
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(receiver).toStrongRef()) {
                if (JNIEnv *env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    QMetaEnum enm = QMetaEnum::fromType<QEvent::Type>();
                    QString eventDescr;
                    if(enm.isValid()){
                        eventDescr = QLatin1String(enm.valueToKeys(event->type()));
                    }else{
                        eventDescr = QString::number(int(event->type()));
                    }
                    JavaException::raiseQThreadAffinityException(env, qPrintable(QString("Cannot send events to objects owned by a different thread (event type: %1).").arg(eventDescr)) QTJAMBI_STACKTRACEINFO ,
                                                                 link->getJavaObjectLocalRef(env),
                                                                 thr, currentThread);
                    return true;
                }
            }
        }
        switch (event->type()) {
        case QEvent::ChildAdded:
        case QEvent::ChildRemoved:
            {
                QChildEvent *e = static_cast<QChildEvent *>(event);

                // We're not interested in objects that don't have a link as the GC won't
                // be interferring with them anyway.
                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(e->child()).toStrongRef()) {
                    if (link->qobject() || !link->isInitialized()) {
                        if (JNIEnv *env = qtjambi_current_environment()){
                            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                            if (e->added())
                               link->setCppOwnership(env);
                            else
                               link->setDefaultOwnership(env);
                        }
                    } else if (event->type() == QEvent::ChildAdded) {
                        qWarning("'%s' [%s] was garbage collected before it was reparented to '%s' [%s]",
                             qPrintable(e->child()->objectName()), e->child()->metaObject()->className(),
                             qPrintable(receiver->objectName()), receiver->metaObject()->className());
                    }
                }
            }
            break;
        case QEvent::DeferredDelete:
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(receiver).toStrongRef()) {
                if (link->qobject()) {
                    if (JNIEnv *env = qtjambi_current_environment()){
                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                        link->setCppOwnership(env);
                    }
                }
            }
            break;

        default:
            break;
        }
    }
    return false;
}

void registerTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, EntryTypes entryTypes);
void registerTypeAlias(const std::type_info& typeId, const char *qt_name, const char *java_name);

template<typename T>
const std::type_info& registerSpecialTypeInfo(const char *qt_name, const char *java_name)
{
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
    const std::type_info& id = typeid(T);
    registerTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, EntryTypes::StringTypeInfo);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerPrimitiveTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = typeid(T);
    registerTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, EntryTypes::PrimitiveTypeInfo);
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    registerSizeOfType(id, sizeof(T));
    registerOperators<T>();
    return id;
}

template<typename T>
const std::type_info& registerPrimitiveMetaTypeInfo(const char *qt_name, const char *java_name)
{
    const std::type_info& id = registerPrimitiveTypeInfo<T>(qt_name, java_name);
    registerMetaTypeID(id, qRegisterMetaType<T>(qt_name));
    registerOperators<T>();
    return id;
}

void registerContainerTypeInfo(const std::type_info& typeId, const QtJambiTypeInfo& info, const char *qt_name, const char *java_name, const char *java_interface);

template<typename T>
const std::type_info& registerContainerTypeInfo(const char *qt_name, const char *java_name, const char *java_interface)
{
    const std::type_info& id = typeid(T);
    registerContainerTypeInfo(id, QtJambiTypeInfo::of<T>(), qt_name, java_name, java_interface);
    registerSizeOfType(id, sizeof(T));
    registerAlignmentOfType(id, Q_ALIGNOF(T));
    //registerOperators<T>();
    return id;
}

QObject* connectionSender(const QMetaObject::Connection* connection);

#ifdef QT_QTJAMBI_PORT
int __signal_method_indexes_QThread0();
int __signal_method_indexes_QThread1();
#endif

void qtjambi_startup(JNIEnv * __jni_env){
    try{
//        qAddPostRoutine(&qtjambi_shutdown);
        QInternal::registerCallback(QInternal::EventNotifyCallback, qtjambi_event_notify);
#ifdef QT_QTJAMBI_PORT
        QInternal::registerCallback(QInternal::ConnectCallback,    qtjambi_connect_callback);
        QInternal::registerCallback(QInternal::DisconnectCallback, qtjambi_disconnect_callback);
        QInternal::registerCallback(QInternal::AdoptCurrentThread, qtjambi_adopt_current_thread);
        qtjambi_thread_factory->data();
#endif
        QtJambiVariant::registerHandler();
#if QT_VERSION >= 0x050C00
        registerSpecialTypeInfo<QCborValueRef>("QCborValueRef", "io/qt/core/QCborValue");
#endif
        registerSpecialTypeInfo<JObjectWrapper>("JObjectWrapper", "java/lang/Object");
        registerSpecialTypeInfo<std::nullptr_t>("std::nullptr_t", "java/lang/Object");
        registerMetaType<std::nullptr_t>("std::nullptr_t");
        registerSpecialTypeInfo<QVariant>("QVariant", "java/lang/Object");
        registerTypeAlias(typeid(QVariant), nullptr, "io/qt/core/QVariant");
        registerMetaType<QVariant>("QVariant");
        registerMetaType<JObjectWrapper>("JObjectWrapper");
        registerMetaType<JEnumWrapper>("JEnumWrapper");
        registerMetaType<JIteratorWrapper>("JIteratorWrapper");
        registerMetaType<JCollectionWrapper>("JCollectionWrapper");
        registerMetaType<JMapWrapper>("JMapWrapper");
        registerMetaType<QMap<QVariant,QVariant> >("QMap<QVariant,QVariant>");
        registerSpecialTypeInfo<JEnumWrapper>("JEnumWrapper", "java/lang/Enum");
        registerSpecialTypeInfo<JIteratorWrapper>("JIteratorWrapper", "java/util/Iterator");
        registerSpecialTypeInfo<JCollectionWrapper>("JCollectionWrapper", "java/util/Collection");
        registerSpecialTypeInfo<JMapWrapper>("JMapWrapper", "java/util/Map");
        QMetaType::registerEqualsComparator<JEnumWrapper>();
        QMetaType::registerEqualsComparator<JCollectionWrapper>();
        QMetaType::registerEqualsComparator<JObjectWrapper>();
        QMetaType::registerEqualsComparator<JIteratorWrapper>();
        QMetaType::registerEqualsComparator<JMapWrapper>();
        //qRegisterMetaTypeStreamOperators<JCollectionWrapper>();
        QMetaType::registerConverter<JEnumWrapper,qint32>(&JEnumWrapper::ordinal);
        QMetaType::registerConverter<JCollectionWrapper,QList<QVariant>>(&JCollectionWrapper::toList);
        QMetaType::registerConverter<JCollectionWrapper,QStringList>(&JCollectionWrapper::toStringList);
        QMetaType::registerConverter<JCollectionWrapper,JObjectWrapper>([](JCollectionWrapper w) -> JObjectWrapper {return w;});
        QMetaType::registerConverter<JIteratorWrapper,JObjectWrapper>([](JIteratorWrapper w) -> JObjectWrapper {return w;});
        QMetaType::registerConverter<JMapWrapper,JObjectWrapper>([](JMapWrapper w) -> JObjectWrapper {return w;});
        QMetaType::registerConverter<JCollectionWrapper,QString>([](JCollectionWrapper w) -> QString {return w.toString();});
        QMetaType::registerConverter<JIteratorWrapper,QString>([](JIteratorWrapper w) -> QString {return w.toString();});
        QMetaType::registerConverter<JMapWrapper,QString>([](JMapWrapper w) -> QString {return w.toString();});
        QMetaType::registerConverter<JMapWrapper,QMap<QVariant,QVariant>>(&JMapWrapper::toMap);
        QMetaType::registerConverter<JMapWrapper,QMap<QString,QVariant>>(&JMapWrapper::toStringMap);
        QMetaType::registerConverter<JMapWrapper,QHash<QString,QVariant>>(&JMapWrapper::toStringHash);
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

#ifdef QT_QTJAMBI_PORT
        {
            const std::type_info& threadTypeId = typeid(QThread);
            registerQObjectTypeInfo(threadTypeId, "QThread", "io/qt/core/QThread$Wrapper");
            registerMetaObject(threadTypeId, QThread::staticMetaObject, &__qt_QThread_connect_signals, QVector<SignalMetaInfo>()
                                        << SignalMetaInfo("io/qt/core/QThread$Wrapper", "started", "Lio/qt/core/QObject$PrivateSignal0;", nullptr /* private signal */, 0, &__signal_method_indexes_QThread0)
                                        << SignalMetaInfo("io/qt/core/QThread$Wrapper", "finished", "Lio/qt/core/QObject$PrivateSignal0;", nullptr /* private signal */, 0, &__signal_method_indexes_QThread1)
                                    );
        }
#endif

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
        registerPrimitiveMetaTypeInfo<float>("float", "float");
        registerPrimitiveMetaTypeInfo<double>("double", "double");
        registerPrimitiveMetaTypeInfo<bool>("bool", "boolean");
        registerPrimitiveMetaTypeInfo<int>("int", "int");
        registerPrimitiveMetaTypeInfo<char>("char", "byte");
        registerPrimitiveMetaTypeInfo<long long>("long long", "long");
        registerPrimitiveMetaTypeInfo<short>("short", "short");
        registerPrimitiveMetaTypeInfo<QChar>("QChar", "char");
        registerPrimitiveTypeInfo<QLatin1Char>("QLatin1Char", "char");

        registerStringTypeInfo<QString>("QString", "java/lang/String");
        registerMetaType<QString>("QString");

        registerStringTypeInfo<QXmlStreamStringRef>("QXmlStreamStringRef", "java/lang/String");
        registerMetaType<QXmlStreamStringRef>("QXmlStreamStringRef");

        registerStringTypeInfo<QLatin1String>("QLatin1String", "java/lang/String");

        registerStringTypeInfo<QStringRef>("QStringRef", "java/lang/String");
        registerMetaType<QStringRef>("QStringRef");

        registerStringTypeInfo<QStringView>("QStringView", "java/lang/String");
        registerMetaType<QStringView>("QStringView");

        registerSpecialTypeInfo<QModelIndex>("QModelIndex", "io/qt/core/QModelIndex");
        registerMetaType<QModelIndex>("QModelIndex");

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
            registerOwnerFunction(typeId, [](void* p) -> const QObject* {
                return connectionSender(reinterpret_cast<const QMetaObject::Connection*>(p));
            });
        }

        registerTypeInfo<QMetaType>("QMetaType", "io/qt/core/QMetaType");
        registerOperators<QMetaType>();

        {
            registerSpecialTypeInfo<QUrl::FormattingOptions>("QUrlTwoFlags<QUrl::UrlFormattingOption,QUrl::ComponentFormattingOption>", "io/qt/core/QUrl$FormattingOptions");
            registerTypeAlias(typeid(QUrl::FormattingOptions), "QUrl::FormattingOptions", nullptr);
            int id = registerMetaType<QUrl::FormattingOptions>("QUrlTwoFlags<QUrl::UrlFormattingOption,QUrl::ComponentFormattingOption>");
#ifndef QT_NO_DATASTREAM
            QMetaType::registerStreamOperators(id, qGenericSaveEnumerator<QUrl::FormattingOptions>, qGenericLoadEnumerator<QUrl::FormattingOptions>);
#endif // QT_NO_DATASTREAM
            QMetaType::registerTypedef("QList<QString>", QMetaType::QStringList);
            QMetaType::registerTypedef("QList<QByteArray>", QMetaType::QByteArrayList);
            QMetaType::registerTypedef("QList<QVariant>", QMetaType::QVariantList);
            QMetaType::registerTypedef("QMap<QString,QVariant>", QMetaType::QVariantMap);
            QMetaType::registerTypedef("QHash<QString,QVariant>", QMetaType::QVariantHash);
            registerContainerTypeInfo<QHash<QVariant,QVariant>>("QHash", "io/qt/core/QHash", "java/util/Map");
            registerContainerTypeInfo<QMultiHash<QVariant,QVariant>>("QMultiHash", "io/qt/core/QMultiHash", "java/util/Map");
            registerContainerTypeInfo<QMultiMap<QVariant,QVariant>>("QMultiMap", "io/qt/core/QMultiMap", "java/util/NavigableMap");
            registerContainerTypeInfo<QMap<QVariant,QVariant>>("QMap", "io/qt/core/QMap", "java/util/NavigableMap");
            registerContainerTypeInfo<QList<QVariant>::const_iterator>("QIterator", "io/qt/core/QIterator", "java/lang/Iterable");
            registerContainerTypeInfo<QMap<QVariant,QVariant>::const_iterator>("QMapIterator", "io/qt/core/QMapIterator", "java/lang/Iterable");
            registerContainerTypeInfo<QLinkedList<QVariant>>("QLinkedList", "io/qt/core/QLinkedList", "java/util/Collection");
            registerContainerTypeInfo<QList<QVariant>>("QList", "io/qt/core/QList", "java/util/List");
            registerContainerTypeInfo<QQueue<QVariant>>("QQueue", "io/qt/core/QQueue", "java/util/Queue");
            registerContainerTypeInfo<QSet<QVariant>>("QSet", "io/qt/core/QSet", "java/util/Set");
            registerContainerTypeInfo<QVector<QVariant>>("QVector", "io/qt/core/QVector", "java/util/List");
            registerContainerTypeInfo<QStack<QVariant>>("QStack", "io/qt/core/QStack", "java/util/Deque");
        }

        if(__jni_env){
            jobject __java_libraryPaths = Java::Private::QtJambi::QtJambiInternal.getLibraryPaths(__jni_env);
            // don't use qtjambi_cast here!
            jobject __qt__iterator = qtjambi_collection_iterator(__jni_env, __java_libraryPaths);
            while(qtjambi_iterator_has_next(__jni_env, __qt__iterator)) {
                jobject __java_element = qtjambi_iterator_next(__jni_env, __qt__iterator);
                QCoreApplication::addLibraryPath(qtjambi_to_qstring(__jni_env, jstring(__java_element)));
            }
        }
    }catch(const JavaException& e){
        if(__jni_env)
            e.raiseInJava(__jni_env);
        else
            qWarning("%s", e.what());
    }
}

void clear_typehandlers_at_shutdown();
void clear_polymorphic_handlers_at_shutdown();
void clear_supertypes_at_shutdown();
void clear_repository_at_shutdown();
void clear_registry_at_shutdown(JNIEnv * env);
void clear_metaobjects_at_shutdown();

void qtjambi_shutdown(JNIEnv * env)
{
    JavaException exception;
    static bool invoked = [](JNIEnv * env, JavaException &exception) -> bool {
        if(env){
            try{
                Java::Private::QtCore::QMessageHandler.shutdown(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
            try{
                Java::Private::Runtime::System.gc(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
            try{
                Java::Private::QtJambi::QtJambiInternal.shutdown(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
        }
    #ifdef QT_QTJAMBI_PORT
        QInternal::unregisterCallback(QInternal::AdoptCurrentThread, qtjambi_adopt_current_thread);
        QInternal::unregisterCallback(QInternal::DisconnectCallback, qtjambi_disconnect_callback);
        QInternal::unregisterCallback(QInternal::ConnectCallback,    qtjambi_connect_callback);
    #endif

    #if QT_VERSION >= 0x040300
        QInternal::unregisterCallback(QInternal::EventNotifyCallback, qtjambi_event_notify);
    #endif

        QtJambiVariant::unregisterHandler();
    #ifdef QT_QTJAMBI_PORT
        if(qtjambi_thread_factory){
            qtjambi_thread_factory->reset();
        }
    #endif

        clear_metaobjects_at_shutdown();
        clear_typehandlers_at_shutdown();
        clear_polymorphic_handlers_at_shutdown();
        clear_supertypes_at_shutdown();
        clear_repository_at_shutdown();
        clear_registry_at_shutdown(env);
        {
            QWriteLocker locker(qtjambi_vm_lock());
            Q_UNUSED(locker)
            qtjambi_vm = nullptr;
        }
        return true;
    }(env, exception);
    Q_UNUSED(invoked)
    exception.raise();
}

/*!
 * This function is called by Qt Jambi shutdown hook to indicate that
 * the library is being unloaded. It prevents any access to the
 * current JNIEnv *.
 */
void qtjambi_shutdown()
{
    qtjambi_shutdown(qtjambi_current_environment(false));
}

const char* getInterfaceForIID(const char*interface_iid);

#ifndef QT_NO_DEBUG

void qtjambi_debug_trace(const char *location, const char *file, int line)
{
    static bool should = getenv("QTJAMBI_DEBUG_TRACE") != nullptr;
    if (should) {
        fprintf(stderr, "%s in THREAD %p ( %s:%d )\n", location, reinterpret_cast<void*>(QThread::currentThreadId()), file, line);
        fflush(stderr);
    }
}

QtJambiDebugMethodPrint::QtJambiDebugMethodPrint(const char* enterMessage, const char* leaveMethod, const char* file, int line) :
    m_enterMessage(enterMessage),
    m_leaveMethod(leaveMethod),
    m_file(file),
    m_line(line) {
    qtjambi_debug_trace(m_enterMessage, m_file, m_line);
}
QtJambiDebugMethodPrint::~QtJambiDebugMethodPrint(){
    qtjambi_debug_trace(m_leaveMethod, m_file, m_line);
}

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
void qtjambi_debug_trace_with_type(const char *location, const char * typeName, const char *file, int line)
{
    static bool should = getenv("QTJAMBI_DEBUG_TRACE") != nullptr;
    if (should) {
        if(typeName){
            fprintf(stderr, "%s on object %s in THREAD %p ( %s:%d )\n", location, typeName, reinterpret_cast<void*>(QThread::currentThreadId()), file, line);
        }else{
            fprintf(stderr, "%s in THREAD %p ( %s:%d )\n", location, reinterpret_cast<void*>(QThread::currentThreadId()), file, line);
        }
        fflush(stderr);
    }
}

QtJambiDebugMethodPrintWithType::QtJambiDebugMethodPrintWithType(const char* enterMessage, const char* leaveMethod, const QtJambiShell* shell, const char* file, int line) :
    m_enterMessage(enterMessage),
    m_leaveMethod(leaveMethod),
    m_type(nullptr),
    m_file(file),
    m_line(line) {
    if(QSharedPointer<QtJambiLink> link = static_cast<const QtJambiShellImpl*>(shell)->link()){
        m_type = link->qtTypeName();
    }
    qtjambi_debug_trace_with_type(m_enterMessage, m_type, m_file, m_line);
}
QtJambiDebugMethodPrintWithType::~QtJambiDebugMethodPrintWithType(){
    qtjambi_debug_trace_with_type(m_leaveMethod, m_type, m_file, m_line);
}
#endif
#endif

void qtjambi_assert(const char *assertion, const char *file, int line){
#ifndef QT_NO_DEBUG
    qtjambi_debug_trace(assertion, file, line);
#endif
    qt_assert(assertion, file, line);
}

size_t qtjambi_value_size_for_class(JNIEnv *env, jclass object_class)
{
    if(Java::Private::QtJambi::QFlags.isAssignableFrom(env, object_class)){
        enum E{X};
        return sizeof(QFlags<E>);
    }
    if(Java::Private::QtJambi::QtEnumerator.isAssignableFrom(env, object_class)){
        return sizeof(qint32);
    }
    if(Java::Private::QtJambi::QtShortEnumerator.isAssignableFrom(env, object_class)){
        return sizeof(qint16);
    }
    if(Java::Private::QtJambi::QtByteEnumerator.isAssignableFrom(env, object_class)){
        return sizeof(qint8);
    }
    if(Java::Private::QtJambi::QtLongEnumerator.isAssignableFrom(env, object_class)){
        return sizeof(qint64);
    }
    if(Java::Private::Runtime::Enum.isAssignableFrom(env, object_class)){
        return sizeof(qint32);
    }

    size_t result = 0;
    if(jclass cls = resolveClosestQtSuperclass(env, object_class, nullptr)){
        QString javaClassName = qtjambi_class_name(env, cls).replace(".", "/");
        if(const std::type_info* qtClassName = getTypeByJavaName(javaClassName)){
            result = getValueSize(*qtClassName);
        }
    }
    return result;
}

size_t qtjambi_extended_size_for_class(JNIEnv *env, jclass object_class)
{
    size_t size = 0;
    const SuperTypeInfos& infos = getSuperTypeInfos(env, object_class);
    if(!infos.isEmpty()){
        for(const SuperTypeInfo& info : infos){
            size += info.size;
            size += sizeof(void*);
        }
    }
    return size;
}

size_t qtjambi_shell_size_for_class(JNIEnv *env, jclass object_class)
{
    size_t result = 0;
    if(jclass cls = resolveClosestQtSuperclass(env, object_class, nullptr)){
        QString javaClassName = qtjambi_class_name(env, cls).replace(".", "/");
        if(const std::type_info* qtClassName = getTypeByJavaName(javaClassName)){
            result = getShellSize(*qtClassName);
        }
    }
    return result;
}

bool qtjambi_metaobject_is_dynamic(const QMetaObject *meta_object) {
    return QtDynamicMetaObject::isDynamic(meta_object);
}

bool JObjectWrapper::operator==(const JObjectWrapper &other) const
{
    jobject myObject = object();
    jobject otherObject = other.object();
    if (!myObject && !otherObject){
        return true;
    }else if (!myObject || !otherObject){
        return false;
    }else if(JNIEnv *env = qtjambi_current_environment()){
        Q_ASSERT(Java::Private::Runtime::Object.isInstanceOf(env, myObject));  // check the java object is right type (objects on JVM don't have to inherit java.lang.Object)
        return Java::Private::Runtime::Object.equals(env, myObject, otherObject);
    }else{
        return false;
    }
}

bool JObjectWrapper::operator<(const JObjectWrapper &other) const
{
    jobject myObject = object();
    jobject otherObject = other.object();
    if (!myObject && !otherObject){
        return false;
    }else if (!myObject || !otherObject){
        return false;
    }else if(JNIEnv *env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        if(Java::Private::Runtime::Comparable.isInstanceOf(env, myObject)){  // check the java object is right type (objects on JVM don't have to inherit java.lang.Object)
            try{
                return Java::Private::Runtime::Comparable.compareTo(env, myObject, otherObject)<0;
            }catch(const JavaException&){
            }
            return false;
        }else if(Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, myObject) && Java::Private::QtJambi::QtEnumerator.isInstanceOf(env, otherObject)){
            jint h1 = Java::Private::QtJambi::QtEnumerator.value(env, myObject);
            jint h2 = Java::Private::QtJambi::QtEnumerator.value(env, otherObject);
            return h1<h2;
        }else if(Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, myObject) && Java::Private::QtJambi::QtShortEnumerator.isInstanceOf(env, otherObject)){
            jshort h1 = Java::Private::QtJambi::QtShortEnumerator.value(env, myObject);
            jshort h2 = Java::Private::QtJambi::QtShortEnumerator.value(env, otherObject);
            return h1<h2;
        }else if(Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, myObject) && Java::Private::QtJambi::QtByteEnumerator.isInstanceOf(env, otherObject)){
            jbyte h1 = Java::Private::QtJambi::QtByteEnumerator.value(env, myObject);
            jbyte h2 = Java::Private::QtJambi::QtByteEnumerator.value(env, otherObject);
            return h1<h2;
        }else if(Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, myObject) && Java::Private::QtJambi::QtLongEnumerator.isInstanceOf(env, otherObject)){
            jlong h1 = Java::Private::QtJambi::QtLongEnumerator.value(env, myObject);
            jlong h2 = Java::Private::QtJambi::QtLongEnumerator.value(env, otherObject);
            return h1<h2;
        }else if(Java::Private::Runtime::Enum.isInstanceOf(env, myObject) && Java::Private::Runtime::Enum.isInstanceOf(env, otherObject)){
            jint h1 = Java::Private::Runtime::Enum.ordinal(env, myObject);
            jint h2 = Java::Private::Runtime::Enum.ordinal(env, otherObject);
            return h1<h2;
        }
        jint h1 = Java::Private::Runtime::System.identityHashCode(env, myObject);
        jint h2 = Java::Private::Runtime::System.identityHashCode(env, otherObject);
        return h1<h2;
    }else{
        return false;
    }
}

bool JEnumWrapper::operator==(const JEnumWrapper &other) const
{
    return JObjectWrapper::operator==(static_cast<const JObjectWrapper &>(other));
}

bool JIteratorWrapper::operator==(const JIteratorWrapper &other) const
{
    return JObjectWrapper::operator==(static_cast<const JObjectWrapper &>(other));
}

bool JCollectionWrapper::operator==(const JCollectionWrapper &other) const
{
    return JObjectWrapper::operator==(static_cast<const JObjectWrapper &>(other));
}

bool JMapWrapper::operator==(const JMapWrapper &other) const
{
    return JObjectWrapper::operator==(static_cast<const JObjectWrapper &>(other));
}

bool JEnumWrapper::operator<(const JEnumWrapper &other) const
{
    return JObjectWrapper::operator<(static_cast<const JObjectWrapper &>(other));
}

bool JIteratorWrapper::operator<(const JIteratorWrapper &other) const
{
    return JObjectWrapper::operator<(static_cast<const JObjectWrapper &>(other));
}

bool JCollectionWrapper::operator<(const JCollectionWrapper &other) const
{
    return JObjectWrapper::operator<(static_cast<const JObjectWrapper &>(other));
}

bool JMapWrapper::operator<(const JMapWrapper &other) const
{
    return JObjectWrapper::operator<(static_cast<const JObjectWrapper &>(other));
}

JObjectWrapper::JObjectWrapper() : m_data()
{
}

JObjectWrapper::JObjectWrapper(const JObjectWrapper &wrapper) : m_data(wrapper.m_data)
{
}

JObjectWrapper::JObjectWrapper(JNIEnv *env, jobject obj, bool globalRefs) : m_data()
{
    if(obj){
        REF_JOBJECT;
        m_data.reset(globalRefs ? static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, obj)) : static_cast<JObjectWrapperData*>(new JObjectWeakWrapperData(env, obj)));
    }
}

JObjectWrapper& JObjectWrapper::operator=(const JObjectWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JMapWrapper& JMapWrapper::operator=(const JObjectWrapper &wrapper) {
    JObjectWrapper::operator=(wrapper);
    return *this;
}

JMapWrapper& JMapWrapper::operator=(const JMapWrapper &wrapper) {
    JObjectWrapper::operator=(wrapper);
    return *this;
}

JCollectionWrapper& JCollectionWrapper::operator=(const JObjectWrapper &wrapper) {
    JObjectWrapper::operator=(wrapper);
    return *this;
}

JCollectionWrapper& JCollectionWrapper::operator=(const JCollectionWrapper &wrapper) {
    JObjectWrapper::operator=(wrapper);
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(const JObjectWrapper &wrapper) {
    JObjectWrapper::operator=(wrapper);
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(const JIteratorWrapper &wrapper) {
    JObjectWrapper::operator=(wrapper);
    return *this;
}

JEnumWrapper& JEnumWrapper::operator=(const JObjectWrapper &wrapper) {
    JObjectWrapper::operator=(wrapper);
    return *this;
}

JEnumWrapper& JEnumWrapper::operator=(const JEnumWrapper &wrapper) {
    JObjectWrapper::operator=(wrapper);
    return *this;
}

jobject JObjectWrapper::object() const{
    return m_data && m_data.data() ? m_data.data()->data() : nullptr;
}

JObjectWrapper::~JObjectWrapper(){}

JObjectWrapper qtjambi_to_jobjectwrapper(JNIEnv *env, jobject java_object)
{
    return JObjectWrapper(env, java_object);
}

jobject qtjambi_from_jobjectwrapper(JNIEnv *env, const JObjectWrapper &wrapper)
{
    return env->NewLocalRef(wrapper.object());
}

QVarLengthArray<jvalue> qtjambi_from_jobjectArray(JNIEnv *env,
                                                  jobjectArray args,
                                                  jintArray _cnvTypes,
                                                  bool globalRefs)
{
    Q_ASSERT(_cnvTypes);
    jsize len = env->GetArrayLength(_cnvTypes);
    jint *cnvTypes = env->GetIntArrayElements(_cnvTypes, nullptr);
    QVarLengthArray<jvalue> argsArray(len);
    Q_ASSERT(args);
    for (int i=0; i<len; ++i) {
        jobject arg_object = env->GetObjectArrayElement(args, i);
        switch (cnvTypes[i]) {
        case 'L': argsArray[i].l = globalRefs ? env->NewGlobalRef(arg_object) : arg_object; break ;
        case 'Z': argsArray[i].z = qtjambi_to_boolean(env, arg_object); break ;
        case 'J': argsArray[i].j = qtjambi_to_long(env, arg_object); break ;
        case 'I': argsArray[i].i = qtjambi_to_int(env, arg_object); break ;
        case 'F': argsArray[i].f = qtjambi_to_float(env, arg_object); break ;
        case 'D': argsArray[i].d = qtjambi_to_double(env, arg_object); break ;
        case 'S': argsArray[i].s = qtjambi_to_short(env, arg_object); break ;
        case 'B': argsArray[i].b = qtjambi_to_byte(env, arg_object); break ;
        case 'C': argsArray[i].c = qtjambi_to_jchar(env, arg_object); break ;
        default:
            Q_ASSERT_X(false, "qtjambi_invoke_slot", "Error in conversion array");
        }
    }
    env->ReleaseIntArrayElements(_cnvTypes, cnvTypes, JNI_ABORT);

    return argsArray;
}

jobject qtjambi_invoke_method(JNIEnv *env,
                            jobject receiver,
                            jmethodID methodId,
                            jbyte returnType,
                            QVarLengthArray<jvalue> argsArray)
{
    jobject result = nullptr;
    switch (returnType)
    {
    case 'L': result = env->CallObjectMethodA(receiver, methodId, argsArray.data()); break;
    case 'V': env->CallVoidMethodA(receiver, methodId, argsArray.data()); break;
    case 'I': result = qtjambi_from_int(env, env->CallIntMethodA(receiver, methodId, argsArray.data())); break;
    case 'J': result = qtjambi_from_long(env, env->CallLongMethodA(receiver, methodId, argsArray.data())); break;
    case 'S': result = qtjambi_from_short(env, env->CallShortMethodA(receiver, methodId, argsArray.data())); break;
    case 'Z': result = qtjambi_from_boolean(env, env->CallBooleanMethodA(receiver, methodId, argsArray.data())); break;
    case 'F': result = qtjambi_from_float(env, env->CallFloatMethodA(receiver, methodId, argsArray.data())); break;
    case 'D': result = qtjambi_from_double(env, env->CallDoubleMethodA(receiver, methodId, argsArray.data())); break;
    case 'B': result = qtjambi_from_byte(env, env->CallByteMethodA(receiver, methodId, argsArray.data())); break;
    case 'C': result = qtjambi_from_char(env, env->CallCharMethodA(receiver, methodId, argsArray.data())); break;
    default:
        Q_ASSERT_X(false, "qtjambi_invoke_slot", "Invalid return type parameter");
        break;
    }
    qtjambi_throw_java_exception(env)
    return result;
}

jobject qtjambi_invoke_static_method(JNIEnv *env,
                            jclass clazz,
                            jmethodID methodId,
                            jbyte returnType,
                            QVarLengthArray<jvalue> argsArray)
{
    jobject result = nullptr;
    switch (returnType)
    {
    case 'L': result = env->CallStaticObjectMethodA(clazz, methodId, argsArray.data()); break;
    case 'V': env->CallStaticVoidMethodA(clazz, methodId, argsArray.data()); break;
    case 'I': result = qtjambi_from_int(env, env->CallStaticIntMethodA(clazz, methodId, argsArray.data())); break;
    case 'J': result = qtjambi_from_long(env, env->CallStaticLongMethodA(clazz, methodId, argsArray.data())); break;
    case 'S': result = qtjambi_from_short(env, env->CallStaticShortMethodA(clazz, methodId, argsArray.data())); break;
    case 'Z': result = qtjambi_from_boolean(env, env->CallStaticBooleanMethodA(clazz, methodId, argsArray.data())); break;
    case 'F': result = qtjambi_from_float(env, env->CallStaticFloatMethodA(clazz, methodId, argsArray.data())); break;
    case 'D': result = qtjambi_from_double(env, env->CallStaticDoubleMethodA(clazz, methodId, argsArray.data())); break;
    case 'B': result = qtjambi_from_byte(env, env->CallStaticByteMethodA(clazz, methodId, argsArray.data())); break;
    case 'C': result = qtjambi_from_char(env, env->CallStaticCharMethodA(clazz, methodId, argsArray.data())); break;
    default:
        Q_ASSERT_X(false, "qtjambi_invoke_slot", "Invalid return type parameter");
        break;
    }
    qtjambi_throw_java_exception(env)
    return result;
}

qint32 JEnumWrapper::ordinal() const {
    jobject _object = object();
    if(_object){
        JNIEnv *env = qtjambi_current_environment();
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        return Java::Private::Runtime::Enum.ordinal(env, _object);
    }else{
        return -1;
    }
}

int JCollectionWrapper::size() const {
    jobject _object = object();
    if(_object){
        JNIEnv *env = qtjambi_current_environment();
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        return Java::Private::Runtime::Collection.size(env, _object);
    }else{
        return 0;
    }
}

JIteratorWrapper JCollectionWrapper::iterator() const {
    jobject _object = object();
    if(_object){
        JNIEnv *env = qtjambi_current_environment();
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        jobject iter = Java::Private::Runtime::Collection.iterator(env, _object);
        return JIteratorWrapper(env, iter);
    }else{
        return JIteratorWrapper();
    }
}

QList<QVariant> JCollectionWrapper::toList() const {
    QList<QVariant> list;
    if(object()){
        for (JIteratorWrapper iter = iterator(); iter.hasNext();) {
            list << iter.next();
        }
    }
    return list;
}

QStringList JCollectionWrapper::toStringList(bool * ok) const {
    QStringList list;
    if(object()){
        JNIEnv *env = qtjambi_current_environment();
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        for (JIteratorWrapper iter = iterator(); iter.hasNext();) {
            jobject o = iter._next();
            if(ok && o && !Java::Runtime::String.isInstanceOf(env, o)){
                ok[0] = false;
                list.clear();
                return list;
            }
            list << qtjambi_to_qvariant(env, o).toString();
        }
    }
    if(ok) ok[0] = true;
    return list;
}

QString JObjectWrapper::toString() const {
    jobject _object = object();
    if(_object){
        JNIEnv *env = qtjambi_current_environment();
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        return qtjambi_to_qstring(env, _object);
    }
    return "null";
}

bool JIteratorWrapper::hasNext() const {
    jobject _object = object();
    if(_object){
        JNIEnv *env = qtjambi_current_environment();
        return Java::Private::Runtime::Iterator.hasNext(env, _object);
    }else{
        return false;
    }
}

QVariant JIteratorWrapper::next() const {
    return qtjambi_to_qvariant(qtjambi_current_environment(), _next());
}

jobject JIteratorWrapper::_next() const {
    jobject _object = object();
    if(_object){
        JNIEnv *env = qtjambi_current_environment();
        return Java::Private::Runtime::Iterator.next(env, _object);
    }else{
        return jobject();
    }
}

jobject JMapWrapper::_entrySet() const {
    jobject _object = object();
    if(_object){
        JNIEnv *env = qtjambi_current_environment();
        return Java::Private::Runtime::Map.entrySet(env, _object);
    }else{
        return jobject();
    }
}

JCollectionWrapper JMapWrapper::entrySet() const {
    return JCollectionWrapper(qtjambi_current_environment(), _entrySet());
}

QMap<QVariant,QVariant> JMapWrapper::toMap() const {
    QMap<QVariant,QVariant> map;
    JNIEnv *env = qtjambi_current_environment();
    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
    for(JIteratorWrapper iter = entrySet().iterator(); iter.hasNext();){
        jobject entry = iter._next();
        jobject key = Java::Private::Runtime::Map$Entry.getKey(env, entry);
        jobject value = Java::Private::Runtime::Map$Entry.getValue(env, entry);
        map.insert(qtjambi_to_qvariant(env, key), qtjambi_to_qvariant(env, value));
    }
    return map;
}

QVariantMap JMapWrapper::toStringMap(bool* ok) const {
    QVariantMap map;
    JNIEnv *env = qtjambi_current_environment();
    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
    for(JIteratorWrapper iter = entrySet().iterator(); iter.hasNext();){
        jobject entry = iter._next();
        jobject key = Java::Private::Runtime::Map$Entry.getKey(env, entry);
        if(ok && key && !Java::Runtime::String.isInstanceOf(env, key)){
            ok[0] = false;
            map.clear();
            return map;
        }
        jobject value = Java::Private::Runtime::Map$Entry.getValue(env, entry);
        map.insert(qtjambi_to_qvariant(env, key).toString(), qtjambi_to_qvariant(env, value));
    }
    if(ok) ok[0] = true;
    return map;
}

QVariantHash JMapWrapper::toStringHash(bool* ok) const {
    QVariantHash map;
    JNIEnv *env = qtjambi_current_environment();
    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
    for(JIteratorWrapper iter = entrySet().iterator(); iter.hasNext();){
        jobject entry = iter._next();
        jobject key = Java::Private::Runtime::Map$Entry.getKey(env, entry);
        if(ok && key && !Java::Runtime::String.isInstanceOf(env, key)){
            ok[0] = false;
            map.clear();
            return map;
        }
        jobject value = Java::Private::Runtime::Map$Entry.getValue(env, entry);
        map.insert(qtjambi_to_qvariant(env, key).toString(), qtjambi_to_qvariant(env, value));
    }
    if(ok) ok[0] = true;
    return map;
}

uint qHash(const JIteratorWrapper &value)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        if(value.object()){
            return uint(Java::Private::Runtime::Object.hashCode(env, value.object()));
        }
    }
    return 0;
}

uint qHash(const JMapWrapper &value)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        if(value.object()){
            return uint(Java::Private::Runtime::Object.hashCode(env, value.object()));
        }
    }
    return 0;
}

uint qHash(const JCollectionWrapper &value)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        if(value.object()){
            return uint(Java::Private::Runtime::Object.hashCode(env, value.object()));
        }
    }
    return 0;
}

uint qHash(const JEnumWrapper &value)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        if(value.object()){
            return uint(Java::Private::Runtime::Object.hashCode(env, value.object()));
        }
    }
    return 0;
}

uint qHash(const JObjectWrapper &value)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        if(value.object()){
            return uint(Java::Private::Runtime::Object.hashCode(env, value.object()));
        }
    }
    return 0;
}

JavaException::JavaException()
    : QException(), JObjectWrapper(){}

JavaException::JavaException(JNIEnv *env, jthrowable obj)
    : QException(), JObjectWrapper(env, obj){
    update(env);
}

void JavaException::update(JNIEnv *env)
{
    try{
        if(env){
            if(object()){
                jstring msg = Java::Private::Runtime::Throwable.tryGetMessage(env, object());
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                if(msg){
                    int length = env->GetStringUTFLength(msg);
                    if(env->ExceptionCheck()){
                        env->ExceptionDescribe();
                        env->ExceptionClear();
                    }
                    if(length>0){
                        m_what.resize(length);
                        env->GetStringUTFRegion(msg, 0, length, m_what.data());
                        if(env->ExceptionCheck()){
                            env->ExceptionDescribe();
                            env->ExceptionClear();
                        }
                    }
                }
                //report(env);
            }else{
                m_what.clear();
            }
        }
    }catch(const JavaException&){}
}

JavaException::JavaException(const JavaException& copy)
    : QException(copy), JObjectWrapper(copy), m_what(copy.m_what) {}

JavaException::~JavaException(){}

char const* JavaException::what() const Q_DECL_NOEXCEPT
{
    return m_what.isEmpty() ? "Unknown exception" : m_what.data();
}

void JavaException::raiseInJava(JNIEnv* env) const {
    jthrowable t = object();
    if(t)
        env->Throw(t);
}

void JavaException::raise() const{
    if(object()){
        throw *this;
    }
}

JavaException* JavaException::clone() const{
    return new JavaException(*this);
}

jthrowable JavaException::object() const{
    return reinterpret_cast<jthrowable>(JObjectWrapper::object());
}

void JavaException::addSuppressed(JNIEnv* env, const JavaException& exn) const{
    if(exn){
        try{
            if(!object()){
                JavaException& _this = *const_cast<JavaException*>(this);
                _this = exn;
            }else{
                Java::Private::Runtime::Throwable.addSuppressed(env, object(), exn.object());
            }
        }catch(const JavaException& exn){ exn.report(env); }
    }
}

void JavaException::report(JNIEnv* env) const{
    jthrowable t = object();
    if(t){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        try{
            Java::Private::QtJambi::QtJambiInternal.reportException(env, env->NewStringUTF("QtJambi: Exception pending in native code"), t);
        }catch(const JavaException& exn){
            printf("QtJambi: exception pending at QtJambiInternal.reportException(...): %s\n", exn.what());
        }
    }
}

JavaException& JavaException::operator =(const JavaException& other){
    QException::operator=(static_cast<const QException&>(other));
    JObjectWrapper::operator=(static_cast<const JObjectWrapper&>(other));
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        update(env);
    }
    return *this;
}

JavaException::operator bool() const{
    return object();
}

#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) env, methodName, fileName, lineNumber
#else
#define QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env)
#endif

#ifdef QTJAMBI_STACKTRACE
void JavaException::check(JNIEnv* env){
    if(env->ExceptionCheck()){
        jthrowable exn = env->ExceptionOccurred();
        env->ExceptionClear();
        JavaException jexn(env, exn);
        jexn.raise();
    }
}
#endif

void JavaException::check(JNIEnv* env QTJAMBI_STACKTRACEINFO_DECL ){
    if(env->ExceptionCheck()){
        jthrowable exn = env->ExceptionOccurred();
        env->ExceptionClear();
        JavaException jexn(env, exn);
        jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
    }
}

#ifdef QTJAMBI_STACKTRACE
void JavaException::raise(JNIEnv* env, const char *methodName, const char *fileName, int lineNumber) const {
    jthrowable t = object();
    if(t){
        jstring jmethodName = methodName ? env->NewStringUTF(methodName) : nullptr;
        jstring jfileName = fileName ? env->NewStringUTF(fileName) : nullptr;
        try{
            Java::Private::QtJambi::QtJambiInternal.extendStackTrace(env, t, jmethodName, jfileName, lineNumber);
        }catch(const JavaException& exn){ exn.report(env); }
        raise();
    }
}
#endif

void JavaException::raiseIllegalAccessException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::IllegalAccessException.newInstance(env, jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseNullPointerException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::NullPointerException.newInstance(env, jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseIndexOutOfBoundsException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::IndexOutOfBoundsException.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseIllegalArgumentException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::IllegalArgumentException.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseIllegalStateException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::IllegalStateException.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseQNoNativeResourcesException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::QtJambi::QNoNativeResourcesException.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseNumberFormatException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::NumberFormatException.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseQNonVirtualOverridingException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::QtJambi::QNonVirtualOverridingException.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseQNoImplementationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::QtJambi::QNoImplementationException.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseQThreadAffinityException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::QtJambi::QThreadAffinityException.newInstance(env,jmessage, t1,
#ifdef QT_QTJAMBI_PORT
                                                          qtjambi_from_thread(env, t2),
                                                          qtjambi_from_thread(env, t3)
#else
                                                          qtjambi_cast<jobject>(env, t2),
                                                          qtjambi_cast<jobject>(env, t3)
#endif
                                        );
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseIllegalAccessError(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::IllegalAccessError.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseError(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::Error.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

void JavaException::raiseRuntimeException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    qtjambi_throw_java_exception(env)
    jthrowable t = Java::Private::Runtime::RuntimeException.newInstance(env,jmessage);
    JavaException jexn(env, t);
    jexn.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
}

const QObject* qtjambi_main_thread_owner(void *)
{
    if(QCoreApplicationPrivate::is_app_closing){
        return nullptr;
    }
    return QCoreApplication::instance();
}

void qtjambi_raise_exception_in_java(JNIEnv *env, const JavaException& exn){
    exn.raiseInJava(env);
}

void qtjambi_report_exception(JNIEnv *env, const JavaException& exn, QTextStream& stream){
    exn.report(env);
    stream << exn.what();
}

// Boxing functions
jobject qtjambi_from_int(JNIEnv *env, jint int_value) {
    return Java::Runtime::Integer.valueOf(env, int_value);
}


jint qtjambi_to_int(JNIEnv *env, jobject int_object)
{
    return Java::Runtime::Number.intValue(env, int_object);
}


jobject qtjambi_from_double(JNIEnv *env, jdouble double_value) {
    return Java::Runtime::Double.valueOf(env, double_value);
}


jdouble qtjambi_to_double(JNIEnv *env, jobject double_object) {
    return Java::Runtime::Number.doubleValue(env, double_object);
}

jobject qtjambi_from_boolean(JNIEnv *env, jboolean bool_value)
{
    return Java::Runtime::Boolean.valueOf(env, bool_value);
}

jobject qtjambi_from_jvalue(JNIEnv *env, const jvalue& value, jValueType valueType)
{
    jobject inobject = nullptr;
    switch(valueType){
    case jValueType::l:
        inobject = value.l;
    break;
    case jValueType::z:
        inobject = qtjambi_from_boolean(env, value.z);
    break;
    case jValueType::b:
        inobject = qtjambi_from_byte(env, value.b);
    break;
    case jValueType::s:
        inobject = qtjambi_from_short(env, value.s);
    break;
    case jValueType::i:
        inobject = qtjambi_from_int(env, value.i);
    break;
    case jValueType::j:
        inobject = qtjambi_from_long(env, value.j);
    break;
    case jValueType::f:
        inobject = qtjambi_from_float(env, value.f);
    break;
    case jValueType::d:
        inobject = qtjambi_from_double(env, value.d);
    break;
    case jValueType::c:
        inobject = qtjambi_from_char(env, value.c);
    break;
    }
    return inobject;
}

bool qtjambi_to_boolean(JNIEnv *env, jobject bool_object)
{
    return Java::Runtime::Boolean.booleanValue(env, bool_object);
}

jlong qtjambi_to_long(JNIEnv *env, jobject long_object)
{
    return Java::Runtime::Number.longValue(env, long_object);
}

jobject qtjambi_from_long(JNIEnv *env, jlong long_value)
{
    return Java::Runtime::Long.valueOf(env,long_value);
}

jobject qtjambi_from_short(JNIEnv *env, jshort short_value)
{
    return Java::Runtime::Short.valueOf(env, short_value);
}

jobject qtjambi_from_float(JNIEnv *env, jfloat float_value)
{
    return Java::Runtime::Float.valueOf(env, double(float_value));
}

jobject qtjambi_from_byte(JNIEnv *env, jbyte byte_value)
{
    return Java::Runtime::Byte.valueOf(env, byte_value);
}


jchar qtjambi_to_char(JNIEnv *env, jobject char_object)
{
    return Java::Runtime::Character.charValue(env, char_object);
}

jobject qtjambi_from_char(JNIEnv *env, jchar char_value)
{
    return Java::Runtime::Character.valueOf(env, char_value);
}

jfloat qtjambi_to_float(JNIEnv *env, jobject float_object)
{
    return Java::Runtime::Number.floatValue(env, float_object);
}

jshort qtjambi_to_short(JNIEnv *env, jobject short_object)
{
    return Java::Runtime::Number.shortValue(env, short_object);
}

jchar qtjambi_to_jchar(JNIEnv *env, jobject char_object)
{
    return Java::Runtime::Character.charValue(env, char_object);
}

jbyte qtjambi_to_byte(JNIEnv *env, jobject byte_object)
{
    return Java::Runtime::Number.byteValue(env, byte_object);
}

// Container helpers...
jobject qtjambi_pair_new(JNIEnv *env, jobject first, jobject second) {
    return Java::Private::QtCore::QPair.newInstance(env, first, second);
}


jobject qtjambi_pair_get(JNIEnv *env, jobject pair, int pos) {
    Q_ASSERT(pos == 0 || pos == 1);
    if (pos == 0)
        return Java::Private::QtCore::QPair.first(env,pair);
    else
        return Java::Private::QtCore::QPair.second(env,pair);
}

jobject qtjambi_hashset_new(JNIEnv *env) {
    return Java::Private::Runtime::HashSet.newInstance(env);
}

jobject qtjambi_hashmap_new(JNIEnv *env, int size) {
    return Java::Private::Runtime::HashMap.newInstance(env, size);
}

jobject qtjambi_treemap_new(JNIEnv *env) {
    return Java::Private::Runtime::TreeMap.newInstance(env);
}

jobject qtjambi_treeset_new(JNIEnv *env, LessThanFunction lessThanFct) {
    jobject cmp = Java::Private::QtJambi::QtJambiInternal.createComparator(env, jlong(lessThanFct));
    return Java::Private::Runtime::TreeSet.newInstance2(env, cmp);
}

jobject qtjambi_treemap_new(JNIEnv *env, LessThanFunction lessThanFct) {
    jobject cmp = Java::Private::QtJambi::QtJambiInternal.createComparator(env, jlong(lessThanFct));
    return Java::Private::Runtime::TreeMap.newInstance2(env, cmp);
}

void qtjambi_map_put(JNIEnv *env, jobject map, jobject key, jobject val)
{
    (void)Java::Private::Runtime::Map.put(env, map, key, val);
}

void qtjambi_multimap_put(JNIEnv *env, jobject map, jobject key, jobject val)
{
    Java::Private::QtJambi::QtJambiInternal.putMultiMap(env, map, key, val);
}

int qtjambi_map_size(JNIEnv *env, jobject map)
{
    return Java::Private::Runtime::Map.size(env,map);
}

void qtjambi_map_clear(JNIEnv *env, jobject map)
{
    Java::Private::Runtime::Map.clear(env, map);
}

jobject qtjambi_map_entryset_iterator(JNIEnv *env, jobject map)
{
    jobject set = Java::Private::Runtime::Map.entrySet(env,map);
    return Java::Private::Runtime::Collection.iterator(env,set);
}

jobjectArray qtjambi_map_entryset_array(JNIEnv *env, jobject map)
{
    jobject set = Java::Private::Runtime::Map.entrySet(env,map);
    return Java::Private::Runtime::Collection.toArray(env, set);
}

jobject qtjambi_map$entry_key(JNIEnv *env, jobject entry)
{
    return Java::Private::Runtime::Map$Entry.getKey(env, entry);
}

jobject qtjambi_map$entry_value(JNIEnv *env, jobject entry)
{
    return Java::Private::Runtime::Map$Entry.getValue(env,entry);
}

QPair<jobject, jobject> qtjambi_entryset_array_get(JNIEnv *env, jobjectArray array, jint idx)
{
    jobject entry = env->GetObjectArrayElement(array, idx);
    jobject key = Java::Private::Runtime::Map$Entry.getKey(env,entry);
    jobject value = Java::Private::Runtime::Map$Entry.getValue(env,entry);
    return QPair<jobject, jobject>(key, value);
}

jobject qtjambi_arraylist_new(JNIEnv *env, jint size) {
    return Java::Private::Runtime::ArrayList.newInstance(env, size);
}

jobject qtjambi_linkedlist_new(JNIEnv *env) {
    return Java::Private::Runtime::LinkedList.newInstance(env);
}

jobject qtjambi_deque_new(JNIEnv *env) {
    return Java::Private::Runtime::ArrayDeque.newInstance(env);
}

void qtjambi_collection_add(JNIEnv *env, jobject list, jobject obj) {
    Java::Private::Runtime::Collection.add(env, list, obj);
}

void qtjambi_collection_clear(JNIEnv *env, jobject collection)
{
    Java::Private::Runtime::Collection.clear(env,collection);
}

jobject qtjambi_list_get(JNIEnv *env, jobject list, jint index)
{
    return Java::Private::Runtime::List.get(env, list, index);
}

jobjectArray qtjambi_collection_toArray(JNIEnv *env, jobject col)
{
    return Java::Private::Runtime::Collection.toArray(env, col);
}

int qtjambi_collection_size(JNIEnv *env, jobject col)
{
    return int(Java::Private::Runtime::Collection.size(env, col));
}

jobject qtjambi_collection_iterator(JNIEnv *env, jobject col)
{
    return Java::Private::Runtime::Collection.iterator(env, col);
}

jobject qtjambi_iterator_next(JNIEnv *env, jobject col)
{
    return Java::Private::Runtime::Iterator.next(env, col);
}

bool qtjambi_iterator_has_next(JNIEnv *env, jobject col)
{
    return Java::Private::Runtime::Iterator.hasNext(env, col);
}

jobjectArray qtjambi_to_jobjectArray(JNIEnv *env, const std::type_info& typeInfo, const void* iterable, jsize length, std::function<jobject(JNIEnv *,const void*,jsize)> convertFunction) {
    jclass elementClass = resolveClass(env, getJavaInterfaceName(typeInfo));
    qtjambi_throw_java_exception(env)
    if(!elementClass){
        elementClass = Java::Private::Runtime::Object.getClass(env);
    }

    Q_ASSERT(elementClass);
    jobjectArray out = nullptr;
    if(length>=0){
        out = env->NewObjectArray(length, elementClass, nullptr);
        qtjambi_throw_java_exception(env)
        for (jsize i = 0; i < length; ++i) {
            env->SetObjectArrayElement(out, i, convertFunction(env, iterable, i));
            qtjambi_throw_java_exception(env)
        }
    }
    return out;
}

#define QTJAMBI_TO_COLLECTION(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject collection, jclass expectedElementType) {\
    if(collection && Java::Private::QtJambi::QtJambi##NAME##Object.isInstanceOf(env,collection)){\
        jclass elementType = Java::Private::QtJambi::QtJambiCollectionObject.elementType(env, collection);\
        return env->IsSameObject(elementType, expectedElementType);\
    }\
    return false;\
}\
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject collection, const std::type_info& typeId) {\
    jclass expectedElementType = resolveClass(env, getJavaName(typeId));\
    qtjambi_throw_java_exception(env)\
    return qtjambi_is_Q##NAME(env, collection, expectedElementType);\
}

#define QTJAMBI_TO_MAP(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject mapObject, jclass expectedKeyType, jclass expectedValueType) {\
    if(mapObject && Java::Private::QtJambi::QtJambi##NAME##Object.isInstanceOf(env, mapObject)){\
        jclass keyType = Java::Private::QtJambi::QtJambiAbstractMapObject.keyType(env, mapObject);\
        jclass valueType = Java::Private::QtJambi::QtJambiAbstractMapObject.valueType(env, mapObject);\
        return env->IsSameObject(keyType, expectedKeyType) && env->IsSameObject(valueType, expectedValueType);\
    }\
    return qtjambi_is_QMulti##NAME(env, mapObject, expectedKeyType, expectedValueType);\
}\
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject mapObject, const std::type_info& keyTypeId, const std::type_info& valueTypeId) {\
    jclass expectedKeyType = resolveClass(env, getJavaName(keyTypeId));\
    qtjambi_throw_java_exception(env)\
    jclass expectedValueType = resolveClass(env, getJavaName(valueTypeId));\
    qtjambi_throw_java_exception(env)\
    return qtjambi_is_Q##NAME(env, mapObject, expectedKeyType, expectedValueType);\
}

#define QTJAMBI_TO_MULTIMAP(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject mapObject, jclass expectedKeyType, jclass expectedValueType) {\
    if(mapObject && Java::Private::QtJambi::QtJambi##NAME##Object.isInstanceOf(env, mapObject)){\
        jclass keyType = Java::Private::QtJambi::QtJambiAbstractMultiMapObject.keyType(env, mapObject);\
        jclass valueType = Java::Private::QtJambi::QtJambiAbstractMultiMapObject.valueType(env, mapObject);\
        return env->IsSameObject(keyType, expectedKeyType) && env->IsSameObject(valueType, expectedValueType);\
    }\
    return false;\
}\
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject mapObject, const std::type_info& keyTypeId, const std::type_info& valueTypeId) {\
    jclass expectedKeyType = resolveClass(env, getJavaName(keyTypeId));\
    qtjambi_throw_java_exception(env)\
    jclass expectedValueType = resolveClass(env, getJavaName(valueTypeId));\
    qtjambi_throw_java_exception(env)\
    return qtjambi_is_Q##NAME(env, mapObject, expectedKeyType, expectedValueType);\
}

QTJAMBI_TO_COLLECTION(Stack)
QTJAMBI_TO_COLLECTION(Vector)
QTJAMBI_TO_COLLECTION(LinkedList)
QTJAMBI_TO_COLLECTION(List)
QTJAMBI_TO_COLLECTION(Set)
QTJAMBI_TO_COLLECTION(Queue)
QTJAMBI_TO_MULTIMAP(MultiMap)
QTJAMBI_TO_MULTIMAP(MultiHash)
QTJAMBI_TO_MAP(Map)
QTJAMBI_TO_MAP(Hash)

#undef QTJAMBI_TO_MAP

#undef QTJAMBI_TO_COLLECTION

void qtjambi_check_resource(JNIEnv *env, const void* ptr, const std::type_info& typeId){
    if(!ptr){
        QLatin1String java_type(getJavaName(typeId));
        QString msg = QLatin1String("Function call on incomplete object of type: %1");
        if(!java_type.isEmpty()){
            msg = msg.arg(QString(QLatin1String(java_type)).replace(QLatin1Char('/'), QLatin1Char('.')).replace(QLatin1Char('$'), QLatin1Char('.')));
        }else{
            QLatin1String qt_type(getQtName(typeId));
            if(!qt_type.isEmpty()){
                msg = msg.arg(qt_type);
            }else{
                msg = msg.arg(QLatin1String(typeId.name()));
            }
        }
        jstring jmessage = qtjambi_from_qstring(env, msg);
        jthrowable exn = Java::Private::QtJambi::QNoNativeResourcesException.newInstance(env, jmessage);
        JavaException(env, exn).raise();
    }
}

JniLocalFrame::JniLocalFrame(JNIEnv *env, int capacity) : m_env(env){
    m_env->PushLocalFrame(capacity);
}

JniLocalFrame::~JniLocalFrame(){
    m_env->PopLocalFrame(nullptr);
}

DoFinally::DoFinally(std::function<void()> _action) : action(_action)
{

}

DoFinally::~DoFinally(){
    try{
        if(action)
            action();
    }catch(const JavaException& exn){
        if(JNIEnv* env = qtjambi_current_environment(false))
            exn.report(env);
        else qWarning("%s", exn.what());
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

InvalidateAfterUse::InvalidateAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership)
    : m_env(env),
      m_object(object),
      m_checkJavaOwnership(checkJavaOwnership)
{}

InvalidateAfterUse::~InvalidateAfterUse()
{
    try{
        qtjambi_invalidate_object(m_env, m_object, m_checkJavaOwnership);
    }catch(const JavaException& exn){
        if(JNIEnv* env = qtjambi_current_environment(false))
            exn.report(env);
        else qWarning("%s", exn.what());
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

InvalidateContainerAfterUse::InvalidateContainerAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership)
    : m_env(env),
      m_object(object),
      m_checkJavaOwnership(checkJavaOwnership)
{}

InvalidateContainerAfterUse::~InvalidateContainerAfterUse()
{
    try{
        qtjambi_invalidate_collection(m_env, m_object, m_checkJavaOwnership);
    }catch(const JavaException& exn){
        if(JNIEnv* env = qtjambi_current_environment(false))
            exn.report(env);
        else qWarning("%s", exn.what());
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

InvalidateArrayAfterUse::InvalidateArrayAfterUse(JNIEnv *env, jobjectArray object, bool checkJavaOwnership)
    : m_env(env),
      m_object(object),
      m_checkJavaOwnership(checkJavaOwnership)
{}

InvalidateArrayAfterUse::~InvalidateArrayAfterUse()
{
    try{
        qtjambi_invalidate_array(m_env, m_object, m_checkJavaOwnership);
    }catch(const JavaException& exn){
        if(JNIEnv* env = qtjambi_current_environment(false))
            exn.report(env);
        else qWarning("%s", exn.what());
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

AboutToPaint::AboutToPaint(JNIEnv *env, jobject object)
    : m_env(env),
      m_object(object)
{
    qtjambi_begin_paint(env, object);
}

AboutToPaint::~AboutToPaint()
{
    try{
        qtjambi_end_paint(m_env, m_object);
    }catch(const JavaException& exn){
        JNIEnv* env = qtjambi_current_environment(false);
        if(env) exn.report(env);
        else qWarning("%s", exn.what());
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

BlockExceptions::BlockExceptions(bool block)
    : m_blocked(qtjambi_block_exceptions(block))
{
}

BlockExceptions::~BlockExceptions()
{
    qtjambi_block_exceptions(m_blocked);
}

ApplicationData::ApplicationData(JNIEnv *env, jobjectArray array)
    : QObjectUserData(), m_chars(nullptr), m_buffers()
{
    m_size = int(env->GetArrayLength(array));
    m_chars = new char*[quint32(m_size)];
    for(jsize i=0; i<m_size; ++i){
        J2CStringBuffer* buffer = new J2CStringBuffer(env, jstring(env->GetObjectArrayElement(array, i)));
        m_buffers << buffer;
        m_chars[i] = *buffer;
    }
}

ApplicationData::~ApplicationData(){
    try{
        operator delete(m_chars);
        while(!m_buffers.isEmpty())
            delete m_buffers.takeFirst();
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

char** ApplicationData::chars() const{
    return m_chars;
}

int& ApplicationData::size() const{
    return m_size;
}

uint ApplicationData::id(){
    static uint id = QObject::registerUserData();
    return id;
}

FunctionalBase::FunctionalBase()
    :m_ref(0)
{}

FunctionalBase::~FunctionalBase(){}

class FunctorBasePrivate : public QSharedData{
public:
    FunctorBasePrivate(const std::type_info& typeId, FunctionalBase& functional)
        : QSharedData(),
          m_method(functional.__shell()->javaMethod(typeId, 0)),
          m_link(static_cast<const QtJambiShellImpl*>(functional.__shell())->link()),
          m_typeId(typeId)
    {
        if(0==functional.m_ref++){
            if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
                link->setCppOwnership(qtjambi_current_environment());
            }
        }
    }
    ~FunctorBasePrivate()
    {
        if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
            if(void* ptr = link->typedPointer(m_typeId)){
                FunctionalBase* functional = reinterpret_cast<FunctionalBase*>(ptr);
                if(--functional->m_ref==0){
                    if(JNIEnv* env = qtjambi_current_environment(false))
                        link->setJavaOwnership(env);
                }
            }
        }
    }

private:
    jmethodID m_method;
    QWeakPointer<QtJambiLink> m_link;
    const std::type_info& m_typeId;
    friend FunctorBase;
};

FunctorBase::FunctorBase(const FunctorBase& functor)
    : d(functor.d)
{
}

FunctorBase::FunctorBase(const std::type_info& typeId, FunctionalBase& functional)
    : d(QSharedDataPointer<FunctorBasePrivate>( new FunctorBasePrivate(typeId, functional) )) {}

FunctorBase::~FunctorBase()
{
}

jobject FunctorBase::getJavaObjectLocalRef(JNIEnv *env) const
{
    if(QSharedPointer<QtJambiLink> link = d->m_link.toStrongRef()){
        return link->getJavaObjectLocalRef(env);
    }
    return nullptr;
}

QtJambiShell* FunctorBase::shell() const
{
    if(QSharedPointer<QtJambiLink> link = d->m_link.toStrongRef()){
        void* ptr = link->pointer();
        return reinterpret_cast<FunctionalBase*>(ptr)->__shell();
    }
    return nullptr;
}

jmethodID FunctorBase::javaMethod() const
{
    return d->m_method;
}

void JObjectGlobalWrapperCleanup::cleanup(jobject object){
    if(object && !QCoreApplication::closingDown()){
        DEREF_JOBJECT;
        if(JNIEnv *env = qtjambi_current_environment(false)){
            jthrowable throwable = nullptr;
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            env->DeleteGlobalRef(object);
            if(throwable)
                env->Throw(throwable);
        }
    }
}

void JObjectWeakWrapperCleanup::cleanup(jobject object){
    if(object && !QCoreApplication::closingDown()){
        DEREF_JOBJECT;
        if(JNIEnv *env = qtjambi_current_environment(false)){
            jthrowable throwable = nullptr;
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            env->DeleteWeakGlobalRef(object);
            if(throwable)
                env->Throw(throwable);
        }
    }
}

JObjectWrapperData::JObjectWrapperData(){}
JObjectWrapperData::~JObjectWrapperData(){}

JObjectGlobalWrapperData::JObjectGlobalWrapperData(){}
JObjectWeakWrapperData::JObjectWeakWrapperData(){}

JObjectGlobalWrapperData::~JObjectGlobalWrapperData(){}
JObjectWeakWrapperData::~JObjectWeakWrapperData(){}

JObjectGlobalWrapperData::JObjectGlobalWrapperData(JNIEnv* env, jobject object)
    : pointer( env->NewGlobalRef(object) ){}
JObjectWeakWrapperData::JObjectWeakWrapperData(JNIEnv* env, jobject object)
    : pointer( env->NewWeakGlobalRef(object) ){}

jobject JObjectGlobalWrapperData::data() const {return pointer.data();}
jobject JObjectWeakWrapperData::data() const {return pointer.data();}

class QtJambiScopePrivate : public QLinkedList<std::function<void()>>
{
public:
    QtJambiScopePrivate();
    virtual ~QtJambiScopePrivate();
    virtual QtJambiLink * link() const;
};

QtJambiScopePrivate::QtJambiScopePrivate() : QLinkedList<std::function<void()>>() {}
QtJambiScopePrivate::~QtJambiScopePrivate(){}
QtJambiLink *QtJambiScopePrivate::link() const { return nullptr; }

class QtJambiScopePrivateLink : public QtJambiScopePrivate
{
public:
    QtJambiScopePrivateLink(const QSharedPointer<QtJambiLink>& _link);
    ~QtJambiScopePrivateLink();
    QtJambiLink * link() const;
private:
    QSharedPointer<QtJambiLink> m_link;
};

QtJambiScopePrivateLink::QtJambiScopePrivateLink(const QSharedPointer<QtJambiLink>& _link) : QtJambiScopePrivate(), m_link(_link) {}
QtJambiScopePrivateLink::~QtJambiScopePrivateLink(){}
QtJambiLink *QtJambiScopePrivateLink::link() const { return m_link.data(); }

QtJambiLinkScope::QtJambiLinkScope(const QSharedPointer<QtJambiLink>& _link)
 : QtJambiScope(*new QtJambiScopePrivateLink(_link)){

}

QtJambiScope::QtJambiScope() : d(nullptr) {}

QtJambiScope::QtJambiScope(QtJambiNativeID nativeId)
    : d(nullptr)
{
    if(!!nativeId)
        d = new QtJambiScopePrivateLink(QtJambiLink::fromNativeId(nativeId));
}

QtJambiScope::QtJambiScope(QtJambiShell* shell)
    : d(nullptr)
{
    if(shell){
        if(QSharedPointer<QtJambiLink> link = static_cast<QtJambiShellImpl*>(shell)->link())
            d = new QtJambiScopePrivateLink(link);
    }
}

QtJambiScope::QtJambiScope(QtJambiScopePrivate& _d)
    :d(&_d)
{
}

QtJambiScope::~QtJambiScope(){
    if(d){
        while(!d->isEmpty()){
            try {
                d->takeLast()();
            }catch(const JavaException& exn){
                if(JNIEnv* env = qtjambi_current_environment(false))
                    exn.report(env);
                else qWarning("%s", exn.what());
            } catch (const std::exception& e) {
                qWarning("%s", e.what());
            } catch (...) {
            }
        }
        delete d;
    }
}

void QtJambiScope::addFinalAction(std::function<void()> action){
    if(!d)
        d = new QtJambiScopePrivate();
    d->append(action);
}

QtJambiNativeID QtJambiScope::relatedNativeID() const{
    return d ? QtJambiNativeID(jlong(d->link())) : InvalidNativeID;
}

bool qtjambi_is_shell(QtJambiNativeID nativeId)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->isShell() : false;
}

jobject qtjambi_java_object_reference(QtJambiNativeID nativeId, JNIEnv* env)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->getJavaObjectLocalRef(env) : nullptr;
}

void *qtjambi_from_nativeId(QtJambiNativeID nativeId)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->pointer() : nullptr;
}

void *qtjambi_from_nativeId(QtJambiNativeID nativeId, const std::type_info& typeId)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->typedPointer(typeId) : nullptr;
}

void qtjambi_end_paint(JNIEnv *env, jobject device)
{
    Java::Private::QtGui::QPainter.endPaint(env, device);
}

void qtjambi_begin_paint(JNIEnv *env, jobject device)
{
    Java::Private::QtGui::QPainter.beginPaint(env, device);
}

uint qtjambi_java_object_identity(JNIEnv *env, jobject object){
    return uint(Java::Private::Runtime::System.identityHashCode(env, object));
}

uint qtjambi_java_object_hashcode(JNIEnv *env, jobject object){
    return uint(Java::Private::Runtime::Object.hashCode(env, object));
}

jobject qtjambi_from_destroyed_qobject(JNIEnv *env, QObject* object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(object)){
        return link->getJavaObjectLocalRef(env);
    }
    return nullptr;
}
