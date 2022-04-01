/****************************************************************************
**
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define MutexLocker QMutexLocker
#else
#define MutexLocker QMutexLocker<QMutex>
#endif

JNIEnv *qtjambi_current_environment(bool initializeJavaThread);

void qtjambi_initialize_main_thread(JNIEnv *__jni_env)
{
    //if(!QCoreApplicationPrivate::theMainThread.loadRelaxed())
    {
        QCoreApplicationPrivate::theMainThread = reinterpret_cast<QThread*>(1);
        QThread* currentQThread = QThread::currentThread();
        QCoreApplicationPrivate::theMainThread = currentQThread;
        qtjambi_cast<jobject>(__jni_env, currentQThread);
    }
    /*else{
        JavaException::raiseError(__jni_env, "There is already a main thread defined." QTJAMBI_STACKTRACEINFO );
        qtjambi_initialize_current_thread(__jni_env);
    }*/
}

void qtjambi_initialize_current_thread(JNIEnv *env)
{
    QThread* currentQThread = QThread::currentThread();
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(!QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentQThread)){
        locker.unlock();
        qtjambi_cast<jobject>(env, currentQThread);
        locker.relock();
    }
}

QThreadUserData::QThreadUserData(QThread* thread):
    QtJambiObjectData(),
    m_finalActions(nullptr),
    m_mutex(nullptr),
    m_threadGroup(),
    m_isDaemon(true),
    m_name(),
    m_uncaughtExceptionHandler(),
    m_threadType(NoMainThread)
{
    if(QCoreApplicationPrivate::theMainThread.loadRelaxed() == thread){
#if defined(Q_OS_LINUX)
        m_threadType = quintptr(gettid()) == quintptr(getpid()) ? ProcessMainThread : VirtualMainThread;
#elif defined(Q_OS_MACOS)
        m_threadType = pthread_main_np() ? ProcessMainThread : VirtualMainThread;
#elif defined(Q_OS_WIN)
        m_threadType = ProcessMainThread;
#endif
    }
    switch(m_threadType){
    case VirtualMainThread:
        m_finishedConnection = QObject::connect(thread, &QThread::finished, thread, [this]() {
            QCoreApplicationPrivate::theMainThread.storeRelaxed(nullptr);
            this->cleanup();
            QObject::disconnect(this->m_finishedConnection);
        }, Qt::DirectConnection);
        break;
    case NoMainThread:
        m_finishedConnection = QObject::connect(thread, &QThread::finished, thread, [this]() {
            this->cleanup();
            QObject::disconnect(this->m_finishedConnection);
        }, Qt::DirectConnection);
        break;
    default:
        break;
    }
}

QThreadUserData::QThreadUserData(JNIEnv *env, jobject threadGroup, QThread* thread):
    QThreadUserData(thread)
{
    m_threadGroup = JObjectWrapper(env, threadGroup);
}

QThreadUserData::~QThreadUserData(){
    switch(m_threadType){
    case ProcessMainThread:
        cleanup();
        break;
    default:
        if(m_finishedConnection){
            QObject::disconnect(m_finishedConnection);
            cleanup();
        }
        break;
    }
}

void QThreadUserData::cleanup(){
    QList<QPointer<QObject>> objectsForDeletion;
    {
        MutexLocker locker(m_mutex);
        Q_UNUSED(locker)
        objectsForDeletion.swap(m_objectsForDeletion);
    }
    for(const QPointer<QObject>& ptr : objectsForDeletion){
        if(ptr)
            delete ptr.data();
    }
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,QThreadUserData::)

void qtjambi_adopt_thread(JNIEnv *env, jobject java_thread, jobject java_qthread, QThread *qt_thread, bool created_by_java)
{
    Q_UNUSED(created_by_java)
    QString threadName = qtjambi_to_qstring(env, Java::Runtime::Thread::getName(env, java_thread));
    qt_thread->setObjectName(threadName);
    jobject javaQThread = java_qthread ? env->NewLocalRef(java_qthread) : nullptr;
    if(!javaQThread){
        javaQThread = qtjambi_cast<jobject>(env, qt_thread);
    }
    if(javaQThread){
        QThreadUserData* threadData;
        {
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, qt_thread);
        }
        if(threadData){
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
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, qt_thread, new QThreadUserData(qt_thread));
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
                if(!EventDispatcherCheck::storage.hasLocalData() || !EventDispatcherCheck::storage.localData()){
                    EventDispatcherCheck::storage.setLocalData(QSharedPointer<EventDispatcherCheck>(
                         new EventDispatcherCheck(
                             JObjectWrapper(env, java_thread),
                             qt_thread,
                             link,
                             [](QPointer<QThread>& thread, JObjectWrapper&& jthreadObjectWrapper, QWeakPointer<QtJambiLink>&& wlink, QList<std::function<void()>>&& finalActions){
                                  struct ObjectReleaser : public QtJambiObjectData {
                                     ObjectReleaser(JObjectWrapper&& _jthreadObjectWrapper,
                                                    QList<std::function<void()>>&& _finalActions,
                                                    QPointer<QThread>& _thread,
                                                    QWeakPointer<QtJambiLink>&& wlink)
                                         : QtJambiObjectData(),
                                           jthreadObjectWrapper(std::move(_jthreadObjectWrapper)),
                                           finalActions(std::move(_finalActions)),
                                           m_thread(),
                                           m_wlink(std::move(wlink)) {
                                         QTJAMBI_DEBUG_METHOD_PRINT("native", "ObjectReleaser::ObjectReleaser()")
                                         m_thread.swap(_thread);
                                     }
                                     ~ObjectReleaser() override {
                                         QTJAMBI_DEBUG_METHOD_PRINT("native", "ObjectReleaser::~ObjectReleaser()")
                                         if(JNIEnv* env = qtjambi_current_environment(m_thread.data()==QThread::currentThread())){
                                              QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                                              QSharedPointer<QtJambiLink> link;
                                              {
                                                   QReadLocker locker(QtJambiLinkUserData::lock());
                                                   link = m_wlink.toStrongRef();
                                              }
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

                                     QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)

                                     JObjectWrapper jthreadObjectWrapper;
                                     QList<std::function<void()>> finalActions;
                                     QPointer<QThread> m_thread;
                                     QWeakPointer<QtJambiLink> m_wlink;
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
                                       QTJAMBI_SET_OBJECTUSERDATA_ID(QTJAMBI_OBJECTUSERDATA_TYPE_ID(ObjectReleaser), eventDispatcher, new ObjectReleaser(std::move(jthreadObjectWrapper), std::move(finalActions), thread, std::move(wlink)));
                                  }else{
                                       ObjectReleaser releaser(std::move(jthreadObjectWrapper), std::move(finalActions), thread, std::move(wlink));
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
}

QThreadStorage<QSharedPointer<EventDispatcherCheck>> EventDispatcherCheck::storage;

EventDispatcherCheck::EventDispatcherCheck(JObjectWrapper&& jthreadObjectWrapper, QThread* thread, QWeakPointer<QtJambiLink>&& wlink, std::function<void(QPointer<QThread>&,JObjectWrapper&&,QWeakPointer<QtJambiLink>&&,QList<std::function<void()>>&&)>&& _cleaner)
    : m_jthreadObjectWrapper(std::move(jthreadObjectWrapper)),
      m_thread(thread),
      m_wlink(std::move(wlink)),
      cleaner(std::move(_cleaner)) {
    QTJAMBI_DEBUG_METHOD_PRINT("native", "EventDispatcherCheck::EventDispatcherCheck()")
    QThreadUserData* data;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread);
    }
    if(data){
        data->m_finalActions = &m_finalActions;
        data->m_mutex = &m_mutex;
    }
}

EventDispatcherCheck::~EventDispatcherCheck(){
    QTJAMBI_DEBUG_METHOD_PRINT("native", "EventDispatcherCheck::~EventDispatcherCheck()")
    QThreadUserData* data;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread);
    }
    if(data){
        data->m_finalActions = nullptr;
        data->m_mutex = nullptr;
    }
    try{
        if(!QCoreApplicationPrivate::is_app_closing && cleaner){
            cleaner(m_thread, std::move(m_jthreadObjectWrapper), std::move(m_wlink), std::move(m_finalActions));
            cleaner = {};
        }else{
            while(!m_finalActions.isEmpty()){
                m_finalActions.takeFirst()();
            }
            m_finalActions.clear();
            if(data && !data->purgeOnExit()){
                QWriteLocker locker(QtJambiLinkUserData::lock());
                QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_thread, nullptr);
                delete data;
            }
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

bool SelfDeletingThread::deleteLaterIfIsInFinish(){
    MutexLocker locker(&d_func()->mutex);
    if(d_func()->isInFinish){
        this->deleteLater();
        return true;
    }else{
        return false;
    }
}

void QThreadUserData::deleteAtThreadEnd(QObject* object){
    MutexLocker locker(m_mutex);
    Q_UNUSED(locker)
    m_objectsForDeletion.append(QPointer<QObject>(object));
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
    if(Java::QtCore::QThread::isInstanceOf(env, associatedObject)){
        return associatedObject;
    }
    return nullptr;
}

void qtjambi_initialize_thread(JNIEnv *env, jobject jthread, jobject threadGroup)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData(env, threadGroup, thread);
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    jobject current_jthread = Java::Runtime::Thread::currentThread(env);
    jobject contextClassLoader = Java::Runtime::Thread::getContextClassLoader(env, current_jthread);
    if(!contextClassLoader){
        contextClassLoader = Java::Runtime::Class::getClassLoader(env, Java::QtCore::QThread::getClass(env));
    }
    threadData->setContextClassLoader(env, contextClassLoader);
    threadData->setDaemon(Java::Runtime::Thread::isDaemon(env, current_jthread));
    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection());
    *connection = QObject::connect(thread, &QThread::started, [connection](){
        qtjambi_current_environment();
        QObject::disconnect(*connection);
    });
}

void qtjambi_thread_set_daemon(JNIEnv *__jni_env, jobject jthread, bool daemon)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(__jni_env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        if(!threadData){
            threadData = new QThreadUserData(thread);
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
        }
    }
    threadData->setDaemon(daemon);
}

bool qtjambi_thread_is_daemon(QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return threadData->isDaemon();
    }
    return false;
}

void qtjambi_thread_set_name(JNIEnv *__jni_env, jobject jthread, jstring name)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(__jni_env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        if(!threadData){
            threadData = new QThreadUserData(thread);
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
        }
    }
    QString _name = qtjambi_to_qstring(__jni_env, name);
    threadData->setName(_name.toUtf8());
}

jstring qtjambi_thread_get_name(JNIEnv *__jni_env, QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return qtjambi_from_qstring(__jni_env, QLatin1String(threadData->getName()));
    }
    return nullptr;
}

jobject qtjambi_thread_get_group(JNIEnv *__jni_env, QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getThreadGroup());
    }
    return nullptr;
}

void qtjambi_thread_set_UncaughtExceptionHandler(JNIEnv *__jni_env, jobject jthread, jobject handler)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(__jni_env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QWriteLocker locker(QtJambiLinkUserData::lock());
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData(thread);
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    threadData->setUncaughtExceptionHandler(__jni_env, handler);
}

jobject qtjambi_thread_get_UncaughtExceptionHandler(JNIEnv *__jni_env, QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getUncaughtExceptionHandler());
    }
    return nullptr;
}

void qtjambi_thread_set_ContextClassLoader(JNIEnv *__jni_env, jobject jthread, jobject cl)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(__jni_env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QWriteLocker locker(QtJambiLinkUserData::lock());
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData(thread);
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    threadData->setContextClassLoader(__jni_env, cl);
}

jobject qtjambi_thread_get_ContextClassLoader(JNIEnv *__jni_env, QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getContextClassLoader());
    }
    return nullptr;
}


