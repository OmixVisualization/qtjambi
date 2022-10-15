/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <qglobal.h>
QT_WARNING_DISABLE_DEPRECATED

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
#include <QtCore/QPluginLoader>
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/QVariant>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>
#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QCborValueRef>
#include <QtCore/QUrl>

#include "qtjambi_registry.h"
#include "qtjambi_core.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambi_registry_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambilink_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambimetaobject_p.h"
#include "qtjambivariant_p.h"
#include "qtjambi_thread.h"
#include "qtjambi_thread_p.h"
#include "qtjambi_interfaces_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_application.h"
#include "qtjambishell_p.h"
#include "qtjambi_templates.h"
#include "qtjambi_qml.h"
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_nativeinterface.h"
#if QT_VERSION < 0x050000
#include <QtGui/QStyleOption>
#endif
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QXmlStreamStringRef>
#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#endif
#include <stdio.h>
#include <string.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#if defined(QTJAMBI_DEBUG_TOOLS)
 #include "qtjambidebugevent_p.h"
#endif

#ifndef JNI_VERSION_1_8
#define JNI_VERSION_1_8 JNI_VERSION_1_6
#endif

#include "qtjambi_cast.h"

bool operator !(QtJambiNativeID nativeId) { return nativeId == InvalidNativeID; }

// #define JOBJECT_REFCOUNT

Q_GLOBAL_STATIC_WITH_ARGS(std::atomic<JavaVM *>, qtjambi_vm, (nullptr));

Q_GLOBAL_STATIC_WITH_ARGS(std::atomic<bool>, gIsLoaded, (false))

jint qtjambi_startup(JavaVM *vm);
void qtjambi_shutdown(JNIEnv * env);
JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

/*!
 * This function is called by the Virtual Machine when it loads the
 * library. We need this to get a hold of the global VM pointer
 */
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnLoad)(JavaVM *vm, void *)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi::JNI_OnLoad(JavaVM *, void *)")
    return qtjambi_startup(vm);
}

void clear_repository_at_shutdown(JNIEnv *env);
void clear_registry_at_shutdown(JNIEnv * env);

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(JNI_OnUnload)(JavaVM *, void *)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi::JNI_OnUnload(JavaVM *, void *)")
    JNIEnv *env = qtjambi_current_environment(false);
    qtjambi_shutdown(env);
    clear_repository_at_shutdown(env);
    clear_registry_at_shutdown(env);
    if(std::atomic<JavaVM *>* atm = qtjambi_vm)
        atm->store(nullptr);
}

JNIEnv *qtjambi_current_environment(bool initializeJavaThread)
{
    JNIEnv *env = nullptr;
    JavaVM *vm = nullptr;
    if(std::atomic<JavaVM *>* atm = qtjambi_vm)
        vm = atm->load();
    if (!vm) {
        return nullptr;
    }
    int result = vm->GetEnv( reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
    if (result == JNI_EDETACHED) {
        if(std::atomic<bool>* atm = gIsLoaded){
            if(!atm->load())
                return nullptr;
        }else return nullptr;
        QThread* currentThread = QThread::currentThread();
        if(!currentThread)// this is possible during thread destruction.
            return nullptr;
        jobject uncaughtExceptionHandler = nullptr;
        jobject contextClassLoader = nullptr;
        QThreadUserData* threadData;
        {
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentThread);
        }
        if(threadData){
            uncaughtExceptionHandler = threadData->getUncaughtExceptionHandler();
            contextClassLoader = threadData->getContextClassLoader();
            QByteArray name(threadData->getName());
            JavaVMAttachArgs args;
            args.version = JNI_VERSION_1_8;
            args.name = name.isEmpty() ? nullptr : name.data();
            args.group = threadData->getThreadGroup();
            JavaVMAttachArgs* _args = args.name && args.group ? &args : nullptr;
            if(threadData->isDaemon())
#ifdef Q_OS_ANDROID
                result = vm->AttachCurrentThreadAsDaemon(&env, _args);
            else
                result = vm->AttachCurrentThread(&env, _args);
#else
                result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), _args);
            else
                result = vm->AttachCurrentThread(reinterpret_cast<void **>(&env), _args);
#endif
            if (result != JNI_OK && _args) {
#ifdef Q_OS_ANDROID
              result = vm->AttachCurrentThreadAsDaemon(&env, nullptr);
#else
              result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), nullptr);
#endif
            }
        }else{
#ifdef Q_OS_ANDROID
            result = vm->AttachCurrentThreadAsDaemon(&env, nullptr);
#else
            result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), nullptr);
#endif
        }
        if (result != JNI_OK) {
            if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                if(mainThread!=currentThread)
                    qWarning("Failed attaching current thread (%i).", result);
            }else{
                qWarning("Failed attaching current thread (%i).", result);
            }
        }
        if(env && env->functions){
            if(threadData)
                threadData->clearThreadGroup(env);
            if(initializeJavaThread){
                jobject jthread = Java::Runtime::Thread::currentThread(env);
                if(uncaughtExceptionHandler){
                    Java::Runtime::Thread::setUncaughtExceptionHandler(env, jthread, uncaughtExceptionHandler);
                    threadData->clearUncaughtExceptionHandler(env);
                    uncaughtExceptionHandler = nullptr;
                }
                if(contextClassLoader){
                    Java::Runtime::Thread::setContextClassLoader(env, jthread, contextClassLoader);
                    threadData->clearContextClassLoader(env);
                    contextClassLoader = nullptr;
                }
                QSharedPointer<QtJambiLink> link;
                if(jobject qthreadObject = qtjambi_cast<jobject>(env, currentThread)){
                    link = QtJambiLink::findLinkForJavaObject(env, qthreadObject);
                    if(jobject jthrd = Java::QtCore::QThread::javaThread(env, qthreadObject)){
                        env->DeleteLocalRef(jthrd);
                    }else{
                        Java::QtCore::QThread::set_javaThread(env, qthreadObject, jthread);
                        Java::QtJambi::QtJambiInternal::createAssociation(env, jthread, qthreadObject);
                        if(jobject interruptible = Java::QtCore::QThread::interruptible(env)){
                            JObjectSynchronizer sync(env, qthreadObject);
                            Q_UNUSED(sync)
                            Java::QtJambi::QtJambiInternal::setThreadInterruptible(env, jthread, interruptible);
                            env->DeleteLocalRef(interruptible);
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
                            EventDispatcherCheck::storage.setLocalData(QSharedPointer<EventDispatcherCheck>(
                                 new EventDispatcherCheck(
                                     JObjectWrapper(env, jthread),
                                     currentThread,
                                     link,
                                     [](QPointer<QThread>& thread, JObjectWrapper&& jthreadObjectWrapper, QWeakPointer<QtJambiLink>&& wlink, QList<std::function<void()>>&& finalActions) {
                                          struct ThreadDetacher : public QtJambiObjectData {
                                              JObjectWrapper m_jthreadObjectWrapper;
                                              QPointer<QThread> m_thread;
                                              QWeakPointer<QtJambiLink> m_wlink;
                                              QList<std::function<void()>> m_finalActions;
                                              ThreadDetacher(JObjectWrapper&& jthreadObjectWrapper,
                                                             QPointer<QThread>& thread,
                                                             QWeakPointer<QtJambiLink>&& wlink,
                                                             QList<std::function<void()>>&& finalActions)
                                                  : QtJambiObjectData(),
                                                    m_jthreadObjectWrapper(std::move(jthreadObjectWrapper)),
                                                    m_thread(),
                                                    m_wlink(std::move(wlink)),
                                                    m_finalActions(std::move(finalActions))
                                              {
                                                  QTJAMBI_DEBUG_METHOD_PRINT("native", "ThreadDetacher::ThreadDetacher()")
                                                  m_thread.swap(thread);
                                              }
                                              ~ThreadDetacher() override {
                                                  QTJAMBI_DEBUG_METHOD_PRINT("native", "ThreadDetacher::~ThreadDetacher()")
                                                  JavaVM *vm = nullptr;
                                                  if(std::atomic<JavaVM *>* atm = qtjambi_vm)
                                                      vm = atm->load();
                                                  if (vm) {
                                                       QSharedPointer<QtJambiLink> link;
                                                       {
                                                           QReadLocker locker(QtJambiLinkUserData::lock());
                                                           link = m_wlink.toStrongRef();
                                                       }
                                                       if(link || m_jthreadObjectWrapper.object()){
                                                             JNIEnv* env = nullptr;
                                                             int result = vm->GetEnv( reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
                                                             if(result==JNI_OK &&  env){
                                                                 QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                                 Q_ASSERT(link->isQObject());
                                                                 if(!link->qobject() || !link->qobject()->parent())
                                                                    link->setJavaOwnership(env);
                                                                 jobject javaThread = env->NewLocalRef(m_jthreadObjectWrapper.object());
                                                                 m_jthreadObjectWrapper.clear(env);
                                                                 if(javaThread){
                                                                     try{
                                                                         Java::QtJambi::QtJambiInternal::setThreadInterruptible(env, javaThread, nullptr);
                                                                     } catch (const JavaException& exn) {
                                                                         exn.report(env);
                                                                     }
                                                                     try {
                                                                         Java::QtJambi::QtJambiInternal::deleteAssociation(env, javaThread);
                                                                     } catch (const JavaException& exn) {
                                                                         exn.report(env);
                                                                     }
                                                                 }
                                                             }
                                                       }
                                                       while(!m_finalActions.isEmpty()){
                                                           m_finalActions.takeFirst()();
                                                       }
                                                       {
                                                           QWriteLocker locker(QtJambiLinkUserData::lock());
                                                           if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread.data())){
                                                               if(data->purgeOnExit()){
                                                                   QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_thread.data(), nullptr);
                                                                   locker.unlock();
                                                                   delete data;
                                                                   locker.relock();
                                                               }
                                                           }
                                                       }
                                                       vm->DetachCurrentThread();
                                                  }else{
                                                      while(!m_finalActions.isEmpty()){
                                                          m_finalActions.takeFirst()();
                                                      }

                                                      {
                                                           QWriteLocker locker(QtJambiLinkUserData::lock());
                                                           if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread.data())){
                                                               if(data->purgeOnExit()){
                                                                   QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_thread.data(), nullptr);
                                                                   locker.unlock();
                                                                   delete data;
                                                                   locker.relock();
                                                               }
                                                           }
                                                      }
                                                  }
                                              }
                                              QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)
                                          };
                                          QAbstractEventDispatcher * eventDispatcher = nullptr;
                                          if(thread && !(eventDispatcher = thread->eventDispatcher())){
                                               if (QThread::currentThread()==thread && (QCoreApplication::instance() || !QCoreApplicationPrivate::threadRequiresCoreApplication())) {
                                                   QEventLoop loop;
                                                   eventDispatcher = thread->eventDispatcher();
                                               }
                                          }
                                          if(eventDispatcher){
                                                QWriteLocker locker(QtJambiLinkUserData::lock());
                                                QTJAMBI_SET_OBJECTUSERDATA_ID(QTJAMBI_OBJECTUSERDATA_TYPE_ID(ThreadDetacher), eventDispatcher, new ThreadDetacher(std::move(jthreadObjectWrapper), thread, std::move(wlink), std::move(finalActions)));
                                          }else{
                                                ThreadDetacher releaser(std::move(jthreadObjectWrapper), thread, std::move(wlink), std::move(finalActions));
                                                Q_UNUSED(releaser)
                                          }
                                      })
                                ));
                        }
                    }
                    if(env->GetObjectRefType(qthreadObject)==jobjectRefType::JNILocalRefType){
                        env->DeleteLocalRef(qthreadObject);
                        qthreadObject = nullptr;
                    }
                }
                env->DeleteLocalRef(jthread);
                jthread = nullptr;
            }
        }
        {
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentThread);
        }
        if(!threadData){
            threadData = new QThreadUserData(currentThread);
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, currentThread, threadData);
        }
    } else {
        Q_ASSERT(result == JNI_OK);
    }
    if(env && !env->functions)
        env = nullptr;
    return env;
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
    jclass cls = Java::QtCore::QObject$QDeclarativeConstructor::cls(env, placement_object);
    void* placement = reinterpret_cast<void*>(Java::QtCore::QObject$QDeclarativeConstructor::placement(env, placement_object));
    if(!env->IsAssignableFrom(cls, callingClass) || !placement){
        JavaException::raiseIllegalAccessException(env, "Illegal use of QDeclarativeConstructor parameter." QTJAMBI_STACKTRACEINFO );
    }
    Java::QtCore::QObject$QDeclarativeConstructor::set_placement(env, placement_object, jlong(0));
    return placement;
}

void registerLambdaClass(JNIEnv *env, jclass lambdaClass, const char *className);
jclass resolveLambdaInterface(JNIEnv* env, jclass lambdaClass);

QString qtjambi_class_name(JNIEnv *env, jclass java_class)
{
    Q_ASSERT(java_class);
    //Q_ASSERT(Java::Runtime::Class::isInstanceOf(env, java_class));  // check the java object is right type
    jstring name = Java::Runtime::Class::getName(env,java_class);
    QString fullJavaName;
    qtjambi_to_qstring(fullJavaName, env, name);
    if(Java::Runtime::Class::isSynthetic(env,java_class)){
        jobjectArray interfaces = Java::Runtime::Class::getInterfaces(env, java_class);
        QString strClassName;
        QString strPackage;
        QtJambiTypeManager::splitClassName(strClassName, strPackage, fullJavaName, QLatin1Char('.'));
        if(interfaces && env->GetArrayLength(interfaces)>0){
            registerLambdaClass(env, java_class, qPrintable(fullJavaName.replace(".", "/")));
            java_class = jclass( env->GetObjectArrayElement(interfaces, 0) );
            fullJavaName = qtjambi_class_name(env, java_class);
            QtJambiTypeManager::splitClassName(strClassName, strPackage, fullJavaName, QLatin1Char('.'));
        }
    }
    return fullJavaName;
}

QString qtjambi_object_class_name(JNIEnv *env, jobject java_object)
{
    Q_ASSERT(java_object);
    QString className = qtjambi_class_name(env, env->GetObjectClass(java_object));
    return className;
}

jobject qtjambi_from_qvariant(JNIEnv *env, const QVariant &qt_variant)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMetaType metaType(qt_variant.type() == QVariant::UserType ? qt_variant.userType() : int(qt_variant.type()));
#else
    QMetaType metaType = qt_variant.metaType();
#endif
    switch (metaType.id()) {
    case QMetaType::UnknownType:
        return nullptr;
    case QMetaType::Nullptr:
    case QMetaType::Void:
        return qtjambi_to_variantobject(env, qt_variant);
    case QMetaType::SChar:
    case QMetaType::Char:
    case QMetaType::UChar:
        return Java::Runtime::Byte::valueOf(env, qt_variant.value<qint8>());
    case QMetaType::UShort:
    case QMetaType::Short:
        return Java::Runtime::Short::valueOf(env, qt_variant.value<qint16>());
    case QMetaType::Int:
    case QMetaType::UInt:
        return Java::Runtime::Integer::valueOf(env, qt_variant.toInt());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::Char16:
        return Java::Runtime::Character::valueOf(env, jchar(qt_variant.value<char16_t>()));
    case QMetaType::Char32:
        return Java::Runtime::Integer::valueOf(env, jchar(qt_variant.value<char32_t>()));
#endif
    case QMetaType::QChar:
        return Java::Runtime::Character::valueOf(env, jchar(qt_variant.toChar().unicode()));
    case QMetaType::Float:
        return Java::Runtime::Float::valueOf(env, qt_variant.toFloat());
    case QMetaType::Double:
        return Java::Runtime::Double::valueOf(env, qt_variant.toDouble());
    case QMetaType::QString:
        return qtjambi_from_qstring(env, qt_variant.toString());
    case QMetaType::Long:
    case QMetaType::ULong:
        if(metaType.sizeOf()==4)
            return Java::Runtime::Integer::valueOf(env, qt_variant.toInt());
        else
            return Java::Runtime::Long::valueOf(env, qt_variant.toLongLong());
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        return Java::Runtime::Long::valueOf(env, qt_variant.toLongLong());
    case QMetaType::Bool:
        return Java::Runtime::Boolean::valueOf(env, qt_variant.toBool());
    case QMetaType::QStringList:
        return qtjambi_cast<jobject>(env, qt_variant.value<QStringList>());
    case QMetaType::QByteArrayList:
        return qtjambi_cast<jobject>(env, qt_variant.value<QByteArrayList>());
    case QMetaType::QVariantMap:
        return qtjambi_cast<jobject>(env, qt_variant.value<QVariantMap>());
    case QMetaType::QVariantHash:
        return qtjambi_cast<jobject>(env, qt_variant.value<QVariantHash>());
    case QMetaType::QVariantList:
        return qtjambi_cast<jobject>(env, qt_variant.value<QVariantList>());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case QMetaType::QVariantPair:
        return qtjambi_cast<jobject>(env, qt_variant.value<QVariantPair>());
#endif
    case QMetaType::QObjectStar:
        return qtjambi_cast<jobject>(env, qt_variant.value<QObject*>());
    default:
        break;
    }

    // generic java object
    if (metaType == QMetaType::fromType<qint8>()
            || metaType == QMetaType::fromType<quint8>()) {
        return Java::Runtime::Byte::valueOf(env, jbyte(qt_variant.value<qint8>()));
    }else if (metaType == QMetaType::fromType<qint16>()
            || metaType == QMetaType::fromType<quint16>()) {
        return Java::Runtime::Short::valueOf(env, jshort(qt_variant.value<qint16>()));
    }else if (metaType == QMetaType::fromType<float>()) {
        return Java::Runtime::Float::valueOf(env, qt_variant.value<float>());
    }else if (metaType == QMetaType::fromType<JObjectWrapper>()) {
		JObjectWrapper wrapper = qt_variant.value<JObjectWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JIntArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JIntArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JByteArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JByteArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JShortArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JShortArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JLongArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JLongArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JBooleanArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JBooleanArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JCharArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JCharArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JDoubleArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JDoubleArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JFloatArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JFloatArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    }else if (metaType == QMetaType::fromType<JObjectArrayWrapper>()) {
        JObjectWrapper wrapper = qt_variant.value<JObjectArrayWrapper>();
        return env->NewLocalRef(wrapper.object());
    } else if (metaType == QMetaType::fromType<JEnumWrapper>()) {
        JEnumWrapper wrapper = qt_variant.value<JEnumWrapper>();
        return env->NewLocalRef(wrapper.object());
    } else if (metaType == QMetaType::fromType<JCollectionWrapper>()
               || metaType == QMetaType::fromType<JMapWrapper>()
               || metaType == QMetaType::fromType<JIteratorWrapper>()) {
        JCollectionWrapper wrapper = qt_variant.value<JCollectionWrapper>();
        return env->NewLocalRef(wrapper.object());
    } else if (metaType == QMetaType::fromType<JMapWrapper>()) {
         JMapWrapper wrapper = qt_variant.value<JMapWrapper>();
         return env->NewLocalRef(wrapper.object());
    } else if (metaType == QMetaType::fromType<JIteratorWrapper>()) {
         JIteratorWrapper wrapper = qt_variant.value<JIteratorWrapper>();
         return env->NewLocalRef(wrapper.object());
     } else {
        QString qtType = QLatin1String(metaType.name());
        QString javaType = QtJambiTypeManager::getExternalTypeName(env, qtType, metaType);
        // Find usage pattern
        InternalToExternalConverter converter = QtJambiTypeManager::getInternalToExternalConverter(
                                                env,
                                                qtType,
                                                metaType,
                                                resolveClass(env, qPrintable(javaType)),
                                                true);
        jvalue val;
        val.l = nullptr;
        if(!converter || !converter(env, nullptr, qt_variant.constData(), &val, true)){
            qWarning("Unable to convert qVariant to jobject");
        }
        return val.l;
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define META_TYPE(t) t
#define FROM_META_TYPE(t) t.id()
#define META_TYPE_FROM(t) qMetaTypeId<t>()
#else
#define META_TYPE(t) QMetaType(t)
#define FROM_META_TYPE(t) t
#define META_TYPE_FROM(t) QMetaType::fromType<t>()

void qtjambi_to_QModelRoleData(JNIEnv *env, QtJambiScope& scope, jobject java_object, void * &data, qsizetype &length){
    length = qtjambi_map_size(env, java_object);
    if(length>0){
        struct ModelRoleData
        {
            int m_role = 0;
            QVariant m_data;
        };
        ModelRoleData* _data = new ModelRoleData[size_t(length)];
        scope.addArrayDeletion(_data);
        data = _data;
        jobject iter = qtjambi_map_entryset_iterator(env, java_object);
        size_t i=0;
        while(qtjambi_iterator_has_next(env, iter)){
            jobject entry = qtjambi_iterator_next(env, iter);
            _data[i].m_role = qtjambi_to_int(env, qtjambi_map$entry_key(env, entry));
            _data[i].m_data = qtjambi_to_qvariant(env, qtjambi_map$entry_value(env, entry));
            ++i;
        }
    }
}

#endif

#define EXCLUDE_GT_END(strg) strg //.endsWith(">") ? strg+" " : strg

QString qtjambi_to_qstring(JNIEnv *env, jobject object);

int qtjambiMetaTypeId_for_object(JNIEnv *env, jobject java_object){
    if (!java_object)
        return QMetaType::Nullptr;
    jclass object_class = env->GetObjectClass(java_object);
    if (Java::QtCore::QMetaType$GenericTypeInterface::isSameClass(env, object_class)) {
        return qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object)).id();
    } else if (Java::Runtime::String::isSameClass(env, object_class)) {
        return QMetaType::QString;
    } else if (Java::Runtime::Integer::isSameClass(env, object_class)) {
        return QMetaType::Int;
    } else if (Java::Runtime::Character::isSameClass(env, object_class)) {
        return QMetaType::QChar;
    } else if (Java::Runtime::Double::isSameClass(env, object_class)) {
        return QMetaType::Double;
    } else if (Java::Runtime::Float::isSameClass(env, object_class)) {
        return QMetaType::Float;
    } else if (Java::Runtime::Short::isSameClass(env, object_class)) {
        return QMetaType::Short;
    } else if (Java::Runtime::Byte::isSameClass(env, object_class)) {
        return QMetaType::SChar;
    } else if (Java::Runtime::Long::isSameClass(env, object_class)) {
        return QMetaType::LongLong;
    } else if (Java::Runtime::Boolean::isSameClass(env, object_class)) {
        return QMetaType::Bool;
    }else if (Java::QtCore::QList::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QList::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->elementMetaType().name()).contains("*"))
                    return qMetaTypeId<JCollectionWrapper>();
                QByteArray qtName;
                if(Java::QtCore::QQueue::isInstanceOf(env, java_object)){
                    qtName = "QQueue<";
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                }else if(Java::QtCore::QStack::isInstanceOf(env, java_object)){
                    qtName = "QStack<";
#endif
                }else{
                    qtName = "QList<";
                }
                qtName += containerAccess->elementMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QSet::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QSet::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->elementMetaType().name()).contains("*"))
                    return qMetaTypeId<JCollectionWrapper>();
                QByteArray qtName = "QSet<";
                qtName += containerAccess->elementMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }else if (Java::QtCore::QLinkedList::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QLinkedList::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->elementMetaType().name()).contains("*"))
                    return qMetaTypeId<JCollectionWrapper>();
                QByteArray qtName = "QLinkedList<";
                qtName += containerAccess->elementMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QVector::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QVector::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->elementMetaType().name()).contains("*"))
                    return qMetaTypeId<JCollectionWrapper>();
                QByteArray qtName;
                if(Java::QtCore::QStack::isInstanceOf(env, java_object)){
                    qtName = "QStack<";
                }else{
                    qtName = "QVector<";
                }
                qtName += containerAccess->elementMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
#endif
    }else if (Java::QtCore::QMap::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QMap::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->valueMetaType().name()).contains("*") || QByteArray(containerAccess->keyMetaType().name()).contains("*"))
                    return qMetaTypeId<JMapWrapper>();
                QByteArray qtName = "QMap<";
                qtName += containerAccess->keyMetaType().name();
                qtName += ",";
                qtName += containerAccess->valueMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QHash::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QHash::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->valueMetaType().name()).contains("*") || QByteArray(containerAccess->keyMetaType().name()).contains("*"))
                    return qMetaTypeId<JMapWrapper>();
                QByteArray qtName = "QHash<";
                qtName += containerAccess->keyMetaType().name();
                qtName += ",";
                qtName += containerAccess->valueMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QMultiMap::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QMultiMap::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->valueMetaType().name()).contains("*") || QByteArray(containerAccess->keyMetaType().name()).contains("*"))
                    return qMetaTypeId<JMapWrapper>();
                QByteArray qtName = "QMultiMap<";
                qtName += containerAccess->keyMetaType().name();
                qtName += ",";
                qtName += containerAccess->valueMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else if (Java::QtCore::QMultiHash::isInstanceOf(env, java_object)) {
        if (!Java::QtCore::QMultiHash::isSameClass(env, object_class)) {
            if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, object_class))){
                int metaType = registeredMetaTypeID(*typeId);
                if(metaType!=QMetaType::UnknownType){
                    return metaType;
                }
            }
        }
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
            if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(link->containerAccess())){
                if(QByteArray(containerAccess->valueMetaType().name()).contains("*") || QByteArray(containerAccess->keyMetaType().name()).contains("*"))
                    return qMetaTypeId<JMapWrapper>();
                QByteArray qtName = "QMultiHash<";
                qtName += containerAccess->keyMetaType().name();
                qtName += ",";
                qtName += containerAccess->valueMetaType().name();
                qtName += ">";
                return containerAccess->registerContainer(qtName);
            }
        }
        return qMetaTypeId<JCollectionWrapper>();
    }else{
        return qtjambiMetaTypeId(env, object_class);
    }
}

bool qtjambi_jobjectwrapper_to_qobject_converter(const void *src, void *target) {
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        if(src){
            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
            if(Java::QtCore::QObject::isInstanceOf(env, wrapper->object())){
                QObject* object = qtjambi_to_qobject(env, wrapper->object());
                if(target)
                    *reinterpret_cast<void**>(target) = object;
                return target && object;
            }else if(env->IsSameObject(nullptr, wrapper->object())){
                if(target)
                    *reinterpret_cast<void**>(target) = nullptr;
                return target;
            }else{
                return false;
            }
        }else{
            if(target)
                *reinterpret_cast<void**>(target) = nullptr;
            return target;
        }
    }
    return false;
};

bool qtjambi_qobject_to_jobjectwrapper_converter(const void *src, void *target) {
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        jobject object = qtjambi_from_QObject(env, *reinterpret_cast<QObject* const*>(src));
        new (target)JObjectWrapper(env, object);
        return true;
    }
    return false;
};

bool qtjambi_trivial_converter(const void *src, void *target) {
    *reinterpret_cast<void**>(target) = *reinterpret_cast<void* const*>(src);
    return true;
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QMetaType qtjambiRegisterMetaType(const QByteArray& _type, const QMetaObject *metaObject, bool isQObject, const QString& javaClassName){
    QMetaType metaType(isQObject ? QMetaType::QObjectStar : QMetaType::VoidStar);
    const QtPrivate::QMetaTypeInterface* iface = metaType.iface();
    QMetaType _metaType = createMetaType(QMetaObject::normalizedType(_type),
                                         true,
                                         iface->defaultCtr,
                                         iface->copyCtr,
                                         iface->moveCtr,
                                         iface->dtor,
                                         iface->equals,
                                         iface->lessThan,
                                         iface->debugStream,
                                         iface->dataStreamOut,
                                         iface->dataStreamIn,
                                         iface->legacyRegisterOp,
                                         uint(iface->size),
                                         ushort(iface->alignment),
                                         QMetaType::UnknownType,
                                         QMetaType::TypeFlags(int(iface->flags)),
                                         metaObject, nullptr);
    registerJavaClassForCustomMetaType(_metaType, javaClassName.toLatin1());
    _metaType.id();
    return _metaType;
}

void registerConverterVariant(JNIEnv *env, const QMetaType& metaType, const QString& qtName, const QString& fullJavaName, jclass clazz){
    QMetaType jObjectWrapperType = QMetaType::fromType<JObjectWrapper>();
    if(metaType.isValid() && metaType!=jObjectWrapperType){
        if(Java::QtCore::QObject::isAssignableFrom(env, clazz)){
            if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, metaType))
                QMetaType::registerConverterFunction(&qtjambi_jobjectwrapper_to_qobject_converter, jObjectWrapperType, metaType);
            if(!QMetaType::hasRegisteredConverterFunction(metaType, jObjectWrapperType))
                QMetaType::registerConverterFunction(&qtjambi_qobject_to_jobjectwrapper_converter, metaType, jObjectWrapperType);
        }else{
            if(!QMetaType::hasRegisteredConverterFunction(jObjectWrapperType, metaType)){
                ExternalToInternalConverter converter = QtJambiTypeManager::getExternalToInternalConverter(env, clazz, qtName, metaType);
                clazz = getGlobalClassRef(env, clazz);
                QMetaType::registerConverterFunction([clazz, metaType, converter](const void *src, void *target) -> bool {
                    if(src){
                        if(JNIEnv* env = qtjambi_current_environment()){
                            QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                            const JObjectWrapper * wrapper = reinterpret_cast<const JObjectWrapper *>(src);
                            if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), clazz)){
                                if(!target)
                                    metaType.construct(target, nullptr);
                                if(!wrapper->object()){
                                    return true;
                                }else{
                                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                    jvalue val;
                                    val.l = wrapper->object();
                                    return converter(env, nullptr, val, target, jValueType::l);
                                }
                            }
                        }
                    }
                    return false;
                }, jObjectWrapperType, metaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(metaType, jObjectWrapperType)){
                InternalToExternalConverter converter = QtJambiTypeManager::getInternalToExternalConverter(env, qtName, metaType, clazz);
                QMetaType::registerConverterFunction([converter](const void *src, void *target) -> bool {
                    if(!src){
                        new (target)JObjectWrapper();
                        return true;
                    }else if(JNIEnv* env = qtjambi_current_environment()){
                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                        jvalue val;
                        val.l = nullptr;
                        if(converter(env, nullptr, src, &val, true)){
                            new (target)JObjectWrapper(env, val.l);
                            return true;
                        }
                    }
                    return false;
                }, metaType, jObjectWrapperType);
            }
        }
    }
    JObjectArrayWrapper interfaces(env, Java::Runtime::Class::getInterfaces(env, clazz));
    for(int i=0; i<interfaces.length(); ++i){
        if(jclass interfaceClass = jclass(interfaces.at(env, i))){
            if(!Java::QtJambi::QtObjectInterface::isSameClass(env, interfaceClass)
                    && !Java::QtJambi::QtObject::isSameClass(env, interfaceClass)
                    && !Java::Runtime::Object::isSameClass(env, interfaceClass)){
                QString _fullJavaName = qtjambi_class_name(env, interfaceClass).replace(".", "/");
                QString _qtName = QtJambiTypeManager::getInternalTypeName(env, interfaceClass, false);
                QMetaType _metaType = QMetaType::fromName(_qtName.toLatin1());
                const std::type_info* interfaceTypeId = getTypeByJavaName(_fullJavaName);
                if(!interfaceTypeId){
                    if(jclass _object_class = resolveClosestQtSuperclass(env, interfaceClass)){
                        interfaceClass = _object_class;
                        _fullJavaName = qtjambi_class_name(env, _object_class).replace(".", "/");
                        interfaceTypeId = getTypeByJavaName(_fullJavaName);
                    }
                }
                if(!_metaType.isValid() && _qtName.endsWith("*") && interfaceTypeId){
                    _metaType = qtjambiRegisterMetaType(_qtName.toUtf8(), registeredOriginalMetaObject(*interfaceTypeId), false, _fullJavaName);
                }
                if(_metaType.isValid()){
                    if(!QMetaType::hasRegisteredConverterFunction(metaType, _metaType)){
                        if(metaType==jObjectWrapperType
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                || JObjectValueWrapper::isValueType(metaType)
#endif
                                || isJObjectWrappedMetaType(metaType)){
                            interfaceClass = getGlobalClassRef(env, interfaceClass);
                            const QSharedPointer<QtJambiLink>& (*findLinkForJavaObject)(JNIEnv*,jobject)
                                    = Java::QtJambi::QtJambiObject::isAssignableFrom(env, interfaceClass) ? QtJambiLink::findLinkForJavaObject : QtJambiLink::findLinkForJavaInterface;
                            if(_qtName.endsWith("*")){
                                QMetaType::registerConverterFunction([interfaceClass, interfaceTypeId, findLinkForJavaObject](const void *src, void *target) -> bool {
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                        if(src){
                                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                            if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), interfaceClass)){
                                                void* object = nullptr;
                                                if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object()))
                                                    object = interfaceTypeId ? link->typedPointer(*interfaceTypeId) : link->pointer();
                                                *reinterpret_cast<void**>(target) = object;
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    return false;
                                }, metaType, _metaType);
                            }else{
                                QMetaType::registerConverterFunction([interfaceClass, interfaceTypeId, findLinkForJavaObject, _metaType](const void *src, void *target) -> bool {
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                        if(src){
                                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                            if(env->IsInstanceOf(wrapper->object(), interfaceClass)){
                                                void* object = nullptr;
                                                if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object()))
                                                    object = interfaceTypeId ? link->typedPointer(*interfaceTypeId) : link->pointer();
                                                _metaType.destruct(target);
                                                _metaType.construct(target, object);
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    return false;
                                }, metaType, _metaType);
                            }
                        }else if(!getTypeByJavaName(fullJavaName) && interfaceTypeId){
                            if(jclass _object_class = resolveClosestQtSuperclass(env, clazz)){
                                if(const std::type_info* typeId = getTypeByJavaName(qtjambi_class_name(env, _object_class).replace(".", "/"))){
                                    int offset = registeredInterfaceOffset(*typeId, *interfaceTypeId);
                                    if(offset==0){
                                        QMetaType::registerConverterFunction(&qtjambi_trivial_converter, metaType, _metaType);
                                    }else if(offset>0){
                                        QMetaType::registerConverterFunction([offset](const void *src, void *target) ->bool {
                                                                                 char* ptr = *reinterpret_cast<char*const*>(src);
                                                                                 ptr += offset;
                                                                                 *reinterpret_cast<const void**>(target) = ptr;
                                                                                 return true;
                                                                             }, metaType, _metaType);
                                    }
                                }
                            }
                        }
                    }
                    registerConverterVariant(env, _metaType, _qtName, _fullJavaName, interfaceClass);
                }
            }
        }
    }
    if(!Java::Runtime::Class::isInterface(env, clazz)){
        if(jclass _clazz = env->GetSuperclass(clazz)){
            if(!Java::QtJambi::QtObject::isSameClass(env, _clazz)
                    && !Java::Runtime::Object::isSameClass(env, _clazz)
                    && env->GetArrayLength(Java::Runtime::Class::getTypeParameters(env, _clazz))==0){
                QString _fullJavaName = qtjambi_class_name(env, _clazz).replace(".", "/");
                QString _qtName = QtJambiTypeManager::getInternalTypeName(env, _clazz, false);
                QMetaType _metaType = QMetaType::fromName(_qtName.toLatin1());
                if(!_metaType.isValid()){
                    if(jclass _object_class = resolveClosestQtSuperclass(env, _clazz)){
                        _fullJavaName = qtjambi_class_name(env, _object_class).replace(".", "/");
                        QString __fullJavaName = qtjambi_class_name(env, _object_class).replace(".", "/");
                        QString __qtName = QtJambiTypeManager::getInternalTypeName(env, _object_class, false);
                        _metaType = QMetaType::fromName(__qtName.toLatin1());
                        if(_metaType.isValid()){
                            _fullJavaName = __fullJavaName;
                            _qtName = __qtName;
                        }
                    }
                }
                const std::type_info* superTypeId = getTypeByJavaName(_fullJavaName);
                if(!superTypeId){
                    if(jclass _object_class = resolveClosestQtSuperclass(env, _clazz)){
                        _clazz = _object_class;
                        _fullJavaName = qtjambi_class_name(env, _object_class).replace(".", "/");
                        superTypeId = getTypeByJavaName(_fullJavaName);
                    }
                }
                if(!_metaType.isValid() && _qtName.endsWith("*") && !superTypeId){
                    _metaType = qtjambiRegisterMetaType(_qtName.toUtf8(), registeredOriginalMetaObject(*superTypeId), Java::QtCore::QObject::isAssignableFrom(env, _clazz), _fullJavaName);
                }
                if(_metaType.isValid()){
                    if(!QMetaType::hasRegisteredConverterFunction(metaType, _metaType)){
                        if(metaType==jObjectWrapperType
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                || JObjectValueWrapper::isValueType(metaType)
#endif
                                || isJObjectWrappedMetaType(metaType)){
                            _clazz = getGlobalClassRef(env, _clazz);
                            const QSharedPointer<QtJambiLink>& (*findLinkForJavaObject)(JNIEnv*,jobject)
                                    = Java::QtJambi::QtJambiObject::isAssignableFrom(env, _clazz) ? QtJambiLink::findLinkForJavaObject : QtJambiLink::findLinkForJavaInterface;
                            if(_qtName.endsWith("*")){
                                QMetaType::registerConverterFunction([_clazz, findLinkForJavaObject](const void *src, void *target) -> bool {
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                        if(src){
                                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                            if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), _clazz)){
                                                void* object = nullptr;
                                                if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object()))
                                                    object = link->pointer();
                                                *reinterpret_cast<void**>(target) = object;
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    return false;
                                }, metaType, _metaType);
                            }else{
                                QMetaType::registerConverterFunction([_clazz, findLinkForJavaObject, _metaType](const void *src, void *target) -> bool {
                                    if(JNIEnv* env = qtjambi_current_environment()){
                                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                        if(src){
                                            const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                            if(env->IsInstanceOf(wrapper->object(), _clazz)){
                                                void* object = nullptr;
                                                if(QSharedPointer<QtJambiLink> link = findLinkForJavaObject(env, wrapper->object()))
                                                    object = link->pointer();
                                                _metaType.destruct(target);
                                                _metaType.construct(target, object);
                                                return true;
                                            }else{
                                                return false;
                                            }
                                        }
                                    }
                                    return false;
                                }, metaType, _metaType);
                            }
                        }else{
                            QMetaType::registerConverterFunction(&qtjambi_trivial_converter, metaType, _metaType);
                        }
                    }
                    registerConverterVariant(env, _metaType, _qtName, _fullJavaName, _clazz);
                }
            }
        }
    }

    if(const std::type_info* typeId = getTypeByJavaName(fullJavaName)){
        jobject interfaceList = Java::QtJambi::QtJambiInternal::getAllImplementedInterfaces(env, clazz);
        int count = interfaceList ? qtjambi_collection_size(env, interfaceList) : 0;
        if(count>0){
            jobject iterator = qtjambi_collection_iterator(env, interfaceList);
            while(qtjambi_iterator_has_next(env, iterator)) {
                jclass interfaceClass = jclass(qtjambi_iterator_next(env, iterator));
                if(env->GetArrayLength(Java::Runtime::Class::getTypeParameters(env, interfaceClass))==0){
                    QString _fullJavaName = qtjambi_class_name(env, interfaceClass).replace(".", "/");
                    if(const std::type_info* interfaceTypeId = getTypeByJavaName(_fullJavaName)){
                        QString _qtName = QtJambiTypeManager::getInternalTypeName(env, interfaceClass, false);
                        QMetaType _metaType = QMetaType::fromName(qPrintable(_qtName));
                        if(_metaType.isValid()){
                            if(!QMetaType::hasRegisteredConverterFunction(metaType, _metaType)){
                                if(metaType==jObjectWrapperType
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                        || JObjectValueWrapper::isValueType(metaType)
#endif
                                        || isJObjectWrappedMetaType(metaType)){
                                    if(_qtName.endsWith("*")){
                                        interfaceClass = getGlobalClassRef(env, interfaceClass);
                                        QMetaType::registerConverterFunction([interfaceClass,interfaceTypeId](const void *src, void *target) -> bool {
                                            if(JNIEnv* env = qtjambi_current_environment()){
                                                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                if(src){
                                                    const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                                    if(!wrapper->object() || env->IsInstanceOf(wrapper->object(), interfaceClass)){
                                                        void* object = qtjambi_to_interface(env, wrapper->object(), *interfaceTypeId);
                                                        Q_ASSERT(target);
                                                        *reinterpret_cast<void**>(target) = object;
                                                        return true;
                                                    }else{
                                                        return false;
                                                    }
                                                }
                                            }
                                            return false;
                                        }, metaType, _metaType);
                                    }else{
                                        interfaceClass = getGlobalClassRef(env, interfaceClass);
                                        QMetaType::registerConverterFunction([interfaceClass,interfaceTypeId, _metaType](const void *src, void *target) -> bool {
                                            if(JNIEnv* env = qtjambi_current_environment()){
                                                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                                if(src){
                                                    const JObjectWrapper* wrapper = reinterpret_cast<const JObjectWrapper*>(src);
                                                    if(env->IsInstanceOf(wrapper->object(), interfaceClass)){
                                                        void* object = qtjambi_to_interface(env, wrapper->object(), *interfaceTypeId);
                                                        Q_ASSERT(target);
                                                        _metaType.destruct(target);
                                                        _metaType.construct(target, object);
                                                        return true;
                                                    }else{
                                                        return false;
                                                    }
                                                }
                                            }
                                            return false;
                                        }, metaType, _metaType);
                                    }
                                }else{
                                    int offset = registeredInterfaceOffset(*typeId, *interfaceTypeId);
                                    if(offset==0){
                                        QMetaType::registerConverterFunction(&qtjambi_trivial_converter, metaType, _metaType);
                                    }else if(offset>0){
                                        QMetaType::registerConverterFunction([offset](const void *src, void *target) ->bool {
                                                                                 char* ptr = *reinterpret_cast<char*const*>(src);
                                                                                 ptr += offset;
                                                                                 *reinterpret_cast<const void**>(target) = ptr;
                                                                                 return true;
                                                                             }, metaType, _metaType);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif

AbstractContainerAccess* qtjambi_create_container_access(JNIEnv* env, MapType mapType, const QMetaType& memberMetaType1, const QMetaType& memberMetaType2);
AbstractContainerAccess* qtjambi_create_container_access(JNIEnv* env, MapType mapType,
                                                         const QMetaType& memberMetaType1,
                                                         size_t align1, size_t size1,
                                                         bool isPointer1,
                                                         const QHashFunction& hashFunction1,
                                                         const InternalToExternalConverter& memberConverter1,
                                                         const ExternalToInternalConverter& memberReConverter1,
                                                         const QMetaType& memberMetaType2,
                                                         size_t align2, size_t size2,
                                                         bool isPointer2,
                                                         const QHashFunction& hashFunction2,
                                                         const InternalToExternalConverter& memberConverter2,
                                                         const ExternalToInternalConverter& memberReConverter2);

QVariant qtjambi_to_qvariant(JNIEnv *env, jobject java_object, bool convert)
{
    if (!java_object)
        return QVariant();

    jclass object_class = env->GetObjectClass(java_object);
    if (!object_class)
        return QVariant();

    // Test some quick ones first...
    if (Java::QtCore::QVariant::isInstanceOf(env, java_object)) {
        QVariant* variant = qtjambi_to_object<QVariant>(env, java_object);
        if(variant)
            return *variant;
        else return QVariant();
    } else if (Java::Runtime::Object::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(qMetaTypeId<JObjectWrapper>()), nullptr);
        return QVariant::fromValue(JObjectWrapper(env, java_object));
    } else if (Java::QtCore::QMetaType$GenericEnumerator::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        int value = Java::QtJambi::QtEnumerator::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericByteEnumerator::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        qint8 value = Java::QtJambi::QtByteEnumerator::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericShortEnumerator::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        qint16 value = Java::QtJambi::QtShortEnumerator::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericLongEnumerator::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        qint64 value = Java::QtJambi::QtLongEnumerator::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericFlags::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        int value = Java::QtJambi::QFlags::value(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &value);
    } else if (Java::QtCore::QMetaType$GenericValue::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        return QVariant(FROM_META_TYPE(internalMetaType), qtjambi_to_object(env, java_object));
    } else if (Java::QtCore::QMetaType$GenericObject::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        void* ptr = qtjambi_to_object(env, java_object);
        return QVariant(FROM_META_TYPE(internalMetaType), &ptr);
    } else if (Java::QtCore::QMetaType$GenericGadget::isSameClass(env, object_class)) {
        const QMetaType& internalMetaType = qtjambi_cast<const QMetaType&>(env, Java::QtCore::QMetaType$GenericTypeInterface::metaType(env, java_object));
        if(!convert)
            return QVariant(FROM_META_TYPE(internalMetaType), nullptr);
        void* ptr = qtjambi_to_object(env, java_object);
        if(internalMetaType.flags() & QMetaType::PointerToGadget)
            return QVariant(FROM_META_TYPE(internalMetaType), &ptr);
        else
            return QVariant(FROM_META_TYPE(internalMetaType), ptr);
    } else if (Java::Runtime::String::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::QString), nullptr);
        return qtjambi_to_qstring(env, static_cast<jstring>(java_object));
    } else if (Java::QtCore::QString::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::QString), nullptr);
        QString* strg = qtjambi_to_object<QString>(env, java_object);
        return QVariant(META_TYPE(QMetaType::QString), strg);
    } else if (Java::Runtime::Integer::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Int), nullptr);
        qint32 i = Java::Runtime::Number::intValue(env,java_object);
        return QVariant::fromValue<qint32>(i);
    } else if (Java::Runtime::Character::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::QChar), nullptr);
        ushort s = ushort(Java::Runtime::Character::charValue(env, java_object));
        return QVariant::fromValue<QChar>(QChar(s));
    } else if (Java::Runtime::Double::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Double), nullptr);
        double d = Java::Runtime::Number::doubleValue(env, java_object);
        return QVariant::fromValue<double>(d);
    } else if (Java::Runtime::Float::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Float), nullptr);
        float f = Java::Runtime::Number::floatValue(env, java_object);
        return QVariant::fromValue<float>(f);
    } else if (Java::Runtime::Short::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Short), nullptr);
        qint16 i = Java::Runtime::Number::shortValue(env, java_object);
        return QVariant::fromValue<qint16>(i);
    } else if (Java::Runtime::Byte::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::SChar), nullptr);
        qint8 i = Java::Runtime::Number::byteValue(env, java_object);
        return QVariant::fromValue<qint8>(i);
    } else if (Java::Runtime::Long::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::LongLong), nullptr);
        qint64 i = Java::Runtime::Number::longValue(env, java_object);
        return QVariant::fromValue<qint64>(i);
    } else if (Java::Runtime::Boolean::isSameClass(env, object_class)) {
        if(!convert)
            return QVariant(META_TYPE(QMetaType::Bool), nullptr);
        bool b = Java::Runtime::Boolean::booleanValue(env, java_object);
        return QVariant::fromValue<bool>(b);
    } else if (Java::QtCore::QPair::isInstanceOf(env, java_object)) {
        jobject first = Java::QtCore::QPair::first(env, java_object);
        jobject second = Java::QtCore::QPair::second(env, java_object);
        jclass firstClass = first ? env->GetObjectClass(first) : nullptr;
        jclass secondClass = first ? env->GetObjectClass(second) : nullptr;
        QMetaType firstMetaType(first ? qtjambiMetaTypeId_for_object(env, first) : QMetaType::Nullptr);
        QMetaType secondMetaType(second ? qtjambiMetaTypeId_for_object(env, second) : QMetaType::Nullptr);
        if(firstMetaType.isValid() && secondMetaType.isValid()){
            AbstractContainerAccess* containerAccess = qtjambi_create_container_access(env, MapType::QPair, firstMetaType, secondMetaType);
            if(!containerAccess){
                QString firstType = QLatin1String(firstMetaType.name());
                QString secondType = QLatin1String(secondMetaType.name());
                size_t size1 = 0;
                size_t align1 = 0;
                size_t size2 = 0;
                size_t align2 = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                bool isPointer1 = firstMetaType.flags() & QMetaType::IsPointer;
                if(!isPointer1){
                    size1 = size_t(firstMetaType.sizeOf());
                    align1 = size_t(firstMetaType.alignOf());
                }
#else
                bool isPointer1 = firstMetaType.name().endsWith("*")
                        || firstMetaType.name().contains("(*)")
                        || firstMetaType.name().contains("(__cdecl*)");
                if(!isPointer1){
                    size1 = size_t(firstMetaType.sizeOf());
                    align1 = QtJambiTypeManager::getInternalAlignment(QLatin1String(firstMetaType.name()));
                }
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                bool isPointer2 = secondMetaType.flags() & QMetaType::IsPointer;
                if(!isPointer2){
                    size2 = size_t(secondMetaType.sizeOf());
                    align2 = size_t(secondMetaType.alignOf());
                }
#else
                bool isPointer2 = secondMetaType.name().endsWith("*")
                        || secondMetaType.name().contains("(*)")
                        || secondMetaType.name().contains("(__cdecl*)");
                if(!isPointer2){
                    size2 = size_t(secondMetaType.sizeOf());
                    align2 = QtJambiTypeManager::getInternalAlignment(QLatin1String(secondMetaType.name()));
                }
#endif
                QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, firstMetaType.id());
                QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, secondMetaType.id());
                InternalToExternalConverter memberConverter1 = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                firstType,
                                                                firstMetaType,
                                                                firstClass);
                InternalToExternalConverter memberConverter2 = QtJambiTypeManager::getInternalToExternalConverter(
                                                                env,
                                                                secondType,
                                                                secondMetaType,
                                                                secondClass);
                ExternalToInternalConverter memberReConverter1 = QtJambiTypeManager::getExternalToInternalConverter(
                                                                env, firstClass, firstType, firstMetaType);
                ExternalToInternalConverter memberReConverter2 = QtJambiTypeManager::getExternalToInternalConverter(
                                                                env, secondClass, secondType, secondMetaType);
                containerAccess = qtjambi_create_container_access(
                                               env, MapType::QPair,
                                               firstMetaType,
                                               align1, size1,
                                               isPointer1,
                                               hashFunction1,
                                               memberConverter1,
                                               memberReConverter1,
                                               secondMetaType,
                                               align2, size2,
                                               isPointer2,
                                               hashFunction2,
                                               memberConverter2,
                                               memberReConverter2);
                if(QSharedPointer<AbstractPairAccess> pairAccess = QSharedPointer<AbstractContainerAccess>(containerAccess, &containerDisposer).dynamicCast<AbstractPairAccess>()){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    QString qtName = QLatin1String("QPair<%1,%2>");
#else
                    QString qtName = QLatin1String("std::pair<%1,%2>");
#endif
                    qtName = qtName.arg(firstType, secondType);
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(firstType.contains("*") || secondType.contains("*")){
                        qtjambi_register_container_converter(pairAccess, containerMetaType);
                        return QVariant::fromValue(JObjectWrapper(env, java_object));
                    }else{
                        void* pair = pairAccess->createContainer();
                        pairAccess->setFirst(env, pair, first);
                        pairAccess->setSecond(env, pair, second);
                        QVariant variant(FROM_META_TYPE(containerMetaType), pair);
                        pairAccess->deleteContainer(pair);
                        return variant;
                    }
                }
            }
        }
        return QVariant::fromValue(JObjectWrapper(env, java_object));
    } else if (Java::Runtime::Collection::isInstanceOf(env, java_object)) {
        if (Java::QtCore::QList::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QList<%1>");
                    qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperListAccess*>(containerAccess)){
                        qtjambi_register_container_converter(ContainerType::QList, containerMetaType, containerAccess->elementMetaType());
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, object_class))) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QSet::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractSetAccess* containerAccess = dynamic_cast<AbstractSetAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QSet<%1>");
                    qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperSetAccess*>(containerAccess)){
                        qtjambi_register_container_converter(ContainerType::QSet, containerMetaType, containerAccess->elementMetaType());
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, object_class))) QTJAMBI_STACKTRACEINFO );
            }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        }else if (Java::QtCore::QLinkedList::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QLinkedList<%1>");
                    qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    qtjambi_register_container_converter(ContainerType::QLinkedList, containerMetaType, containerAccess->elementMetaType());
                    if(dynamic_cast<WrapperLinkedListAccess*>(containerAccess)){
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, object_class))) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QVector::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractVectorAccess* containerAccess = dynamic_cast<AbstractVectorAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QVector<%1>");
                    qtName = qtName.arg(QLatin1String(containerAccess->elementMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    qtjambi_register_container_converter(ContainerType::QVector, containerMetaType, containerAccess->elementMetaType());
                    if(dynamic_cast<WrapperVectorAccess*>(containerAccess)){
                        return QVariant::fromValue(JCollectionWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, object_class))) QTJAMBI_STACKTRACEINFO );
            }
#endif
        }
        if(Java::Runtime::Collection::size(env, java_object)>0){
            jobject iter = qtjambi_collection_iterator(env, java_object);
            jobject element = qtjambi_iterator_next(env, iter);
            int elementMetaTypeId = element ? qtjambiMetaTypeId_for_object(env, element) : QMetaType::Nullptr;
            while(qtjambi_iterator_has_next(env, iter)){
                element = qtjambi_iterator_next(env, iter);
                if(element){
                    int _elementMetaTypeId = qtjambiMetaTypeId_for_object(env, element);
                    if(elementMetaTypeId==QMetaType::Nullptr){
                        elementMetaTypeId = _elementMetaTypeId;
                    }else if(_elementMetaTypeId!=elementMetaTypeId){
                        elementMetaTypeId = QMetaType::UnknownType;
                        break;
                    }
                }
            }
            if(elementMetaTypeId == QMetaType::QString){
                QStringList list;
                iter = qtjambi_collection_iterator(env, java_object);
                while(qtjambi_iterator_has_next(env, iter)){
                    element = qtjambi_iterator_next(env, iter);
                    list << qtjambi_to_qstring(env, element);
                }
                return QVariant::fromValue(list);
            }else if(elementMetaTypeId == QMetaType::QVariant){
                QVariantList list;
                iter = qtjambi_collection_iterator(env, java_object);
                while(qtjambi_iterator_has_next(env, iter)){
                    element = qtjambi_iterator_next(env, iter);
                    list << qtjambi_to_qvariant(env, element);
                }
                return QVariant::fromValue(list);
            }else if(elementMetaTypeId != QMetaType::UnknownType && elementMetaTypeId != QMetaType::Nullptr){
                if (Java::Runtime::Set::isInstanceOf(env, java_object)) {
                    return qtjambi_to_qvariant(env,
                                               Java::QtCore::QSet::newInstance2(env, java_object),
                                               convert);
                }else if (Java::Runtime::Deque::isInstanceOf(env, java_object)) {
                    return qtjambi_to_qvariant(env,
                                               Java::QtCore::QStack::newInstance2(env, java_object),
                                               convert);
                }else if (Java::Runtime::Queue::isInstanceOf(env, java_object)) {
                    return qtjambi_to_qvariant(env,
                                               Java::QtCore::QQueue::newInstance2(env, java_object),
                                               convert);
                }else{
                    return qtjambi_to_qvariant(env,
                                               Java::QtCore::QList::newInstance2(env, java_object),
                                               convert);
                }
            }
        }
        if(!convert)
            return QVariant(META_TYPE_FROM(JCollectionWrapper), nullptr);
        return QVariant::fromValue<JCollectionWrapper>(JCollectionWrapper(env, java_object));
    } else if (Java::Runtime::Map::isInstanceOf(env, java_object)) {
        if (Java::QtCore::QMap::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QMap<%1,%2>");
                    qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                   .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperMapAccess*>(link->containerAccess())){
                        qtjambi_register_container_converter(MapType::QMap, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, object_class))) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QHash::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QHash<%1,%2>");
                    qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                   .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperHashAccess*>(link->containerAccess())){
                        qtjambi_register_container_converter(MapType::QHash, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, object_class))) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QMultiMap::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QMultiMap<%1,%2>");
                    qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                   .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperMultiMapAccess*>(link->containerAccess())){
                        qtjambi_register_container_converter(MapType::QMultiMap, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, object_class))) QTJAMBI_STACKTRACEINFO );
            }
        }else if (Java::QtCore::QMultiHash::isInstanceOf(env, java_object)) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
                if(AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(link->containerAccess())){
                    QString qtName = QLatin1String("QMultiHash<%1,%2>");
                    qtName = qtName.arg(QLatin1String(containerAccess->keyMetaType().name()))
                                   .arg(QLatin1String(containerAccess->valueMetaType().name()));
                    QMetaType containerMetaType(containerAccess->registerContainer(qtName.toLatin1()));
                    if(dynamic_cast<WrapperMultiHashAccess*>(link->containerAccess())){
                        qtjambi_register_container_converter(MapType::QMultiHash, containerMetaType, containerAccess->keyMetaType(), containerAccess->valueMetaType());
                        return QVariant::fromValue(JMapWrapper(env, java_object));
                    }else{
                        return QVariant(FROM_META_TYPE(containerMetaType), link->pointer());
                    }
                }
            }else{
                JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, object_class))) QTJAMBI_STACKTRACEINFO );
            }
        }
        if(Java::Runtime::Map::size(env, java_object)>0){
            jobject iter = qtjambi_map_entryset_iterator(env, java_object);
            jobject entry = qtjambi_iterator_next(env, iter);
            jobject keyObject = qtjambi_map$entry_key(env, entry);
            jobject valueObject = qtjambi_map$entry_value(env, entry);
            int keyMetaTypeId = qtjambiMetaTypeId_for_object(env, keyObject);
            int valueMetaTypeId = qtjambiMetaTypeId_for_object(env, valueObject);
            while(qtjambi_iterator_has_next(env, iter)){
                entry = qtjambi_iterator_next(env, iter);
                if(entry){
                    keyObject = qtjambi_map$entry_key(env, entry);
                    if(keyObject){
                        int _keyMetaTypeId = qtjambiMetaTypeId_for_object(env, keyObject);
                        if(keyMetaTypeId==QMetaType::Nullptr){
                            keyMetaTypeId = _keyMetaTypeId;
                        }else if(_keyMetaTypeId!=keyMetaTypeId){
                            keyMetaTypeId = QMetaType::UnknownType;
                            break;
                        }
                    }
                    valueObject = qtjambi_map$entry_value(env, entry);
                    if(valueObject){
                        int _valueMetaTypeId = qtjambiMetaTypeId_for_object(env, valueObject);
                        if(valueMetaTypeId==QMetaType::Nullptr){
                            valueMetaTypeId = _valueMetaTypeId;
                        }else if(_valueMetaTypeId!=valueMetaTypeId){
                            valueMetaTypeId = QMetaType::UnknownType;
                            break;
                        }
                    }
                }
            }
            if(keyMetaTypeId == QMetaType::QString && valueMetaTypeId == QMetaType::QVariant){
                if(Java::Runtime::NavigableMap::isInstanceOf(env, java_object)){
                    QVariantMap map;
                    iter = qtjambi_collection_iterator(env, java_object);
                    while(qtjambi_iterator_has_next(env, iter)){
                        jobject entry = qtjambi_iterator_next(env, iter);
                        jobject keyObject = qtjambi_map$entry_key(env, entry);
                        jobject valueObject = qtjambi_map$entry_value(env, entry);
                        map[qtjambi_to_qstring(env, keyObject)] = qtjambi_to_qvariant(env, valueObject);
                    }
                    return QVariant::fromValue(map);
                }else{
                    QVariantHash map;
                    iter = qtjambi_collection_iterator(env, java_object);
                    while(qtjambi_iterator_has_next(env, iter)){
                        jobject entry = qtjambi_iterator_next(env, iter);
                        jobject keyObject = qtjambi_map$entry_key(env, entry);
                        jobject valueObject = qtjambi_map$entry_value(env, entry);
                        map[qtjambi_to_qstring(env, keyObject)] = qtjambi_to_qvariant(env, valueObject);
                    }
                    return QVariant::fromValue(map);
                }
            }else if(keyMetaTypeId != QMetaType::UnknownType
                     && keyMetaTypeId != QMetaType::Nullptr
                     && valueMetaTypeId != QMetaType::UnknownType
                     && valueMetaTypeId != QMetaType::Nullptr){
                if (Java::Runtime::NavigableMap::isInstanceOf(env, java_object)) {
                    return qtjambi_to_qvariant(env,
                                               Java::QtCore::QMap::newInstance2(env, java_object),
                                               convert);
                }else{
                    return qtjambi_to_qvariant(env,
                                               Java::QtCore::QHash::newInstance2(env, java_object),
                                               convert);
                }
            }
        }
        if(!convert)
            return QVariant(META_TYPE_FROM(JMapWrapper), nullptr);
        return QVariant::fromValue(JMapWrapper(env, java_object));
    } else if (Java::Runtime::Iterator::isInstanceOf(env, java_object)) {
        if(!convert)
            return QVariant(META_TYPE_FROM(JIteratorWrapper), nullptr);
        return QVariant::fromValue(JIteratorWrapper(env, java_object));
    }

    if(Java::Runtime::Class::isArray(env, object_class)){
        jclass componentType = Java::Runtime::Class::getComponentType(env, object_class);
        if(!Java::Runtime::Class::isPrimitive(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JObjectArrayWrapper), nullptr);
            return QVariant::fromValue(JObjectArrayWrapper(env, jobjectArray(java_object)));
        }else if(Java::Runtime::Integer::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JIntArrayWrapper), nullptr);
            return QVariant::fromValue(JIntArrayWrapper(env, jintArray(java_object)));
        }else if(Java::Runtime::Long::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JLongArrayWrapper), nullptr);
            return QVariant::fromValue(JLongArrayWrapper(env, jlongArray(java_object)));
        }else if(Java::Runtime::Short::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JShortArrayWrapper), nullptr);
            return QVariant::fromValue(JShortArrayWrapper(env, jshortArray(java_object)));
        }else if(Java::Runtime::Byte::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JByteArrayWrapper), nullptr);
            return QVariant::fromValue(JByteArrayWrapper(env, jbyteArray(java_object)));
        }else if(Java::Runtime::Boolean::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JBooleanArrayWrapper), nullptr);
            return QVariant::fromValue(JBooleanArrayWrapper(env, jbooleanArray(java_object)));
        }else if(Java::Runtime::Character::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JCharArrayWrapper), nullptr);
            return QVariant::fromValue(JCharArrayWrapper(env, jcharArray(java_object)));
        }else if(Java::Runtime::Double::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JDoubleArrayWrapper), nullptr);
            return QVariant::fromValue(JDoubleArrayWrapper(env, jdoubleArray(java_object)));
        }else if(Java::Runtime::Float::isPrimitiveType(env, componentType)){
            if(!convert)
                return QVariant(META_TYPE_FROM(JFloatArrayWrapper), nullptr);
            return QVariant::fromValue(JFloatArrayWrapper(env, jfloatArray(java_object)));
        }
    }

    // Do the slightly slower fallback...
    QString fullJavaName = qtjambi_class_name(env, object_class).replace(".", "/");

    // may it allow enum and flags?
    QString qtName = QtJambiTypeManager::getInternalTypeName(env, object_class, false);
    if(qtName=="JEnumWrapper"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JEnumWrapper), nullptr);
        return QVariant::fromValue(JEnumWrapper(env, java_object));
    }else if(qtName=="JMapWrapper"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JMapWrapper), nullptr);
        return QVariant::fromValue(JMapWrapper(env, java_object));
    }else if(qtName=="JCollectionWrapper"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JCollectionWrapper), nullptr);
        return QVariant::fromValue(JCollectionWrapper(env, java_object));
    }else if(qtName=="JIteratorWrapper"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JIteratorWrapper), nullptr);
        return QVariant::fromValue(JIteratorWrapper(env, java_object));
    }else if(qtName=="JIntArrayWrapper" || qtName=="JArrayWrapper<jint>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JIntArrayWrapper), nullptr);
        return QVariant::fromValue(JIntArrayWrapper(env, jintArray(java_object)));
    }else if(qtName=="JLongArrayWrapper" || qtName=="JArrayWrapper<jlong>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JLongArrayWrapper), nullptr);
        return QVariant::fromValue(JLongArrayWrapper(env, jlongArray(java_object)));
    }else if(qtName=="JShortArrayWrapper" || qtName=="JArrayWrapper<jshort>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JShortArrayWrapper), nullptr);
        return QVariant::fromValue(JShortArrayWrapper(env, jshortArray(java_object)));
    }else if(qtName=="JByteArrayWrapper" || qtName=="JArrayWrapper<jbyte>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JByteArrayWrapper), nullptr);
        return QVariant::fromValue(JByteArrayWrapper(env, jbyteArray(java_object)));
    }else if(qtName=="JBooleanArrayWrapper" || qtName=="JArrayWrapper<jboolean>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JBooleanArrayWrapper), nullptr);
        return QVariant::fromValue(JBooleanArrayWrapper(env, jbooleanArray(java_object)));
    }else if(qtName=="JCharArrayWrapper" || qtName=="JArrayWrapper<jchar>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JCharArrayWrapper), nullptr);
        return QVariant::fromValue(JCharArrayWrapper(env, jcharArray(java_object)));
    }else if(qtName=="JDoubleArrayWrapper"
             || qtName=="JArrayWrapper<double>"
             || qtName=="JArrayWrapper<jdouble>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JDoubleArrayWrapper), nullptr);
        return QVariant::fromValue(JDoubleArrayWrapper(env, jdoubleArray(java_object)));
    }else if(qtName=="JFloatArrayWrapper"
             || qtName=="JArrayWrapper<float>"
             || qtName=="JArrayWrapper<jfloat>"){
        if(!convert)
            return QVariant(META_TYPE_FROM(JFloatArrayWrapper), nullptr);
        return QVariant::fromValue(JFloatArrayWrapper(env, jfloatArray(java_object)));
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMetaType type = QMetaType::fromName(qPrintable(qtName));
    if(!type.isValid() && qtName.endsWith("*")){
        const std::type_info* typeId = getTypeByJavaName(fullJavaName);
        type = qtjambiRegisterMetaType(qtName.toUtf8(),
                                       typeId ? registeredOriginalMetaObject(*typeId) : nullptr,
                                       Java::QtCore::QObject::isAssignableFrom(env, object_class), fullJavaName);
    }else if(type.isValid() && isJObjectWrappedMetaType(type)){
        JObjectWrapper wrapper(env, java_object);
        return QVariant(type, &wrapper);
    }
#else
    int metaTypeId = !qtName.isEmpty()
        ? QMetaType::type(qPrintable(qtName))
        : int(QMetaType::UnknownType);
    if(metaTypeId == QMetaType::UnknownType && qtName.endsWith("*")){
        if(Java::QtCore::QObject::isAssignableFrom(env, object_class)){
            metaTypeId = qRegisterMetaType<QObject*>(qPrintable(qtName));
        }else{
            metaTypeId = qRegisterMetaType<void*>(qPrintable(qtName));
        }
    }
    QMetaType type(metaTypeId);
#endif

    if(type.isValid() && type != QMetaType::fromType<JObjectWrapper>()){
        bool findConverter = !qtName.endsWith("*") || !convert;
        if(!findConverter){
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_object)){
                if(link->ownership()!=QtJambiLink::Ownership::Java)
                    findConverter = true;
            }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object)){
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }
        if(findConverter){
            if(ExternalToInternalConverter converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, qtName, type)){
                if(convert){
                    void* ptr = nullptr;
                    QtJambiScope scope;
                    jvalue val;
                    val.l = java_object;
                    bool result = converter(env, &scope, val, ptr, jValueType::l);
                    if(!result){
                        qWarning("Cannot convert external type '%s' to '%s'",
                                 qPrintable(fullJavaName.replace("/", ".").replace("$", ".")),
                                 qPrintable(qtName));
                        return QVariant(META_TYPE(QMetaType::UnknownType), nullptr);
                    }
                    return QVariant(FROM_META_TYPE(type), ptr);
                }else{
                    return QVariant(FROM_META_TYPE(type), nullptr);
                }
            }
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    registerConverterVariant(env, type, qtName, fullJavaName, object_class);
#endif
    if(!convert)
        return QVariant(META_TYPE_FROM(JObjectWrapper), nullptr);
    else
        return QVariant::fromValue(JObjectWrapper(env, java_object));
}
#undef META_TYPE

QVariant qtjambi_to_qvariant(JNIEnv *env, jobject java_object){
    return qtjambi_to_qvariant(env, java_object, true);
}

#ifndef Q_CC_MSVC
QByteArray analyze_type_name(QByteArray& typeName);
QByteArray analyze_functionpointer(QByteArray& typeName);
QByteArrayList analyze_namespace(QByteArray& typeName);
QByteArrayList analyze_instantiations(QByteArray& typeName);

QByteArray analyze_type_name(QByteArray& typeName){
    if(!typeName.isEmpty()){
        bool isPointer = false;
        bool isReference = false;
        if(typeName.startsWith("PF")){
            return analyze_functionpointer(typeName);
        }else if(typeName.startsWith("PK")){
            isPointer = true;
            typeName = typeName.mid(2);
        }else if(typeName.startsWith("P")){
            isPointer = true;
            typeName = typeName.mid(1);
        }else if(typeName.startsWith("R")){
            isReference = true;
            typeName = typeName.mid(1);
        }
        if(!typeName.isEmpty()){
            QByteArray result;
            if(typeName.startsWith("St3__1")){
                typeName = typeName.mid(6);
                result = "std::";
            }else if(typeName.startsWith("St")){
                typeName = typeName.mid(2);
                result = "std::";
            }
            switch(typeName[0]){
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                {
                    QString number;
                    number += typeName[0];
                    typeName = typeName.mid(1);
                    while(!typeName.isEmpty() && typeName[0]>='0' && typeName[0]<='9'){
                        number += typeName[0];
                        typeName = typeName.mid(1);
                    }
                    int count = number.toInt();
                    result += typeName.mid(0, count);
                    typeName = typeName.mid(count);
                }
                break;
            case 'b':
                result = "bool";
                typeName = typeName.mid(1);
                break;
            case 'a':
                result = "signed char";
                typeName = typeName.mid(1);
                break;
            case 'c':
                result = "char";
                typeName = typeName.mid(1);
                break;
            case 'h':
                result = "unsigned char";
                typeName = typeName.mid(1);
                break;
            case 'f':
                result = "float";
                typeName = typeName.mid(1);
                break;
            case 'd':
                result = "double";
                typeName = typeName.mid(1);
                break;
            case 'i':
                result = "int";
                typeName = typeName.mid(1);
                break;
            case 'j':
                result = "unsigned int";
                typeName = typeName.mid(1);
                break;
            case 'x':
                result = "long long";
                typeName = typeName.mid(1);
                break;
            case 'y':
                result = "unsigned long long";
                typeName = typeName.mid(1);
                break;
            case 's':
                result = "short";
                typeName = typeName.mid(1);
                break;
            case 't':
                result = "unsigned short";
                typeName = typeName.mid(1);
                break;
            case 'v':
                result = "void";
                typeName = typeName.mid(1);
                break;
            case 'N':
                result = analyze_namespace(typeName).join("::");
                break;
            default:break;
            }
            if(!result.isEmpty()){
                if(!typeName.isEmpty() && typeName[0]=='I'){
                    QByteArrayList instantiations = analyze_instantiations(typeName);
                    if(!instantiations.isEmpty()){
                        result += "<" + instantiations.join(",") + ">";
                    }
                }
                if(isPointer){
                    result += "*";
                }else if(isReference){
                    result += "&";
                }
                return result;
            }
        }
    }
    return typeName;
}

QByteArray analyze_functionpointer(QByteArray& typeName){
    QByteArrayList types;
    typeName = typeName.mid(2);//PF
    while(!typeName.isEmpty()){
        if(typeName[0]=='E'){
            typeName = typeName.mid(1);
            break;
        }else{
            auto size1 = typeName.size();
            types << analyze_type_name(typeName);
            auto size2 = typeName.size();
            if(size1==size2){
                auto idx = typeName.indexOf('E');
                if(idx<0)
                    typeName.clear();
                else
                    typeName = typeName.mid(idx+1);
            }
        }
    }
    QByteArray result;
    if(!types.isEmpty()){
        result += types.takeFirst() + "(*)(";
        if(types.size()>1 || types[0]!="void"){
            result += types.join(",");
        }
        result += ")";
    }
    return result;
}

QByteArrayList analyze_namespace(QByteArray& typeName){
    QByteArrayList result;
    typeName = typeName.mid(1);//N
    while(!typeName.isEmpty()){
        if(typeName=="St3__1"){
        }else if(typeName[0]=='E'){
            typeName = typeName.mid(1);
            break;
        }else{
            auto size1 = typeName.size();
            result << analyze_type_name(typeName);
            auto size2 = typeName.size();
            if(size1==size2){
                auto idx = typeName.indexOf('E');
                if(idx<0)
                    typeName.clear();
                else
                    typeName = typeName.mid(idx+1);
            }
        }
    }
    return result;
}

QByteArrayList analyze_instantiations(QByteArray& typeName){
    QByteArrayList result;
    typeName = typeName.mid(1);//I
    while(!typeName.isEmpty()){
        if(typeName[0]=='I'){
            typeName = typeName.mid(1);
            result << "jlong";
        }else if(typeName[0]=='E'){
            typeName = typeName.mid(1);
            break;
        }else{
            auto size1 = typeName.size();
            result << analyze_type_name(typeName);
            auto size2 = typeName.size();
            if(size1==size2){
                auto idx = typeName.indexOf('E');
                if(idx<0)
                    typeName.clear();
                else
                    typeName = typeName.mid(idx+1);
            }
        }
    }
    return result;
}
#endif

QByteArray qtjambi_type_name(const std::type_info& typeId){
    QByteArray typeName = typeId.name();
#ifdef Q_CC_MSVC
    typeName = typeName.replace(" (__cdecl*)", "(*)");
    typeName = typeName.replace(" __cdecl(", "(");
    typeName = typeName.replace("class ", "");
    typeName = typeName.replace("struct ", "");
    typeName = typeName.replace("union ", "");
    typeName = typeName.replace("enum ", "");
    typeName = typeName.replace(" & __ptr64", "&");
    typeName = typeName.replace(" const * __ptr64", "*");
    typeName = typeName.replace(" * __ptr64", "*");
    typeName = typeName.replace("(void)", "()");
    typeName = typeName.replace("__int64", "long long");
#else
    typeName = analyze_type_name(typeName);
#endif
    return typeName;
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
    }else{
        return false;
    }
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
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, java_object))
        return link->pointer();
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

QObject *qtjambi_to_qobject(JNIEnv *env, jobject java_object)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, java_object))
        return link->isQObject() ? link->qobject() : nullptr;
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
}

struct QtJambiShellAccess : QtJambiShellImpl{
    void overrideLink(const QSharedPointer<QtJambiLink>& link){
        QtJambiShellImpl::overrideLink(link);
    }
};

const void *qtjambi_to_qobject_from_shared_pointer(JNIEnv *env, jobject java_object, PointerCreator pointerCreator, PointerDeleter pointerDeleter, PointerQObjectGetter pointerGetter)
{
    if(java_object){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
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
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(ptr) + infos.at(0).size() );
                }

                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }

                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSharedPointerToQObject(env, java_object, createdByJava, is_shell, pointerCreator(ptr), pointerDeleter, pointerGetter);
                Q_ASSERT(link && link->isSharedPointer());
                if(shell){
                    shell->overrideLink(link);
                }
                return static_cast<QSharedPointerLink*>(link.data())->getSharedPointer();
            }
        }
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }
    // this is a memory leak because this is not linked to java object!!!
    return nullptr;
}

const void *qtjambi_to_object_from_shared_pointer(JNIEnv *env, jobject java_object, PointerCreator pointerCreator, PointerDeleter pointerDeleter, PointerGetter pointerGetter)
{
    if(java_object){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_object)){
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
                    shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(ptr) + infos.at(0).size() );
                }
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                const char* className = link->qtTypeName();
#endif
                PtrOwnerFunction registeredOwnerFunction = nullptr;
                if(!link->isQObject()){
                    PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                    registeredOwnerFunction = polink->ownerFunction();
                }

                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }

                link->invalidate(env);
                link.clear();
                link = QtJambiLink::createLinkForSharedPointerToObject(env,
                                                                           java_object,
                                                                           LINK_NAME_ARG(className)
                                                                           createdByJava,
                                                                           is_shell,
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
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }
    // this is a memory leak because this is not linked to java object!!!
    return nullptr;
}

int qtjambi_to_enum(JNIEnv *env, jobject java_object)
{
    int returned = 0;
    Q_ASSERT(java_object);
    if(Java::Runtime::Number::isInstanceOf(env, java_object)){
        returned = qtjambi_to_int(env, java_object);
    }else if(Java::QtJambi::QFlags::isInstanceOf(env, java_object)){
        returned = Java::QtJambi::QFlags::value(env, java_object);
    }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, java_object)){
        returned = Java::QtJambi::QtEnumerator::value(env,java_object);
    }else if(Java::QtJambi::QtByteEnumerator::isInstanceOf(env, java_object)){
        returned = Java::QtJambi::QtByteEnumerator::value(env, java_object);
    }else if(Java::QtJambi::QtShortEnumerator::isInstanceOf(env, java_object)){
        returned = Java::QtJambi::QtShortEnumerator::value(env, java_object);
    }else if(Java::QtJambi::QtLongEnumerator::isInstanceOf(env, java_object)){
        returned = int(Java::QtJambi::QtLongEnumerator::value(env, java_object));
    }else if(Java::Runtime::Enum::isInstanceOf(env, java_object)){
        returned = Java::Runtime::Enum::ordinal(env, java_object);
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
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object))
        return link->typedPointer(typeId);
    else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    return nullptr;
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

QString qtjambi_external_type_name(JNIEnv *env, const QString& internalName){
    return QtJambiTypeManager::getExternalTypeName(env, internalName);
}

const std::type_info* getTypeByJavaName(QByteArray javaName);

jobject qtjambi_find_object(JNIEnv *env, const void * pointer)
{
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(pointer)){
        if(link)
            return link->getJavaObjectLocalRef(env);
    }
    return nullptr;
}

jobject qtjambi_from_object_no_typeId(JNIEnv *env, const void *qt_object, QByteArray className, jclass clazz, bool makeCopyOfValueTypes, bool cppOwnership)
{
    if (!qt_object || !clazz)
        return nullptr;
    int metaType = QMetaType::UnknownType;
    if(const std::type_info* typeId = getTypeByJavaName(className)){
        metaType = registeredMetaTypeID(*typeId);
        if(metaType==QMetaType::UnknownType){
            metaType = QMetaType::type(getQtName(*typeId));
        }
    }

    // If it's not a value type, we just link to the pointer directly.
    void *copy = nullptr;
    if (!makeCopyOfValueTypes || metaType == QMetaType::UnknownType) {
        // If the object is constructed in Java, then we can look it up
        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
            if(link){
                jobject obj = link->getJavaObjectLocalRef(env);
                if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                    link->invalidate(env);
                }else{
                    jclass targetType = nullptr;
                    try{
                        targetType = resolveClass(env, className);
                    }catch(...){}
                    if(targetType && env->IsInstanceOf(obj, targetType)){
                        return obj;
                    }
                }
            }
        }

        // Otherwise we have to create it
        copy = const_cast<void *>(qt_object);
    } else {
        copy = QMetaType::create(metaType, qt_object);
        if (!copy)
            return nullptr;
    }

    jobject returned = nullptr;
    if (clazz) {
        try{
            if(Java::QtJambi::QtObject::isAssignableFrom(env, clazz)){
                jmethodID constructorId = findInternalPrivateConstructor(env, clazz);
                if(constructorId){
                    returned = env->NewObject(clazz, constructorId, 0);
                    qtjambi_throw_java_exception(env);
                }else{
                    return nullptr;
                }
            }
        }catch(const JavaException& exn){
            if (metaType != QMetaType::UnknownType && copy)
                QMetaType::destroy(metaType, copy);
            throw exn;
        }
    }

    if (!returned)
        return nullptr;

    if (const QSharedPointer<QtJambiLink>& link =
               metaType == QMetaType::UnknownType ? QtJambiLink::createLinkForObject(
                                                    env,
                                                    returned,
                                                    copy,
                                                    LINK_NAME_ARG(nullptr)
                                                    false,
                                                    false)
                                                : QtJambiLink::createLinkForObject(
                                                    env,
                                                    returned,
                                                    copy,
                                                    QMetaType(metaType),
                                                    false,
                                                    false) ) {
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
            qtjambi_throw_java_exception(env);
        }else{
            return nullptr;
        }

        // If it's not a value type, we just link to the pointer directly.
        void *copy = nullptr;
        if (metaType == QMetaType::UnknownType) {
            // If the object is constructed in Java, then we can look it up
            for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
                if(link){
                    jobject obj = link->getJavaObjectLocalRef(env);
                    if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                        link->invalidate(env);
                    }else{
                        jclass targetType = nullptr;
                        try{
                            targetType = resolveClass(env, className);
                        }catch(...){}
                        if(targetType && env->IsInstanceOf(obj, targetType)){
                            return obj;
                        }
                    }
                }
            }

            // Otherwise we have to create it
            copy = const_cast<void *>(qt_object);
        } else {
            copy = QMetaType::create(metaType, qt_object);
            if (!copy)
                return nullptr;
        }

        const QSharedPointer<QtJambiLink>& link =
                metaType == QMetaType::UnknownType ? QtJambiLink::createLinkForObject(env, returned, copy,
                                                       LINK_NAME_ARG(nullptr)
                                                       false, false)
                                                   : QtJambiLink::createLinkForObject(env, returned, copy, QMetaType(metaType),
                                                       false, false);
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

    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(qt_object)){
        if(link){
            jobject obj = link->getJavaObjectLocalRef(env);
            if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                link->invalidate(env);
            }else{
                jclass targetType = nullptr;
                try{
                    targetType = resolveClass(env, className);
                }catch(...){}
                if(targetType && env->IsInstanceOf(obj, targetType)){
                    return obj;
                }
            }
        }
    }

    jobject returned = nullptr;
    jclass clazz = resolveClass(env, className);
    if (clazz && Java::QtJambi::QtObject::isAssignableFrom(env, clazz)) {
        jmethodID constructorId = findInternalPrivateConstructor(env, clazz);
        if(constructorId){
            returned = env->NewObject(clazz, constructorId, 0);
        }else{
            qtjambi_throw_java_exception(env);
            return nullptr;
        }
    }
    if (!returned)
        return nullptr;

    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(
                env,
                returned,
                LINK_NAME_ARG(nullptr)
                false,
                false,
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

bool _qtjambi_invalidate_object(JNIEnv *env, jobject java_object, bool checkJavaOwnership)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, java_object)){
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
    while (meta_object && QtJambiMetaObject::isInstance(meta_object))
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
    else
        return qtjambi_from_qobject_notype(env, qt_object, className, nullptr);
}

jobject qtjambi_from_qobject_type(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId, bool* ok)
{
    if(ok) *ok = true;
    if (!qt_object)
        return nullptr;
    const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeid(*qt_object));
    if(!typeEntry)
        typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeId);
    if(!typeEntry)
        typeEntry = QtJambiTypeEntry::getTypeEntry(env, typeid(QObject));
    if(typeEntry){
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
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object);
    {
        // Since QObjects are created in a class based on virtual function calls,
        // if they at some point during their constructor are converted to Java,
        // the Java object will get the wrong class. In order to fix this as well
        // as possible, we replace the java object if it turns out it has previously
        // been created using a different metaObject than the current. This should
        // at least make the brokeness identical to that of C++, and we can't do this
        // better than C++ since we depend on C++ to do it.
        if(link && !link->createdByJava()){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
            if (p && p->metaObject() && p->metaObject() != qt_object->metaObject()) {
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                // object isn't deleted.
                jobject nativeLink = link->nativeLink(env);
                Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                link->setSplitOwnership(env);
                link.clear();
                locker.relock();
            }
        }
    }
    jobject obj = nullptr;
    if(link){
        obj = link->getJavaObjectLocalRef(env);
        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            if(QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object)){
                QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                locker.unlock();
                delete p;
                jobject nativeLink = link->nativeLink(env);
                if(nativeLink){
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                }
                link->invalidate(env);
                link.clear();
                locker.relock();
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
                    qtjambi_throw_java_exception(env);
                }catch(...){}
                if(cls){
                    if(!clazz || env->IsAssignableFrom(cls, clazz)){
                        java_name = javaName;
                        clazz = cls;
                        break;
                    }
                }
            }
            mo = mo->superClass();
        }
        if(!java_name.isEmpty()){
            if (!clazz) {
                clazz = resolveClass(env, qPrintable(java_name));
            }
            if (clazz) {
                if(Java::Runtime::Class::isInterface(env, clazz)){
                    java_name += "$Impl";
                    clazz = resolveClass(env, qPrintable(java_name));
                }
            }
            if (clazz) {
                int modifiers = Java::Runtime::Class::getModifiers(env, clazz);
                if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                    java_name += "$ConcreteWrapper";
                    clazz = resolveClass(env, qPrintable(java_name));
                }
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
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_object);
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
                QWriteLocker locker(QtJambiLinkUserData::lock());
                QtJambiLinkUserData *p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
                if (p && p->metaObject() && p->metaObject() != qt_object->metaObject()) {
                    QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                    locker.unlock();
                    delete p;
                    // It should already be split ownership, but in case it has been changed, we need to make sure the c++
                    // object isn't deleted.
                    jobject nativeLink = link->nativeLink(env);
                    Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                    link->setSplitOwnership(env);
                    link.clear();
                    locker.relock();
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
                        shell = *reinterpret_cast<QtJambiShellAccess**>( quintptr(qt_object) + infos.at(0).size() );
                    }
                    jobject nativeLink = link->nativeLink(env);
                    if(nativeLink){
                        Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                    }
                    link->invalidate(env);
                    link.clear();
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
                    QtJambiLinkUserData *p;
                    {
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        Q_UNUSED(locker)
                        p = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object);
                        QTJAMBI_SET_OBJECTUSERDATA(QtJambiLinkUserData, qt_object, nullptr);
                    }
                    if(p){
                        delete p;
                        jobject nativeLink = link->nativeLink(env);
                        if(nativeLink){
                            Java::QtJambi::QtJambiInternal$NativeLink::reset(env, nativeLink);
                        }
                        link->invalidate(env);
                        link.clear();
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
            if(Java::Runtime::Class::isInterface(env, clazz)){
                javaClassName += "$Impl";
                clazz = resolveClass(env, javaClassName);
            }
        }
        if (clazz) {
            int modifiers = Java::Runtime::Class::getModifiers(env, clazz);
            if(Java::Runtime::Modifier::isAbstract(env, modifiers)){
                javaClassName += "$ConcreteWrapper";
                clazz = resolveClass(env, javaClassName);
            }
        }
        if (clazz) {
            jmethodID constructorId = findInternalPrivateConstructor(env, clazz);
            qtjambi_throw_java_exception(env);
            Q_ASSERT(constructorId);
            if(constructorId){
                object = env->NewObject(clazz, constructorId, 0);
                qtjambi_throw_java_exception(env);
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
    jmethodID method(nullptr);
    try{
        method = resolveMethod(env, "resolve", qPrintable(sig.arg(className)), cl, true);
    }catch(const JavaException&){}
    if(method){
        obj = env->CallStaticObjectMethod(cl, method, qt_enum);
        qtjambi_throw_java_exception(env);
    }else{
        jint hashCode = Java::Runtime::Object::hashCode(env,cl);
        obj = resolveEnum(env, hashCode, cl, qt_enum, nullptr);
    }
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

jclass getArrayClass(JNIEnv *env, jclass cls, int arrayDepth);

jobject qtjambi_from_flags(JNIEnv *env, int qt_flags, jclass cl)
{
    jobject obj = nullptr;
    Q_ASSERT(cl);
    if(Java::QtJambi::QtFlagEnumerator::isAssignableFrom(env, cl)){
        cl = Java::QtCore::QMetaEnum::flagsType(env, cl);
    }
    if(Java::QtJambi::QFlags::isAssignableFrom(env, cl)){
        if(Java::QtJambi::QFlags::isSameClass(env, cl)){
            cl = Java::QtJambi::QFlags$ConcreteWrapper::getClass(env);
        }
        jmethodID method = nullptr;
        try{
            method = resolveMethod(env, "<init>", "(I)V", cl);
        }catch(const JavaException&){}
        if(method){
            obj = env->NewObject(cl, method, qt_flags);
        }else{
            try{
                method = resolveMethod(env, "<init>", "()V", cl);
            }catch(const JavaException&){}
            if(method){
                obj = env->NewObject(cl, method);
                qtjambi_throw_java_exception(env);
            }else{
                jclass enumClass = Java::QtJambi::MetaObjectTools::getEnumForQFlags(env, cl);
                QString args = QLatin1String("(L%1;)V");
                args = args.arg(qtjambi_class_name(env, enumClass).replace(".", "/"));
                try{
                    method = resolveMethod(env, "<init>", qPrintable(args), cl);
                }catch(const JavaException&){}
                if(method){
                    obj = env->NewObject(cl, method, env->NewObjectArray(0, enumClass, nullptr));
                    qtjambi_throw_java_exception(env);
                }else{
                    obj = env->AllocObject(cl);
                    qtjambi_throw_java_exception(env);
                }
            }
            Java::QtJambi::QFlags::setValue(env, obj, qt_flags);
        }
    }
    return obj;
}

QVariant qtjambi_to_enumerator(JNIEnv *env, jobject value)
{
    Q_ASSERT(value);
    QVariant result;
    if(Java::QtJambi::QFlags::isInstanceOf(env, value)){
        result = QVariant::fromValue<qint32>(Java::QtJambi::QFlags::value(env, value));
    }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, value)){
        result = QVariant::fromValue<qint32>(Java::QtJambi::QtEnumerator::value(env, value));
    }else if(Java::QtJambi::QtByteEnumerator::isInstanceOf(env, value)){
        result = QVariant::fromValue<qint8>(Java::QtJambi::QtByteEnumerator::value(env, value));
    }else if(Java::QtJambi::QtShortEnumerator::isInstanceOf(env, value)){
        result = QVariant::fromValue<qint16>(Java::QtJambi::QtShortEnumerator::value(env, value));
    }else if(Java::QtJambi::QtLongEnumerator::isInstanceOf(env, value)){
        result = QVariant::fromValue<qint64>(Java::QtJambi::QtLongEnumerator::value(env, value));
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
    Q_ASSERT(Java::QtJambi::QNativePointer::isInstanceOf(env, java_object));  // check the java object is right type
    int object_indirections = Java::QtJambi::QNativePointer::indirections(env, java_object);
    // What is this != test doing ?
    if (object_indirections != indirections) {
        JavaException::raiseIllegalArgumentException(env, "Illegal number of indirections" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
    return reinterpret_cast<void *>(Java::QtJambi::QNativePointer::pointer(env,java_object));
}

jobject qtjambi_from_cpointer(JNIEnv *env, const void *qt_object, QNativePointer::Type type, int indirections)
{
    return Java::QtJambi::QNativePointer::fromNative(env, reinterpret_cast<jlong>(qt_object), int(type), jlong(-1), indirections, false);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
jstring qtjambi_from_qstringref(JNIEnv *env, const QStringRef &s)
{
    Q_ASSERT(s.length()>=0);
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.constData()), s.length());
    qtjambi_throw_java_exception(env);
    return str;
}
#else
jstring qtjambi_from_qutf8stringview(JNIEnv *env, QUtf8StringView s)
{
    return qtjambi_from_qstring(env, s.toString());
}

namespace QtAnyStringView{
jstring convert(JNIEnv *env, QUtf8StringView text){
    return qtjambi_from_qstring(env, text.toString());
}

jstring convert(JNIEnv *env, QStringView s){
    Q_ASSERT(s.length()>=0);
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.data()), jsize(s.length()));
    qtjambi_throw_java_exception(env);
    return str;
}

jstring convert(JNIEnv *env, QLatin1String s){
    return qtjambi_from_qstring(env, QString(s));
}
}

jstring qtjambi_from_qanystringview(JNIEnv *env, QAnyStringView s)
{
    return s.visit([env](auto text){return QtAnyStringView::convert(env, text);});
}
#endif

jstring qtjambi_from_qstringview(JNIEnv *env, QStringView s)
{
    Q_ASSERT(s.length()>=0);
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.data()), jsize(s.length()));
    qtjambi_throw_java_exception(env);
    return str;
}

jobject qtjambi_to_charobject(JNIEnv *env, const QChar &strg)
{
    jobject returned = Java::QtCore::QChar::newInstance(env, nullptr);
    QChar* sptr = new QChar(strg);
    if (const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                returned,
                sptr,
                LINK_NAME_ARG("QChar")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QChar*>(ptr);})) {
        // If the type is copied in, we own the pointer
        link->setJavaOwnership(env);
    } else {
        delete sptr;
        returned = nullptr;
    }
    return returned;
}

jobject qtjambi_to_charobject(JNIEnv *env, QChar *strg)
{
    jobject returned = Java::QtCore::QChar::newInstance(env, nullptr);
    if (const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                returned,
                strg,
                LINK_NAME_ARG("QChar")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QChar*>(ptr);})) {
    } else {
        returned = nullptr;
    }
    return returned;
}

jobject qtjambi_to_variantobject(JNIEnv *env, const QVariant &variant)
{
    jobject returned = Java::QtCore::QVariant::newInstance(env, nullptr);
    QVariant* vptr = new QVariant(variant);
    if (const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                returned,
                vptr,
                LINK_NAME_ARG("QVariant")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QVariant*>(ptr);})) {
        // If the type is copied in, we own the pointer
        link->setJavaOwnership(env);
    } else {
        delete vptr;
        returned = nullptr;
    }
    return returned;
}

jobject qtjambi_to_variantobject(JNIEnv *env, QVariant *variant)
{
    jobject returned = Java::QtCore::QVariant::newInstance(env, nullptr);
    if (const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                returned,
                variant,
                LINK_NAME_ARG("QVariant")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QVariant*>(ptr);})) {
        // If the type is copied in, we own the pointer
        link->setJavaOwnership(env);
    } else {
        returned = nullptr;
    }
    return returned;
}

jobject qtjambi_to_stringobject(JNIEnv *env, const QString &strg)
{
    jobject returned = Java::QtCore::QString::newInstance(env, nullptr);
    QString* sptr = new QString(strg);
    if (const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                returned,
                sptr,
                LINK_NAME_ARG("QString")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QString*>(ptr);})) {
        // If the type is copied in, we own the pointer
        link->setJavaOwnership(env);
    } else {
        delete sptr;
        returned = nullptr;
    }
    return returned;
}

jobject qtjambi_to_stringobject(JNIEnv *env, QString *strg)
{
    jobject returned = Java::QtCore::QString::newInstance(env, nullptr);
    if (const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForObject(
                env,
                returned,
                strg,
                LINK_NAME_ARG("QString")
                false,
                false, [](void * ptr,bool){delete reinterpret_cast<QString*>(ptr);})) {
    } else {
        returned = nullptr;
    }
    return returned;
}

jstring qtjambi_from_qstring(JNIEnv *env, const QString &s)
{
    Q_ASSERT(s.length()>=0);
    jstring str = env->NewString(reinterpret_cast<const jchar *>(s.constData()), jsize(s.length()));
    qtjambi_throw_java_exception(env);
    return str;
}

jstring qtjambi_to_jstring(JNIEnv *env, jobject object)
{
    jstring java_string = nullptr;
    if (object){
        if(Java::Runtime::String::isInstanceOf(env, object)){
            java_string = jstring(object);
        }else{
            java_string = Java::Runtime::Object::toString(env, object);
            qtjambi_throw_java_exception(env);
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
    if(Java::QtCore::QString::isInstanceOf(env, object)){
        QString* strg = qtjambi_to_object<QString>(env, object);
        qtjambi_check_resource(env, strg);
        return *strg;
    }else{
        QString result;
        qtjambi_to_qstring(result, env, object);
        return result;
    }
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
        qtjambi_throw_java_exception(env);
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
        qtjambi_throw_java_exception(env);
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

JBufferConstData::JBufferConstData(JNIEnv *env, jobject buffer_object, bool purgeOnDelete) :
    m_buffer_object(buffer_object ? env->NewGlobalRef(buffer_object) : nullptr),
    m_size(0),
    m_data(nullptr),
    m_purgeOnDelete(purgeOnDelete),
    m_isdirect(true)
{
    if(m_buffer_object){
        m_isdirect = Java::Runtime::Buffer::isDirect(env, m_buffer_object);
        if(m_isdirect){
            m_size = env->GetDirectBufferCapacity(m_buffer_object);
            m_data = env->GetDirectBufferAddress(m_buffer_object);
        }else{
            jobject bufferArray = nullptr;
            try{
                bufferArray = Java::Runtime::Buffer::array(env, m_buffer_object);
            }catch(const JavaException&){}
            if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetByteArrayElements(jbyteArray(bufferArray), nullptr);
                        }else{
                            jbyte* array = new jbyte[size_t(m_size)];
                            env->GetByteArrayRegion(jbyteArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jbyte* array = new jbyte[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::ByteBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                //m_size = jsize(size_t(m_size) * sizeof(jbyte) / sizeof(char));
            }else if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetIntArrayElements(jintArray(bufferArray), nullptr);
                        }else{
                            jint* array = new jint[size_t(m_size)];
                            env->GetIntArrayRegion(jintArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jint* array = new jint[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::IntBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jint) / sizeof(char));
            }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetShortArrayElements(jshortArray(bufferArray), nullptr);
                        }else{
                            jshort* array = new jshort[size_t(m_size)];
                            env->GetShortArrayRegion(jshortArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jshort* array = new jshort[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::ShortBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jshort) / sizeof(char));
            }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetCharArrayElements(jcharArray(bufferArray), nullptr);
                        }else{
                            jchar* array = new jchar[size_t(m_size)];
                            env->GetCharArrayRegion(jcharArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jchar* array = new jchar[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::CharBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jchar) / sizeof(char));
            }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetLongArrayElements(jlongArray(bufferArray), nullptr);
                        }else{
                            jlong* array = new jlong[size_t(m_size)];
                            env->GetLongArrayRegion(jlongArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jlong* array = new jlong[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::LongBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jlong) / sizeof(char));
            }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetFloatArrayElements(jfloatArray(bufferArray), nullptr);
                        }else{
                            jfloat* array = new jfloat[size_t(m_size)];
                            env->GetFloatArrayRegion(jfloatArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jfloat* array = new jfloat[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::FloatBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jfloat) / sizeof(char));
            }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetDoubleArrayElements(jdoubleArray(bufferArray), nullptr);
                        }else{
                            jdouble* array = new jdouble[size_t(m_size)];
                            env->GetDoubleArrayRegion(jdoubleArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jdouble* array = new jdouble[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::DoubleBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jdouble) / sizeof(char));
            }
        }
    }
}

JBufferConstData::~JBufferConstData(){
    try{
        if(m_buffer_object && m_data){
            if(JNIEnv* env = qtjambi_current_environment(false)){
                QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                if(!m_isdirect && m_size>0 && m_purgeOnDelete){
                    jobject bufferArray = nullptr;
                    try{
                        bufferArray = Java::Runtime::Buffer::array(env, m_buffer_object);
                    }catch(const JavaException&){}
                    if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                            env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jbyte*>(m_data);
                    }else if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                            env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jint*>(m_data);
                    }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                            env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jshort*>(m_data);
                    }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                            env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jchar*>(m_data);
                    }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                            env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jlong*>(m_data);
                    }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                            env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jfloat*>(m_data);
                    }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                            env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jdouble*>(m_data);
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
    JBufferConstData(env, buffer_object, true)
{
}

JBufferData::~JBufferData(){
    try{
        if(m_buffer_object){
            if(JNIEnv* env = qtjambi_current_environment(false)){
                QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                jboolean isReadOnly = Java::Runtime::Buffer::isReadOnly(env, m_buffer_object);
                if(!m_isdirect && m_size>0){
                    jobject bufferArray = nullptr;
                    try{
                        bufferArray = Java::Runtime::Buffer::array(env, m_buffer_object);
                    }catch(const JavaException&){}
                    if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                            env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), 0);
                            m_data = nullptr;
                        }else{
                            jbyte* array = reinterpret_cast<jbyte*>(m_data);
                            m_data = nullptr;
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jbyte));
                                for(jsize i=0; i<size; ++i){
                                    Java::Runtime::ByteBuffer::put(env, m_buffer_object, i, array[i]);
                                }
                            }
                            delete[] array;
                        }
                    }else if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                            env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data), 0);
                            m_data = nullptr;
                        }else{
                            jint* array = reinterpret_cast<jint*>(m_data);
                            m_data = nullptr;
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jint));
                                for(jsize i=0; i<size; ++i){
                                    Java::Runtime::IntBuffer::put(env, m_buffer_object, i, array[i]);
                                }
                            }
                            delete[] array;
                        }
                    }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                            env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data), 0);
                            m_data = nullptr;
                        }else{
                            jshort* array = reinterpret_cast<jshort*>(m_data);
                            m_data = nullptr;
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jshort));
                                for(jsize i=0; i<size; ++i){
                                    Java::Runtime::ShortBuffer::put(env, m_buffer_object, i, array[i]);
                                }
                            }
                            delete[] array;
                        }
                    }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                            env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data), 0);
                            m_data = nullptr;
                        }else{
                            jchar* array = reinterpret_cast<jchar*>(m_data);
                            m_data = nullptr;
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jchar));
                                for(jsize i=0; i<size; ++i){
                                    Java::Runtime::CharBuffer::put(env, m_buffer_object, i, array[i]);
                                }
                            }
                            delete[] array;
                        }
                    }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                            env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data), 0);
                            m_data = nullptr;
                        }else{
                            jlong* array = reinterpret_cast<jlong*>(m_data);
                            m_data = nullptr;
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jlong));
                                for(jsize i=0; i<size; ++i){
                                    Java::Runtime::LongBuffer::put(env, m_buffer_object, i, array[i]);
                                }
                            }
                            delete[] array;
                        }
                    }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                            env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data), 0);
                            m_data = nullptr;
                        }else{
                            jfloat* array = reinterpret_cast<jfloat*>(m_data);
                            m_data = nullptr;
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jfloat));
                                for(jsize i=0; i<size; ++i){
                                    Java::Runtime::FloatBuffer::put(env, m_buffer_object, i, double(array[i]));
                                }
                            }
                            delete[] array;
                        }
                    }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                            env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data), 0);
                            m_data = nullptr;
                        }else{
                            jdouble* array = reinterpret_cast<jdouble*>(m_data);
                            m_data = nullptr;
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jdouble));
                                for(jsize i=0; i<size; ++i){
                                    Java::Runtime::DoubleBuffer::put(env, m_buffer_object, i, array[i]);
                                }
                            }
                            delete[] array;
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
    qtjambi_throw_java_exception(env);
    Q_ASSERT(arrayElementType);
    jobjectArray array = env->NewObjectArray(jsize(size), arrayElementType, nullptr);
    qtjambi_throw_java_exception(env);
    return array;
}

const char * getJavaInterfaceName(const std::type_info& typeId);

jobjectArray qtjambi_create_array(JNIEnv *env, const std::type_info& componentType, jsize size){
    jclass arrayElementType = resolveClass(env, getJavaInterfaceName(componentType));
    qtjambi_throw_java_exception(env);
    Q_ASSERT(arrayElementType);
    jobjectArray array = env->NewObjectArray(jsize(size), arrayElementType, nullptr);
    qtjambi_throw_java_exception(env);
    return array;
}

#define PointerArrayINIT(Type,type)\
     : PointerArray(env, pointer, env->New##Type##Array(pointer ? size : 0), pointer ? size : 0) {\
    if(pointer){\
        qtjambi_throw_java_exception(env);\
        env->Set##Type##ArrayRegion(array(), 0, size, reinterpret_cast<const j##type *>(pointer));\
        qtjambi_throw_java_exception(env);\
    }\
}

#define PointerArrayDEL(Type,type)\
{\
    if(m_array){\
        if(JNIEnv* env = qtjambi_current_environment()){\
            QTJAMBI_JNI_LOCAL_FRAME(env, 100)\
            env->Get##Type##ArrayRegion(m_array, 0, m_size, reinterpret_cast<j##type *>(m_pointer));\
            qtjambi_throw_java_exception(env);\
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
Char16PointerArray::Char16PointerArray(JNIEnv *env, char16_t* pointer, jsize size) PointerArrayINIT(Char,char)
Char16PointerArray::~Char16PointerArray() PointerArrayDEL(Char,char)
ConstChar16PointerArray::ConstChar16PointerArray(JNIEnv *env, const char16_t* pointer, jsize size) PointerArrayINIT(Char,char)
Char32PointerArray::Char32PointerArray(JNIEnv *env, char32_t* pointer, jsize size) PointerArrayINIT(Int,int)
Char32PointerArray::~Char32PointerArray() PointerArrayDEL(Int,int)
ConstChar32PointerArray::ConstChar32PointerArray(JNIEnv *env, const char32_t* pointer, jsize size) PointerArrayINIT(Int,int)
#endif

#undef PointerArrayINIT
#undef PointerArrayDEL

#define ArrayPointerINIT(Type)\
    if(m_array && m_size>0){\
        m_array_elements = env->Get##Type##ArrayElements(m_array, &m_is_copy);\
        qtjambi_throw_java_exception(env);\
    }
#define ArrayPointerDEL(Type)\
    if(m_array && m_size>0){\
        if(JNIEnv* env = qtjambi_current_environment()){\
            QTJAMBI_JNI_LOCAL_FRAME(env, 100)\
            env->Release##Type##ArrayElements(m_array, m_array_elements, m_writable ? 0 : JNI_ABORT);\
            qtjambi_throw_java_exception(env);\
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
        qtjambi_throw_java_exception(env);
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
                QTJAMBI_JNI_LOCAL_FRAME(env, 100)
                if(sizeof(jboolean)!=sizeof(bool) && m_writable){
                    for (int i= 0; i < m_size; ++i) {
                        m_boolean_array[i] = m_array_elements[i] ? JNI_TRUE : JNI_FALSE;
                    }
                }
                env->ReleaseBooleanArrayElements(m_array, m_boolean_array, m_writable ? 0 : JNI_ABORT);
                qtjambi_throw_java_exception(env);
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
    if(!object)
        return false;
    jclass arrayClass = env->GetObjectClass(object);
    if(Java::Runtime::Class::isArray(env, arrayClass)){
        jclass componentType = Java::Runtime::Class::getComponentType(env, arrayClass);
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
    return qtjambi_is_valid_array(env, object, Java::Runtime::Boolean::primitiveType(env));
}

bool JIntArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Integer::primitiveType(env));
}

bool JShortArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Short::primitiveType(env));
}

bool JByteArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Byte::primitiveType(env));
}

bool JCharArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Character::primitiveType(env));
}

bool JLongArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Long::primitiveType(env));
}

bool JDoubleArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Double::primitiveType(env));
}

bool JFloatArrayPointer::isValidArray(JNIEnv *env, jobject object){
    return qtjambi_is_valid_array(env, object, Java::Runtime::Float::primitiveType(env));
}

J2CStringBuffer::J2CStringBuffer(JNIEnv* env, jstring strg)
    : m_strg(strg ? jstring(env->NewGlobalRef(strg)) : nullptr),
      m_length(strg ? env->GetStringUTFLength(strg) : 0),
      m_data(strg ? env->GetStringUTFChars(strg, nullptr) : nullptr) {
}

J2CStringBuffer::~J2CStringBuffer(){
    if(m_strg){
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 100)
            env->ReleaseStringUTFChars(m_strg, m_data);
            env->DeleteGlobalRef(m_strg);
        }
    }
}

const char* J2CStringBuffer::constData() const {return m_data;}

int J2CStringBuffer::length() const {return m_length;}

JString2QChars::JString2QChars(JNIEnv* env, jstring strg)
    : m_strg(strg ? jstring(env->NewGlobalRef(strg)) : nullptr),
      m_length(strg ? env->GetStringLength(strg) : 0),
      m_data(strg ? env->GetStringChars(strg, nullptr) : nullptr) {
}

JString2QChars::~JString2QChars(){
    if(m_strg){
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 500)
            env->ReleaseStringChars(m_strg, m_data);
            env->DeleteGlobalRef(m_strg);
        }
    }
}

const QChar* JString2QChars::constData() const {return reinterpret_cast<const QChar*>(m_data);}

int JString2QChars::length() const {return m_length;}

Destructor registeredDestructor(const std::type_info& typeId);

jobject qtjambi_array_to_nativepointer(JNIEnv *env, jobjectArray array, size_t elementSize)
{
    Q_ASSERT(array);
    jsize len = env->GetArrayLength(array);
    if (len == 0)
        return nullptr;
    jobject nativePointer = Java::QtJambi::QNativePointer::newInstance(env, jint(QNativePointer::Type::Byte), elementSize * size_t(len), 1, false);
    char *buf = reinterpret_cast<char *>(qtjambi_to_cpointer(env, nativePointer, 1));
    for (jsize i=0; i<len; ++i) {
        jobject java_object = env->GetObjectArrayElement(array, i);
        void *ptr = nullptr;
        if (java_object) {
            if (const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, java_object))
                ptr = link->pointer();
            else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
        if (ptr)
            memcpy(buf + size_t(i) * elementSize, ptr, elementSize);
    }
    return nativePointer;
}

QThread *qtjambi_find_qthread_for_jthread(JNIEnv *env, jobject thread)
{
    jobject associatedObject = Java::QtJambi::QtJambiInternal::findAssociation(env, thread);
    if(associatedObject && Java::QtCore::QThread::isInstanceOf(env, associatedObject)){
        return static_cast<QThread *>(qtjambi_to_qobject(env, associatedObject));
    }
    return nullptr;
}

QThread *qtjambi_to_thread(JNIEnv *env, jobject thread)
{
    if(thread==nullptr)
        return nullptr;

    return qtjambi_find_qthread_for_jthread(env, thread);
}

jobject qtjambi_from_thread_impl(JNIEnv * env, jobject java_qthread, QThread *thread)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, java_qthread)){
        link->setNoThreadInitializationOnPurge(true);
    }
    if(thread == QThread::currentThread()){
        jobject currentThread = Java::Runtime::Thread::currentThread(env);
        qtjambi_adopt_thread(env, currentThread, java_qthread, thread, false);
        return currentThread;
    }else{
        if(thread->eventDispatcher()){
            class Adopter : public QObject{
            public:
                bool event(QEvent* event) override {
                    if(JNIEnv *env = qtjambi_current_environment()){
                        QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                        jobject currentThread = Java::Runtime::Thread::currentThread(env);
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
    }
    return nullptr;
}

jobject qtjambi_from_thread(JNIEnv * env, QThread *thread)
{
    if(!thread)
        return nullptr;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        if(QtJambiLinkUserData* userData = QTJAMBI_GET_OBJECTUSERDATA(QtJambiLinkUserData, thread)){
            QSharedPointer<QtJambiLink> link = userData->link();
            if(link){
                if(jobject o = link->getJavaObjectLocalRef(env)){
                    locker.unlock();
                    jobject result = Java::QtCore::QThread::javaThread(env, o);
                    locker.relock();
                    if(result)
                        return result;
                }
            }
        }
    }
    return qtjambi_from_thread_impl(env, nullptr, thread);
}

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
        Java::QtCore::QModelIndex::row(env,index),
        Java::QtCore::QModelIndex::column(env,index),
        quintptr(Java::QtCore::QModelIndex::internalId(env, index)),
        qtjambi_cast<const QAbstractItemModel *>(env, Java::QtCore::QModelIndex::model(env, index))
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
    return Java::QtCore::QModelIndex::newInstance(env,
                                    jint(index.row()),
                                    jint(index.column()),
                                    jlong(index.internalId()),
                                    qtjambi_cast<jobject>(env, index.model()));
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

Q_GLOBAL_STATIC(QReadWriteLock, gMessageHandlerLock)
Q_GLOBAL_STATIC(QSet<QtMsgType>, gEnabledMessages)
Q_GLOBAL_STATIC(JObjectWrapper, gMessageHandler)
static bool messageHandlerInstalled = false;

void qtjambi_messagehandler_proxy(QtMsgType type, const QMessageLogContext & context, const QString & message)
{
    if(!gMessageHandlerLock.isDestroyed()){
        QReadLocker locker(gMessageHandlerLock());
        if(gEnabledMessages->contains(type)){
            if(JNIEnv *env = qtjambi_current_environment()) {
                QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                QtJambiExceptionInhibitor __exnHandler;
                try{
                    jobject msgType = qtjambi_cast<jobject>(env, type);
                    jobject _context = qtjambi_cast<jobject>(env, &context);
                    jobject msg = qtjambi_from_qstring(env, message);
                    Java::QtCore::QtMessageHandler::accept(env, gMessageHandler->object(), msgType, _context, msg);
                    qtjambi_invalidate_object(env, _context);
                }catch(const JavaException& exn){
                    __exnHandler.handle(env, exn, nullptr);
                }
            }
        }
    }else{
        switch(type){
        case QtMsgType::QtCriticalMsg:
            printf("CRITICAL: %s\n", qPrintable(message));
            break;
        case QtMsgType::QtDebugMsg:
            printf("DEBUG: %s\n", qPrintable(message));
            break;
        case QtMsgType::QtInfoMsg:
            printf("INFO: %s\n", qPrintable(message));
            break;
        case QtMsgType::QtWarningMsg:
            printf("WARNING: %s\n", qPrintable(message));
            break;
        case QtMsgType::QtFatalMsg:
            printf("FATAL: %s\n", qPrintable(message));
            break;
        }
    }
}

jobject qtjambi_install_message_handler(JNIEnv *env, jobject supportedMessageTypes, jobject handler){
    QWriteLocker locker(gMessageHandlerLock());
    gEnabledMessages->clear();
    if(supportedMessageTypes){
        jobject iter = qtjambi_collection_iterator(env, supportedMessageTypes);
        while(qtjambi_iterator_has_next(env, iter)){
            jobject supportedMessageType = qtjambi_iterator_next(env, iter);
            gEnabledMessages->insert(qtjambi_cast<QtMsgType>(env, supportedMessageType));
        }
    }
    messageHandlerInstalled = handler!=nullptr;
    QtMessageHandler oldHandler;
    jobject result;
    if(gEnabledMessages->size()==5){
        QtMessageHandler messageHandler = qtjambi_cast<QtMessageHandler>(env, handler, "QtMessageHandler");
        oldHandler = qInstallMessageHandler(messageHandler);
        if(oldHandler==messageHandler)
            return handler;
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, handler)) {
            if(link->isShell() && link->ownership()==QtJambiLink::Ownership::Java)
                link->setCppOwnership(env);
        }
        if(oldHandler==&qtjambi_messagehandler_proxy){
            result = env->NewLocalRef(gMessageHandler->object());
            *gMessageHandler = JObjectWrapper();
        }else{
            result = qtjambi_cast<jobject>(env, oldHandler);
        }
    }else{
        *gMessageHandler = JObjectWrapper(env, handler);
        oldHandler = qInstallMessageHandler(&qtjambi_messagehandler_proxy);
        if(oldHandler==&qtjambi_messagehandler_proxy){
            result = env->NewLocalRef(gMessageHandler->object());
        }else{
            result = qtjambi_cast<jobject>(env, oldHandler);
        }
        *gMessageHandler = JObjectWrapper(env, handler);
    }
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, result)) {
        if(link->isShell() && link->ownership()!=QtJambiLink::Ownership::Java)
            link->setJavaOwnership(env);
    }
    return result;
}

QObject* connectionSender(const QMetaObject::Connection* connection);

void jobjectwrapper_save(QDataStream &stream, const void *_jObjectWrapper);
void jcollectionwrapper_save(QDataStream &stream, const void *_jCollectionWrapper);
void jmapwrapper_save(QDataStream &stream, const void *_jMapWrapper);
void jcollectionwrapper_load(QDataStream &stream, void *_jCollectionWrapper);
void jmapwrapper_load(QDataStream &stream, void *_jMapWrapper);
void jobjectwrapper_load(QDataStream &stream, void *_jObjectWrapper);

void qtjambi_initialize_thread_affinity_check(JNIEnv *env);

QString qtjambi_function_library_path(const void* qt_plugin_query_metadata);
bool qtjambi_simple_event_notify(void **data);
bool qtjambi_thread_affine_event_notify(void **data);
void registerMetaTypeID(const std::type_info& typeId, const std::type_info& nonPointerTypeId, int qtMetaType);
//void qtjambi_register_containeraccess_all();
void qtjambi_register_containeraccess_0();
void qtjambi_shutdown_atexit();
void registerPluginImporter();

jint qtjambi_startup(JavaVM *vm){
    if(std::atomic<bool>* atm = gIsLoaded){
        if(atm->load())
            return JNI_VERSION_1_8;
        atm->store(true);
    }else{
        return JNI_VERSION_1_8;
    }
    if(std::atomic<JavaVM *>* atm = qtjambi_vm){
        if(atm->load()!=nullptr)
            return JNI_VERSION_1_8;
        atm->store(vm);
    }else{
        return JNI_VERSION_1_8;
    }
    ::atexit(&qtjambi_shutdown_atexit);

    JNIEnv * env = qtjambi_current_environment(true);
#if defined(Q_OS_ANDROID) && !defined(QT_NO_DEBUG)
    if(env)
        Java::Runtime::System::setProperty(env, env->NewStringUTF("io.qt.debug"), env->NewStringUTF("debug"));
#endif
    {
        try{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QtJambiLinkUserData::id();
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if(env){
                qtjambi_initialize_thread_affinity_check(env);
                if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("qt.disable.event.thread.affinity.check"))
                        || Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.disable-event-thread-affinity-check"))
                        || Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("qt.disable.thread.affinity.check"))
                        || Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.disable-thread-affinity-check")))
                    QInternal::registerCallback(QInternal::EventNotifyCallback, &qtjambi_simple_event_notify);
                else
                    QInternal::registerCallback(QInternal::EventNotifyCallback, &qtjambi_thread_affine_event_notify);
            }else{
                QInternal::registerCallback(QInternal::EventNotifyCallback, &qtjambi_thread_affine_event_notify);
            }

            qtjambi_register_containeraccess_0();
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    if(Java::Runtime::Enum::isInstanceOf(env, w.object())){
                        return JEnumWrapper(env, w.object());
                    }
                }
                return JEnumWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JCollectionWrapper>([](const JObjectWrapper& w) -> JCollectionWrapper {
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    if(Java::Runtime::Collection::isInstanceOf(env, w.object())){
                        return JCollectionWrapper(env, w.object());
                    }
                }
                return JCollectionWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JMapWrapper>([](const JObjectWrapper& w) -> JMapWrapper {
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    if(Java::Runtime::Map::isInstanceOf(env, w.object())){
                        return JMapWrapper(env, w.object());
                    }
                }
                return JMapWrapper();
            });
            QMetaType::registerConverter<JObjectWrapper,JObjectArrayWrapper>([](const JObjectWrapper& w) -> JObjectArrayWrapper {
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
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
                registerContainerTypeInfo<QList<QVariant>::const_iterator>("QIterator", "io/qt/core/QIterator", "java/lang/Iterable");
                registerContainerTypeInfo<QMap<QVariant,QVariant>::const_iterator>("QMapIterator", "io/qt/core/QMapIterator", "java/lang/Iterable");
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
                bool hasChanged = false;
                QStringList libraryPaths = QCoreApplication::libraryPaths();
                QFileInfo sunBootLibraryPath(qtjambi_to_qstring(env, Java::Runtime::System::getProperty(env, env->NewStringUTF("sun.boot.library.path"))));
                if(sunBootLibraryPath.exists())
                    hasChanged |= libraryPaths.removeAll(sunBootLibraryPath.absoluteFilePath()) > 0;
                QFileInfo thisLibraryPath(qtjambi_function_library_path(reinterpret_cast<const void*>(&qtjambi_startup)));
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
                        Java::QtJambi::QtJambiResources::addSearchPath(env, Java::Runtime::URL::newInstance(env, qtjambi_cast<jstring>(env, _sourceDir)));
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
                jobject __java_libraryPaths = Java::QtJambi::QtJambiInternal::getLibraryPaths(env);
                // don't use qtjambi_cast here!
                jobject __qt__iterator = qtjambi_collection_iterator(env, __java_libraryPaths);
                while(qtjambi_iterator_has_next(env, __qt__iterator)) {
                    jobject __java_element = qtjambi_iterator_next(env, __qt__iterator);
                    libraryPaths << QFileInfo(qtjambi_to_qstring(env, jstring(__java_element))).absoluteFilePath();
                    hasChanged = true;
                }
                hasChanged |= libraryPaths.removeDuplicates() > 0;
                if(hasChanged)
                    QCoreApplication::setLibraryPaths(libraryPaths);
            }

            if(!QCoreApplication::instance()){
                qAddPreRoutine([](){
                    QtJambiExceptionInhibitor __exnHandler;
                    if(JNIEnv *__jni_env = qtjambi_current_environment()){
                        QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 400)
                        try{
                            Java::QtCore::Internal::QCoreApplication::execPreRoutines(__jni_env);
                        }catch(const JavaException& exn){
                            __exnHandler.handle(__jni_env, exn, "preRoutine");
                        }
                    }
                });
            }
            qAddPostRoutine([](){
                QtJambiExceptionInhibitor __exnHandler;
                if(JNIEnv *__jni_env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 400)
                    try{
                        Java::QtCore::Internal::QCoreApplication::execPostRoutines(__jni_env);
                    }catch(const JavaException& exn){
                        __exnHandler.handle(__jni_env, exn, "postRoutine");
                    }
                }
            });

        }catch(const JavaException& e){
            if(env)
                e.raiseInJava(env);
            else
                qWarning("%s", e.what());
        }catch (const std::exception& exn) {
            qWarning("An error occurred: %s", exn.what());
        }catch (...) {
            qWarning("An error occurred.");
        }
    }
    return JNI_VERSION_1_8;
}

#ifdef Q_OS_ANDROID

int main(int argc, char *argv[])
{
    JNIEnv *env = qtjambi_current_environment();
    if(!env){
        qWarning("Unable to load JVM.");
        return -1;
    }
    QTJAMBI_JNI_LOCAL_FRAME(env, 200)

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
    return -1;
}
#endif

void clear_typehandlers_at_shutdown(JNIEnv *env);
void clear_supertypes_at_shutdown(JNIEnv *env);
void clear_metaobjects_at_shutdown(JNIEnv * env);
void clear_pluginimporter_at_shutdown(JNIEnv * env);
void clear_functionpointers_at_shutdown();

/*!
 * This function is called by Qt Jambi shutdown hook to indicate that
 * the library is being unloaded. It prevents any access to the
 * current JNIEnv *.
 */
void qtjambi_shutdown_atexit()
{
    qtjambi_shutdown(nullptr);
}

void qtjambi_shutdown(JNIEnv * env)
{
    if(std::atomic<bool>* atm = gIsLoaded){
        if(!atm->load())
            return;
        JavaException exception;
        if(env){
            {
                QWriteLocker locker(gMessageHandlerLock());
                if(messageHandlerInstalled){
                    gEnabledMessages->clear();
                    QtMessageHandler oldHandler = qInstallMessageHandler(nullptr);
                    if(oldHandler==&qtjambi_messagehandler_proxy){
                        jobject result = env->NewLocalRef(gMessageHandler->object());
                        *gMessageHandler = JObjectWrapper();
                        messageHandlerInstalled = false;
                        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, result)) {
                            if(link->isShell() && link->ownership()!=QtJambiLink::Ownership::Java)
                                link->setJavaOwnership(env);
                        }
                    }
                }
            }
            try{
                Java::Runtime::System::gc(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
            try{
                Java::QtJambi::QtJambiInternal::terminateCleanupThread(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
        }

        QInternal::unregisterCallback(QInternal::EventNotifyCallback, &qtjambi_simple_event_notify);
        QInternal::unregisterCallback(QInternal::EventNotifyCallback, &qtjambi_thread_affine_event_notify);

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QtJambiVariant::unregisterHandler();
    #endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        clear_pluginimporter_at_shutdown(env);
        clear_supertypes_at_shutdown(env);
        clear_metaobjects_at_shutdown(env);
        clear_typehandlers_at_shutdown(env);
        if(env){
            try{
                Java::QtJambi::NativeLibraryManager::resetDeploymentSpecs(env);
            }catch (const JavaException& exn) {
                exception.addSuppressed(env, exn);
            }
        }
        clear_functionpointers_at_shutdown();
        atm->store(false);
        if(exception)
            throw exception;
    }
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
    if(Java::QtJambi::QFlags::isAssignableFrom(env, object_class)){
        enum E{X};
        return sizeof(QFlags<E>);
    }
    if(Java::QtJambi::QtEnumerator::isAssignableFrom(env, object_class)){
        return sizeof(qint32);
    }
    if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, object_class)){
        return sizeof(qint16);
    }
    if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, object_class)){
        return sizeof(qint8);
    }
    if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, object_class)){
        return sizeof(qint64);
    }
    if(Java::Runtime::Enum::isAssignableFrom(env, object_class)){
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
            size += info.size();
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

JObjectWrapper qtjambi_to_jobjectwrapper(JNIEnv *env, jobject java_object)
{
    return JObjectWrapper(env, java_object);
}

jobject qtjambi_from_jobjectwrapper(JNIEnv *env, const JObjectWrapper &wrapper)
{
    return env->NewLocalRef(wrapper.object());
}

#if 0
QTJAMBI_EXPORT hash_type qHash(const QVariant &variant);

hash_type qHash(const QVariant &variant)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        hash_type hashCode = qHash(variant.userType());
        if(QByteArray(QMetaType(variant.userType()).name()).endsWith("*")){
            const void* ptr = *reinterpret_cast<void*const*>(variant.constData());
            hashCode = hashCode * 31 + qHash(quintptr(ptr));
        }else{
            jobject object = qtjambi_from_qvariant(env, variant);
            if(object){
                hashCode = hashCode * 31 + hash_type(Java::Runtime::Object::hashCode(env, object));
            }else{
                hashCode = hashCode * 31 + qHash(quintptr(variant.constData()));
            }
        }
        return hashCode;
    }
    return 0;
}
#endif

hash_type qHash(const JObjectWrapper &value)
{
    if(JNIEnv* env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        if(value.object()){
            return hash_type(Java::Runtime::Object::hashCode(env, value.object()));
        }
    }
    return 0;
}

class JavaExceptionPrivate : public QSharedData{
public:
    JavaExceptionPrivate() = default;
    JavaExceptionPrivate(const JavaExceptionPrivate& copy)
        : QSharedData(copy), m_throwable(copy.m_throwable), m_what(copy.m_what){}
    JavaExceptionPrivate(JNIEnv *env, jthrowable obj)
        : QSharedData(), m_throwable(env, obj), m_what(){}
    JObjectWrapper m_throwable;
    QByteArray m_what;
    friend JavaException;
};

JavaException::JavaException() Q_DECL_NOEXCEPT
    : QException(), p(){}

JavaException::JavaException(JNIEnv *env, jthrowable obj)
    : QException(), p(env->IsSameObject(obj, nullptr) ? nullptr : new JavaExceptionPrivate(env, obj)){
    update(env);
#if defined(Q_OS_ANDROID)
    static bool preprint = qgetenv("QTJAMBI_PREPRINT_EXCEPTIONS")=="true";
    if(preprint){
        fprintf(stderr, "JavaException about to throw: %s", what());
        env->Throw(obj);
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}

void JavaException::update(JNIEnv *env)
{
    try{
        if(env){
            if(jthrowable t = object()){
                jstring msg = Java::Runtime::Throwable::tryGetMessage(env, t);
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                int length = msg ? env->GetStringUTFLength(msg) : 0;
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                if(length<=0){
                    msg = Java::Runtime::Class::tryGetName(env, env->GetObjectClass(t));
                    if(env->ExceptionCheck()){
                        env->ExceptionDescribe();
                        env->ExceptionClear();
                    }
                    length = msg ? env->GetStringUTFLength(msg) : 0;
                    if(env->ExceptionCheck()){
                        env->ExceptionDescribe();
                        env->ExceptionClear();
                    }
                }
                if(length>0){
                    p->m_what.resize(length);
                    env->GetStringUTFRegion(msg, 0, length, p->m_what.data());
                    if(env->ExceptionCheck()){
                        env->ExceptionDescribe();
                        env->ExceptionClear();
                    }
                }else{
                    p->m_what.clear();
                }
                //report(env);
            }else if(p){
                p = nullptr;
            }
        }
    }catch(...){}
}

JavaException::JavaException(const JavaException& copy) Q_DECL_NOEXCEPT
    : QException(copy), p(copy.p) {}

JavaException::JavaException(JavaException&& copy) Q_DECL_NOEXCEPT
    : QException(std::move(copy)), p(std::move(copy.p)) {}

JavaException::~JavaException() Q_DECL_NOEXCEPT {}

char const* JavaException::what() const Q_DECL_NOEXCEPT
{
    return !p || p->m_what.isEmpty() ? "Unknown exception" : p->m_what.data();
}

void JavaException::raiseInJava(JNIEnv* env) const {
    jthrowable t = object();
    if(t){
        env->Throw(t);
    }
}

JavaException* JavaException::clone() const{
    return new JavaException(*this);
}

jthrowable JavaException::object() const{
    return reinterpret_cast<jthrowable>(p ? p->m_throwable.object() : nullptr);
}

void JavaException::addSuppressed(JNIEnv* env, const JavaException& exn) const{
    if(exn){
        try{
            if(!object()){
                JavaException& _this = *const_cast<JavaException*>(this);
                _this = exn;
            }else{
                Java::Runtime::Throwable::addSuppressed(env, object(), exn.object());
            }
        }catch(const JavaException& _exn){ exn.report(env); _exn.report(env); }catch(...){}
    }
}

void JavaException::report(JNIEnv* env) const{
    jthrowable t = object();
    if(t){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        try{
            Java::QtJambi::QtJambiInternal::reportException(env, nullptr, t);
        }catch(const JavaException& exn){
            printf("QtJambi: exception pending at QtJambiInternal.reportException(...): %s\n", exn.what());
            printf("QtJambi: exception pending in native code: %s\n", what());
        }catch(...){
            printf("QtJambi: exception pending in native code: %s\n", what());
        }
    }
}

JavaException& JavaException::operator =(const JavaException& other) Q_DECL_NOEXCEPT {
    QException::operator=(static_cast<const QException&>(other));
    p = other.p;
    return *this;
}

JavaException& JavaException::operator =(JavaException&& other) Q_DECL_NOEXCEPT {
    QException::operator=(std::move(static_cast<QException&&>(other)));
    p = std::move(other.p);
    return *this;
}

JavaException::operator bool() const Q_DECL_NOEXCEPT {
    return object();
}

void JavaException::raise() const{
    if(object()){
#if defined(Q_OS_ANDROID)
        if(JNIEnv* env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            throw JavaException(env, object());
        }
#endif
        JavaException e = *this;
        throw e;
    }
}

#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) env, t, methodName, fileName, lineNumber
#else
#define QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) env, t
#endif

#ifdef QTJAMBI_STACKTRACE
void JavaException::raise(JNIEnv* env, const char *methodName, const char *fileName, int lineNumber) const {
    jthrowable t = object();
    if(t){
        jstring jmethodName = methodName ? env->NewStringUTF(methodName) : nullptr;
        jstring jfileName = fileName ? env->NewStringUTF(fileName) : nullptr;
        try{
            Java::QtJambi::QtJambiInternal::extendStackTrace(env, t, jmethodName, jfileName, lineNumber);
        }catch(const JavaException& exn){ exn.report(env); }
        throw JavaException(env, t);
    }
}

void raiseJavaException(JNIEnv* env, jthrowable t, const char *methodName, const char *fileName, int lineNumber) {
    jstring jmethodName = methodName ? env->NewStringUTF(methodName) : nullptr;
    jstring jfileName = fileName ? env->NewStringUTF(fileName) : nullptr;
    try{
        Java::QtJambi::QtJambiInternal::extendStackTrace(env, t, jmethodName, jfileName, lineNumber);
    }catch(const JavaException& exn){ exn.report(env); }
    throw JavaException(env, t);
}
#else
#define raiseJavaException(...) throw JavaException(env, t)
#endif

#ifdef QTJAMBI_STACKTRACE
void JavaException::check(JNIEnv* env){
    if(env->ExceptionCheck()){
        jthrowable t = env->ExceptionOccurred();
        env->ExceptionClear();
        if(t){
            throw JavaException(env, t);
        }
    }
}
#endif

void JavaException::check(JNIEnv* env QTJAMBI_STACKTRACEINFO_DECL ){
    if(env->ExceptionCheck()){
        jthrowable t = env->ExceptionOccurred();
        env->ExceptionClear();
        if(t)
            raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
    }
}

void JavaException::raiseIllegalAccessException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::IllegalAccessException::newInstance(env, jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseNullPointerException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::NullPointerException::newInstance(env, jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseIndexOutOfBoundsException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::IndexOutOfBoundsException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseIllegalArgumentException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::IllegalArgumentException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseIllegalStateException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::IllegalStateException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQNoNativeResourcesException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::QtJambi::QNoNativeResourcesException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseNumberFormatException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::NumberFormatException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQNonVirtualOverridingException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::QtJambi::QNonVirtualOverridingException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQNoImplementationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::QtJambi::QNoImplementationException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQThreadAffinityException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::QtJambi::QThreadAffinityException::newInstance(env,jmessage, t1,
                                                          qtjambi_cast<jobject>(env, t2),
                                                          qtjambi_cast<jobject>(env, t3)
                                        );
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseIllegalAccessError(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::IllegalAccessError::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseError(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::Error::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseRuntimeException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::RuntimeException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseUnsupportedOperationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::UnsupportedOperationException::newInstance(env,jmessage);
    raiseJavaException( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

#if defined(QTJAMBI_CENTRAL_TRY_CATCH)

void TypedCatcher::operator()(const JavaException& exn){m_caller(m_functor, exn);}
void TypedTrial::operator()(){m_caller(m_functor);}

void qtjambi_try_catch_any(TypedTrial&& fct, TypedTrial&& anyHandler){
    try{
        fct();
    }catch(...){
        anyHandler();
    }
}

void qtjambi_try_catch(TypedTrial&& fct, TypedCatcher&& handler){
    try{
        fct();
    }catch(const JavaException& exn){
        handler(exn);
    }
}
#endif

const QObject* qtjambi_main_thread_owner(const void *)
{
    if(QCoreApplicationPrivate::is_app_closing){
        return nullptr;
    }
    return QCoreApplication::instance();
}

// Boxing functions
jobject qtjambi_from_int(JNIEnv *env, jint int_value) {
    return Java::Runtime::Integer::valueOf(env, int_value);
}


jint qtjambi_to_int(JNIEnv *env, jobject int_object)
{
    return int_object ? Java::Runtime::Number::intValue(env, int_object) : 0;
}


jobject qtjambi_from_double(JNIEnv *env, jdouble double_value) {
    return Java::Runtime::Double::valueOf(env, double_value);
}


jdouble qtjambi_to_double(JNIEnv *env, jobject double_object) {
    return double_object ? Java::Runtime::Number::doubleValue(env, double_object) : 0;
}

jobject qtjambi_from_boolean(JNIEnv *env, jboolean bool_value)
{
    return Java::Runtime::Boolean::valueOf(env, bool_value);
}

bool qtjambi_to_boolean(JNIEnv *env, jobject bool_object)
{
    return bool_object && Java::Runtime::Boolean::booleanValue(env, bool_object);
}

jlong qtjambi_to_long(JNIEnv *env, jobject long_object)
{
    return long_object ? Java::Runtime::Number::longValue(env, long_object) : 0;
}

jobject qtjambi_from_long(JNIEnv *env, jlong long_value)
{
    return Java::Runtime::Long::valueOf(env,long_value);
}

jobject qtjambi_from_short(JNIEnv *env, jshort short_value)
{
    return Java::Runtime::Short::valueOf(env, short_value);
}

jobject qtjambi_from_float(JNIEnv *env, jfloat float_value)
{
    return Java::Runtime::Float::valueOf(env, double(float_value));
}

jobject qtjambi_from_byte(JNIEnv *env, jbyte byte_value)
{
    return Java::Runtime::Byte::valueOf(env, byte_value);
}


jchar qtjambi_to_char(JNIEnv *env, jobject char_object)
{
    return char_object ? Java::Runtime::Character::charValue(env, char_object) : 0;
}

jobject qtjambi_from_char(JNIEnv *env, jchar char_value)
{
    return Java::Runtime::Character::valueOf(env, char_value);
}

jfloat qtjambi_to_float(JNIEnv *env, jobject float_object)
{
    return float_object ? Java::Runtime::Number::floatValue(env, float_object) : 0;
}

jshort qtjambi_to_short(JNIEnv *env, jobject short_object)
{
    return short_object ? Java::Runtime::Number::shortValue(env, short_object) : 0;
}

jchar qtjambi_to_jchar(JNIEnv *env, jobject char_object)
{
    return char_object ? Java::Runtime::Character::charValue(env, char_object) : 0;
}

jbyte qtjambi_to_byte(JNIEnv *env, jobject byte_object)
{
    return byte_object ? Java::Runtime::Number::byteValue(env, byte_object) : 0;
}

// Container helpers...
jobject qtjambi_pair_new(JNIEnv *env, jobject first, jobject second) {
    return Java::QtCore::QPair::newInstance(env, first, second);
}


jobject qtjambi_pair_get(JNIEnv *env, jobject pair, int pos) {
    Q_ASSERT(pos == 0 || pos == 1);
    if (pos == 0)
        return Java::QtCore::QPair::first(env,pair);
    else
        return Java::QtCore::QPair::second(env,pair);
}

void qtjambi_pair_set(JNIEnv *env, jobject pair, int pos, jobject value) {
    Q_ASSERT(pos == 0 || pos == 1);
    if (pos == 0)
        Java::QtCore::QPair::set_first(env,pair,value);
    else
        Java::QtCore::QPair::set_second(env,pair,value);
}

jobject qtjambi_hashset_new(JNIEnv *env) {
    return Java::Runtime::HashSet::newInstance(env);
}

jobject qtjambi_hashmap_new(JNIEnv *env, int size) {
    return Java::Runtime::HashMap::newInstance(env, size);
}

jobject qtjambi_treemap_new(JNIEnv *env) {
    return Java::Runtime::TreeMap::newInstance(env);
}

jobject qtjambi_treeset_new(JNIEnv *env, LessThanFunction lessThanFct) {
    jobject cmp = Java::QtJambi::QtJambiInternal::createComparator(env, jlong(lessThanFct));
    return Java::Runtime::TreeSet::newInstance2(env, cmp);
}

jobject qtjambi_treemap_new(JNIEnv *env, LessThanFunction lessThanFct) {
    jobject cmp = Java::QtJambi::QtJambiInternal::createComparator(env, jlong(lessThanFct));
    return Java::Runtime::TreeMap::newInstance2(env, cmp);
}

void qtjambi_map_put(JNIEnv *env, jobject map, jobject key, jobject val)
{
    (void)Java::Runtime::Map::put(env, map, key, val);
}

void qtjambi_multimap_put(JNIEnv *env, jobject map, jobject key, jobject val)
{
    Java::QtJambi::QtJambiInternal::putMultiMap(env, map, key, val);
}

int qtjambi_map_size(JNIEnv *env, jobject map)
{
    return Java::Runtime::Map::size(env,map);
}

void qtjambi_map_clear(JNIEnv *env, jobject map)
{
    Java::Runtime::Map::clear(env, map);
}

jobject qtjambi_map_entryset_iterator(JNIEnv *env, jobject map)
{
    jobject set = Java::Runtime::Map::entrySet(env,map);
    return Java::Runtime::Collection::iterator(env,set);
}

jobjectArray qtjambi_map_entryset_array(JNIEnv *env, jobject map)
{
    jobject set = Java::Runtime::Map::entrySet(env,map);
    return Java::Runtime::Collection::toArray(env, set);
}

jobject qtjambi_map$entry_key(JNIEnv *env, jobject entry)
{
    return Java::Runtime::Map$Entry::getKey(env, entry);
}

jobject qtjambi_map$entry_value(JNIEnv *env, jobject entry)
{
    return Java::Runtime::Map$Entry::getValue(env,entry);
}

QPair<jobject, jobject> qtjambi_entryset_array_get(JNIEnv *env, jobjectArray array, jint idx)
{
    jobject entry = env->GetObjectArrayElement(array, idx);
    jobject key = Java::Runtime::Map$Entry::getKey(env,entry);
    jobject value = Java::Runtime::Map$Entry::getValue(env,entry);
    return QPair<jobject, jobject>(key, value);
}

jobject qtjambi_arraylist_new(JNIEnv *env, jint size) {
    return Java::Runtime::ArrayList::newInstance(env, size);
}

jobject qtjambi_linkedlist_new(JNIEnv *env) {
    return Java::Runtime::LinkedList::newInstance(env);
}

jobject qtjambi_deque_new(JNIEnv *env) {
    return Java::Runtime::ArrayDeque::newInstance(env);
}

void qtjambi_collection_add(JNIEnv *env, jobject list, jobject obj) {
    Java::Runtime::Collection::add(env, list, obj);
}

jobject qtjambi_collection_replace(JNIEnv *env, jobject list, int idx, jobject obj){
    if(Java::Runtime::List::isInstanceOf(env, list)){
        return Java::Runtime::List::set(env, list, idx, obj);
    }else{
        jobject result = nullptr;
        int size = qtjambi_collection_size(env, list);
        jobject newList = qtjambi_arraylist_new(env, size);
        jobject iterator = qtjambi_collection_iterator(env, list);
        int i = 0;
        while(qtjambi_iterator_has_next(env, iterator)){
            if(i==idx){
                result = qtjambi_iterator_next(env, iterator);
                qtjambi_collection_add(env, newList, obj);
            }else{
                qtjambi_collection_add(env, newList, qtjambi_iterator_next(env, iterator));
            }
            ++i;
        }
        qtjambi_collection_clear(env, list);
        Java::Runtime::Collection::addAll(env, list, newList);
        return result;
    }
}

jobject qtjambi_collection_remove_last(JNIEnv *env, jobject list){
    int size = qtjambi_collection_size(env, list);
    if(size>0){
        if(Java::Runtime::List::isInstanceOf(env, list)){
            return Java::Runtime::List::removeByIndex(env, list, size-1);
        }else{
            jobject iterator = qtjambi_collection_iterator(env, list);
            jobject last = nullptr;
            while(qtjambi_iterator_has_next(env, iterator)){
                last = qtjambi_iterator_next(env, iterator);
            }
            try {
                Java::Runtime::Iterator::remove(env, iterator);
                return last;
            } catch (const JavaException&) {
            }
            if(last){
                Java::Runtime::Collection::remove(env, list, last);
            }
            return last;
        }
    }else return nullptr;
}

void qtjambi_collection_clear(JNIEnv *env, jobject collection)
{
    Java::Runtime::Collection::clear(env,collection);
}

jobject qtjambi_list_get(JNIEnv *env, jobject list, jint index)
{
    return Java::Runtime::List::get(env, list, index);
}

jobjectArray qtjambi_collection_toArray(JNIEnv *env, jobject col)
{
    return Java::Runtime::Collection::toArray(env, col);
}

int qtjambi_collection_size(JNIEnv *env, jobject col)
{
    return int(Java::Runtime::Collection::size(env, col));
}

jobject qtjambi_collection_iterator(JNIEnv *env, jobject col)
{
    return Java::Runtime::Collection::iterator(env, col);
}

jobject qtjambi_iterator_next(JNIEnv *env, jobject col)
{
    return Java::Runtime::Iterator::next(env, col);
}

bool qtjambi_iterator_has_next(JNIEnv *env, jobject col)
{
    return Java::Runtime::Iterator::hasNext(env, col);
}

jobjectArray qtjambi_to_jobjectArray(JNIEnv *env, const std::type_info& typeInfo, const void* iterable, jsize length, std::function<jobject(JNIEnv *,const void*,jsize)> convertFunction) {
    jclass elementClass = resolveClass(env, getJavaInterfaceName(typeInfo));
    qtjambi_throw_java_exception(env);
    if(!elementClass){
        elementClass = Java::Runtime::Object::getClass(env);
    }

    Q_ASSERT(elementClass);
    jobjectArray out = nullptr;
    if(length>=0){
        out = env->NewObjectArray(length, elementClass, nullptr);
        qtjambi_throw_java_exception(env);
        for (jsize i = 0; i < length; ++i) {
            env->SetObjectArrayElement(out, i, convertFunction(env, iterable, i));
            qtjambi_throw_java_exception(env);
        }
    }
    return out;
}

struct UIInitialCheck{
    typedef void (*WidgetConstructorCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*WindowConstructorCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*ConstructorCheck)(JNIEnv *, const std::type_info&);
    typedef void (*UseCheck)(JNIEnv *, const std::type_info&);
    typedef void (*ArgumentCheck)(JNIEnv *, const char*, const std::type_info&);
    typedef void (*QObjectThreadCheck)(JNIEnv *, const QObject*);
    typedef void (*QObjectArgumentThreadCheck)(JNIEnv *, const char*, const QObject*);
    typedef void (*ValueArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&, const QObject*);
    typedef void (*ValueThreadCheck)(JNIEnv *, const std::type_info&, const QObject*);
    typedef void (*UIArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&);
    typedef void (*GeneralArgumentThreadCheck)(JNIEnv *, const char*, const std::type_info&, const void*);
    typedef void (*GeneralThreadCheck)(JNIEnv *, const std::type_info&, const void*);
    typedef bool (*EventNotify)(QObject *receiver, QEvent *event, bool* result);
    static WindowConstructorCheck windowConstructorCheck;
    static WidgetConstructorCheck widgetConstructorCheck;
    static ConstructorCheck uiConstructorCheck;
    static ConstructorCheck pixmapConstructorCheck;
    static UseCheck pixmapUseCheck;
    static UseCheck uiThreadCheck;
    static ArgumentCheck pixmapArgumentThreadCheck;
    static PtrOwnerFunction getPixmapOwner;
    static ThreadedPixmapsChecker threadedPixmapsChecker;
    static QObjectThreadCheck objectThreadCheck;
    static QObjectArgumentThreadCheck objectArgumentThreadCheck;
    static ValueArgumentThreadCheck valueArgumentThreadCheck;
    static UIArgumentThreadCheck uiArgumentThreadCheck;
    static GeneralArgumentThreadCheck generalArgumentThreadCheck;
    static QObjectThreadCheck objectConstructorThreadCheck;
    static ValueThreadCheck valueConstructorThreadCheck;
    static GeneralThreadCheck generalConstructorThreadCheck;
    static EventNotify eventNotify;

    static void trivial(JNIEnv *, const std::type_info&);
    static void trivial(JNIEnv *, const std::type_info&, const QObject*);
    static void trivial(JNIEnv *, const char*, const std::type_info&);
    static void trivial(JNIEnv *, const char*, const std::type_info&, const void*);
    static void trivial(JNIEnv *, const std::type_info&, const void*);
    static void trivial(JNIEnv *, const QObject*);
    static void trivial(JNIEnv *, const char*, const QObject*);
    static void trivial(JNIEnv *env, const char*, const std::type_info&, const QObject*);
    static bool trivial(QObject *, QEvent *, bool*);
    static const QObject* trivial(const void * ptr);
    static void initialWindowConstructorCheck(JNIEnv *, const std::type_info&, const QObject*);
    static void initialWidgetConstructorCheck(JNIEnv *, const std::type_info&, const QObject*);
    static void initialPixmapConstructorCheck(JNIEnv *, const std::type_info&);
    static void initialUIConstructorCheck(JNIEnv *, const std::type_info&);
    static void initialPixmapUseCheck(JNIEnv *, const std::type_info&);
    static void initialPixmapArgumentCheck(JNIEnv *, const char*, const std::type_info&);
    static bool initialEventNotify(QObject *receiver, QEvent *event, bool* result);
    static bool enabledEventNotify(QObject *receiver, QEvent *event, bool* result);
    static void uiConstructorThreadCheck(JNIEnv *, const std::type_info&);
    static void windowConstructorThreadCheck(JNIEnv *, const std::type_info&, const QObject*);
    static void widgetConstructorThreadCheck(JNIEnv *, const std::type_info&, const QObject*);
    static void pixmapUseThreadCheck(JNIEnv *, const std::type_info&);
    static void enabledPixmapArgumentThreadCheck(JNIEnv *, const char*, const std::type_info&);
    static const QObject* initialGetPixmapOwner(const void * ptr);
    static void enabledQObjectThreadCheck(JNIEnv *env, const QObject* object);
    static void enabledQObjectConstructorThreadCheck(JNIEnv *env, const QObject*);
    static void enabledValueConstructorThreadCheck(JNIEnv *env, const std::type_info&, const QObject*);
    static void enabledGeneralConstructorThreadCheck(JNIEnv *env, const std::type_info&, const void*);
    static void enabledQObjectArgumentThreadCheck(JNIEnv *env, const char* argumentName, const QObject* argument);
    static void enabledValueArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner);
    static void enabledGeneralArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* ptr);
    static void enabledGeneralThreadCheck(JNIEnv *env, const std::type_info& argumentType, const void* ptr);
    static void enabledUIArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);
    static void enabledUIThreadCheck(JNIEnv *, const std::type_info&);
};

UIInitialCheck::WindowConstructorCheck UIInitialCheck::windowConstructorCheck = &UIInitialCheck::initialWindowConstructorCheck;
UIInitialCheck::WidgetConstructorCheck UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::initialWidgetConstructorCheck;
UIInitialCheck::ConstructorCheck UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::initialPixmapConstructorCheck;
UIInitialCheck::ConstructorCheck UIInitialCheck::uiConstructorCheck = &UIInitialCheck::initialUIConstructorCheck;
UIInitialCheck::UseCheck UIInitialCheck::pixmapUseCheck = &UIInitialCheck::initialPixmapUseCheck;
UIInitialCheck::UseCheck UIInitialCheck::uiThreadCheck = &UIInitialCheck::enabledUIThreadCheck;
UIInitialCheck::ArgumentCheck UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::initialPixmapArgumentCheck;
PtrOwnerFunction UIInitialCheck::getPixmapOwner = &UIInitialCheck::initialGetPixmapOwner;
ThreadedPixmapsChecker UIInitialCheck::threadedPixmapsChecker = nullptr;
UIInitialCheck::QObjectThreadCheck UIInitialCheck::objectThreadCheck = &UIInitialCheck::enabledQObjectThreadCheck;
UIInitialCheck::QObjectArgumentThreadCheck UIInitialCheck::objectArgumentThreadCheck = &UIInitialCheck::enabledQObjectArgumentThreadCheck;
UIInitialCheck::ValueArgumentThreadCheck UIInitialCheck::valueArgumentThreadCheck = &UIInitialCheck::enabledValueArgumentThreadCheck;
UIInitialCheck::UIArgumentThreadCheck UIInitialCheck::uiArgumentThreadCheck = &UIInitialCheck::enabledUIArgumentThreadCheck;
UIInitialCheck::GeneralArgumentThreadCheck UIInitialCheck::generalArgumentThreadCheck = &UIInitialCheck::enabledGeneralArgumentThreadCheck;
UIInitialCheck::QObjectThreadCheck UIInitialCheck::objectConstructorThreadCheck = &UIInitialCheck::enabledQObjectConstructorThreadCheck;
UIInitialCheck::ValueThreadCheck UIInitialCheck::valueConstructorThreadCheck = &UIInitialCheck::enabledValueConstructorThreadCheck;
UIInitialCheck::GeneralThreadCheck UIInitialCheck::generalConstructorThreadCheck = &UIInitialCheck::enabledGeneralConstructorThreadCheck;
UIInitialCheck::EventNotify UIInitialCheck::eventNotify = &UIInitialCheck::initialEventNotify;

void qtjambi_install_threaded_pixmaps_checker(ThreadedPixmapsChecker threadedPixmapsChecker){
    UIInitialCheck::threadedPixmapsChecker = threadedPixmapsChecker;
}

void UIInitialCheck::trivial(JNIEnv *, const std::type_info&){}
void UIInitialCheck::trivial(JNIEnv *, const std::type_info&, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&){}
void UIInitialCheck::trivial(JNIEnv *, const std::type_info&, const void *){}
void UIInitialCheck::trivial(JNIEnv *, const QObject*){}
const QObject* UIInitialCheck::trivial(const void *){return nullptr;}
void UIInitialCheck::trivial(JNIEnv *, const char*, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&, const QObject*){}
void UIInitialCheck::trivial(JNIEnv *, const char*, const std::type_info&, const void*){}
bool UIInitialCheck::trivial(QObject *, QEvent *, bool*){return false;}

bool UIInitialCheck::initialEventNotify(QObject *receiver, QEvent *event, bool* result){
    QCoreApplication* instance = QCoreApplication::instance();
    if(instance) {
        if(dynamic_cast<QtJambiShellInterface*>(instance)){
            UIInitialCheck::eventNotify = &UIInitialCheck::trivial;
        }else{
            UIInitialCheck::eventNotify = &UIInitialCheck::enabledEventNotify;
            return enabledEventNotify(receiver, event, result);
        }
    }
    return false;
}

bool UIInitialCheck::enabledEventNotify(QObject *receiver, QEvent *event, bool* result){
    QObjectPrivate *d = QObjectPrivate::get(receiver);
    QThreadData *threadData = d->threadData;
    QScopedScopeLevelCounter scopeLevelCounter(threadData);
    QtJambiExceptionBlocker __qt_exceptionHandler;
    *result = QCoreApplication::instance()->notify(receiver, event);
    __qt_exceptionHandler.release(nullptr);
    return true;
}

void UIInitialCheck::uiConstructorThreadCheck(JNIEnv *env, const std::type_info& constructedType){
    QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=mainThread){
        JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 created outside main thread").arg(getQtName(constructedType))) QTJAMBI_STACKTRACEINFO, nullptr,
                                                    currentThread, mainThread );
    }
}

void UIInitialCheck::pixmapUseThreadCheck(JNIEnv *env, const std::type_info& constructedType){
    QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=mainThread){
        const char* typeName = getQtName(constructedType);
        if(QLatin1String("QPixmap")==typeName || QLatin1String("QBitmap")==typeName){
            JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 used from outside main thread").arg(typeName)) QTJAMBI_STACKTRACEINFO, nullptr,
                                                        currentThread, mainThread );
        }else{
            JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("QPixmap used in %1 from outside main thread").arg(typeName)) QTJAMBI_STACKTRACEINFO, nullptr,
                                                        currentThread, mainThread );
        }
    }
}

void UIInitialCheck::enabledPixmapArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=mainThread){
        JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 used as argument '%2' from outside main thread").arg(getQtName(argumentType)).arg(argumentName)) QTJAMBI_STACKTRACEINFO ,
                                                     nullptr,
                                                     mainThread, currentThread);
    }
}

void UIInitialCheck::initialPixmapConstructorCheck(JNIEnv *env, const std::type_info& constructedType){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        JavaException::raiseIllegalStateException(env, qPrintable(QLatin1String("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType))) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            JavaException::raiseIllegalStateException(env, qPrintable(QLatin1String("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType)).arg(instance->metaObject()->className())) QTJAMBI_STACKTRACEINFO);
        else{
            if(threadedPixmapsChecker()){
                pixmapConstructorCheck = &trivial;
            }else{
                pixmapConstructorCheck = &uiConstructorThreadCheck;
            }
            qAddPostRoutine([](){ UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::initialPixmapConstructorCheck; });
            pixmapConstructorCheck(env, constructedType);
        }
    }
}

const QObject* UIInitialCheck::initialGetPixmapOwner(const void * ptr){
    if(threadedPixmapsChecker()){
        getPixmapOwner = &trivial;
    }else{
        getPixmapOwner = &qtjambi_main_thread_owner;
    }
    return getPixmapOwner(ptr);
}

void UIInitialCheck::initialUIConstructorCheck(JNIEnv *env, const std::type_info& constructedType){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        JavaException::raiseIllegalStateException(env, qPrintable(QLatin1String("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType))) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            JavaException::raiseIllegalStateException(env, qPrintable(QLatin1String("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType)).arg(instance->metaObject()->className())) QTJAMBI_STACKTRACEINFO);
        else{
            uiConstructorCheck = &uiConstructorThreadCheck;
            qAddPostRoutine([](){ UIInitialCheck::uiConstructorCheck = &UIInitialCheck::initialUIConstructorCheck; });
            uiConstructorCheck(env, constructedType);
        }
    }
}

void UIInitialCheck::initialPixmapUseCheck(JNIEnv *env, const std::type_info& usedType){
    if(threadedPixmapsChecker()){
        pixmapUseCheck = &trivial;
    }else{
        pixmapUseCheck = &pixmapUseThreadCheck;
    }
    qAddPostRoutine([](){ UIInitialCheck::pixmapUseCheck = &UIInitialCheck::initialPixmapUseCheck; });
    pixmapUseCheck(env, usedType);
}

void UIInitialCheck::initialPixmapArgumentCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    if(threadedPixmapsChecker()){
        pixmapArgumentThreadCheck = &trivial;
    }else{
        pixmapArgumentThreadCheck = &enabledPixmapArgumentThreadCheck;
    }
    qAddPostRoutine([](){ UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::initialPixmapArgumentCheck; });
    pixmapArgumentThreadCheck(env, argumentName, argumentType);
}

void UIInitialCheck::windowConstructorThreadCheck(JNIEnv *env, const std::type_info&, const QObject* parent){
    if(parent){
        QThread* currentThread = QThread::currentThread();
        QThread* objectThread = parent->thread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parent))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used as parent from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         qtjambi_from_QObject(env, parent),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::widgetConstructorThreadCheck(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=mainThread){
        JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 created outside main thread").arg(getQtName(constructedType))) QTJAMBI_STACKTRACEINFO, nullptr,
                                                    currentThread, mainThread );
    }else if(parent){
        QThread* objectThread = parent->thread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parent))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used as parent from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         qtjambi_from_QObject(env, parent),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::initialWindowConstructorCheck(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        JavaException::raiseIllegalStateException(env, qPrintable(QLatin1String("Cannot create %1 before initializing QGuiApplication.").arg(getQtName(constructedType))) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QGuiApplication"))
            JavaException::raiseIllegalStateException(env, qPrintable(QLatin1String("Cannot create %1 without QGuiApplication (instead of %2).").arg(getQtName(constructedType)).arg(instance->metaObject()->className())) QTJAMBI_STACKTRACEINFO);
        else{
            windowConstructorCheck = &windowConstructorThreadCheck;
            qAddPostRoutine([](){ UIInitialCheck::windowConstructorCheck = &UIInitialCheck::initialWindowConstructorCheck; });
            windowConstructorCheck(env, constructedType, parent);
        }
    }
}

void UIInitialCheck::initialWidgetConstructorCheck(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    QCoreApplication* instance = QCoreApplication::instance();
    if(!instance) {
        JavaException::raiseIllegalStateException(env, qPrintable(QLatin1String("Cannot create %1 before initializing QApplication.").arg(getQtName(constructedType))) QTJAMBI_STACKTRACEINFO);
    }else{
        if(!instance->inherits("QApplication")){
            JavaException::raiseIllegalStateException(env, qPrintable(QLatin1String("Cannot create %1 without QApplication (instead of %2).").arg(getQtName(constructedType)).arg(instance->metaObject()->className())) QTJAMBI_STACKTRACEINFO);
        }else{
            widgetConstructorCheck = &widgetConstructorThreadCheck;
            qAddPostRoutine([](){ UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::initialWidgetConstructorCheck; });
            widgetConstructorCheck(env, constructedType, parent);
        }
    }
}

void UIInitialCheck::enabledQObjectThreadCheck(JNIEnv *env, const QObject* object){
    if(object){
        QThread* objectThread = object->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(object))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         qtjambi_from_QObject(env, object),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledUIThreadCheck(JNIEnv *env, const std::type_info& typeId){
    QThread* objectThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=objectThread){
        JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 used from outside main thread").arg(getQtName(typeId))) QTJAMBI_STACKTRACEINFO ,
                                                     nullptr,
                                                     objectThread, currentThread);
    }
}

void UIInitialCheck::enabledQObjectArgumentThreadCheck(JNIEnv *env, const char* argumentName, const QObject* argument){
    if(argument){
        QThread* objectThread = argument->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(argument))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("QObject used as argument '%1' from outside its own thread").arg(argumentName)) QTJAMBI_STACKTRACEINFO ,
                                                         qtjambi_from_QObject(env, argument),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledValueArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner){
    if(argumentOwner){
        QThread* objectThread = argumentOwner->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(argumentOwner))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 used as argument '%2' from outside its own thread").arg(getQtName(argumentType)).arg(argumentName)) QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledGeneralArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* argument){
    if(argument){
        if(PtrOwnerFunction ownerFunction = registeredOwnerFunction(argumentType)){
            enabledValueArgumentThreadCheck(env, argumentName, argumentType, ownerFunction(argument));
        }
    }
}

void UIInitialCheck::enabledUIArgumentThreadCheck(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    QThread* objectThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
    QThread* currentThread = QThread::currentThread();
    if(currentThread!=objectThread){
        JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 used as argument '%2' from outside main thread").arg(getQtName(argumentType)).arg(argumentName)) QTJAMBI_STACKTRACEINFO ,
                                                     nullptr,
                                                     objectThread, currentThread);
    }
}

void UIInitialCheck::enabledQObjectConstructorThreadCheck(JNIEnv *env, const QObject* parent){
    if(parent){
        QThread* currentThread = QThread::currentThread();
        QThread* objectThread = parent->thread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parent))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, "QObject used as parent from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                         qtjambi_from_QObject(env, parent),
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledValueConstructorThreadCheck(JNIEnv *env, const std::type_info& parentType, const QObject* parentOwner){
    if(parentOwner){
        QThread* objectThread = parentOwner->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(const_cast<QObject*>(parentOwner))){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 used as parent from outside its own thread").arg(getQtName(parentType))) QTJAMBI_STACKTRACEINFO ,
                                                         nullptr,
                                                         objectThread, currentThread);
        }
    }
}

void UIInitialCheck::enabledGeneralConstructorThreadCheck(JNIEnv *env, const std::type_info& parentType, const void* parent){
    if(parent){
        if(PtrOwnerFunction ownerFunction = registeredOwnerFunction(parentType)){
            enabledValueConstructorThreadCheck(env, parentType, ownerFunction(parent));
        }
    }
}

void qtjambi_thread_check(JNIEnv *env, const QObject* object){
    UIInitialCheck::objectThreadCheck(env, object);
}

void qtjambi_pixmap_thread_check(JNIEnv *env, const std::type_info& typeId){
    UIInitialCheck::pixmapUseCheck(env, typeId);
}

void qtjambi_ui_thread_check(JNIEnv *env, const std::type_info& typeId){
    UIInitialCheck::uiThreadCheck(env, typeId);
}

void qtjambi_argument_thread_check(JNIEnv *env, const char* argumentName, const QObject* argument){
    UIInitialCheck::objectArgumentThreadCheck(env, argumentName, argument);
}

void qtjambi_argument_thread_check(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner){
    UIInitialCheck::valueArgumentThreadCheck(env, argumentName, argumentType, argumentOwner);
}

void qtjambi_argument_ui_thread_check(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    UIInitialCheck::uiArgumentThreadCheck(env, argumentName, argumentType);
}

void qtjambi_argument_pixmap_thread_check(JNIEnv *env, const char* argumentName, const std::type_info& argumentType){
    UIInitialCheck::pixmapArgumentThreadCheck(env, argumentName, argumentType);
}

const QObject* qtjambi_pixmap_owner_function(const void * ptr){
    return UIInitialCheck::getPixmapOwner(ptr);
}

void qtjambi_argument_thread_check(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* argument){
    UIInitialCheck::generalArgumentThreadCheck(env, argumentName, argumentType, argument);
}

void qtjambi_constructor_thread_check(JNIEnv *env, const QObject* parent){
    UIInitialCheck::objectConstructorThreadCheck(env, parent);
}

void qtjambi_constructor_thread_check(JNIEnv *env, const std::type_info& parentType, const QObject* parentOwner){
    UIInitialCheck::valueConstructorThreadCheck(env, parentType, parentOwner);
}

void qtjambi_constructor_thread_check(JNIEnv *env, const std::type_info& parentType, const void* parent){
    UIInitialCheck::generalConstructorThreadCheck(env, parentType, parent);
}

void qtjambi_constructor_ui_thread_check(JNIEnv *env, const std::type_info& constructedType){
    UIInitialCheck::uiConstructorCheck(env, constructedType);
}

void qtjambi_constructor_pixmap_thread_check(JNIEnv *env, const std::type_info& constructedType){
    UIInitialCheck::pixmapConstructorCheck(env, constructedType);
}

void qtjambi_constructor_widget_thread_check(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    UIInitialCheck::widgetConstructorCheck(env, constructedType, parent);
}

void qtjambi_constructor_window_thread_check(JNIEnv *env, const std::type_info& constructedType, const QObject* parent){
    UIInitialCheck::windowConstructorCheck(env, constructedType, parent);
}

void qtjambi_constructor_application_thread_check(JNIEnv *env, const std::type_info& constructedType){
#ifdef Q_OS_DARWIN
    if (constructedType!=typeid(QCoreApplication) && !pthread_main_np()) {
        JavaException::raiseError(env, "QtJambi does not appear to be running on the main thread and will "
                 "most likely be unstable and crash. "
                 "Please make sure to launch your 'java' command with the "
                 "'-XstartOnFirstThread' command line option. For instance: "
                 "java -XstartOnFirstThread any.vendor.MainClass" QTJAMBI_STACKTRACEINFO );
    }
#endif
    if(!Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("qt.disable.thread.affinity.check"))
            && !Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.disable-thread-affinity-check"))){
        QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
        QThread* currentThread = QThread::currentThread();
        if(currentThread!=mainThread){
            JavaException::raiseQThreadAffinityException(env, qPrintable(QLatin1String("%1 created outside main thread").arg(getQtName(constructedType))) QTJAMBI_STACKTRACEINFO, nullptr,
                                                        currentThread, mainThread );
        }
    }
}

void qtjambi_initialize_thread_affinity_check(JNIEnv *env){
    if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("qt.disable.thread.affinity.check"))
            || Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.disable-thread-affinity-check"))){
        UIInitialCheck::objectThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::pixmapUseCheck = &UIInitialCheck::trivial;
        UIInitialCheck::uiThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::objectArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::valueArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::uiArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::pixmapArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::generalArgumentThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::objectConstructorThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::valueConstructorThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::generalConstructorThreadCheck = &UIInitialCheck::trivial;
        UIInitialCheck::uiConstructorCheck = &UIInitialCheck::trivial;
        UIInitialCheck::pixmapConstructorCheck = &UIInitialCheck::trivial;
        UIInitialCheck::widgetConstructorCheck = &UIInitialCheck::trivial;
        UIInitialCheck::windowConstructorCheck = &UIInitialCheck::trivial;
    }
}



bool qtjambi_event_notify(QObject *receiver, QEvent *event, bool* result)
{
    switch (event->type()) {
    case QEvent::ChildAdded:
    case QEvent::ChildRemoved:
        {
            QChildEvent *e = static_cast<QChildEvent *>(event);
            // We're not interested in objects that don't have a link as the GC won't
            // be interferring with them anyway.
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(e->child())) {
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
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(receiver)) {
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
    if (!QCoreApplicationPrivate::threadRequiresCoreApplication()){
        QObjectPrivate *d = QObjectPrivate::get(receiver);
        QThreadData *threadData = d->threadData;
        QScopedScopeLevelCounter scopeLevelCounter(threadData);
        if (receiver == nullptr) {                        // serious error
            qWarning("QCoreApplication::notify: Unexpected null receiver");
            return true;
        }
#ifndef QT_NO_DEBUG
        QCoreApplicationPrivate::checkReceiverThread(receiver);
#endif
        QtJambiExceptionBlocker __qt_exceptionHandler;
        *result = receiver->isWidgetType() ? false : QCoreApplicationPrivate::notify_helper(receiver, event);
        __qt_exceptionHandler.release(nullptr);
        return true;
    }else{
        return UIInitialCheck::eventNotify(receiver, event, result);
    }
}

bool qtjambi_simple_event_notify(void **data)
{
    if(QObject *receiver = reinterpret_cast<QObject *>(data[0])){
        QEvent *event = reinterpret_cast<QEvent *>(data[1]);
        bool* result = reinterpret_cast<bool*>(data[2]);
        return qtjambi_event_notify(receiver, event, result);
    }
    return false;
}

bool qtjambi_thread_affine_event_notify(void **data)
{
    if(QObject *receiver = reinterpret_cast<QObject *>(data[0])){
        QEvent *event = reinterpret_cast<QEvent *>(data[1]);
        {
            if(const QObjectPrivate* p = QObjectPrivate::get(receiver)){
                QThreadData *thr = p->threadData;
                QScopedScopeLevelCounter scopeLevelCounter(thr);
                QThreadData *currentThread = QThreadData::get2(QThread::currentThread());
                QScopedScopeLevelCounter cscopeLevelCounter(currentThread);
                if(thr && thr->threadId!=nullptr && currentThread != thr){
                    QMetaEnum enm = QMetaEnum::fromType<QEvent::Type>();
                    QString eventDescr;
                    if(enm.isValid()){
                        eventDescr = QLatin1String(enm.valueToKeys(event->type()));
                    }else{
                        eventDescr = QString::number(int(event->type()));
                    }
                    if(QCoreApplicationPrivate::is_app_closing)
                        return false;
                    if (JNIEnv *env = qtjambi_current_environment()){
                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                        QtJambiExceptionHandler __exceptionHandler;
                        try{
                            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(receiver)) {
                                JavaException::raiseQThreadAffinityException(env, qPrintable(QString::asprintf("Cannot send events to objects owned by a different thread (event type: %ls). "
                                                                                                               "Current thread 0x%p. Receiver '%ls' (of type '%s') was created in thread 0x%p",
                                                                                                               qUtf16Printable(eventDescr),
                                                                                                               currentThread, qUtf16Printable(receiver->objectName()),
                                                                                                               receiver->metaObject()->className(), thr)) QTJAMBI_STACKTRACEINFO ,
                                                                             link->getJavaObjectLocalRef(env),
                                                                             nullptr, nullptr);
                            }else{
                                JavaException::raiseQThreadAffinityException(env, qPrintable(QString::asprintf("Cannot send events to objects owned by a different thread (event type: %ls). "
                                                  "Current thread 0x%p. Receiver '%ls' (of type '%s') was created in thread 0x%p",
                                                  qUtf16Printable(eventDescr),
                                                  currentThread, qUtf16Printable(receiver->objectName()),
                                                  receiver->metaObject()->className(), thr)) QTJAMBI_STACKTRACEINFO ,
                                                  nullptr, nullptr, nullptr);
                            }
                        }catch(const JavaException& exn){
                            __exceptionHandler.handle(nullptr, exn, "QCoreApplication::sendEvent");
                        }
                        return true;
                    }
                }
            }
        }
        bool* result = reinterpret_cast<bool*>(data[2]);
        return qtjambi_event_notify(receiver, event, result);
    }
    return false;
}

#define QTJAMBI_TO_COLLECTION1(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {\
    if(collection && expectedElementMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env,collection)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, collection)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess()))\
                    return containerAccess->elementMetaType()==expectedElementMetaType;\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, collection).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(collection, Java::Runtime::Collection::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(qtjambi_object_class_name(env, collection).replace("$", "."), qtjambi_class_name(env, Java::Runtime::Collection::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define QTJAMBI_TO_COLLECTION2(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject collection, const std::type_info&, const QMetaType& expectedElementMetaType) {\
    return qtjambi_is_Q##NAME(env, collection, expectedElementMetaType);\
}

#define QTJAMBI_TO_MAP1(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess()))\
                    return containerAccess->keyMetaType()==expectedKeyMetaType && containerAccess->valueMetaType()==expectedValueMetaType;\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, mapObject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(qtjambi_object_class_name(env, mapObject).replace("$", "."), qtjambi_class_name(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define QTJAMBI_TO_MAP2(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject mapObject, const std::type_info&, const QMetaType& expectedKeyMetaType, const std::type_info&, const QMetaType& expectedValueMetaType) {\
    return qtjambi_is_Q##NAME(env, mapObject, expectedKeyMetaType, expectedValueMetaType);\
}

#define QTJAMBI_TO_MULTIMAP1(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType) {\
    if(mapObject && expectedKeyMetaType.isValid() && expectedValueMetaType.isValid()){\
        if(Java::QtCore::Q##NAME::isInstanceOf(env, mapObject)){\
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, mapObject)){\
                if(Abstract##NAME##Access* containerAccess = dynamic_cast<Abstract##NAME##Access*>(link->containerAccess()))\
                    return containerAccess->keyMetaType()==expectedKeyMetaType && containerAccess->valueMetaType()==expectedValueMetaType;\
            }else\
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, mapObject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
        }else if(!env->IsInstanceOf(mapObject, Java::Runtime::Map::getClass(env)))\
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong argument given: %1, expected: %2").arg(qtjambi_object_class_name(env, mapObject).replace("$", "."), qtjambi_class_name(env, Java::Runtime::Map::getClass(env)).replace("$", ".")) QTJAMBI_STACKTRACEINFO );\
    }\
    return false;\
}

#define QTJAMBI_TO_MULTIMAP2(NAME) \
bool qtjambi_is_Q##NAME(JNIEnv *env, jobject mapObject, const std::type_info&, const QMetaType& expectedKeyMetaType, const std::type_info&, const QMetaType& expectedValueMetaType) {\
    return qtjambi_is_Q##NAME(env, mapObject, expectedKeyMetaType, expectedValueMetaType);\
}

bool qtjambi_is_QQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {
    return qtjambi_is_QList(env, collection, expectedElementMetaType);
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_TO_COLLECTION1(Vector)
QTJAMBI_TO_COLLECTION1(LinkedList)
QTJAMBI_TO_COLLECTION2(Vector)
QTJAMBI_TO_COLLECTION2(LinkedList)
bool qtjambi_is_QStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {
    return qtjambi_is_QVector(env, collection, expectedElementMetaType);
}
#else
bool qtjambi_is_QStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType) {
    return qtjambi_is_QList(env, collection, expectedElementMetaType);
}
#endif // QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_TO_COLLECTION1(List)
QTJAMBI_TO_COLLECTION1(Set)
QTJAMBI_TO_MULTIMAP1(MultiMap)
QTJAMBI_TO_MULTIMAP1(MultiHash)
QTJAMBI_TO_MAP1(Map)
QTJAMBI_TO_MAP1(Hash)
QTJAMBI_TO_COLLECTION2(List)
QTJAMBI_TO_COLLECTION2(Set)
QTJAMBI_TO_COLLECTION2(Queue)
QTJAMBI_TO_COLLECTION2(Stack)
QTJAMBI_TO_MULTIMAP2(MultiMap)
QTJAMBI_TO_MULTIMAP2(MultiHash)
QTJAMBI_TO_MAP2(Map)
QTJAMBI_TO_MAP2(Hash)

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
                msg = msg.arg(QLatin1String(qtjambi_type_name(typeId)));
            }
        }
        JavaException::raiseQNoNativeResourcesException(env, qPrintable(msg) QTJAMBI_STACKTRACEINFO );
    }
}

DoFinally::DoFinally(std::function<void()> _action) : action(_action)
{

}

DoFinally::~DoFinally(){
    QtJambiExceptionInhibitor __exnHandler;
    try{
        if(action)
            action();
    }catch(const JavaException& exn){
        if(JNIEnv* env = qtjambi_current_environment(false)){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            __exnHandler.handle(env, exn, nullptr);
        }
        else qWarning("%s", exn.what());
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

InvalidateAfterUse::InvalidateAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership)
    : m_env(env),
      m_object(m_env->NewGlobalRef(object)),
      m_checkJavaOwnership(checkJavaOwnership)
{}

InvalidateAfterUse::~InvalidateAfterUse()
{
    QtJambiExceptionInhibitor __exnHandler;
    try{
        qtjambi_invalidate_object(m_env, m_object, m_checkJavaOwnership);
    }catch(const JavaException& exn){
        m_env->DeleteGlobalRef(m_object);
        m_object = nullptr;
        __exnHandler.handle(m_env, exn, nullptr);
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
    if(m_object)
        m_env->DeleteGlobalRef(m_object);
}

InvalidateContainerAfterUse::InvalidateContainerAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership)
    : m_env(env),
      m_object(m_env->NewGlobalRef(object)),
      m_checkJavaOwnership(checkJavaOwnership)
{}

InvalidateContainerAfterUse::~InvalidateContainerAfterUse()
{
    QtJambiExceptionInhibitor __exnHandler;
    try{
        qtjambi_invalidate_collection(m_env, m_object, m_checkJavaOwnership);
    }catch(const JavaException& exn){
        m_env->DeleteGlobalRef(m_object);
        m_object = nullptr;
        __exnHandler.handle(m_env, exn, nullptr);
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
    if(m_object)
        m_env->DeleteGlobalRef(m_object);
}

InvalidateArrayAfterUse::InvalidateArrayAfterUse(JNIEnv *env, jobjectArray object, bool checkJavaOwnership)
    : m_env(env),
      m_object(object),
      m_checkJavaOwnership(checkJavaOwnership)
{}

InvalidateArrayAfterUse::~InvalidateArrayAfterUse()
{
    QtJambiExceptionInhibitor __exnHandler;
    try{
        qtjambi_invalidate_array(m_env, m_object, m_checkJavaOwnership);
    }catch(const JavaException& exn){
        __exnHandler.handle(m_env, exn, nullptr);
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

AboutToPaint::AboutToPaint(JNIEnv *env, jobject object)
    : m_env(env),
      m_object(object)
{
    Java::QtGui::QPainter::beginPaint(env, object);
}

AboutToPaint::~AboutToPaint()
{
    QtJambiExceptionInhibitor __exnHandler;
    try{
        Java::QtGui::QPainter::endPaint(m_env, m_object);
    }catch(const JavaException& exn){
        __exnHandler.handle(m_env, exn, nullptr);
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

struct ExceptionHandler{
    ExceptionHandler() : exn(), methodName(nullptr), blocking(false), reraise(false) {}
    ~ExceptionHandler(){
        if(exn){
            if(JNIEnv *env = qtjambi_current_environment(false)){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                try{
                    jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
                    Java::QtJambi::QtJambiInternal::reportException(env, msg, exn.object());
                }catch(const JavaException& _exn){
                    if(methodName){
                        printf("An exception occured in %s: %s\n", methodName, exn.what());
                    }else{
                        printf("An exception occured: %s\n", exn.what());
                    }
                    printf("An exception occured in QtJambiInternal.reportException(...): %s\n", _exn.what());
                }
            }else if(methodName){
                qWarning("An exception occured in %s: %s", methodName, exn.what());
            }else{
                qWarning("An exception occured: %s", exn.what());
            }
        }
    }
    JavaException exn;
    const char* methodName;
    uint blocking : 1;
    uint reraise : 1;
    static QThreadStorage<ExceptionHandler> storage;
};

QThreadStorage<ExceptionHandler> ExceptionHandler::storage;

QtJambiExceptionHandler::QtJambiExceptionHandler()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    data = exceptionHandler.blocking;
    exceptionHandler.blocking = false;
}

QtJambiExceptionHandler::~QtJambiExceptionHandler(){
    ExceptionHandler::storage.localData().blocking = data;
}

void QtJambiExceptionHandler::handle(JNIEnv *env, const JavaException& exn, const char* methodName){
    if(data){
        ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
        if(exceptionHandler.exn){
            if(!env){
                env = qtjambi_current_environment();
                if(!env){
                    if(methodName){
                        qWarning("An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qWarning("An exception occured: %s", exn.what());
                    }
                    return;
                }else{
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    exceptionHandler.exn.addSuppressed(env, exn);
                }
            }else{
                exceptionHandler.exn.addSuppressed(env, exn);
            }
        }else{
            exceptionHandler.exn = exn;
            exceptionHandler.methodName = methodName;
        }
    }else if(exn){
        throw JavaException(exn);//JavaException(env, exn.object());
    }
}

QtJambiExceptionInhibitor::QtJambiExceptionInhibitor()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    data = (exceptionHandler.blocking ? 0x01 : 0x0) | (exceptionHandler.reraise ? 0x10 : 0x0);
    exceptionHandler.blocking = false;
}

QtJambiExceptionInhibitor::~QtJambiExceptionInhibitor(){
    ExceptionHandler::storage.localData().blocking = (data & 0x01);
}

void QtJambiExceptionInhibitor::handle(JNIEnv *env, const JavaException& exn, const char* methodName){
    if(!(data & 0x01) || (data & 0x10)){
        ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
        if(exceptionHandler.exn){
            if(!env){
                env = qtjambi_current_environment();
                if(!env){
                    if(methodName){
                        qWarning("An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qWarning("An exception occured: %s", exn.what());
                    }
                    return;
                }else{
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    exceptionHandler.exn.addSuppressed(env, exn);
                }
            }else{
                exceptionHandler.exn.addSuppressed(env, exn);
            }
        }else{
            exceptionHandler.exn = exn;
            exceptionHandler.methodName = methodName;
        }
    }else{
        if(!env){
            env = qtjambi_current_environment();
            if(!env){
                if(methodName){
                    qWarning("An exception occured in %s: %s", methodName, exn.what());
                }else{
                    qWarning("An exception occured: %s", exn.what());
                }
                return;
            }
        }
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        try{
            jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
            Java::QtJambi::QtJambiInternal::reportException(env, msg, exn.object());
        }catch(const JavaException& _exn){
            if(methodName){
                printf("An exception occured in %s: %s\n", methodName, exn.what());
            }else{
                printf("An exception occured: %s\n", exn.what());
            }
            printf("An exception occured in QtJambiInternal.reportException(...): %s\n", _exn.what());
        }
    }
}

QtJambiExceptionBlocker::QtJambiExceptionBlocker()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    data = exceptionHandler.blocking;
    exceptionHandler.blocking = true;
}

QtJambiExceptionBlocker::~QtJambiExceptionBlocker(){
    ExceptionHandler::storage.localData().blocking = data;
}

void QtJambiExceptionBlocker::release(JNIEnv *env){
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    if(exceptionHandler.exn){
        JavaException exn = exceptionHandler.exn;
        exceptionHandler.exn = JavaException();
        if(exceptionHandler.reraise){
            exceptionHandler.methodName = nullptr;
            if(exn)
                throw exn;
        }else{
            const char* methodName = exceptionHandler.methodName;
            exceptionHandler.methodName = nullptr;
            if(!env){
                env = qtjambi_current_environment();
                if(!env){
                    if(methodName){
                        qWarning("An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qWarning("An exception occured: %s", exn.what());
                    }
                    return;
                }
            }
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            try{
                jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
                Java::QtJambi::QtJambiInternal::reportException(env, msg, exn.object());
            }catch(const JavaException& _exn){
                if(methodName){
                    printf("An exception occured in %s: %s\n", methodName, exn.what());
                }else{
                    printf("An exception occured: %s\n", exn.what());
                }
                printf("An exception occured in QtJambiInternal.reportException(...): %s\n", _exn.what());
            }
        }
    }
}

QtJambiExceptionRaiser::QtJambiExceptionRaiser()
    : data(0) {
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    data = exceptionHandler.reraise;
    exceptionHandler.reraise = true;
}

QtJambiExceptionRaiser::~QtJambiExceptionRaiser(){
    ExceptionHandler::storage.localData().reraise = data;
}

void QtJambiExceptionRaiser::raise(JNIEnv *){
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    if(exceptionHandler.exn){
        JavaException exn = exceptionHandler.exn;
        exceptionHandler.exn = JavaException();
        if(exn)
            throw exn;
    }
}

ApplicationData::ApplicationData(JNIEnv *env, jobjectArray array)
    : QtJambiObjectData(), m_size(int(env->GetArrayLength(array))), m_chars(m_size>0 ? new char*[size_t(m_size)] : nullptr)
{
    for(jsize i=0; i<m_size; ++i){
        jstring strg = jstring(env->GetObjectArrayElement(array, i));
        if(strg){
            jsize length = env->GetStringUTFLength(strg);
            if(length>=0){
                m_chars[i] = new char[size_t(length+1)];
                m_chars[i][length] = 0;
                env->GetStringUTFRegion(strg, 0, length, m_chars[i]);
            }else{
                m_chars[i] = nullptr;
            }
        }else{
            m_chars[i] = new char[1];
            m_chars[i][0] = 0;
        }
    }
}

ApplicationData::~ApplicationData(){
    for(jsize i=0; i<m_size; ++i){
        delete[] m_chars[i];
    }
    delete[] m_chars;
}

char** ApplicationData::chars(){
    return m_chars;
}

int& ApplicationData::size(){
    return m_size;
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,ApplicationData::)

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
                if(JNIEnv* env = qtjambi_current_environment()){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    link->setCppOwnership(env);
                }
            }
        }
    }
    ~FunctorBasePrivate()
    {
        if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
            if(void* ptr = link->typedPointer(m_typeId)){
                FunctionalBase* functional = reinterpret_cast<FunctionalBase*>(ptr);
                if(--functional->m_ref==0){
                    if(JNIEnv* env = qtjambi_current_environment(false)){
                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                        link->setJavaOwnership(env);
                    }
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

JniLocalFrame::JniLocalFrame(JNIEnv *env, int capacity) : m_env(env){
    m_env->PushLocalFrame(capacity);
}

JniLocalFrame::~JniLocalFrame(){
    m_env->PopLocalFrame(nullptr);
}

class QtJambiScopePrivate : public QVector<std::function<void()>>
{
public:
    QtJambiScopePrivate();
    virtual ~QtJambiScopePrivate();
    virtual QtJambiLink * link() const;
};

QtJambiScopePrivate::QtJambiScopePrivate() : QVector<std::function<void()>>() {}
QtJambiScopePrivate::~QtJambiScopePrivate(){}
QtJambiLink *QtJambiScopePrivate::link() const { return nullptr; }

class QtJambiScopePrivateLink : public QtJambiScopePrivate
{
public:
    QtJambiScopePrivateLink(const QSharedPointer<QtJambiLink>& _link);
    QtJambiLink * link() const override;
private:
    QSharedPointer<QtJambiLink> m_link;
};

QtJambiScopePrivateLink::QtJambiScopePrivateLink(const QSharedPointer<QtJambiLink>& _link) : QtJambiScopePrivate(), m_link(_link) {}
QtJambiLink *QtJambiScopePrivateLink::link() const { return m_link.data(); }

class QtJambiScopePrivateObject : public QtJambiScopePrivate
{
public:
    QtJambiScopePrivateObject(JNIEnv* env, jobject object);
    QtJambiLink * link() const override;
private:
    JNIEnv* m_env;
    jobject m_object;
};

QtJambiScopePrivateObject::QtJambiScopePrivateObject(JNIEnv* env, jobject object) : QtJambiScopePrivate(), m_env(env), m_object(object) {}
QtJambiLink *QtJambiScopePrivateObject::link() const {
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(m_env, m_object))
        return link.data();
    else return nullptr;
}

QtJambiLinkScope::QtJambiLinkScope(const QSharedPointer<QtJambiLink>& _link)
 : QtJambiScope(*new QtJambiScopePrivateLink(_link)){

}

QtJambiLink * QtJambiLinkScope::link() const{
    return reinterpret_cast<QtJambiLink *>(relatedNativeID());
}

QtJambiScope::QtJambiScope() : d(nullptr) {}

QtJambiScope::QtJambiScope(QtJambiNativeID nativeId)
    : d(!nativeId ? nullptr : new QtJambiScopePrivateLink(QtJambiLink::fromNativeId(nativeId)))
{
}

QtJambiScope::QtJambiScope(JNIEnv *env, jobject object)
    : d(object ? new QtJambiScopePrivateObject(env, object) : nullptr)
{
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
                if(JNIEnv* env = qtjambi_current_environment(false)){
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    exn.report(env);
                }
                else qWarning("%s", exn.what());
            } catch (const std::exception& e) {
                qWarning("%s", e.what());
            } catch (...) {
            }
        }
        delete d;
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void QtJambiScope::addDeletion(int metaTypeId, void* pointer){
    addFinalAction([metaTypeId,pointer](){QMetaType::destroy(metaTypeId, pointer);});
}
#else
void QtJambiScope::addDeletion(QMetaType metaType, void* pointer){
    addFinalAction([metaType,pointer](){metaType.destroy(pointer);});
}
#endif

void QtJambiScope::addObjectInvalidation(JNIEnv *env, jobject object, bool checkJavaOwnership, bool persistent){
    if(persistent){
        JObjectWrapper obj(env, object);
        addFinalAction([obj, checkJavaOwnership](){
            if(JNIEnv* env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                qtjambi_invalidate_object(env, obj.object(), checkJavaOwnership);
            }
        });
    }else{
        addFinalAction([env, object, checkJavaOwnership](){
            qtjambi_invalidate_object(env, object, checkJavaOwnership);
        });
    }
}

void QtJambiScope::addFinalAction(std::function<void()>&& action){
    if(!d)
        d = new QtJambiScopePrivate();
    d->append(std::move(action));
}

QtJambiNativeID QtJambiScope::relatedNativeID() const{
    return d ? QtJambiNativeID(jlong(d->link())) : InvalidNativeID;
}

bool qtjambi_is_shell(QtJambiNativeID nativeId)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->isShell() : false;
}

bool qtjambi_object_is_shell(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object))
        return link->isShell();
    return false;
}

bool qtjambi_interface_is_shell(JNIEnv *env, jobject object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object))
        return link->isShell();
    return false;
}

QtJambiNativeID qtjambi_get_object_native_id(JNIEnv *env, jobject object){
    /*if(object){
        if(jobject nativeLink = Java::QtJambi::QtJambiObject::nativeLink(env, object)){
            JObjectSynchronizer sync(env, nativeLink);
            return QtJambiNativeID(Java::QtJambi::QtJambiInternal$NativeLink::native__id(env, nativeLink));
        }
    }*/
    return QtJambiNativeID(Java::QtJambi::QtJambiInternal::nativeId(env, object));
}

QtJambiNativeID qtjambi_get_interface_native_id(JNIEnv *env, jobject object){
    /*if(object){
        if(jobject nativeLink = Java::QtJambi::QtJambiInternal::findInterfaceLink(env, object, true, true)){
            JObjectSynchronizer sync(env, nativeLink);
            return QtJambiNativeID(Java::QtJambi::QtJambiInternal$NativeLink::native__id(env, nativeLink));
        }
    }
    return QtJambiNativeID::Invalid;
    */
    return QtJambiNativeID(Java::QtJambi::QtJambiInternal::nativeIdInterface(env, object));
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

uint qtjambi_java_object_identity(JNIEnv *env, jobject object){
    return uint(Java::Runtime::System::identityHashCode(env, object));
}

uint qtjambi_java_object_hashcode(JNIEnv *env, jobject object){
    return uint(Java::Runtime::Object::hashCode(env, object));
}

jobject qtjambi_make_optional(JNIEnv *env, bool hasValue, jobject object){
    if(hasValue){
        return Java::Runtime::Optional::ofNullable(env, object);
    }else{
        return Java::Runtime::Optional::empty(env);
    }
}

jobject qtjambi_make_optionalint(JNIEnv *env, bool hasValue, jint value){
    if(hasValue){
        return Java::Runtime::OptionalInt::of(env, value);
    }else{
        return Java::Runtime::OptionalInt::empty(env);
    }
}

jobject qtjambi_make_optionallong(JNIEnv *env, bool hasValue, jlong value){
    if(hasValue){
        return Java::Runtime::OptionalLong::of(env, value);
    }else{
        return Java::Runtime::OptionalLong::empty(env);
    }
}

jobject qtjambi_make_optionaldouble(JNIEnv *env, bool hasValue, jdouble value){
    if(hasValue){
        return Java::Runtime::OptionalDouble::of(env, value);
    }else{
        return Java::Runtime::OptionalDouble::empty(env);
    }
}

jobject qtjambi_read_optional(JNIEnv *env, jobject object, bool& isPresent){
    if((isPresent = object && Java::Runtime::Optional::isPresent(env, object))){
        Java::Runtime::Optional::get(env, object);
    }
    return nullptr;
}

jint qtjambi_read_optionalint(JNIEnv *env, jobject object, bool& isPresent){
    if((isPresent = object && Java::Runtime::OptionalInt::isPresent(env, object))){
        Java::Runtime::OptionalInt::getAsInt(env, object);
    }
    return 0;
}

jlong qtjambi_read_optionallong(JNIEnv *env, jobject object, bool& isPresent){
    if((isPresent = object && Java::Runtime::OptionalLong::isPresent(env, object))){
        Java::Runtime::OptionalLong::getAsLong(env, object);
    }
    return 0;
}

jdouble qtjambi_read_optionaldouble(JNIEnv *env, jobject object, bool& isPresent){
    if((isPresent = object && Java::Runtime::OptionalDouble::isPresent(env, object))){
        Java::Runtime::OptionalDouble::getAsDouble(env, object);
    }
    return 0;
}

jobject qtjambi_from_destroyed_qobject(JNIEnv *env, QObject* object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(object)){
        return link->getJavaObjectLocalRef(env);
    }
    return nullptr;
}

jobject qtjambi_method_from_javamethod(JNIEnv * env, jlong metaObjectPointer, jobjectArray ok)
{
    if(metaObjectPointer){
        env->EnsureLocalCapacity(100);
        jobject reflect_method = env->GetObjectArrayElement(ok, 0);
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        int index = QtJambiMetaObject::methodFromJMethod(metaObject, env->FromReflectedMethod(reflect_method));
        QMetaMethod method = metaObject->method(index);
        if(method.isValid()){
            return qtjambi_cast<jobject>(env, method);
        }else{
            env->SetObjectArrayElement(ok, 0, nullptr);
            return nullptr;
        }
    }
    return nullptr;
}

jobject qtjambi_metamethod_invoke(JNIEnv * env, jobject _metaMethod, jobject _qobject, jobjectArray argClassTypeArray, jint connection, jobjectArray args)
{
    env->EnsureLocalCapacity(500);
    jvalue result;
    result.l = nullptr;
    QObject* object = qtjambi_to_qobject(env, _qobject);
    if(object){
        QMetaMethod* method = qtjambi_to_object<QMetaMethod>(env, _metaMethod);
        if(method && method->isValid()){
            QtJambiScope scope(env, _qobject);
            bool ok = false;
            const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *method);
            int argsCount = env->GetArrayLength(args);
            const int parameterCount = method->parameterCount();
            if(parameterCount==env->GetArrayLength(argClassTypeArray)-1
                    && parameterCount==argsCount && argsCount<10){
                QGenericArgument val[10];
                QList<QByteArray> parameterTypes = method->parameterTypes();
                ok = true;
                for(int i=0; i<argsCount; ++i){
                    jvalue jval;
                    jval.l = env->GetObjectArrayElement(args, i);
                    void* qtPtr = nullptr;
                    const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                    ok = parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l);
                    if(ok)
                        val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
                    else
                        break;
                }
                if(ok){
                    void* resultPtr = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(method->returnType()!=QMetaType::Void){
                        resultPtr = QMetaType::create(method->returnType());
                        scope.addDeletion(method->returnType(), resultPtr);
                    }
#else
                    if(method->returnMetaType().id()!=QMetaType::Void){
                        resultPtr = method->returnMetaType().create();
                        scope.addDeletion(method->returnMetaType(), resultPtr);
                    }
#endif
                    ok = method->invoke(object,
                                        Qt::ConnectionType(connection),
                                        QGenericReturnArgument(method->typeName(), resultPtr),
                                        val[0],
                                        val[1],
                                        val[2],
                                        val[3],
                                        val[4],
                                        val[5],
                                        val[6],
                                        val[7],
                                        val[8],
                                        val[9]);
                    if(ok && resultPtr){
                        ok = parameterTypeInfos[0].convertInternalToExternal(env, nullptr, resultPtr, &result, true);
                    }
                }
            }
            if(!ok){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String(method->methodSignature()) QTJAMBI_STACKTRACEINFO );
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Invalid method") QTJAMBI_STACKTRACEINFO );
        }
    }else if(_qobject){
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, _qobject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }else{
        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("QObject must not be null.") QTJAMBI_STACKTRACEINFO );
    }
    return result.l;
}

jobject qtjambi_metamethod_parameter_class_types(JNIEnv * env, jobject _this){
    const QMetaMethod *__qt_this = qtjambi_to_object<QMetaMethod>(env, _this);
    jobject result = qtjambi_arraylist_new(env, __qt_this->parameterCount());
    if(__qt_this && __qt_this->isValid()){
        const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *__qt_this);
        for(int i=1; i<parameterTypeInfos.size(); ++i){
            qtjambi_collection_add(env, result, parameterTypeInfos[i].javaClass());
        }
    }
    return result;
}

jobject qtjambi_metamethod_return_class_type(JNIEnv * env, jobject _this){
    const QMetaMethod *__qt_this = qtjambi_to_object<QMetaMethod>(env, _this);
    if(__qt_this && __qt_this->isValid()){
        if(__qt_this->methodType()==QMetaMethod::Constructor)
            return nullptr;
        const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *__qt_this);
        if(parameterTypeInfos.size()>0){
            return parameterTypeInfos[0].javaClass();
        }
    }
    return nullptr;
}

jobject qtjambi_metamethod_to_reflected(JNIEnv * env, jobject _this){
    const QMetaMethod *__qt_this = qtjambi_to_object<QMetaMethod>(env, _this);
    if(__qt_this && __qt_this->isValid()){
        return QtJambiMetaObject::toReflected(env, *__qt_this);
    }
    return nullptr;
}


jobject qtjambi_metamethod_invoke_on_gadget
    (JNIEnv * env, jobject _metaMethod, jobject object, jobjectArray argClassTypeArray, jobjectArray args)
{
    env->EnsureLocalCapacity(500);
    jvalue result;
    result.l = nullptr;
    JObjectWrapper wrapper;
    QMetaMethod* method = qtjambi_to_object<QMetaMethod>(env, _metaMethod);
    if(method && method->isValid()){
        QtJambiScope scope;
        void* ptr = nullptr;
        if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object)){
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
                const std::type_info* typeId = nullptr;
                const QMetaObject *metaObject = method->enclosingMetaObject();
                do{
                    typeId = getTypeByQtName(metaObject->className());
                    if(typeId)
                        break;
                    metaObject = metaObject->superClass();
                }while(metaObject);
                if(typeId){
                    ptr = link->typedPointer(*typeId);
                }else{
                    typeId = &typeid(JObjectWrapper);
                    ptr = link->pointer();
                }
                qtjambi_check_resource(env, ptr, *typeId);
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }else{
            wrapper = JObjectWrapper(env, object);
            ptr = &wrapper;
        }
        if(ptr){
            bool ok = false;
            const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *method);
            int argsCount = env->GetArrayLength(args);
            const int parameterCount = method->parameterCount();
            if(parameterCount==env->GetArrayLength(argClassTypeArray)-1
                    && parameterCount==argsCount && argsCount<10){
                QGenericArgument val[10];
                QList<QByteArray> parameterTypes = method->parameterTypes();
                ok = true;
                for(int i=0; i<argsCount; ++i){
                    jvalue jval;
                    jval.l = env->GetObjectArrayElement(args, i);
                    void* qtPtr = nullptr;
                    const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                    ok = parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l);
                    if(ok)
                        val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
                    else
                        break;
                }
                if(ok){
                    void* resultPtr = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(method->returnType()!=QMetaType::Void){
                        resultPtr = QMetaType::create(method->returnType());
                        scope.addDeletion(method->returnType(), resultPtr);
                    }
#else
                    if(method->returnMetaType().id()!=QMetaType::Void){
                        resultPtr = method->returnMetaType().create();
                        scope.addDeletion(method->returnMetaType(), resultPtr);
                    }
#endif
                    ok = method->invokeOnGadget(ptr,
                                        QGenericReturnArgument(method->typeName(), resultPtr),
                                        val[0],
                                        val[1],
                                        val[2],
                                        val[3],
                                        val[4],
                                        val[5],
                                        val[6],
                                        val[7],
                                        val[8],
                                        val[9]);
                    if(ok && resultPtr){
                        ok = parameterTypeInfos[0].convertInternalToExternal(env, nullptr, resultPtr, &result, true);
                    }
                }
            }
            if(!ok){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String(method->methodSignature()) QTJAMBI_STACKTRACEINFO );
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Invalid method") QTJAMBI_STACKTRACEINFO );
        }
    }
    return result.l;
}

jclass qtjambi_metaproperty_class(JNIEnv *env, jobject _this){
    try{
        const QMetaProperty *__qt_this = qtjambi_to_object<QMetaProperty>(env, _this);
        if(__qt_this && __qt_this->isValid()){
            if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(__qt_this->enclosingMetaObject())){
                return mo->typeOfProperty(__qt_this->relativePropertyIndex());
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

jboolean qtjambi_metaproperty_reset_on_gadget(JNIEnv *env, jobject _this, jobject gadget){
    try{
        const QMetaProperty *__qt_this = qtjambi_to_object<QMetaProperty>(env, _this);
        if(__qt_this && __qt_this->isValid()){
            void* ptr = nullptr;
            JObjectWrapper wrapper;
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, gadget)){
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, gadget);
                const std::type_info* typeId = nullptr;
                const QMetaObject* metaObject = __qt_this->enclosingMetaObject();
                do{
                    typeId = getTypeByQtName(metaObject->className());
                    if(typeId)
                        break;
                    metaObject = metaObject->superClass();
                }while(metaObject);
                if(!typeId)
                    typeId = &typeid(JObjectWrapper);
                if(link){
                    ptr = link->typedPointer(*typeId);
                }
                qtjambi_check_resource(env, ptr, *typeId);
            }else{
                wrapper = JObjectWrapper(env, gadget);
                ptr = &wrapper;
            }
            return __qt_this->resetOnGadget(ptr);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

jobject qtjambi_metaproperty_read_on_gadget(JNIEnv *env, jobject _this, jobject gadget){
    try{
        QVariant variant;
        const QMetaProperty *__qt_this = qtjambi_to_object<QMetaProperty>(env, _this);
        if(__qt_this && __qt_this->isValid()){
            void* ptr = nullptr;
            JObjectWrapper wrapper;
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, gadget)){
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, gadget);
                const std::type_info* typeId = nullptr;
                const QMetaObject* metaObject = __qt_this->enclosingMetaObject();
                do{
                    typeId = getTypeByQtName(metaObject->className());
                    if(typeId)
                        break;
                    metaObject = metaObject->superClass();
                }while(metaObject);
                if(!typeId)
                    typeId = &typeid(JObjectWrapper);
                if(link){
                    ptr = link->typedPointer(*typeId);
                }
                qtjambi_check_resource(env, ptr, *typeId);
            }else{
                wrapper = JObjectWrapper(env, gadget);
                ptr = &wrapper;
            }
            variant = __qt_this->readOnGadget(ptr);
            if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(__qt_this->enclosingMetaObject())){
                if(InternalToExternalConverter converter = mo->internalToExternalConverterOfProperty(__qt_this->relativePropertyIndex())){
                    jvalue val;
                    val.l = nullptr;
                    QtJambiScope scope(nullptr);
                    if(converter(env, &scope, variant.data(), &val, true))
                        return val.l;
                }
            }

            int type;
            if (__qt_this->type()==QVariant::Invalid) {
                return nullptr;
            }else if(__qt_this->type()==QVariant::UserType){
                type = __qt_this->userType();
            }else{
                type = __qt_this->type();
            }
            QLatin1String qtName(__qt_this->typeName());
            if(const std::type_info* typeId = getTypeByQtName(qtName)){
                const char* fullJavaName = getJavaName(*typeId);
                jclass object_class = resolveClass(env, fullJavaName);
                Q_ASSERT(object_class);
                jvalue val;
                val.l = nullptr;
                qtjambi_throw_java_exception(env);
                QMetaType _type(type);
                InternalToExternalConverter converter = QtJambiTypeManager::getInternalToExternalConverter(env, qtName, _type, object_class);
                QtJambiScope scope(nullptr);
                bool result = converter && converter(env, &scope, variant.data(), &val, true);
                if(result)
                    return val.l;
            }else{
                return qtjambi_cast<jobject>(env, variant);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

jboolean qtjambi_metaproperty_write_on_gadget(JNIEnv *env, jobject _this, jobject gadget, jobject value){
    try{
        const QMetaProperty *__qt_this = qtjambi_to_object<QMetaProperty>(env, _this);
        if(__qt_this && __qt_this->isValid()){
            void* ptr = nullptr;
            JObjectWrapper wrapper;
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, gadget)){
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, gadget);
                const std::type_info* typeId = nullptr;
                const QMetaObject* metaObject = __qt_this->enclosingMetaObject();
                do{
                    typeId = getTypeByQtName(metaObject->className());
                    if(typeId)
                        break;
                    metaObject = metaObject->superClass();
                }while(metaObject);
                if(!typeId)
                    typeId = &typeid(JObjectWrapper);
                if(link){
                    ptr = link->typedPointer(*typeId);
                }
                qtjambi_check_resource(env, ptr, *typeId);
            }else{
                wrapper = JObjectWrapper(env, gadget);
                ptr = &wrapper;
            }
            int type;
            if (__qt_this->type()==QVariant::Invalid) {
                return false;
            }else if(__qt_this->type()==QVariant::UserType){
                type = __qt_this->userType();
            }else{
                type = __qt_this->type();
            }
            QLatin1String qtName(__qt_this->typeName());
            QVariant valueVariant;
            bool success = false;
            if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(__qt_this->enclosingMetaObject())){
                if(ExternalToInternalConverter converter = mo->externalToInternalConverterOfProperty(__qt_this->relativePropertyIndex())){
                    jvalue val;
                    val.l = value;
                    void *copy = nullptr;
                    QtJambiScope scope(nullptr);
                    if(!(success = converter(env, &scope, val, copy, jValueType::l))){
                        qWarning("Cannot convert external type '%s' to '%s'",
                                 qPrintable(QString(qtjambi_object_class_name(env, value)).replace("$", ".")),
                                 qPrintable(qtName));
                        return false;
                    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    valueVariant = QVariant(type, copy);
#else
                    valueVariant = QVariant(QMetaType(type), copy);
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                }
            }
            if(!success){
                if(const std::type_info* typeId = getTypeByQtName(qtName)){
                    const char* fullJavaName = getJavaName(*typeId);
                    jclass object_class = resolveClass(env, fullJavaName);
                    Q_ASSERT(object_class);
                    if(value && !env->IsInstanceOf(value, object_class)){
                        return false;
                    }
                    jvalue val;
                    val.l = value;
                    qtjambi_throw_java_exception(env);
                    QMetaType _type(type);
                    ExternalToInternalConverter converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, qtName, _type);
                    void *copy = nullptr;
                    QtJambiScope scope(nullptr);
                    if(!converter(env, &scope, val, copy, jValueType::l)){
                        qWarning("Cannot convert external type '%s' to '%s'",
                                 qPrintable(QString(QLatin1String(fullJavaName)).replace("/", ".").replace("$", ".")),
                                 qPrintable(qtName));
                        return false;
                    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    valueVariant = QVariant(type, copy);
#else
                    valueVariant = QVariant(QMetaType(type), copy);
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                }else{
                    valueVariant = qtjambi_cast<QVariant>(env, value);
                }
            }
            return __qt_this->writeOnGadget(ptr, valueVariant);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

void pre_exit_function(){
    QtJambiExceptionInhibitor __exnHandler;
    if(JNIEnv *__jni_env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(__jni_env, 400)
        try{
            Java::QtJambi::NativeLibraryManager::resetDeploymentSpecs(__jni_env);
        }catch(const JavaException& exn){
            __exnHandler.handle(__jni_env, exn, "postRoutine");
        }
    }
}

void qtjambi_pre_exit(){
    qAddPostRoutine(&pre_exit_function);
}

void qtjambi_unexit(){
    qRemovePostRoutine(&pre_exit_function);
}

void qtjambi_invalidate_object(JNIEnv *env, QtJambiNativeID nativeId){
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId)) {
        if(!link->isShell() && !link->isQObject()){
            link->invalidate(env);
        }
    }
}

jclass qtjambi_lambda_return_type(JNIEnv *, jobject){
    return nullptr;
}

void qtjambi_register_dependent_interface(JNIEnv *env, jobject dependentObject, jobject owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaInterface(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::findLinkForJavaInterface(env, owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

void qtjambi_register_dependent_interface(JNIEnv *env, jobject dependentObject, QtJambiNativeID nativeId){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaInterface(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(nativeId);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

void qtjambi_register_dependent_object(JNIEnv *env, jobject dependentObject, jobject owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaObject(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::findLinkForJavaInterface(env, owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

void qtjambi_unregister_dependent_interface(JNIEnv *env, jobject dependentObject, jobject owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaInterface(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::findLinkForJavaInterface(env, owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterDependentObject(_dependentLink);
    }
}

void qtjambi_unregister_dependent_object(JNIEnv *env, jobject dependentObject, jobject owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaObject(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::findLinkForJavaInterface(env, owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterDependentObject(_dependentLink);
    }
}

jobject qtjambi_get_internal_access(JNIEnv *env, jclass){
    return Java::QtJambi::QtJambiInternal::internalAccess(env);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QtJambiObjectData::QtJambiObjectData(){}
QtJambiObjectData::~QtJambiObjectData(){}

QtJambiObjectData* QtJambiObjectData::userData(const QObject* object, const std::type_info& id)
{
    const QObjectPrivate* p = object ? QObjectPrivate::get(object) : nullptr;
    if(p && (id!=typeid(ValueOwnerUserData) || !p->wasDeleted) && p->extraData){
        char name[sizeof(void*)+1];
        memcpy(name, &p->extraData, sizeof(void*));
        name[sizeof(void*)] = '\0';
        const auto i = p->extraData->propertyNames.indexOf(name);
        if(i>=0){
            const QVariant& variant = p->extraData->propertyValues.at(i);
            if(variant.metaType()==QMetaType::fromType<QtJambiObjectDataContainer>()){
                const QtJambiObjectDataContainer* container = reinterpret_cast<const QtJambiObjectDataContainer*>(variant.data());
                return container->userData(id);
            }
        }
    }
    return nullptr;
}

void QtJambiObjectData::setUserData(QObject* object, const std::type_info& id, QtJambiObjectData* data)
{
    QObjectPrivate* p = object ? QObjectPrivate::get(object) : nullptr;
    if(p && (id!=typeid(ValueOwnerUserData) || !p->wasDeleted)){
        if (!p->extraData)
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
            p->extraData = new QObjectPrivate::ExtraData;
#else
            p->extraData = new QObjectPrivate::ExtraData(p);
#endif
        char name[sizeof(void*)+1];
        memcpy(name, &p->extraData, sizeof(void*));
        name[sizeof(void*)] = '\0';
        const auto i = p->extraData->propertyNames.indexOf(name);
        if(i>=0 && p->extraData->propertyValues[i].metaType()==QMetaType::fromType<QtJambiObjectDataContainer>()){
            QtJambiObjectDataContainer* container = reinterpret_cast<QtJambiObjectDataContainer*>(p->extraData->propertyValues[i].data());
            container->setUserData(id, data);
        }else{
            QtJambiObjectDataContainer container;
            container.setUserData(id, data);
            if(i<0){
                p->extraData->propertyNames.append(name);
                p->extraData->propertyValues.append(QVariant::fromValue(container));
            }else{
                p->extraData->propertyValues.replace(i, QVariant::fromValue(container));
            }
        }
    }
}

bool QtJambiObjectData::isRejectedUserProperty(const QObject* object, const char * propertyName) {
    const QObjectPrivate* p = object ? QObjectPrivate::get(object) : nullptr;
    if(propertyName && p && p->extraData){
        char name[sizeof(void*)+1];
        memcpy(name, &p->extraData, sizeof(void*));
        name[sizeof(void*)] = '\0';
        return strcmp(name, propertyName)==0;
    }
    return false;
}

class QtJambiObjectDataContainerPrivate : public QSharedData {
public:
    QtJambiObjectDataContainerPrivate();
    ~QtJambiObjectDataContainerPrivate();
private:
    QVector<size_t> indexes;
    QVector<QtJambiObjectData*> data;
    friend QtJambiObjectDataContainer;
};

QtJambiObjectDataContainerPrivate::QtJambiObjectDataContainerPrivate() : indexes(), data() {
    indexes.fill(0, 3);
    data.fill(nullptr, 3);
}

QtJambiObjectDataContainerPrivate::~QtJambiObjectDataContainerPrivate(){
    QVector<QtJambiObjectData*> _data;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        Q_UNUSED(locker)
        _data.resize(data.size(), nullptr);
        data.swap(_data);
    }
    qDeleteAll(_data);
}

QtJambiObjectDataContainer::QtJambiObjectDataContainer()
    : p(new QtJambiObjectDataContainerPrivate)
{}

QtJambiObjectDataContainer::QtJambiObjectDataContainer(const QtJambiObjectDataContainer& other)
    : p(other.p)
{}

QtJambiObjectDataContainer::QtJambiObjectDataContainer(QtJambiObjectDataContainer&& other)
    : p(std::move(other.p))
{}

QtJambiObjectDataContainer::~QtJambiObjectDataContainer(){}

QtJambiObjectDataContainer& QtJambiObjectDataContainer::operator=(const QtJambiObjectDataContainer& other){
    p = other.p;
    return *this;
}

QtJambiObjectDataContainer& QtJambiObjectDataContainer::operator=(QtJambiObjectDataContainer&& other){
    p = std::move(other.p);
    return *this;
}

bool QtJambiObjectDataContainer::operator==(const QtJambiObjectDataContainer& other){
    return p==other.p;
}

void QtJambiObjectDataContainer::swap(QtJambiObjectDataContainer& other){
    p.swap(other.p);
}

void QtJambiObjectDataContainer::setUserData(const std::type_info& id, QtJambiObjectData* data){
    auto idx = p->indexes.indexOf(unique_id(id));
    if(idx<0){
        if(!data)
            return;
        idx = p->indexes.size();
        if(p->indexes.size()<=idx)
            p->indexes.resize(idx+1, 0);
        p->indexes[idx] = unique_id(id);
    }
    if(p->data.size()<=idx){
        if(!data)
            return;
        p->data.resize(idx+1, nullptr);
    }
    p->data[idx] = data;
}

QtJambiObjectData* QtJambiObjectDataContainer::userData(const std::type_info& id) const{
    auto idx = p->indexes.indexOf(unique_id(id));
    return idx>=0 && idx<p->data.size() ? p->data.at(idx) : nullptr;
}

void swap(QtJambiObjectDataContainer& a, QtJambiObjectDataContainer& b){
    a.swap(b);
}
#endif

