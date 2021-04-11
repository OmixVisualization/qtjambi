/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "qtjambi_repository_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambilink_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambivariant_p.h"
#include "qtjambi_thread.h"
#include "qtjambi_thread_p.h"
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qobject_p.h>

#include <qglobal.h>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QCoreApplication>
#include <QtCore/private/qcoreapplication_p.h>
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
#include <QtCore/QVariant>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>
#include <QtCore/QAbstractEventDispatcher>
#if QT_VERSION < 0x050000
#include <QtGui/QStyleOption>
#endif
#include <stdio.h>
#include <string.h>

#if defined(QTJAMBI_DEBUG_TOOLS)
 #include "qtjambidebugevent_p.h"
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
# endif
#include "qtjambi_cast.h"

QT_WARNING_DISABLE_DEPRECATED

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

#ifdef QT_QTJAMBI_PORT

class PublicThread : public QThread{
public:
    inline int exec(){ return QThread::exec(); }

    static void __qt_setTerminationEnabled(bool enabled){
        QThread::setTerminationEnabled(enabled);
    }
};

/*
 * Class:     io_qt_QThread
 * Method:    setTerminationEnabled
 * Signature: (Z)V
 */
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_setTerminationEnabled)
    (JNIEnv *env, jclass, jboolean enabled)
{
    try{
        PublicThread::__qt_setTerminationEnabled(enabled);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

/*
 * Class:     io_qt_QThread
 * Method:    getQThreadObject
 * Signature: (Ljava/lang/Thread;)Lio/qt/core/QThread$Wrapper;
 */
extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_getQThreadObject)
    (JNIEnv *__jni_env, jclass, jobject thread)
{
    try{
        QObject* qthread = qtjambi_to_thread(__jni_env, thread);
        return qtjambi_cast<jobject>(__jni_env, qthread);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

/*
 * Class:     io_qt_QThread
 * Method:    currentQThread
 * Signature: ()Lio/qt/core/QThread$Wrapper;
 */
extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_currentQThread)
    (JNIEnv *__jni_env, jclass)
{
    try{
        return qtjambi_cast<jobject>(__jni_env, QThread::currentThread());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

/*
 * Class:     io_qt_QThread$Wrapper
 * Method:    exec_internal
 * Signature: (J)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper_exec_1internal)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        PublicThread* __qt_this = qtjambi_object_from_nativeId<PublicThread>(__this_nativeId);
        qtjambi_check_resource<QThread>(__jni_env, __qt_this);
        return __qt_this ? jint(__qt_this->exec()) : -1;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return 0;
    }
}

/*
 * Class:     io_qt_QThread$Wrapper
 * Method:    __qt_loopLevel
 * Signature: (J)I
 */
extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper__1_1qt_1loopLevel)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return __qt_this ? jint(__qt_this->loopLevel()) : 0;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return 0;
    }
}

/*
 * Class:     io_qt_QThread$Wrapper
 * Method:    _1_1qt_1exit
 * Signature: (JI)V
 */
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper__1_1qt_1exit)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId, jint retcode)
{
    try{
    QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
    qtjambi_check_resource(__jni_env, __qt_this);
    __qt_this->exit(retcode);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

/*
 * Class:     io_qt_QThread
 * Method:    _1_1qt_1eventDispatcher
 * Signature: (J)Lio/qt/coreQAbstractEventDispatcher;
 */
extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper__1_1qt_1eventDispatcher)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        return qtjambi_cast<jobject>(__jni_env, __qt_this->eventDispatcher());
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

/*
 * Class:     io_qt_QThread
 * Method:    _1_1qt_1requestInterruption
 * Signature: (J)V
 */
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper__1_1qt_1requestInterruption)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        __qt_this->requestInterruption();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

/*
 * Class:     io_qt_QThread
 * Method:    _1_1qt_1terminate
 * Signature: (J)V
 */
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper__1_1qt_1terminate)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        __qt_this->terminate();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

/*
 * Class:     io_qt_QThread
 * Method:    _1_1qt_1started
 * Signature: (J)V
 */
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper__1_1qt_1preStarted)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        AdoptedThread* __qt_this = qtjambi_object_from_nativeId<AdoptedThread>(__this_nativeId);
        qtjambi_check_resource<QThread>(__jni_env, __qt_this);
        __qt_this->onStart();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

/*
 * Class:     io_qt_QThread
 * Method:    _1_1qt_1started
 * Signature: (J)V
 */
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper__1_1qt_1started)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QMetaObject::activate(__qt_this, &QThread::staticMetaObject, 0, Q_NULLPTR);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

/*
 * Class:     io_qt_QThread
 * Method:    _1_1qt_1finished
 * Signature: (J)V
 */
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QThread_00024Wrapper__1_1qt_1finished)
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        QThread* __qt_this = qtjambi_object_from_nativeId<QThread>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QMetaObject::activate(__qt_this, &QThread::staticMetaObject, 1, Q_NULLPTR);
        QSharedPointer<QtJambiLink> lnk = QtJambiLink::fromNativeId(__this_nativeId);
        lnk->blockReconnect();
        QObject::disconnect(__qt_this, &QThread::finished, nullptr, nullptr);
        lnk->unblockReconnect();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

int __signal_method_indexes_QThread0(){ static int result = QMetaMethod::fromSignal(&QThread::started).methodIndex(); return result; }
int __signal_method_indexes_QThread1(){ static int result = QMetaMethod::fromSignal(&QThread::finished).methodIndex(); return result; }

// emitting (writeInterfaceCastFunction)

// emitting (writeSignalFunction)
void __qt_signal_QThread_started(const QWeakPointer<QtJambiLink>& wlink)
{
      QTJAMBI_DEBUG_METHOD_PRINT("shell", "void QThread::started()");
      if(QSharedPointer<QtJambiLink> link = wlink.toStrongRef()){
          if(JNIEnv *__jni_env = qtjambi_current_environment()){
              QTJAMBI_JNI_LOCAL_FRAME(env, 200)
              link->emitSignal(__jni_env, &__signal_method_indexes_QThread0, nullptr);
          }
      }
}

// emitting (writeSignalFunction)
void __qt_signal_QThread_finished(const QWeakPointer<QtJambiLink>& wlink)
{
      QTJAMBI_DEBUG_METHOD_PRINT("shell", "void QThread::finished()");
      if(QSharedPointer<QtJambiLink> link = wlink.toStrongRef()){
          if(!QCoreApplication::closingDown()){
              if(JNIEnv *__jni_env = qtjambi_current_environment()){
                  QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                  link->emitSignal(__jni_env, &__signal_method_indexes_QThread1, nullptr);
              }
          }
      }
}

void __qt_QThread_connect_signals(QWeakPointer<QtJambiLink> wlink, QObject* qobj,
                                  QHash<int,QMetaObject::Connection>* connections)
{
    Q_ASSERT(qobj);
    QThread* object = static_cast<QThread*>(qobj);
    if(!connections->value(__signal_method_indexes_QThread0(), QMetaObject::Connection())){
        connections->insert(__signal_method_indexes_QThread0(), QObject::connect(object, &QThread::started, [wlink](){__qt_signal_QThread_started(wlink);}));
    }
    if(!connections->value(__signal_method_indexes_QThread1(), QMetaObject::Connection())){
        connections->insert(__signal_method_indexes_QThread1(), QObject::connect(object, &QThread::finished, [wlink](){__qt_signal_QThread_finished(wlink);}));
    }
}
#endif

void qtjambi_initialize_current_thread(JNIEnv *env)
{
    QThread* currentQThread = QThread::currentThread();
    if(!QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentQThread)){
        qtjambi_cast<jobject>(env, currentQThread);
    }
}

QThreadUserData::QThreadUserData():
    QtJambiObjectData(),
    m_finalActions(nullptr),
    m_mutex(nullptr)
#ifndef QT_QTJAMBI_PORT
  , m_threadGroup(),
    m_isDaemon(true),
    m_name(),
    m_uncaughtExceptionHandler()
#endif
{
}

#ifndef QT_QTJAMBI_PORT
QThreadUserData::QThreadUserData(JNIEnv *env, jobject threadGroup):
    QtJambiObjectData(),
    m_finalActions(nullptr),
    m_mutex(nullptr),
    m_threadGroup(env, threadGroup),
    m_isDaemon(true),
    m_name(),
    m_uncaughtExceptionHandler()
{
}
#endif

#ifdef QT_QTJAMBI_PORT

QThreadInterrupterUserData::QThreadInterrupterUserData(const QSharedPointer<QtJambiLink>& link) : m_link(link)
{
}

QThreadInterrupterUserData::~QThreadInterrupterUserData()
{
    try{
        if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
            JNIEnv *env = qtjambi_current_environment(false);
            if (!env || link->javaLinkRemoved())
              return;
            QTJAMBI_JNI_LOCAL_FRAME(env, 1000)
            jobject wrapper = link->getJavaObjectLocalRef(env);
            if(wrapper){
                QThread* thread = static_cast<QThread*>(link->qobject());
                if(thread){
                    QTJAMBI_DEBUG_TRACE(qPrintable(QString("deleting THREAD 0x%1").arg(QString::number(qint64(reinterpret_cast<qintptr>(thread)), 16))));
                    jobject jThread = Java::QtCore::QThread$Wrapper::thread(env,wrapper);
                    if(jThread){
                        bool isAlive = Java::Runtime::Thread::isAlive(env, jThread);
                        if(isAlive && !QAbstractEventDispatcher::instance(thread)){
                            Java::Runtime::Thread::interrupt(env, jThread);
                        }
                    }
                }
                link->setJavaOwnership(env, wrapper);
            }
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,QThreadInterrupterUserData::)
#endif

QThreadUserData::~QThreadUserData(){
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,QThreadUserData::)

#ifdef QT_QTJAMBI_PORT
const QSharedPointer<QtJambiLink>&
#else
void
#endif
qtjambi_adopt_thread(JNIEnv *env, jobject java_thread, jobject java_qthread, QThread *qt_thread, bool created_by_java)
{
#ifdef QT_QTJAMBI_PORT
    Q_ASSERT(java_thread);
    // No thread found, need to create a "fake" thread and insert it
    // into the thread table for later mapping between Qt / Java.
    // This thread object is already ref'ed by Qt.
    jobject threadWrapper = Java::QtCore::QThread$Wrapper::newInstance(env, java_thread, nullptr);
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForQObject(env, threadWrapper, qt_thread, created_by_java, false);
    jobject threadName = Java::Runtime::Thread::getName(env,java_thread);
    if(threadName){
        QString qThreadName = qtjambi_to_qstring(env, jstring(threadName));
        bool b = qt_thread->blockSignals(true);
        qt_thread->setObjectName(qThreadName);
        qt_thread->blockSignals(b);
    }

    if(!Java::QtCore::QThread::isInstanceOf(env, java_thread)){
        if(created_by_java)
            link->setJavaOwnership(env, threadWrapper);
        else
            link->setCppOwnership(env, threadWrapper);
        Java::Runtime::resolveQtJambiInternal(env);
        Java::Runtime::resolveQtJambiObject(env);
        Java::QtJambi::QtJambiInternal::registerCleaner(env, Java::QtJambi::QtJambiInternal::getClass(env), java_thread, Java::QtJambi::QtJambiInternal::findInterfaceLink(env, threadWrapper, false, false));
    }
    /* don't use interruptible for adopted threads!
    jobject interruptible = Java::QtCore::QThread::interruptible(env);
    if(interruptible){
        Java::QtJambi::QtJambiInternal::setThreadInterruptible(env, jthread, interruptible);
    }
    */
    Java::QtJambi::QtJambiInternal::createAssociation(env, java_thread, threadWrapper);

    QTJAMBI_SET_OBJECTUSERDATA(QThreadInterrupterUserData, qt_thread, new QThreadInterrupterUserData(link));
    QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, qt_thread, new QThreadUserData());
    env->DeleteLocalRef(threadWrapper);
    return link;
#else
    Q_UNUSED(created_by_java)
    QString threadName = qtjambi_to_qstring(env, Java::Runtime::Thread::getName(env, java_thread));
    qt_thread->setObjectName(threadName);
    jobject javaQThread = java_qthread ? env->NewLocalRef(java_qthread) : nullptr;
    if(!javaQThread){
        javaQThread = qtjambi_cast<jobject>(env, qt_thread);
    }
    if(javaQThread){
        if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, qt_thread)){
            if(threadData->getUncaughtExceptionHandler()){
                Java::Runtime::Thread::setUncaughtExceptionHandler(env, javaQThread, threadData->getUncaughtExceptionHandler());
                threadData->clearUncaughtExceptionHandler(env);
            }
            if(threadData->getContextClassLoader()){
                Java::Runtime::Thread::setContextClassLoader(env, javaQThread, threadData->getContextClassLoader());
                threadData->clearContextClassLoader(env);
            }
            threadData->clearThreadGroup(env);
        }else{
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, qt_thread, new QThreadUserData());
        }
        jobject jthread = Java::QtCore::QThread::javaThread(env, javaQThread);
        if(!jthread){
            Java::QtCore::QThread::set_javaThread(env, javaQThread, java_thread);
            Java::QtJambi::QtJambiInternal::createAssociation(env, java_thread, javaQThread);
            /* don't use interruptible for adopted threads!
            if(jobject interruptible = Java::QtCore::QThread::interruptible(env)){
                JObjectSynchronizer sync(env, javaQThread);
                Q_UNUSED(sync)
                Java::QtJambi::QtJambiInternal::setThreadInterruptible(env, java_thread, interruptible);
                env->DeleteLocalRef(interruptible);
                interruptible = nullptr;
            }
            */
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_thread)){
                if(!qt_thread->parent()){
                    if(QThreadData::get2(qt_thread)->isAdopted || link->qobject()==QCoreApplicationPrivate::mainThread()){
                        link->setCppOwnership(env);
                    }else{
                        link->setJavaOwnership(env);
                    }
                }
                JObjectWrapper jthreadObjectWrapper(env, java_thread);
                if(!EventDispatcherCheck::storage.hasLocalData() || !EventDispatcherCheck::storage.localData()){
                    EventDispatcherCheck::storage.setLocalData(QSharedPointer<EventDispatcherCheck>(
                         new EventDispatcherCheck(
                             qt_thread,
                             [jthreadObjectWrapper](QPointer<QThread>& thread, QList<std::function<void()>>&& finalActions) {
                                  struct ObjectReleaser : public QtJambiObjectData {
                                     ObjectReleaser(const JObjectWrapper& _jthreadObjectWrapper,
                                                    QList<std::function<void()>>&& _finalActions,
                                                    QPointer<QThread>& _thread)
                                         : QtJambiObjectData(),
                                           jthreadObjectWrapper(_jthreadObjectWrapper),
                                           finalActions(std::move(_finalActions)),
                                           m_thread() {
                                         QTJAMBI_DEBUG_METHOD_PRINT("native", "ObjectReleaser::ObjectReleaser()")
                                         m_thread.swap(_thread);
                                     }
                                     ~ObjectReleaser() override {
                                         QTJAMBI_DEBUG_METHOD_PRINT("native", "ObjectReleaser::~ObjectReleaser()")
                                         QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(m_thread.data());
                                         if(JNIEnv* env = qtjambi_current_environment(m_thread.data()==QThread::currentThread())){
                                              QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                              if(!QThreadData::get2(m_thread.data())->isAdopted
                                                      && link
                                                      && link->qobject()
                                                      && !link->qobject()->parent()
                                                      && link->qobject()!=QCoreApplicationPrivate::mainThread()){
                                                  link->setJavaOwnership(env);
                                              }
                                              try{
                                                Java::QtJambi::QtJambiInternal::deleteAssociation(env, jthreadObjectWrapper.object());
                                              }catch(const JavaException& exn){
                                                  exn.report(env);
                                              }
                                              jthreadObjectWrapper.clear(env);
                                         }
                                         while(!finalActions.isEmpty()){
                                             finalActions.takeFirst()();
                                         }
                                         finalActions.clear();
                                         if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread.data())){
                                             QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_thread.data(), nullptr);
                                             delete data;
                                         }
                                     }

                                     QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)

                                     JObjectWrapper jthreadObjectWrapper;
                                     QList<std::function<void()>> finalActions;
                                     QPointer<QThread> m_thread;
                                  };

                                  QAbstractEventDispatcher * eventDispatcher = nullptr;
                                  if(thread && !(eventDispatcher = thread->eventDispatcher())){
                                        if (QThread::currentThread()==thread && (QCoreApplication::instance() || !QCoreApplicationPrivate::threadRequiresCoreApplication())) {
                                            QEventLoop loop;
                                            eventDispatcher = thread->eventDispatcher();
                                        }
                                  }
                                  if(eventDispatcher){
                                       QTJAMBI_SET_OBJECTUSERDATA_ID(QTJAMBI_OBJECTUSERDATA_TYPE_ID(ObjectReleaser), eventDispatcher, new ObjectReleaser(jthreadObjectWrapper, std::move(finalActions), thread));
                                  }else{
                                       ObjectReleaser releaser(jthreadObjectWrapper, std::move(finalActions), thread);
                                       Q_UNUSED(releaser)
                                  }
                              })
                         ));
                }
            }
        }else{
            env->DeleteLocalRef(jthread);
        }
        env->DeleteLocalRef(javaQThread);
        javaQThread = nullptr;
    }
#endif
}

#ifndef QT_QTJAMBI_PORT

QThreadStorage<QSharedPointer<EventDispatcherCheck>> EventDispatcherCheck::storage;

EventDispatcherCheck::EventDispatcherCheck(QThread* thread, std::function<void(QPointer<QThread>&,QList<std::function<void()>>&&)> _cleaner) : m_thread(thread), cleaner(_cleaner) {
    QTJAMBI_DEBUG_METHOD_PRINT("native", "EventDispatcherCheck::EventDispatcherCheck()")
    if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread)){
        data->m_finalActions = &m_finalActions;
        data->m_mutex = &m_mutex;
    }
}

EventDispatcherCheck::~EventDispatcherCheck(){
    QTJAMBI_DEBUG_METHOD_PRINT("native", "EventDispatcherCheck::~EventDispatcherCheck()")
    if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread)){
        data->m_finalActions = nullptr;
        data->m_mutex = nullptr;
    }
    try{
        if(!QCoreApplicationPrivate::is_app_closing && cleaner){
            cleaner(m_thread, std::move(m_finalActions));
            cleaner = {};
        }else{
            while(!m_finalActions.isEmpty()){
                m_finalActions.takeFirst()();
            }
            m_finalActions.clear();
            if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread)){
                QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_thread, nullptr);
                delete data;
            }
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

#endif


#ifdef QT_QTJAMBI_PORT
AdoptedThread::AdoptedThread(JNIEnv *env, jobject thread, bool running)
    : QThread(),
      m_link(qtjambi_adopt_thread(env, thread, nullptr, this, true))
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", QString("AdoptedThread::AdoptedThread() of %1").arg(objectName()));
    d_func()->running = running;
    d_func()->finished = false;
    this->moveToThread(this);
}

AdoptedThread::~AdoptedThread()
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", QString("AdoptedThread::~AdoptedThread() of %1").arg(objectName()));
}

bool AdoptedThread::event(QEvent *event)
{
    // this makes sure that the java-created QThread is exited before deleted.
    if(event->type()==QEvent::DeferredDelete) {
        if(this->eventDispatcher() && this->isRunning()){
            this->exit(0);
            this->deleteLater();
            return false;
        }
    }
    return QThread::event(event);
}

void AdoptedThread::onStart()
{
    QThread::currentThread();
}

bool AdoptedThread::isAdopted() const{
    return d_func()->data->isAdopted;
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define MutexLocker QMutexLocker
#else
#define MutexLocker QMutexLocker<QMutex>
#endif

bool SelfDeletingThread::deleteLaterIfIsInFinish(){
    MutexLocker locker(&d_func()->mutex);
    if(d_func()->isInFinish){
        this->deleteLater();
        return true;
    }else{
        return false;
    }
}

ObjectDeleter::ObjectDeleter(QObject* object) : m_object(object){}

ObjectDeleter::~ObjectDeleter(){
    if(m_object){
        delete m_object.data();
    }
}

void QThreadUserData::deleteAtThreadEnd(QObject* object){
    if(m_finalActions){
        MutexLocker locker(m_mutex);
        Q_UNUSED(locker)
        ObjectDeleter* deleter = new ObjectDeleter(object);
        m_finalActions->append([deleter](){delete deleter;});
    }
}


void QThreadUserData::doAtThreadEnd(const std::function<void()>& action){
    if(m_finalActions){
        MutexLocker locker(m_mutex);
        Q_UNUSED(locker)
        m_finalActions->append(action);
    }
}

jobject qtjambi_get_qthread_for_jthread(JNIEnv *env, jobject thread){
    jobject associatedObject = Java::QtJambi::QtJambiInternal::findAssociation(env, thread);
#ifdef QT_QTJAMBI_PORT
    if(Java::QtCore::QThread$Wrapper::isInstanceOf(env, associatedObject)){
        QThread * qt_thread = static_cast<QThread *>(qtjambi_to_qobject(env, associatedObject));
        if(!qt_thread)
            qt_thread = new AdoptedThread(env, thread, false);
        return qtjambi_from_thread(env, qt_thread);
#else
    if(Java::QtCore::QThread::isInstanceOf(env, associatedObject)){
        return associatedObject;
#endif
    }
    return nullptr;
}

#ifndef QT_QTJAMBI_PORT

void qtjambi_initialize_thread(JNIEnv *env, QtJambiNativeID nativeId, jobject threadGroup)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData(env, threadGroup);
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    jobject jthread = Java::Runtime::Thread::currentThread(env);
    jobject contextClassLoader = Java::Runtime::Thread::getContextClassLoader(env, jthread);
    if(!contextClassLoader){
        contextClassLoader = Java::Runtime::Class::getClassLoader(env, Java::QtCore::QThread::getClass(env));
    }
    threadData->setContextClassLoader(env, contextClassLoader);
    threadData->setDaemon(Java::Runtime::Thread::isDaemon(env, jthread));
    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection());
    *connection = QObject::connect(thread, &QThread::started, [connection](){
        qtjambi_current_environment();
        QObject::disconnect(*connection);
    });
}

void qtjambi_thread_set_daemon(JNIEnv *, QtJambiNativeID nativeId, bool daemon)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData();
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    threadData->setDaemon(daemon);
}

bool qtjambi_thread_is_daemon(QThread* thread)
{
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return threadData->isDaemon();
    }
    return false;
}

void qtjambi_thread_set_name(JNIEnv *__jni_env, QtJambiNativeID nativeId, jstring name)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData();
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    QString _name = qtjambi_to_qstring(__jni_env, name);
    threadData->setName(_name.toUtf8());
}

jstring qtjambi_thread_get_name(JNIEnv *__jni_env, QThread* thread)
{
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return qtjambi_from_qstring(__jni_env, QLatin1String(threadData->getName()));
    }
    return nullptr;
}

jobject qtjambi_thread_get_group(JNIEnv *__jni_env, QThread* thread)
{
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getThreadGroup());
    }
    return nullptr;
}

void qtjambi_thread_set_UncaughtExceptionHandler(JNIEnv *__jni_env, QtJambiNativeID nativeId, jobject handler)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData();
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    threadData->setUncaughtExceptionHandler(__jni_env, handler);
}

jobject qtjambi_thread_get_UncaughtExceptionHandler(JNIEnv *__jni_env, QThread* thread)
{
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getUncaughtExceptionHandler());
    }
    return nullptr;
}

void qtjambi_thread_set_ContextClassLoader(JNIEnv *__jni_env, QtJambiNativeID nativeId, jobject cl)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData();
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    threadData->setContextClassLoader(__jni_env, cl);
}

jobject qtjambi_thread_get_ContextClassLoader(JNIEnv *__jni_env, QThread* thread)
{
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getContextClassLoader());
    }
    return nullptr;
}

#endif


